#pragma ident "$Id: ElevationRange.hpp 441 2007-03-22 12:52:38Z ocibu $"

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

#ifndef ELEVATIONRANGE_HPP
#define ELEVATIONRANGE_HPP

#include <iostream>
#include <list>

typedef std::pair<float, float> ElevationRange;
typedef std::list< ElevationRange > ElevationRangeList;

inline void dump(std::ostream& s, const ElevationRange& er)
{
   s << er.first << "-" << er.second;
};

inline std::ostream& operator<<(std::ostream& s, const ElevationRange& er)
{
   dump(s, er);
   return s;
};

inline void dump(std::ostream& s, const ElevationRangeList& erl)
{
   for (ElevationRangeList::const_iterator i=erl.begin(); i != erl.end(); i++)
      s << *i << " ";
   s << std::endl;
};

inline std::ostream& operator<<(std::ostream& s, const ElevationRangeList& erl)
{
   dump(s, erl);
   return s;
};

#endif
