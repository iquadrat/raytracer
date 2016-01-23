/* *************************************************************************
                          boundingbox.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: boundingbox.h,v 1.17 2003/05/25 11:25:52 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../src/statistics.h"
#include "../src/transformatable.h"
#include "../src/vector.h"

class TransparentTransformatable;

/**A boundingbox encloses the object's surface. The whole surface is inside the
  *box between boxmin and boxmax. 
  *Boundingboxes are in upper-level space and always axis-aligned.
  *@author Micha Riser
  */  
class BoundingBox {

public: // Constructors and destructor

    /**Generates a bounding box from two edge points. All components of
      *edgemin have to be smaller than the corresponding components of
      *edgemax.
      */
    BoundingBox(Vector3 edgemin, Vector3 edgemax): boxmin(edgemin), boxmax(edgemax) {}

public: // Methods that implement Transformatable    

    void translate(const Vector3& v) {boxmin.add(v); boxmax.add(v);}    
    void scale(const Vector3& v);    
    void rotateDeg(const Vector3& v);    
    void rotateRad(const Vector3& v);
    void applyOrthogonalMatrix(const Matrix &m) {applyMatrix(m);}
    void applyMatrix(const Matrix &m);    
    
public: // Methods that give access to min/max edge

    /**
      *@returns Minimum edge point of the box.
      */
    Vector3 getBoxMin() const {return boxmin;}

    /**
      *@returns Maximum edge point of the box.
      */
    Vector3 getBoxMax() const {return boxmax;}
        
public: // other methods

    /**Transformate the box the same way as the given Transformatable.
      *@param t the Transformatable
      */
    void transformate(const TransparentTransformatable &t);

    /**Intersect Box with Ray.
      *@param r the Ray
      *@returns -1 if no intersection, INTERSECTION_TIME_EPSILON if Ray's origin is inside the box
      *otherwise the time of first intersection.
      */
    DBL intersect(const Ray &r) const;
    
    /**Intersect Box with Ray limited to maximum time.
      *@param r the Ray
      *@param tmax Maximum intersection time.
      *@returns -1 if no intersection, INTERSECTION_TIME_EPSILON if Ray's origin is inside the box
      *otherwise the time of first intersection.
      */
    DBL intersect(const Ray &r, DBL tmax) const;

    /**Test point for box insidness.
      *@param a the point
      *@returns true if the point is inside the box else false
      */
    bool isInsideAt(const Vector3& a) const;

private: // Static members

    /**Intersection statistics.
      */
    static HitStatistics stat;
    
private: // Private attributes

    /**stores minimal edge
      */
    Vector3 boxmin;
    /**stores maximal edge
      */
    Vector3 boxmax;

private: // Private methods
    void updateBoundingBoxPoints(Vector3& min, Vector3& max, const Matrix& rot, Vector3 test);

};

#endif
