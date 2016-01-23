/* *************************************************************************
                            transformatable.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: transformatable.h,v 1.12 2003/03/04 11:47:42 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef TRANSFORMATABLE_H
#define TRANSFORMATABLE_H

#include "../src/vector.h"

class Ray;

/**Describes the interface for everything that is in space and can be
  *translated and transformed.
  *@author Micha Riser
  */

class Transformatable {

public: // Constructor and destructor

    Transformatable() {}
    virtual ~Transformatable() {}

private: // Disallow assignment constructor
    Transformatable& operator=(const Transformatable&);

public: // Public methods

    /**Translate by vector.
      */
    virtual void translate(const Vector3 &v) = 0;

    /**Scale by x,y,z factor from vector.
      */
    virtual void scale(const Vector3 &v) = 0;

    /**Rotate around x,y and z axis.
      *@param v vector holding the rotation angles in degrees
      */
    virtual void rotateDeg(const Vector3 &v) = 0;

    /**Rotate around x,y and z axis.
      *@param v vector holding the rotation angles in radians
      */
    virtual void rotateRad(const Vector3 &v) = 0;    

    /**Transform by orthogonal matrix.
      *@param M matrix for which M * M^T = I holds
      */
    virtual void applyOrthogonalMatrix(const Matrix &M) = 0;

    /**Transform by non-singular matrix.
      *@param M matrix for which matrix L exists with M*L = I
      */
    virtual void applyMatrix(const Matrix &M) = 0;

    /**Rotate around given axis by given angle in radians
      *@param axis rotation axis
      *@param a angle in radians
      */
    void axisRotate(const Vector3& axis, DBL a);

};


#endif
