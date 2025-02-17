#pragma ident "$Id: OrdEngine.hpp 1266 2008-06-25 15:45:00Z ocibu $"

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
//  Copyright 2004, The University of Texas at Austin
//
//============================================================================

//============================================================================
//
//This software developed by Applied Research Laboratories at the University of
//Texas at Austin, under contract to an agency or agencies within the U.S. 
//Department of Defense. The U.S. Government retains all rights to use,
//duplicate, distribute, disclose, or release this software. 
//
//Pursuant to DoD Directive 523024 
//
// DISTRIBUTION STATEMENT A: This software has been approved for public 
//                           release, distribution is unlimited.
//
//=============================================================================

#ifndef ORDENGINE_HPP
#define ORDENGINE_HPP

#include "ObsEpochMap.hpp"
#include "WxObsMap.hpp"
#include "ORDEpoch.hpp"
#include "XvtStore.hpp"
#include "SatID.hpp"
#include "TropModel.hpp"

class OrdEngine
{
public:
   OrdEngine(
      const gpstk::XvtStore<gpstk::SatID>& e, 
      const gpstk::WxObsData& w,
      const gpstk::Triple& p,
      const std::string& mode,
      gpstk::TropModel& t);

   void setMode(const gpstk::ObsEpoch& obs);

   // The crank for this engine. Input an ObsEpoch, get back an ORDEpoch.
   gpstk::ORDEpoch operator()(const gpstk::ObsEpoch& obs);

   const gpstk::Triple& antennaPos;
   const gpstk::XvtStore<gpstk::SatID>& eph;
   const gpstk::WxObsData& wod;
   gpstk::TropModel& tm;

   bool svTime, forceSvTime;
   bool keepWarts;
   bool keepUnhealthy;
   unsigned long wartCount;
   std::string mode;
   int verboseLevel;
   int debugLevel;

private:
   bool dualFreq;
   bool oidSet;
   gpstk::ObsID oid1, oid2;
   gpstk::GPSGeoid gm;
};

#endif
