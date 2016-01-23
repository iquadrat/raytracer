/* *************************************************************************
                          pigment3d.cpp  -  description
                             -------------------
    begin                : Tue Sep 3 2002
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/pigment3d.h"

Pigment3DMapped::~Pigment3DMapped() {
    
    delete(pattern);
    
}

ColourA Pigment3DMapped::evaluateAt(const Vector3& p_o) const {

    Vector3 p = p_o;
    transformPointIn(p);
    return lookup(pattern->evaluateAt(p),p);

}

Pigment3D* Pigment3DMapped::copy() const {

    return new Pigment3DMapped(*this);

}
