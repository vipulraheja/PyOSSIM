#pragma ident "$Id: result.h 1895 2009-05-12 19:34:29Z afarris $"

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


#ifndef _RESULT_
#define _RESULT_

#define _2D	2
#define _3D	3

#include <string.h>
#include "datapoint.h"

class result
{
	public:
	 result();
	 result(result const &copy);
	 ~result();
	 void addResult(double x, double y, double z = 0);
	 void setTitle(char *str);
	 void setDem(int d);
	 dataPoint getResult(int i);
	 char *getTitle();
	 int getLength();
	 int getDem();
	 result& result::operator=(const result& r);

	private:
	 void resize();
	 dataPoint *points;
	 int dlength;
	 int length;
	 int dem;
	 char title[256];
};

#endif
