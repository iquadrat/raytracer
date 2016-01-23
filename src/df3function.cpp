/* *************************************************************************
                          df3function.cpp  -  description
                             -------------------
    begin                : Sat May 10 2003
    copyright            : (C) 2003 by Micha Riser
    email                : micha@povworld.org
    
    $Id: df3function.cpp,v 1.3 2005/02/04 19:23:37 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/df3function.h"
//#include <algorithm>

//const DBL DF3Function::OUT_OF_RANGE_VALUE = 0.0;

DF3Function::DF3Function(std::istream& df3) {

    std::cout << "here";
    //fread(cdim,3,2,fp);
    df3.read( (char*)dim, 6 );
    
    //for(CNT8 i=0; i<3; i++) df3 >> dim[i];
    for(CNT8 i=0; i<3; i++) dim[i] = ( (dim[i]&0xff)<<8 ) + (dim[i] >>8 ) + 2;
    std::cout << dim[0]<<" "<<dim[1]<<" "<<dim[2]<<std::endl;
    voxels = new unsigned char[dim[0]*dim[1]*dim[2]];
    unsigned char *voxels_tmp = new unsigned char[(dim[0]-2)*(dim[1]-2)*(dim[2]-2)];
    df3.read( (char*)voxels_tmp,(dim[0]-2)*(dim[1]-2)*(dim[2]-2));
    for(int i=0; i<dim[0]-2; i++)
      for(int j=0; j<dim[1]-2; j++)
        for(int k=0; k<dim[2]-2; k++)
          voxels[i + j*dim[0] + k*dim[0]*dim[1]] = voxels_tmp[i+j*(dim[0]-2)+k*(dim[0]-2)*(dim[1]-2)];
//    for(CNT i=0; i<3; i++) df3 >> voxels[i];
    std::cout <<df3.gcount()<< "done";

}

DF3Function::~DF3Function(){
    delete[] voxels;
}

DBL DF3Function::interpolateBI(const Vector3& v) const {

    CNT pos[3];
    Vector3 w;
    for(CNT8 i=0; i<3; ++i) {
        pos[i] = int(dim[i]*v[i]);
        w[i] = (dim[i]*v[i]-pos[i]);
   //     assert(w[i]>=0 && w[i]<1);
    }

    DBL sum;
    sum = data(pos[0]  ,pos[1]  ,pos[2]  )*(1-w[0])*(1-w[1])*(1-w[2])
        + data(pos[0]+1,pos[1]  ,pos[2]  )*(  w[0])*(1-w[1])*(1-w[2])
        + data(pos[0]  ,pos[1]+1,pos[2]  )*(1-w[0])*(  w[1])*(1-w[2])
        + data(pos[0]+1,pos[1]+1,pos[2]  )*(  w[0])*(  w[1])*(1-w[2])
        + data(pos[0]  ,pos[1]  ,pos[2]+1)*(1-w[0])*(1-w[1])*(  w[2])
        + data(pos[0]+1,pos[1]  ,pos[2]+1)*(  w[0])*(1-w[1])*(  w[2])
        + data(pos[0]  ,pos[1]+1,pos[2]+1)*(1-w[0])*(  w[1])*(  w[2])
        + data(pos[0]+1,pos[1]+1,pos[2]+1)*(  w[0])*(  w[1])*(  w[2]);

    return sum;
    

}

DBL DF3Function::interpolateTRI(const Vector3& v0) const {

    Vector3 p(v0[0]*dim[0],v0[1]*dim[1],v0[2]*dim[2]);

//    std::cout << v << " " << w <<std::endl;        
    
/*    DBL sum = 0;
    for(int i=0; i<2; i++)
      for(int j=0; j<2; j++)
        for(int k=0; k<2; k++) {

            DBL wt;
            if (i==0)
        
            for(l=0; l<3; l++) if (i&(1<<j)) {
            test[j]+=grids;
            w*=diff[j];
            }
        
            sum += data(pos[0]+i,pos[1]+j,pos[2]+k)
            //voxels[ (pos[0]+i) + dim[0]*( pos[1]+j + dim[1]*(pos[2]+k)) ]
              * (w[i])*(w[j])*(w[k]);

        }*/
