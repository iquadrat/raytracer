/* *************************************************************************
                          box.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: box.h,v 1.16 2003/05/25 11:25:52 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef BOX_H
#define BOX_H

#include "../src/intersection.h"
#include "../src/object3d.h"

/**Box object.
  *@author Micha Riser
  */

class Box: public Object3D  {
    
public:  // Constructor and destructor

    /**Creates new box.
      */
    Box(const Vector3& min, const Vector3& max): boxmin(min), boxmax(max) {}
    
public: // Public methods

    // overwrite in SurfacePatch3D
    bool hasFinitBounding() const;
    Vector3 getBoundingMin() const;
    Vector3 getBoundingMax() const;

    // overwrite in Object3D    
    void rotateRad(const Vector3& v);
    void rotateDeg(const Vector3& v);
    void scale(const Vector3& v);
    void translate(const Vector3& v);

    // to implement Object3D
    Object3D* duplicate() const {return new Box(*this);}            
    Intersection* intersectLocalLimitedTime(const Ray &r, DBL maxtime) const;
    bool isInsideAt(const Vector3 &locallocation) const;

private: // Private type definitions
    enum boxside {X,Y,Z,NONE};

private: // Static members
    /**Intersection statistics.
      */
    static HitStatistics stat;
    
private: // Private attributes
    Vector3 boxmin;    
    Vector3 boxmax;

private: // Private methods
    void calculateBoundingBox();

private: // Private inner classes

    /**Describes Ray-Box intersection.
      */
    class BoxIntersection: public Intersection {
    public: // Constructor and destructor
        BoxIntersection(const Object3D* o, const Ray& r, DBL t1, DBL t2,
          boxside s1, boxside s2): Intersection(o, r), currenttime(t1),
          secondtime(t2), currentside(s1), secondside(s2) {}

    public: // Public methods
        // inherited from Intersection
        DBL currentTime() {return currenttime;}
        void next();
        Vector3 normal();

    private: // Private attributes
        DBL currenttime, secondtime;
        boxside currentside, secondside;

    };

};

#endif
