/* *************************************************************************
                          pattern.h  -  description
                             -------------------
    begin                : Fri Oct 18 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: pattern.h,v 1.5 2003/07/21 23:41:51 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef PATTERN_H
#define PATTERN_H

#include "../src/function.h"
#include "../src/types.h"
#include "../src/vector.h"

template<class D, class R> class Noise;

namespace pattern {

 /**A pattern is a function which orders a value (in the range [0,1]) to each point in space.
   *@author Micha Riser
   */

 class Pattern: public Function<Vector3,FLT> {
 public: // Constructor and destructor
     Pattern() {}
     virtual ~Pattern() {}

 public: // Pure virtual functions
     virtual Pattern* copy() const = 0;

 protected: // Constants
     static const DBL SPACE_EPSILON;

 };


 /**Checkered Pattern.
   *Returns 0 for Points in "even" cubes, 1 in "uneven".
   *@author Micha Riser
   */
 class Checker: public Pattern {

 public: // Public methods

     //Inherited form Pattern
     FLT evaluateAt(const Vector3& v) const;
     Pattern* copy() const;

 };


 /**Gradient Pattern. Repeating pattern along given axis.
   *@author Micha Riser
   */
 class Gradient: public Pattern {
 public: // Constructor and destructor
     Gradient(const Vector3& g): gradient(g) {}
 public: // Public methods
     //Inherited form Pattern
     FLT evaluateAt(const Vector3& v) const;
     Pattern* copy() const;
 private: // Private attributes
     Vector3 gradient;
 };


 /**Dents Pattern.
   */
 class Dents: public Pattern {
 public: // Constructor and destructor
     Dents(const Noise<Vector3,DBL>* n): noisegen(n) {}
 public: // Public methods
     //Inherited form Pattern
     FLT evaluateAt(const Vector3& v) const;
     Pattern* copy() const;
 private: // Private attributes
     const Noise<Vector3,DBL>* noisegen;
 };

 /**Granite Pattern.
   */
 class Granite: public Pattern {
 public: // Constructor and destructor
     Granite(const Noise<Vector3,DBL>* n): noisegen(n) {}
 public: // Public methods
     //Inherited form Pattern
     FLT evaluateAt(const Vector3& v) const;
     Pattern* copy() const;
 private: // Private attributes
     const Noise<Vector3,DBL>* noisegen;
 };

 /**Wrinkles Pattern.
   */
 class Wrinkles: public Pattern {
 public: // Constructor and destructor
     Wrinkles(const Noise<Vector3,DBL>* n): noisegen(n) {}
 public: // Public methods
     //Inherited form Pattern
     FLT evaluateAt(const Vector3& v) const;
     Pattern* copy() const;
 private: // Private attributes
     const Noise<Vector3,DBL>* noisegen;
 };


} // end of namespace

#endif
