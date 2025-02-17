/*-----------------------------------------------------------------------------
 * Filename        : ossimFontProperty.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimLsrRay
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimFontInformation.h>
#include <ossim/base/ossimFontProperty.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimFontProperty.h"

#endif
