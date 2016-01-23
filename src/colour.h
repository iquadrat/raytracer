/* *************************************************************************
                          colour.h  -  description
                             -------------------
    begin                : Wed Oct 15 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: colour.h,v 1.18 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */


#ifndef COLOUR_H
#define COLOUR_H

#include<cmath>

#include "../src/types.h"

class Colour;

/**Colour vector including an alpha (transparency) component.
  *@author Micha Riser
  */
class ColourA {

    friend class Colour;

private: // constants
    static const CNT8 RED = 0, GREEN = 1, BLUE = 2;
    static const CNT8 ALPHA = 3;    

public: // Constructors and destructor
    
    /**Constructs new colour form red, green, blue and alpha components.
      */
    ColourA(CLR r, CLR g, CLR b, CLR a) {
        c[0]=r; c[1]=g; c[2]=b; c[3]=a;
    }

    /**Copy constructor.
      */
    ColourA(const CLR& a) {
        for(int i=0; i<4; i++) c[i]=a;
    }

    /**Construct ColourA from Colour. Alpha component is set to 0.
      */
    ColourA(const Colour& d);

private: // Private constructor
    ColourA() {};
            
public: // Public methods

    CLR red() const {return c[RED];}
    CLR green() const {return c[GREEN];}
    CLR blue() const {return c[BLUE];}
    CLR alpha() const {return c[ALPHA];}


    /**Add ColourA. (What to do with alpha?)
      *@param d colour to add
      */
    void add(const ColourA &d) {for(int i=0; i<4; i++) c[i] += d.c[i];}

    /**Subtract ColourA. (What to do with alpha?)
      *@param d colour to subtract
      */
    void sub(const ColourA &d) {for(int i=0; i<4; i++) c[i] -= d.c[i];}

    /**Scale colour by float.
      *@param s scaling coifficient
      */
    void scale(CLR s) {for(int i=0; i<4; i++) c[i] *= s;}

    /**Scale colour componentwise. (What to do with alpha?)
      *@param d ColourA holding the scaling coifficients
      */
    void scale(const ColourA& d) {for(int i=0; i<4; i++) c[i] *= d.c[i];}

    /**Scale rgb components componentwise. Alpha is not changed.
      *@param d ColourA holding the scaling coifficients
      */
    void scale(const Colour& d);

    /**
      *@returns colour scaled by factor
      *@param s scaling factor
      */
    ColourA scaled(CLR s) const {ColourA ret; for(int i=0; i<4; i++) ret.c[i]=c[i]*s; return ret;}

    /**Colour norm.
      *@returns a (unspecified) norm of the colour
      */
    CLR norm() const {
        // calculate maximum (infinity) norm / what with alpha?
        CLR ret = c[0];
        for(int i=1; i<3; i++) if (c[i]>ret) ret = c[i];
        return ret;
    }

private: // Private attributes
    CLR c[4]; // colour RGBA

};


/**Colour vector with red, green and blue component.
  *@author Micha Riser
  */
class Colour {

    friend class ColourA;

public: // Private constants
    static const char RGB_LOOP = 3;
private:    
    static const CNT8 RED = 0, GREEN = 1, BLUE = 2;
    
public: // Constructors and destructor

    /**Construct colour by individual components
      */
    Colour(CLR r, CLR g, CLR b) {c[0]=r; c[1]=g; c[2]=b;}

    /**Construct gray-value colour.
      *@param a value for all the rgb components
      */
    Colour(const CLR& a) {for(int i=0; i<RGB_LOOP; i++) c[i]=a;}

    /**Construct rgb colour from ColourA. Alpha value is just ignored.
      */
    Colour(const ColourA& a) {for(int i=0; i<RGB_LOOP; i++) c[i]=a.c[i];}

private:
    /**Construct uninitialized colour.
      */
    Colour() {};

public: // Public methods

    CLR red() const {return c[RED];}
    CLR green() const {return c[GREEN];}
    CLR blue() const {return c[BLUE];}

    /**Add colour.
      *@param d colour to add
      */
    void add(const Colour &d) {for(int i=0; i<RGB_LOOP; i++) c[i] += d.c[i];}

    /**Add colour.
      *@param d colour to add
      */
    void add(const ColourA &d) {for(int i=0; i<RGB_LOOP; i++) c[i] += d.c[i];}

    /**Subtract colour.
      *@param d colour to subtract
      */
    void sub(const Colour &d) {for(int i=0; i<RGB_LOOP; i++) c[i] -= d.c[i];}

