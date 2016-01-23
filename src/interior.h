/* *************************************************************************
                          interior.h  -  description
                             -------------------
    begin                : Wed Oct 15 2002
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: interior.h,v 1.5 2003/03/03 11:44:05 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef INTERIOR_H
#define INTERIOR_H

#include "../src/media3d.h"
#include "../src/types.h"

/**Describes the interior properties of a material. Includes ior value and
  *media.
  *@author Micha Riser
  */

class Interior {

public: // Public attributes
    FLT ior;
    Media3D* media;	
    
public: // Constructors and destructor

    /**Construct default interior.
      */
    Interior(): ior(1), media(0) {}

    /**Construct interior by given ior value and media.
      *@param i ior value
      *@param m media
      */
    Interior(FLT i, Media3D* m): ior(i), media(m) {}

    /**Copy constructor.
      */
    Interior(const Interior &i): ior(i.ior) {
        if (i.media) media = i.media->copy(); else media = 0;
    }

    ~Interior() {
        delete(media);
    }

private:
    Interior& operator=(const Interior&);

public: // Public methods

    /**Set interior fields by given Interior if they are not already set.
      */
    void overlay(const Interior &i) {
        if ( !media && i.media ) media = i.media->copy();
    }

};

#endif
