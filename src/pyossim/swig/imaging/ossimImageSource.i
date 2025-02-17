/*-----------------------------------------------------------------------------
Filename        : ossimImageSource.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimImageSource 
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimRefPtr.h>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Handling ossimImageSource Assignment operator */
%rename(__set__) ossimImageSource::operator=;

/* Include the header file containing the constants */
%import "ossim/base/ossimConstants.h"

/* Wrapping the ossimImageSource class */
%include "ossim/imaging/ossimImageSource.h"

#endif
