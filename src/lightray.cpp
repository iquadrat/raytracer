/* *************************************************************************
                          lightray.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: lightray.cpp,v 1.17 2005/02/04 19:23:37 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/lightray.h"

#include "../src/scene.h"

Colour LightRay::shoot() {

    // check for degenerated light ray
    if (length == 0) return colour;

    // shoot ray
    Colour retcolour = colour;
    Colour absorption = Colour(0);
    bool hasmedia = false;
    DBL lasttime = 0;
    CLR transmitamouth = 1;       
     
    Intersection* li = myscene->object.intersectLimitedTime(*this,length);

    if (li != 0) {

        DBL time = li->currentTime();
        do {

            Media3D* media = inside.getMedia();
            if (media) {
                              
                media->sample(lasttime,time,*this,absorption);
                hasmedia = true;
                delete(media);
                 
            }
  
            Texture* thisma = li->texture();
            transmitamouth *= thisma->colour->alpha();
            delete(thisma);      
            if (transmitamouth == 0) { // *add* close to 0 can abort as well
                delete(li);
                return Colour(0);
            }
            inside.swap(dynamic_cast<const Object3D*>(li->hitObject()));
            li->next();
            lasttime = time;
            time = li->currentTime();
         
        } while (time != INTERSECTION_INFINIT_TIME);

        delete(li);
    }

    // consider last interval from lasttime to
    // the lightsource (at time=length)
    Media3D* media = inside.getMedia();
    if (media) {
        
        media->sample(lasttime,length,*this,absorption);
	hasmedia = true;
   
    }
    if (hasmedia) {

        // Multiply colour with absorption
        absorption.negExp();
        retcolour.scale(absorption);
        delete(media);
        
    }     

    return retcolour.scaled(transmitamouth);

}
