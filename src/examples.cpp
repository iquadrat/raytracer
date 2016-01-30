/* *************************************************************************
                          examples.cpp  -  description
                             -------------------
    begin                : Wed Oct 23 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org
    
    $Id: examples.cpp,v 1.47 2003/08/04 13:20:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/examples.h"

#include "../src/box.h"
#include "../src/cameraperspective.h"
#include "../src/csg.h"
#include "../src/cylinder.h"
#include "../src/function.h"
#include "../src/interior.h"
#include "../src/intervalmap.h"
#include "../src/isosurface.h"
#include "../src/material3d.h"
#include "../src/noise.h"
#include "../src/patchobject.h"
#include "../src/plane.h"
#include "../src/pointlight.h"
#include "../src/random.h"
#include "../src/scene.h"
#include "../src/sphere.h"
#include "../src/statistics.h"
#include "../src/texture3d.h"
#include "../src/trianglepatch.h"

//////////////////////////////////////////////////////////////////////////////
// Isosurface Scene
// Shows isosurface, reflection, refraction, patterned pigment, transparency
//////////////////////////////////////////////////////////////////////////////

class MyFunction: public Isosurface::IsosurfaceFunction {

private:
    mutable HitStatistics evalcount;
    PerlinNoise p;
    VNoise q;

public:

    MyFunction():evalcount("Function","Evaluations",""), q(new PerlinNoise(), new PerlinNoise(), new PerlinNoise()) {}

    DBL evaluateAt(const Vector3& vo) const {

        evalcount.eval();
        Vector3 v(vo);
        v.add(q.sum1FNoise(v.scaled(.2),4,2,2).scaled(.3));
        v.add(q.sum1FNoise(v,2,2,2).scaled(0.05));        
        DBL x = v[0]; DBL y = v[1]; DBL z=v[2];
        return sqrt(x*x+y*y+z*z)-4;//+ (p.eval01(v.scaled(.5))-.5);
        //return v.length()-1+.2*( sin(10*v.a[0])+cos(9*v.a[1])+sin(8*v.a[2]) );
        //return -(v.a[0]+v.a[1]+v.a[2]);
        /*DBL f1 = sqrt(y*y + z*z) - 0.8;
        DBL f2 = fabs(x)+fabs(y)-1;
        DBL f3 = sqrt(y*y + z*z) - .6;
        DBL Blob_threshold = 0.05;
        return (1+Blob_threshold)-pow(Blob_threshold,f1)-pow(Blob_threshold,f2)*pow(Blob_threshold,f3);*/
        DBL k=5, a=0.95, b=0.8;
/*        return pow( (x*x+y*y+z*z-a*k*k),2 )-b*( pow(z-k,2)-2*x*x )*( pow(z+k,2)-2*y*y )
         + (p.eval01(v.scaled(2.5))-.5)*30; */
//        DBL s1 = pow( (x*x+y*y+z*z-a*k*k),2 )-b*( pow(z-k,2)-2*x*x )*( pow(z+k,2)-2*y*y );
//        return  std::max( s1, std::min( sqrt(x*x+y*y+z*z)-3, sqrt( (x-2)*(x-2)+(y+2)*(y+2)+z*z )-2.5 ) );


        //return pow(x,4)-5*x*x+pow(y,4)-5*y*y+pow(z,4)-5*z*z+11.8;
        //return v.length()-1;                                              

    }


};

#include "../src/df3function.h"
#include <fstream>

