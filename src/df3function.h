/* *************************************************************************
                          df3function.h  -  description
                             -------------------
    begin                : Sat May 10 2003
    copyright            : (C) 2003 by Micha Riser
    email                : micha@povworld.org
    
    $Id: df3function.h,v 1.3 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef DF3FUNCTION_H
#define DF3FUNCTION_H

#include <iostream>
#include "../src/isosurface.h"

/**
  *@author Micha Riser
  */

class DF3Function: public Isosurface::IsosurfaceFunction {
  
public: 
    DF3Function(std::istream& df3stream);
    ~DF3Function();
private:
    DF3Function(const DF3Function&);
    DF3Function& operator=(const DF3Function&);
    
public:
    DBL evaluateAt(const Vector3&) const;

private:

    DBL interpolateTRI(const Vector3&) const;
    DBL interpolateBI(const Vector3&) const;    

    unsigned char data(CNT x, CNT y, CNT z) const;

//    static const DBL OUT_OF_RANGE_VALUE;
    static const unsigned char OUT_OF_RANGE_VALUE = 0;

    unsigned short int dim[3];
    unsigned char* voxels;
    

};

#endif
