/* *************************************************************************
                            vector.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002  
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: vector.h,v 1.20 2003/07/21 23:41:51 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>
#include <immintrin.h>

#include "../src/types.h"

#define VECTOR_AVX
//#define VECTOR_AVX_ALIGN
//#define VECTOR_AVX_OPS

class Matrix;

#ifdef VECTOR_AVX_ALIGN
inline void * operator new(size_t size)        // your operator new might
{
    return _mm_malloc(size, 32);
}

inline void operator delete(void *rawMemory) {
    _mm_free(rawMemory);
}
#endif


/**Class for describing points, normal vectors, direction vectors in 3D space.
  *@author Micha Riser
  */
class Vector3 {

public: // Static methods ("constants")

    /**Returns vector describing (positive) infinit point.
      */
    static Vector3 positiveInfinit() {return Vector3(DBL_INFINITY);}
    
    /**Returns vector describing (negative) infinit point.
      */    
    static Vector3 negativeInfinit() {return Vector3(-DBL_INFINITY);}

public: // Constructors and destructor

    /**Create uninitialized vector.
      */
    Vector3() {
        a[0] = a[1] = a[2] = a[3] = 0;
    }

    /**Create vector with same coifficients.
      *@param w value for all coifficients.
      */
    Vector3(DBL w) {
#ifndef VECTOR_AVX_OPS
        for(int i=0; i<3; i++) a[i]=w;
#ifdef VECTOR_AVX
        a[3] = 0;
#endif
#else
        _v = _mm256_set1_pd(w);
#endif
    }

    /**Create vector from given coifficients.
      */
    Vector3(DBL x, DBL y, DBL z) {
#ifndef VECTOR_AVX_OPS
        a[0]=x; a[1]=y; a[2]=z;
#ifdef VECTOR_AVX
        a[3] = 0;
#endif
#else
        _v = _mm256_set_pd(0,z,y,x);
#endif
    }

public: // Public methods


    /**Access to the vector components.
      */
    DBL& operator[](int i) { return a[i]; }
    
    /**Access to the const vector's components.
      */
    const DBL& operator[](int i) const {return a[i];}
    
    /**Add vector.
      */
    void add(const Vector3 &v) {
#ifndef VECTOR_AVX_OPS
        for(int i=0; i<3; i++) a[i]+=v[i];
#else
        _v = _mm256_add_pd(_v, v._v);
#endif
    }
    
    /**Subtract vector.
      */
    void sub(const Vector3 &v) {
#ifndef VECTOR_AVX_OPS
        for(int i=0; i<3; i++) a[i]-=v[i];
#else
        _v = _mm256_sub_pd(_v, v._v);
#endif
    }
    
    /**Scale vector by scalar.
      */
    void scale(DBL s) {
#ifndef VECTOR_AVX_OPS
        for(int i=0; i<3; i++) a[i]*=s;
#else
        _v = _mm256_mul_pd(_v, _mm256_set1_pd(s));
#endif
    }

    /**Scale vector non-uniformly in its component.
      */
    void scale(const Vector3& v) {
#ifndef VECTOR_AVX_OPS
        for(int i=0; i<3; i++) (*this)[i]*=v[i];
#else
        _v = _mm256_mul_pd(_v, v._v);
#endif
    }
    
    /**Normalize vector. Scale vector so that its length is 1.
      *Note: Checking for 0-vector is not done here!!
      */
    void normalize() {scale(1.0/length());}
    
    /**Inverse vector componentwise. Calculates 1/a for all coifficients.
      */      
    void inverse() {
#ifndef VECTOR_AVX_OPS
        for(int i=0; i<3; i++) a[i]=1/a[i];
#else
        _v = _mm256_div_pd(_mm256_set1_pd(1.0), _v);
#endif
    }
    
    /**Calculate v -> M*v.
      *@param m matrix M
      */
    void applyFromLeft(const Matrix &m);
    
    /**Calculate v -> M^T * v.
      *@param m matrix M
      */
    void applyFromLeftTransposed(const Matrix &m);
    
    /**Rotates point corresponding to the vector around the origin by given x,y,z angles.
      */
    void rotate(const Vector3& v);
    
