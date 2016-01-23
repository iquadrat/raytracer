/* *************************************************************************
                          intervalmap.h  -  description
                             -------------------
    begin                : Fri Oct 18 2002
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: intervalmap.h,v 1.5 2003/05/03 20:53:24 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef INTERVALMAP_H
#define INTERVALMAP_H

#include<vector>

#include "../src/function.h"
#include "../src/types.h"
#include "../src/vector.h"

/**Template for interval maps. Orders some interpolatable object for each
  *value in given range. Eval class has to support 'add' and 'scale' method.
  *@author Micha Riser
  */
template <class Interpol, class Eval> class IntervalMap {

public: // Constructor and destructor
    /**Create empty interval map.
      */
    IntervalMap(): data() {}

    /**Copy constructor.
      */
    IntervalMap(const IntervalMap& im): data() {
        for(unsigned int i=0; i<im.data.size(); ++i) {
            data.push_back(Entry(im.data[i].value, im.data[i].obj->copy() ));
        }        
    }

    virtual ~IntervalMap() {

        for(unsigned int i=0; i<data.size(); ++i) {
            delete(data[i].obj);
        }
        
    }

public: // Public Methods

    /**Insert entry into interval map.
      */
    void insert(FLT value, const Interpol* obj) {
        Entry newentry = Entry(value,obj);
        data.push_back(newentry);
        int i = data.size()-1;
        while(i>0 && data[i].value<data[i-1].value) {
            // swap i and i-1
            // i is newentry
            data[i]=data[i-1];
            data[i-1]=newentry;
            i--;
        }
    }

    Eval lookup(FLT value, const Vector3& v) const { 

        // do binary search
        int i = 0;
        int j = data.size()-1;

        while(i<j-1) {
            int k = (i+j)/2;
            if ( data[k].value>value) {
                j = k;
            } else {
                i = k;
            }
        }

        // do interpolation

        // exceptional cases
        if (i==j) return data[i].obj->evaluateAt(v);
        if (value<data[i].value) return data[i].obj->evaluateAt(v);
        if (value>data[j].value) return data[j].obj->evaluateAt(v);

        // normal case
        return linearInterpolate(i,j,value,v);

    }

private: // Inner classes
    class Entry {
    public: // Public attributes
        FLT value; // DBL?
        const Interpol* obj;
    public: // Constructor and destructor
        Entry(FLT v, const Interpol* o): value(v), obj(o) {};
    };
        
private: // Private attributes
    std::vector<Entry> data;

private: // Private methods
    Eval linearInterpolate(int a, int b, FLT v, const Vector3& point) const {
        Eval tmp1 = data[a].obj->evaluateAt(point);
        Eval tmp2 = data[b].obj->evaluateAt(point);
        FLT va = data[a].value;
        FLT vb = data[b].value;
        tmp1.scale( (vb-v)/(vb-va) );
        tmp2.scale( (v-va)/(vb-va) );
        tmp1.add(tmp2);
        return tmp1;
    }

};

#endif
