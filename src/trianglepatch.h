/* *************************************************************************
                          trianglepatch.h  -  description
                             -------------------
    begin                : Sun Dec 1 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org
    
    $Id: trianglepatch.h,v 1.14 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef TRIANGLEPATCH_H
#define TRIANGLEPATCH_H

#include "../src/intersection.h"
#include "../src/statistics.h"
#include "../src/surfacepatch3d.h"

/**A single triangle for a mesh (SharedPointPatchObject).
  *@author Micha Riser
  */

class TrianglePatch: public SurfacePatch3D {

public: // Constructor and destructor

    /**Creates triangle that can share its vertices with others.
      */
    TrianglePatch(Vector3* edge1, Vector3* edge2, Vector3* edge3) {
        edge[0] = edge1;
        edge[1] = edge2;
        edge[2] = edge3;
    }

private:
    // Copy constructor
    TrianglePatch(const TrianglePatch& p): SurfacePatch3D(p) {
        for(CNT8 i = 0; i<3; ++i) edge[i] = p.edge[i];
    }

    TrianglePatch& operator=(const TrianglePatch&);

public: // Public methods

    // to implement SurfacePatch3D
    SurfacePatch3D* duplicate() const;
    bool hasFinitBounding() const;
    Vector3 getBoundingMin() const;
    Vector3 getBoundingMax() const;
        
    DBL intersectBounding(const Ray &r) const;
    DBL intersectBounding(const Ray &r, DBL tmax) const;    
    Intersection* intersectLimitedTime(const Ray &r, DBL maxtime) const;

private: // Private methods
    const Vector3& p(CNT8 i) const {return *edge[i];}    

private: // Static members
    static HitStatistics stat;

private: // Private members    
    Vector3* edge[3];

private: // inner classes

    class TriangleIntersection: public Intersection {

    public: // Constructor and destructor
        TriangleIntersection(const SurfacePatch3D* o, const Ray& r, DBL time): Intersection(o,r), t(time) {}

    public:
        void next() {t = INTERSECTION_INFINIT_TIME;}
        DBL currentTime() {return t;}
        Vector3 normal();

    private: // Private attributes
        DBL t;

    };
    

};

#endif
