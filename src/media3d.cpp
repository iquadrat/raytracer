/* *************************************************************************
                          media3d.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002 
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: media3d.cpp,v 1.21 2003/07/16 12:30:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include <iostream>

#include "../src/lightlist.h"
#include "../src/random.h"
#include "../src/scene.h"
using std::cout;

HitStatistics Media3D::evaluations("Media","Evaluations","Samples");

Colour Media3D::simps(const Colour& c1, const Colour& c2, const Colour& c3) {

  // Simpson: c1 + 4*c2 + c3
  Colour ret = c2.scaled(4);
  ret.add(c1); ret.add(c3);
  return ret;

}

Colour Media3D::simpsExp(const Colour& c1, const Colour& c2, const Colour& c3,
                          const Colour& a1, const Colour& a2, const Colour& a3,
                          DBL h) {

    // "Exp-Simpson": 
    return (( c1*(a1*12-a2*8+a3*2+a1*a1*(4*h)) + c2*a1*(-16) + c3*4*a1 ) * h + c1 + c2*4 + c3 ) * (h/3);
    
}

Colour Media3D::sampleRecursive(const CameraRay& ray, DBL l, DBL m, DBL r,
  const Colour& ab_l, const Colour& ab_m, const Colour& ab_r,
  Colour& absorb, const DBL il) {

    DBL h = (r-l)/4;
    Colour absorb_exp = Colour::exp(absorb);
    CLR absorb_exp_norm = absorb_exp.norm();
    
    // evaluate new point between l and m (absorption only)
    DBL ml = (l+m)/2;
    Vector3 p_ml = ray.positionAtTime(ml);
    Colour ab_ml = evaluateAbsorb(p_ml);

    // evaluate new point between m and r (absorption only)
    DBL mr = (r+m)/2;
    Vector3 p_mr = ray.positionAtTime(mr);
    Colour ab_mr = evaluateAbsorb(p_mr);

    // calculate absorption integrals for both halfs
    Colour ab_im = simps(ab_l,ab_ml,ab_m).scaled(-h/3);
    Colour ab_ir = simps(ab_m,ab_mr,ab_r).scaled(-h/3);
    Colour ab_tot = Colour::add(ab_im,ab_ir);

    if (absorb_exp_norm < 1e-10) {
        absorb.add(ab_tot);
        return Colour(0);        
    }
    
    if (ab_tot.norm()*absorb_exp_norm<0.3) {

        // tolerated error can be increased by the apsorption factor
        CLR awderr = coltolerance/absorb_exp_norm;
        FLT leval,lmin = 0.99*h;
        
        leval = il; // evaluate emitting at l
        Colour em_l = evaluateEmit(ray.positionAtTime(l),ray,awderr,leval);
        if (leval < lmin) goto rec; // interval too big
        
        leval = il; // evaluate emitting at m
        Colour em_m = evaluateEmit(ray.positionAtTime(m),ray,awderr,leval);
        if (leval < lmin) goto rec;
                
        leval = il; // evaluate emitting at r
        Colour em_r = evaluateEmit(ray.positionAtTime(r),ray,awderr,leval);
        if (leval < lmin) goto rec;

        leval = il; // evaluate emitting at ml
        Colour em_ml = evaluateEmit(p_ml,ray,awderr,leval);
        if (leval < lmin) goto rec;
                
        leval = il; // evaluate emitting at mr
        Colour em_mr = evaluateEmit(p_mr,ray,awderr,leval);
        if (leval < lmin) goto rec;
        
        ab_im.exp();

        // approximate the integral with the points l,m,r
        Colour i1 = simpsExp(em_l,em_m,em_r,ab_l,ab_m,ab_r,2*h);

        // do a more precise approximation using all 5 points
        Colour i2 = simpsExp(em_l,em_ml,em_m,ab_l,ab_ml,ab_m,h) + ab_im*simpsExp(em_m,em_mr,em_r,ab_m,ab_mr,ab_r,h);

        // estimate error

        // do Romberg step
        Colour i3 = i2.scaled(16); i3.sub(i1); i3.scale(1.0/15);

/*        // no Romberg step
        Colour i3 = i2;
        Colour i2 = i1;
*/
        DBL err = Colour::sub(i3,i2).scaled( absorb_exp ).norm();
        
        if (err<coltolerance) {
            absorb.add(ab_tot);
            return i3.scaled(absorb_exp);
        }

    } else {

         if (absorb_exp_norm*20*h<coltolerance) {

//             if (DEBUG) cout << "total extinction level reached, abort\n";
             absorb.add(ab_tot);
             return Colour(0);

        }

    }


