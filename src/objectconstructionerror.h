/* *************************************************************************
                          objectconstructionerror.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002        
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: objectconstructionerror.h,v 1.6 2003/05/25 20:13:33 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef OBJECTCONSTRUCTIONERROR_H
#define OBJECTCONSTRUCTIONERROR_H

#include<string>

/**Error to throw when a problem arises during Object3D creation.
  *@author Micha Riser
  */

class ObjectConstructionError {
public: // Private attributes
    const std::string message;
  
public: 
    ObjectConstructionError(): message() {}
    /**Create new error.
      *@param s error message
      */
    ObjectConstructionError(std::string s): message(s) {}
    ~ObjectConstructionError() {}
};

#endif
