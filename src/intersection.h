/* *************************************************************************
                          intersection.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002       
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: intersection.h,v 1.9 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "../src/ray.h"
#include "../src/types.h"
#include "../src/vector.h"

class Texture;
class SurfacePatch3D;
class Ray;

/**Describes the intersection of a ray with an object. Also provides
  *information about the surface point hit.
  *@author Micha Riser
  */
class Intersection {

public: // Constructors and destructor
    Intersection(const SurfacePatch3D* o, const Ray& r): hitobject(o), ray(r) {}

    virtual ~Intersection() {};

private:
    Intersection(const Intersection&);
    Intersection& operator=(const Intersection&);

protected: // Private attributes
    const SurfacePatch3D* hitobject;
    Ray ray;
    
public: // Public methods

    /**
      *@returns a pointer to the object which is hit by the ray
      */
    const SurfacePatch3D* hitObject() const {return hitobject;}

    // Virtual methods

    /**
      *@returns the texture at the current intersection
      */
    virtual Texture* texture();

    // Pure virutal methods

    /**Go to the next intersecting point.
      */
    virtual void next() = 0;

    /**
      *@returns Current time of intersecting. If no more intersection then
      *it returns INTERSECTION_TIME_INFINIT.
      */
    virtual DBL currentTime() = 0;

    /**
      *@returns the surface normal at the current intersection
      */
    virtual Vector3 normal() = 0;
    
};

#endif
