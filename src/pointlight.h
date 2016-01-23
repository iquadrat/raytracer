/* *************************************************************************
                          pointlight.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: pointlight.h,v 1.8 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "../src/lightcache.h"
#include "../src/lightsource.h"

/**Point light. Light with no dimension.
  *@author Micha Riser
  */

class PointLight: public LightSource  {
  
public: // Constructors and destructor
    PointLight(const Vector3& location, const Colour& col, Scene* s, bool do_fading=false): // *change me* default to true
      LightSource(location), c(col), scene(s), fading(do_fading) {}

    ~PointLight() {}

public: // Public methods

    // Inherited by LightSource
    LightRay shineTo(const Vector3& to, InsideList* l) const;
    Colour enlight(const CameraRay& cray, const Texture* t, InsideList* l) const;

private: // Private attributes
    Colour c;
    Scene* scene;
    bool fading;
    
};

#endif
