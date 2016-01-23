/* *************************************************************************
                          render.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: render.cpp,v 1.10 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/render.h"

#include <iostream>
#include <stdio.h>

#include "../src/lightcache.h"
#include "../src/media3d.h"
#include "../src/pixeloutputstream.h"

void Render::doRendering() {

    FILE* statfile = fopen("mediastat.ppm", "w");
    fprintf(statfile,"P3\n%d %d\n1000\n",w,h);
    int oldsamp = 0;
        
    std::cout<<"Rendering..\n";
    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {

//            std::cout << "# ("<<x<<","<<y<<")\n";

            Vector3 pos( ((DBL)x+.5)/w, 1-((DBL)y+0.5)/h, 0);
            ColourA res = imageplane->evaluateAt(pos);
            Pixel p(x,y,res);
            out->putPixel(p);

            int samples = LightCache::sampleCounts();
            int ds = samples -oldsamp;
            oldsamp = samples;
            fprintf(statfile,"%d %d %d\n",ds,ds,ds);
//            std::cout << "<"<<x<<","<<y<<">\n";

        }
        std::cout<<"*"<<std::flush;

    }

    fclose(statfile);

}
