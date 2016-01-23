/* *************************************************************************
                          lightcache.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: lightcache.cpp,v 1.9 2003/07/15 00:50:27 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/lightcache.h"

#include "../src/prime.h"
#include "../src/scene.h"

// static member initialisation:
HitStatistics LightCache::Hash::hitstat("LightCache","Evaluations","Hits");
HitStatistics LightCache::Hash::insertstat("LightCache","Inserts","Collisions");
HitStatistics LightCache::VarianceHash::hitstat("LightVarianceCache","Evaluations","Hits");

// LightCache main class

LightCache::LightCache(const Scene* s): scene(s), map(), varmap() {

}

void LightCache::setType(int size) {

    map.setSize(size);
    varmap.setSize( std::max(1000,size/23) );

}

LightList* LightCache::lookup(const Vector3& location, CLR allowederror, FLT& length, FLT variance_keep_factor) {

#define USE_LIGHT_CACHE

#ifdef USE_LIGHT_CACHE    

    // initialize "variance"
    CLR variance =  allowederror / length / variance_keep_factor;

    // round length to a power of 2
    int pp;
    frexp(length, &pp);
    FLT grids = ldexp(1.0,pp);

    unsigned int grids_int = -pp;

    do {

        // round location to grid
        Vector3f locmin, diff;
        for(int i=0; i<3; i++) {
            diff[i] = fmod((FLT)location[i],grids);
            locmin[i] = (FLT)location[i] - diff[i];
            if (location[i]<0) {locmin[i]-=grids; diff[i]=grids+diff[i];}
        }

        CLR newvariance;
        // do lookup in variance cache:
        if (varmap.get(locmin,grids_int,newvariance) == true) {

            // cache hit
            // update variance:
            variance = variance_keep_factor*variance + (1-variance_keep_factor)*newvariance;

            if (variance*length < allowederror) {

                // precise enough, calculate trilinear interpolation
                CLR grids3 = 1.0/pow3(grids);
                LightList* sum = 0;
                for (int i=0; i<8; i++) { // loop over all edge points

                    Vector3f test = locmin;
                    CLR w = grids3;
                    for(int j=0; j<3; j++) if (i&(1<<j)) { // calculate w
                        test[j]+=grids;
                        w*=diff[j];
                    } else {
                        w*=(grids-diff[j]);
                    }

                    LightList* l = sample(test);
                    sum = LightList::merge(sum,l,w);

                }

                return sum;

            }

        } else {

            // cache miss

            // calculate grid midpoint
            Vector3f midpoint = locmin;
            for(int i=0; i<3; i++) midpoint[i] += grids/2;

            CLR grids3 = 1.0/(grids*grids*grids);
            LightList* merge = 0;
            LightList* sum = 0;
            for (int i=0; i<8; i++) { // loop over all edge points

                Vector3f test = locmin;
                CLR w = grids3;
                for(int j=0; j<3; j++) if (i&(1<<j)) {
                    test[j]+=grids;
                    w*=diff[j];
                } else {
                    w*=(grids-diff[j]);
                }

                LightList* l = sample(test);
                sum = LightList::merge(sum,l,w);
                merge = LightList::merge(merge,l,1.0/8);

            }

            LightList* mid_l = sample(midpoint);
            newvariance = LightList::calculateLightListDifference(*mid_l,*merge);
            variance = variance_keep_factor*variance + (1-variance_keep_factor)*newvariance;
            delete(merge);

            varmap.put(locmin,grids_int,newvariance);

            if (variance*length < allowederror) {

                // precise enough, we can return
                return sum;

            }

            delete(sum);

        }
        grids /= 2;
        length /= 2;
        grids_int++;

    } while( true ); // kinda ugly

#else
    LightList* ret = 0;
    return LightList::merge(ret,scene->getLightList(location),1.0);
    
#endif

}

LightList* LightCache::sample(const Vector3f& location) {

    LightList* l;
    if (map.get(location, l) == false) {
        l = scene->getLightList(location.toVector3());
        map.put(location, l);
    }
    return l;

}

// inner classes

LightCache::Hash::Hash() {
    size = 0;
    data = 0;
}

LightCache::Hash::~Hash() {
    for(int i=0; i<size; i++) delete(data[i].list);
    delete[] data;
}

void LightCache::Hash::setSize(int s) {

    for(int i=0; i<size; i++) delete(data[i].list);
    delete[] data;

    size = Prime::nextPrimeBiggerThan(s);
    sondmod = Prime::nextPrimeSmallerThan(int(0.98*s));    
    data = new MapEntry[size];

}


bool LightCache::Hash::get(const Vector3f& location, LightList* &ret) {

    unsigned int loc_i = location.hashValue();
    unsigned int i = calcHashValue(loc_i);
    unsigned int sond = calcHashValueSond(loc_i);

    for(int j=0; j<sondsize; j++) {
        if (data[i].hitcount != 0 && data[i].location.equals(location)) {
            data[i].hitcount += hitbonus;
            ret = data[i].list;
            hitstat.evalSuccess();
            return true;
        }
        i = (i+sond)%size;
    }

    hitstat.evalFailed();
    return false;

}



void LightCache::Hash::put(const Vector3f& loc, LightList* list) {

    insertstat.eval();
    unsigned int lp_i = loc.hashValue();
    unsigned int i = calcHashValue(lp_i);
    unsigned int sond = calcHashValueSond(lp_i);

    unsigned int worst = i;
    unsigned int worst_count = data[i].hitcount;
    data[i].hitcount = data[i].hitcount/2+1;
    
    for(int j=0; j<sondsize-1; j++) {

        i = (i+sond)%size;        
        if (data[i].hitcount<worst_count) {
            worst = i;
            worst_count = data[i].hitcount;
        }
        data[i].hitcount = data[i].hitcount/2+1;

    }

    delete(data[worst].list);
    data[worst] = MapEntry(loc, list);
    if (worst_count>1) insertstat.success();    

}

unsigned int LightCache::Hash::calcHashValue(unsigned int res) {
    return res % size;
}

unsigned int LightCache::Hash::calcHashValueSond(unsigned int res) {
    return res % sondmod + 1;
}


LightCache::VarianceHash::VarianceHash() {
    size = 0;
    data = 0;
}

LightCache::VarianceHash::~VarianceHash() {
    delete[] data;
}

bool LightCache::VarianceHash::get(const Vector3f& location, unsigned int gridsize, CLR& variance) {

    unsigned int loc_i = gridsize + location.hashValue();
    unsigned int i = loc_i % size;

    if ( (data[i].location.equals(location)) && (data[i].gridsize == gridsize) ) {
        hitstat.evalSuccess();
        variance = data[i].variance;
        return true;
    }

    hitstat.evalFailed();
    return false;

}


void LightCache::VarianceHash::put(const Vector3f& location, unsigned int gridsize, CLR variance) {

    unsigned int loc_i = gridsize + location.hashValue();
    unsigned int i = loc_i % size;

    data[i] = VarianceMapEntry(location, gridsize, variance);

}



void LightCache::VarianceHash::setSize(int s) {

    delete[] data;

    size = Prime::nextPrimeBiggerThan(s);
    data = new VarianceMapEntry[size];

}
