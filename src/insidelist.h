/* *************************************************************************
                          insidelist.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002       
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: insidelist.h,v 1.6 2003/03/03 11:44:05 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */


#ifndef INSIDELIST_H
#define INSIDELIST_H

#include<list>

#include "../src/types.h"

class Object3D;
class Media3D;

/**List to state which objects a point is inside.
  *@author Micha Riser
  */

class InsideList {

public: // Public attributes
    std::list<const Object3D*> objectlist; // public?? -- better not
    FLT ior;

public: // Constructors and destructor

    /**Create empty inside list.
      */
    InsideList(): objectlist(), ior(1) {}

    /**Copy constructor.
      */
    InsideList(const InsideList& l): objectlist(l.objectlist), ior(l.ior) {};

public: // Public methods
    /**Add object to list.
      */
    void add(const Object3D* o);

    /**Swap object on list.
      *@returns fraction index
      */    
    FLT swap(const Object3D* o);

    /**
      *@returns media of all objects in the list
      */
    Media3D* getMedia();

private: // Private methods

    /**Remove object from list.
      */
    bool remove(const Object3D* o);

};

#endif
