/* *************************************************************************
                          transparenttransformatable.cpp  -  description
                             -------------------
    begin                : Fri Jan 17 2003
    copyright            : (C) 2003 by Micha Riser
    email                : mriser@gmx.net
    
    $Id: transparenttransformatable.cpp,v 1.3 2003/01/29 21:56:43 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/transparenttransformatable.h"

void TransparentTransformatable::scale(const Vector3 &s) {

    if (transformation == 0) {

        inverse = new Matrix(Matrix::generateScaleMatrix(s));
        Vector3 v = s; v.inverse();
        transformation = new Matrix(Matrix::generateScaleMatrix(v));
        translation.applyFromLeft(*inverse);

    } else {

        Matrix S = Matrix::generateScaleMatrix(s);
        inverse->applyFromLeft(S);
        translation.applyFromLeft(S);
        Vector3 v = s; v.inverse();
        S = Matrix::generateScaleMatrix(v);
        transformation->applyFromLeft(S);

    }

}

void TransparentTransformatable::rotateDeg(const Vector3 &v) {
    applyOrthogonalMatrix(Matrix::generateRotationMatrix(Vector3::deg2rad(v)));
}

void TransparentTransformatable::rotateRad(const Vector3 &v) {
    applyOrthogonalMatrix(Matrix::generateRotationMatrix(v));
}


void TransparentTransformatable::applyOrthogonalMatrix(const Matrix& m) {

    if (transformation) {

        inverse->applyFromLeft(m);
        translation.applyFromLeft(m);
        Matrix tmp = m;
        tmp.transpose();
        transformation->applyFromLeft(tmp);

    } else {

        transformation = new Matrix(m);
        transformation->transpose();
        inverse = new Matrix(m);
        translation.applyFromLeft(m);

    }

}

void TransparentTransformatable::applyMatrix(const Matrix& m) {

    // *check me* for correctness

    if (transformation) {

        inverse->applyFromLeft(m);
        translation.applyFromLeft(m);
        Matrix tmp = m;
        tmp.invert();
        transformation->applyFromLeft(tmp);

    } else {

        transformation = new Matrix(m);
        transformation->invert();
        inverse = new Matrix(m);
        translation.applyFromLeft(m);

    }

}

void TransparentTransformatable::transform(const TransparentTransformatable& t) {
    // *check me* for correctness
    // unused

    translation.add(t.translation);
    if (t.transformation) {
        if (!transformation) {
            transformation = new Matrix(*t.transformation);
            inverse = new Matrix(*t.inverse);
        } else {
            transformation->applyFromLeft(*t.transformation);
            inverse->applyFromLeft(*t.inverse);
        }
    }
}

void TransparentTransformatable::transformInverse(const TransparentTransformatable& t) {
    // It is v = T(v'-t) where we have to solve for v':
    // v' = T^(-1) (v + T*t)
    // Therefore: T' = T^(-1), t' = -T*t

    Vector3 ts = t.translation;
    if (t.transformation) ts.applyFromLeft(*t.transformation);
    translation.sub(ts);

    if (t.transformation) {
        if (!transformation) {

            transformation = new Matrix(*t.inverse);
            inverse = new Matrix(*t.transformation);

        } else {

            transformation->applyFromLeft(*t.inverse);
            Matrix tmp = *inverse;
            *inverse = *t.transformation;
            inverse->applyFromLeft(tmp);

        }
    }
}

/*
void TransparentTransformatable::transformNormalIn(Vector3 &p) const {
    //unused
    if (inverse) p.applyFromLeftTransposed(*inverse);
}
*/
