/* *************************************************************************
                          rendererror.h  -  description
                             -------------------
    begin                : Wed Oct 16 2002    
    copyright            : (C) 2002 by Micha Riser
    email                : mriser@gmx.net

    $Id: rendererror.h,v 1.4 2003/03/03 11:44:06 micha Exp $
    
 ************************************************************************* */

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef RENDERERROR_H
#define RENDERERROR_H

#include<string>

/**Error to throw when problem arises during render process.
  *@author Micha Riser
  */

class RenderError {
    
public: // Constructors and destructor

    /**Create new error with empty error message.
      */
    RenderError():message("") {};

    /**Create new error.
      *@param error message
      */
    RenderError(const std::string& s): message(s) {}
    
    ~RenderError() {}

private: // Private attributes
    const std::string message;
            
};

#endif