Pigment3D* setUpScene_Isosurface() {

/*
    Scene* s = new Scene();

    Pigment3DMapped check = Pigment3DMapped(new Checker());
    check.insertCol(0,Colour(1,0,0));
    check.insertCol(1,Colour(0,0,0));

    Pigment3DMapped imap = Pigment3DMapped(new Gradient(Vector3(1,0,0)));
    imap.insertCol(0,Colour(1,1,1));
    imap.insert(1,check.copy());

    Plane *pl = new Plane(Vector3(0,1,0));
//    pl->setPigment(imap);
    pl->setPigment(Pigment3DConstant(Colour(1)));
    pl->setFinish(Finish(0.1,0.6,0.5,0,0));
    pl->translate(Vector3(0,-.5-3,0));
    s->addObject(pl);
    
    std::ifstream df3in("lobster.df3");
    if (!df3in.good()) {std::cerr << "failed to open df3 data file" << std::endl; exit(1);}
    DF3Function *myf = new DF3Function(df3in);
    df3in.close();

    Isosurface* myiso = new Isosurface(myf,new Box(Vector3(0),Vector3(1)),160,false);
    myiso->translate(Vector3(-.5,-.5,-.5));
    myiso->scale(Vector3(1,1,34.0/120));
    myiso->scale(Vector3(5));
    myiso->rotateDeg(Vector3(-90,55,0));    
    
    myiso->translate(Vector3(-.4,-3,4));
    myiso->setPigment(Pigment3DConstant(ColourA(1,.3,0.1,0)));//.65)));
    myiso->setInterior(Interior(1.33,0));
    myiso->setFinish(Finish(0.1,0.6,0,0.5,4));
    s->addObject(myiso);

    PointLight* p = new PointLight(Vector3(0,10,-8),Colour(1),s);
    s->addLight(p);

    Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0*2,10)), new Interior() );
    s->setDefaultMaterial(m);

    s->finishParsing();

    CameraPerspective* c = new CameraPerspective(s,Vector3(0),Vector3(0,-.5,.5),Vector3(1,0,0),Vector3(0,1,0));

    return c;
*/

    Scene* s = new Scene();

    PerlinNoise* ng =  new PerlinNoise();
    Pigment3DMapped check = Pigment3DMapped(new pattern::Granite(ng));//pattern::Checker());
//    check.insertCol(1,Colour(0.3,0.3,1));
    check.insertCol(1,Colour(0.3,1,1));
    check.insertCol(0,Colour(0.5,0,.3));
    check.scale(6);

/*    Plane *pl = new Plane(Vector3(0,1,0));
    pl->setPigment(Pigment3DConstant(ColourA(1,1,1,0)));
    pl->setFinish(Finish(0.05,0.3,0.5,1,1));
    pl->translate(Vector3(0,-2,0));
    s->addObject(pl);*/

/*    pl = new Plane(Vector3(0,0,1));
    pl->setPigment(check);
    pl->translate(Vector3(0,0,15));
    s->addObject(pl);*/

    MyFunction* myf = new MyFunction();
    Isosurface* myiso = new Isosurface(myf,new Sphere(6),10,false);
    myiso->scale(Vector3(.5,.5,.5));
    myiso->rotateDeg(Vector3(0,35,0));
    myiso->translate(Vector3(0,0,5));
    myiso->setPigment(check);//Pigment3DConstant(ColourA(1,.8,.6,0)));
    myiso->setInterior(Interior(1.33,0));            
    myiso->setFinish(Finish(0.4,0.45,0.0,.1,10));
    s->addObject(myiso);

//    PointLight* p = new PointLight(Vector3(0,10,-8),Colour(1),s);
    PointLight* p = new PointLight(Vector3(15,10,-8),Colour(2),s);
    s->addLight(p);

    PointLight* p2 = new PointLight(Vector3(-4,0.1,-1),Colour(0.45),s);
    s->addLight(p2);
    
    Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0*2,10)), new Interior() );
    s->setDefaultMaterial(m);

//    s->object.close();
    s->finishParsing();

    CameraPerspective* c = new CameraPerspective(s);

    return c;
    
};


/////////////////////////////////////////////////////
// Function to create 3d menger sponge
////////////////////////////////////////////////