    /**Minimize vector components.
      */
    void minimize(const Vector3 &v) {
#ifndef VECTOR_AVX_OPS
        for(int i=0; i<3; i++) if (v[i]<a[i]) a[i]=v[i];
#else
        _v = _mm256_min_pd(_v, v._v);
#endif
    }
    
    /**Maximize vector components.
      */
    void maximize(const Vector3 &v) {
#ifndef VECTOR_AVX_OPS
        for(int i=0; i<3; i++) if (v[i]>a[i]) a[i]=v[i];
#else
        _v = _mm256_max_pd(_v, v._v);
#endif
    }
    
    /**
      *@returns vector scaled by scalar s
      */
    Vector3 scaled(DBL s) const {
#ifndef VECTOR_AVX_OPS
        Vector3 ret;
        for(int i=0; i<3; i++) ret[i] = a[i]*s;
        return ret;
#else
        return Vector3(_mm256_mul_pd(_v, _mm256_set1_pd(s)));
#endif
    }
    
    /**
      *@returns square of the length
      */
    double lengthSq() const {
#ifndef VECTOR_AVX_OPS
        return a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
#else
        __m256d m = _mm256_mul_pd(_v, _v);
        __m256d sum = _mm256_hadd_pd(m, m);
        __m128d sum_high = _mm256_extractf128_pd(sum, 1);
        __m128d result = _mm_add_sd(sum_high, _mm256_castpd256_pd128(sum));
        return _mm_cvtsd_f64(result);
#endif
    }
    
    /**
      *@returns length of vector
      */
    double length() const {return sqrt(lengthSq());}
    
    /**Compare vector to other vector.
      */
    bool equals(const Vector3& v) const {
        for(int i=0; i<3; i++)
            if((*this)[i]!=v[i]) return false;
        return true;
    }

    /** Calucates hash-value for the vector.
      */    
    unsigned int hashValue() const {
        unsigned int res = 0;
        for(int i=0; i<3; i++) {
            const long long* p = reinterpret_cast<const long long*>(&((*this)[i]));
            unsigned int val = ((*p)>>38) ^ ((*p)>>49);
            res = (res<<4) ^ val;
        }
        return res;
    }

    // operator methods:

    /**Introduce order between vectors.
      */
    bool operator<(const Vector3& v) const {
        for(int i=0; i<3; i++) {
            if ((*this)[i]<v[i])
                return true;
            else
                if ((*this)[i]>v[i]) return false;
        }
        return false;
    }

    /**Same as equals.
      */
    bool operator==(const Vector3& v) const {return equals(v);}
    
    /**Same as add.
      */
    void operator+=(const Vector3& v) {add(v);}
    
    /**Same as sub.
      */
    void operator-=(const Vector3& v) {sub(v);}

    /**Same as scale
      */
    void operator*=(DBL s) {scale(s);}
    
public: // Static methods:

    /**Calculate dot product of two vectors.
      *@returns u * v
      */
    static DBL dotProd(const Vector3& v, const Vector3 &w) {
#ifndef VECTOR_AVX_OPS
        DBL ret = 0;
        for(int i=0; i<3; i++) ret+=v[i]*w[i];
        return ret;
#else
        __m256d m = _mm256_mul_pd(v._v, w._v);
        __m256d sum = _mm256_hadd_pd(m, m);
        __m128d sum_high = _mm256_extractf128_pd(sum, 1);
        __m128d result = _mm_add_sd(sum_high, _mm256_castpd256_pd128(sum));
        return _mm_cvtsd_f64(result);
#endif
    }

#ifdef VECTOR_AVX_OPS
    static __m256d dotProd4(
            const Vector3& v0, const Vector3 &w0,
            const Vector3& v1, const Vector3 &w1,
            const Vector3& v2, const Vector3 &w2,
            const Vector3& v3, const Vector3 &w3
            ) {
            __m256d m0 = _mm256_mul_pd(v0._v, w0._v);
            __m256d m1 = _mm256_mul_pd(v1._v, w1._v);
            __m256d m2 = _mm256_mul_pd(v2._v, w2._v);
            __m256d m3 = _mm256_mul_pd(v3._v, w3._v);

            __m256d temp01 = _mm256_hadd_pd( m0, m1 );
            __m256d temp23 = _mm256_hadd_pd( m2, m3 );

            // low to high: xy02+xy03 xy12+xy13 xy20+xy21 xy30+xy31
            __m256d swapped = _mm256_permute2f128_pd( temp01, temp23, 0x21 );

            // low to high: xy00+xy01 xy10+xy11 xy22+xy23 xy32+xy33
            __m256d blended = _mm256_blend_pd(temp01, temp23, 0b1100);

            return _mm256_add_pd( swapped, blended );
        }
#endif

