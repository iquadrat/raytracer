/* *************************************************************************
                          plane.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002                                 
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: plane.h,v 1.11 2003/05/25 11:25:52 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef PLANE_H
#define PLANE_H

#include "../src/intersection.h"
#include "../src/object3d.h"

/**Plane object.
  *@author Micha Riser
  */

class Plane: public Object3D  {

public: // Constructors and destructor
    /**Construct new plane through origin.
      *@param n normal vector
      */
    Plane(const Vector3& n): normal(n) {};
    ~Plane() {}

public: // Public methods

    // Inherited by Object3D
    Object3D* duplicate() const {return new Plane(*this);}  
    Intersection* intersectLocal(const Ray& r) const;
    Intersection* intersectLocalLimitedTime(const Ray &r, DBL maxtime) const;
    bool isInsideAt(const Vector3 &locallocation) const;

private: // Static members

    /**Intersection statistics.
      */
    static HitStatistics stat;
    
private: // Private attributes
#ifdef WRONG_INNER_CLASSES_ACCESS
public:
#endif
    Vector3 normal;
      
private: // Private inner classes

    /**Describes Ray-Plane intersection.
      */
    class PlaneIntersection: public Intersection {
    private: // Private attributes
        DBL time;

    public: // Constructors and destructor
        PlaneIntersection(const Plane* o, const Ray& r, DBL hittime):
          Intersection(o,r), time(hittime) {}
        ~PlaneIntersection() {}

    public: // Intersection methods
        DBL currentTime() {return time;}
        void next() {time = INTERSECTION_INFINIT_TIME;}
        Vector3 normal();
    };
      
};

#endif
