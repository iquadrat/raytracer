/* *************************************************************************
                            sphere.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: sphere.h,v 1.13 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef SPHERE_H
#define SPHERE_H

//#define SPHERE_STORE_RSQ

#include "../src/intersection.h"
#include "../src/object3d.h"

/**Sphere object.
  *@author Micha Riser
  */

class Sphere: public Object3D {

public: // Constructors and destructor

    /**Create sphere at origin.
      *@param r radius
      */
    Sphere(DBL r): radius(r)
    #ifdef SPHERE_STORE_RSQ
      , radiussq(r*r)
    #endif
      {}
      

    /**Create sphere.
      *@param r radius
      *@param l midpoint
      */
    Sphere(DBL r, Vector3 l): Object3D(l), radius(r)
    #ifdef SPHERE_STORE_RSQ    
      , radiussq(r*r)
    #endif
      {}

public: // Pulblic methods

    // Inherited from Object3D
    Object3D* duplicate() const {return new Sphere(*this);}
    
    void rotateRad(const Vector3& v);
    void rotateDeg(const Vector3& v);    
    void scale(const Vector3& v);
    Intersection* intersectLocal(const Ray &r) const;
    Intersection* intersectLocalLimitedTime(const Ray &r, DBL maxtime) const;
    bool isInsideAt(const Vector3 &locallocation) const;

    bool hasFinitBounding() const;
    Vector3 getBoundingMin() const;
    Vector3 getBoundingMax() const;
    PatchObject* toPatch(double precision) const;

private: // Static members
    static HitStatistics stat;
    
private: // Private attributes
    DBL radius;
#ifdef SPHERE_STORE_RSQ    
    DBL radiussq;
#endif
        
private: // Private methods
    void calculateBoundingBox();
    Vector3 point(double angle1, double angle2) const;

private: // Inner classes

    /**Describes Ray-Sphere Intersection.
      */
    class SphereIntersection: public Intersection {

    private: // Private attributes
        DBL currenttime;
        DBL secondtime;

    public: // Constructor and destructor
        SphereIntersection(const Object3D* o, const Ray &r, DBL t1, DBL t2):
          Intersection(o,r), currenttime(t1), secondtime(t2) {}

    public: // Pulblic methods

        //Inherited from Intersection
        DBL currentTime() {return currenttime;}
        void next();
        Vector3 normal();
        
    };
            
};

#endif
