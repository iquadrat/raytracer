/* *************************************************************************
                            matrix.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: matrix.h,v 1.6 2003/03/04 11:47:42 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef MATRIX_H
#define MATRIX_H

#include<cmath>
#include<cstring>

#include "../src/types.h"

class Vector3;

/**3x3 transformation matrix.
  *@author Micha Riser
  */

class Matrix {

public: // Constructors and destructor

    /**Create uninitialized matrix.
      */
    Matrix() {}
    
    /**Create matrix from array.
      *@param d 3x3 array holding the matrix entries
      */
    Matrix(DBL d[3][3]) {memcpy(&d,&a,sizeof(a));}

    /**Create matrix with same entries.
      *@param d value for all elements
      */
    Matrix(int d) {memset(&a,d,sizeof(DBL)*3*3);}

public: // Public methods

    /**Access to matrix elements.
      */
    DBL& operator() (int i, int j) {return a[i][j];}

    /**Access to matrix elements in constant matrix.
      */
    const DBL& operator() (int i, int j) const {return a[i][j];}
    
    /**Set all matrix entries to zero.
      */
    void zero() {memset(&a,0,sizeof(DBL)*3*3);}

    /**Transpose matrix.
      */
    void transpose();

    /**Invert matrix.
      *@returns false if matrix is singular else (that means inversion was successful) true
      */
    bool invert();

    /**Matrix multiplication. Calculate A = M*A.
      *@param m the matrix M
      */
    void applyFromLeft(const Matrix &m);

public: // Static methods
    /**Create scaling matrix. Elements of v on the diagonal and zeros elsewhere.
      *@param v x,y,z scaling value
      */
    static Matrix generateScaleMatrix(const Vector3& v);

    /**Create rotation matrix.
      *@param v x,y,z rotation values in radians. Rotating is first done around x then
      *around y and finally around z axis
      */
    static Matrix generateRotationMatrix(const Vector3& v);

private: // Private attributes
    static const DBL SMALL_DET;
    DBL a[3][3];
        
};

#endif
