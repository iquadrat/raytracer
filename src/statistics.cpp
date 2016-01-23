/* *************************************************************************
                          statistics.cpp  -  description
                             -------------------
    begin                : Wed Oct 16 2002     
    copyright            : (C) 2002 by Micha Riser
    email                : micha@povworld.org

    $Id: statistics.cpp,v 1.3 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#include "../src/statistics.h"

Statistics::Group* Statistics::global = 0;

Statistics::Statistics(Group* g) {

    if (!global) global = new Group();
    if (!g) g = global;
    
    group = g;
    group->registerStat(this);
    
}


Statistics::~Statistics(){
//    group->unregisterStat(this);
}

void Statistics::Group::registerStat(const Statistics* s) {
     members.push_back(s);
}

void Statistics::Group::unregisterStat(const Statistics* s) {
    std::vector<const Statistics*>::iterator it = members.begin();
    while(it != members.end()) {
        if (*it == s) {
            members.erase(it);
            break;
        }
        ++it;
    }
}

void Statistics::Group::print() const {

    std::cout << "Statistics:\n";
    std::vector<const Statistics*>::const_iterator it = members.begin();
    for(; it!=members.end(); ++it) {
        (*it)->print();
    }

}

void HitStatistics::print() const {

    if (evals)
        std::cout << descr << ":  " << evaldescr << " "<<evals<< " / " << succdescr << " " << succ <<"\n";

}
