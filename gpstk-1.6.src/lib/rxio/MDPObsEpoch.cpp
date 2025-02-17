#pragma ident "$Id: MDPObsEpoch.cpp 1093 2008-02-26 21:36:53Z vorce $"

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

#include <sstream>
#include <StringUtils.hpp>
#include <StringUtils.hpp>
#include <BinUtils.hpp>
#include <gps_constants.hpp>

#include "MDPObsEpoch.hpp"
#include "MDPStream.hpp"

using gpstk::StringUtils::asString;
using gpstk::BinUtils::hostToNet;
using gpstk::BinUtils::netToHost;
using gpstk::BinUtils::encodeVar;
using gpstk::BinUtils::decodeVar;
using namespace std;

namespace gpstk
{
   //---------------------------------------------------------------------------
   MDPObsEpoch::MDPObsEpoch()
      throw(): 
      numSVs(0), channel(0), prn(0),status(0), elevation(0),azimuth(0)
   {
      id = myId;
   } // MDPObsEpoch::MDPObsEpoch()


   //---------------------------------------------------------------------------
   string MDPObsEpoch::encode() const
      throw()
   {
      string str;
      unsigned short ustemp;
      unsigned char svsobs = (numSVs & 0x0f);
      svsobs <<= 4;
      svsobs |= ((unsigned char)obs.size() & 0x0f);
      
      str += encodeVar( static_cast<uint8_t>(svsobs));
      str += encodeVar( static_cast<uint8_t>(channel));
      str += encodeVar( static_cast<uint8_t>(prn));
      str += encodeVar( static_cast<uint8_t>(status & 0x3f));
      str += encodeVar( static_cast<uint16_t>(elevation * 100));
      str += encodeVar( static_cast<uint16_t>(azimuth * 100));

      for(ObsMap::const_iterator oli = obs.begin(); 
          oli != obs.end(); oli++)
      {
         MDPObsEpoch::Observation mdpobs(oli->second);
         str += mdpobs.encode();
      }
      return str;
   } // MDPObsEpoch::encode()

      
   //---------------------------------------------------------------------------
   void MDPObsEpoch::decode(string str)
      throw()
   {
      if (str.length() < myLength)
         return;

      clearstate(lenbit);

      unsigned obsSVs;
      obsSVs    = decodeVar<uint8_t>(str);
      channel   = decodeVar<uint8_t>(str);
      prn       = decodeVar<uint8_t>(str);
      status    = decodeVar<uint8_t>(str);
      elevation = decodeVar<uint16_t>(str) * 0.01 ;
      azimuth   = decodeVar<uint16_t>(str) * 0.01;

      numSVs = obsSVs >> 4;
      numSVs &= 0x0f;
      obsSVs &= 0x0f;

      bool obsError=false;

      obs.erase(obs.begin(), obs.end());
      for(int j=0; j<obsSVs; j++)
      {
         if (str.length() < myObsLength)
         {
            clear(lenbit);
            if (debugLevel)
               cout << "MDP Obs block decode requires at least " << myObsLength
                    << " bytes.  Received " << str.length() << " bytes" << endl;
            return;
         }
         MDPObsEpoch::Observation o;      
         o.decode(str);

         if (o.carrier >= ccMax || o.range >= rcMax || o.snr > 65 || o.bw > 100)
            obsError=true;

         if (obsError && debugLevel)
         {            
            if (o.carrier >= ccMax)
               cout << "Carrier code out of range: " << o.carrier << endl;
            if (o.range >= rcMax)
               cout << "Range code out of range: " << o.range << endl;
            if (o.snr > 65)
               cout << "SNR out of range: " << o.snr << endl;
            if (o.bw > 100)
               cout << "BW out of range: " << o.snr << endl;
         }
  
         if (o.carrier < ccMax && o.range < rcMax)
         {
            ObsKey key(o.carrier, o.range);
            obs[key] = o;
         }
      }

      if (prn > gpstk::MAX_PRN || elevation>90 || azimuth > 360 || obsError)
      {
         if (debugLevel)
         {
            if (prn > gpstk::MAX_PRN)
               cout << "PRN out of range: " << prn << endl;
            if (elevation > 90)
               cout << "Elevation out of range: " << elevation << endl;
            if (azimuth > 360)
               cout << "Azimuth out of range: " << azimuth << endl;
         }
         return;
      }

      clearstate(fmtbit);
   } // MDPObsEpoch::decode()