rec:
    // do recursion
    Colour left = sampleRecursive(ray, l,ml,m, ab_l,ab_m,ab_r, absorb, il);
    Colour right = sampleRecursive(ray, m,mr,r, ab_m,ab_mr,ab_r, absorb, il);
    return Colour::add(left,right);

}


Colour Media3D::sample(DBL start, DBL end, const CameraRay& r, Colour& totalabsorb) {

    evaluations.eval();

    switch(method) {

    // LINEAR SAMPLING    
    case LINEAR: {

        // init:
        DBL time = start; // holds time how far we've integrated yet
        DBL ilength = end-start;
        DBL tol = coltolerance;

        Colour ret = Colour(0);
        totalabsorb = Colour(0);
        Colour tab_exp = Colour(1);

        // evaluate at interval start
        // ab0, em0 are values at interval start
        // length is length of current interval
        Vector3 p = r.positionAtTime(time);
        Colour ab0 = evaluateAbsorb(p);
        FLT length = ilength;
        Colour em0 = evaluateEmit(p,r,tol,length);

        while( end-time>INTERSECTION_TIME_EPSILON ) {

            // approximation to exp function becomes too
            // bad for big absorption in one interval
            if (length*tab_exp.norm()*ab0.norm() > 0.1) {
                length = 0.0999/(ab0.norm()*tab_exp.norm());
            }

            // interval cannot go past the end
            if (time+length>end) length = end-time;

            // forward evaulating
            Vector3 pf = r.positionAtTime(time+length);
            FLT length_next = ilength;
            Colour em1 = evaluateEmit(pf,r,tol,length_next);
            Colour ab1(0);
            
            if (length_next < length) {            

                // evaluate at interval end
                length = length_next;
                Vector3 p = r.positionAtTime(time+length);
                ab1 = evaluateAbsorb(p);
                length_next = ilength;
                em1 = evaluateEmit(p,r,tol,length_next);

            } else {
                
                ab1 = evaluateAbsorb(pf);

            }

            // apply jitter
            length *= Random::global.jitterFactor(jitter);
            
            // interval cannot go past the end
            if (time+length>end) length = end-time;            

            if (em0.norm() != 0 || em1.norm() !=0) {

                if (ab0.norm()*length <0.01) {

                    ret.add( tab_exp*(em0+em1)/2*length );

                } else {

                    ret.add( tab_exp*(
                          (em0+em1)/2*length -
                          (em1*ab1*(1.0/8) + em0*ab1*(1.0/24) + em1*ab0*(5.0/24) + em0*ab0*(1.0/8))*sqr(length)
                          )
                    );

                }

            }

            totalabsorb.sub( ab0*length );
            tab_exp = Colour::exp(totalabsorb);

            CLR tab_exp_norm = tab_exp.norm();
            if (tab_exp_norm < 1e-10) break;
            
            time = time + length;
            tol = coltolerance / tab_exp_norm;

            ab0 = ab1;
            em0 = em1;
            length = length_next;

        }

        return ret;
    }
    break;

    // RECURSIVE SAMPLING
    case RECURSIVE: {

        if (end-start < 4*INTERSECTION_TIME_EPSILON) return Colour(0);

        Vector3 p_l = r.positionAtTime(start);
        DBL m = start + (end-start)/2*Random::global.jitterFactor(jitter);
        Vector3 p_m = r.positionAtTime(m);
        Vector3 p_r = r.positionAtTime(end);

        Colour ab_l = evaluateAbsorb(p_l);
        Colour ab_m = evaluateAbsorb(p_m);
        Colour ab_r = evaluateAbsorb(p_r);

        Colour ret = sampleRecursive(r,start,m,end, ab_l,ab_m,ab_r,totalabsorb,end-start);
    //    cout << "I:["<<start<<","<<end<<"] --> "<<ret.norm()<<"\n";
        return ret;
    }
    break;

    default:
        assert(false);
        return Colour(0);

    }


}

