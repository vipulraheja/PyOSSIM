#pragma ident "$Id: ConstellationSet.cpp 1885 2009-05-11 15:07:45Z afarris $"
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
*   @file ConstellationSet.cpp
*  
*  July 2007
*  Applied Research Laboratories, The University of Texas at Austin
*/
   // Language Headers
#include <stdio.h>
#include <string>


   // Project Headers
#include "ConstellationSet.hpp"

using namespace std;
using namespace gpstk;

ConstellationSet::ConstellationSet( ) { }
ConstellationSet::ConstellationSet( const std::string filename, FileType ftype )
{
   loadFile( filename, ftype ); 
}


gpstk::DayTime ConstellationSet::getEarliestDate() const
         throw(NoConstellationFound)
{
   CI ci = cdMap.begin();
   if (ci!=cdMap.end())
   {
      const ConstellationDefinition& cd = ci->second;
      DayTime dt = cd.getDate();
      return( dt );
   }
   ConstellationSet::NoConstellationFound exc("No Constellation Definitions Loaded");
   GPSTK_THROW(exc);   
}

gpstk::DayTime ConstellationSet::getLatestDate() const
         throw(NoConstellationFound)
{
   std::map<gpstk::DayTime,ConstellationDefinition>::const_reverse_iterator cri
                                                              = cdMap.rbegin();
   if (cri!=cdMap.rend()) return( cri->second.getDate() ); 

   ConstellationSet::NoConstellationFound exc("No Constellation Definitions Loaded");
   GPSTK_THROW(exc);   
}
      
int ConstellationSet::loadFile( const std::string filename, FileType ftype )
{
   int count = 0;
   if (ftype==ARLFileType) count = loadFileARL(filename);
   if (ftype==OpAdvisory) count = loadFileOpAdvisory(filename);
   if (ftype==CSV) count = loadFileCSV(filename);
      // Other (future) file types go here.
   return(count);
}

void ConstellationSet::clearSet( )
{
   cdMap.clear();
}

void ConstellationSet::dump( FILE * logfp) const
{
   for (CI ci=cdMap.begin();ci!=cdMap.end();++ci)
   {
      const ConstellationDefinition& cd = ci->second;
      cd.dump(logfp);
   }
}

ConstellationDefinition ConstellationSet::findCD( const gpstk::DayTime dt ) const
         throw(NoConstellationFound)
{
   DayTime localDT = dt;
   localDT.setSecOfDay(  (DayTime::SEC_DAY/2) );   // Set to noon to match CDs
   
      // Best case (hopefully nominal) is that there is a definition
      // available for the date in question.
   CI ci = cdMap.find(localDT);
   //cout << "Found in map: " << ci->first << endl;
   if (ci==cdMap.end())
   {
         // If no, first check to see whether date in question is later
         // than any date available.
      if (localDT>getLatestDate())
      {
         ConstellationSet::NoConstellationFound exc("All Constellation Definitions Too Early");
         GPSTK_THROW(exc);
      }
      
         // If not, start at the day of interest and back up until we
         // find a definition or hit the earliest definition
      bool done = false;
      localDT -= DayTime::SEC_DAY;
      while (!done && localDT > getEarliestDate() )
      {
         ci = cdMap.find(localDT);
         if (ci!=cdMap.end()) done = true;
         localDT -= DayTime::SEC_DAY;
      }
         // Did not find one before reaching the "head" of the list
      if (!done)
      {
         ConstellationSet::NoConstellationFound exc("All Constellation Definitions Too Late");
         GPSTK_THROW(exc);   
      }
   }

   ConstellationDefinition cd = ci->second;
   return(cd);
}

int ConstellationSet::loadFileOpAdvisory( const std::string filename )
  throw(InvalidDateString, MismatchSize)
{
   FILE* inf = fopen(filename.c_str(), "rt");
   if (inf==NULL) return (1);
   int numDefFound = 0;
   char fileLine[101];
   gpstk::DayTime date;
   list<string> PRNs;
   list<string> SLOTs;
   ConstellationDefinition cd;

   while (fgets(fileLine, 100, inf))
   {
      string currentLine = fileLine;
      if (currentLine.find("SUBJ: GPS STATUS") != string::npos)
      {
         if(cd.getDate() != DayTime::BEGINNING_OF_TIME)
         {
            //cout << date.asString() << endl;
            cdMap.insert( make_pair( date, cd ) );
            numDefFound++;
            cd.clearDefinition();
            PRNs.clear();
            SLOTs.clear();
         }
         date = parseDate(currentLine);
         cd.setEffectiveTime(date);
      }
      if (currentLine.find("PRNS") != string::npos)
      {
         PRNs = parseNums(currentLine, "PRNS");
      }
      if (currentLine.find("SLOT") != string::npos)
      {
         SLOTs = parseNums(currentLine, "SLOT");
         if (SLOTs.size()!=PRNs.size())
         {
            ConstellationSet::MismatchSize exc("PRNs and Slots disagree.");
            GPSTK_THROW(exc);
         }
         list<string>::const_iterator prnI;
         list<string>::const_iterator slotI;
         slotI = SLOTs.begin();
         for (prnI = PRNs.begin(); prnI!=PRNs.end(); prnI++)
         {
            int prnInt = StringUtils::asInt(*prnI);
            SatID SV(prnInt, SatID::systemGPS);
            string slotString = *slotI;
            int slotNum = StringUtils::asInt(slotString.substr(1,1));
            cd.setPlaneSlot(SV, slotString[0], slotNum);
            slotI++;
         }
      }
   }
   if(cd.getDate() != DayTime::BEGINNING_OF_TIME)
   {
      //cout << date.asString() << endl;
      cdMap.insert( make_pair( date, cd ) );
      numDefFound++;
   }
   //cout << numDefFound << endl;
   return(numDefFound);
}

