/* *************************************************************************
                          object3d.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: object3d.h,v 1.24 2003/05/25 11:25:52 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "../src/boundingbox.h"
#include "../src/objectconstructionerror.h"
#include "../src/surfacepatch3d.h"
#include "../src/transparenttransformatable.h"

class PatchObject;
class InsideList;

class Material3D;
class Texture3D;
class Pigment3D;
class Finish;
class Interior;
class Media3D;

/**Superclass of all objects. As enhancement to SurfacePatch3D the Object3D
  *class has a well-defined interior and therefore have to provide a
  *function which defines if a point is inside or not.
  *@author Micha Riser
  */

class Object3D: public SurfacePatch3D, public TransparentTransformatable {

public: // Public attributes
    /**The object's material.
      */
    Material3D* material;

public: // Constructors and destructor

    /**Constructs an object at origin. Material defaults to 0 and
      *object not inversed.
      */
    Object3D(): material(0), boundingbox(0), inverse(false) {};

    /**Constructs an object at given position. Material defaults to 0 and
      *object not inversed.
      *@param location Position to create the object.
      */
    Object3D(Vector3 location): TransparentTransformatable(location),
      material(0), boundingbox(0), inverse(false) {};

    virtual ~Object3D();
    
protected:
    /**Copy constructor.
      */
    Object3D(const Object3D &o);

private:
    /**Objects cannot be assigned with the '=' operator
      */
    Object3D& operator=(const Object3D&);
    
public: // Public methods

    // Methods used during pre-rendering stage

    // overwrite the transformation in TransparentTranformatable to get the
    // boundingbox transformed as well
    virtual void translate(const Vector3& v);
    virtual void scale(const Vector3& v);
    virtual void rotateDeg(const Vector3& v);
    virtual void rotateRad(const Vector3& v);
    virtual void applyOrthogonalMatrix(const Matrix& m);
    virtual void applyMatrix(const Matrix& m);        
        
    virtual void transform(const TransparentTransformatable& t);
    virtual void transformInverse(const TransparentTransformatable& t);

    // to implement SurfacePatch3D:

    virtual bool hasFinitBounding() const {return boundingbox != 0;}

    virtual Vector3 getBoundingMin() const {
        if (boundingbox == 0) throw ObjectConstructionError("Object3D::getBoundingMin(): no bounding");
        return boundingbox->getBoxMin();
    }

    virtual Vector3 getBoundingMax() const {
        if (boundingbox == 0) throw ObjectConstructionError("Object3D::getBoundingMin(): no bounding");
        return boundingbox->getBoxMax();
    }

    virtual DBL intersectBounding(const Ray &r) const;
    virtual DBL intersectBounding(const Ray &r, DBL tmax) const;
    
    virtual Intersection* intersect(const Ray &r) const {
        Ray local = r;
        transformRayIn(local);
        return intersectLocal(local);
    }
    
    virtual Intersection* intersectLimitedTime(const Ray &r, DBL maxtime) const {
        Ray local = r;
        transformRayIn(local);
        return intersectLocalLimitedTime(local,maxtime);
    }


    // methods to set Object3D's material and sub-components

    /**Set / override object's material.
      *@param m new Material3D
      */
    void setMaterial(const Material3D &m);

    /**Set / override object's texture.
      *@param t new Texture3D
      */
    void setTexture(const Texture3D &t);

    /**Set / override object's pigment.
      *@param p new Pigment3D
      */
    void setPigment(const Pigment3D &p);

    /**Set / override object's finish.
      *@param f new Finish
      */
    void setFinish(const Finish &f);

    /**Set / override object's interior.
      *@param i new Interior
      */
    void setInterior(const Interior &i);

    /**Set / override object's media.
      *@param m new Media3D
      */
    void setMedia(const Media3D &m);

    /**Inverse the object.
      *@returns a copy of the object with inversed interior
      */
    virtual Object3D* invert() const {  
        Object3D* inv = static_cast<Object3D*>(duplicate());
        if (inverse) inv->inverse=false; else inv->inverse=true;
        return inv;
    }

    /**Creates an approximatly equivalent object out of 'patches'. Returns
      *@param precision max tolerance for the surface to be away from the original
      *@returns pointer to approximated object, 0 if method not implemented
      */
    virtual PatchObject* toPatch(double precision) const {return 0;}
    
    /**Create insidness list. Adds the object (or all subcomponents in the case
      *of a container object) to the given list if the location is inside.
      *@param location the test point
      *@param l list to add the object
      */
    virtual void listInside(const Vector3& location, InsideList& l) const;

    /**Test point for insidness.
      *@param location test point
      *@returns true if point is inside object else false
      */
    bool isInsideAtBounded(Vector3 location) const {
        if (boundingbox && !boundingbox->isInsideAt(location)) return inverse;        
        transformPointIn(location);
        return isInsideAt(location);
    }

    /**
      *@returns true if object is inversed else false
      */
    bool isInverse() const {return inverse;}

protected:
    // virtual methods for local object intersection
    virtual Intersection* intersectLocal(const Ray& r) const {
        return intersectLocalLimitedTime(r,INTERSECTION_INFINIT_TIME);
    }

    virtual Intersection* intersectLocalLimitedTime(const Ray& r, DBL maxtime) const = 0;


    /**Test point for insideness.
      *@param locallocation point in object space to test
      *@returns true if point is inside the object else false
      */
    virtual bool isInsideAt(const Vector3 &locallocation) const = 0;    

    
protected: // Protected attributes
    /**The object's bounding box.
      */
    BoundingBox* boundingbox;

    /**Holds the object's inverting status. True means inversed.
      */
    bool inverse;
    
};

#endif
