#pragma ident "$Id: PreciseRange.hpp 778 2007-09-25 16:14:58Z ocibu $"

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

/**
 * @file PreciseRange.hpp
 * Include file defining class PreciseRange: computation of range and associated
 * quantities from EphemerisStore, including earth orientation corrections,
 * given receiver position and time.
 */
 
//------------------------------------------------------------------------------------
#ifndef EPHEMERIS_RANGE_HPP
#define EPHEMERIS_RANGE_HPP

//------------------------------------------------------------------------------------
// GPSTk
#include "DayTime.hpp"
#include "Position.hpp"
#include "XvtStore.hpp"
#include "SatID.hpp"
#include "Matrix.hpp"
#include "Xvt.hpp"

// DDBase
#include "EarthOrientation.hpp"

//------------------------------------------------------------------------------------
namespace gpstk
{
   /** @addtogroup ephemcalc */
   //@{

   /** class PreciseRange. Compute the corrected range from receiver
    * at position Rx, to the GPS satellite given by PRN prn, as well as azimuth,
    * elevation, etc., given a nominal timetag (either received or transmitted
    * time) and an EphemerisStore.
    */
   class PreciseRange
   {
   public:
         /// Default constructor.
      PreciseRange() {}

      /// Compute the corrected range at RECEIVE time, from receiver at
      /// position Rx, to the GPS satellite given by PRN prn, as well as all
      /// the CER quantities, given the nominal receive time tr_nom and
      /// an EphemerisStore.
      double ComputeAtReceiveTime(const DayTime& tr_nom,
            const Position& Rx, const int prn, const XvtStore<SatID>& Eph,
            const EarthOrientation& EO)
      throw(InvalidRequest);

      /// Compute the corrected range at TRANSMIT time, from receiver at
      /// position Rx, to the GPS satellite given by PRN prn, as well as all
      /// the CER quantities, given the nominal receive time tr_nom and
      /// an EphemerisStore.
      double ComputeAtTransmitTime(const DayTime& tr_nom, const double& pr,
            const Position& Rx, const int prn, const XvtStore<SatID>& Eph,
            const EarthOrientation& EO);

      /// The computed raw (geometric) range in meters.
      double rawrange;
      /// The satellite clock bias in meters.
      double svclkbias;
      /// The satellite clock drift in m/s.
      double svclkdrift;
      /// The relativity correction in meters.
      double relativity;
      /// The satellite elevation, as seen at the receiver, in degrees.
      double elevation;
      /// The satellite azimuth, as seen at the receiver, in degrees.
      double azimuth;
      /// The computed transmit time of the signal.
      DayTime transmit;
      /// The direction cosines of the satellite, as seen at the receiver (XYZ).
      Triple cosines;
      /// The satellite position (m) and velocity (m/s) in ECEF coordinates.
      Position SVR,SVV;
      double SVdtime,SVdrift;

   }; // end class PreciseRange

   /// Compute relativity correction (meters) from the satellite position and velocity
   double PreciseRelativityCorrection(const Position& R, const Position& V);

   //@}

}  // namespace gpstk

#endif
