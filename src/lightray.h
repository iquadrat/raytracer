/* *************************************************************************
                          lightray.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: lightray.h,v 1.16 2003/07/15 11:03:09 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef LIGHTRAY_H
#define LIGHTRAY_H

#include "../src/colour.h"
#include "../src/insidelist.h"
#include "../src/ray.h"

class Scene;

/**Ray emitted by light source. Used for shadow tests.
  *@author Micha Riser
  */
class LightRay: public Ray {

public: // Constructor and destructor
    LightRay(const Vector3& start, const Vector3& end, Scene* s,
      const Colour lightcolour, const InsideList* l):
      Ray(start, Vector3::sub(end,start)), myscene(s), colour(lightcolour),
      inside(), length(direction.length()) {

        direction.scale(1/length);
        if (l) inside = InsideList(*l);

    }

    LightRay(const LightRay& l): Ray(l), myscene(l.myscene), colour(l.colour),
      inside(l.inside), length(l.length) {}
    
    ~LightRay() {}

private:
    LightRay& operator=(const LightRay&);

public: // Public methods
    /**Shoots the ray.
      *@returns Light colour that arrives at end point.
      */
    Colour shoot();

private: // Private attributes
    Scene* myscene;
    Colour colour;
    InsideList inside;
    DBL length;

};

#endif
