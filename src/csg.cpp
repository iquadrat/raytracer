/* *************************************************************************
                          csg.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002       
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: csg.cpp,v 1.38 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/csg.h"

#include<cassert>
 
#include "../src/intersectionheap.h"
#include "../src/material3d.h"
#include "../src/object3d.h"
#include "../src/texture3d.h"


HitStatistics CSGUnion::stat("CSGUnion","Tests","Hits");
HitStatistics CSGMerge::stat("CSGMerge","Tests","Hits");
HitStatistics CSGIntersect::stat("CSGIntersection","Tests","Hits");


class CSGUnionIntersection: public Intersection {

    friend class CSGUnion;

public: // Constructors and destructor
    CSGUnionIntersection(const CSGUnion* p, const Ray& r, DBL maxtime):
      Intersection(0, r), iheap(), parent(p), tmax(maxtime) {}

    ~CSGUnionIntersection() {}

private:
    CSGUnionIntersection(const CSGUnionIntersection&);
    CSGUnionIntersection& operator=(const CSGUnionIntersection&);

public: // Public methods
    // Inherited from Intersection
    DBL currentTime() {return iheap.time();}
    void next();
    Vector3 normal();
    Texture* texture();

private: // private methods
    void put(Intersection* i, DBL time, SurfacePatch3D* o) {
        iheap.put(i,time,o);
    }
    bool init() {
        iheap.build();

        if (checkHead() == false) next();
        if (iheap.time() == INTERSECTION_INFINIT_TIME) {
            return false;
        } else {
            hitobject = iheap.intersection()->hitObject();
            return true;
        }
    }
    bool checkHead();

private: // Private attributes
    IntersectionHeap iheap;
    const CSGUnion* parent;
    DBL tmax;

};


class CSGMergeIntersection: public Intersection {

    friend class CSGMerge;

public: // Constructors and destructor
    CSGMergeIntersection(const CSGMerge* p, const Ray& r, DBL maxtime):
      Intersection(0, r), iheap(), parent(p), tmax(maxtime) {}

    ~CSGMergeIntersection() {}
    
private:
    CSGMergeIntersection(const CSGMergeIntersection&);
    CSGMergeIntersection& operator=(const CSGMergeIntersection&);    

public: // Public methods
    // Inherited from Intersection
    DBL currentTime() {return iheap.time();}
    void next();
    Vector3 normal();
    Texture* texture();

private: // Private methods
    void put(Intersection* i, DBL time, SurfacePatch3D* o) {
        iheap.put(i,time,o);
    }
    bool init() {
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
    const CSGMerge* parent;
    DBL tmax;

};


class CSGIntersectIntersection: public Intersection {

    friend class CSGIntersect;

public: // Constructors and destructor
    CSGIntersectIntersection(const CSGIntersect* p, const Ray& r, DBL maxtime):
      Intersection(0, r), iheap(), parent(p), tmax(maxtime) {}

    ~CSGIntersectIntersection() {}
    
private:
    CSGIntersectIntersection(const CSGIntersectIntersection&);
    CSGIntersectIntersection& operator=(const CSGIntersectIntersection&);

public: // Public methods
    // Inherited from Intersection
    DBL currentTime() {return iheap.time();}
    void next();
    Vector3 normal();
    Texture* texture();

private: // Private methods
    void put(Intersection* i, DBL time, SurfacePatch3D* o) {
        iheap.put(i,time,o);
    }
    bool init() {
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
    const CSGIntersect* parent;
    DBL tmax;

};


void CSGUnion::addObject(Object3D* o) {
    if (isclosed || material) { throw ObjectConstructionError("Cannot add object to closed ObjectContainer"); }
    collector_list->push_back(o);
}

void CSGMerge::addObject(Object3D* o) {
    if (isclosed || material) { throw ObjectConstructionError("Cannot add object to closed ObjectContainer"); }
    collector_list->push_back(o);
}

void CSGIntersect::addObject(Object3D* o) {
    if (isclosed || material) { throw ObjectConstructionError("Cannot add object to closed ObjectContainer"); }
    collector_list->push_back(o);
}


Object3D* CSGUnion::invert() const {

    assert(isclosed==true);
    assert(collector_list==0);

    if (objects->inverse) {

        return new CSGIntersect(*this,objects->inverse);

    } else {

        ObjectList* newinverse = new ObjectList();
        newinverse->inverse = objects;
        objects->inverse = newinverse;
        newinverse->count = 1;   
        objects->count++; 
        for(unsigned int i=0; i<objects->listsize; i++) {
            Object3D* dupl = static_cast<Object3D*>(objects->list[i])->invert();
            newinverse->list[i] = dupl;
        }
        newinverse->listsize=objects->listsize;
        
        return new CSGIntersect(*this,newinverse);

    }

}

Object3D* CSGMerge::invert() const {

    assert(isclosed==true);
    assert(collector_list==0);

    if (objects->inverse) {

        return new CSGIntersect(*this,objects->inverse);

    } else {

        ObjectList* newinverse = new ObjectList();
        newinverse->inverse = objects;
        objects->inverse = newinverse;
        newinverse->count = 1;
        objects->count++;
        for(unsigned int i=0; i<objects->listsize; i++) {
            Object3D* dupl = static_cast<Object3D*>(objects->list[i])->invert();
            newinverse->list[i] = dupl;
        }
        newinverse->listsize=objects->listsize;
       
        return new CSGIntersect(*this,newinverse);

    }

}

Object3D* CSGIntersect::invert() const {

    assert(isclosed==true);
    assert(collector_list==0);

    if (objects->inverse) {

        return new CSGMerge(*this,objects->inverse);

    } else {

        ObjectList* newinverse = new ObjectList();
        newinverse->inverse = objects;
        objects->inverse = newinverse;
        newinverse->count = 1;
        objects->count++;
        for(unsigned int i=0; i<objects->listsize; i++) {
            Object3D* dupl = static_cast<Object3D*>(objects->list[i])->invert();
            newinverse->list[i] = dupl;;
        }

        return new CSGMerge(*this,newinverse);

    }

}



void CSGUnion::listInside(const Vector3& location, InsideList& l) const {
    
    if (boundingbox && !boundingbox->isInsideAt(location) && !isInverse()) return; // optimal?
    Vector3 local = location;
    transformPointIn(local);
    for(unsigned int i=0; i<objects->listsize; i++) {
        static_cast<const Object3D*>(objects->list[i])->listInside(local,l);
    }

}


Intersection* CSGUnion::intersectLocalLimitedTime(const Ray &shoot, DBL tmax) const {

    assert(isclosed);
    stat.eval();
    CSGUnionIntersection* i = 0;

    // fill Array with initial values
    for(unsigned int j=0; j<objects->listsize; j++) {

        Object3D* current = static_cast<Object3D*>(objects->list[j]);

        double t;
        if ( (t=current->intersectBounding(shoot,tmax))!=INTERSECTION_TIME_EPSILON ) {
            
            if (t > INTERSECTION_TIME_EPSILON) {
                // boundingbox hit
                if (i == 0)  i = new CSGUnionIntersection(this,shoot,tmax);
                i->put(0,t,current);
            } 

        } else {
            
            // there is no boundingbox or we are inside
            Intersection* ci = current->intersectLimitedTime(shoot,tmax);
            
            if (ci) {
                if (i == 0)  i = new CSGUnionIntersection(this,shoot,tmax);
                i->put(ci,ci->currentTime(),current);
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

Intersection* CSGMerge::intersectLocalLimitedTime(const Ray &shoot, DBL tmax) const {

    assert(isclosed);    
    stat.eval();
    CSGMergeIntersection* i = 0;

    // fill Array with initial values
    for(unsigned int j=0; j<objects->listsize; j++) {

        Object3D* current = static_cast<Object3D*>(objects->list[j]);
        
        double t;
        if ( (t=current->intersectBounding(shoot,tmax))!=INTERSECTION_TIME_EPSILON ) {

            if (t > INTERSECTION_TIME_EPSILON) {
                // boundingbox hit
                if (i == 0)  i = new CSGMergeIntersection(this,shoot,tmax);
                i->put(0,t,current);
                continue;
            }

        } else {

            // there is no boundingbox or we are inside
            Intersection* ci = current->intersectLimitedTime(shoot,tmax);

            if (ci) {
                if (i == 0)  i = new CSGMergeIntersection(this,shoot,tmax);
                i->put(ci,ci->currentTime(),current);
                continue;
            }

        }

        // no intersection with that object, test if we are inside
        Vector3 testpoint = Vector3::add( shoot.getOrigin(), shoot.getDirection() );
        if (static_cast<const Object3D*>(current)->isInsideAtBounded(testpoint)==true) {delete(i); return 0;}

    }

    if (i) {
        // build up structure 
        if (i->init()==false) {delete(i); return 0;}
        stat.success();
    }

    return i;

}


Intersection* CSGIntersect::intersectLocalLimitedTime(const Ray &shoot, DBL tmax) const {

    assert(isclosed);
    stat.eval();
    CSGIntersectIntersection* i = 0;

    // fill Array with initial values
    for(unsigned int j=0; j<objects->listsize; j++) {

        Object3D* current = static_cast<Object3D*>(objects->list[j]);
        
        double t;
        if ( (t=current->intersectBounding(shoot,tmax))!=INTERSECTION_TIME_EPSILON ) {

            if (t > INTERSECTION_TIME_EPSILON) {
                // boundingbox hit
                if (i == 0)  i = new CSGIntersectIntersection(this,shoot,tmax);
                i->put(0,t,current);
                continue;
            }

        } else {

            // there is no boundingbox or we are inside
            Intersection* ci = current->intersectLimitedTime(shoot,tmax);

            if (ci) {
                if (i == 0)  i = new CSGIntersectIntersection(this,shoot,tmax);
                i->put(ci,ci->currentTime(),current);
                continue;
            }

        }

        // no intersection with that object, test if we are inside
        Vector3 testpoint = Vector3::add( shoot.getOrigin(), shoot.getDirection() );
        if (static_cast<const Object3D*>(current)->isInsideAtBounded(testpoint)==false) {delete(i); return 0;}
        
    }

    if (i) {
        // build up structure 
        if (i->init() == false) {delete(i); return 0;}
        stat.success();
    }
    
    return i;

}

bool CSGUnion::isInsideAt(const Vector3 &locallocation) const {

    for(unsigned int i=0; i<objects->listsize; i++) 
        if ( static_cast<const Object3D*>(objects->list[i])->isInsideAtBounded(locallocation) ) return true;

    return false;
                                              
}

bool CSGMerge::isInsideAt(const Vector3 &locallocation) const {

    for(unsigned int i=0; i<objects->listsize; i++)
        if ( static_cast<const Object3D*>(objects->list[i])->isInsideAtBounded(locallocation) ) return true;

    return false;

}

bool CSGIntersect::isInsideAt(const Vector3 &locallocation) const {

    for(unsigned int i=0; i<objects->listsize; i++)
        if ( !static_cast<const Object3D*>(objects->list[i])->isInsideAtBounded(locallocation) ) return false;

    return true;

}

void CSGIntersect::calculateBoundingBox() {

    assert(isclosed==true);

    // reset object's bb properties
    delete(boundingbox);
    boundingbox = 0;
    inverse = false;
    
    Vector3 min = Vector3::negativeInfinit();
    Vector3 max = Vector3::positiveInfinit();

    for(unsigned int i=0; i<objects->listsize; ++i) {

        SurfacePatch3D* current = objects->list[i];
        if ( static_cast<Object3D*>(current)->isInverse()==false ) {

            // an infinit bb does not help
            if (!current->hasFinitBounding()) continue;

            // transform bb into ObjectContainer's space
            BoundingBox checkbox(current->getBoundingMin(),current->getBoundingMax());
            checkbox.transformate(*this);

            // narrow min/max if possible
            min.maximize( checkbox.getBoxMin() );
            max.minimize( checkbox.getBoxMax() );
            
        }
        
    }
    
    // check for degenerated boundingbox
    // * add me *

    if  ( min.equals(Vector3::negativeInfinit()) && max.equals(Vector3::positiveInfinit()) ) {

        // if all members are inversed we can still try to draw a bb 
        // around all surfaces and set the container object to be inversed
    
        max = Vector3::negativeInfinit();
        min = Vector3::positiveInfinit();

        for(unsigned int i=0; i<objects->listsize; ++i) {

            SurfacePatch3D* current = objects->list[i];

            // if one member has no finit bb we must give up
            if (!current->hasFinitBounding()) return;

            // transform bb into ObjectContainer's space
            BoundingBox checkbox(current->getBoundingMin(),current->getBoundingMax());
            checkbox.transformate(*this);

            // extend min/max if necessary
            min.minimize( checkbox.getBoxMin() );
            max.maximize( checkbox.getBoxMax() );

        }
        
        inverse = true;
        
    }

    if ((!min.equals(Vector3::negativeInfinit()))&&(!max.equals(Vector3::positiveInfinit()))) {
        boundingbox = new BoundingBox(min,max);
    } 

}


void CSGUnionIntersection::next() {

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
    
    hitobject = iheap.intersection()->hitObject();
    
}

Vector3 CSGUnionIntersection::normal() {

    Vector3 local = iheap.intersection()->normal();

    Object3D* oc = static_cast<Object3D*>( iheap.object() );
    oc->transformNormalOut(local);

    return local;

}


Texture* CSGUnionIntersection::texture() {

    Texture* ret = iheap.intersection()->texture();
    if (ret->colour == 0 || ret->finish == 0) {
        Vector3 p = ray.positionAtTime( iheap.time() );
        if (parent->material && parent->material->texture) {
            Texture* mytex = parent->material->texture->evaluateAt(p);
            if (ret->colour == 0) ret->setColour(mytex->colour);
            if (ret->finish == 0) ret->setFinish(mytex->finish);
            delete(mytex);
        }
    }
    return ret;

}

bool CSGUnionIntersection::checkHead() {

    if (iheap.intersection() == 0)
        return false;
    else
        return true;

}

void CSGMergeIntersection::next() {

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


Vector3 CSGMergeIntersection::normal() {

    Vector3 local = iheap.intersection()->normal();
    static_cast<Object3D*>(iheap.object())->transformNormalOut(local);
    return local;

}


Texture* CSGMergeIntersection::texture() {

    Texture* ret = iheap.intersection()->texture();
    if (ret->colour == 0 || ret->finish == 0) {
        Vector3 p = ray.positionAtTime( iheap.time() );
        if (parent->material && parent->material->texture) {
            Texture* mytex = parent->material->texture->evaluateAt(p);
            if (ret->colour == 0) ret->setColour(mytex->colour);
            if (ret->finish == 0) ret->setFinish(mytex->finish);
            delete(mytex);
        }
    }
    return ret;

}


bool CSGMergeIntersection::checkHead() {

    if (iheap.intersection() == 0) return false;

    Vector3 ipoint = ray.positionAtTime(iheap.time());
    for(int j=2; j<=iheap.getSize(); j++) {
        if ( static_cast<const Object3D*>( iheap.object(j) )->isInsideAtBounded(ipoint) == (true) ) return false;
    }
    return true;

}


void CSGIntersectIntersection::next() {

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


Vector3 CSGIntersectIntersection::normal() {

    Vector3 local = iheap.intersection()->normal();
    static_cast<Object3D*>(iheap.object())->transformNormalOut(local);
    return local;

}


Texture* CSGIntersectIntersection::texture() {

    Texture* ret = iheap.intersection()->texture();
    if (ret->colour == 0 || ret->finish == 0) {
        Vector3 p = ray.positionAtTime( iheap.time() );
        if (parent->material && parent->material->texture) {
            Texture* mytex = parent->material->texture->evaluateAt(p);
            if (ret->colour == 0) ret->setColour(mytex->colour);
            if (ret->finish == 0) ret->setFinish(mytex->finish);
            delete(mytex);
        }
    }
    return ret;

}



bool CSGIntersectIntersection::checkHead() {

    if (iheap.intersection() == 0) return false;

    Vector3 ipoint = ray.positionAtTime(iheap.time());
    for(int j=2; j<=iheap.getSize(); j++) {
        if ( static_cast<const Object3D*>( iheap.object(j) )->isInsideAtBounded(ipoint) == (false) ) return false;
    }

    return true;

}

ObjectContainer* CSGUnion::getEmptyContainer() {
    return new CSGUnion();
}

ObjectContainer* CSGMerge::getEmptyContainer() {
    return new CSGMerge();
}

ObjectContainer* CSGIntersect::getEmptyContainer() {
    return new CSGIntersect();
}


