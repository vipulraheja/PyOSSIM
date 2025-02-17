/*-----------------------------------------------------------------------------
 * Filename        : ossimImageViewProjectionTransform.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageViewProjectionTransform
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimImageViewProjectionTransform.h>
%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling the reserved function print */
%rename(ossimImageViewProjectionTransform_print) ossimImageViewProjectionTransform::print;

%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimImageViewProjectionTransform */
%include "ossim/projection/ossimImageViewProjectionTransform.h"

#endif
