/* *************************************************************************
                          box.cpp  -  description
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
 
#include "../src/box.h"

#include "../src/rendererror.h"

HitStatistics Box::stat("Box","Tests","Hits");


void Box::rotateRad(const Vector3& v) {

    Object3D::rotateRad(v);
    calculateBoundingBox();

}


void Box::rotateDeg(const Vector3& v) {

    Object3D::rotateDeg(v);
    calculateBoundingBox();
    
}


void Box::scale(const Vector3& v) {

    if (getTransformationMatrix()==0) {

        // box remains axis-aligned
        boxmin.scale(v);
        boxmax.scale(v);        

    } else {

        Object3D::scale(v);

    }

}


void Box::translate(const Vector3& v) {

    if (getTransformationMatrix()==0) {
        
        boxmin.add(v);
        boxmax.add(v);
        
    } else {

        Object3D::translate(v);

    }
    
}    


Intersection* Box::intersectLocalLimitedTime(const Ray &r, DBL maxtime) const {

    stat.eval();

    DBL tmin, tmax = maxtime;
    boxside smin = X, smax = X;

    Vector3 direction = r.getDirection();
    Vector3 origin = r.getOrigin();

    // calculate x time interval
    if (direction[0]>0) {

        DBL t = (-origin[0]+boxmax[0]) / direction[0];
        if (t<INTERSECTION_TIME_EPSILON) return 0;  if (t<tmax) tmax = t;
        tmin = (-origin[0]+boxmin[0]) / direction[0];
        if (tmin<INTERSECTION_TIME_EPSILON) tmin = INTERSECTION_TIME_EPSILON;
        else if (tmin>tmax) return 0;

    } else if (direction[0]!=0){

        DBL t = (-origin[0]+boxmin[0]) / direction[0];
        if (t<INTERSECTION_TIME_EPSILON) return 0;  if (t<tmax) tmax = t;
        tmin = (-origin[0]+boxmax[0]) / direction[0];
        if (tmin<INTERSECTION_TIME_EPSILON) tmin = INTERSECTION_TIME_EPSILON;
        else if (tmin>tmax) return 0;

    } else {

        if (origin[0]<boxmin[0] || origin[0]>boxmax[0] ) return 0;
        tmin = INTERSECTION_TIME_EPSILON;

    }

    // calculate y time interval
    if (direction[1]>0) {

        DBL t = (-origin[1]+boxmax[1]) / direction[1]; //y max
        if (t<tmin) return 0;  if (t<tmax) {tmax = t; smax = Y;}
        t = (-origin[1]+boxmin[1]) / direction[1]; //y min
        if (t>tmax) return 0;  if (t>tmin) {tmin = t; smin = Y;}

    } else if (direction[1]!=0) {

        DBL t = (-origin[1]+boxmin[1]) / direction[1]; //y max
        if (t<tmin) return 0;  if (t<tmax) {tmax = t; smax = Y;}
        t = (-origin[1]+boxmax[1]) / direction[1]; //y min
        if (t>tmax) return 0;  if (t>tmin) {tmin = t; smin = Y;}

    } else {
        
        if (origin[1]<boxmin[1] || origin[1]>boxmax[1] ) return 0;
        
    }

    // calculate z time interval
    if (direction[2]>0) {

        DBL t = (-origin[2]+boxmax[2]) / direction[2];  // z max
        if (t<tmin) return 0;  if (t<tmax) {tmax = t; smax = Z;}
        t = (-origin[2]+boxmin[2]) / direction[2];  // z min
        if (t>tmax) return 0;  if (t>tmin) {tmin = t; smin = Z;}

    } else if(direction[2]!=0) {

        DBL t = (-origin[2]+boxmin[2]) / direction[2]; // z max
        if (t<tmin) return 0;  if (t<tmax) {tmax = t; smax = Z;}
        t = (-origin[2]+boxmax[2]) / direction[2]; // z min
        if (t>tmax) return 0;  if (t>tmin) {tmin = t; smin = Z;}

    } else {
        
        if (origin[2]>boxmin[2] || origin[2]<boxmax[2] ) return 0;
        
    }
    
    if (tmin == INTERSECTION_TIME_EPSILON) {
        if (tmax == maxtime) {
            return 0;
        } else {
            stat.success();
            return new BoxIntersection(this,r,tmax,INTERSECTION_INFINIT_TIME,smax,NONE);            
        }
    }

    if (tmax == maxtime) tmax = INTERSECTION_INFINIT_TIME;

    stat.success();
    return new BoxIntersection(this,r,tmin,tmax,smin,smax);
  
}


bool Box::isInsideAt(const Vector3 &a) const {

    return (a[0]>=boxmin[0] && a[0]<=boxmax[0] &&
            a[1]>=boxmin[1] && a[1]<=boxmax[1] &&
            a[2]>=boxmin[2] && a[2]<=boxmax[2])
           ^inverse;
           
}


bool Box::hasFinitBounding() const {

    return true;

}


Vector3 Box::getBoundingMin() const {

    if (boundingbox) {
        return Object3D::getBoundingMin();
    } else {
        return boxmin;
    }

}


Vector3 Box::getBoundingMax() const {

    if (boundingbox) {
        return Object3D::getBoundingMax();
    } else {
        return boxmax;
    }

}


void Box::calculateBoundingBox() {

    if (boundingbox) delete(boundingbox);
    boundingbox = new BoundingBox(boxmin,boxmax);
    boundingbox->transformate(*this);

}


void Box::BoxIntersection::next() {
    
    currenttime = secondtime;
    currentside = secondside;
    secondtime = INTERSECTION_INFINIT_TIME;
    
}


Vector3 Box::BoxIntersection::normal() {
    switch(currentside) {
        case(X): return Vector3(1,0,0);
        case(Y): return Vector3(0,1,0);
        case(Z): return Vector3(0,0,1);
#ifndef NDEBUG        
        default: throw(RenderError("Invalid Box side"));
#endif        
    }
}