Object3D* make_menger(double length, int depth) {

    if (depth>0) {

        Object3D* piece = make_menger(length/3,depth-1);

        CSGUnion* u = new CSGUnion();

        Object3D* q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(-length/3,-length/3,-length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(-length/3,-length/3,length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(-length/3,length/3,-length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(-length/3,length/3,length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(length/3,-length/3,-length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(length/3,-length/3,length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(length/3,length/3,-length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(length/3,length/3,length/3));
        u->addObject(q);


        //

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(-length/3,-length/3,0));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(length/3,-length/3,0));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(-length/3,length/3,0));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(length/3,length/3,0));
        u->addObject(q);

        //

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(0,-length/3,-length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(0,length/3,-length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(0,-length/3,length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(0,length/3,length/3));
        u->addObject(q);

        //

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(-length/3,0,-length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(length/3,0,-length/3));
        u->addObject(q);

        q = static_cast<Object3D*>(piece->duplicate());
        q->translate(Vector3(-length/3,0,length/3));
        u->addObject(q);

        q = piece;
        q->translate(Vector3(length/3,0,length/3));
        u->addObject(q);

        u->close();

        return u;

    } else
    {
        //Object3D* o = new Sphere(length/2);
        Object3D* o = new Box(-length/2*1.0001,length/2*1.0001);
        return o;
    }

}


//////////////////////////////////////////////////////////////
// Cut menger sponge, recurse level 6
// Shows: CSG union, intersection, boundingboxes, reflections
/////////  /////////////////////////////////////////////////////

Pigment3D* setUpScene_Menger() {

    Scene* s = new Scene();

    CSGIntersect* csg = new CSGIntersect();

    Object3D* menger = make_menger(3,6);
    csg->addObject(menger);

    Plane *pl = new Plane(Vector3(0,1,0));
    pl->translate(Vector3(0,1.1,0));
    pl->rotateDeg(Vector3(-45,-40,0));
    csg->addObject(pl);

    csg->close();

    csg->setPigment(Pigment3DConstant(ColourA(.7,.7,.7,0)));
    csg->setFinish(Finish(0.03,0.3,0.6,1,5));
    csg->rotateDeg(Vector3(11,23,0));
    csg->translate(Vector3(0,0,6));
    s->addObject(csg);

    PointLight* p = new PointLight(Vector3(0,10,-8),Colour(1),s);
    s->addLight(p);

    PointLight* p2 = new PointLight(Vector3(-3,4,-6),Colour(.5),s);
    s->addLight(p2);

    Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0,0)), new Interior() );
    s->setDefaultMaterial(m);

//    s->object.close();
//    s->object.prepareToRender();
    s->finishParsing();

    CameraPerspective* c = new CameraPerspective(s);

    return c;

};

//////////////////////////////////////////////////////////////
// Media (inversed) menger sponge
// shows: CSG, Scattering Media
//////////////////////////////////////////////////////////////

Pigment3D* setUpScene_MediaMenger() {

    Scene* s = new Scene();

    CSGIntersect* mengerc = new CSGIntersect();
//    CSGUnion* mengerc = new CSGUnion();

    Object3D* menger = make_menger(3, 2);

    Box* b = new Box(Vector3(-1.499),Vector3(1.499));
    mengerc->addObject(b);

    Object3D* minversed = menger->invert();
    delete(menger); menger = 0;
    mengerc->addObject(minversed);

//    mengerc->addObject(menger);

    mengerc->close();

    mengerc->setPigment(Pigment3DConstant(ColourA(1,1,1,1)));
    mengerc->rotateDeg(Vector3(11,23,0));
    mengerc->translate(Vector3(0,0,6));
    Media3DSingle media(Colour(0),Colour(2.5),Colour(2.5),Media3D::MIE_MURKY,
      s,0.1,1.0/100,0.5,Media3D::LINEAR);
    mengerc->setMedia(media);

    s->addObject(mengerc);

    Plane *pl = new Plane(Vector3(0,1,0));
    pl->setPigment(Pigment3DConstant(ColourA(0,0,.5,0)));
    pl->translate(Vector3(0,-2.5,0));
    s->addObject(pl);

    Plane *pl2 = new Plane(Vector3(0,0,1));
    pl2->setPigment(Pigment3DConstant(ColourA(0,0,.5,0)));
    pl2->translate(Vector3(0,0,11.5));
    s->addObject(pl2);

    PointLight* p = new PointLight(Vector3(0,10,-8),Colour(1*1.5),s);
    s->addLight(p);

    PointLight* p2 = new PointLight(Vector3(-3,4,-6),Colour(.5*1.5),s);
    s->addLight(p2);

    Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0*2,10)), new Interior() );
    s->setDefaultMaterial(m);

//    s->object.close();
    s->finishParsing();

    CameraPerspective* c = new CameraPerspective(s);

    return c;

};


////////////////////////////////////////////////////////////////////////
// Media room
// Shows: CSG, scattering media
////////////////////////////////////////////////////////////////////////

Object3D* makeBox(Vector3 v1, Vector3 v2) {
    Box* b = new Box(v1,v2);
    b->setPigment(Pigment3DConstant(ColourA(.6,.6,.6,0)));
    return b;
}

