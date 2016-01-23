/* *************************************************************************
                          pixeloutputstream.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: pixeloutputstream.h,v 1.3 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef PIXELOUTPUTSTREAM_H
#define PIXELOUTPUTSTREAM_H

#include "../src/pixel.h"

/**Interface for a stream that accepts Pixels in unordered sequence
  *and generates some sort of output.
  *@author Micha Riser
  */

class PixelOutputStream {
  
public: // Constructors and destructors
    PixelOutputStream() {}
    virtual ~PixelOutputStream() {} 
  
public: // Virtual methods
    /**Write pixel to stream. Pixel can come in arbitrary order.
      */
    virtual void putPixel(const Pixel& p) = 0;

    /**Finish output.
      */
    virtual void done() = 0;
};

#endif
