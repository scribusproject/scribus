/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
//
//
// Author: Craig Bradney <cbradney@zip.com.au>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "insertaframe.h"
#include "insertaframe.moc"

#include "mspinbox.h"
#include "scribusdoc.h"

#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qwidgetstack.h>

InsertAFrame::InsertAFrame(QWidget* parent, ScribusDoc *doc) :
	InsertAFrameBase(parent),
	m_Doc(doc)
{
	typeButtonGroup->setButton(0);
	pagePlacementButtonGroup->setButton(0);
	framePositionButtonGroup->setButton(0);
	sizeButtonGroup->setButton(0);
	slotSelectType(0);
	slotSelectPagePlacement(0);
	slotSelectPosition(0);
	slotSelectSize(0);
 	connect(typeButtonGroup, SIGNAL(clicked(int)), this, SLOT(slotSelectType(int)));
 	connect(pagePlacementButtonGroup, SIGNAL(clicked(int)), this, SLOT(slotSelectPagePlacement(int)));
 	connect(framePositionButtonGroup, SIGNAL(clicked(int)), this, SLOT(slotSelectPosition(int)));
 	connect(sizeButtonGroup, SIGNAL(clicked(int)), this, SLOT(slotSelectSize(int)));
}

void InsertAFrame::slotSelectType( int id )
{
	switch (id)
	{
		case 0:
			typeTextEdit->setText("Insert a text frame");
			optionsWidgetStack->raiseWidget(0);
			break;
		case 1:
			typeTextEdit->setText("Insert an image frame");
			optionsWidgetStack->raiseWidget(0);
			break;
		case 2:
			typeTextEdit->setText("Insert a table");
			optionsWidgetStack->raiseWidget(1);
			break;
		case 3:
			typeTextEdit->setText("Insert a shape");
			optionsWidgetStack->raiseWidget(1);
			break;
		case 4:
			typeTextEdit->setText("Insert a polygon");
			optionsWidgetStack->raiseWidget(1);
			break;
		case 5:
			typeTextEdit->setText("Insert a line");
			optionsWidgetStack->raiseWidget(1);
			break;
		case 6:
			typeTextEdit->setText("Insert a bezier curve");
			optionsWidgetStack->raiseWidget(1);
			break;
	}
}

void InsertAFrame::slotSelectPagePlacement( int id )
{
	placementPagesLineEdit->setEnabled(id==1);
}

void InsertAFrame::slotSelectPosition( int id )
{
	xPosMSpinBox->setEnabled(id==2);
	yPosMSpinBox->setEnabled(id==2);
}

void InsertAFrame::slotSelectSize( int id )
{
	widthMSpinBox->setEnabled(id==2);
	heightMSpinBox->setEnabled(id==2);
}
