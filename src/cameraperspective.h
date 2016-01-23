/* *************************************************************************
                          cameraperspective.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002       
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: cameraperspective.h,v 1.8 2003/03/03 11:44:05 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef CAMERAPERSPECTIVE_H
#define CAMERAPERSPECTIVE_H

#include "../src/colour.h"
#include "../src/insidelist.h"
#include "../src/pigment3d.h"
#include "../src/vector.h"

class Scene;

/**Perspective Camera.
  *@author Micha Riser
  */
class CameraPerspective : public Pigment3D  {

public: // Constructors and destructor

    /**Constructs a perspective camera with default parameters.
      */
    CameraPerspective(Scene* s);

    /**Constructs a camera for given parameters.
      */
    CameraPerspective(Scene* s, const Vector3& loc, const Vector3& dir,
      const Vector3& r, const Vector3& u);

    ~CameraPerspective();

private:
    CameraPerspective& operator=(const CameraPerspective&);

public: // Public methods

    // Inherited from Pigment3D
    ColourA evaluateAt(const Vector3& p) const;
    Pigment3D* copy() const {return new CameraPerspective(*this);}
    
private: // Private attributes
    Vector3 location;
    Vector3 direction;
    Vector3 right;
    Vector3 up;

    /**Pointer to the scene attached to the camera.
      */
    Scene* scene;

    /**Lists the object which the camera's origin is inside.
      */
    mutable InsideList* insides;
        
private:
    // Copy constructor
    CameraPerspective(const CameraPerspective& c);
    
};

#endif
