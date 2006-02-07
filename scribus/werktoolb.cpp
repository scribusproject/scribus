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


WerkToolB::WerkToolB(QMainWindow* parent) : ScToolBar(tr("Tools"), parent, QDockWindow::Vertical)
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
	QImage newShapeIcon = Rechteck->getIconPixmap(0).convertToImage();
	newShapeIcon.smoothScale(16,16);
	ScMW->scrActions["toolsInsertShape"]->setIconSet(QIconSet(newShapeIcon,Rechteck->getIconPixmap(0)));

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

void WerkToolB::GetPolyProps()
{
	PolygonProps* dia = new PolygonProps(this, ScMW->doc->toolSettings.polyC, ScMW->doc->toolSettings.polyFd, ScMW->doc->toolSettings.polyF, ScMW->doc->toolSettings.polyS, ScMW->doc->toolSettings.polyR);
	if (dia->exec())
		dia->getValues(&ScMW->doc->toolSettings.polyC, &ScMW->doc->toolSettings.polyFd, &ScMW->doc->toolSettings.polyF, &ScMW->doc->toolSettings.polyS, &ScMW->doc->toolSettings.polyR);
	delete dia;
}

void WerkToolB::SelShape(int s, int c, double *vals)
{
	const QPixmap* newIcon = Rechteck->find(s)->pixmap();
	QImage newShapeIcon = Rechteck->find(s)->pixmap()->convertToImage();
	newShapeIcon.smoothScale(16,16);
	ScMW->scrActions["toolsInsertShape"]->setIconSet(QIconSet(newShapeIcon, *newIcon));
	insertShapeButtonMenu->hide();
	SubMode = s;
	ValCount = c;
	ShapeVals = vals;
	ScMW->scrActions["toolsInsertShape"]->setOn(true);
}

void WerkToolB::languageChange()
{
	insertPolygonButtonMenu->changeItem(idInsertPolygonButtonMenu, tr("Properties..."));
}


WerkToolBP::WerkToolBP(QMainWindow* parent) : ScToolBar( tr("PDF Tools"), parent)
{
	PDFM = new QPopupMenu();
	PDFTool = new QToolButton(loadIcon("pushbutton.png"), "Insert PDF Fields", QString::null, this, SLOT(ModeFromTB()), this);
	PDFTool->setToggleButton(true);
	PDFTool->setPopup(PDFM);
	PDFTool->setPopupDelay(0);
	PDFwerkz = 0;
	PDFA = new QPopupMenu();
	PDFaTool = new QToolButton(loadIcon("charset.png"), "Insert PDF Annotations", QString::null, this, SLOT(ModeFromTB()), this);
	PDFaTool->setToggleButton(true);
	PDFaTool->setPopup(PDFA);
	PDFaTool->setPopupDelay(0);
	PDFnotiz = 0;

	languageChange();
	connect(PDFM, SIGNAL(activated(int)), this, SLOT(setPDFtool(int)));
	connect(PDFA, SIGNAL(activated(int)), this, SLOT(setPDFnotiz(int)));
}

void WerkToolBP::setPDFnotiz(int id)
{
	int c = PDFA->indexOf(id);
	PDFnotiz = c;
	switch (c)
	{
		case 0:
			PDFaTool->setPixmap(loadIcon("charset.png"));
			break;
		case 1:
			PDFaTool->setPixmap(loadIcon("goto.png"));
			break;
	}
	PDFTool->setOn(false);
	PDFaTool->setOn(true);
	emit NewMode(modeInsertPDFTextAnnotation+PDFnotiz);
}

void WerkToolBP::setPDFtool(int id)
{
	int c = PDFM->indexOf(id);
	PDFwerkz = c;
	QString tmp[] = {"pushbutton.png", "textview.png", "checkbox.png", "combobox.png", "listbox.png"};
	PDFTool->setPixmap(loadIcon(tmp[c]));
	PDFTool->setOn(true);
	PDFaTool->setOn(false);
	emit NewMode(modeInsertPDFButton+PDFwerkz);
}

void WerkToolBP::ModeFromTB()
{
	PDFTool->setOn(false);
	PDFaTool->setOn(false);
	if (PDFTool == sender())
	{
		PDFTool->setOn(true);
		emit NewMode(modeInsertPDFButton+PDFwerkz);
	}
	if (PDFaTool == sender())
	{
		PDFaTool->setOn(true);
		emit NewMode(modeInsertPDFTextAnnotation+PDFnotiz);
	}
}

void WerkToolBP::languageChange()
{
	PDFM->clear();
	QString tmp_icn[] = {"pushbutton.png", "textview.png", "checkbox.png", "combobox.png", "listbox.png"};
	QString tmp_txt[] = { tr("Button"), tr("Text Field"), tr("Check Box"), tr("Combo Box"), tr("List Box")};
	size_t ar_tmp = sizeof(tmp_icn) / sizeof(*tmp_icn);
	for (uint a = 0; a < ar_tmp; ++a)
		PDFM->insertItem(loadIcon(tmp_icn[a]), tmp_txt[a]);
		
	PDFTool->setTextLabel( tr("Insert PDF Fields"));
	PDFaTool->setTextLabel( tr("Insert PDF Annotations"));
	PDFA->clear();
	PDFA->insertItem(loadIcon("charset.png"), tr("Text"));
	PDFA->insertItem(loadIcon("goto.png"), tr("Link"));
}
