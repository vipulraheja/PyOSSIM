#pragma ident "$Id: SEMStream.hpp 383 2007-01-30 16:23:29Z ehagen $"



/**
 * @file SEMStream.hpp
 * gpstk::SEMStream - ASCII SEM file stream container.
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






#ifndef SEMSTREAM_HPP
#define SEMSTREAM_HPP


#include "FFTextStream.hpp"
#include "SEMHeader.hpp"

namespace gpstk
{
   /** @addtogroup SEM */
   //@{

      /**
       * This class performs file i/o on a SEM file for the 
       * SEMHeader and SEMData classes.
       *
       * @sa tests/SEM for examples.
       * @sa SEMData.
       * @sa SEMHeader for information on writing SEM files.
       *
       * @warning The SEM header information and data information don't
       * correctly talk to each other at the time of completion of this file.
       * The current fix is in SEMAlamanacStore.hpp.
       */
   class SEMStream : public FFTextStream
   {
   public:
         
	 /// Default constructor
      SEMStream() {}
      
         /**
          * Constructor
          * @param fn the name of the ascii SEM file to be opened
          * @param mode the ios::openmode to be used on \a fn
          */
      SEMStream(const char* fn,
                std::ios::openmode mode=std::ios::in)
            : FFTextStream(fn, mode), headerRead(false) {};

         /// destructor per the coding standards
      virtual ~SEMStream() {}

         /// overrides open to reset the header
      virtual void open(const char* fn, std::ios::openmode mode)
         {
	    FFTextStream::open(fn, mode);
	    headerRead = false;
	    header = SEMHeader();
	 }
	 
	 ///SEMHeader for this file
      SEMHeader header;
         
	 /// Flag showing whether or not the header has been read.
      bool headerRead;

   }; // class SEMStream
   
   //@}
   
} // namespace gpstk

#endif
