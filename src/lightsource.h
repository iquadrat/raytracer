/* *************************************************************************
                          lightsource.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: lightsource.h,v 1.10 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "../src/cameraray.h"
#include "../src/colour.h"
#include "../src/lightray.h"
#include "../src/transparenttransformatable.h"
#include "../src/vector.h"

/**Superclass for all light sources.
  *@author Micha Riser
  */
class LightSource : public TransparentTransformatable {

public: // Constructors and destructor
    LightSource(const Vector3& loc): TransparentTransformatable(loc) {}

public: // Public methods
    /**@return location of the light source
      */
    Vector3 location() const {return getTranslation();}

    /**Introduces order between light sources for holding them
      *in a list. Order has no special meaning, it is just to distinguish them.
      */
    bool operator<(const LightSource& l) {return this<&l;}

    // Pure virtual methods
    virtual LightRay shineTo(const Vector3& to, InsideList* l) const = 0;
    virtual Colour enlight(const CameraRay& cray, const Texture* t, InsideList* l) const = 0;
        
};

#endif
