/* *************************************************************************
                          transparenttransformatable.h  -  description
                             -------------------
    begin                : Fri Jan 17 2003
    copyright            : (C) 2003 by Micha Riser
    email                : micha@povworld.org
    
    $Id: transparenttransformatable.h,v 1.7 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef TRANSPARENTTRANSFORMATABLE_H
#define TRANSPARENTTRANSFORMATABLE_H

#include "../src/matrix.h"
#include "../src/ray.h"
#include "../src/transformatable.h"

/**Implements the Transformatable interface in such a way that it hides the
  *transformation from its subclasses.
  *@author Micha Riser
  */  
class TransparentTransformatable: public Transformatable  {
    
public: // Constructors and destructor

    /**Create untransformated TransparentTransformatable.*/
    TransparentTransformatable():
      translation(0),transformation(0),inverse(0) {}

    /**Create TransparentTransformatable at given location.*/
    TransparentTransformatable(const Vector3& loc):
      translation(loc),transformation(0),inverse(0) {}

    /**Copy constructor.*/
    TransparentTransformatable(const TransparentTransformatable &t): 
      Transformatable(t), translation(t.translation), transformation(0), inverse(0) {
          
        if (t.transformation) {
            transformation = new Matrix(*t.transformation);
            inverse = new Matrix(*t.inverse);
        } 

    }
    
    /**Destructor.*/
    virtual ~TransparentTransformatable() {
        delete(transformation);
        delete(inverse);
    }

private:
    TransparentTransformatable& operator=(const TransparentTransformatable);

public: // Methods that implement Transformatable

    virtual void translate(const Vector3& v) {translation.add(v);}    
    virtual void scale(const Vector3& v);    
    virtual void rotateDeg(const Vector3& v);
    virtual void rotateRad(const Vector3& v);    
    virtual void applyOrthogonalMatrix(const Matrix& m);
    virtual void applyMatrix(const Matrix& m);

public: // Additional transformation methods

    /**Transform the same way as given Transformatable.
      */
    virtual void transform(const TransparentTransformatable& t);

    /**Transform inverse as given Transformatble.
      */
    virtual void transformInverse(const TransparentTransformatable& t);
    
public: // Methods to do space transformations    

    /**Transform point into local space.
      */            
    void transformPointIn(Vector3 &p) const {
        // p' = T*(p-t)
        p.sub(translation);
        if (transformation) p.applyFromLeft(*transformation);
    }

    /**Transform point out of local space.
      */
    void transformPointOut(Vector3 &p) const {
        // p = T*p'+t  /* *check me* for correctness */
        if (transformation) p.applyFromLeft(*inverse);
        p.add(translation);
    }

    /**Transform normal vector into local space.
      */    
//    void transformNormalIn(Vector3 &p) const;

    /**Transform normal vector from local space to outer space.
      */
    void transformNormalOut(Vector3 &p) const {
        if (transformation) p.applyFromLeftTransposed(*transformation);
    }

    /**Transform ray into local space.
      */
    void transformRayIn(Ray &r) const {
        r.origin.sub(translation);
        if (transformation) {
            r.origin.applyFromLeft(*transformation);
            r.direction.applyFromLeft(*transformation);
        }
    }


public: // Functions that give read access to transformations    
    /**
      *@returns translation vector
      */
    Vector3 getTranslation() const {return translation;}

    /**
      *@returns pointer to transformation matrix
      */
    const Matrix* getTransformationMatrix() const {return transformation;}

    /**
      *@returns pointer to inverse transformation matrix
      */
    const Matrix* getInverseMatrix() const {return inverse;}
    
protected: // Functions that give write access to transformations
    /**Clears all transformation information.
      */
    void clearTransformation() {
        delete(transformation);
        delete(inverse);
        transformation = 0;
        inverse = 0;
        translation = Vector3(0);
    }

    /**Change translation vector.
      */
    void setTranslation(const Vector3& v) {
        translation = v;
    }
    
private: // Protected attributes

    /**Translation vector. Always present.
      */
    Vector3 translation;

    /**Transformation matrix. May be 0.
      */
    Matrix* transformation;

    /**Inverse matrix of transformation. Is 0 iff no transformation.
      */
    Matrix* inverse;

};

#endif
