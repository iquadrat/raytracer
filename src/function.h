/* *************************************************************************
                          function.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002 
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: function.h,v 1.4 2003/03/03 11:44:05 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef FUNCTION_H
#define FUNCTION_H

/**Template for a function class.
  *@author Micha Riser
  */

template <class Domain, class Range> class Function {  
public: // Constructor and destructor
    Function() {}
    virtual ~Function() {}

public: // Pure virtual methods
    /**Evaluates function.
      */
    virtual Range evaluateAt(const Domain&) const = 0;
};

#endif
