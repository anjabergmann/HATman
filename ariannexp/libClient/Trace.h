/* $Id: Trace.h,v 1.2 2004/03/31 15:28:47 arianne_rpg Exp $ */
/***************************************************************************
    copyright            : (C) 2004 by Miguel Angel Blanch Lardin
 ***************************************************************************
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ARIANNE_Trace_H
#define ARIANNE_Trace_H

#include <string>
#include <fstream>

namespace arianne
  {
  class Trace
    {
    std::ofstream *os;

    public:
    Trace();
    ~Trace();
  
    void add(std::string const& method, std::string const& event, std::string const& info="");
    };

  extern Trace global_trace;
  }
  
#endif
