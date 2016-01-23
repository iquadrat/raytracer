/* *************************************************************************
                          patchobject.h  -  description
                             -------------------
    begin                : Sun Jan 19 2003
    copyright            : (C) 2003 by Micha Riser
    email                : mriser@gmx.net
    
    $Id: patchobject.h,v 1.9 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef PATCHOBJECT_H
#define PATCHOBJECT_H

#include <vector>
//#include <ext/hash_map>
#include <map>

#include "../src/objectcontainer.h"
 
class PatchObject: public ObjectContainer {
    
public: // Constructors and destructors
    PatchObject(): ObjectContainer(), insidevector(-PI,-2,4.1) {};
    PatchObject(const Vector3& insidetestvector): ObjectContainer(), insidevector(insidetestvector) {};
    virtual ~PatchObject() {}
    
protected:
    PatchObject(const PatchObject& o): ObjectContainer(o), insidevector(o.insidevector) {};

public: // Public Methods

    // to implement Object3D
    Object3D* duplicate() const;
    Intersection* intersectLocalLimitedTime(const Ray &r, DBL maxtime) const;
    bool isInsideAt(const Vector3 &locallocation) const;

    /**Add patch to ObjectContainer member list.
      */
    void addPatch(SurfacePatch3D* o);
    
protected:
    // to implement ObjectContainer
    ObjectContainer* getEmptyContainer();

private:

    /**Vector which is used in testing if a point is inside the object.
      */
    Vector3 insidevector;

    /**Intersection statistics.
      */
    static HitStatistics stat;

};


class SharedPointsPatchObject: public PatchObject {

public:
    class Connectivity;
    typedef std::vector<Vector3> plist_t;
    typedef std::vector<Connectivity*> clist_t;
    typedef CNTREF pindex_t;

    class Connectivity { // move this class somewhere else
    public:
        virtual ~Connectivity() {}
        virtual SurfacePatch3D* makePatch(Vector3* pointlist) const = 0;
    };

    class TriangleConnectivity: public Connectivity { // move this class somewhere else
    public:
        TriangleConnectivity(pindex_t A, pindex_t B, pindex_t C);
        SurfacePatch3D* makePatch(Vector3* pointlist) const;
    private:
        pindex_t edge[3]; 
    };

    class PatchList {
        friend class SharedPointsPatchObject;
    public:
        ~PatchList();
        void addTriangle(const Vector3& A, const Vector3& B, const Vector3& C);
    private:
        typedef std::map<Vector3,pindex_t> pmap_t;
        pmap_t pmap;
        clist_t clist;
        pindex_t insertPoint(const Vector3&);
    };

public: // Constructor and destructor
    SharedPointsPatchObject(const PatchList&);
    ~SharedPointsPatchObject();

private:// Copy constructor
    SharedPointsPatchObject(const SharedPointsPatchObject&);    

public: // Public methods

    // overwrite from PatchObject
    Object3D* duplicate() const;

private: // Private attributes
    Vector3* pointlist;
    CNTREF *pointlistcount;

};

#endif

