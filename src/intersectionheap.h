/* *************************************************************************
                          intersectionheap.h  -  description
                             -------------------
    begin                : Thu Feb 6 2003
    copyright            : (C) 2003 by Micha Riser
    email                : micha@povworld.org
    
    $Id: intersectionheap.h,v 1.4 2005/02/04 19:23:37 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef INTERSECTIONHEAP_H
#define INTERSECTIONHEAP_H

#include <cassert>
#include "../src/objectcontainer.h"

/**	
  *@author Micha Riser
  */

class IntersectionHeap {

public:
    IntersectionHeap(): size(0) {};

    ~IntersectionHeap() {
        for(unsigned int i=1; i<=size; i++) delete(heap[i].intersection);
    }    

private:
    class Node {
    public: // Public attributes
        Intersection* intersection;
        DBL time;
        SurfacePatch3D* object;

    public: // Constructors
        Node() {}
        Node(Intersection* i, DBL t, SurfacePatch3D* o):
          intersection(i), time(t), object(o) {}
          
    };

public:
    void put(Intersection* i, DBL time_, SurfacePatch3D* o) {
        assert(size<ObjectContainer::MAX_MEMBERS_COUNT);
        heap[++size] = Node(i,time_,o);
    }

    Intersection*& intersection() {return heap[1].intersection;}
    DBL& time() {return heap[1].time;}
    SurfacePatch3D* object() {return heap[1].object;}
    SurfacePatch3D* object(int i) {return heap[i].object;}
    int getSize() {return size;}
    
    void reassureHeap();
    void build();
    
private:
    Node heap[ObjectContainer::MAX_MEMBERS_COUNT+1];
    unsigned int size;

};

#endif
