/* *************************************************************************
                          ppmoutputstream.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: ppmoutputstream.h,v 1.5 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef PPMOUTPUTSTREAM_H
#define PPMOUTPUTSTREAM_H

#include <string>
#include <set>
#include <stdio.h>

#include "../src/colour.h"
#include "../src/pixeloutputstream.h"

/**Writes a PPM image.
  *@author Micha Riser
  */

class PPMOutputStream : public PixelOutputStream  {
  
public: // Constructors and destructors
    PPMOutputStream(const std::string& filename, int w, int h);
    ~PPMOutputStream() {};

public: // Public methods
    /**Write pixel to stream. Pixel can come in arbitrary order.
      */
    void putPixel(const Pixel& p);

    /**Finish output.
      */
    void done();

private: // Private attributes
    FILE* ofile;
    int xnext;
    int ynext;
    int width;
    int height;
    std::set<Pixel> buffer;
    
private: // Private methods
    void writeColour(const Colour& c);
    void next();      
};

#endif
