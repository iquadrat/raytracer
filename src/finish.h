/* *************************************************************************
                          finish.h  -  description
                             -------------------
    begin                : Wed Oct 15 2002
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: finish.h,v 1.6 2003/05/25 11:25:52 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef FINISH_H
#define FINISH_H

#include "../src/colour.h"
#include "../src/types.h"

/**Surface finish. Includes diffuse and reflection properties and phong
  *highlighting.
  *@author Micha Riser
  */

class Finish {

public: // Public attributes
    Colour ambient;
    FLT diffuse;
    FLT reflection;
    FLT phong;
    FLT phongsize;
    
public: // Constructors and destructor
    Finish(Colour fambient, FLT fdiffuse, FLT freflection, 
      FLT fphong, FLT fphongsize): 
      ambient(fambient), diffuse(fdiffuse), reflection(freflection), 
      phong(fphong), phongsize(fphongsize) {}

    ~Finish() {}

};

#endif
