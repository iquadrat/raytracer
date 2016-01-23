/* *************************************************************************
                          isosurface.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/isosurface.h"

#include "../src/objectconstructionerror.h"

const DBL Isosurface::NORMAL_E = 1e-5;
const DBL Isosurface::K_INCREASE_FACT = 1.1;

HitStatistics Isosurface::stat("Isosurface","Tests","Hits");
int Isosurface::isosurfacecount = 0;

// tmpryl
#include <iostream>
using std::cout;

#include<string>
#include<sstream>

// Move this somewhere else:
template<typename Source>
std::string to_string(Source arg)
{
   std::ostringstream os;
   os << arg;
   return os.str();
};       


Isosurface::Isosurface(IsosurfaceFunction* func, Object3D* cont, DBL maxd, bool doeval):
    container(cont), f(func), K(maxd), missstepsize(0), eval(doeval), maxdstat("Isosurface "+to_string<int>(isosurfacecount)+" max D") {

    boundingbox = new BoundingBox(cont->getBoundingMin(),cont->getBoundingMax());

}


Isosurface::~Isosurface(){
    
    delete(container);
    delete(f);
    
}


Object3D* Isosurface::duplicate() const {

    throw(ObjectConstructionError("Duplication / copy of insosurface not implemented yet"));    
    // *implement me*
    return 0;
    
}


bool Isosurface::isInsideAt(const Vector3& locallocation) const {
    
    return container->isInsideAtBounded(locallocation) && ( (f->evaluateAt(locallocation)<0)^inverse );
    
}


Intersection* Isosurface::intersectLocalLimitedTime(const Ray& r, DBL maxtime) const {

    IsosurfaceIntersection* i = new IsosurfaceIntersection(this,r,maxtime);
    if (i->nointersection) {
        delete(i);
        stat.evalFailed();
        return 0;
    } else {
        stat.evalSuccess();
        return i;
   }

}


Isosurface::IsosurfaceIntersection::IsosurfaceIntersection(const Isosurface* object,
  const Ray& r, DBL tmax): Intersection(object,r) {

    iso = object;
    nointersection = false;
    containerhit = false;
    

/*
    if (iso->tests > 100000) {

        if (iso->missstepsize<5*INTERSECTION_TIME_EPSILON) {
            //iso->K/=iso->fact;
            cout << "K now "<<iso->K<<"\n";
            iso->missstepsize = 0;
        } else {
            //iso->fact = .05 + 0.95*iso->fact;
            //iso->missstepsize*=0.95;
            //cout << "fact now"<<iso->fact<<"\n";
            //iso->missstepsize = 0;                
        }

    iso->tests = 0;

    }
*/

    // intersect ray with container
    // addme: transformed containers

/*    Ray rtrans(r);
    iso->container->transformRayIn(rtrans);*/

    i_container = iso->container->intersect(r/*trans*/);
    if (!i_container) {
        nointersection = true;
        return;
    }

    DBL t_container_min;
    if (iso->container->isInsideAtBounded(r.getOrigin()) == true) {
        // origin is inside container => timeinterval [epsilon, hittime]
        t_container_min = INTERSECTION_TIME_EPSILON;
        t_container_max = i_container->currentTime();
        if (t_container_max >tmax) maxtime = tmax; else maxtime = t_container_max;
        last = StackElement(t_container_min,iso->f->evaluateAt(r.positionAtTime(t_container_min)));
    } else {
        // origin outside container => 2 intersections => timeinterval [firsthit,secondhit]
        t_container_min = i_container->currentTime();
        last = StackElement(t_container_min,iso->f->evaluateAt(r.positionAtTime(t_container_min)));
        // check if first container hit is inside
        if (last.value <= 0) {
            root = t_container_min;
            containerhit = true;
            return;
        }
        i_container->next();
        t_container_max = i_container->currentTime();
        if (t_container_max > tmax) maxtime = tmax; else maxtime = t_container_max;
    }

    // we know know that we have to look for root in the interval [t_container_min,maxtime]
    //cout << "timeinterval ["<<t_container_min<<","<<maxtime<<"]\n";

    // adjust max curvature
    klocal = iso->K * r.getDirection().lengthSq();

    // init time evaluation
    prelast = StackElement( t_container_min-INTERSECTION_TIME_EPSILON,
      iso->f->evaluateAt(  ray.positionAtTime( t_container_min-INTERSECTION_TIME_EPSILON)) );

    // start calculation
    findRoot();

}

Isosurface::IsosurfaceIntersection::~IsosurfaceIntersection() {
    delete(i_container);
}

void Isosurface::IsosurfaceIntersection::next() {

    if (nointersection == false) {
        prelast = StackElement(root,0);
        last = StackElement(root+INTERSECTION_TIME_EPSILON, iso->f->evaluateAt(ray.positionAtTime(root+INTERSECTION_TIME_EPSILON)) );
        findRoot();
    }

}

