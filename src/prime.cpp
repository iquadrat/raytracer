/* *************************************************************************
                          prime.cpp  -  description
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

#include "../src/prime.h"

Prime::Prime(){
}
Prime::~Prime(){
}

bool Prime::isPrime(int v) {

    if ( v == 2 ) return true;
    if ( v%2 == 0 || v<2 ) return false;
    
    int test = 1;    
    do {
        test += 2;
        if (v % test == 0) return false;
    } while(test*test<v);
    return true;

}

int Prime::nextPrimeBiggerThan(int v) {
    
    v++;
    while( !isPrime(v) ) v++;
    return v;
    
}

int Prime::nextPrimeSmallerThan(int v) {
    
    if (v<=2) return 2;
    v--;
    while( !isPrime(v) ) v--;
    return v;
    
}

