/* *************************************************************************
                          trianglepatch.cpp  -  description
                             -------------------
    begin                : Sun Dec 1 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org
    
    $Id: trianglepatch.cpp,v 1.9 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/trianglepatch.h"

#include "../src/matrix.h"

HitStatistics TrianglePatch::stat("Triangle","Tests","Hits");

SurfacePatch3D* TrianglePatch::duplicate() const {
    return new TrianglePatch(*this);
}

bool TrianglePatch::hasFinitBounding() const {
    return true;
}

Vector3 TrianglePatch::getBoundingMin() const {

    Vector3 min = p(0);
    min.minimize(p(1));
    min.minimize(p(2));
    return min;

}

Vector3 TrianglePatch::getBoundingMax() const {

    Vector3 max = p(0);
    max.maximize(p(1));
    max.maximize(p(2));
    return max;

}


Intersection* TrianglePatch::intersectLimitedTime(const Ray &r, DBL maxtime) const {

    stat.eval();

    // *check me* for correctness

    Vector3 dir = r.getDirection();
    DBL inverselength = 1.0/dir.length();
    dir.scale(inverselength);

    Vector3 edge1 = Vector3::sub(p(1),p(0)); 
    Vector3 edge2 = Vector3::sub(p(2),p(0)); 

    Vector3 pvec = Vector3::crossProd(dir,edge2);
    DBL det = Vector3::dotProd(edge1,pvec);

    if (det > -INTERSECTION_TIME_EPSILON && det < INTERSECTION_TIME_EPSILON) return 0;

    DBL inv_det = 1 / det;

    Vector3 tvec = Vector3::sub(r.getOrigin(),p(0));
    DBL u = Vector3::dotProd(tvec,pvec) * inv_det;
    if (u<0 || u>1) return 0;

    Vector3 qvec = Vector3::crossProd(tvec,edge1);
    DBL v = Vector3::dotProd(dir,qvec) * inv_det;
    if (v<0 || u+v>1) return 0;

    DBL t = Vector3::dotProd(edge2,qvec) * inv_det * inverselength;

    if (t>INTERSECTION_TIME_EPSILON && t<maxtime) {

        stat.success();
        return new TriangleIntersection(this,r,t);

    } else return 0;

}

Vector3 TrianglePatch::TriangleIntersection::normal() {

    const TrianglePatch* par = static_cast<const TrianglePatch*>(hitobject);

    Vector3 u = Vector3::sub(par->p(1),par->p(0));
    Vector3 v = Vector3::sub(par->p(2),par->p(0));
    return Vector3::crossProd(u,v);

}


DBL TrianglePatch::intersectBounding(const Ray &r) const {
    return INTERSECTION_TIME_EPSILON;    
}

DBL TrianglePatch::intersectBounding(const Ray &r, DBL tmax) const {
    return INTERSECTION_TIME_EPSILON;
}

