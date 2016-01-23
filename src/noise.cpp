/* *************************************************************************
                          noise.cpp  -  description
                             -------------------
    begin                : Die Jul 15 2003
    copyright            : (C) 2003 by Micha Riser
    email                : mriser@gmx.net
    
    $Id: noise.cpp,v 1.3 2003/07/21 23:41:51 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/noise.h"

DBL PerlinNoiseCommon::evaluate(const Vector3& point) const {

    int gridPtL, gridPtR, gridPtD, gridPtU, gridPtB, gridPtF;
    int idxL, idxR, idxLD, idxLU, idxRD, idxRU;
    DBL distL, distR, distD, distU, distB, distF;
    DBL sx, sy, sz, a, b, c, d, u, v;

    setupValues(point[0], gridPtL, gridPtR, distL, distR);
    setupValues(point[1], gridPtD, gridPtU, distD, distU);
    setupValues(point[2], gridPtB, gridPtF, distB, distF);

    idxL = permutationTbl[gridPtL];
    idxR = permutationTbl[gridPtR];

    idxLD = permutationTbl[idxL + gridPtD];
    idxRD = permutationTbl[idxR + gridPtD];
    idxLU = permutationTbl[idxL + gridPtU];
    idxRU = permutationTbl[idxR + gridPtU];

    sx = sCurve(distL);
    sy = sCurve(distD);
    sz = sCurve(distB);

    u = Vector3::dotProd( gradTbl[idxLD + gridPtB], Vector3(distL, distD, distB) );
    v = Vector3::dotProd( gradTbl[idxRD + gridPtB], Vector3(distR, distD, distB) );
    a = linInterp(sx, u, v);

    u = Vector3::dotProd( gradTbl[idxLU + gridPtB], Vector3(distL, distU, distB) );
    v = Vector3::dotProd( gradTbl[idxRU + gridPtB], Vector3(distR, distU, distB) );
    b = linInterp(sx, u, v);

    c = linInterp(sy, a, b);

    u = Vector3::dotProd( gradTbl[idxLD + gridPtF], Vector3(distL, distD, distF) );
    v = Vector3::dotProd( gradTbl[idxRD + gridPtF], Vector3(distR, distD, distF) );
    a = linInterp(sx, u, v);

    u = Vector3::dotProd( gradTbl[idxLU + gridPtF], Vector3(distL, distU, distF) );
    v = Vector3::dotProd( gradTbl[idxRU + gridPtF], Vector3(distR, distU, distF) );
    b = linInterp(sx, u, v);

    d = linInterp(sy, a, b);
    return linInterp(sz, c, d);
}



void PerlinNoiseCommon::
  setupValues(DBL comp, int & gridIdx0, int & gridIdx1, DBL& dist0, DBL& dist1) const {

    gridIdx0 = ((int)comp) % 256;
    if (comp <= 0) {
        gridIdx0 += 255;
        dist1 = comp - (int)comp; //get fractional value of t
        dist0 = dist1 + 1;
    }  else {
        dist0 = comp - (int)comp; //get fractional value of t
        dist1 = dist0 - 1;
    }
    gridIdx1 = gridIdx0 + 1;

}

void PerlinNoiseRandomGradients::seed(long seedVal) {

    Random rng(seedVal);

    for(int j=0; j<256; j++) {
        permutationTbl[j] = j;
        gradTbl[j] = rng.pointInSphere();
    }

    for(int j=0; j<256; j++) {
        int k = int(rng.getDouble()*256);
        std::swap(permutationTbl[j],permutationTbl[k]);
    }

    for(int j=0; j<258; j++) {
        permutationTbl[256 + j] = permutationTbl[j];
    }

    for(int j=0; j<514; j++) {
        gradTbl[j] = gradTbl[j%256];
    }

}

void PerlinNoiseBaseGradients::seed(long seedVal) {

    Random rng(seedVal);

    for(int j=0; j<256; j++) {
        permutationTbl[j] = j;
    }

    for(int j=0; j<256; j++) {
        int k = int(rng.getDouble()*256);
        std::swap(permutationTbl[j],permutationTbl[k]);
    }

    for(int j=0; j<258; j++) {
        permutationTbl[256 + j] = permutationTbl[j];
    }

    for(int j=0; j<514; j++) {
        gradTbl[j] = baseGradients[j%16];
    }

}

const Vector3 PerlinNoiseBaseGradients::baseGradients[16] =
{
    Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3( 1,-1, 0), Vector3(-1,-1, 0),
    Vector3(1, 0, 1), Vector3(-1, 0, 1), Vector3( 1, 0,-1), Vector3(-1, 0,-1),
    Vector3(0, 1, 1), Vector3( 0,-1, 1), Vector3( 0, 1,-1), Vector3( 0,-1,-1),
    Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3( 0,-1, 1), Vector3( 0,-1,-1)
};