Pigment3D* setUpScene_MediaRoom() {

  Scene* s = new Scene();

  CSGIntersect* diff = new CSGIntersect();

  Object3D* b;
  b = new Box(Vector3(-10,-2,-10),Vector3(10,2,10));
  diff->addObject(b);
  Object3D* btmp = new Box(Vector3(-9.9,-1.9,-9.9),Vector3(9.9,2.1,9.9));
  b = btmp->invert(); delete(btmp);
  diff->addObject(b);
  diff->scale(Vector3(.4,.9,.9));
  diff->setPigment(Pigment3DConstant(ColourA(.6,.6,.6,0)));
  diff->setFinish(Finish(0.1,0.6,0,0,0));

  diff->close();
  s->addObject(diff);

  s->addObject(makeBox(Vector3(-10,2,5),Vector3(10,2.1,7)));
  s->addObject(makeBox(Vector3(-10,2,8),Vector3(10,2.1,10)));
  s->addObject(makeBox(Vector3(-10,2,1),Vector3(10,2.1,4)));
  s->addObject(makeBox(Vector3(-10,2,-10),Vector3(10,2.1,-5)));
  s->addObject(makeBox(Vector3(-10,2,-4),Vector3(10,2.1,1)));
  s->addObject(makeBox(Vector3(-10,2,-10),Vector3(-8,2.1,10)));
  s->addObject(makeBox(Vector3(-6,2,-10),Vector3(-4,2.1,10)));
  s->addObject(makeBox(Vector3(-2,2,-11),Vector3(0,2.1,10)));

  s->addObject(makeBox(Vector3(2,2,-10),Vector3(4,2.1,10)));
  s->addObject(makeBox(Vector3(6,2,-10),Vector3(8,2.1,10)));

  Sphere* sp = new Sphere(.6,Vector3(1,1,7.5));
  sp->setPigment(Pigment3DConstant(Colour(.3,0,0)));
  sp->setFinish(Finish(0.1,0.6,0.5,0,0));
  s->addObject(sp);

  b = new Box(Vector3(-12,-10,-10),Vector3(12,6,10));
  b->setPigment(Pigment3DConstant(ColourA(.5,.5,.5,1)));
  b->setFinish(Finish(0.1,1,0,0,0));
  Media3DSingle media(Colour(0),Colour(0.2),Colour(0.2),Media3D::MIE_MURKY,
    s,0.05,1.0/1000,0.5,Media3D::RECURSIVE);
  b->setMedia(media);
  s->addObject(b);

  PointLight* l = new PointLight(Vector3(10,20,0),Colour(10),s);
  s->addLight(l);
  l = new PointLight(Vector3(-5,20,10),Colour(10),s);
  s->addLight(l);

  Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0*2,10)), new Interior() );
  s->setDefaultMaterial(m);

//  s->object.close();
    s->finishParsing();

  CameraPerspective* c = new CameraPerspective(s);

  return c;

}

///////////////////////////////////////////////////////////////
// Recursive camera scene.
// Shows: pigments, camera-is-a-pigment principle
///////////////////////////////////////////////////////////////

