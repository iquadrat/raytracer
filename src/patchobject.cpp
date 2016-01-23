/* *************************************************************************
                          patchobject.cpp  -  description
                             -------------------
    begin                : Sun Jan 19 2003
    copyright            : (C) 2003 by Micha Riser
    email                : mriser@gmx.net
    
    $Id: patchobject.cpp,v 1.12 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/patchobject.h"

#include "../src/intersectionheap.h"
#include "../src/trianglepatch.h"

class PatchObjectIntersection: public Intersection {

public: // Constructors and destructor
    PatchObjectIntersection(const PatchObject* p, const Ray& r, DBL maxtime):
      Intersection(0, r), parent(p), tmax(maxtime) {}

    ~PatchObjectIntersection() {}

public: // Public methods
    // Inherited from Intersection
    DBL currentTime() {return iheap.time();}
    void next();
    Vector3 normal();
    Texture* texture();
    // ...
    void put(Intersection* i, DBL time, SurfacePatch3D* o) {
        iheap.put(i,time,o);
    }
    bool init() { // check me
        iheap.build();
        if (checkHead() == false) next();
        if (iheap.time() == INTERSECTION_INFINIT_TIME) {
            return false;
        } else {
            hitobject = parent;
            return true;
        }
    }
    bool checkHead();

private: // Private attributes
    IntersectionHeap iheap;
    const PatchObject* parent;
    DBL tmax;

};

HitStatistics PatchObject::stat("PatchObject","Tests","Hits");

Object3D* PatchObject::duplicate() const {
    return new PatchObject(*this);
}

void PatchObject::addPatch(SurfacePatch3D* o) {
    if (isclosed || material) { throw ObjectConstructionError("Cannot add object to closed ObjectContainer"); }
    collector_list->push_back(o);
}

Intersection* PatchObject::intersectLocalLimitedTime(const Ray &shoot, DBL tmax) const {

    assert(isclosed);
    stat.eval();
    PatchObjectIntersection* i = 0;

    // fill Array with initial values
    for(unsigned int j=0; j<objects->listsize; j++) {

        SurfacePatch3D* current = objects->list[j];
        
        double t;
        if ( (t=current->intersectBounding(shoot,tmax))!=INTERSECTION_TIME_EPSILON ) {

            if (t > INTERSECTION_TIME_EPSILON) {
                // boundingbox hit
                if (i == 0)  i = new PatchObjectIntersection(this,shoot,tmax);
                i->put(0, t, current);
            }

        } else {

            // there is no boundingbox or we are inside
            Intersection* ci = current->intersectLimitedTime(shoot,tmax);
            if (ci) {
                if (i == 0)  i = new PatchObjectIntersection(this,shoot,tmax);
                i->put(ci, ci->currentTime(), current);
            }

        }

    }

    if (i) {
        // build up structure
        if (i->init() == false) {delete(i); return 0;}
        stat.success();
    }

    return i;

}

bool PatchObject::isInsideAt(const Vector3 &locallocation) const {

//    std::cout << inverse << std::endl;

    // set up ray originating from given location going into some direction
    Ray testray(locallocation,insidevector);

    // count the number of intersections between testray and surface
    Intersection* i = intersectLocal(testray);
    if (i != 0) {
        
        bool inside = //false;
                    inverse;
        do {            
            i->next();            
            // even number of intersection means outside, odd inside
            // if the object has the inverse flag set, it's just the other way round            
            inside = !inside;            
        } while(i->currentTime()!=DBL_INFINITY);

        delete(i);        
        return inside;
        
    } else {

        // never hit the surface so we must be outside
        return //false;
            inverse;
        
    }

}


ObjectContainer* PatchObject::getEmptyContainer() {
    
    return new PatchObject();
    
}


void SharedPointsPatchObject::PatchList::addTriangle(const Vector3& A, const Vector3& B, const Vector3& C) {

    pindex_t iA = insertPoint(A);
    pindex_t iB = insertPoint(B);
    pindex_t iC = insertPoint(C);
    clist.push_back(new TriangleConnectivity(iA,iB,iC));
    
}

SharedPointsPatchObject::PatchList::~PatchList() {
    for(unsigned int i=0; i<clist.size(); ++i) {
        delete(clist[i]);
    }
}


SharedPointsPatchObject::pindex_t SharedPointsPatchObject::PatchList::insertPoint(const Vector3& v) {

    // look up v
    pmap_t::iterator i = pmap.find(v);

    if (i == pmap.end()) {
        // not found, add it
        pindex_t newindex = pmap.size();
        pmap[v] = newindex;
        return newindex;
    } else {
        // found, return index
        return i->second;
    }

}

SharedPointsPatchObject::TriangleConnectivity::TriangleConnectivity(pindex_t A, pindex_t B, pindex_t C) {
    edge[0]=A; edge[1]=B; edge[2]=C;
}

SurfacePatch3D* SharedPointsPatchObject::TriangleConnectivity::makePatch(Vector3* l) const {
    return new TrianglePatch(&(l[edge[0]]),&(l[edge[1]]),&(l[edge[2]]));
}

SharedPointsPatchObject::SharedPointsPatchObject(const PatchList& l) {

    pointlist = new Vector3[l.pmap.size()];
    pointlistcount = new CNTREF(0);
    std::cout << "size: "<< l.pmap.size()<<std::endl;
    PatchList::pmap_t::const_iterator i = l.pmap.begin();
    for(;i!=l.pmap.end(); ++i) {
        pointlist[i->second] = i->first;
    }
    for(pindex_t i = 0; i<l.clist.size(); i++) {
        addPatch(l.clist[i]->makePatch(pointlist));
    }
    

}

SharedPointsPatchObject::~SharedPointsPatchObject() {

    *pointlistcount -= 1;
    if (*pointlistcount == 0) {
        delete(pointlist);
        delete(pointlistcount);
    }

}


SharedPointsPatchObject::SharedPointsPatchObject(const SharedPointsPatchObject& o):
  PatchObject(o)
{
    pointlist = o.pointlist;
    pointlistcount = o.pointlistcount;
    *pointlistcount += 1;
}


Object3D* SharedPointsPatchObject::duplicate() const {
    assert(isclosed);
    return new SharedPointsPatchObject(*this);
}


Vector3 PatchObjectIntersection::normal() {

    Vector3 local = iheap.intersection()->normal();
    return local;

}


Texture* PatchObjectIntersection::texture() {

    return Intersection::texture();

}



bool PatchObjectIntersection::checkHead() {

    if (iheap.intersection() == 0) return false;
    return true;

}


void PatchObjectIntersection::next() {

    do{

        if (iheap.intersection() == 0) {

                // we had only the boundingbox intersection with this object so far
                Intersection* ci = iheap.object()->intersectLimitedTime(ray,tmax);
                if (ci) {
                    DBL t = ci->currentTime();
                    iheap.intersection() = ci;
                    iheap.time() = t;
                } else iheap.time() = INTERSECTION_INFINIT_TIME;

            } else {
                iheap.intersection()->next();
                iheap.time() = iheap.intersection()->currentTime();
            }

            iheap.reassureHeap();

            if (iheap.time() == INTERSECTION_INFINIT_TIME) return;

        } while (checkHead()==false);

}
