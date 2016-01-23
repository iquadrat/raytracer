/* *************************************************************************
                          pixel.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: pixel.h,v 1.4 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef PIXEL_H
#define PIXEL_H

#include "../src/colour.h"

/**Element of the picture.
  *@author Micha Riser
  */
class Pixel {
  
public: // Public attributes
    /**Horizontal coordinate.
      */
    int x;

    /**Vertical coordinate.
      */
    int y;

    /**RGB Colour value.
      */
    Colour c;
  
public: // Constructor and destructor
    Pixel(): c(0) {}
    Pixel(int px, int py, const Colour& col): x(px), y(py), c(col) {}
    ~Pixel() {}

public: // Public methods

    /**Introduce order between pixels.
      */
    bool operator<(const Pixel& s) const {
        if (y<s.y) return true; else if (y>s.y) return false;
        if (x<s.x) return true; else if (x>s.x) return false;
        return false;
    }

};

#endif