void Media3D::sampleRecursive(const LightRay& ray, DBL l, DBL m, DBL r,
  const Colour& ab_l, const Colour& ab_m, const Colour& ab_r, Colour& absorb) {

//    cout << "["<<l<<","<<r<<"]";

    DBL h = (r-l)/4;
    DBL ml = l+h;
    DBL mr = r-h;

    Colour ab_ml = evaluateAbsorb( ray.positionAtTime(ml) );
    Colour ab_mr = evaluateAbsorb( ray.positionAtTime(mr) );

    // approximate the integral with the points l,m,r
    Colour i1  = simps(ab_l,ab_m,ab_r).scaled(h/1.5);

    // do a more precise approximation using all 5 points
    Colour i2 = Colour::add( simps(ab_l,ab_ml,ab_m), simps(ab_m,ab_mr,ab_r) ).scaled(h/3);

    // do Romberg step
    Colour i3 = i2.scaled(16); i3.sub(i1); i3.scale(1.0/15);

    // estimate error
    DBL err = Colour::sub(i3,i2).norm();

//    cout << " i1: "<<i1.norm()<< " i2: "<<i2.norm() << " i3: "<<i3.norm() << "err: " << err << "\n";

    if (err<coltolerance) {
        absorb.add(i3);
        return;
    }

//    cout << "rec\n";
    // do recursion
    sampleRecursive(ray, l,ml,m, ab_l,ab_m,ab_r, absorb);
    sampleRecursive(ray, m,mr,r, ab_m,ab_mr,ab_r, absorb);

}


void Media3D::sample(DBL start, DBL end, const LightRay& r, Colour& totalabsorb) {

    DBL m = start + (end-start)/2*Random::global.jitterFactor(jitter);
    Colour ab_l = evaluateAbsorb( r.positionAtTime(start) );
    Colour ab_m = evaluateAbsorb( r.positionAtTime(m) );
    Colour ab_r = evaluateAbsorb( r.positionAtTime(end) );

    sampleRecursive(r, start,m,end, ab_l,ab_m,ab_r, totalabsorb);

}



Media3DSingle::Media3DSingle(
  const Colour& emitting, const Colour& absorbing,
  const Colour& scattering, scattertype t, const Scene* s,
  DBL jitter, CLR tol, FLT variancekeep, samplingmethod m):
  Media3D(jitter,tol, variancekeep, m), emit(emitting), absorb(absorbing), scatter(scattering),
  density(0), scene(s), stype(t) {


}

Media3DSingle::~Media3DSingle() {
    delete(density);
}

Media3DSingle::Media3DSingle(const Media3DSingle& m): Media3D(m), emit(m.emit), absorb(m.absorb), scatter(m.scatter),
  density(0), scene(m.scene), stype(m.stype) {

    if (m.density) {
        density = m.density->copy();
    }

}

