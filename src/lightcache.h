/* *************************************************************************
                          lightcache.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: lightcache.h,v 1.6 2003/07/15 00:36:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef LIGHTCACHE_H
#define LIGHTCACHE_H

#include "../src/colour.h"
#include "../src/lightlist.h"
#include "../src/statistics.h"
#include "../src/vector.h"

class Scene;

/**Light Cache for Media calculation
  *@author Micha Riser
  */
class LightCache {

public: // Constructors and destructor
    LightCache(const Scene* s);

private:
    LightCache(const LightCache&);
    LightCache& operator=(const LightCache&);

public: // Public methods

    /**Set cache interpolation type and cache size.
      */
    void setType(int size);

    /**Looks up a point given by location and returns the list of lights
      *there. 
      *@param location lookup point
      *@param err returns approx. maximum error in lookup
      *@param precision space tolerance for lookup
      */
    LightList* lookup(const Vector3& location, CLR err, FLT& precision, FLT variance_keep_factor);

    static int sampleCounts() {
        return Hash::insertstat.getEvalCount();
    }

private: // Inner classes

    /**MapEntry holds one element of the cache.
      */
    class MapEntry {

    public: // Public attributes
        Vector3f location;
        LightList* list;
        unsigned int hitcount;

    public: // Constructor and destructor
        MapEntry(const Vector3f& l, LightList* data):
          location(l), list(data), hitcount(initialcount) {}
  
        MapEntry(): list(0), hitcount(0) {}

        ~MapEntry() {}
        
    };


    /**Holds the data in a hash data structure.
      */
    class Hash {

    public: // Static members

        /**Counts number of cache evaluations and successes.
          */
        static HitStatistics hitstat;
        /**Counts number of data insertions and collisions thereby.
          */
        static HitStatistics insertstat;
            
    public: // Constructor and destructor
        Hash();
        ~Hash();
    private:
        Hash(const Hash&);
        Hash& operator=(const Hash&);

    public: // Public methods
        void setSize(int size);
        bool get(const Vector3f& location, LightList*&);
        void put(const Vector3f& location, LightList* l);

    private: // Private attributes
        int size, sondmod;
        MapEntry* data;

    private: // Private methods
        unsigned int calcHashValue(unsigned int);
        unsigned int calcHashValueSond(unsigned int);

    };


    class VarianceMapEntry {
    public:
        Vector3f location; 
        unsigned int gridsize;
        CLR variance;
    public:
        VarianceMapEntry(): location(0), gridsize(0), variance(0) {};
        VarianceMapEntry(const Vector3f& l, unsigned int size, CLR var):
          location(l), gridsize(size), variance(var) {};
    };


    class VarianceHash {

    public:
        static HitStatistics hitstat;
        VarianceHash();
        ~VarianceHash();
    private:
        VarianceHash(const VarianceHash&);
        VarianceHash& operator=(const VarianceHash&);

    public: // Public methods
        void setSize(int size);
        bool get(const Vector3f& location, unsigned int gridsize, CLR& variance);
        void put(const Vector3f& location, unsigned int gridsize, CLR variance);

    private: // Private attributes
        int size, sondmod;
        VarianceMapEntry* data;

    };


private: // Constants
    // these constants are set quite arbitrarly, affect fine-tune of the cache
    static const int sondsize = 3;
    static const unsigned int hitbonus = 1000;
    static const unsigned int initialcount = 1000;

private: // Private methods
    LightList* sample(const Vector3f& p);

private: // Private attributes
    const Scene* scene;
    Hash map;
    VarianceHash varmap;

};

#endif
