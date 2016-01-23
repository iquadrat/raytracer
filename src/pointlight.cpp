/* *************************************************************************
                          pointlight.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: pointlight.cpp,v 1.12 2003/07/15 11:03:09 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/pointlight.h"

#include "../src/texture.h"

LightRay PointLight::shineTo(const Vector3& to, InsideList* l) const {

    if (fading) {
        CLR dist = Vector3::distance(location(),to);
        return LightRay(to, getTranslation(), scene, c.scaled(1.0f/(1.0f+sqr(dist))), l);
    } else {
        return LightRay(to, getTranslation(), scene, c, l);    
    }
}

Colour PointLight::enlight(const CameraRay& cray, const Texture* t, InsideList* l) const {
    LightRay test = shineTo(cray.hitpoint(), l);
    return t->enlightLight(test, cray);
}