Pigment3D* setUpScene_RecCamera() {

  Scene* s = new Scene();

  CameraPerspective* c = new CameraPerspective(s);

  Object3D* sp = new Box(Vector3(0),Vector3(1,1,0.01));
  sp->scale(Vector3(2));
  sp->rotateDeg(Vector3(-1,-35,0));
  sp->translate(Vector3(-2,0,4));
  sp->setPigment(*c);
  sp->setFinish(Finish(1,0,0,0,0));
//  s->addObject(sp);

  PerlinNoise* ng = new PerlinNoise();
  Pigment3DMapped check = Pigment3DMapped(new pattern::Dents(ng));//pattern::Checker());
  check.insertCol(0,Colour(1,0,0));
  check.insertCol(1,Colour(0,0,1));

  sp = new Box(Vector3(-.1,-.1,.01),Vector3(1.1,1.1,0.2));
  sp->scale(Vector3(2));
  sp->setPigment(Pigment3DConstant(ColourA(0,1,0,0)));
  sp->rotateDeg(Vector3(-1,-35,0));
  sp->translate(Vector3(-2,0,4));
//  s->addObject(sp);

  sp = new Plane(Vector3(0,1,0));
  sp->translate(Vector3(0,-2,0));
  sp->setPigment(check);
  s->addObject(sp);

  sp = new Plane(Vector3(0,0,1));
  sp->translate(Vector3(0,0,15));
  sp->setPigment(Pigment3DConstant(Colour(1)));
  s->addObject(sp);


/*  Object3D* cyl = new Cylinder();
  cyl->rotateDeg(Vector3(60,0,0));
//  cyl->rotate(Vector3(10,0,0));
  cyl->translate(Vector3(0,-.5,0));
//  cyl->scale(Vector3(1,10,1));
  cyl->translate(Vector3(0,0,5));
  cyl->setPigment(Pigment3DConstant(ColourA(1,1,1,0,0.5)));
  cyl->setInterior(Interior(1.5,0));
  s->addObject(cyl);
*/

  CSGIntersect* csg = new CSGIntersect();
//  CSGUnion* csg = new CSGUnion();

  Box* box = new Box(Vector3(-3,-1.8,-1.8),Vector3(3,1.8,1.8));
  box->setPigment(Pigment3DConstant(ColourA(1,0,0,0)));
  csg->addObject(box);

  sp = new Sphere(2.5,Vector3(0,0,0));
  sp->setPigment(Pigment3DConstant(ColourA(0,1,1,0)));
//  sp->translate(Vector3(3,0,0));
  sp = sp->invert();
  csg->addObject(sp);

  csg->close();

  csg->rotateDeg(Vector3(0,15,0));
  csg->translate(Vector3(0,0,8));

  s->addObject(csg);

  Sphere* sphere = new Sphere(1);
//  Object3D* sphere = new Box(Vector3(-1),Vector3(1));
  sphere->translate(Vector3(0,0,5));
  sphere->scale(Vector3(3.2));
  sphere->setPigment(Pigment3DConstant(ColourA(0,1,1,0)));
//  s->addObject(sphere);



  PatchObject* mesh = new PatchObject();
  SurfacePatch3D* pa = new TrianglePatch(new Vector3(-3,0,4),new Vector3(2,2,4.5),new Vector3(1,3,7));
  mesh->addPatch(pa);
  mesh->close();
  s->addObject(mesh);
  mesh->setPigment(Pigment3DConstant(ColourA(0,1,1,0)));


  Vector3 cap(0,-.3,3);
  Vector3 base(0,.4,3);
//  Cylinder* cyl = new Cylinder( Vector3(3,1,3), Vector3(-2,3,5), 0.6);
  Cylinder* cyl = new Cylinder(base, cap, .1);
  cyl->setPigment(Pigment3DConstant(ColourA(0,1,0,0)));
  s->addObject(cyl);

  Sphere* sbase = new Sphere(0.04,base);
  Sphere* scap = new Sphere(0.04,cap);
  
  sbase->setPigment(Pigment3DConstant(ColourA(0,0,1,0)));
  sbase->setFinish(Finish(Colour(0.5),0.5,0,1,1));
  scap->setPigment(Pigment3DConstant(ColourA(0,0,1,0)));
  scap->setFinish(Finish(Colour(0.5),0.5,0,1,1));
  s->addObject(scap);
  s->addObject(sbase);

  Sphere* small = new Sphere(1,Vector3(0,0,3));
  small->scale(Vector3(1,1.01,1.01));
  small->setPigment(Pigment3DConstant(Colour(1)));
  small->setFinish(Finish(Colour(0.1),0.5,0.6,0.1,0.1));
//  s->addObject(small);
  
  PointLight* l = new PointLight(Vector3(10,20,-100),Colour(1),s);
  s->addLight(l);

  Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0,0)), new Interior() );
  s->setDefaultMaterial(m);

//  s->object.close();
    s->finishParsing();

  return c;

}


///////////////////////////////////////////////////////////////
// Scene blending
/////////////////////////////////////////////////////////////

Pigment3D* setUpScene_BlendCamera() {

    Pigment3D* s1 = setUpScene_Isosurface();
    Pigment3D* s2 = setUpScene_Menger();

    Pigment3DMapped* imap = new Pigment3DMapped(new pattern::Gradient(Vector3(1,0,0)));
    imap->insert(0,s1);
    imap->insert(1,s2);

    return imap;

}


