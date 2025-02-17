/*-----------------------------------------------------------------------------
 * Filename        : ossimGeoidEgm96.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimGeoid.h>
#include <ossim/base/ossimGeoidEgm96.h>
#include <vector>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimGeoidEgm96.h"

#endif
