/* *************************************************************************
                          cylinder.h  -  description
                             -------------------
    begin                : Sun Nov 24 2002
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: cylinder.h,v 1.5 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef CYLINDER_H
#define CYLINDER_H

#include "../src/intersection.h"
#include "../src/object3d.h"

/**Cylinder Object
  *@author Micha Riser
  */

class Cylinder : public Object3D  {

public: // Constructors and destructor
    /**Create Cylinder: base point origin, cap point <0,0,1>, radius 1
      */
    Cylinder();

    /**Create Cylinder from base point, cap point and radius.
      */
    Cylinder(const Vector3& base, const Vector3& cap, DBL r);

public: // Pulblic methods

    // Inherited from Object3D
    Object3D* duplicate() const {return new Cylinder(*this);}

    Intersection* intersectLocalLimitedTime(const Ray &r, DBL maxtime) const;
    bool isInsideAt(const Vector3 &locallocation) const;

private: // Types
    enum hittype {SIDE,BASE,CAP,NONE};

private: // Static members
    static HitStatistics stat;

private: // Inner classes

    /**Describes Ray-Cylinder Intersection.
      */
    class CylinderIntersection: public Intersection {

    private: // Private attributes
        DBL currenttime, secondtime;
        hittype hit1, hit2;

    public: // Constructor and destructor
        CylinderIntersection(const Object3D* o, const Ray &r, DBL t1, DBL t2, hittype h1, hittype h2):
          Intersection(o,r), currenttime(t1), secondtime(t2), hit1(h1), hit2(h2) {}

    public: // Pulblic methods

        //Inherited from Intersection
        DBL currentTime() {return currenttime;}
        void next();
        Vector3 normal();

    };

private: // Private methods
    static DBL absVec2(const Vector3&);
    static DBL dotVec2(const Vector3&, const Vector3&);
    
};

#endif
