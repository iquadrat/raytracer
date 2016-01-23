/* *************************************************************************
                            texture.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: texture.cpp,v 1.7 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/texture.h"

#include "../src/cameraray.h"

Colour Texture::enlightShadow(const CameraRay&) const {

    ColourA ret = finish->ambient.scaled(*colour);
    ret.scale( 1-colour->alpha() );
    return ret;

}

Colour Texture::enlightLight(LightRay &lray, const CameraRay& cray) const {

    Colour ret(0);
    Colour intensityorig = lray.shoot();

    // diffuse:
    DBL factor = Vector3::dotProd(cray.normal(), lray.getDirection());
    if (factor > 0) {
        Colour intensity = intensityorig.scaled(factor*finish->diffuse);
        intensity.scale(*colour);
        ret = intensity;
    }
    ret.scale(1-colour->alpha());
    
    // phong:
    if (factor>0 && finish->phong !=0) {
        DBL x = -2*Vector3::dotProd(cray.normal(),cray.getDirection());
        Vector3 reflecteddirection = cray.normal().scaled(x);
        reflecteddirection.add(cray.getDirection());
        DBL a = Vector3::dotProd(reflecteddirection, lray.getDirection());
        if (a>0) {
            a = pow(a,finish->phongsize);
            Colour c = intensityorig.scaled(a*finish->phong);
            ret.add(c);
        }
    }
    return ret;
  
}
