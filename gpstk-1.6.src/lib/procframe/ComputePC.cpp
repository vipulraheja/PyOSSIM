#pragma ident "$Id: ComputePC.cpp 1308 2008-07-22 20:01:04Z architest $"

/**
 * @file ComputePC.cpp
 * This class eases computing PC combination for GNSS data structures.
 */

//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Dagoberto Salazar - gAGE ( http://www.gage.es ). 2007, 2008
//
//============================================================================


#include "ComputePC.hpp"


namespace gpstk
{

      // Index initially assigned to this class
   int ComputePC::classIndex = 2250000;


      // Returns an index identifying this object.
   int ComputePC::getIndex() const
   { return index; }


      // Returns a string identifying this object.
   std::string ComputePC::getClassName() const
   { return "ComputePC"; }


      // Default constructor
   ComputePC::ComputePC()
      : DEN(0.646944444)
   {
      type1 = TypeID::P1;
      type2 = TypeID::P2;
      resultType = TypeID::PC;
      setIndex();
   }


} // End of namespace gpstk
