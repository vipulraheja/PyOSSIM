/*-----------------------------------------------------------------------------
 * Filename        : ossimImageGeometry.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageGeometry
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossim2dTo2dTransform.h>

#include <ossim/imaging/ossimImageGeometry.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling primitive data types */
%include "typemaps.i"
%typemap(in) ossimjni_int32
{
        $1 = PyInt_AsLong($input);
}
%typemap(out) ossimjni_int32
{
        $result = PyInt_FromLong((long) $1);
}

/* Include the required header files */
%import "ossim/base/ossimConstants.h";

/* Handling ossimImageMetaDataWriterFactory operator */
%rename(__set__) ossimImageGeometry::operator=;
%rename(__eq__) ossimImageGeometry::operator==; 

/* Handling the reserved function print */
%rename(ossimImageGeometry_print) ossimImageGeometry::print;


/* Wrapping the class ossimImageGeometry */
%include "ossim/imaging/ossimImageGeometry.h"

#endif
