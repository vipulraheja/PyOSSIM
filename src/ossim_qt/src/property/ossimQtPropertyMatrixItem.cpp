//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimQtPropertyMatrixItem.cpp 9114 2006-06-14 17:42:07Z gpotts $
#include <qhbox.h>
#include <qvariant.h>
#include "ossimQtPropertyListView.h"
#include "ossimQtPropertyMatrixItem.h"
#include <ossim/base/ossimMatrixProperty.h>

#include "ossimQtMatrixPropertyDialog.h"
#include "ossimQtMatrixPropertyController.h"

ossimQtPropertyMatrixItem::ossimQtPropertyMatrixItem(ossimQtPropertyListView *propList,
                                                     ossimQtPropertyItem *after,
                                                     ossimQtPropertyItem *parent,
                                                     ossimRefPtr<ossimProperty> oProp)
   :ossimQtPropertyItem(propList, after, parent, oProp),
    theBox(0),
    theButton(0),
    theMatrixDialog(0)
{
    theBox = new QHBox( theListView->viewport() );
    theBox->hide();
    theBox->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    theBox->setLineWidth( 2 );
    theButton = new QPushButton( "...", theBox );
    theButton->setFixedWidth( 20 );

    connect( theButton, SIGNAL( clicked() ),
             this, SLOT( getMatrix() ) );
}

ossimQtPropertyMatrixItem::~ossimQtPropertyMatrixItem()
{
   if(theBox)
   {
      delete theBox;
      theBox = 0;
   }
   if(theMatrixDialog)
   {
      delete theMatrixDialog;
      theMatrixDialog = 0;
   }
}

void ossimQtPropertyMatrixItem::showEditor()
{
   ossimQtPropertyItem::showEditor();
   placeEditor( theBox );
   
   if ( !theBox->isVisible() )
   {
      theBox->show();
   }
}

void ossimQtPropertyMatrixItem::hideEditor()
{
   ossimQtPropertyItem::hideEditor();
   theBox->hide();
}

void ossimQtPropertyMatrixItem::resetProperty(bool notify)
{
   ossimQtPropertyItem::resetProperty(notify);

   if(theMatrixDialog)
   {
      theMatrixDialog->controller()->setOssimProperty(PTR_CAST(ossimMatrixProperty,
                                                               getOssimProperty().get()));
   }
}

void ossimQtPropertyMatrixItem::getMatrix()
{
   if(!theMatrixDialog)
   {
      theMatrixDialog = new ossimQtMatrixPropertyDialog(theListView->viewport(),
                                                        "Matrix dialog",
                                                        false,
                                                        Qt::WDestructiveClose);
      connect(theMatrixDialog->controller(),
              SIGNAL(apply(ossimRefPtr<ossimMatrixProperty>)),
              this, SLOT(matrixChanged(ossimRefPtr<ossimMatrixProperty>)));
      connect(theMatrixDialog->controller(),
              SIGNAL(changed(ossimRefPtr<ossimMatrixProperty>)),
              this, SLOT(matrixChanged(ossimRefPtr<ossimMatrixProperty>)));
   }
   theMatrixDialog->controller()->setOssimProperty(PTR_CAST(ossimMatrixProperty,
                                                            getOssimProperty().get()));

   theMatrixDialog->show();
}

void ossimQtPropertyMatrixItem::matrixChanged(ossimRefPtr<ossimMatrixProperty> prop)
{
   if(getOssimProperty().valid())
   {
      if(getOssimProperty()->valueToString() != ((ossimProperty*)prop.get())->valueToString())
      {
         getOssimProperty()->assign(*prop);

         notifyValueChange();
      }
   }
}

