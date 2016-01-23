/* *************************************************************************
                          cameraray.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002   
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: cameraray.cpp,v 1.18 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */      

#include "../src/cameraray.h"

#include "../src/rendererror.h"
#include "../src/scene.h"
#include "../src/texture.h"

HitStatistics CameraRay::statcount("Rays","Count","Saved");
HitStatistics CameraRay::statref("Rays","Reflected","Refracted");

CameraRay::CameraRay(const Vector3 o, const Vector3 d, Scene* s, InsideList* l):
  Ray(o,d), myscene(s), inside(l), hit(0), htime(0), ltime(0), hitstatus(NONE),
  tracelevel(myscene->maxtracelevel), weight(1.0/myscene->colourprecision) {

    statcount.eval();
    direction.normalize();

}

CameraRay::~CameraRay() {
    delete(hit);
    delete(inside);  
}

void CameraRay::getIntersectionInfo() {
    ltime = htime;
    htime = hit->currentTime();
    hpoint = positionAtTime(hit->currentTime());
    norm = hit->normal();
    norm.normalize();
    if (Vector3::dotProd(norm,direction)>0) norm.scale(-1);
}

bool CameraRay::shoot() {

    if (tracelevel<=0  || weight < 1) {

        statcount.success();
        return false;
        
    }

    if (hitstatus == NONE || hitstatus == OLD) {
    
        delete(hit);
        hit = myscene->object.SurfacePatch3D::intersect(*this);
        hitstatus = OWN;
        if (hit == 0) return false;
        getIntersectionInfo();
        return true;

    } else { // hitstatus == OWN

        if (hit == 0) return false;
        hit->next();
        if (hit->currentTime() == INTERSECTION_INFINIT_TIME) return false;
        getIntersectionInfo();
        return true;
    
    }

}

bool CameraRay::refract(FLT weightmult) {

    const Object3D* hitobj = dynamic_cast<const Object3D*>(hit->hitObject());
    FLT m = inside->swap(hitobj);

    if (m != 1.0) {

        statref.success();

        htime = 0;

        // calculate new direction
        DBL cosa = -Vector3::dotProd(direction,norm);
        DBL x = m*m*(cosa*cosa-1);

        if (x<-1.0) {
            // total reflection
            inside->add(hitobj);
            return true;
        }
     
        tracelevel--;
        
        if (hitstatus == NONE) throw(RenderError("can't refract new rays!"));
        hitstatus = OLD;
    
        origin = hpoint;
        direction.scale(m);
        DBL s = -(sqrt(x+1)-m*cosa);
        Vector3 nscal = norm.scaled(s);
        direction.add(nscal);
        direction.normalize();
    }

    weight *= weightmult;
    return false;
    
}

void CameraRay::reflect(FLT weightmult) {

    statref.eval();

    htime = 0;

    weight *= weightmult;

    DBL x = -2*Vector3::dotProd(norm,direction);
    Vector3 reflecteddirection = norm.scaled(x);
    reflecteddirection.add(direction);
    origin = hpoint;
    direction = reflecteddirection;
    hitstatus = OLD;
    tracelevel--;
  
}


Colour CameraRay::evaluateMedia(Colour& a) const {

    a = Colour(0);
    Media3D* thismedia = inside->getMedia();
    if (thismedia) {
        Colour ret = thismedia->sample(ltime+2*INTERSECTION_TIME_EPSILON,htime-2*INTERSECTION_TIME_EPSILON,*this,a);
        delete(thismedia);
        return ret;
    } else return Colour(0);

}
