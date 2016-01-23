/* *************************************************************************
                            types.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef TYPES_H
#define TYPES_H

#if defined(__GNUC__) && (__GNUC__ < 3)

    // gcc 2.95 does not have limits
    // gcc handles access from inner classes to private attributes wrong
    #define WRONG_INNER_CLASSES_ACCESS
    // gcc 2.95 hast no limits include file
    
#else

    #define HAS_LIMITS
    
#endif


#ifdef HAS_LIMITS
    #include<limits>    
#endif

#include <algorithm>

/* ************************************************************
 *  Type definitions
 ************************************************************ */

/**Double precision floating point type. Used where high precision
  *is needed.
  */
typedef double DBL;

/**Single precison floating point type.*/
typedef float FLT;

/**Colour component type.*/
typedef float CLR;


/**Counter type for small loops (unsigned, at least 8bit)*/
typedef unsigned short int CNT8;

/**Counter type for general counting things like indexes (at least 16bit)*/
typedef unsigned int CNT;

/**Counter for reference counting of objects and such things
  *(unsigned, at least 16bit)
  */
typedef unsigned int CNTREF;


/* ************************************************************
 *  Global constants
 ************************************************************ */

static const DBL PI = 3.1415926535897932384626;

#ifdef HAS_LIMITS
static const DBL DBL_INFINITY = std::numeric_limits<DBL>::max();
#else
static const DBL DBL_INFINITY = 1.79769e308;
#endif

static const DBL DIRECTION_EPSILON = 1e-10;

static const DBL INTERSECTION_INFINIT_TIME = DBL_INFINITY;
static const DBL INTERSECTION_TIME_EPSILON = 1e-6;

class Scene;
extern Scene* globalscene; //tmp

/* ****************************************************************
 * Global functions
 **************************************************************** */

inline FLT rad2deg(FLT angle) {return angle * (180.0/PI);} // this will probably be converted to DBL for the calculation... cast the constant to float?
inline DBL rad2deg(DBL angle) {return angle * (180.0/PI);}

inline FLT deg2rad(FLT angle) {return angle * (PI/180.0);} // this will probably be converted to DBL for the calculation... cast the constant to float?
inline DBL deg2rad(DBL angle) {return angle * (PI/180.0);}

template<class T> static T sqr(T a) {return a*a;}
template<class T> static T pow3(T a) {return a*a*a;}
template<class T> static T pow4(T a) {return a*a*a*a;}
template<class T> static T pow5(T a) {return a*a*a*a*a;}

template<class T> static T clip01(T a) {return std::max(std::min((T)1,a),(T)0);}

/*inline DBL sqr(DBL a) {return a*a;}
inline FLT sqr(FLT a) {return a*a;}

inline DBL pow3(DBL a) {return a*a*a;}
inline FLT pow3(FLT a) {return a*a*a;}*/

#endif
