/* *************************************************************************
                          statistics.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: statistics.h,v 1.4 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef STATISTICS_H
#define STATISTICS_H

#include<string>
#include<vector>
#include<iostream>

/**Provides a class for gathering statistical values during rendering.
  *@author Micha Riser
  */
class Statistics {

public: // Inner classes

    /**Statistical group.
      */
    class Group {

    public: // Constructor and destructor
        Group(): members() {}
        ~Group() {}

    public: // Public methods
        void registerStat(const Statistics* s);
        void unregisterStat(const Statistics* s);
        void print() const;

    private: // Private attributes
        std::vector<const Statistics*> members;

    };
    
public: // Static members
    /**Default group.
      */
    static Group* global;

    static void init() {if (!global) global = new Group();}
    static void finish() {delete(global); global = 0;}
    
public: // Constructor and destructor
    Statistics(Group* g=0);
    virtual ~Statistics();
private:
    Statistics(const Statistics&);
    Statistics& operator=(const Statistics&);

public: // Pulblic pure virtual methods
    virtual void print() const = 0;

private: // Private attributes
    Group* group;

};

class HitStatistics: public Statistics {

public: // Constructor and destructor
  HitStatistics(const std::string description, const std::string evaldescription,
    const std::string succdescription, Group* g = 0): Statistics(g), descr(description),
    evaldescr(evaldescription), succdescr(succdescription), evals(0), succ(0) {}

public: // Public methods
    void eval() {evals++;}
    void success() {succ++;}
    void evalSuccess() {evals++; succ++;}
    void evalFailed() {evals++;}

    int getSuccessCount() {return succ;}
    int getEvalCount() {return evals;}

    void print() const;
            
private: // Private attributes
    const std::string descr;
    const std::string evaldescr;
    const std::string succdescr;
    int evals;
    int succ;
};

template <class Type> class MaxStatistics: public Statistics {

public: // Constructor and destructor
    MaxStatistics(const std::string description): descr(description), currmax(0) {}

public: // Public methods
    void update(Type t) {if (t>currmax) currmax=t;}

    void print() const {
          std::cout << descr <<": "<<currmax<<"\n";
    }

private:
    const std::string descr;
    Type currmax;

};

#endif
