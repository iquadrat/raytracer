/* *************************************************************************
                          isosurface.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: isosurface.h,v 1.11 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef ISOSURFACE_H
#define ISOSURFACE_H

#include "../src/function.h"
#include "../src/intersection.h"
#include "../src/object3d.h"

/**Object described by a function. The isosuface's surface is formed by
  *all points where the given function is zero. Function value smaller than
  *zero means inside the isosurface.
  *@author Micha Riser
  */
class Isosurface: public Object3D  {

public: // Type definitions
    typedef Function<const Vector3,DBL> IsosurfaceFunction;

public: // Constructor and destructor

    /**Constructor.
      *@param func the function describing the surface
      *@param cont container object
      *@param max maximal second derivative
      *@param doeval If eval is enabled the algorithm increases the max second
      *derivative if it sees that this is necessary.
      */
    Isosurface(IsosurfaceFunction* func, Object3D* cont, DBL maxd, bool doeval);
    
    ~Isosurface();
    
private: // Private constructor
    /**Copy constructor.
      */
    Isosurface(const Isosurface&);
    Isosurface& operator=(const Isosurface&);

public: // Public methods

    // Inherited from Object3D
    Object3D* duplicate() const;
    bool isInsideAt(const Vector3& locallocation) const;
    Intersection* intersectLocalLimitedTime(const Ray& r, DBL maxtime) const;

private: // Constants
    static const DBL NORMAL_E;
    static const int MAX_STEP_INCREASE = 1024;
#ifdef WRONG_INNER_CLASSES_ACCESS
public:
#endif
    static const DBL K_INCREASE_FACT;
    
private: // Static members
    /**Intersection statistics.
      */
    static HitStatistics stat;
    static int isosurfacecount;

private: // Private attributes
#ifdef WRONG_INNER_CLASSES_ACCESS
public:
#endif

    /**Container limits the space where intersections are searched.
      *Container has to be convex and non-inversed!
      */
    Object3D* container;

    /**The function f describes the surface. f(v)<0 means inside object,
      *f(v)>0 means outside, f(v)=0 describes the surface
      */
    IsosurfaceFunction* f;

    mutable DBL K;
    mutable DBL missstepsize;
    bool eval;

    // max derivative statistics
    mutable MaxStatistics<DBL> maxdstat;

private: // Private inner classes

    /**Structure holding time and associated function value.
      */
    class StackElement {
    public: // Public attributes
        DBL time;
        DBL value;
    public: // Constructors and destructor
        StackElement() {}
        StackElement(DBL t, DBL v): time(t), value(v) {}
    };

    /**Describes Ray-Isosurface intersecion. The actual root-finding is done
      *in this class.
      */
    class IsosurfaceIntersection: public Intersection {

    public: // Public attributes
        bool nointersection;

    public: // Constructor and destructor
        IsosurfaceIntersection(const Isosurface* object, const Ray& r, DBL tmax);
        ~IsosurfaceIntersection();

    public: // Public methods
        void next();
        DBL currentTime();
        Vector3 normal();
        
    private: // Private attributes
        const Isosurface* iso;
        StackElement prelast, last;
        DBL root;
        DBL klocal;
        DBL maxtime;
        DBL t_container_max;
        bool containerhit;
        Intersection* i_container;

    private: // Private methods
        void findExactRoot();
        void findRoot();
        DBL calcPolyRoot();

    };

};

#endif