   //---------------------------------------------------------------------------
   std::string MDPObsEpoch::Observation::encode() const
      throw()
   {
      string str;

      uint8_t ccrc = carrier & 0x0f;
      ccrc <<= 4;
      ccrc |= range & 0x0f;

      str += encodeVar( (uint8_t)  ccrc);
      str += encodeVar( (uint8_t)  bw);
      str += encodeVar( (uint16_t) std::max(static_cast<int>(snr*100),0));
      str += encodeVar( (uint32_t) lockCount);
      str += encodeVar( (double)   pseudorange);
      str += encodeVar( (double)   phase);
      str += encodeVar( (double)   doppler);
      return str;
   } // MDPObservation::encode()


   //---------------------------------------------------------------------------
   void MDPObsEpoch::Observation::decode(std::string& str)
      throw()
   {
      unsigned char ccrc;

      ccrc        = decodeVar<uint8_t>(str);      
      bw          = decodeVar<uint8_t>(str);
      snr         = decodeVar<uint16_t>(str) * 0.01;
      lockCount   = decodeVar<uint32_t>(str);
      pseudorange = decodeVar<double>(str);
      phase       = decodeVar<double>(str);
      doppler     = decodeVar<double>(str);

      range = RangeCode(ccrc & 0x0f);
      ccrc >>= 4;
      carrier = CarrierCode(ccrc & 0x0f);
   }  // MDPObservation::decode()


   //---------------------------------------------------------------------------
   bool MDPObsEpoch::haveObservation(
      const CarrierCode cc,
      const RangeCode rc) const
   {
      return (obs.find(ObsKey(cc,rc)) != obs.end());
   }


   //---------------------------------------------------------------------------
   MDPObsEpoch::Observation MDPObsEpoch::getObservation(
      const CarrierCode cc, 
      const RangeCode rc) const
   {
      if (haveObservation(cc, rc))
         return obs.find(ObsKey(cc,rc))->second;
      else
         return Observation();
   }


   //---------------------------------------------------------------------------
   FFStream& operator>>(FFStream& s, MDPEpoch& oe)
   {
      MDPStream& mdps = dynamic_cast<MDPStream&>(s);
      MDPObsEpoch me;
      DayTime t;
      int i=0;
      for (int i=0; i<10000 && mdps >> me; i++)
      {
         i++;
         if (me.time != t)
         {
            if (oe.size() > 0 && MDPHeader::debugLevel>2)
               cout << "Tossing partial epoch at " << me.time
                    << ".  Expected " << me.numSVs
                    << " SVs but received only " << oe.size()
                    << endl;
            oe.clear();
         }
         oe.insert(pair<const int, MDPObsEpoch>(me.prn,me));
         t = me.time;

         if (me.numSVs == oe.size())
            break;
      }
      if (i>1000)
         cout << "didn't find an obs epoch after 10000 reads." << endl;
      return s;
   }


   //---------------------------------------------------------------------------
   FFStream& operator<<(FFStream& s, const MDPEpoch& oe)
   {
      MDPStream& mdps = dynamic_cast<MDPStream&>(s);
      MDPEpoch::const_iterator i;
      for (i=oe.begin(); i != oe.end(); i++)
         mdps << i->second;
      return s;
   }


   //---------------------------------------------------------------------------
   void MDPObsEpoch::dump(ostream& out) const
      throw()
   {
      ostringstream oss;

      MDPHeader::dump(oss);

      oss << getName() << "0:"
          << " #SV:" << (int)numSVs
          << " Ch:" << (int)channel
          << " PRN:" << (int)prn
          << " El:" << fixed << setprecision(2) <<elevation
          << " Az:" << azimuth
          << " H:0x" << hex << (int)status
          << dec << endl;

      int j=1;
      for (ObsMap::const_iterator i = obs.begin(); i != obs.end(); i++)
      {
         oss << getName() << j++ << ":";
         i->second.dump(oss);
         oss << endl;
      }
      
      out << oss.str();
   } // MDPObsEpoch::dump()


   //---------------------------------------------------------------------------
   void MDPObsEpoch::Observation::dump(ostream& out) const 
      throw()
   {
      using gpstk::StringUtils::asString;

      ostringstream oss;
      oss << " "     << asString(carrier)
          << " "     << asString(range)
          << " BW:"  << bw
          << " SNR:" << snr
          << " LC:"  << lockCount
          << " PR:"  << asString(pseudorange, 3)
          << " PH:"  << asString(phase, 3)
          << " Dop:" << asString(doppler, 3);      
      out << oss.str();
   } // MDPObsEpoch::Observation::dump()


   //---------------------------------------------------------------------------
   void dump(ostream& s, const MDPEpoch& me)
   {
      MDPEpoch::const_iterator i;
      for (i=me.begin(); i != me.end(); i++)
         i->second.dump(s);
   }
}
