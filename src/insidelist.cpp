/* *************************************************************************
                          insidelist.cpp  -  description
                             -------------------
    begin                : Wed Oct 15 2002
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: insidelist.cpp,v 1.6 2003/03/03 11:44:05 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/insidelist.h"

#include<iostream>
#include "../src/material3d.h"
#include "../src/media3d.h"
#include "../src/object3d.h"

void InsideList::add(const Object3D* o) {
    
    objectlist.push_back(o);
    if (o->material->interior->ior != ior) {
        if (ior != 1) {
            std::cout << "Warning! Multiple iors defined";
        } else {
            ior = o->material->interior->ior;
        }
    }
    
}


bool InsideList::remove(const Object3D* o) {
    
    std::list<const Object3D*>::iterator it = objectlist.begin();
    for(; it!=objectlist.end(); ++it) {
        if (*it == o) {
            objectlist.erase(it);
            return true;
        }
    }
    return false;
    
}

FLT InsideList::swap(const Object3D* o) {

    if (remove(o)) {
        // leaving object
        if (o->material->interior->ior != ior) 
            std::cout << "Warning! Ray leaves at different ior than entered!";
        FLT tmp = ior;
        ior = 1;
        return tmp;
    } else {
        if (o->material == 0 || o->material->interior == 0) return 1;
        // entering object
        objectlist.push_back(o);
        ior = o->material->interior->ior;
        return 1.0/ior;
    }
    
}       

Media3D* InsideList::getMedia() {
    
    std::list<const Object3D*>::iterator it = objectlist.begin();
    
    Media3DMultiple* ret=0;
    int count;
    
    for(; it!=objectlist.end(); ++it) {
        if ((*it)->material->interior->media !=0) {
            //return (*it)->material->interior->media;
            if (!ret) {
                ret = new Media3DMultiple( (*it)->material->interior->media->copy() );
            } else {
                ret->addMedia( (*it)->material->interior->media->copy() );
            }
            count++;
        }
    }
    return ret;

}
