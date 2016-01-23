/* *************************************************************************
                          surfacepatch3d.h  -  description
                             -------------------
    begin                : Sat Nov 30 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org
    
    $Id: surfacepatch3d.h,v 1.11 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef SURFACEPATCH_H
#define SURFACEPATCH_H

#include "../src/types.h"
#include "../src/vector.h"

class Intersection;
class Ray;

/**A piece of surface.
  *@author Micha Riser
  */
class SurfacePatch3D {

public: // Constructor and destructor
    SurfacePatch3D() {}
    virtual ~SurfacePatch3D() {}

public:
    // Methods used during pre-rendering stage:
    
    /**Create a duplicate of the object. Must be of the same type!
      *@returns An exact duplicate of the object.
      */
    virtual SurfacePatch3D* duplicate() const = 0;    

    /**Returns true if the surface can be enclosed by a finit box.
      */
    virtual bool hasFinitBounding() const = 0;

    /**Get bounding dimension (minimum edge).
      */
    virtual Vector3 getBoundingMin() const = 0;

    /**Get bounding dimension (maximum edge).
      */
    virtual Vector3 getBoundingMax() const = 0;

    /**Prepare object for render process. Some optimisations can be done only
      *before the rendering starts. This is the method that is to be called
      *just before doing so. The object has to be tolerant against multiple
      *calls of this method.
      */
    virtual void prepareToRender() {}

        
    // Methods used during rendering:
    
    /**Intersect Bounding with Ray.
      *@param r the Ray
      *@returns -1 if no intersection, INTERSECTION_TIME_EPSILON if Ray's origin
      *is inside the bounding. Otherwise it returns the time of first intersection with box.
      */
    virtual DBL intersectBounding(const Ray &r) const = 0;

    /**Intersect Bounding with Ray limited to maximum time.
      *@param r the Ray
      *@param tmax Maximum intersection time.
      *@returns -1 if no intersection, INTERSECTION_TIME_EPSILON if Ray's origin
      *is inside the bounding. Otherwise it returns the time of first intersection with box.
      */
    virtual DBL intersectBounding(const Ray &r, DBL tmax) const = 0;

    /**Intersect object with ray.
      *@param r the Ray
      *@returns 0 if no intersection else Intersection object
      */
    virtual Intersection* intersect(const Ray &r) const;

    /**Intersect object with ray limited to maximum time.
      *@param r the Ray
      *@param maxtime maximal intersecting time
      *@returns 0 if no intersection else Intersection object
      */
    virtual Intersection* intersectLimitedTime(const Ray &r, DBL maxtime) const = 0;

};

#endif
