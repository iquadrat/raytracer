/* *************************************************************************
                            vector.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002  
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

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

#include "../src/types.h"

class Matrix;

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
    Vector3() {}

    /**Create vector with same coifficients.
      *@param w value for all coifficients.
      */
    Vector3(DBL w) {for(int i=0; i<3; i++) a[i]=w;}

    /**Create vector from given coifficients.
      */
    Vector3(DBL x, DBL y, DBL z) {
        a[0]=x; a[1]=y; a[2]=z;
    }

public: // Public methods


    /**Access to the vector components.
      */
    DBL& operator[](int i) {return a[i];}
    
    /**Access to the const vector's components.
      */
    const DBL& operator[](int i) const {return a[i];}
    
    /**Add vector.
      */
    void add(const Vector3 &v) {for(int i=0; i<3; i++) a[i]+=v[i];}
    
    /**Subtract vector.
      */
    void sub(const Vector3 &v) {for(int i=0; i<3; i++) a[i]-=v[i];}
    
    /**Scale vector by scalar.
      */
    void scale(DBL s) {for(int i=0; i<3; i++) a[i]*=s;}

    /**Scale vector non-uniformly in its component.
      */
    void scale(const Vector3& v) {for(int i=0; i<3; i++) a[i]*=v[i];}
    
    /**Normalize vector. Scale vector so that its length is 1.
      *Note: Checking for 0-vector is not done here!!
      */
    void normalize() {scale(1.0/length());}
    
    /**Inverse vector componentwise. Calculates 1/a for all coifficients.
      */      
    void inverse() {for(int i=0; i<3; i++) a[i]=1/a[i];}
    
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
    void minimize(const Vector3 &v) {for(int i=0; i<3; i++) if (v[i]<a[i]) a[i]=v[i];}
    
    /**Maximize vector components.
      */
    void maximize(const Vector3 &v) {for(int i=0; i<3; i++) if (v[i]>a[i]) a[i]=v[i];}
    
    /**
      *@returns vector scaled by scalar s
      */
    Vector3 scaled(DBL s) const {
        Vector3 ret;
        for(int i=0; i<3; i++) ret[i] = a[i]*s;
        return ret;
    }
    
    /**
      *@returns square of the length
      */
    double lengthSq() const {return a[0]*a[0]+a[1]*a[1]+a[2]*a[2];}
    
    /**
      *@returns length of vector
      */
    double length() const {return sqrt(lengthSq());}
    
    /**Compare vector to other vector.
      */
    bool equals(const Vector3& v) const {
        for(int i=0; i<3; i++)
            if(a[i]!=v[i]) return false;
        return true;
    }

    /** Calucates hash-value for the vector.
      */    
    unsigned int hashValue() const {
        unsigned int res = 0;
        for(int i=0; i<3; i++) {
            const long long* p = reinterpret_cast<const long long*>(&(a[i]));
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
            if (a[i]<v[i])
                return true;
            else
                if (a[i]>v[i]) return false;
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
        DBL ret = 0;
        for(int i=0; i<3; i++) ret+=v[i]*w[i];
        return ret;
    }

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
        Vector3 ret;
        for(int i=0; i<3; i++) ret[i] = v[i]+w[i];
        return ret;
    }
    
    /**Subtract two vectors.
      */
    static Vector3 sub(const Vector3& v, const Vector3 &w) {
        Vector3 ret;
        for(int i=0; i<3; i++) ret[i] = v[i]-w[i];
        return ret;
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
    DBL a[3];

};

// Output Vector to stream
std::ostream& operator<<(std::ostream&s, const Vector3& v);



/** Single-precision version of vector. For storage purposes only.*/
class Vector3f {

public: // public methods

    // Constructors
    
    /** Constructor without initialisation. */
    Vector3f() {}

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
    
private:
    FLT a[3];

};

#endif
