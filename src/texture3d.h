/* *************************************************************************
                            texture.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: texture3d.h,v 1.5 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "../src/colour.h"
#include "../src/finish.h"
#include "../src/pigment3d.h"
#include "../src/texture.h"
#include "../src/transparenttransformatable.h"

/**Describes a texture for each point in space. Texture includes colour and
  *finish.
  *@author Micha Riser
  */

class Texture3D: public TransparentTransformatable {

public: // Public members
    Pigment3D *pigment;
    Finish *finish;

public: // Constructors and destructor
    Texture3D(): pigment(0), finish(0) {}
    Texture3D(Pigment3D *p, Finish *f): pigment(p), finish(f) {}
    Texture3D(const Texture3D &t): TransparentTransformatable(t) {
        if (t.pigment) pigment = t.pigment->copy(); else pigment = 0;
        if (t.finish) finish = new Finish(*t.finish); else finish = 0;
    }

    ~Texture3D() {
        delete(finish);
        delete(pigment);
    }

private:
    Texture3D& operator=(const Texture3D&);
    
public: // Public methods

    /**Set texture components if not already set.
      */
    void overlay(const Texture3D& t) {
        if (!pigment) pigment = t.pigment->copy();
        if (!finish) finish = new Finish(*t.finish);
    }

    /**Evaluate texture at given point.
      */
    Texture* evaluateAt(const Vector3 &p) const;

};


#endif