////////////////////////////////////////////////////////////////
// Sphere spiral scene
/////////////////////////////////////////////////////////////////

Pigment3D* setUpScene_SphereSpiral() {

    Scene* s = new Scene();

    CameraPerspective* c = new CameraPerspective(s);

    CSGUnion* spiral = new CSGUnion();

    for(DBL deg=0; deg<20000; deg += .02123*10) {
        Sphere* sp = new Sphere(0.002*2);
//        sp->scale(Vector3(1,1,10));
        sp->translate(Vector3(1-deg/20000, 0, (deg/900-deg*deg/7000/7000)/6 ));
        sp->rotateDeg(Vector3(0,0,deg));
        spiral->addObject(sp);
    }

    spiral->rotateDeg(Vector3(-90,0,0));
    spiral->setPigment(Pigment3DConstant(ColourA(1,0.3,0.2,0)));
    spiral->translate(Vector3(0,-2,5));
    spiral->close();
    s->addObject(spiral);

    Object3D* clone = spiral->duplicate();
    clone->setPigment(Pigment3DConstant(ColourA(0.1,1,0.2,0)));
    clone->translate(Vector3(2,0,0));
    s->addObject(clone);

    clone = spiral->duplicate();
    clone->setPigment(Pigment3DConstant(ColourA(0.1,0.3,1,0)));
    clone->translate(Vector3(-2,0,0));
    s->addObject(clone);

    clone = spiral->duplicate();
    clone->setPigment(Pigment3DConstant(ColourA(1,0.3,1,0)));
    clone->translate(Vector3(1,0,3));
    s->addObject(clone);

    clone = spiral->duplicate();
    clone->setPigment(Pigment3DConstant(ColourA(0.2,1.3,1,0)));
    clone->translate(Vector3(-1,0,3));
    s->addObject(clone);

    clone = spiral->duplicate();
    clone->scale(10);
    clone->setPigment(Pigment3DConstant(ColourA(1,1,1,0)));
    clone->translate(Vector3(0,3,-30));
    s->addObject(clone);


    PointLight* l = new PointLight(Vector3(0,0,0),Colour(1),s);
    s->addLight(l);

    Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0,0)), new Interior() );
    s->setDefaultMaterial(m);

    std::cout << "Preparing for rendering..\n";
//    s->object.close();
    s->finishParsing();

    return c;
}


//////////////////////////////////////////////////////////////
// Random Sphere scene
///////////////////////////////////////////////////////////////
/*
Pigment3D* setUpScene_RandomSpheres() {


    Scene* s = new Scene();

    CameraPerspective* c = new CameraPerspective(s);

    CSG* spiral = new CSG(CSG::UNION);

    for(int n=0; n<25000; ++n) {
        double r =exp(-pow(Random::getDouble(),2))*0.025;
        Sphere* sp = new Sphere(r,Vector3(Random::getDouble()*(1-4*r)+2*r,Random::getDouble()*(1-4*r)+2*r,Random::getDouble()*(1-2*r)+r));
//        Sphere* sp = new Sphere(r,Vector3(Random::getDouble(),Random::getDouble(),Random::getDouble() ));
        spiral->addObject(sp);
    }
    spiral->translate(Vector3(-.5,-.5,2));
    spiral->setPigment(Pigment3DConstant(ColourA(1,0.3,0.2,0,0)));
    spiral->setInterior(Interior(1.33,0));
    s->addObject(spiral);

    PointLight* l = new PointLight(Vector3(0,0,0),Colour(1),s);
    s->addLight(l);

    Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0,0)), new Interior() );
    s->setDefaultMaterial(m);

    cout << "Preparing for rendering..\n";
    s->object.prepareToRender();
    return c;
} */

