/* *************************************************************************
                          pattern.cpp  -  description
                             -------------------
    begin                : Fri Oct 18 2002
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: pattern.cpp,v 1.5 2003/07/21 23:41:51 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/pattern.h"

#include "../src/noise.h"

namespace pattern {

 const DBL Pattern::SPACE_EPSILON = 1e-10;

 FLT Checker::evaluateAt(const Vector3& v) const {

     int ret = 0;
     for(int i=0; i<3; i++) ret = ret + (int)floor(v[i]+SPACE_EPSILON);
     if (ret % 2) return 1.0; else return 0.0;

 }

 Pattern* Checker::copy() const {
     return new Checker(*this);
 }

 FLT Gradient::evaluateAt(const Vector3& v) const {

     double ret = Vector3::dotProd(v,gradient);
     return ret - floor(ret);

 }

 Pattern* Gradient::copy() const {
     return new Gradient(*this);
 }


 FLT Dents::evaluateAt(const Vector3& v) const {
     return pow3(noisegen->eval01(v));

 }

 Pattern* Dents::copy() const {
     return new Dents(*this);
 }

 FLT Granite::evaluateAt(const Vector3& v) const {
     
     Vector3 p = v.scaled(4);
     DBL noise = 0;
     DBL freq = 1;

     for(CNT8 i=0; i<6; i++, freq *= 2) {
         DBL tmp = 0.28*noisegen->evalN(p.scaled(freq));
         tmp = fabs(tmp);
         if (tmp > 0.5) tmp = 0.5;
         noise += tmp / freq;         
     }
     
    return noise;

 }

 Pattern* Granite::copy() const {
    return new Granite(*this);
 }

 FLT Wrinkles::evaluateAt(const Vector3& v) const {

     DBL value = (0.22*noisegen->evalN(v)+.5);
     DBL lambda = 2;
     DBL omega = 0.5;

     for(CNT8 i=0; i<10; i++) {

         value += omega*(0.22*noisegen->evalN(v.scaled(lambda))+.5);
         lambda *= 2;
         omega /= 2;
     }

    return clip01(value/2);

 }

 Pattern* Wrinkles::copy() const {
    return new Wrinkles(*this);
 }
 

}
