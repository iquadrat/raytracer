/* *************************************************************************
                            objectcontainer.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: objectcontainer.h,v 1.13 2003/05/25 20:13:33 micha Exp $

 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */
#ifndef OBJECTCONTAINER_H
#define OBJECTCONTAINER_H

#include<vector>

#include "../src/intersection.h"
#include "../src/object3d.h"
#include "../src/types.h"

/**Provides a container for multiple objects and different set operations
  *between them.
  *@author Micha Riser
  */

class ObjectContainer: public Object3D {

    friend class IntersectionHeap;

public: // Constructors and destructor

    /**Constructs new empty ObjectContainer object.*/
    ObjectContainer();
    
    /**Destructor.*/
    virtual ~ObjectContainer();

private:
    ObjectContainer& operator=(const ObjectContainer&);

protected:

    /**Creates a new ObjectContainer from another Object with the givenObjectList.
      *Used for creating inverse.
      */
    class ObjectList;
    ObjectContainer(const Object3D& o, ObjectList* l);
    
    /**Copy constructor.
      */
    ObjectContainer(const ObjectContainer& c);

public: // Public methods

    /**Finish ObjectContainer object list creation. After calling this method no more
      *members can be added to this ObjectContainer.
      */
    void close();

    // overwrite in SurfacePatch3D
    void prepareToRender();

protected: // Type definitions
    typedef std::vector<SurfacePatch3D*> spvector;

protected: // Constants
    static const unsigned int MAX_MEMBERS_COUNT = 31;
    static const unsigned int MIN_MEMBERS_COUNT = 7;

    static long ObjectContainer_count;
    static long ObjectContainer_list_size[MAX_MEMBERS_COUNT];
    
protected: // Protected inner classes
   
    /**Holds the list of member objects. Copied by reference.
      */
    class ObjectList {

    public: // Public attributes
        /**array of pointer to objects*/
        SurfacePatch3D* list[MAX_MEMBERS_COUNT];
        /**To cache the inverse*/
        ObjectList *inverse;
        /**Reference count.*/
        int count;
        /**Size of list*/
        CNTREF listsize;

    public: // Constructor and destructor
        ObjectList();
        ~ObjectList();

    private:
        ObjectList(const ObjectList&);
        ObjectList& operator=(const ObjectList&);
    };

#ifndef WRONG_INNER_CLASSES_ACCESS    
protected: // Protected attributes
#else
public: // should be protected but gcc 2.95 does not allow outer classes to access private inner members
#endif

    ObjectList* objects;
    spvector* collector_list;
    bool isclosed;
    bool isprepared;

protected: // Protected methods
    /**Calculates and creates a bounding box for the ObjectContainer if it is finit.
      */
    virtual void calculateBoundingBox();

    /**Returns empty, untransformed container of same type
      */
    virtual ObjectContainer* getEmptyContainer() = 0;

private:
    void split();
    // helper functions for splitting
    static bool xAxisLess(const SurfacePatch3D* o1, const SurfacePatch3D* o2);
    static bool yAxisLess(const SurfacePatch3D* o1, const SurfacePatch3D* o2);
    static bool zAxisLess(const SurfacePatch3D* o1, const SurfacePatch3D* o2);
    static DBL calcArea(const Vector3&);
};

#endif


