/* *************************************************************************
                          cameraperspective.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002       
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: cameraperspective.cpp,v 1.11 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/cameraperspective.h"

#include "../src/cameraray.h"
#include "../src/scene.h"

CameraPerspective::CameraPerspective(Scene* s): location(0,0,0), direction(0,0,1),
  right(4.0/3,0,0), up(0,1,0), scene(s), insides(0) {
        
    s->referencecount++;

}

CameraPerspective::CameraPerspective(Scene* s, const Vector3& loc, const Vector3& dir,
  const Vector3& r, const Vector3& u): location(loc), direction(dir), right(r),
  up(u), scene(s), insides(0) {

    s->referencecount++;
          
}

CameraPerspective::CameraPerspective(const CameraPerspective& c): Pigment3D(c), location(c.location), direction(c.direction),
  right(c.right), up(c.up), scene(c.scene), insides(0) {

    scene->referencecount++;

}


CameraPerspective::~CameraPerspective() {

        delete(insides);
        if (--scene->referencecount==0) delete(scene); // dangerous, better use reference counting on scene! *done*

}

ColourA CameraPerspective::evaluateAt(const Vector3& p) const {

    if (insides == 0) {
        insides = new InsideList();
        scene->finishParsing();
        scene->object.listInside(location,*insides);
    }
    Vector3 rdir = direction;
    rdir.add(right.scaled(p[0]-.5));
    rdir.add(up.scaled(p[1]-.5));
    CameraRay shoot(location,rdir,scene,new InsideList(*insides));
    return scene->intersect(shoot);
  
}

