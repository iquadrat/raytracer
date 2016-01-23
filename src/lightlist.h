/* *************************************************************************
                          lightlist.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: lightlist.h,v 1.4 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef LIGHTLIST_H
#define LIGHTLIST_H

#include <cassert>
#include "../src/colour.h"

class LightSource;

/**List of lights shining to a certain point. Returned by LightCache lookups.
  *@author Micha Riser
  */  
class LightList {

public: // Constructor and destructor
    LightList(const Colour& c, LightSource* l, LightList* n):
      colour(c), light(l), next(n) {
          
        assert( next==0 || light>next->light );
        
    }

    ~LightList() {delete(next);}

private:
    LightList(const LightList& l):colour(l.colour), light(l.light), next(l.next) {};
    LightList& operator=(const LightList&);
                                    
public: // Public attributes

    /* Store "higher" light sources at the front of the list, so
     * invariant is: light > next->light
     * *change me* make those things private and provide an iterator through it
     */
    Colour colour;
    LightSource* light;
    LightList* next;

public: // Static methods
    /**Add the lights from the second light list to the first one. The lights'
      *colours of the second list are weightend by weight before adding to the
      *first one.
      *@param first First light list. May be 0.
      *@param second Second light list. May be 0.
      */
    static LightList* merge(LightList* first, const LightList* second, CLR weight);

    /**Calculates a scalar value that relates to how different two light lists are.
      */
    static CLR calculateLightListDifference(const LightList& l1, const LightList& l2);
    
};

#endif
