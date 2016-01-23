/* *************************************************************************
                            texture.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: texture.h,v 1.4 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../src/colour.h"
#include "../src/finish.h"
#include "../src/lightray.h"

class CameraRay;

/**Texture describes evaluated colour and finish properties at
  *object surface.
  *@author Micha Riser
  */
class Texture {

public: // Public attributes
    ColourA* colour;
    const Finish* finish;

public: // Constructors and destructor	
    Texture(ColourA *c, const Finish *f): colour(c), finish(f) {}

    ~Texture() {
        delete(colour);
    }

private: // Private constructor
    Texture(const Texture& t);
    Texture& operator=(const Texture&);
  
public: // Public methods

    void setColour(ColourA* c) {/* check for existing colour? */ if (c) colour = new ColourA(*c);}
    void setFinish(const Finish* f) {finish = f;}

    Colour enlightShadow(const CameraRay &cray) const;
    Colour enlightLight(LightRay& lray, const CameraRay& cray) const;  

};

#endif
