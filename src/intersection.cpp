/* *************************************************************************
                          intersection.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002   
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: intersection.cpp,v 1.8 2003/01/18 18:23:20 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */
 
#include "../src/intersection.h"

#include "../src/material3d.h"
#include "../src/object3d.h"
#include "../src/ray.h"

Texture* Intersection::texture() {

    if (hitobject == 0) return new Texture(0,0);

    const Object3D* hitobject1 = static_cast<const Object3D*>(hitobject);    // *fix me*: surfacepatch has no material

    if (!hitobject1->material || !hitobject1->material->texture) return new Texture(0,0);

    return hitobject1->material->texture->evaluateAt( ray.positionAtTime(currentTime()) );
  
}
