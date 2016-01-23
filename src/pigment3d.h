/* *************************************************************************
                          pigment3d.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: pigment3d.h,v 1.9 2003/07/21 23:41:51 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */
 
#ifndef PIGMENT3D_H
#define PIGMENT3D_H

#include "../src/colour.h"
#include "../src/function.h"
#include "../src/intervalmap.h"
#include "../src/pattern.h"
#include "../src/transparenttransformatable.h"
#include "../src/vector.h"

/**Describes a colour for each point in space.
  *@author Micha Riser
  */
class Pigment3D: public Function<Vector3, ColourA>, public TransparentTransformatable {

public: // Constructors and destructor
    Pigment3D() {}
    virtual ~Pigment3D() {}

public: // Public pure virtual methods
    /**Copy pigment.
      */
    virtual Pigment3D* copy() const = 0;

protected:
    Pigment3D(const Pigment3D& p): Function<Vector3, ColourA>(), TransparentTransformatable(p) {}

private:
    Pigment3D& operator=(const Pigment3D&);

};


/**Single-coloured pigment.
  *@author Micha Riser
  */
class Pigment3DConstant: public Pigment3D  {

public: // Constructors and destructor
    Pigment3DConstant(const ColourA& c): colour(c) {}
    ~Pigment3DConstant() {}

public: // Public methods

    // Inherited from Pigment3D
    ColourA evaluateAt(const Vector3&) const {return colour;}
    Pigment3D* copy() const {return new Pigment3DConstant(colour);}

    // Inherited from Transformatable
    void translate(const Vector3&) {}
    void scale(const Vector3&) {}
    void rotate(const Vector3&) {}
    void applyOrthogonalMatrix(const Matrix&) {}

private: // Private attributes
    ColourA colour;

};


/**Patterned pigment.
  *@author Micha Riser
  */
  
class Pigment3DMapped: public Pigment3D, public IntervalMap<Pigment3D, ColourA> {

public: // Constructor and destructor
    Pigment3DMapped(const pattern::Pattern* p):
      Pigment3D(), IntervalMap<Pigment3D, ColourA>(), pattern(p) {};

    /**Copy constructor.
      */
    Pigment3DMapped(const Pigment3DMapped& p): Pigment3D(p), IntervalMap<Pigment3D, ColourA>(p),
      pattern(p.pattern->copy()) {};      

    ~Pigment3DMapped();

public: // Public methods
    ColourA evaluateAt(const Vector3&) const;
    Pigment3D* copy() const;

    void insertCol(FLT value, const ColourA& obj) {
        insert(value,new Pigment3DConstant(obj));
    }


private:
    const pattern::Pattern* pattern;

};  

#endif