gpstk::DayTime ConstellationSet::parseDate(string date)
  throw(InvalidDateString)
{
   string whitespace = " \t\r\n";
   string::size_type end  = date.find_last_not_of(whitespace);
   string::size_type front = date.find("STATUS");
   front = date.find_first_not_of(whitespace, front+6);
   string dateString = date.substr(front, end-front+1);
   DayTime dt;
   try
   {
      //cout << dateString << endl;
      dt.setToString(dateString, "%d %b %Y");
      dt.setSecOfDay(  (DayTime::SEC_DAY/2) );
      return dt;
   }
   catch(DayTime::DayTimeException exc)
   {
      string s = "Invalid date: '" + dateString + "'";
      //cout << s << endl;
      ConstellationSet::InvalidDateString excids(s);
      GPSTK_THROW(excids);
   }
}

list<string> ConstellationSet::parseNums(string data, string searching)
{
   string vals = "ABCDEF0123456789";
   list<string> nums;

   string::size_type frontIndex = data.find(searching);
   frontIndex = data.find_first_of(vals, frontIndex+4);
   while (frontIndex!=string::npos)
   {
      string::size_type endIndex = data.find_first_not_of(vals, frontIndex);
      string numstring = data.substr(frontIndex, endIndex-frontIndex);
      nums.push_back(numstring);
      frontIndex = data.find_first_of(vals, endIndex+1);
   }
   return nums;
}

int ConstellationSet::loadFileARL( const std::string filename )
{
   FILE* inf = fopen(filename.c_str(),"rt");
   if (inf==0) return(0);
   
   std::string temp;
   DayTime tempDT;
   string whiteSpace = " \t\n\r";
   char comma = ',';
   
   bool inDefinition = false;
   int lineCount =0;
   int cdCount = 0;
   char fileLine[200];
   ConstellationDefinition cd;
   
   while (fgets(fileLine, 200, inf))
   {
      lineCount++;
      string lineIn = fileLine;
         
         // strip trailing whitespace
      string::size_type endIndex = lineIn.find_last_not_of(whiteSpace);
      lineIn = lineIn.substr( 0, endIndex+1 );
      
      string lead4Chars = lineIn.substr(0,4);
 
         // Start new CD definition
      if (lead4Chars.compare("DATE")==0)
      {
            // Store any CD currently "in progress" before starting new.
         if (inDefinition) 
         {
            cdMap.insert( make_pair( tempDT, cd ) );
            cdCount++;
            cd.clearDefinition();
            inDefinition = false;
         }
         
            // Attempt to crack Date and form an effective date for this CD
         string::size_type start = lineIn.find_first_not_of(whiteSpace,4);
         int y,m,d;
         string t = lineIn.substr(start);
         int ntest = sscanf(t.c_str(),"%4d%2d%2d", &y, &m, &d);
         if (start!=string::npos && ntest==3) 
         {
            try
            {
               tempDT.setYMDHMS( (short) y, (short)m, (short)d, 12,0,0.0);
               cd.setEffectiveTime( tempDT );
               inDefinition = true;
            }
            catch(DayTime::DayTimeException e)
            {
               // do nothing - ACTUALLY the fact that we did NOT
               // set inDefinition is the action.
            }
         }
      }
         // Add to current CD definition
      if (lead4Chars.compare("PLAN")==0 && inDefinition)
      {
            // List should be comma separated value (CSV) list
            // First find plane, then parse list of PRNs 
         string::size_type end = lineIn.find(comma); // Comma after "PLANE"
         end = lineIn.find(comma, end+1);            // Comma after plane character
         char plane = lineIn[end-1];
         string::size_type start = end+1;
         bool done = false;
         int slot = 1;
         while (!done)
         {
            end = lineIn.find(comma,start);
            bool empty = true;
            string::size_type check = lineIn.find_first_not_of(whiteSpace,start);
            if (check<end) empty = false;
            int PRNID = 0;
            if (!empty)
            {
	      try
	      {               
		PRNID = StringUtils::asInt(lineIn.substr(start,(end-start)));
	      }
	      catch(StringUtils::StringException e)
	      {
		
	      }
               SatID SV( PRNID, SatID::systemGPS );
               cd.setPlaneSlot( SV, plane, slot );
            }
               // If there's a null entry within the first four slots, 
               // we simply want to move to the next item.
               // If the null entry is after the fourth slot, then
               // it's time to stop looking. 
            else
            {
               if (slot>4) done = true;
            }
            slot++;
            start = end +1;
         }
      }
   }
   if (inDefinition)
   {
      cdMap.insert( make_pair( tempDT, cd ) );
      cdCount++;
   }
   fclose(inf);
   return(cdCount);
}