Pigment3D* setUpScene_RandomCylinders() {

    Scene* s = new Scene();

    CameraPerspective* c = new CameraPerspective(s);

    CSGMerge* spiral = new CSGMerge();

    for(int n=0; n<100000; ++n) {
        double l = Random::global.getDouble();
        Vector3 v(Random::global.getDouble(),Random::global.getDouble(),Random::global.getDouble());
        v.scale(l);
        Object3D* part = new Cylinder();
        part->scale(Vector3(.007,0.007,.02));
        part->rotateDeg(v.scaled(360));
        part->translate(v);
        spiral->addObject(part);
    }
    spiral->close();
    spiral->translate(Vector3(-.5,-.5,2));
    spiral->setPigment(Pigment3DConstant(ColourA(1,0.3,0.2,0)));
    spiral->setInterior(Interior(1.33,0));
    s->addObject(spiral);

    PointLight* l = new PointLight(Vector3(0,0,0),Colour(1),s);
    s->addLight(l);

    Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0,0)), new Interior() );
    s->setDefaultMaterial(m);

    std::cout << "Preparing for rendering..\n";
//    s->object.close();
    s->finishParsing();

    return c;
}

Pigment3D* setUpScene_RandomSpheres() {

    Scene* s = new Scene();

    CameraPerspective* c = new CameraPerspective(s);

/*
//    SharedPointsPatchObject* spiral = new SharedPointsPatchObject();
    SharedPointsPatchObject::PatchList list;
    Vector3 p1,p2,p3;

    p1 = Vector3(Random::getDouble(),Random::getDouble(),Random::getDouble());
    p2 = p1;
    p2.add(Vector3(Random::getDouble(),Random::getDouble(),Random::getDouble()).scaled(0.1));

    for(int n=0; n<25000; ++n) {
        p3 = p1;
        p3.add(Vector3(Random::getDouble()-.5,Random::getDouble()-.5,Random::getDouble()-.5).scaled(0.1));
//        p3.sub(Vector3(0.01,0.01,0.01));
//        SurfacePatch3D* sp = new TrianglePatch(p1,p2,p3);
//        spiral->addPatch(sp);
//        spiral->addTriangle(p1,p2,p3);
        list.addTriangle(p1,p2,p3);
        p1 = p2; p2 = p3;
    }
    Object3D* spiral = new SharedPointsPatchObject(list);
    spiral->translate(Vector3(-.5,-.5,2));
    spiral->setPigment(Pigment3DConstant(ColourA(1,0.3,0.2,0,0)));
    spiral->setInterior(Interior(1.33,0));
    s->addObject(spiral);
*/

    CSGIntersect* is = new CSGIntersect();
//    CSGUnion* is = new CSGUnion();

    Sphere* testo = (Sphere*)(new Sphere(1));
//    testo = (Sphere*)(testo->invert());


  PatchObject* mesh = testo->toPatch(0.001);
//  mesh = (PatchObject*)mesh->invert();
//    Object3D* mesh = new Sphere(1);
//    Object3D* mesh = testo;
    mesh->setPigment(Pigment3DConstant(ColourA(0,1.3,0.2,0)));
//    mesh = (PatchObject*)mesh->invert();
//    mesh->rotateDeg(Vector3(70,0,0));

//    mesh->translate(Vector3(0.7,0,0));
//    mesh->scale(Vector3(1,1,3));
    mesh->scale(Vector3(1,1,10));

//    s->addObject(mesh);

//    Object3D* s2 = (Object3D*)mesh->duplicate();
//    Object3D* s2 = testo->toPatch(0.001);
    Object3D* s2 = new Sphere(1);
    s2->translate(Vector3(0.7,0,0));

    s2->setPigment(Pigment3DConstant(ColourA(1,0.3,0.2,0)));
    s2->scale(Vector3(2,1.5,2.8));
    s2->translate(Vector3(-.7,0,0));
//    is->addObject(testo);

//    mesh = (PatchObject*)mesh->invert();
    is->addObject(mesh);
//    testo = (Sphere*)testo->invert();
    is->addObject(s2);
    is->close();

    is->rotateDeg(Vector3(0,25,0));
    is->translate(Vector3(-0.2,0,5));
    s->addObject(is);

    PointLight* l = new PointLight(Vector3(2,0,0),Colour(1),s);
    s->addLight(l);

    Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0,0)), new Interior() );
    s->setDefaultMaterial(m);

    std::cout << "Preparing for rendering..\n";
//    s->object.close();
    s->finishParsing();

    return c;

}

