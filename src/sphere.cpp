/* *************************************************************************
                            sphere.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: sphere.cpp,v 1.26 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/sphere.h"

#include "../src/patchobject.h"
#include "../src/trianglepatch.h"


HitStatistics Sphere::stat("Sphere","Tests","Hits");


void Sphere::rotateRad(const Vector3& v) {

    // if there is no transfomraiton matrix yet, we can just rotate the
    // sphere's midpoint
    if (getTransformationMatrix()==0) {

        Vector3 trans = getTranslation();        
        trans.rotate(v);
        setTranslation(trans);
        
    } else {

        Object3D::rotateRad(v);
        calculateBoundingBox();
        
    }

}


void Sphere::rotateDeg(const Vector3& v) {
    
    rotateRad(v.scaled(PI/180.0));
    
}


void Sphere::scale(const Vector3& v) {

    // if there is no transformation matrix yet and the scaling is uniform
    // then just scale the radius and the sphere's midpoint
    if ( (getTransformationMatrix()==0) && (v[0]==v[1] && v[0]==v[2]) ) {

        // uniform scaling
        radius *= v[0];
#ifdef STORE_RADIUS_SQR        
        radiussq = radius * radius;
#endif        
        Vector3 trans = getTranslation();
        trans.scale(v[0]);
        setTranslation(trans);

    } else {
    
        Object3D::scale(v);
        calculateBoundingBox();

    }
    
}


Intersection* Sphere::intersectLocal(const Ray &r) const {

    stat.eval();

    // *optimize me*
    DBL a=r.getDirection().lengthSq();
    DBL b=Vector3::dotProd(r.getOrigin(),r.getDirection());
#ifdef SPHERE_STORE_RSQ    
    DBL c=r.getOrigin().lengthSq()-radiussq;
#else
    DBL c=r.getOrigin().lengthSq()-radius*radius;
#endif    
    DBL det=b*b-a*c;

    if (det<0) return 0;

    det = sqrt(det);
    DBL t1=-b-det;
    if (t1>INTERSECTION_TIME_EPSILON) {
        
        stat.success();    
        return new SphereIntersection(this, r,t1/a, (-b+det)/a );
        
    } else {
        DBL t2=-b+det;
        if (t2>INTERSECTION_TIME_EPSILON) {
            
            stat.success();
            return new SphereIntersection(this, r, t2/a, INTERSECTION_INFINIT_TIME);
            
        }
    }

    return 0;

}

Intersection* Sphere::intersectLocalLimitedTime(const Ray &r, DBL tmax) const {

    stat.eval();

    // *optimize me*
    DBL a=r.getDirection().lengthSq();
    DBL b=Vector3::dotProd(r.getOrigin(),r.getDirection());
#ifdef SPHERE_STORE_RSQ    
    DBL c=r.getOrigin().lengthSq()-radiussq;
#else
    DBL c=r.getOrigin().lengthSq()-radius*radius;
#endif            
    DBL det=b*b-a*c;

    if (det<0) return 0;

    det = sqrt(det);
    DBL t1 = (-b-det)/a;
    if (t1>INTERSECTION_TIME_EPSILON) {

        if (t1<tmax) {
            DBL t2 = (-b+det)/a;
            stat.success();            
            if (t2>tmax) 
                return new SphereIntersection(this, r,t1,INTERSECTION_INFINIT_TIME);
            else 
                return new SphereIntersection(this, r,t1,t2);
            
        }
        
    } else {

        DBL t2 = (-b+det)/a;
        if ((t2>1e-10)&&(t2<tmax)) {
            stat.success();    
            return new SphereIntersection(this, r,t2,INTERSECTION_INFINIT_TIME);
        }
        
    }         
    return 0;

}

bool Sphere::isInsideAt(const Vector3 &locallocation) const {
#ifdef SPHERE_STORE_RSQ    
    return (locallocation.lengthSq()<radiussq)^inverse;
#else
    return (locallocation.lengthSq()<radius*radius)^inverse;
#endif    
}


void Sphere::calculateBoundingBox() {
    
    if (boundingbox) delete(boundingbox);
    boundingbox = new BoundingBox(Vector3(-radius,-radius,-radius),Vector3(radius,radius,radius));
    boundingbox->transformate(*this);
    
}


bool Sphere::hasFinitBounding() const {
    
    return true;
    
}


Vector3 Sphere::getBoundingMin() const {
    
    if (boundingbox) {
        return Object3D::getBoundingMin();
    } else {
        return Vector3::sub(getTranslation(),Vector3(radius,radius,radius));
    }
    
}

Vector3 Sphere::getBoundingMax() const {
    if (boundingbox) {
        return Object3D::getBoundingMax();
    } else {
        return Vector3::add(getTranslation(),Vector3(radius,radius,radius));
    }
}

Vector3 Sphere::point(double a, double b) const {

    Vector3 ret(0,radius,0);
    ret.rotate(Vector3(a,0,0));
    ret.rotate(Vector3(0,b,0));
    transformPointOut(ret);
    return ret;
    
}

PatchObject* Sphere::toPatch(double precision) const {

//    PatchObject* ret = new PatchObject();
    SharedPointsPatchObject::PatchList list;
    
    double maxstep = acos(1.0-precision/radius);
    double astep = PI;
    for(int i=2; maxstep<astep; astep = PI/(i++) );
    double bstep = astep;

    std::cout << "stepsize: "<< astep<<std::endl;
       
    for(double a=0; a<(PI-astep/2); a+=astep) {

        for(double b=0; b<(2*PI-astep/2); b+=bstep) {

//            std::cout << point(a,b) <<std::endl;
        
          list.addTriangle( point(a,b), point(a+astep,b), point(a+astep,b+bstep) );
          list.addTriangle( point(a,b), point(a,b+bstep), point(a+astep,b+bstep) );
/*          ret->addPatch(
          new TrianglePatch( point(a,b), point(a+astep,b), point(a+astep,b+bstep) ) );
          ret->addPatch(new TrianglePatch( point(a,b), point(a,b+bstep), point(a+astep,b+bstep) ) );
*/
        
        }
    }

    PatchObject* ret = new SharedPointsPatchObject(list);
    ret->close();

    // *add me*: copy texture, etc to patchobject
//     ret->copyAttributes(*this);
    
//    ret->transform(*this);
    return ret;    

}

void Sphere::SphereIntersection::next() {
    
    if (currenttime != INTERSECTION_INFINIT_TIME) {
        currenttime = secondtime;
        secondtime = INTERSECTION_INFINIT_TIME;
    }
    
}

Vector3 Sphere::SphereIntersection::normal() {
    return ray.positionAtTime(currenttime);
}


