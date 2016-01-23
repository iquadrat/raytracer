/* *************************************************************************
                          colour.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002   
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: colour.cpp,v 1.4 2003/03/03 11:44:05 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */


#include "../src/colour.h"

ColourA::ColourA(const Colour& a) {
    
    for(int i=0; i<3; i++) c[i] = a.c[i];
    c[3]=0;
    
}


void ColourA::scale(const Colour& d) {
    
    for(int i=0; i<3; i++) c[i] *= d.c[i];
    
} 
