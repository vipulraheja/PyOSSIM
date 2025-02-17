/*-----------------------------------------------------------------------------
 * Filename        : ossimCustomEditorWindowFactoryBase.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimCustomEditorWindowFactoryBase.h>
%}        

#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/* Handling class */
%include "ossim/base/ossimCustomEditorWindowFactoryBase.h"

#endif
