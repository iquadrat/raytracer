/* *************************************************************************
                          random.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: random.h,v 1.5 2003/08/04 13:20:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef RANDOM_H
#define RANDOM_H

#include "../src/types.h"
#include "../src/vector.h"

/**Random number stream.
  *@author Micha Riser
  */

class Random {
    
public: 
    Random(long s = 0) {randx = s;}
    void seed(long s) {randx = s;}
    
    static Random global;    

public: // Static methods

    /**Get next random double value in interval [0,1[
      */
    DBL getDouble() {return abs(draw())/max();}          
    
    /**Get factor in the range [1-amouth,1+amouth]
      */
    DBL jitterFactor(DBL amouth) {
        return 1-amouth+2*amouth*getDouble();
    }


    /**Returns random point in box from <-1,-1,-1> to <1,1,1>.
      */
    Vector3 pointInBox() {
        DBL d1 = getDouble();
        DBL d2 = getDouble();
        DBL d3 = getDouble();
        return Vector3(2*d1-1,2*d2-1,2*d3-1);
    }

    /**Returns random point in a unit sphere. (uniform distributed)
      */
    Vector3 pointInSphere() {
        Vector3 ret;
        do {
            ret = pointInBox();
        } while (ret.lengthSq()>1);
        return ret;
    }

    /**Returns random unit vector. (uniform distributed)
      */
    Vector3 direction() {        
        Vector3 ret;
        do{ret = pointInSphere();} while(ret == Vector3(0));
        ret.normalize();
        return ret;
    }

private:
    unsigned long randx;
    static long abs(long x) {return x&0x7fffffff;}
    static double max() {return 2147483648.0;}
    long draw() {return randx = randx*1103515245 + 12345;}
    
};

#endif
