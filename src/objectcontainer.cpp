/* *************************************************************************
                    objectcontainer.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002       
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: objectcontainer.cpp,v 1.17 2003/05/25 20:13:33 micha Exp $

 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/objectcontainer.h"

#include <cassert>
#include <algorithm>
 
#include "../src/material3d.h"
#include "../src/objectconstructionerror.h"

long ObjectContainer::ObjectContainer_count = 0;
long ObjectContainer::ObjectContainer_list_size[MAX_MEMBERS_COUNT];

ObjectContainer::ObjectContainer(): Object3D(),
objects(new ObjectList()), collector_list(new spvector()),
isclosed(false), isprepared(false) {
      
    if (++ObjectContainer_count == 1)
        for(unsigned int i=0; i<MAX_MEMBERS_COUNT; i++) ObjectContainer_list_size[i] = 0;
        
}


ObjectContainer::ObjectContainer(const ObjectContainer& c):
Object3D(c), collector_list(0), isclosed(true), isprepared(false) {

    // copy object list by reference
    objects = c.objects;
    // increase list reference count
    objects->count++;
//    std::cout << objects << " count now (copied) "<< objects->count<<"\n";

    // note: bounding box get's copied and is OK for the new object

}

ObjectContainer::ObjectContainer(const Object3D& o, ObjectList* l):
Object3D(o), objects(l), collector_list(0), isclosed(true), isprepared(false) {

    l->count++;

    // bounding box may need to be updated, so recalculate it
    calculateBoundingBox();

}

ObjectContainer::~ObjectContainer() {

    assert(isclosed==true);

/*    for(int i=0; i<MAX_MEMBERS_COUNT; i++) {

             cout << "list size " << i <<": count "<<ObjectContainer_list_size[i]<<std::endl;
    }*/

    // decrease list reference count
    objects->count--;
//    std::cout << objects << " count now (destr) "<< objects->count<<"\n";    
    // check if list is still needed
    if ( (objects->count == 0) || (objects->inverse!=0 && objects->count==1 && objects->inverse->count==1) ) {
        delete(objects);
    }

}

void ObjectContainer::close() {

    if (isclosed == false) {

        isclosed = true;    

        // splitting..
//      cout << "list size: "<<collector_list->size()<<std::endl;
        if (collector_list->size()>MIN_MEMBERS_COUNT) split();

        // ..merging
        // *to be implemented*

        if (collector_list->size()>MAX_MEMBERS_COUNT) {
            std::cout <<" OOOOOOOOPPPS, too many objects\n";
        }
        assert(collector_list->size()<=MAX_MEMBERS_COUNT);

        // transfrom object list into array
        std::copy( collector_list->begin(), collector_list->end(), objects->list );
        objects->listsize = collector_list->size();
        delete(collector_list);
        collector_list = 0;

        calculateBoundingBox();
//        ObjectContainer_list_size[listsize]++;                        

    }

    assert(isclosed==true);
    assert(objects!=0);
    assert(collector_list==0);

}

void ObjectContainer::calculateBoundingBox() {

    assert(isclosed==true);

    // reset object's bb properties    
    delete(boundingbox);
    boundingbox = 0;
    inverse = false;
    
    Vector3 max = Vector3::negativeInfinit();
    Vector3 min = Vector3::positiveInfinit();

    for(CNTREF i=0; i<objects->listsize; ++i) {

        SurfacePatch3D* current = objects->list[i];
        
        // if one member has infinit bounding no bb can be calculated
        if (!current->hasFinitBounding()) return;

        // transform bb into ObjectContainer's space        
        BoundingBox checkbox(current->getBoundingMin(),current->getBoundingMax());
        checkbox.transformate(*this);

        // extend min/max if neccessary
        min.minimize( checkbox.getBoxMin() );
        max.maximize( checkbox.getBoxMax() );

    }

    // *add me* test for degenerated bb

    if ((!min.equals(Vector3::negativeInfinit()))&&(!max.equals(Vector3::positiveInfinit()))) {
        boundingbox = new BoundingBox(min,max);
    } 

}


void ObjectContainer::prepareToRender() {

    assert(isclosed);
    if (isprepared) return;
    isprepared=true;
    std::cout << "+";

    if (objects->inverse != 0) {
        objects->inverse->count--;
        if (objects->inverse->count == 0) {
            delete(objects->inverse);            
        }
        objects->inverse = 0;
    }

    for(CNTREF i=0; i<objects->listsize; i++) {
        objects->list[i]->prepareToRender();
    }

} 


ObjectContainer::ObjectList::ObjectList():
inverse(0), count(1),  listsize(0) {

//    std::cout << this << " created\n";
            
}

ObjectContainer::ObjectList::~ObjectList() {

//    std::cout << this << " destructor called \n";    
    count = -1;
    if (inverse) {
        inverse->count--;
        if (inverse->count == 0) {delete(inverse); inverse=0;}
    }

    for(unsigned int i=0; i<listsize; i++) delete(list[i]);

}

