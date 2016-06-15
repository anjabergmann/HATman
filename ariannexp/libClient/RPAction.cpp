#include "RPAction.h"
#include "RPObject.h"

arianne::RPAction::RPAction():
  Attributes(arianne::defaultRPClass::getDefaultRPClass())
  {
  }
  
arianne::RPAction::RPAction(RPAction const& copy):
  Attributes(copy)
  {
  }

arianne::RPAction::~RPAction()
  {
  }

bool arianne::RPAction::equals(arianne::RPAction const& b) const
  {
  return Attributes::equals(b);
  }

bool arianne::operator==(arianne::RPAction const& a, arianne::RPAction const& b)
  {
  return a.equals(b);
  }
