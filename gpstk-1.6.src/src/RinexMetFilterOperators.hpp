#pragma ident "$Id: RinexMetFilterOperators.hpp 438 2007-03-21 17:22:21Z btolman $"



/**
 * @file RinexMetFilterOperators.hpp
 * Operators for FileFilter using Rinex meteorological data
 */

#ifndef GPSTK_RINEXMETFILTEROPERATORS_HPP
#define GPSTK_RINEXMETFILTEROPERATORS_HPP

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






#include "DayTime.hpp"
#include "FileFilter.hpp"
#include "RinexMetData.hpp"
#include "RinexMetHeader.hpp"

#include <set>

namespace gpstk
{
   /** @addtogroup RinexMet */
   //@{

   typedef std::unary_function<gpstk::RinexMetHeader, bool> RinexMetDataUnaryOperator;
   typedef std::binary_function<gpstk::RinexMetData, gpstk::RinexMetData, bool> RinexMetDataBinaryOperator;

      /// This compares all elements of the RinexMetData with less than
      /// (only for those fields which the two obs data share).
   struct RinexMetDataOperatorLessThanFull : 
      public RinexMetDataBinaryOperator
   {
   public:
         /// The set is a set of RinexMetType that the two files have in 
         /// common.  This is easily generated with the set_intersection
         /// STL function.  See difftools/rmwdiff.cpp for an example.
      RinexMetDataOperatorLessThanFull
      (const std::set<gpstk::RinexMetHeader::RinexMetType>& rmhset)
            : obsSet(rmhset)
         {}

      bool operator()(const gpstk::RinexMetData& l,
                      const gpstk::RinexMetData& r) const
         {
               // compare the times, offsets, then only those elements
               // that are common to both.  this ignores the flags
               // that are set to 0
            if (l.time < r.time)
               return true;
            else if (l.time != r.time)
               return false;
            
               // then check that each observation has the same data
               // for each item in the set of common observations
            gpstk::RinexMetData::RinexMetMap::const_iterator 
               lItr, rItr;
            std::set<gpstk::RinexMetHeader::RinexMetType>::const_iterator
               obsItr = obsSet.begin();
         
            while (obsItr != obsSet.end())
            {
               rItr = r.data.find(*obsItr);
               if (rItr == r.data.end())
                  return false;

               lItr = l.data.find(*obsItr);
               if (lItr == l.data.end())
                  return false;

               if ((*lItr).second < (*rItr).second)
                  return true;
               if ((*lItr).second > (*rItr).second)
                  return false;

               obsItr++;
            }

               // the data is either == or > at this point
            return false;
         }

   private:
      std::set<gpstk::RinexMetHeader::RinexMetType> obsSet;
   };

      /// Only compares times
   struct RinexMetDataOperatorLessThanSimple : 
      public RinexMetDataBinaryOperator
   {
   public:
      bool operator()(const gpstk::RinexMetData& l,
                      const gpstk::RinexMetData& r) const
         {
            if (l.time < r.time)
               return true;
            return false;
         }
   };

      /// Only compares times
   struct RinexMetDataOperatorEqualsSimple : 
      public RinexMetDataBinaryOperator
   {
   public:
      bool operator()(const gpstk::RinexMetData& l,
                      const gpstk::RinexMetData& r) const
         {
            if (l.time == r.time)
               return true;
            return false;
         }
   };

      /// Combines RinexMetHeaders into a single header, combining comments
      /// and adding the appropriate RinexMetTypes.  This assumes that
      /// all the headers come from the same station for setting the other
      /// header fields. After running touch() on a list of RinexMetHeader,
      /// the internal theHeader will be the merged header data for
      /// those files and obsSet will be the set of RinexMetTypes that
      /// will be printed to the file.
   struct RinexMetHeaderTouchHeaderMerge :
      public RinexMetDataUnaryOperator
   {
   public:
      RinexMetHeaderTouchHeaderMerge()
            : firstHeader(true)
         {}

      bool operator()(const gpstk::RinexMetHeader& l)
         {
            if (firstHeader)
            {
               theHeader = l;
               firstHeader = false;
            }
            else
            {
               std::set<gpstk::RinexMetHeader::RinexMetType> thisMetSet, 
                  tempMetSet;
               std::set<std::string> commentSet;
               obsSet.clear();

                  // insert the comments to the set
                  // and let the set take care of uniqueness
               copy(theHeader.commentList.begin(),
                    theHeader.commentList.end(),
                    inserter(commentSet, commentSet.begin()));
               copy(l.commentList.begin(),
                    l.commentList.end(),
                    inserter(commentSet, commentSet.begin()));
                  // then copy the comments back into theHeader
               theHeader.commentList.clear();
               copy(commentSet.begin(), commentSet.end(),
                    inserter(theHeader.commentList,
                             theHeader.commentList.begin()));

                  // find the set intersection of the obs types
               copy(theHeader.obsTypeList.begin(),
                    theHeader.obsTypeList.end(),
                    inserter(thisMetSet, thisMetSet.begin()));
               copy(l.obsTypeList.begin(),
                    l.obsTypeList.end(),
                    inserter(tempMetSet, tempMetSet.begin()));
               set_intersection(thisMetSet.begin(), thisMetSet.end(),
                                tempMetSet.begin(), tempMetSet.end(),
                                inserter(obsSet, obsSet.begin()));
                  // then copy the obsTypes back into theHeader
               theHeader.obsTypeList.clear();
               copy(obsSet.begin(), obsSet.end(),
                    inserter(theHeader.obsTypeList, 
                             theHeader.obsTypeList.begin()));
            }
            return true;
         }

      bool firstHeader;
      gpstk::RinexMetHeader theHeader;
      std::set<gpstk::RinexMetHeader::RinexMetType> obsSet;
   };


      /// This filter will remove any data not within the specified time range.
   struct RinexMetDataFilterTime : public RinexMetDataUnaryOperator
   {
   public:
      RinexMetDataFilterTime(const gpstk::DayTime& startTime,
                             const gpstk::DayTime& endTime)
            : start(startTime), end(endTime)
      {}
      
      bool operator() (const gpstk::RinexMetData& l) const
      {
         if ( (l.time < start) ||
              (l.time >= end))
            return true;
         return false;
      }
      
   private:
      gpstk::DayTime start, end;
   };

   //@}

}


#endif
