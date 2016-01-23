/* *************************************************************************
                          prime.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: prime.h,v 1.2 2002/11/30 18:02:03 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef PRIME_H
#define PRIME_H

/**Class for finding prime numbers (not optimized for speed).
  *@author Micha Riser
  */

class Prime {
    
private: // disallow instantiation of Prime class
    Prime();
    ~Prime();

public: // Public static methods

    /**Tests if v is prime.
      */
    static bool isPrime(int v);

    /**Finds next prime bigger than v.
      */
    static int nextPrimeBiggerThan(int v);

    /**Finds next prime smaller than v. Returns 2 if no smaller prime exists.
      */
    static int nextPrimeSmallerThan(int v);
};

#endif
