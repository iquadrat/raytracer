/* *************************************************************************
                          ray.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: ray.h,v 1.6 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef RAY_H
#define RAY_H

#include "../src/vector.h"

/**Ray.
  *@author Micha Riser
  */

class Ray {
        
    friend class TransparentTransformatable;
    friend class BoundingBox;
    
protected: // Protected attributes

    Vector3 origin;
    Vector3 direction;

public: // Constructors and destructor
    Ray(Vector3 o, Vector3 d):origin(o),direction(d) {};

public: // Public methods:
    /**
      *@returns ray's origin
      */
    Vector3 getOrigin() const {return origin;}

    /**
      *@returns ray's direction
      */
    Vector3 getDirection() const {return direction;}

    /**
      *@param t time
      *@returns ray's position at given time
      */
    Vector3 positionAtTime(DBL t) const {
        Vector3 r = direction.scaled(t);
        r.add(origin);
        return r;
    }

};
#endif
