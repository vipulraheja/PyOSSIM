#pragma ident "$Id: ExtractC1.hpp 1889 2009-05-11 15:47:23Z afarris $"

/**
 * @file ExtractC1.hpp
 * This class eases C1 data extraction from a RinexObsData object.
 */

#ifndef ExtractC1_GPSTK
#define ExtractC1_GPSTK

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
//  Dagoberto Salazar - gAGE. 2006
//
//============================================================================



#include "ExtractData.hpp"


namespace gpstk
{

    /** @addtogroup RinexObs */
    //@{


    /// This class eases C1 data extraction from a RinexObsData object.
    class ExtractC1 : public ExtractData
    {
    public:

        /// Default constructor
        ExtractC1() throw(InvalidData) : valid(false) 
        {
            checkData = true;
        };


        /** Pull out the C1 observation from a RinexObsData object
         * @param rinexData     The Rinex data set holding the observations
         *
         * @return
         *  Number of satellites with C1 data available
         */
        inline virtual int getData(const RinexObsData& rinexData) throw(InvalidData)
        {
            return ExtractData::getData(rinexData, RinexObsHeader::C1);
        };


        /// Destructor
        inline virtual ~ExtractC1() {};


    protected:
        bool valid;         // true only if results are valid

   }; // end class ExtractData
   

   //@}
   
}

#endif