    /**Calculate cross prodcut of two vectors.
      *@returns u x v
      */
    static Vector3 crossProd(const Vector3& u, const Vector3& v) {
        Vector3 res;
        res[0] = u[1]*v[2] - u[2]*v[1];
        res[1] = u[2]*v[0] - u[0]*v[2];
        res[2] = u[0]*v[1] - u[1]*v[0];
        return res;
    }
    
    /**Add two vectors.
      */
    static Vector3 add(const Vector3& v, const Vector3 &w) {
#ifndef VECTOR_AVX_OPS
        Vector3 ret;
        for(int i=0; i<3; i++) ret[i] = v[i]+w[i];
        return ret;
#else
        return Vector3(_mm256_add_pd(v._v, w._v));
#endif
    }
    
    /**Subtract two vectors.
      */
    static Vector3 sub(const Vector3& v, const Vector3 &w) {
#ifndef VECTOR_AVX_OPS
        Vector3 ret;
        for(int i=0; i<3; i++) ret[i] = v[i]-w[i];
        return ret;
#else
        return Vector3(_mm256_sub_pd(v._v, w._v));
#endif
    }

    Vector3 operator*(DBL s) const {
        return scaled(s);    
    }

    /**Calculate distance between two points given as vectors.
      */
    static DBL distance(const Vector3& v, const Vector3 &w) {
        return sub(v,w).length();
    }

    /**Transforms a vector with rotation angles in degree into
      *a corresponding one in radiants
      */
    static Vector3 deg2rad(const Vector3& v) {
        return v.scaled(PI/180.0);
    }
    
    /**Transforms a vector with rotation angles in radiant into
      *a corresponding one in degrees
      */
    static Vector3 rad2deg(const Vector3& v) {
        return v.scaled(180.0/PI);
    }
    
private: // Private members
public:
#ifdef VECTOR_AVX
/*  Vector3(__m256d v): _v(/v) {}
  union {
      __m256d _v;*/
      DBL a[4];
//  };
#else
  DBL a[3];
#endif

};

// Output Vector to stream
std::ostream& operator<<(std::ostream&s, const Vector3& v);



/** Single-precision version of vector. For storage purposes only.*/
class Vector3f {

public: // public methods

    // Constructors
    
    /** Constructor without initialisation. */
    Vector3f() {}

    /**Create vector from given coifficients.
     */
    Vector3f(FLT x, FLT y, FLT z) {
        a[0]=x; a[1]=y; a[2]=z;
    }

    /** Constructor from DBL Vector3. */
    Vector3f(const Vector3& v) {
        for(int i=0; i<3; i++) a[i] = v[i];
    }

    /** @returns a DBL-valued Vector3. */
    Vector3 toVector3() const {
        Vector3 ret;
        for(int i=0; i<3; i++) ret[i] =a[i];
        return ret;
    }

    /**Access to the vector components.
      */
    FLT& operator[](int i) {return a[i];}

    /**Access to the const vector's components.
      */
    const FLT& operator[](int i) const {return a[i];}

    /** Comparing against other FLT-vector.
      * @returns true if its equal to the given vector else false
      */
    bool equals(const Vector3f& v) const {
        for(int i=0; i<3; i++)
            if(a[i]!=v[i]) return false;
        return true;
    }

    /** Calucates hash-value for the vector.
      */
    unsigned int hashValue() const {
        unsigned int res = 0;
        for(int i=0; i<3; i++) {
            const unsigned int* p = reinterpret_cast<const unsigned int*>(&(a[i]));
            unsigned int val = ((*p)>>9) ^ ((*p)>>22);
            res = (res<<4) ^ val;
        }
        return res;
    }
    
    /**Calculate dot product of two vectors.
     *@returns u * v
     */
    static FLT dotProd(const Vector3f& v, const Vector3f &w) {
        FLT ret = 0;
        for (int i = 0; i < 3; i++)
            ret += v[i] * w[i];
        return ret;
    }

private:
    FLT a[3];

};

#endif
