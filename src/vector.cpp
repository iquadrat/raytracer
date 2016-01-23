/* *************************************************************************
                            vector.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002  
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: vector.cpp,v 1.9 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/vector.h"

#include "../src/matrix.h"

void Vector3::applyFromLeft(const Matrix &m) {
    Vector3 ret(0,0,0);
    for(int j=0; j<3; j++) {
        for(int i=0; i<3; i++) ret[j] += m(j,i)*a[i];
    }
    *this = ret;
}

void Vector3::applyFromLeftTransposed(const Matrix &m) {
    Vector3 ret(0,0,0);
    for(int j=0; j<3; j++) {
        for(int i=0; i<3; i++) ret[j] += m(i,j)*a[i];
    }
    *this = ret;
}

void Vector3::rotate(const Vector3& v) {
    applyFromLeft(Matrix::generateRotationMatrix(v));
}

std::ostream& operator<<(std::ostream&s, const Vector3& v) {

    return s<<'<'<<v[0]<<','<<v[1]<<','<<v[2]<<'>';

}
