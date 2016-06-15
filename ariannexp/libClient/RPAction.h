/* $Id: RPAction.h,v 1.7 2004/03/31 15:28:47 arianne_rpg Exp $ */
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

#ifndef ARIANNE_RPAction_H
#define ARIANNE_RPAction_H

#include "Attributes.h"

namespace arianne
  {
  class RPAction:	public Attributes
    {
    public:
    RPAction();
    RPAction(RPAction const& copy);
    ~RPAction();
    
    bool equals(arianne::RPAction const& b) const;
    };

  bool operator==(arianne::RPAction const& a, arianne::RPAction const& b);
  }
  
#endif
