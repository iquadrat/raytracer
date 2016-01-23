/* *************************************************************************
                          intersectionheap.cpp  -  description
                             -------------------
    begin                : Thu Feb 6 2003
    copyright            : (C) 2003 by Micha Riser
    email                : micha@povworld.org
    
    $Id: intersectionheap.cpp,v 1.2 2003/02/21 22:19:20 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/intersectionheap.h"

void IntersectionHeap::reassureHeap() {

    // re-assure heap condition
    unsigned int j=1;
    unsigned int c=2;
    while( c <= size ) {
        if (c!=size && heap[c+1].time<heap[c].time) c++;
        if (heap[j].time<=heap[c].time) break;
        std::swap(heap[j],heap[c]);
        j = c;
        c *= 2;
    }

}


void IntersectionHeap::build() {

    for(unsigned int i=1; i<=size;i++) {

        unsigned int  k=i;
        while( (k/2>0) && (heap[k/2].time>heap[k].time) ) {
            std::swap(heap[k/2],heap[k]);
            k = k/2;
        }
         
    }

}
