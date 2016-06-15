/* $Id: RPObject.h,v 1.18 2004/07/05 10:16:25 arianne_rpg Exp $ */
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

#ifndef ARIANNE_RPObject_H
#define ARIANNE_RPObject_H

#include "Attributes.h"
#include "RPSlot.h"

namespace arianne
  {
  class SlotNotFoundException: public std::exception
    {
    std::string name;

    public:
    SlotNotFoundException(std::string name);
    ~SlotNotFoundException() throw();
    const char *what() const throw();
    };
  
  class RPClass
    {
    public:
    enum VISIBILITY{VISIBLE=1,HIDDEN=2};
    enum TYPE_ATTRIBUTE{STRING=1,SHORT_STRING=2,INT=3,SHORT=4,BYTE=5,FLAG=6};
    
    private:
    struct AttributeDesc
      {
      Sint16 code;
      std::string name;
      TYPE_ATTRIBUTE type;
      VISIBILITY visibility;   

	  virtual void write(arianne::Serializer &s) const;	  
  	  virtual void read(arianne::Serializer &s);
      };
    
    protected:  
    RPClass *parent;
    std::string name;
    std::map<std::string,AttributeDesc> attributes;
    
    static std::map<std::string, RPClass*> rpclasses;

    public:
    void isA(RPClass *parent);
    bool add(std::string const& name, TYPE_ATTRIBUTE type, VISIBILITY visibility=VISIBLE);
    std::string const& getName() const;
    virtual Sint16 getCode(std::string const& name) const;
    virtual std::string getName(Uint16 code) const;
    virtual TYPE_ATTRIBUTE getType(std::string const& name) const;
    virtual VISIBILITY getVisibility(std::string const& name) const;
    virtual bool hasAttribute(std::string const& name) const;

	virtual void read(arianne::Serializer &s);
    
    static bool hasRPClass(std::string const& name);
    static RPClass *getRPClass(std::string const& name);
    static void addRPClass(RPClass *rpclass);
    static int size();
    };
  
  class defaultRPClass: public RPClass
    {
    static defaultRPClass *defaultrpclass;
    
    defaultRPClass();
    public:
    static defaultRPClass *getDefaultRPClass();
    Sint16 getCode(std::string const& name) const;
    std::string getName(Uint16 code) const;
    TYPE_ATTRIBUTE getType(std::string const& name) const;
    VISIBILITY getVisibility(std::string const& name) const;
    bool hasAttribute(std::string const& name) const;
    };
    
  class RPObject: public Attributes
    {
    std::map<std::string,RPSlot> m_slots;

    public:
    RPObject();
    virtual ~RPObject();
    
    void addSlot(RPSlot const& slot);
    /** the slots is own by RPObject */
    RPSlot* getSlot(std::string const& name);
    void removeSlot(std::string const& name);
    bool hasSlot(std::string const& name) const;    
    
	virtual void write(arianne::Serializer &s) const;
	virtual void read(arianne::Serializer &s);

    bool equals(arianne::RPObject const& b) const;
    std::string toString() const;
    Uint32 size();    

    void applyDifferences(RPObject *added,RPObject *deleted);
    void applyAddedDifferences(RPObject added);
    void applyDeletedDifferences(RPObject deleted);
    
    void removeAllHidden();
    };

  bool operator==(arianne::RPObject const& a, arianne::RPObject const& b);
  bool operator<(arianne::RPObject const& a, arianne::RPObject const& b);
  std::ostream& operator<<(std::ostream& s, arianne::RPObject const& object);

  arianne::Serializer& operator<<(arianne::Serializer& s, arianne::RPObject const& b);
  arianne::Serializer& operator>>(arianne::Serializer& s, arianne::RPObject& b);
  }
  
#endif