bool ObjectContainer::xAxisLess(const SurfacePatch3D* o1, const SurfacePatch3D* o2) {
    return o1->getBoundingMin()[0]<o2->getBoundingMin()[0];
/*    return (o1->boundingbox->getBoxMax().a[0]+o1->boundingbox->getBoxMin().a[0])
        <
           (o2->boundingbox->getBoxMax().a[0]+o2->boundingbox->getBoxMin().a[0]);
*/
}

bool ObjectContainer::yAxisLess(const SurfacePatch3D* o1, const SurfacePatch3D* o2) {
    return o1->getBoundingMin()[1]<o2->getBoundingMin()[1];
/*    return (o1->boundingbox->getBoxMax().a[1]+o1->boundingbox->getBoxMin().a[1])
        <
           (o2->boundingbox->getBoxMax().a[1]+o2->boundingbox->getBoxMin().a[1]);
*/
}

bool ObjectContainer::zAxisLess(const SurfacePatch3D* o1, const SurfacePatch3D* o2) {
    return o1->getBoundingMin()[2]<o2->getBoundingMin()[2];
/*    return (o1->boundingbox->getBoxMax().a[2]+o1->boundingbox->getBoxMin().a[2])
        <
           (o2->boundingbox->getBoxMax().a[2]+o2->boundingbox->getBoxMin().a[2]);
*/
}

DBL ObjectContainer::calcArea(const Vector3& v) {
    return v[0]*(v[1]+v[2])+v[1]*v[2];
}

void ObjectContainer::split() {

//    if (collector_list->size() > 1000)
        std::cout << "start splitting (size "<<collector_list->size()<<")\n";

    // separate objects without bounding box
    // *add me*: check for too many object w/o bb
    spvector nobblist, bblist;
    spvector::iterator it = collector_list->begin();
    for(; it != collector_list->end(); ++it) {
        if ( !(*it)->hasFinitBounding() ) {
            nobblist.push_back(*it);
        } else {
            bblist.push_back(*it);
        }
    }

    spvector *best_sorted = 0;
    spvector::iterator best_split_position;    
    DBL best_split_axis = -1;
    DBL best_split_area = DBL_INFINITY;
    unsigned int size = bblist.size();

    // bb_area_from_right[i] stores area from bb enclosing objects i..n
    DBL* bb_area_from_right = new DBL[size];
    
    // find best splitting
    for(int test_axis = 0; test_axis<3; test_axis++) {

        spvector *sorted = new spvector(bblist);
        switch(test_axis) {
            case 0:
                std::sort(sorted->begin(),sorted->end(),xAxisLess);
                break;
            case 1:
                std::sort(sorted->begin(),sorted->end(),yAxisLess);
                break;
            case 2:
                std::sort(sorted->begin(),sorted->end(),zAxisLess);
                break;
            default:
                // should never happen
                assert(false);
        }

        // build bb_area_from_right table
        Vector3 max = Vector3::negativeInfinit();
        Vector3 min = Vector3::positiveInfinit();

        unsigned int i = size-1;
        for(spvector::reverse_iterator it = sorted->rbegin();
          it != sorted->rend(); ++it, --i) {

            min.minimize( (*it)->getBoundingMin() );
            max.maximize( (*it)->getBoundingMax() );
            Vector3 range = Vector3::sub(max,min);
            bb_area_from_right[i] = calcArea(range);

        }

        // search linear for best split position
        max = Vector3::negativeInfinit();
        min = Vector3::positiveInfinit();

        unsigned int j = 0;
        for(spvector::iterator it = sorted->begin(); j<size-1; ++it, ++j) {

            min.minimize( (*it)->getBoundingMin() );
            max.maximize( (*it)->getBoundingMax() );
            DBL bb_area_from_left = calcArea( Vector3::sub(max,min) );
            DBL test_area = bb_area_from_left*(j+1+1) + bb_area_from_right[j+1]*(size-j-1+1);
//            std::cout << j<<": test area = "<<test_area<<std::endl;
            if (test_area < best_split_area) {
                if (test_axis != best_split_axis) {
                    delete(best_sorted);              
                    best_sorted = sorted;
                }
                best_split_axis = test_axis;
                best_split_position = it;
                best_split_area = test_area;
//                std::cout << "split bei "<<test_axis<<"/"<<j<<" -> area: "<<test_area<<std::endl;
            }

        }

    }

    DBL current_area = bb_area_from_right[0]*size;
    
    delete[] bb_area_from_right;
    bb_area_from_right = 0;

//    std::cout << "current area: "<<current_area<<std::endl;

    if (collector_list->size()>MAX_MEMBERS_COUNT || best_split_area < current_area)  {

        if (collector_list->size() > 100000)
            std::cout << "Doing split"<<std::endl;

        // split list into two new ObjectContainers
        ObjectContainer* left = getEmptyContainer();
        left->collector_list->assign(best_sorted->begin(),++best_split_position);
        left->close();

        ObjectContainer* right = getEmptyContainer();
        right->collector_list->assign(best_split_position,best_sorted->end());
        right->close();
                
        // original ObjectContainer contains all finite objects and the
        // two new ObjectContainers
        *collector_list = nobblist;
        collector_list->push_back(left);
        collector_list->push_back(right);

    }

    delete(best_sorted);

}
