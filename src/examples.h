/* *************************************************************************
                          examples.h  -  description
                             -------------------
    begin                : Wed Oct 23 2002
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: examples.h,v 1.7 2003/07/21 23:41:51 micha Exp $
    
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

Pigment3D* setUpScene_Isosurface();
Pigment3D* setUpScene_Menger();
Pigment3D* setUpScene_MediaMenger();
Pigment3D* setUpScene_MediaRoom();
Pigment3D* setUpScene_RecCamera();
Pigment3D* setUpScene_BlendCamera();
Pigment3D* setUpScene_SphereSpiral();
Pigment3D* setUpScene_RandomSpheres();
Pigment3D* setUpScene_RandomCylinders();
Pigment3D* setUpScene_Balls(bool reflective_box);
Pigment3D* setUpScene_PigmentTest();

