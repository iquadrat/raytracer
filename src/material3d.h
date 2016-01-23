/* *************************************************************************
                          material3d.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: material3d.h,v 1.7 2003/03/03 11:44:05 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef MATERIAL3D_H
#define MATERIAL3D_H

#include "../src/interior.h"
#include "../src/texture3d.h"
#include "../src/transparenttransformatable.h"
#include "../src/types.h"

/**Material. Material combines surface properties and interior properties
  *of objects.
  *@author Micha Riser
  */
  
class Material3D: public TransparentTransformatable {

public: // Public attributes
    Texture3D* texture;
    Interior* interior;

public: // Constructors and destructor
    Material3D(): texture(0), interior(0) {}
    Material3D(Texture3D* t, Interior* i): texture(t), interior(i) {}
    Material3D(const Material3D& m): TransparentTransformatable(m) {
        if (m.texture) texture = new Texture3D(*m.texture); else texture = 0;
        if (m.interior) interior = new Interior(*m.interior); else interior = 0;
    }

    ~Material3D() {
        delete(texture);
        delete(interior);
    }

private:
    Material3D& operator=(const Material3D&);

public: // Public methods
    /**Set material properties by given Material3D if they are not already set.
      */
    void overlay(const Material3D& m) {
        if (!texture) 
            texture = new Texture3D(*m.texture);
        else
            texture->overlay(*m.texture);
            
        if (!interior)
            interior = new Interior(*m.interior);
        else
            interior->overlay(*m.interior);
    }

};


#endif
