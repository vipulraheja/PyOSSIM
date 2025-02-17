#pragma ident "$Id: ordGen.cpp 1613 2009-01-22 15:28:08Z ocibu $"

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


#include <MSCData.hpp>
#include <MSCStream.hpp>
#include <GPSEphemerisStore.hpp>


#include "OrdApp.hpp"
#include "OrdEngine.hpp"
#include "ObsReader.hpp"
#include "EphReader.hpp"
#include "MetReader.hpp"

using namespace std;
using namespace gpstk;
using namespace gpstk::StringUtils;

   //---------------------------------------------------------------------------------
   /// The ATS implimentation of a black trop model.
   class ATSTropModel : public TropModel
   {
   public:
      virtual double correction(double elevation) const
         throw(InvalidTropModel)
      {
         double zenith_delay= 2.4225; // meters
         double scale_height=7492.83; // meters
         double height = 0; // Is this an altitude above the geoid?
         double delay = 93.6;
         double el = elevation * 2.0 * gpstk::PI / 360.0;
         if (elevation>0)
            delay = zenith_delay * std::exp(-height/scale_height)/(std::sin(el) + 0.026);
         return delay;
      }

      virtual double correction(const Position& RX,
                                const Position& SV,
                                const DayTime& tt)
         throw(InvalidTropModel)
      { return 0.0; }

      virtual double correction(const Xvt& RX,
                                const Xvt& SV,
                                const DayTime& tt)
         throw(InvalidTropModel)
      { return 0.0; }

      virtual double dry_zenith_delay(void) const
         throw(InvalidTropModel)
      { return 0.0; }

      virtual double wet_zenith_delay(void) const
         throw(InvalidTropModel)
      { return 0.0; }

      virtual double dry_mapping_function(double elevation)
         const throw(InvalidTropModel)
      { return 0.0; }

      virtual double wet_mapping_function(double elevation)
         const throw(InvalidTropModel)
      { return 0.0; }

   }; // end class ATSTropModel


class OrdGen : public OrdApp
{
public:
   OrdGen() throw();
   
   bool initialize(int argc, char *argv[]) throw();

protected:
   virtual void spinUp();
   virtual void process();

private:
   string ordMode;
   Triple antennaPos;
   unsigned msid;
   string tropModel;
   bool useNear;
   bool forceSvTime;

   CommandOptionWithAnyArg obsFileOption, ephFileOption, metFileOption;
};

//-----------------------------------------------------------------------------
// The constructor basically just sets up all the defaults
//-----------------------------------------------------------------------------
OrdGen::OrdGen() throw()
   : OrdApp("ordGen", "Generates observed range deviations."),
     ordMode("smart"), useNear(false),tropModel("nb"),
     msid(0),
     obsFileOption('o', "obs", "Where to get the obs data.", true),
   
     ephFileOption('e', "eph",  "Where to get the ephemeris data. Acceptable "
                "formats include " + EphReader::formatsUnderstood() + ".", true),

     metFileOption('w', "weather", "Weather data file name (RINEX met "
                    "format only).")
{}


