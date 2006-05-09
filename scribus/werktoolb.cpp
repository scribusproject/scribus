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
#include <qpopupmenu.h>
#include <qpixmap.h>

#include "werktoolb.h"
#include "werktoolb.moc"

#include "autoformbuttongroup.h"
#include "menumanager.h"
#include "polyprops.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"

extern QPixmap loadIcon(QString nam);


ModeToolBar::ModeToolBar(QMainWindow* parent) : ScToolBar(tr("Tools"), "Tools", parent, QDockWindow::Vertical)
{
	SubMode = 0;
	ValCount = 32;
	static double AutoShapes0[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	ShapeVals = AutoShapes0;

	ScMW->scrActions["toolsSelect"]->addTo(this);
	ScMW->scrActions["toolsInsertTextFrame"]->addTo(this);
	ScMW->scrActions["toolsInsertImageFrame"]->addTo(this);
	ScMW->scrActions["toolsInsertTableFrame"]->addTo(this);
	
	ScMW->scrActions["toolsInsertShape"]->addTo(this);
	ScMW->scrMenuMgr->createMenu("insertShapeButtonMenu", "insertShapeButtonMenu");
	insertShapeButtonMenu=ScMW->scrMenuMgr->getLocalPopupMenu("insertShapeButtonMenu");
	ScMW->scrMenuMgr->addMenuToWidgetOfAction("insertShapeButtonMenu", ScMW->scrActions["toolsInsertShape"]);
	
	QToolButton *insertShapeButton = dynamic_cast<QToolButton*>(ScMW->scrActions["toolsInsertShape"]->getWidgetAddedTo());
	if (insertShapeButton)
		insertShapeButton->setPopupDelay(0);
	Rechteck = new AutoformButtonGroup( NULL );
	insertShapeButtonMenu->insertItem( Rechteck );
	//QImage newShapeIcon = Rechteck->getIconPixmap(0).convertToImage();
	//newShapeIcon.smoothScale(16,16);
	ScMW->scrActions["toolsInsertShape"]->setIconSet(QIconSet(Rechteck->getIconPixmap(0,16),Rechteck->getIconPixmap(0)));

	ScMW->scrActions["toolsInsertPolygon"]->addTo(this);
	ScMW->scrMenuMgr->createMenu("insertPolygonButtonMenu", "insertPolygonButtonMenu");
	insertPolygonButtonMenu=ScMW->scrMenuMgr->getLocalPopupMenu("insertPolygonButtonMenu");
	ScMW->scrMenuMgr->addMenuToWidgetOfAction("insertPolygonButtonMenu", ScMW->scrActions["toolsInsertPolygon"]);
	idInsertPolygonButtonMenu=insertPolygonButtonMenu->insertItem( "Properties...", this, SLOT(GetPolyProps()));
	
	QToolButton *insertPolygonButton = dynamic_cast<QToolButton*>(ScMW->scrActions["toolsInsertPolygon"]->getWidgetAddedTo());
	if (insertPolygonButton)
		insertPolygonButton->setPopupDelay(0);
	
	ScMW->scrActions["toolsInsertLine"]->addTo(this);
	ScMW->scrActions["toolsInsertBezier"]->addTo(this);
	ScMW->scrActions["toolsInsertFreehandLine"]->addTo(this);
	ScMW->scrActions["toolsRotate"]->addTo(this);
	ScMW->scrActions["toolsZoom"]->addTo(this);
	ScMW->scrActions["toolsEditContents"]->addTo(this);
	ScMW->scrActions["toolsEditWithStoryEditor"]->addTo(this);
	ScMW->scrActions["toolsLinkTextFrame"]->addTo(this);
	ScMW->scrActions["toolsUnlinkTextFrame"]->addTo(this);
	ScMW->scrActions["toolsMeasurements"]->addTo(this);
	ScMW->scrActions["toolsCopyProperties"]->addTo(this);
	ScMW->scrActions["toolsEyeDropper"]->addTo(this);

	languageChange();
	connect(Rechteck, SIGNAL(FormSel(int, int, double *)), this, SLOT(SelShape(int, int, double *)));
}

void ModeToolBar::GetPolyProps()
{
	PolygonProps* dia = new PolygonProps(this, ScMW->doc->toolSettings.polyC, ScMW->doc->toolSettings.polyFd, ScMW->doc->toolSettings.polyF, ScMW->doc->toolSettings.polyS, ScMW->doc->toolSettings.polyR);
	if (dia->exec())
		dia->getValues(&ScMW->doc->toolSettings.polyC, &ScMW->doc->toolSettings.polyFd, &ScMW->doc->toolSettings.polyF, &ScMW->doc->toolSettings.polyS, &ScMW->doc->toolSettings.polyR);
	delete dia;
}

void ModeToolBar::SelShape(int s, int c, double *vals)
{
	//const QPixmap* newIcon = Rechteck->find(s)->pixmap();
	//QImage newShapeIcon = Rechteck->find(s)->pixmap()->convertToImage();
	//newShapeIcon.smoothScale(16,16);
	//ScMW->scrActions["toolsInsertShape"]->setIconSet(QIconSet(newShapeIcon, *newIcon));
	ScMW->scrActions["toolsInsertShape"]->setIconSet(QIconSet(Rechteck->getIconPixmap(s,16),Rechteck->getIconPixmap(s)));
	insertShapeButtonMenu->hide();
	SubMode = s;
	ValCount = c;
	ShapeVals = vals;
	ScMW->scrActions["toolsInsertShape"]->setOn(true);
}

void ModeToolBar::languageChange()
{
	insertPolygonButtonMenu->changeItem(idInsertPolygonButtonMenu, tr("Properties..."));
	ScToolBar::languageChange();
}


PDFToolBar::PDFToolBar(QMainWindow* parent) : ScToolBar( tr("PDF Tools"), "PDF_Tools", parent)
{
	ScMW->scrActions["toolsPDFPushButton"]->addTo(this);
	ScMW->scrActions["toolsPDFTextField"]->addTo(this);
	ScMW->scrActions["toolsPDFCheckBox"]->addTo(this);
	ScMW->scrActions["toolsPDFComboBox"]->addTo(this);
	ScMW->scrActions["toolsPDFListBox"]->addTo(this);
	ScMW->scrActions["toolsPDFAnnotText"]->addTo(this);
	ScMW->scrActions["toolsPDFAnnotLink"]->addTo(this);
}
