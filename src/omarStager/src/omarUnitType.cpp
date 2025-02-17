// $Id$

#include <string>

#include <omarUnitType.h>

omarUnitType::omarUnitType()
   : omar2__UnitType()
{
   code = new std::string();
   description = new std::string();
   id = new LONG64;
   soap = 0;
}

omarUnitType::omarUnitType(const omarUnitType& obj )
   : omar2__UnitType()
{
   code         = new std::string();
   description  = new std::string();
   id           = new LONG64;
   *code        = *(obj.code);
   *description = *(obj.description);
   *id          = *(obj.id);
   
   soap = 0;
}

omarUnitType::~omarUnitType()
{
   if (code)
   {
      delete code;
      code = 0;
   }
   if (description)
   {
      delete description;
      description = 0;
   }
   if (id)
   {
      delete id;
      id = 0;
   }

   // "soap" deleted elsewhere...
}

void omarUnitType::setCode(const std::string& arg)
{
   *code = arg;
}

std::string omarUnitType::getCode() const
{
   return *code;
}

void omarUnitType::setDescription(const std::string& arg)
{
   *description = arg;
}

std::string omarUnitType::getDescription() const
{
   return *description;
}

void omarUnitType::setId(LONG64 arg)
{
   *id = arg;
}

LONG64 omarUnitType::getId() const
{
   return *id;
}