DBL Isosurface::IsosurfaceIntersection::currentTime() {
    
    if (nointersection)
        return INTERSECTION_INFINIT_TIME;
    else
        return root;
        
}

Vector3 Isosurface::IsosurfaceIntersection::normal() {

    if (containerhit) {
        
        return i_container->normal();
        
    } else {
        
        // we could calculate the normal much preciser
        // by using a few more evaluations
        Vector3 location = ray.positionAtTime(root);
        DBL f0 = iso->f->evaluateAt(location);
        DBL dx = iso->f->evaluateAt(Vector3::add(location,Vector3(NORMAL_E,0,0))) - f0;
        DBL dy = iso->f->evaluateAt(Vector3::add(location,Vector3(0,NORMAL_E,0))) - f0;
        DBL dz = iso->f->evaluateAt(Vector3::add(location,Vector3(0,0,NORMAL_E))) - f0;

        return Vector3(dx,dy,dz);
        
    }

}


void Isosurface::IsosurfaceIntersection::findExactRoot() {

    // uses a hybrid method (secant method combined with bisection)
    // to find the root to time epsilon precision
    StackElement min = prelast;
    StackElement max = last;
    StackElement k = min;
    StackElement km1 = max;
    do {
        // do one secant step
        DBL tneu = (km1.time*k.value- k.time*km1.value)/(k.value-km1.value);
        DBL fneu = iso->f->evaluateAt(ray.positionAtTime(tneu));
        StackElement snew;
        if (tneu <min.time || tneu>max.time) {
            // we accidentially left the interval, do one bisection step instead
            snew = StackElement( (max.time+min.time)/2,iso->f->evaluateAt(ray.positionAtTime( (max.time+min.time)/2)) );
        } else {
            // secant is ok
            snew = StackElement(tneu,fneu);
        }

        if ( (snew.value>0) == (min.value>0) ) {
            min = snew;
        } else {
            max = snew;
        }
        km1 = k;
        k = snew;

    } while( fabs(k.time-km1.time) > INTERSECTION_TIME_EPSILON && k.value!=0);
    root = k.time;

}

DBL Isosurface::IsosurfaceIntersection::calcPolyRoot() {

    // Fit a 2nd-order polynom through the points last and prelast
    // which has maximal second derivative klocal and find the next root
    // of this polynom
    
    DBL t1 = prelast.time;
    DBL f1 = fabs(prelast.value);
    DBL t2 = last.time;
    DBL f2 = fabs(last.value);
    DBL a = klocal;
    DBL b= a*(t1+t2)+(f1-f2)/(t1-t2);
    DBL c = f1 - b*t1+a*t1*t1;
    DBL tmp = b/2/a;
    return tmp + sqrt(tmp*tmp+c/a);
    
}


void Isosurface::IsosurfaceIntersection::findRoot() {
    
    int debug = 0;
    if (debug) cout << "\nfindroot [ ("<<last.time<<","<<last.value<<") ,"<<maxtime<<"]\n";
    DBL xneu;
    do {

        if (debug) cout <<"prelast: ("<<prelast.time<<","<<prelast.value<<") / last: ("<<last.time<<","<<last.value<<") \n";

        xneu = calcPolyRoot();

        if (xneu-last.time>MAX_STEP_INCREASE*(last.time-prelast.time)) {

            xneu = last.time+MAX_STEP_INCREASE*(last.time-prelast.time);
            
        }
        
        if (xneu>maxtime) {
            nointersection = true;
            return;
        }
        
        prelast = last;
        last = StackElement(xneu, iso->f->evaluateAt(ray.positionAtTime(xneu)) );
        if (debug) cout << "new: ("<<last.time<<","<<last.value<<") // "<<last.time-prelast.time<<"\n";

        if ( (last.time-prelast.time<INTERSECTION_TIME_EPSILON) || (last.time<prelast.time)
              || (fabs(last.value<1e-15) && fabs(last.value-prelast.value) <1e-10) ) {
            
            root = last.time;
            return;

        }

        if ( (prelast.value>0) != (last.value>0) ) {

            if (iso->eval) {
                DBL stepsize = last.time-prelast.time;
                if ( stepsize > iso->missstepsize) {

                    if (stepsize>2*MAX_STEP_INCREASE*INTERSECTION_TIME_EPSILON) {
                        iso->K*=iso->K_INCREASE_FACT;
                        iso->maxdstat.update(iso->K);
                        //cout << "K:" << iso->K;
                        //iso->missstepsize = stepsize;
                        //if (stepsize>2*MAX_STEP_INCREASE*INTERSECTION_TIME_EPSILON) {
                            //cout << "missstepsize" << iso->missstepsize << "last: "<<lstepsize<<"\n";
                            //iso->fact = 0.05+0.95*iso->fact;
                        //}
                        iso->missstepsize = 0;
                    } else {
                        iso->missstepsize = stepsize;
                    }
                }
            }
            
            findExactRoot();
            return;

        }

        
    } while(true);

} 
