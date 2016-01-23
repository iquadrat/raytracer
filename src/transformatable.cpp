/* *************************************************************************
                            transformatable.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: transformatable.cpp,v 1.10 2005/02/04 19:23:37 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/transformatable.h"

#include <cassert>
#include "../src/matrix.h"

void Transformatable::axisRotate(const Vector3& axis, DBL phi) {

    assert(axis.length()>DIRECTION_EPSILON);

    // construct rotation matrix
    // R(phi,N) = cos(phi)*I + (1-cos(phi))*N^T*N + sin(phi) * A
    // with phi: angle; N: axis; A = [[0 N3 -N2] [-N3 0 N1] [N2 -N1 0]]

    Matrix R;
    Vector3 N = axis;

    N.normalize();
    DBL cosp = cos(phi);
    DBL sinp = sin(phi);
    
    for(CNT8 i=0; i<3; i++) R(i,i) = N[i]*N[i]*(1-cosp) + cosp;

    R(0,1) = N[0]*N[1]*(1-cosp) + N[2]*sinp;
    R(1,0) = N[0]*N[1]*(1-cosp) - N[2]*sinp;
        
    R(0,2) = N[0]*N[2]*(1-cosp) - N[1]*sinp;
    R(2,0) = N[0]*N[2]*(1-cosp) + N[1]*sinp;

    R(1,2) = N[1]*N[2]*(1-cosp) + N[0]*sinp;
    R(2,1) = N[1]*N[2]*(1-cosp) - N[0]*sinp;

    // apply matrix
    applyOrthogonalMatrix(R);    

}

