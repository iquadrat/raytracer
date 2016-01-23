/* *************************************************************************
                          object3d.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: object3d.cpp,v 1.13 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/object3d.h"

#include "../src/intersection.h"
#include "../src/material3d.h"
#include "../src/rendererror.h"

Object3D::Object3D(const Object3D &o): SurfacePatch3D(o), TransparentTransformatable(o) {
    if (o.material) material = new Material3D(*o.material); else material = 0;
    if (o.boundingbox) boundingbox = new BoundingBox(*o.boundingbox); else boundingbox = 0;
    inverse = o.inverse;
}

Object3D::~Object3D() {
    delete(material);
    delete(boundingbox);
}

void Object3D::setMaterial(const Material3D &m) {
    delete(material);
    material = new Material3D(m);  // transform ?
}

void Object3D::setTexture(const Texture3D &t) {
    if (!material) material = new Material3D();
    delete(material->texture);
    material->texture = new Texture3D(t);   // transform ?
}

void Object3D::setPigment(const Pigment3D &p) {
    if (!material) material = new Material3D();
    if (!material->texture) material->texture = new Texture3D(0,0);
    delete(material->texture->pigment);
    material->texture->pigment = p.copy();
    material->texture->pigment->transformInverse(*this);
}

void Object3D::setFinish(const Finish &f) {
    if (!material) material = new Material3D();
    if (!material->texture) material->texture = new Texture3D(0,0);
    delete(material->texture->finish);
    material->texture->finish = new Finish(f);
}

void Object3D::setInterior(const Interior &i) {
    if (!material) material = new Material3D();
    delete(material->interior);
    material->interior = new Interior(i); 
}

void Object3D::setMedia(const Media3D &m) {
    if (!material) material = new Material3D();
    if (!material->interior) material->interior = new Interior();
    delete (material->interior->media) ;
    material->interior->media = m.copy();
}

void Object3D::translate(const Vector3 &v) {
    TransparentTransformatable::translate(v);
    if (boundingbox) boundingbox->translate(v);
}

void Object3D::scale(const Vector3& v) {
    TransparentTransformatable::scale(v);
    if (boundingbox) boundingbox->scale(v);
}

void Object3D::rotateRad(const Vector3 &v) {
    Matrix rot = Matrix::generateRotationMatrix(v);
    TransparentTransformatable::applyOrthogonalMatrix(rot);
    if (boundingbox) boundingbox->applyOrthogonalMatrix(rot);
}

void Object3D::rotateDeg(const Vector3 &vrad) {
    rotateRad(Vector3::deg2rad(vrad));
}


void Object3D::applyOrthogonalMatrix(const Matrix& m) {
    if (boundingbox) boundingbox->applyOrthogonalMatrix(m);
    TransparentTransformatable::applyOrthogonalMatrix(m);
}

void Object3D::applyMatrix(const Matrix& m) {
    if (boundingbox) boundingbox->applyMatrix(m);
    TransparentTransformatable::applyMatrix(m);
}

void Object3D::transform(const TransparentTransformatable& t) {
    throw RenderError("method Object3D::transform not implemented yet");
}

void Object3D::transformInverse(const TransparentTransformatable& t) {
    throw RenderError("method Object3D::transformInverse not implemented yet");
}
   
void Object3D::listInside(const Vector3& location, InsideList& l) const {
    if (material && material->interior && isInsideAtBounded(location)) l.add(this);
}

DBL Object3D::intersectBounding(const Ray &r) const {
    if (boundingbox == 0) return INTERSECTION_TIME_EPSILON;
    return boundingbox->intersect(r);
}

DBL Object3D::intersectBounding(const Ray &r, DBL tmax) const {
    if (boundingbox == 0) return INTERSECTION_TIME_EPSILON;
    return boundingbox->intersect(r,tmax);
}

