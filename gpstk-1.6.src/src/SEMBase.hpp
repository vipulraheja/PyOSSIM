#pragma ident "$Id: SEMBase.hpp 383 2007-01-30 16:23:29Z ehagen $"



/**
 * @file SEMBase.hpp
 * gpstk::SEMBase - base class for SEM FFData.
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






#ifndef SEMBASE_HPP
#define SEMBASE_HPP

#include "FFData.hpp"

namespace gpstk
{

   /** @defgroup SEM SEM Almanac files */
   //@{
   
      /**
       * This is a base class for all SEM-related classes.
       */
   class SEMBase : public FFData
   {
   
   public:
   
         /// in deference to our coding standards, this is declared virtual
      virtual ~SEMBase() {}
      
   };
   
   //@}
}

#endif