Pigment3D* setUpScene_Balls(bool reflective_box) {

    Scene* s = new Scene();
    CameraPerspective* c = new CameraPerspective(s,Vector3(0,2,-3),Vector3(0,-2,3),Vector3(-4*2,0,0),Vector3(0,6*3/sqrt(52)*2,4*3/sqrt(52)*2));

    Box* b;
    if (reflective_box) {
        b = new Box(Vector3(-5),Vector3(5));
        b->rotateDeg(Vector3(50,-72,156));
        b->setPigment(Pigment3DConstant(Colour(1)));
        b->setFinish(Finish(0,0,.99,0,0));
    } else {
        b = new Box(Vector3(-50000),Vector3(50000));
        b->setPigment(Pigment3DConstant(ColourA(1)));
    }
    Media3DSingle media(Colour(0),Colour(.3*.25),Colour(.3),Media3D::RAYLEIGH,
      s,0.1,1.0/500,0.5,Media3D::RECURSIVE);
    b->setMedia(media);    
    
    s->addObject(b);

    PointLight* l = new PointLight(Vector3(0),Colour(4),s,true);
    s->addLight(l);

    DBL rad = .2;

    CSGIntersect* u = new CSGIntersect();
//    CSGUnion* u = new CSGUnion();

    Sphere *sp, *sp2;
    sp = new Sphere(1.05);
    u->addObject(sp);
    sp = new Sphere(0.95);
    sp = (Sphere*)sp->invert();    
    u->addObject(sp);

    sp = new Sphere(rad,Vector3(0,0,-1));
    sp = (Sphere*)sp->invert();
    
    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   52.6625,    0, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   52.6625, -72, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   52.6625, -144, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   52.6625, -216, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   52.6625, -288, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   10.8125, 0, 0 ));
    u->addObject(sp2);
        
    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   10.8125, -72, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   10.8125, -144, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   10.8125, -216, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   10.8125, -288, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -52.6626, -36, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -52.6626, -108, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -52.6626, -180, 0 ));
    u->addObject(sp2);
    
    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -52.6626, -252, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -52.6626, -324, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -10.8125, -36, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -10.8125, -108, 0 ));
    u->addObject(sp2);
                
    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -10.8125, -180, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -10.8125, -252, 0 ));
    u->addObject(sp2);
    
    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -10.8125, -324, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -26.56505117708, 0, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -26.56505117708, -72, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -26.56505117708, -144, 0 ));
    u->addObject(sp2);
    
    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -26.56505117708, -216, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   -26.56505117708, -288, 0 ));
    u->addObject(sp2);
    
    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   26.56505117708, -36, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   26.56505117708, -108, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   26.56505117708, -180, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   26.56505117708, -252, 0 ));
    u->addObject(sp2);

    sp2 = (Sphere*)sp->duplicate();
    sp2->rotateDeg(Vector3(   26.56505117708, -324, 0 ));
    u->addObject(sp2);

    sp = new Sphere(rad,Vector3(0,1,0));
    sp = (Sphere*)sp->invert();
    u->addObject(sp);

    sp = new Sphere(rad,Vector3(0,-1,0));
    sp = (Sphere*)sp->invert();
    u->addObject(sp);    
            
    u->setPigment(Pigment3DConstant(Colour(1,0.7,0.3)));
    u->setFinish(Finish( .3, .5, .4, 0, 0 ));
    u->rotateDeg(Vector3(232,142,54));
    u->rotateDeg(Vector3(88,0,0));
    u->close();
                        
    s->addObject(u);

    Material3D m = Material3D(new Texture3D(new Pigment3DConstant(ColourA(0)), new Finish(0.1,0.6,0,0,0)), new Interior() );
    s->setDefaultMaterial(m);

    return c;


}






Pigment3D* setUpScene_PigmentTest() {

    Scene* s = new Scene();

    PerlinNoise* ng = new PerlinNoise(23423432);
    Pigment3DMapped ptest = Pigment3DMapped(new pattern::Granite(ng));
    ptest.insertCol(0,Colour(1,1,1));
    ptest.insertCol(1,Colour(0,0,0));

    Plane *pl = new Plane(Vector3(0,0,1));
    pl->setPigment(ptest);
    pl->setFinish(Finish(1,0,0,0,0));
    pl->translate(Vector3(0,0,2));
    s->addObject(pl);
    s->finishParsing();

    CameraPerspective* c = new CameraPerspective(s);

    return c;

};
