/* *************************************************************************
                            matrix.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/matrix.h"

#include "../src/vector.h"

const DBL Matrix::SMALL_DET = 1e-10;

void Matrix::transpose() {
    
    std::swap(a[0][1],a[1][0]);
    std::swap(a[0][2],a[2][0]);
    std::swap(a[2][1],a[1][2]);
        
}

bool Matrix::invert() {

    // *check me* for correctness
    // how about the stableness of this method?

    DBL det = a[0][0] * ( a[1][1]*a[2][2] - a[2][1]*a[1][2] )
            + a[1][0] * ( a[0][2]*a[2][1] - a[2][2]*a[0][1] )
            + a[2][0] * ( a[0][1]*a[1][2] - a[1][1]*a[0][2] );

    if (det < SMALL_DET) return false;

    det = 1.0/det;
    Matrix inv;
    inv(0,0) = (a[1][1]*a[2][2]-a[1][2]*a[2][1])*det;
    inv(0,1) = (a[0][2]*a[2][1]-a[0][1]*a[2][2])*det;
    inv(0,2) = (a[0][1]*a[1][2]-a[0][2]*a[1][1])*det;
    inv(1,0) = (a[1][2]*a[2][0]-a[1][0]*a[2][2])*det;
    inv(1,1) = (a[0][0]*a[2][2]-a[0][2]*a[2][0])*det;
    inv(1,2) = (a[0][2]*a[1][0]-a[0][0]*a[1][2])*det;
    inv(2,0) = (a[1][0]*a[2][1]-a[1][1]*a[2][0])*det;
    inv(2,1) = (a[0][1]*a[2][0]-a[0][0]*a[2][1])*det;
    inv(2,2) = (a[0][0]*a[1][1]-a[0][1]*a[1][0])*det;
    *this = inv;
    
    return true;

}

void Matrix::applyFromLeft(const Matrix &m) {
    Matrix tmp = Matrix(*this);
    zero();
    for(int i=0; i<3; ++i)
        for(int j=0; j<3; ++j)
            for(int k=0; k<3; ++k)
                a[i][j] += tmp(i,k) * m(k,j);
}

Matrix Matrix::generateRotationMatrix(const Vector3& v) {

    double sinx = sin(v[0]);
    double siny = sin(v[1]);
    double sinz = sin(v[2]);
        
    double cosx = cos(v[0]);
    double cosy = cos(v[1]);
    double cosz = cos(v[2]);
        
    Matrix rot;
    rot(0,0) = cosy*cosz;
    rot(0,1) = cosy*sinz;
    rot(0,2) =-siny;
            
    rot(1,0) = cosz*sinx*siny-cosx*sinz;
    rot(1,1) = cosx*cosz+sinx*siny*sinz;
    rot(1,2) = cosy*sinx;
        
    rot(2,0) = cosx*cosz*siny+sinx*sinz;
    rot(2,1) =-cosz*sinx+cosx*siny*sinz;
    rot(2,2) = cosx*cosy;

    rot.transpose(); // hmm, could write it transposed already above
    return rot;
}

Matrix Matrix::generateScaleMatrix(const Vector3 &v) {
    Matrix m;
    m(0,0)=v[0]; m(0,1)=0;    m(0,2)=0;
    m(1,0)=0;    m(1,1)=v[1]; m(1,2)=0;
    m(2,0)=0;    m(2,1)=0;    m(2,2)=v[2];   
    return m;
}
