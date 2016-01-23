/* *************************************************************************
                          cylinder.cpp  -  description
                             -------------------
    begin                : Sun Nov 24 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: cylinder.cpp,v 1.6 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/cylinder.h"

HitStatistics Cylinder::stat("Cylinder","Tests","Hits");

Cylinder::Cylinder() {
    boundingbox = new BoundingBox(Vector3(-1,-1,0),Vector3(1,1,1));
}

Cylinder::Cylinder(const Vector3& base, const Vector3& cap, DBL r) {

    // init bounding box
    boundingbox = new BoundingBox(Vector3(-1,-1,0),Vector3(1,1,1));

    // calcuate axis vector
    Vector3 axis = Vector3::sub( cap, base );
    DBL axislength = axis.length();

    // check for degenerated cylinder    
    if (axislength < DIRECTION_EPSILON) {
        throw ObjectConstructionError("degenerated cylinder (cap point = base point)");
    }

    // scale cylinder to fit length and radius
    scale( Vector3(r,r,axislength) );

    // calcuate rotation axis
    Vector3 rotaxis = Vector3::crossProd( axis,Vector3(0,0,1) );
    // if axis is colinear to z-axis then rotate around x
    if (rotaxis.length()<DIRECTION_EPSILON) rotaxis = Vector3(1,0,0);

    DBL rotangle = acos( Vector3::dotProd(Vector3(0,0,1),axis)/axislength );

    // do rotation
    axisRotate(rotaxis,rotangle);
    // do translation
    translate(base);    

}

DBL Cylinder::absVec2(const Vector3& v) {
    DBL l = 0;
    for(int i=0; i<2; i++) l += v[i]*v[i];
    return l;
}

DBL Cylinder::dotVec2(const Vector3& v, const Vector3& w) {
    DBL p = 0;
    for(int i=0; i<2; i++) p += v[i]*w[i];
    return p;
}

Intersection* Cylinder::intersectLocalLimitedTime(const Ray &r, DBL maxtime) const {

    DBL t1 = INTERSECTION_INFINIT_TIME,t2 = INTERSECTION_INFINIT_TIME,t3,t4;

    stat.eval();

    Vector3 dir = r.getDirection();
    Vector3 org = r.getOrigin();

    // find side intersection(s)
    DBL div = absVec2(dir);
    if (div>DIRECTION_EPSILON) {

        DBL p = dotVec2(dir,org);
        DBL det = p*p - (absVec2(org)-1)*div;

        if (det>0) {

            det = sqrt(det);
        
            t1 = (-p - det)/div;
            DBL z = t1*dir[2]+org[2];

            if ( t1>INTERSECTION_TIME_EPSILON && t1<maxtime && z>0 && z<1 ) {

                t2 = (-p + det)/div;
                z = t2*dir[2]+org[2];
                if (!( t2>INTERSECTION_TIME_EPSILON && t2<maxtime && z>0 && z<1 )) {
                    t2 = INTERSECTION_INFINIT_TIME;
                }

            } else {

                t2 = INTERSECTION_INFINIT_TIME;
                t1 = (-p + det)/div;
                z = t1*dir[2]+org[2];
                if (!( t1>INTERSECTION_TIME_EPSILON && t1<maxtime && z>0 && z<1 )) {
                    t1 = INTERSECTION_INFINIT_TIME;
                }

            }

        }
        
    }

    // find base/cap intersection
    t3 = -org[2]/dir[2];    
    if ( t3<INTERSECTION_TIME_EPSILON || t3>maxtime || pow(org[0]+t3*dir[0],2) + pow(org[1]+t3*dir[1],2) > 1) t3 = INTERSECTION_INFINIT_TIME;
    
    t4 = (1-org[2])/dir[2];
    if ( t4<INTERSECTION_TIME_EPSILON || t4>maxtime || pow(org[0]+t4*dir[0],2) + pow(org[1]+t4*dir[1],2) > 1) t4 = INTERSECTION_INFINIT_TIME;

    if (t4<t3) std::swap(t3,t4);

    // find smallest two intersection times    
    if (t1<=t3) {

        if (t1 == INTERSECTION_INFINIT_TIME) return 0;

        stat.success();
        if (t2<=t3)
            return new CylinderIntersection(this,r,t1,t2,SIDE,SIDE);
        else 
            return new CylinderIntersection(this,r,t1,t3,SIDE,BASE);
                    
    } else {

        if (t3 == INTERSECTION_INFINIT_TIME) return 0;

        stat.success();
        if (t4<=t1)
            return new CylinderIntersection(this,r,t3,t4,BASE,BASE);
        else
            return new CylinderIntersection(this,r,t3,t1,BASE,SIDE);
        
    }
    
}

bool Cylinder::isInsideAt(const Vector3 &p) const {

    return (p[2]>=0 && p[2]<=1 && absVec2(p)<1);

}


void Cylinder::CylinderIntersection::next() {

    if (currenttime != INTERSECTION_INFINIT_TIME) {
        currenttime = secondtime;
        hit1 = hit2;
        secondtime = INTERSECTION_INFINIT_TIME;
    }

}

Vector3 Cylinder::CylinderIntersection::normal() {

    if (hit1 == SIDE) {
        Vector3 hp = ray.positionAtTime(currenttime);
        return Vector3(hp[0],hp[1],0);
    } else {
        return Vector3(0,0,1);
    }

}
