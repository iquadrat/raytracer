/* *************************************************************************
                            scene.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: scene.cpp,v 1.12 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/scene.h"

#include "../src/lightsource.h"

Scene::~Scene() {

    std::list<LightSource*>::iterator it = lights.begin();
    for(; it!=lights.end(); ++it) {
        delete(*it);
    }
    
}

void Scene::addLight(LightSource* l) {
    
    // maybe another std-datastructure is better for ordered list *check me*
    lights.push_back(l);
    // this is clearly bad-programming, but as the number of light sources is low
    // and it is done only once per scene this shouldn't hurt right now *fix me*
    lights.sort();
    
}

ColourA Scene::intersect(CameraRay& shoot) {
    finishParsing();
    return intersectRec(shoot);
}

ColourA Scene::intersectRec(CameraRay& shoot) {

    if (shoot.shoot() == true) {

        Colour absorption = Colour(0);
        Colour mediacontribution = shoot.evaluateMedia(absorption);
        absorption.exp();
        shoot.scaleWeight(absorption.minElement());

        Texture* hittexture = shoot.texture();

        Colour tcolour = Colour(0,0,0);
        // do lighting
        std::list<LightSource*>::iterator it = lights.begin();
        for(; it!=lights.end(); ++it) {
            tcolour.add( (*it)->enlight(shoot, hittexture, shoot.getInside() ) );
        }
        tcolour.add(hittexture->enlightShadow(shoot));

        // if (tcolour.norm()*shoot.getWeight()*colourprecision >1) {return retcolour;}

        Colour retcolour = tcolour;
        CLR transmit = hittexture->colour->alpha();
        if (transmit != 0) {
            CameraRay save = CameraRay(shoot);
            bool totalreflection = shoot.refract(transmit);

            if (totalreflection == false) {
                // do reflection
                if (hittexture->finish->reflection>0) {
                    save.reflect(hittexture->finish->reflection);
                    ColourA rcol = intersectRec(save);
                    rcol.scale(hittexture->finish->reflection);
                    retcolour.add(rcol);
                }
                // do refraction        
                ColourA tcol = intersectRec(shoot);
                tcol.scale(transmit);
                retcolour.add(tcol);
            } else {
                // do total reflection
                shoot.reflect(1);
                ColourA rcol = intersectRec(shoot);
                retcolour.add(rcol);
            }
        } else {
            if (hittexture->finish->reflection>0) {
                // do reflection
                shoot.reflect(hittexture->finish->reflection);
                ColourA rcol = intersectRec(shoot);
                rcol.scale(hittexture->finish->reflection);
                retcolour.add(rcol);
            }
        }

        delete(hittexture);

        retcolour.scale(absorption);
        return Colour::add(retcolour,mediacontribution);

    }

    return bgcolour;

}


LightList* Scene::getLightList(const Vector3& location) const {

    InsideList inside;
    object.listInside(location, inside);

    // be careful to build the LightList with descending LightSource order    
    LightList* ret = 0;
    std::list<LightSource*>::const_iterator it = lights.begin();
    for(; it!=lights.end(); ++it) {

        LightRay lray = (*it)->shineTo(location, &inside);
        Colour c = lray.shoot();
        if (c.norm() != 0) {
            ret = new LightList(c, *it, ret);
        }
        
    }
    return ret;

}

