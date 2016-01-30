/* *************************************************************************
                          noise.h  -  description
                             -------------------
    begin                : Die Jul 15 2003
    copyright            : (C) 2003 by Micha Riser
    email                : micha@povworld.org
    
    $Id: noise.h,v 1.3 2003/07/21 23:41:51 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef NOISE_H
#define NOISE_H

#include "../src/function.h"
#include "../src/random.h"
#include "../src/types.h"
#include "../src/vector.h"
#include <immintrin.h>

/**Noise generator.  *@author Micha Riser
  */
template<class D, class R> class Noise {
    
public: 
    Noise() {} 
    virtual ~Noise() {}
    virtual void seed(long seedVal) = 0;

    /** Standard normal distributed noise */
    virtual R evalN(const D&) const = 0;

    /** Noise in the range [0,1] */
    virtual R eval01(const D&) const = 0;

    /** Sum1/F Noise: sum_{i=0}^octaves noise(b^i*p)/(a^i)*/
    R sum1FNoise(D p, int octaves, DBL a, DBL b) const {
        R sum(0);
        DBL ai = 1;
        for(int i=0; i<octaves; i++, ai/=a, p*=b)
            sum += evalN(p)*ai;
        return sum;
    }
    
};


/**Noise vector generator. Generates continous noisy 3D vector field.
  *@author Micha Riser
  */
class VNoise: public Noise<Vector3,Vector3> {

    typedef Noise<Vector3,DBL> NBase;
    
public:
    /**Constructor, takes 3 noise generator for the individual
      *vector components. Generators are destroyed when the VNoise
      *object gets destroyed.
      */
    VNoise(NBase* ngx, NBase* ngy, NBase* ngz, long seedValue = 123):
      genx(ngx), geny(ngy), genz(ngz)
    {
        seed(seedValue);
    }

    ~VNoise() {
        delete(genx); delete(geny); delete(genz);
    }
    
    void seed(long seedValue) {
        genx->seed(seedValue);
        geny->seed(seedValue+1);  
        genz->seed(seedValue+2);                
    }

    /** Standard normal distributed noise */
    Vector3 evalN(const Vector3& p) const {
        return Vector3(genx->evalN(p),geny->evalN(p),genz->evalN(p));
    }

    /** Noise in the range [0,1] */
    Vector3 eval01(const Vector3& p) const {
        return Vector3(genx->eval01(p),geny->eval01(p),genz->eval01(p));
    }
    
private:
    NBase *genx, *geny, *genz;

};


/** Perlin noise 3D.
  * Based on code from Christopher James Huff's 3dmath library.
  */
class PerlinNoiseCommon: public Noise<Vector3,DBL> {

public:
    PerlinNoiseCommon(DBL stddev):
    inv_stdev(1.0/stddev),
    inv_stdev_scaled(1.0/(stddev*2*2.58)) {
        permutationTbl = (int*) _mm_malloc(514 * sizeof(int), 32);
        gradTbl = (Vector3*) _mm_malloc(514 * sizeof(Vector3), 32);
    };

    DBL evalN(const Vector3& p) const {
        return evaluate(p) * inv_stdev;
    }

    DBL eval01(const Vector3& p) const {
        // <1% of the values are clipped
        return clip01( evaluate(p) * inv_stdev_scaled +.5 );
    }

protected:
    const DBL inv_stdev;
    const DBL inv_stdev_scaled;

    /** Permutation table */
    int* permutationTbl; //[514]; // 256*2 + 2

    Vector3* gradTbl; //[514];

    /** Is approximatly normal distributed with mean 0 and
      * standard deviation STDDEV (this value is measured
      * empirically and not analytically)
      */
    DBL evaluate(const Vector3& point) const;

    DBL sCurve(DBL v) const {return ((6*v-15)*v+10)*pow3(v);}

    DBL linInterp(DBL t, DBL a, DBL b) const {return t*(b - a) + a;}

    void setupValues(DBL comp, int & gridIdx0, DBL& dist0, DBL& dist1) const;

};


class PerlinNoiseBaseGradients: public PerlinNoiseCommon {
public:
    PerlinNoiseBaseGradients(long sd = 123456789):
      PerlinNoiseCommon(0.256883) {seed(sd);}

    void seed(long seedVal);
          
private:   
    static const Vector3 baseGradients[16];

};


class PerlinNoiseRandomGradients: public PerlinNoiseCommon {
public:
    PerlinNoiseRandomGradients(long sd = 123456890):
      PerlinNoiseCommon(0.1496) {seed(sd);}

    void seed(long seedVal);
    
};


typedef PerlinNoiseRandomGradients PerlinNoise;
//typedef PerlinNoiseBaseGradients PerlinNoise;

#endif
