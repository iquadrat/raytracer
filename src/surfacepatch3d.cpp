/* *************************************************************************
                          surfacepatch3d.cpp  -  description
                             -------------------
    begin                : Sat Nov 30 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org
    
    $Id: surfacepatch3d.cpp,v 1.7 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/surfacepatch3d.h"

#include "../src/intersection.h"


Intersection* SurfacePatch3D::intersect(const Ray &r) const {
    return intersectLimitedTime(r,INTERSECTION_INFINIT_TIME);
}