/*        
    DBL sum;
    sum = data(pos[0]  ,pos[1]  ,pos[2]  )*(1-w[0])*(1-w[1])*(1-w[2])
        + data(pos[0]+1,pos[1]  ,pos[2]  )*(  w[0])*(1-w[1])*(1-w[2])
        + data(pos[0]  ,pos[1]+1,pos[2]  )*(1-w[0])*(  w[1])*(1-w[2])
        + data(pos[0]+1,pos[1]+1,pos[2]  )*(  w[0])*(  w[1])*(1-w[2])
        + data(pos[0]  ,pos[1]  ,pos[2]+1)*(1-w[0])*(1-w[1])*(  w[2])
        + data(pos[0]+1,pos[1]  ,pos[2]+1)*(  w[0])*(1-w[1])*(  w[2])
        + data(pos[0]  ,pos[1]+1,pos[2]+1)*(1-w[0])*(  w[1])*(  w[2])
        + data(pos[0]+1,pos[1]+1,pos[2]+1)*(  w[0])*(  w[1])*(  w[2]);

//    std::cout << sum << " ";
*/

  int             x, y, z;
  register int    i, j, k;
  FLT           dx, dy, dz;
  unsigned char *pv;
  FLT           u[4], v[4], w[4];
  FLT           r[4], q[4];
  FLT           vox = 0;
  int             xyDim;

  xyDim = dim[0] * dim[1];

  x = (int) p[0], y = (int) p[1], z = (int) p[2];
  if (x < 2 || x >= dim[0]-2 || y < -2 || y >= dim[1]-2 || z < 2 || z >= dim[2]-2)
    return (0);

  dx = p[0] - (float) x, dy = p[1] - (float) y, dz = p[2] - (float) z;
  pv = voxels + (x - 1) + (y - 1) * dim[0] + (z - 1) * xyDim;

# define CUBE(x)   ((x) * (x) * (x))
# define SQR(x)    ((x) * (x))
/*
 #define DOUBLE(x) ((x) + (x))
 #define HALF(x)   ...
 *
 * may also be used to reduce the number of floating point
 * multiplications. The IEEE standard allows for DOUBLE/HALF
 * operations.
 */

  /* factors for Catmull-Rom interpolation */

  u[0] = -0.5 * CUBE (dx) + SQR (dx) - 0.5 * dx;
  u[1] = 1.5 * CUBE (dx) - 2.5 * SQR (dx) + 1;
  u[2] = -1.5 * CUBE (dx) + 2 * SQR (dx) + 0.5 * dx;
  u[3] = 0.5 * CUBE (dx) - 0.5 * SQR (dx);

  v[0] = -0.5 * CUBE (dy) + SQR (dy) - 0.5 * dy;
  v[1] = 1.5 * CUBE (dy) - 2.5 * SQR (dy) + 1;
  v[2] = -1.5 * CUBE (dy) + 2 * SQR (dy) + 0.5 * dy;
  v[3] = 0.5 * CUBE (dy) - 0.5 * SQR (dy);

  w[0] = -0.5 * CUBE (dz) + SQR (dz) - 0.5 * dz;
  w[1] = 1.5 * CUBE (dz) - 2.5 * SQR (dz) + 1;
  w[2] = -1.5 * CUBE (dz) + 2 * SQR (dz) + 0.5 * dz;
  w[3] = 0.5 * CUBE (dz) - 0.5 * SQR (dz);

  for (k = 0; k < 4; k++)
  {
    q[k] = 0;
    for (j = 0; j < 4; j++)
    {
      r[j] = 0;
      for (i = 0; i < 4; i++)
      {
        r[j] += u[i] * *pv;
        pv++;
      }
      q[k] += v[j] * r[j];
      pv += dim[0] - 4;
    }
    vox += w[k] * q[k];
    pv += xyDim - 4 * dim[0];
  }

//  std::cout << vox << " " << std::endl;
//  assert(vox>=0 && vox<256);
  return std::max(0.0f,vox);

}

unsigned char DF3Function::data(CNT x, CNT y, CNT z) const {

    if (x<0 || x>=dim[0] || y<0 || y>=dim[1] || z<0 || z>=dim[2]) return OUT_OF_RANGE_VALUE;
    return voxels[ x + dim[0]*(y + dim[1]*z) ];
    
}

DBL DF3Function::evaluateAt(const Vector3& vo) const {

    Vector3 v(vo);
//    v.scale(0.1);
//    std::cout << vo;
    for(CNT8 i=0; i<3; ++i) if (v[i]<0 || v[i]>1) return 0.3-OUT_OF_RANGE_VALUE;

/*    CNT pos[3];
    for(CNT8 i=0; i<3; ++i) pos[i] = int(dim[i]*v[i]);

//    std::cout << int(voxels[pos[0]+dim[0]*(pos[1]+dim[1]*pos[2])]) << " ";
    return 0.3-voxels[pos[0]+dim[0]*(pos[1]+dim[1]*pos[2])]/256.0;*/
    return 0.3-interpolateTRI(v)/256.0;

}
