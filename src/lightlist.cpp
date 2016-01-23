/* *************************************************************************
                         lightlist.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: lightlist.cpp,v 1.3 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/lightlist.h"

LightList* LightList::merge(LightList* first, const LightList* second, CLR weight) {

    // precondition = postcondition: light sources are ordered in descending order
    // kinda ugly this code *fix me*

    if (second == 0) return first;

    if (first == 0) {
        
        // Copy second and scale it by weight
        LightList* ret = new LightList(second->colour, second->light, 0);
        ret->colour.scale(weight);
        LightList* rethead = ret;
        second = second->next;
        
        while(second != 0) {

            ret->next = new LightList(second->colour, second->light, 0);
            ret = ret->next;
            ret->colour.scale(weight);
            second = second->next;
            
        }
        return rethead;

    } else {

        // make new list head
        
        LightList *rethead;
        if (first->light == second->light) {        
            first->colour.add(second->colour.scaled(weight));
            second = second->next;
            rethead = first;
        } else {
            if (!(first->light < second->light)) {
                rethead = first;
            } else {
                rethead = new LightList(*second);
                rethead->colour.scale(weight);
                second = second->next;
                rethead->next = first;
                first = rethead;
            }
        }
        
        // Do a merge of the two lists (like merge-sort)

        while(second != 0) {

            if (first->next == 0) {

                // copy rest of second list at end of cur
                while(second != 0) {

                    first->next = new LightList(second->colour, second->light, 0);
                    first = first->next;
                    first->colour.scale(weight);
                    second = second->next;

                }
                
                break;

            }

            while( !(first->next->light <= second->light) ) first = first->next;

            if (first->next->light == second->light) {
                first = first->next;
                // add weighted colour
                first->colour.add(second->colour.scaled(weight));

            }  else {

                first->next = new LightList(second->colour,second->light,first->next);
                first = first->next;
                first->colour.scale(weight);

            }

            second = second->next;

        }

        return rethead;

    }

}


CLR LightList::calculateLightListDifference(const LightList& l1, const LightList& l2) {

    const LightList* p1 = &l1;
    const LightList* p2 = &l2;
    CLR diff = 0;

    while( p1 != 0 && p2 != 0 ) {

        if (p1->light == p2->light) {

            diff += (p1->colour - p2->colour).norm();
            p1 = p1->next;
            p2 = p2->next;

        } else if (p1->light < p2->light) {

            // p2 bigger
            diff += p2->colour.norm();
            p2 = p2->next;
        
        } else {

            // p1 bigger
            diff += p1->colour.norm();
            p1 = p1->next;

        }

    }

    while(p1 != 0) {
        diff += p1->colour.norm();
        p1 = p1->next;
    }
    while(p2 != 0) {
        diff += p2->colour.norm();
        p2 = p2->next;    
    }

    return diff;

}
