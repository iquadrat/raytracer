/* *************************************************************************
                            csg.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002   
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: csg.h,v 1.32 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */
 
#ifndef CSG_H
#define CSG_H

#include "../src/objectcontainer.h"

class CSGUnion: public ObjectContainer {
    
public: // Constructors and destructors

    /**Constructs empty CSGUnion container.*/
    CSGUnion(): ObjectContainer() {}
    
    /**Destructor.*/
    ~CSGUnion() {}

private:
    /**Constructs CSGUnion with given ObjectList*/
    CSGUnion(const ObjectContainer& o, ObjectList* l): ObjectContainer(o,l) {
        calculateBoundingBox();
    }

    /**Copy constructor*/
    CSGUnion(const CSGUnion& o): ObjectContainer(o) {}

public: // Public Methods

    // to implement SurfacePatch3D
    Object3D* duplicate() const {return new CSGUnion(*this);}

    // overwrite in Object3D    
    Object3D* invert() const;    
    void listInside(const Vector3& location, InsideList& l) const;
    Intersection* intersectLocalLimitedTime(const Ray &r, DBL maxtime) const;    
    bool isInsideAt(const Vector3 &locallocation) const;    

    /**Add object to ObjectContainer member list.
      */
    void addObject(Object3D* o);
    
protected:
    /**Returns empty CSGUnion container.*/
    ObjectContainer* getEmptyContainer();
    
private:
    /**Intersection statistics.
      */
    static HitStatistics stat;

};


class CSGMerge: public ObjectContainer {

    friend class CSGIntersect;

public: // Constructors and destructors

    /**Constructos empty CSGMerge container.*/
    CSGMerge(): ObjectContainer() {}
    
    /**Destructor.*/
    ~CSGMerge() {}

private:
    /**Constructs CSGMerge with given ObjectList.*/
    CSGMerge(const ObjectContainer& o, ObjectList* l): ObjectContainer(o,l) {
        calculateBoundingBox();
    }

    /**Copy constructor*/
    CSGMerge(const CSGMerge& o): ObjectContainer(o) {}

public: // Public Methods

    // from SurfacePatch3D
    Object3D* duplicate() const {return new CSGMerge(*this);}

    // overwrite in Object3D
    Object3D* invert() const;
    Intersection* intersectLocalLimitedTime(const Ray &r, DBL maxtime) const;    
    bool isInsideAt(const Vector3 &locallocation) const;

    /**Add object to ObjectContainer member list.
      */
    void addObject(Object3D* o);
    
protected:
    ObjectContainer* getEmptyContainer();
    
private:
    /**Intersection statistics.
      */
    static HitStatistics stat;

};


class CSGIntersect: public ObjectContainer {

    friend class CSGUnion;
    friend class CSGMerge;

public: // Constructors and destructors

    /**Constructs empty CSGIntersect container.*/
    CSGIntersect(): ObjectContainer() {}

    /**Destructor.*/
    ~CSGIntersect() {}

private:
    /**Constructs CSGIntersect with given ObjectList*/
    CSGIntersect(const ObjectContainer& o, ObjectList* l): ObjectContainer(o,l) {
        calculateBoundingBox();
    }

    /**Copy constructor*/
    CSGIntersect(const CSGIntersect& o): ObjectContainer(o) {}

public: // Public Methods

    // from SurfacePatch3D
    Object3D* duplicate() const {return new CSGIntersect(*this);}

    // overwrite in Object3D
    Object3D* invert() const;
    Intersection* intersectLocalLimitedTime(const Ray &r, DBL maxtime) const;    
    bool isInsideAt(const Vector3 &locallocation) const;

    /**Add object to ObjectContainer member list.
      */
    void addObject(Object3D* o);

protected:
    // overwrite in ObjectContainer
    void calculateBoundingBox();
    ObjectContainer* getEmptyContainer();    
        
private:
    /**Intersection statistics.
      */
    static HitStatistics stat;

};

#endif



