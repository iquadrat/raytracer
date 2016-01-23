/* *************************************************************************
                          plane.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: plane.cpp,v 1.9 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/plane.h"

HitStatistics Plane::stat("Plane","Tests","Hits");


Intersection* Plane::intersectLocal(const Ray& r) const {

    DBL divisor = Vector3::dotProd(normal, r.getDirection());
    if (divisor == 0) return 0;
    DBL t = -Vector3::dotProd(normal, r.getOrigin()) / divisor;
    if (t>INTERSECTION_TIME_EPSILON) {
        
        stat.evalSuccess();
        return new PlaneIntersection(this,r,t);
        
    } else {
        
        stat.evalFailed();
        return 0;
        
    }

}


Intersection* Plane::intersectLocalLimitedTime(const Ray &r, DBL maxtime) const {

    DBL divisor = Vector3::dotProd(normal, r.getDirection());
    if (divisor == 0) return 0;
    DBL t = -Vector3::dotProd(normal, r.getOrigin()) / divisor;
    if (t>INTERSECTION_TIME_EPSILON && t<maxtime) {
        
        stat.evalSuccess();
        return new PlaneIntersection(this,r,t);
        
    } else {

        stat.evalFailed();
        return 0;
        
    }

}


bool Plane::isInsideAt(const Vector3 &locallocation) const {
    return (Vector3::dotProd(normal, locallocation) < 0)^inverse;
}


Vector3 Plane::PlaneIntersection::normal() {
    return static_cast<const Plane*>(hitobject)->normal;
}


