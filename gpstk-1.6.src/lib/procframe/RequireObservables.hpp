#pragma ident "$Id: RequireObservables.hpp 1460 2008-11-18 15:41:23Z ckiesch $"

/**
 * @file RequireObservables.hpp
 * This class filters out satellites with observations grossly out of bounds.
 */

#ifndef REQUIREOBSERVABLES_HPP
#define REQUIREOBSERVABLES_HPP

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



#include "ProcessingClass.hpp"


namespace gpstk
{

      /** @addtogroup DataStructures */
      //@{


      /** This class filters out satellites that don't have the required 
       *  observations.
       *
       * This class is meant to be used with the GNSS data structures objects
       * found in "DataStructures" class.
       *
       * A typical way to use this class follows:
       *
       * @code
       *   RinexObsStream rin("ebre0300.02o");
       *
       *   gnssRinex gRin;
       *
       *   TypeIDSet requiredSet;
       *   requiredSet.insert(TypeID::P1);
       *   requiredSet.insert(TypeID::P2);
       *
       *   RequireObservables requireTypes(requiredSet);
       *
       *   while(rin >> gRin)
       *   {
       *         // Satellites without P1 and P2 observations will be deleted
       *      gRin >> requireTypes;
       *   }
       * @endcode
       *
       * The "RequireObservables" object will visit every satellite in the
       * GNSS data structure that is "gRin" and will check that the previously
       * given list of observation types is meet by each satellite.
       *
       * Be warned that if a given satellite does not have ALL the observations
       * required, the full satellite record will be summarily deleted from the
       * data structure.
       *
       */
   class RequireObservables : public ProcessingClass
   {
   public:

         /// Default constructor.
      RequireObservables()
      { setIndex(); };


         /** Explicit constructor
          *
          * @param type      TypeID to be required.
          */
      RequireObservables(const TypeID& type)
      { setRequiredType(type); setIndex(); };


         /** Explicit constructor
          *
          * @param typeSet   Set of TypeID's to be required.
          */
      RequireObservables(const TypeIDSet& typeSet)
         : requiredTypeSet(typeSet)
      { setIndex(); };


         /** Returns a satTypeValueMap object, checking the required
          *  observables.
          *
          * @param gData     Data object holding the data.
          */
      virtual satTypeValueMap& Process(satTypeValueMap& gData)
         throw(ProcessingException);


         /** Method to add a TypeID to be required.
          *
          * @param type      Extra TypeID to be required.
          */
      virtual RequireObservables& addRequiredType(const TypeID& type)
      { requiredTypeSet.insert(type); return (*this); };


         /** Method to add a set of TypeID's to be required.
          *
          * @param typeSet    Set of TypeID's to be required.
          */
      virtual RequireObservables& addRequiredType(TypeIDSet& typeSet);


         /** Method to set a TypeID to be required. This method will erase
          *  previous required types.
          *
          * @param type      TypeID to be required.
          */
      virtual RequireObservables& setRequiredType(const TypeID& type)
      { requiredTypeSet.clear(); requiredTypeSet.insert(type); return *this; };


         /** Method to set the TypeID's to be required. This method will erase
          *  previous types.
          *
          * @param typeSet       Set of TypeID's to be required.
          */
      virtual RequireObservables& setRequiredType(const TypeIDSet& typeSet)
      { requiredTypeSet.clear(); requiredTypeSet = typeSet; return (*this); };


         /// Method to get the set of TypeID's to be required.
      virtual TypeIDSet getRequiredType() const
      { return requiredTypeSet; };


         /** Returns a gnnsSatTypeValue object, checking the required
          *  observables.
          *
          * @param gData    Data object holding the data.
          */
      virtual gnssSatTypeValue& Process(gnssSatTypeValue& gData)
         throw(ProcessingException)
      { Process(gData.body); return gData; };



         /** Returns a gnnsRinex object, checking the required observables.
          *
          * @param gData    Data object holding the data.
          */
      virtual gnssRinex& Process(gnssRinex& gData)
         throw(ProcessingException)
      { Process(gData.body); return gData; };


         /// Returns an index identifying this object.
      virtual int getIndex(void) const;


         /// Returns a string identifying this object.
      virtual std::string getClassName(void) const;


         /// Destructor
      virtual ~RequireObservables() {};


   private:


         /// Set of types to be required
      TypeIDSet requiredTypeSet;


         /// Initial index assigned to this class.
      static int classIndex;


         /// Index belonging to this object.
      int index;


         /// Sets the index and increment classIndex.
      void setIndex(void)
      { index = classIndex++; };

   }; // End of class 'RequireObservables'

      //@}

}
#endif  // REQUIREOBSERVABLES_HPP
