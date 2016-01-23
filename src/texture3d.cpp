/* *************************************************************************
                            texture.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: texture3d.cpp,v 1.5 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/texture3d.h"

Texture* Texture3D::evaluateAt(const Vector3 &p) const {
    
    ColourA* evalpig = 0;
    if (pigment) {
        evalpig = new ColourA(pigment->evaluateAt(p));
    }
    return new Texture(evalpig,finish);
    
}
