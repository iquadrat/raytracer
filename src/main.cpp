/* *************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>

#include "../src/statistics.h"
#include "../src/render.h"
#include "../src/ppmoutputstream.h"
#include "../src/scene.h"
#include "../src/objectconstructionerror.h"
#include "../src/examples.h"

#include "../src/cameraperspective.h" //tmp

int width;
int height;

int parseArguments(int argc, char *argv[]) {

    // default values
    width = 800;
    height = 600;
    int scenenumber = 8;
    
    for(int i=0; i<argc; i++) {

        if (strcmp(argv[i],"-n")==0) {
            
            i++;
            scenenumber = atoi(argv[i]);

        } else if (strcmp(argv[i],"-w")==0) {

            i++;
            width = atoi(argv[i]);

        } else if (strcmp(argv[i],"-h")==0) {

            i++;
            height = atoi(argv[i]);

        }
        
        
    }

    return scenenumber;

}
#include "../src/sphere.h"
Scene* globalscene;

int main(int argc, char *argv[]) {

    globalscene = new Scene();

    int scenenumber = parseArguments(argc,argv);

    Statistics::init();
    
//    for(int n=0; n<100; n++) {
    
    Pigment3D* scene;

    try{
        switch(scenenumber) {
            default:
            case 0:
                scene = setUpScene_Isosurface();
                break;
            case 1:
                scene = setUpScene_Menger();
                break;
            case 2:
                scene = setUpScene_MediaMenger();
                break;
            case 3:
                scene = setUpScene_MediaRoom();
                break;
            case 4:
                scene = setUpScene_RecCamera();
                break;
            case 5:
                scene = setUpScene_BlendCamera();
                break;
            case 6:
                scene = setUpScene_SphereSpiral();
                break;
            case 7:
                scene = setUpScene_RandomSpheres();
                break;
            case 8:
                scene = setUpScene_RandomCylinders();
                break;
            case 9:
                scene = setUpScene_Balls(false);
                break;
            case 10:
                scene = setUpScene_Balls(true);
                break;
            case 11:
                scene = setUpScene_PigmentTest();
                break;
        }
    }
    catch(ObjectConstructionError e) {
        std::cout <<"setUpScene failed: "<<e.message<<std::endl;
        exit(1);
    }
    PPMOutputStream pout("out.ppm",width,height);

    Render r( scene, &pout, width, height );
    r.doRendering();
    pout.done();
    Statistics::global->print();

/*    globalscene->setDefaultMaterial(Material3D(new Texture3D(new Pigment3DConstant(ColourA(0.5)),new Finish(Colour(1),0,0,0,0)),0));        
    CameraPerspective* scene2 = new CameraPerspective(globalscene,
      Vector3(10,10,-10),Vector3(-1,-1,1),Vector3(1,0,1),Vector3(1,1,0) );
    PPMOutputStream msampout("msampout.ppm",800,600);
    Render r2(scene2, &msampout, 800, 600);
    r2.doRendering();
    msampout.done();
    Statistics::global->print();*/


        
    delete(scene);

//    }
    Statistics::finish();

}