int ConstellationSet::loadFileCSV( const std::string filename )
{
   FILE* inf = fopen(filename.c_str(),"rt");
   if (inf==0) return(0);
   
   std::string temp;
   DayTime tempDT;
   DayTime testDT = DayTime(2008,1,1,12,0,0.0);
   string whiteSpace = " \t\n\r";
   char comma = ',';
   
   int lineCount =0;
   int cdCount = 0;
   char fileLine[301];
   ConstellationDefinition cd;

   while (fgets(fileLine, 300, inf))
   {
      // Set a clear starting state
      cd.clearDefinition();
      
      lineCount++;
         // Skip first two lines
      if (lineCount<4) continue;

         // Move input from character buffer to string.
      string lineIn = fileLine;

         // strip trailing whitespace
      string::size_type endIndex = lineIn.find_last_not_of(whiteSpace);
      lineIn = lineIn.substr( 0, endIndex+1 );

         // First entry should be date string
      string dateString = lineIn.substr(0,lineIn.find(comma));
      int y,m,d;
      int ntest = sscanf(dateString.c_str(),"%4d-%2d-%2d", &y, &m, &d);
      if (ntest==3) 
      {
         try
         {
            tempDT.setYMDHMS( (short) y, (short)m, (short)d, 12,0,0.0);
            cd.setEffectiveTime( tempDT );
         }
         catch(DayTime::DayTimeException e)
         {
            // Did not successful parse a date, so 
            // reject the line.
            continue;
         }
      }
      //if (tempDT==testDT)
      //cout << "Parsing date: " << tempDT.printf("%02m/%02d/%04Y") 
      //     << "size(): " << lineIn.size() << "." << endl;
      
         // Next 32 entries are PRN->NAVSTAR number entries followed
         // by a blank column.
         // Crack out the NAVSTAR numbers and build the PRN->NAVSTAR number
         // table.
      int numItems = 0;
      int PRNID;
      string::size_type nextPos = 0;
      string::size_type cpos = lineIn.find(comma);
      //cout << "Entering the read loop" << endl;
      while (numItems<33)
      {
         PRNID = numItems + 1;
         nextPos = lineIn.find(comma,cpos+1);
         
         if (PRNID<=gpstk::MAX_PRN)
         {
            string SVNString = lineIn.substr(cpos+1,nextPos-cpos-1);
            if (SVNString.size()>0)
            {
      	      try
	            {      
                  int SVNNum = StringUtils::asInt(SVNString);
                  SatID SV( PRNID, SatID::systemGPS );
                  cd.setSVNforPRN( SV, SVNNum );
                  
                     // DEBUG
                  //if (tempDT==testDT)
                  //cout << "PRNID, SVN, cpos, nextPos: " << PRNID << ", "
                  //      << SVNNum << ", " << cpos << ", " << nextPos << "." << endl;
	            }
               // If the conversion fails, just move on.....
   	         catch(StringUtils::StringException e)
	            {
	            }
            }
         }
         numItems++;
         cpos = nextPos;
      }
      //cout << "Exiting the read loop" << endl;
      
         // Next 32 entries contain the by-PRN slot assignments
      PRNID = 1;
      while (PRNID<=gpstk::MAX_PRN && cpos<lineIn.size() )
      {
         nextPos = lineIn.find(comma,cpos+1);
         if (nextPos==string::npos) nextPos = lineIn.size();
         string slotString = lineIn.substr(cpos+1,nextPos-cpos-1);
         
         //if (tempDT==testDT)
         //cout << "PRN, slotString, cpos,nextPos: " << PRNID << ", '"
         //     << slotString << "', "
         //      << cpos << ", " << nextPos << "." << endl;
         
         
            // Zero length means this PRN isn't available
         if (slotString.size()>0)
         {
            char plane;
            int slot;
   	      try
	         {      
               plane = slotString[0];
               slot = StringUtils::asInt(slotString.substr(1,1));
               SatID SV( PRNID, SatID::systemGPS );
               cd.setPlaneSlot( SV, plane, slot );
               //if (tempDT==testDT)
               //   cout << " plane, slot: " << plane << ", " << slot << ". " << endl;
	         }
               // If the conversion fails, just move on.....
	         catch(StringUtils::StringException e)
	         {
	         }
         }
         PRNID++;
         cpos = nextPos;
      }
      cdMap.insert( make_pair( tempDT, cd ) );
      cdCount++;
   }      
   fclose(inf);
   return(cdCount);
}


