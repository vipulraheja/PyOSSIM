#pragma ident "$Id: datapoint.h 1895 2009-05-12 19:34:29Z afarris $"

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
//  Copyright 2009, The University of Texas at Austin
//
//============================================================================

#ifndef _DATAPOINT_
#define _DATAPOINT_

class dataPoint
{
	public:
	 dataPoint();
	 dataPoint(double x, double y, double z = 0);
	 void setX(double x);
	 void setY(double y);
	 void setZ(double z);
	 double getX();
	 double getY();
	 double getZ();
	 
	private:
	 double X, Y, Z;
	 bool nullPoint;
};

#endif