    /**Scale colour.
      *@param s scaling factor
      */
    void scale(CLR s) {for(int i=0; i<RGB_LOOP; i++) c[i] *= s;}

    /**Scale colour componentwise.
      *@param d scaling factors
      */
    void scale(const ColourA& d) {for(int i=0; i<RGB_LOOP; i++) c[i] *= d.c[i];}

    /**Scale colour componentwise.
      *@param d scaling factors
      */
    void scale(const Colour& d) {for(int i=0; i<RGB_LOOP; i++) c[i] *= d.c[i];}

    /**Calculate e^x for each colour components x.
      */
    void exp() {for(int i=0; i<RGB_LOOP; i++) c[i] = std::exp(c[i]);}

    /**Calculate e^(-x) for each colour components x.
      */
    void negExp() {for(int i=0; i<RGB_LOOP; i++) c[i] = std::exp(-c[i]);}

    /**
      *@returns colour scaled by factor
      *@param s scaling factor
      */
    Colour scaled(CLR s) const {Colour ret; for(int i=0; i<RGB_LOOP; i++) ret.c[i]=c[i]*s; return ret;}

    /**
      *@returns componentwise scaled colour
      *@param d scaling factors
      */
    Colour scaled(const Colour &d) const {
        Colour ret;
        for(int i=0; i<RGB_LOOP; i++) ret.c[i] = c[i]*d.c[i];
        return ret;
    }

    Colour divided(const Colour& d) const {
        Colour ret;
        for(int i=0; i<RGB_LOOP; i++) ret.c[i] = c[i]/d.c[i];
        return ret;
    }
    
    /**
      *@returns componentwise scaled colour
      *@param d scaling factors
      */
    Colour scaled(const ColourA &d) const {
        Colour ret;
        for(int i=0; i<RGB_LOOP; i++) ret.c[i] = c[i]*d.c[i];
        return ret;
    }

    /**Colour norm
      *@returns a (unspecified) norm of the colour components
      */
    CLR norm() const {
        CLR ret = fabs(c[0]);
        for(int i=1; i<RGB_LOOP; i++) if (fabs(c[i])>ret) ret = fabs(c[i]);
        return ret;
    }

    /**Minimum.
      *@returns the minimal element of the colour vector
      */
    CLR minElement() const {
        CLR ret = c[0];
        for(int i=1; i<RGB_LOOP; i++) if (c[1]<ret) ret = c[1];
        return ret;
    }

    /**Maximum.
      *@returns the maximal element of the colour vector
      */
    CLR maxElement() const {
        CLR ret = c[0];
        for(int i=1; i<RGB_LOOP; i++) if (c[1]>ret) ret = c[1];
        return ret;
    }

public: // Static methods

    /**Adds two colours (componentwise).
      */
    static Colour add(const Colour& c1, const Colour& c2) {
        Colour ret;
        for(int i=0; i<RGB_LOOP; i++) ret.c[i]=c1.c[i]+c2.c[i];
        return ret;
    }

    Colour operator+(const Colour& c1) const {

        return add(*this,c1);
    }

    Colour operator*(const CLR d) const {

        return scaled(d);
    }

    Colour operator*(const Colour& d) const {

        return scaled(d);
    }

    Colour operator/(const Colour& d) const {

        return divided(d);
    }
    
    Colour operator-(const Colour& d) const {

        return sub(*this,d);
    }

    /**Subtracts colour c2 from colour c1 (componentwise).
      */
    static Colour sub(const Colour& c1, const Colour& c2) {
        Colour ret;
        for(int i=0; i<RGB_LOOP; i++) ret.c[i]=c1.c[i]-c2.c[i];
        return ret;
    }

    /**Calcluate the square root of a colour (componentwise).
      */
    static Colour sqrt(const Colour& d) {
        Colour ret;
        for(int i=0; i<RGB_LOOP; i++) ret.c[i]=std::sqrt(d.c[i]);
        return ret;
    }

    /**Calculate e^x for each colour component.
      */
    static Colour exp(const Colour& d) {
        Colour ret;
        for(int i=0; i<RGB_LOOP; i++) ret.c[i]=std::exp(d.c[i]);
        return ret;
    }

    static Colour negExp(const Colour& d) {
        Colour ret;
        for(int i=0; i<RGB_LOOP; i++) ret.c[i]=std::exp(-d.c[i]);
        return ret;
    }    


private: // Public members
    /** colour <Red, Green, Blue> */
    CLR c[RGB_LOOP];
    
};

#endif
