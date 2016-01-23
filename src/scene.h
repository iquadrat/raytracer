/* *************************************************************************
                            scene.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: scene.h,v 1.19 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef SCENE_H
#define SCENE_H

#include<list>

#include "../src/cameraray.h"
#include "../src/colour.h"
#include "../src/csg.h"
#include "../src/lightcache.h"
#include "../src/lightlist.h"
#include "../src/material3d.h"
#include "../src/object3d.h"

class LightSource;

/**Describes objects, materials and light source in space.
  *@author Micha Riser
  */
class Scene {

    friend class CameraPerspective;
    friend class Media3D;    
    friend class Media3DSingle;
    friend class LightRay;
    friend class CameraRay;
    

public: // Constructors and destructor

    Scene(): object(), lights(), referencecount(0), maxtracelevel(100),
      colourprecision(1.0/256),  lightcache(this), bgcolour(ColourA(0,0,0,1)) {
          
        lightcache.setType(1000000);
          
    }

    ~Scene();

public: // Public methods
    /**Add object to scene.
      */
    void addObject(Object3D* o) {object.addObject(o);}

    /**Add light source to scene.
      */
    void addLight(LightSource* l);

    /**Finish the scene creation process.
      */
    void finishParsing() {object.close(); /*object.prepareToRender();*/}

    /**Set objects' default material.
      */
    void setDefaultMaterial(const Material3D& m) {object.setMaterial(m);}

    /**Set background colour.
      */
    void setBGColour(ColourA c) {bgcolour = c;}

    /**Intersect scene with ray originating from camera.
      *@returns resulting colour
      */
    ColourA intersect(CameraRay &shoot);

    /**Get a list of lights shining to a certain position.
      *@param location position
      */
    LightList* getLightList(const Vector3& location) const;

private: // Prviate attributes

    /**"The" object of the scene. Actually a container for all objects.
      */
    CSGUnion object;

    /**List of lights in the scene.
      *Note: lights have to be ordered on the list so that the "lowest"
      *light source comes first (where lower is defined by the "<" operator
      *between two light sources), that is, the list is in ascending order.
      */
    std::list<LightSource*> lights;

    /**Reference count.
      */
    int referencecount;

    /**Maximal recursive tracing level.
      */
    int maxtracelevel;

    /**Desired precision of the colour.
      */
    FLT colourprecision;

    /**Light cache for scattering media.
      */
    mutable LightCache lightcache;
    
    /**Background colour.
      */
    ColourA bgcolour;

private: // Private methods
    ColourA intersectRec(CameraRay& shoot);

};

#endif