Colour Media3DSingle::evaluateEmit(const Vector3& p, const CameraRay& r, CLR allowederror, FLT& length) const {

    evaluations.evalSuccess();

    // calculate emitting contribution:
    Colour ret = emit;

    // calculate scattering contribution:
    if (stype != NONE) {

        CLR scattertypeerrorfactor;
        switch(stype) {
                    // these values are quite arbitrary
                    case RAYLEIGH:
                        scattertypeerrorfactor = 0.71/1.2; break;
                    case MIE_HAZY:
                        scattertypeerrorfactor = 0.71/3.5; break;
                    case MIE_MURKY:
                        scattertypeerrorfactor = 0.71/1.5; break;

                    /*
                        case HENYEY_GREENSTEIN:
                        scatertypeerrorfactor =  ; break;
                    */

                    default:
                        assert(false);

        }

        // get list with all relevant LightSources
        LightList* scatterlist = scene->lightcache.lookup(p, allowederror*scattertypeerrorfactor/scatter.norm(), length, varkeep);

        // loop over all LightSources and get the amouth of light comming here
        LightList* it = scatterlist;
        while(it != 0 && it->light!=0) {    

            LightRay l = it->light->shineTo(p, 0);

            FLT factor;
            if (stype != ISOTROPIC) {
                DBL angle = Vector3::dotProd(r.getDirection(),l.getDirection())/r.getDirection().length()/l.getDirection().length();
                switch(stype) {
                    // taken from POV-Ray 3.5, media.cpp
                    case RAYLEIGH:
                        factor = 0.799372013f * (1.0f + pow(angle,2));
                        break;
                    case MIE_HAZY:
                        factor = 0.576655375f * (1.0f + 9.0f * pow(0.5 * (1.0 + angle), 8.0));
                        break;
                    case MIE_MURKY:
                        factor = 0.495714547f * (1.0f + 50.0f * pow(0.5 * (1.0 + angle), 32.0));
                        break;
                    /*
                        case HENYEY_GREENSTEIN:
                            factor *=
                    */

                    default:
                        assert(false);
                }
                if (isnan(factor)) factor = 0; // *check me* when does this happen?
            } else factor =1;

            ret.add(it->colour.scaled(factor).scaled(scatter));
            it = it->next;

        }

        delete(scatterlist);

    }

   if (density) {
        Vector3 loc = p;
        density->transformPointIn(loc);
        Colour densityfactor = density->evaluateAt(loc);
        ret.scale(densityfactor);
    }

    return ret;

}

Colour Media3DSingle::evaluateAbsorb(const Vector3& p) const {

    if (density) {
        Vector3 loc = p;
        density->transformPointIn(loc);
        Colour densityfactor = density->evaluateAt(loc);
        return absorb.scaled(densityfactor);

    }

    return absorb;

}


Media3DMultiple::Media3DMultiple(DBL jitteramouth, CLR tol, FLT variancekeep,
  samplingmethod m): Media3D(jitteramouth,tol,variancekeep,m), list() {};

Media3DMultiple::Media3DMultiple(const Media3D* m): Media3D(*m), list() {
    list.push_back(m);
}

Media3DMultiple::~Media3DMultiple() {
    std::vector<const Media3D*>::const_iterator it = list.begin();
    for(; it!=list.end(); ++it) {
        delete(*it);
    }
}

Media3DMultiple::Media3DMultiple(const Media3DMultiple& m): Media3D(m), list() {

    std::vector<const Media3D*>::const_iterator it = m.list.begin();
    for(; it!=m.list.end(); ++it) {
        list.push_back( (*it)->copy() );
    }

}

Colour Media3DMultiple::evaluateEmit(const Vector3& p, const CameraRay& r, CLR maxerror, FLT& ilength) const {

    Colour ret(0);
    std::vector<const Media3D*>::const_iterator it = list.begin();
    for(; it!=list.end(); ++it) {
        ret.add( (*it)->evaluateEmit(p,r,maxerror,ilength) );
    }
    return ret;

}

Colour Media3DMultiple::evaluateAbsorb(const Vector3& p) const {

    Colour ret(0);
    std::vector<const Media3D*>::const_iterator it = list.begin();
    for(; it!=list.end(); ++it) {
        ret.add( (*it)->evaluateAbsorb(p) );
    }
    return ret;

}
  
