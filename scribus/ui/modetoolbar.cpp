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

#include <QToolTip>
#include <QEvent>
#include <QMenu>
#include <QPixmap>

#include "modetoolbar.h"

#include "autoformbuttongroup.h"
#include "menumanager.h"
#include "polyprops.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "util.h"

ModeToolBar::ModeToolBar(ScribusMainWindow* parent) : ScToolBar( tr("Tools"), "Tools", parent, Qt::Vertical)
{
	SubMode = 0;
	ValCount = 32;
	static double AutoShapes0[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	ShapeVals = AutoShapes0;
	m_ScMW=parent;
	this->addAction(m_ScMW->scrActions["toolsSelect"]);
	this->addAction(m_ScMW->scrActions["toolsInsertTextFrame"]);
	this->addAction(m_ScMW->scrActions["toolsInsertImageFrame"]);
	this->addAction(m_ScMW->scrActions["toolsInsertRenderFrame"]);
	this->addAction(m_ScMW->scrActions["toolsInsertTableFrame"]);
	
	this->addAction(m_ScMW->scrActions["toolsInsertShape"]);
//	insertShapeButtonMenu = new QMenu();
	Rechteck = new AutoformButtonGroup( NULL );
//	insertShapeButtonAct = new QWidgetAction( this );
//	insertShapeButtonAct->setDefaultWidget(Rechteck);
//	insertShapeButtonMenu->addAction(insertShapeButtonAct);
//	m_ScMW->scrActions["toolsInsertShape"]->setMenu(insertShapeButtonMenu);
	m_ScMW->scrActions["toolsInsertShape"]->setMenu(Rechteck);
	QToolButton* tb = dynamic_cast<QToolButton*>(this->widgetForAction(m_ScMW->scrActions["toolsInsertShape"]));
	tb->setPopupMode(QToolButton::MenuButtonPopup);
	m_ScMW->scrActions["toolsInsertShape"]->setIcon(QIcon(Rechteck->getIconPixmap(0,16)));

	this->addAction(m_ScMW->scrActions["toolsInsertPolygon"]);
	insertPolygonButtonMenu = new QMenu();
	idInsertPolygonButtonMenu = insertPolygonButtonMenu->addAction( "Properties...", this, SLOT(GetPolyProps()));
	m_ScMW->scrActions["toolsInsertPolygon"]->setMenu(insertPolygonButtonMenu);
	QToolButton* tb2 = dynamic_cast<QToolButton*>(this->widgetForAction(m_ScMW->scrActions["toolsInsertPolygon"]));
	tb2->setPopupMode(QToolButton::MenuButtonPopup);
		
	this->addAction(m_ScMW->scrActions["toolsInsertLine"]);
	this->addAction(m_ScMW->scrActions["toolsInsertBezier"]);
	this->addAction(m_ScMW->scrActions["toolsInsertFreehandLine"]);
	this->addAction(m_ScMW->scrActions["toolsRotate"]);
	this->addAction(m_ScMW->scrActions["toolsZoom"]);
	this->addAction(m_ScMW->scrActions["toolsEditContents"]);
	this->addAction(m_ScMW->scrActions["toolsEditWithStoryEditor"]);
	this->addAction(m_ScMW->scrActions["toolsLinkTextFrame"]);
	this->addAction(m_ScMW->scrActions["toolsUnlinkTextFrame"]);
	this->addAction(m_ScMW->scrActions["toolsMeasurements"]);
	this->addAction(m_ScMW->scrActions["toolsCopyProperties"]);
	this->addAction(m_ScMW->scrActions["toolsEyeDropper"]);

	languageChange();
	connect(Rechteck, SIGNAL(FormSel(int, int, double *)), this, SLOT(SelShape(int, int, double *)));
}

void ModeToolBar::GetPolyProps()
{
	PolygonProps* dia = new PolygonProps(this, m_ScMW->doc->toolSettings.polyC, m_ScMW->doc->toolSettings.polyFd, m_ScMW->doc->toolSettings.polyF, m_ScMW->doc->toolSettings.polyS, m_ScMW->doc->toolSettings.polyR, m_ScMW->doc->toolSettings.polyCurvature);
	if (dia->exec())
		dia->getValues(&m_ScMW->doc->toolSettings.polyC, &m_ScMW->doc->toolSettings.polyFd, &m_ScMW->doc->toolSettings.polyF, &m_ScMW->doc->toolSettings.polyS, &m_ScMW->doc->toolSettings.polyR, &m_ScMW->doc->toolSettings.polyCurvature);
	delete dia;
}

void ModeToolBar::SelShape(int s, int c, double *vals)
{
	m_ScMW->scrActions["toolsInsertShape"]->setIcon(QIcon(Rechteck->getIconPixmap(s,16)));
//	insertShapeButtonMenu->hide();
	SubMode = s;
	ValCount = c;
	ShapeVals = vals;
	m_ScMW->scrActions["toolsInsertShape"]->setChecked(false);
	m_ScMW->scrActions["toolsInsertShape"]->setChecked(true);
}

void ModeToolBar::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void ModeToolBar::languageChange()
{
	idInsertPolygonButtonMenu->setText( tr("Properties..."));
	ScToolBar::languageChange();
}
