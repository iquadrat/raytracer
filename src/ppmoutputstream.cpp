/* *************************************************************************
                          ppmoutputstream.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: ppmoutputstream.cpp,v 1.6 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/ppmoutputstream.h"

PPMOutputStream::PPMOutputStream(const std::string& filename, int w, int h):
  xnext(0), ynext(0), width(w), height(h) {
      
    ofile = fopen(filename.c_str(), "w");
    fprintf(ofile,"P3\n%d %d\n65535\n",width,height);
      
}


void PPMOutputStream::putPixel(const Pixel& p) {

    if (p.x != xnext && p.y != ynext) {
        buffer.insert(p);
    } else {
        writeColour(p.c);
        next();
        if (buffer.empty()) return;
        Pixel q = *buffer.begin();
        while(q.x == xnext && q.y == ynext) {
            writeColour(q.c);
            buffer.erase(q);
            next();
            if (buffer.empty()) return;
            q = *buffer.begin();
        }
    }
  
}

void PPMOutputStream::done() {
    fclose(ofile);
}

void PPMOutputStream::writeColour(const Colour& c) {

    CLR cvalue[3];
    cvalue[0]=c.red();
    cvalue[1]=c.green();
    cvalue[2]=c.blue();

    int cint[3];
    for(int i=0; i<3; i++) {
        cint[i] = (int)(std::min(cvalue[i],(CLR)1)*65535);
    }
    fprintf(ofile,"%d %d %d\n",cint[0],cint[1],cint[2]);
    
}

void PPMOutputStream::next() {
    if (++xnext >= width) {
        xnext = 0;
        ynext++;
    }
}
