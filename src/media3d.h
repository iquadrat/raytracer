/* *************************************************************************
                          media3d.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: media3d.h,v 1.20 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef MEDIA3D_H
#define MEDIA3D_H

#include<vector>

#include "../src/cameraray.h"
#include "../src/colour.h"
#include "../src/lightsource.h"
#include "../src/pigment3d.h"
#include "../src/ray.h"
#include "../src/statistics.h"
#include "../src/transparenttransformatable.h"
#include "../src/vector.h"

/**Volumetric media.
  *@author Micha Riser
  */
class Media3D: public TransparentTransformatable {

public: // Static members
    static HitStatistics evaluations;

public: // Public type definitions
    enum scattertype {
        NONE,
        ISOTROPIC,
        RAYLEIGH,
        MIE_HAZY,
        MIE_MURKY,
        HENYEY_GREENSTEIN 
    };

    enum samplingmethod {
        LINEAR,
        RECURSIVE
    };
    
public: // Constructors and destructor
    Media3D(DBL jitteramouth, CLR tol, FLT variancekeepfactor, samplingmethod m):
      jitter(jitteramouth), coltolerance(tol), varkeep(variancekeepfactor),
      method(m) {}

    virtual ~Media3D() {}

private:
    Media3D& operator=(const Media3D&);

public: // Public methods
    /**Copy media.
      */
    virtual Media3D* copy() const = 0;

    /**Integrate media contribution over ray interval.
      */
    Colour sample(DBL start, DBL end, const CameraRay& r, Colour& absorb);
    
    /**Integrate absorbtion value over ray interval.
      */
    void sample(DBL start, DBL end, const LightRay& r, Colour& absorb);

public: // Pure virtual methods
    /**Get emitting colour at location.
      */
    virtual Colour evaluateEmit(const Vector3& p, const CameraRay& r, CLR allowederror, FLT& ilength) const = 0;

    /**Get absorbing colour at location.
      */
    virtual Colour evaluateAbsorb(const Vector3& p) const = 0;

protected: // Protected attributes    
    DBL jitter;
    CLR coltolerance;
    FLT varkeep;    
    samplingmethod method;    

private: // Private Methods
    Colour sampleRecursive(
      const CameraRay& ray, DBL l, DBL m, DBL r,
      const Colour&, const Colour&, const Colour&,
      Colour&, const DBL il);

    void sampleRecursive(const LightRay& ray, DBL l, DBL m, DBL r,
      const Colour&, const Colour&, const Colour&, Colour& end_absorb);

    Colour simps(const Colour& c1, const Colour& c2, const Colour& c3);
    Colour simpsExp(const Colour& c1, const Colour& c2, const Colour& c3,
                    const Colour& a1, const Colour& a2, const Colour& a3,
                    DBL h);

};

/**Single media.
  *@author Micha Riser
  */
class Media3DSingle : public Media3D  {

public: // Constructors and destructor
    Media3DSingle(const Colour& emitting, const Colour& absorbing,
      const Colour& scattering, scattertype t, const Scene* s,
      DBL jitter, CLR tol, FLT variancekeep, samplingmethod m);

    ~Media3DSingle();

private:
    Media3DSingle(const Media3DSingle&);

public: // Public methods (inherited from Media3D)

    void setDensity(Pigment3D* map) {delete(density); density = map;}

    Media3D* copy() const {return new Media3DSingle(*this);}
    Colour evaluateEmit(const Vector3& p, const CameraRay& r, CLR maxerror, FLT& ilength) const;
    Colour evaluateAbsorb(const Vector3& p) const;

private: // Private attributes
    Colour emit, absorb, scatter;
    Pigment3D* density;
    const Scene* scene;
    scattertype stype;    
};


/**Multiple media.
  *@author Micha Riser
  */
class Media3DMultiple: public Media3D {

public: // Constructor and destructor
    Media3DMultiple(DBL jitteramouth, CLR tol, FLT variancekeep, samplingmethod m);

    Media3DMultiple(const Media3D* m);

    ~Media3DMultiple();

private:
    Media3DMultiple(const Media3DMultiple& m);

public: // Public methods

    void addMedia(const Media3D* m) {
        list.push_back(m);
    }

    // Inherited from Media3D
    Media3D* copy() const {return new Media3DMultiple(*this);}
    Colour evaluateEmit(const Vector3& p, const CameraRay& r, CLR maxerror, FLT& ilength) const;
    Colour evaluateAbsorb(const Vector3& p) const;
    

private: // Private attributes
    std::vector<const Media3D*> list;

};

#endif