//-----------------------------------------------------------------------------
// Here the command line options are set up, parsed, and used to configure
// the program.
//-----------------------------------------------------------------------------
bool OrdGen::initialize(int argc, char *argv[]) throw()
{
   CommandOptionWithAnyArg
      mscFileOption('c', "msc", "Station coordinate file."),

      ordModeOption('\0', "omode", "Specifies what observations are used to "
                    "compute the ORDs. Valid values are:"
                    "p1p2, z1z2, c1p2, c1c2, c1y2, c1z2, y1y2, c1, p1, y1, z1, c2, p2, y2, "
                    "z2, smo, dynamic, and smart. The default is " + ordMode),
   
      tropModelOption('\0', "trop-model", "Specify the trop model to use. "
                      "Options are zero, simple, nb, and gg. The default is nb."),

      antennaPosOption('p', "pos", "Location of the antenna in meters ECEF.");
   
   CommandOptionWithNumberArg 
      msidOption('m', "msid", "Station to process data for. Used to "
                 "select a station position from the msc file or data "
                 "from a SMODF file.");

   CommandOptionNoArg
      useNearOption('n', "near", "Allows the program to select an ephemeris "
                    "that is not strictly in the future. Only affects the "
                    "selection of which broadcast ephemeris to use."),
      
      forceSvTimeOption('\0', "sv-time", "Assume that the data is time-tagged "
                        "according to each SV's clock, not a common receiver "
                        "clock. The is set by default only for omode=smo");


   if (!OrdApp::initialize(argc,argv)) return false;

   if (ordModeOption.getCount())
      ordMode = lowerCase(ordModeOption.getValue()[0]);

   if (msidOption.getCount())
      msid = asUnsigned(msidOption.getValue().front());

   if (tropModelOption.getCount())
      tropModel = lowerCase(tropModelOption.getValue()[0]);
   else
      tropModel = "nb";

   // Get the station position
   if (antennaPosOption.getCount())
   {
      string aps = antennaPosOption.getValue()[0];
      if (numWords(aps) != 3)
      {
         cerr << "Please specify three coordinates in the antenna postion." << endl;
         return false;
      }
      else
         for (int i=0; i<3; i++)
            antennaPos[i] = asDouble(word(aps, i));
   }
   else if (msid && mscFileOption.getCount() > 0)
   {
      string mscfn = (mscFileOption.getValue())[0];
      MSCStream msc(mscfn.c_str(), ios::in);
      MSCData mscd;
      while (msc >> mscd && mscd.station != msid)
         ;
      if (mscd.station == msid)
         antennaPos = mscd.coordinates;
   }
   else
   {
      string fn = (obsFileOption.getValue())[0];
      ObsReader obsReader(fn, debugLevel);
      if (obsReader.inputType == FFIdentifier::tRinexObs)
         antennaPos = obsReader.roh.antennaPosition;
   }

   useNear = useNearOption.getCount();

   forceSvTime = forceSvTimeOption.getCount();

   if (RSS(antennaPos[0], antennaPos[1], antennaPos[2]) < 1)
   {
      cerr << "Warning! The antenna appears to be within one meter of the" << endl
           << "center of the geoid. This program is not capable of" << endl
           << "accurately estimating the propigation of GNSS signals" << endl
           << "through solids such as a planetary crust or magma. Also," << endl
           << "if this location is correct, your antenna is probably" << endl
           << "no longer in the best of operating condition." << endl;
      return false;
   }

   return true;
}


//-----------------------------------------------------------------------------
// General program setup
//-----------------------------------------------------------------------------
void OrdGen::spinUp()
{
   if (verboseLevel)
   {
      if (msid)
         cout << "# msid: " << msid << endl;
      cout << "# Antenna Position: " << setprecision(8) << antennaPos << endl;
   }
}


//-----------------------------------------------------------------------------
void OrdGen::process()
{
   // This is only needed to help debug the FFIdentifer class
   FFIdentifier::debugLevel = debugLevel;

   // Get the ephemeris data
   EphReader ephReader;
   ephReader.verboseLevel = verboseLevel;
   for (int i=0; i<ephFileOption.getCount(); i++)
      ephReader.read(ephFileOption.getValue()[i]);
   gpstk::XvtStore<SatID>& eph = *ephReader.eph;

   if (useNear && typeid(eph) == typeid(GPSEphemerisStore))
   {
      GPSEphemerisStore& bce = dynamic_cast<GPSEphemerisStore&>(eph);
      bce.SearchNear();
   }

   // Get the weather data...
   MetReader metReader;
   metReader.verboseLevel = verboseLevel;
   for (int i=0; i<metFileOption.getCount(); i++)
      metReader.read(metFileOption.getValue()[i]);
   WxObsData& wod = metReader.wx;

   TropModel* tm; 
   if (tropModel=="nb")
      tm = new NBTropModel;
   else if (tropModel=="zero")
      tm = new ZeroTropModel;
   else if (tropModel=="simple")
      tm = new SimpleTropModel;
   else if (tropModel=="ats")
      tm = new ATSTropModel;
   else if (tropModel=="gg")
      tm = new GGTropModel;
   else
   {
      cerr << "Unknown trop model requested: " << tropModel << ". Exiting." << endl;
      return;
   }

   // Now set up the function object that is used to compute the ords.
   OrdEngine ordEngine(eph, wod, antennaPos, ordMode, *tm);
   ordEngine.verboseLevel = verboseLevel;
   ordEngine.debugLevel = debugLevel;
   if (forceSvTime)
      ordEngine.forceSvTime = true;

   ORDEpochMap ordEpochMap;

   // Walk through each obs file, reading and computing ords along the way.
   for (int i=0; i<obsFileOption.getCount(); i++)
   {
      string fn = (obsFileOption.getValue())[i];
      ObsReader obsReader(fn, debugLevel);
      obsReader.msid = msid;

      while (obsReader)
      {
         ObsEpoch obs(obsReader.getObsEpoch());

         if (!obsReader)
            break;

         ORDEpoch oe = ordEngine(obs);

         write(output, oe);
      }
   }
}


//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
   try
   {
      OrdGen crap;
      if (!crap.initialize(argc, argv))
         exit(0);
      crap.run();
   }
   catch (gpstk::Exception &exc)
   { cerr << exc << endl; }
   catch (std::exception &exc)
   { cerr << "Caught std::exception " << exc.what() << endl; }
   catch (...)
   { cerr << "Caught unknown exception" << endl; }
}
