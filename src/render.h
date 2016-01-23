/* *************************************************************************
                          render.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: render.h,v 1.5 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef RENDER_H
#define RENDER_H

#include "../src/pigment3d.h"
#include "../src/pixel.h"
#include "../src/pixeloutputstream.h"

/**Combines everything that is neccessary for the rendering process.
  *@author Micha Riser
  */

class Render {

private: // Private attributes
    Pigment3D* imageplane;
    PixelOutputStream* out;
    int w;
    int h;
  
public: // Constructors and destructor

    /**Create new render job.
      *@param camera scene's camera.
      *@param o picture output
      *@param width picture width
      *@param height picture height
      */
    Render(Pigment3D* camera, PixelOutputStream* o, int width, int height):
      imageplane(camera), out(o), w(width), h(height) {};
      
    ~Render() {};
private:
    Render(const Render&);
    Render& operator=(const Render&);

public: // Public methods
    /**Start rendering.
      */
    void doRendering();
  
};

#endif
