/* *************************************************************************
                          cameraray.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002       
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: cameraray.h,v 1.16 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef CAMERARAY_H
#define CAMERARAY_H

#include "../src/colour.h"
#include "../src/insidelist.h"
#include "../src/intersection.h"
#include "../src/ray.h"
#include "../src/statistics.h"

class Scene;

/**Global ray emitted by camera.
  *@author Micha Riser
  */

class CameraRay: public Ray {

public: // Constructors and destructor

    /**Constructs new camera ray.
      *@param o ray's origin
      *@param d ray's direction
      *@param s scene belonging to ray
      *@param l list of object which is the ray inside at origin
      */
    CameraRay(const Vector3 o, const Vector3 d, Scene* s, InsideList* l);

    ~CameraRay();

    /**Copy constructor.
      */
    CameraRay(const CameraRay& r):
      Ray(r), norm(r.norm),  myscene(r.myscene), inside(new InsideList(*r.inside)),
      hit(0), htime(0), ltime(0), hpoint(r.hpoint), hitstatus(NONE),
      tracelevel(r.tracelevel), weight(r.weight) {}

private:
    CameraRay& operator=(const CameraRay&);

public: // Public methods

    /**Returns ray's contribution factor to the resulting colour.
      */
    FLT getWeight() {return weight;}

    /**Scale ray's contribution factor.
      *@param s factor
      */
    void scaleWeight(FLT s) {weight *= s;}

    /**Shoot ray. Calculate the ray-scene intersection.
      */
    bool shoot();

    /**Refract ray.
      *@param weightmult factor for colour weight reduction
      *@returns true if total reflection has occured else false
      */
    bool refract(FLT weightmult);

    /**Reflect ray.
      *@param weightmult factor for colour weight reduction
      */
    void reflect(FLT weightmult);

    /**Returns object's surface normal at intersection
      */
    Vector3 normal() const {return norm;}
    
    /**Returns object's texture at intersection
      */
    Texture* texture() const {return hit->texture();}

    /**Returns the intersection point.
      */
    Vector3 hitpoint() const {return hpoint;}

    /**Evaluate media interval. Calculates the colour contribution by the
      *interval between ray's origin and hitpoint.
      */
    Colour evaluateMedia(Colour& absorb) const;

    /**Get list of objects which the ray is inside in.
      */
    InsideList* getInside() const {return inside;}	

private: // Type definitions
    enum hitstatustype {NONE, OWN, OLD};

private: // Static attributes
    /**Counts numbers of CameraRays.
      */
    static HitStatistics statcount;

    /**Counts numbers of reflected and refracted rays.
      */
    static HitStatistics statref;
    
private: // Private attributes

    /**Holds surface normal at intersection after shoot() has been
      *invoked and an intersection been found.
      */
    Vector3 norm;
    
    Scene* myscene;
    InsideList* inside;
    Intersection* hit;
    DBL htime, ltime;
    Vector3 hpoint;
    hitstatustype hitstatus;
    int tracelevel;
    FLT weight;

private: // Private methods
    void getIntersectionInfo();
    void deleteIntersectionInfo();

};

#endif
