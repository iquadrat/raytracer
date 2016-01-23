/* *************************************************************************
                          boundingbox.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: boundingbox.cpp,v 1.17 2003/05/25 11:25:52 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/boundingbox.h"

#include "../src/intersection.h"
#include "../src/transparenttransformatable.h"

HitStatistics BoundingBox::stat("Bounding Box","Tests","Hits");


void BoundingBox::scale(const Vector3& v) {

    for(int i=0; i<3; i++) {
        
        boxmin[i] *= v[i];
        boxmax[i] *= v[i];
                    
        if (v[i]<0) std::swap(boxmin[i],boxmax[i]);
        
    }

}


void BoundingBox::updateBoundingBoxPoints(Vector3& min, Vector3& max,
  const Matrix& mat, Vector3 test) {
    
    test.applyFromLeft(mat);
    min.minimize(test);
    max.maximize(test);
    
}


void BoundingBox::applyMatrix(const Matrix& m) {

    Vector3 newmin = boxmin;
    newmin.applyFromLeft(m);
    Vector3 newmax = newmin;

    updateBoundingBoxPoints(newmin, newmax, m, Vector3(boxmin[0],boxmin[1],boxmax[2]) );
    updateBoundingBoxPoints(newmin, newmax, m, Vector3(boxmin[0],boxmax[1],boxmin[2]) );
    updateBoundingBoxPoints(newmin, newmax, m, Vector3(boxmin[0],boxmax[1],boxmax[2]) );
    updateBoundingBoxPoints(newmin, newmax, m, Vector3(boxmax[0],boxmin[1],boxmin[2]) );
    updateBoundingBoxPoints(newmin, newmax, m, Vector3(boxmax[0],boxmin[1],boxmax[2]) );
    updateBoundingBoxPoints(newmin, newmax, m, Vector3(boxmax[0],boxmax[1],boxmin[2]) );
    updateBoundingBoxPoints(newmin, newmax, m, Vector3(boxmax[0],boxmax[1],boxmax[2]) );
        
    boxmin = newmin;
    boxmax = newmax;
    
}


void BoundingBox::rotateRad(const Vector3& v) {

    applyOrthogonalMatrix(Matrix::generateRotationMatrix(v));

}


void BoundingBox::rotateDeg(const Vector3& v) {

    applyOrthogonalMatrix(Matrix::generateRotationMatrix(v.scaled(PI/180.0)));

}


void BoundingBox::transformate(const TransparentTransformatable &t) {

    if (t.getTransformationMatrix()) {applyOrthogonalMatrix(*t.getInverseMatrix());}
    translate(t.getTranslation());

}


DBL BoundingBox::intersect(const Ray &r) const {

    stat.eval();

    // returns INTERSECTION_TIME_EPSILON if ray's origin is inside box!

    // calculate x time interval
    DBL tmin;
    DBL tmax;

    if (r.direction[0]>0) {

        tmax = (-r.origin[0]+boxmax[0]) / r.direction[0];
        if (tmax<INTERSECTION_TIME_EPSILON) return -1;            
        tmin = (-r.origin[0]+boxmin[0]) / r.direction[0];
        if (tmin<INTERSECTION_TIME_EPSILON) tmin = INTERSECTION_TIME_EPSILON;
            
    } else if (r.direction[0]!=0) {

        tmax = (-r.origin[0]+boxmin[0]) / r.direction[0];
        if (tmax<INTERSECTION_TIME_EPSILON) return -1;
        tmin = (-r.origin[0]+boxmax[0]) / r.direction[0];
        if (tmin<INTERSECTION_TIME_EPSILON) tmin = INTERSECTION_TIME_EPSILON;

    } else {

        if (r.origin[0]<boxmin[0] || r.origin[0]>boxmax[0] ) return -1;
        tmin = INTERSECTION_TIME_EPSILON;
        tmax = INTERSECTION_INFINIT_TIME;
        
    }
        
    // calculate y time interval
    if (r.direction[1]>0) {

        DBL t = (-r.origin[1]+boxmax[1]) / r.direction[1]; //y max
        if (t<tmin) return -1;  if (t<tmax) tmax = t;
        t = (-r.origin[1]+boxmin[1]) / r.direction[1]; //y min
        if (t>tmax) return -1;  if (t>tmin) tmin = t;
            
    } else if (r.direction[1]!=0) {

        DBL t = (-r.origin[1]+boxmin[1]) / r.direction[1]; //y max
        if (t<tmin) return -1;  if (t<tmax) tmax = t;
        t = (-r.origin[1]+boxmax[1]) / r.direction[1]; //y min
        if (t>tmax) return -1;  if (t>tmin) tmin = t;
            
    } else {
        
        if (r.origin[1]<boxmin[1] || r.origin[1]>boxmax[1] ) return -1;
        
    }
        
    // calculate z time interval
    if (r.direction[2]>0) {

        DBL t = (-r.origin[2]+boxmax[2]) / r.direction[2];  // z max
        if (t<tmin) return -1;  if (t<tmax) tmax = t;
        t = (-r.origin[2]+boxmin[2]) / r.direction[2];  // z min
        if (t>tmax) return -1;  if (t>tmin) tmin = t;
            
    } else if(r.direction[2]!=0) {

        DBL t = (-r.origin[2]+boxmin[2]) / r.direction[2]; // z max            
        if (t<tmin) return -1;  if (t<tmax) tmax = t;
        t = (-r.origin[2]+boxmax[2]) / r.direction[2]; // z min
        if (t>tmax) return -1;  if (t>tmin) tmin = t;
            
    } else {
        
        if (r.origin[2]>boxmin[2] || r.origin[2]<boxmax[2] ) return -1;
        
    }

    stat.success();
    return tmin;

}


DBL BoundingBox::intersect(const Ray &r, DBL tmax) const {

    stat.eval();

    // returns INTERSECTION_TIME_EPSILON if ray's origin is inside box!

    // calculate x time interval
    DBL tmin;

    if (r.direction[0]>0) {

        DBL t = (-r.origin[0]+boxmax[0]) / r.direction[0];
        if (t<INTERSECTION_TIME_EPSILON) return -1;  if (t<tmax) tmax = t;
        tmin = (-r.origin[0]+boxmin[0]) / r.direction[0];
        if (tmin<INTERSECTION_TIME_EPSILON) tmin = INTERSECTION_TIME_EPSILON;
        else if (tmin>tmax) return -1;

    } else if (r.direction[0]!=0){

        DBL t = (-r.origin[0]+boxmin[0]) / r.direction[0];
        if (t<INTERSECTION_TIME_EPSILON) return -1;  if (t<tmax) tmax = t;
        tmin = (-r.origin[0]+boxmax[0]) / r.direction[0];
        if (tmin<INTERSECTION_TIME_EPSILON) tmin = INTERSECTION_TIME_EPSILON;
        else if (tmin>tmax) return -1;
            
    } else {

        if (r.origin[0]<boxmin[0] || r.origin[0]>boxmax[0] ) return -1;
        tmin = INTERSECTION_TIME_EPSILON;
            
    }

    // calculate y time interval
    if (r.direction[1]>0) {

        DBL t = (-r.origin[1]+boxmax[1]) / r.direction[1]; //y max
        if (t<tmin) return -1;  if (t<tmax) tmax = t;
        t = (-r.origin[1]+boxmin[1]) / r.direction[1]; //y min
        if (t>tmax) return -1;  if (t>tmin) tmin = t;

    } else if (r.direction[1]!=0) {

        DBL t = (-r.origin[1]+boxmin[1]) / r.direction[1]; //y max
        if (t<tmin) return -1;  if (t<tmax) tmax = t;
        t = (-r.origin[1]+boxmax[1]) / r.direction[1]; //y min
        if (t>tmax) return -1;  if (t>tmin) tmin = t;

    } else {
        
        if (r.origin[1]<boxmin[1] || r.origin[1]>boxmax[1] ) return -1;
        
    }

    // calculate z time interval
    if (r.direction[2]>0) {

        DBL t = (-r.origin[2]+boxmax[2]) / r.direction[2];  // z max
        if (t<tmin) return -1;  if (t<tmax) tmax = t;
        t = (-r.origin[2]+boxmin[2]) / r.direction[2];  // z min
        if (t>tmax) return -1;  if (t>tmin) tmin = t;

    } else if(r.direction[2]!=0) {

        DBL t = (-r.origin[2]+boxmin[2]) / r.direction[2]; // z max
        if (t<tmin) return -1;  if (t<tmax) tmax = t;
        t = (-r.origin[2]+boxmax[2]) / r.direction[2]; // z min
        if (t>tmax) return -1;  if (t>tmin) tmin = t;

    } else {
        
        if (r.origin[2]>boxmin[2] || r.origin[2]<boxmax[2] ) return -1;
        
    }
    
    stat.success();
    
    return tmin;

}


bool BoundingBox::isInsideAt(const Vector3& a) const {

    return (a[0]>=boxmin[0] && a[0]<=boxmax[0] && 
            a[1]>=boxmin[1] && a[1]<=boxmax[1] && 
            a[2]>=boxmin[2] && a[2]<=boxmax[2]);
            
}
