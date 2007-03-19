/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          texttoolb.cpp  -  description
                             -------------------
    begin                : Sun Mar 10 2002
    copyright            : (C) 2002 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qtooltip.h>
#include <q3popupmenu.h>
#include <qpixmap.h>

#include "werktoolb.h"
//#include "werktoolb.moc"

#include "autoformbuttongroup.h"
#include "menumanager.h"
#include "polyprops.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"

extern QPixmap loadIcon(QString nam);


ModeToolBar::ModeToolBar(ScribusMainWindow* parent) : ScToolBar( tr("Tools"), "Tools", parent, Qt::Vertical)
{
	SubMode = 0;
	ValCount = 32;
	static double AutoShapes0[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	ShapeVals = AutoShapes0;
	m_ScMW=parent;
	m_ScMW->scrActions["toolsSelect"]->addTo(this);
	m_ScMW->scrActions["toolsInsertTextFrame"]->addTo(this);
	m_ScMW->scrActions["toolsInsertImageFrame"]->addTo(this);
	m_ScMW->scrActions["toolsInsertTableFrame"]->addTo(this);
	
	m_ScMW->scrActions["toolsInsertShape"]->addTo(this);
	m_ScMW->scrMenuMgr->createMenu("insertShapeButtonMenu", "insertShapeButtonMenu");
	insertShapeButtonMenu=m_ScMW->scrMenuMgr->getLocalPopupMenu("insertShapeButtonMenu");
	m_ScMW->scrMenuMgr->addMenuToWidgetOfAction("insertShapeButtonMenu", m_ScMW->scrActions["toolsInsertShape"]);
	
	QToolButton *insertShapeButton = dynamic_cast<QToolButton*>(m_ScMW->scrActions["toolsInsertShape"]->getWidgetAddedTo());
	if (insertShapeButton)
		insertShapeButton->setPopupDelay(0);
	Rechteck = new AutoformButtonGroup( NULL );
	//Qt4 insertShapeButtonMenu->insertItem( Rechteck );
	//QImage newShapeIcon = Rechteck->getIconPixmap(0).convertToImage();
	//newShapeIcon.smoothScale(16,16);
	m_ScMW->scrActions["toolsInsertShape"]->setIcon(QIcon(Rechteck->getIconPixmap(0,16)));
//qt4 fixme, Rechteck->getIconPixmap(0)));

	m_ScMW->scrActions["toolsInsertPolygon"]->addTo(this);
	m_ScMW->scrMenuMgr->createMenu("insertPolygonButtonMenu", "insertPolygonButtonMenu");
	insertPolygonButtonMenu=m_ScMW->scrMenuMgr->getLocalPopupMenu("insertPolygonButtonMenu");
	m_ScMW->scrMenuMgr->addMenuToWidgetOfAction("insertPolygonButtonMenu", m_ScMW->scrActions["toolsInsertPolygon"]);
	idInsertPolygonButtonMenu=insertPolygonButtonMenu->insertItem( "Properties...", this, SLOT(GetPolyProps()));
	
	QToolButton *insertPolygonButton = dynamic_cast<QToolButton*>(m_ScMW->scrActions["toolsInsertPolygon"]->getWidgetAddedTo());
	if (insertPolygonButton)
		insertPolygonButton->setPopupDelay(0);
	
	m_ScMW->scrActions["toolsInsertLine"]->addTo(this);
	m_ScMW->scrActions["toolsInsertBezier"]->addTo(this);
	m_ScMW->scrActions["toolsInsertFreehandLine"]->addTo(this);
	m_ScMW->scrActions["toolsRotate"]->addTo(this);
	m_ScMW->scrActions["toolsZoom"]->addTo(this);
	m_ScMW->scrActions["toolsEditContents"]->addTo(this);
	m_ScMW->scrActions["toolsEditWithStoryEditor"]->addTo(this);
	m_ScMW->scrActions["toolsLinkTextFrame"]->addTo(this);
	m_ScMW->scrActions["toolsUnlinkTextFrame"]->addTo(this);
	m_ScMW->scrActions["toolsMeasurements"]->addTo(this);
	m_ScMW->scrActions["toolsCopyProperties"]->addTo(this);
	m_ScMW->scrActions["toolsEyeDropper"]->addTo(this);

	languageChange();
	connect(Rechteck, SIGNAL(FormSel(int, int, double *)), this, SLOT(SelShape(int, int, double *)));
}

void ModeToolBar::GetPolyProps()
{
	PolygonProps* dia = new PolygonProps(this, m_ScMW->doc->toolSettings.polyC, m_ScMW->doc->toolSettings.polyFd, m_ScMW->doc->toolSettings.polyF, m_ScMW->doc->toolSettings.polyS, m_ScMW->doc->toolSettings.polyR);
	if (dia->exec())
		dia->getValues(&m_ScMW->doc->toolSettings.polyC, &m_ScMW->doc->toolSettings.polyFd, &m_ScMW->doc->toolSettings.polyF, &m_ScMW->doc->toolSettings.polyS, &m_ScMW->doc->toolSettings.polyR);
	delete dia;
}

void ModeToolBar::SelShape(int s, int c, double *vals)
{
	//const QPixmap* newIcon = Rechteck->find(s)->pixmap();
	//QImage newShapeIcon = Rechteck->find(s)->pixmap()->convertToImage();
	//newShapeIcon.smoothScale(16,16);
	//m_ScMW->scrActions["toolsInsertShape"]->setIconSet(QIconSet(newShapeIcon, *newIcon));
	m_ScMW->scrActions["toolsInsertShape"]->setIcon(QIcon(Rechteck->getIconPixmap(s,16)));
//qt4 fixme ,Rechteck->getIconPixmap(s)));
	insertShapeButtonMenu->hide();
	SubMode = s;
	ValCount = c;
	ShapeVals = vals;
	m_ScMW->scrActions["toolsInsertShape"]->setOn(false);
	m_ScMW->scrActions["toolsInsertShape"]->setOn(true);
}

void ModeToolBar::languageChange()
{
	insertPolygonButtonMenu->changeItem(idInsertPolygonButtonMenu, tr("Properties..."));
	ScToolBar::languageChange();
}


PDFToolBar::PDFToolBar(ScribusMainWindow* parent) : ScToolBar( tr("PDF Tools"), "PDF_Tools", parent)
{
	parent->scrActions["toolsPDFPushButton"]->addTo(this);
	parent->scrActions["toolsPDFTextField"]->addTo(this);
	parent->scrActions["toolsPDFCheckBox"]->addTo(this);
	parent->scrActions["toolsPDFComboBox"]->addTo(this);
	parent->scrActions["toolsPDFListBox"]->addTo(this);
	parent->scrActions["toolsPDFAnnotText"]->addTo(this);
	parent->scrActions["toolsPDFAnnotLink"]->addTo(this);
}
