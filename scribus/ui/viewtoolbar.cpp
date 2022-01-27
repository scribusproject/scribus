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

#include "viewtoolbar.h"

#include <QFont>

#include "scraction.h"

#include "scribusdoc.h"
#include "scribuscore.h"

ViewToolBar::ViewToolBar(ScribusMainWindow* parent) : ScToolBar( tr("View Tools"), "View_Tools", parent)
{
	QFont fo = QFont(font());
	int posi = fo.pointSize() - (ScCore->isWinGUI() ? 1 : 2);
	fo.setPointSize(posi);

	previewQualitySwitcher = new QComboBox( this );
	previewQualitySwitcher->setFocusPolicy(Qt::NoFocus);
	previewQualitySwitcher->setFont(fo);

	visualMenu = new QComboBox( this );
	visualMenu->setFocusPolicy(Qt::NoFocus);
	visualMenu->setFont(fo);
	visualMenu->setEnabled(false);
	visualMenu->setSizeAdjustPolicy(QComboBox::AdjustToContents);

	inPreview = visualMenu->isEnabled();

	addWidget(previewQualitySwitcher);
	addAction(parent->scrActions["viewToggleCMS"]);
	addAction(parent->scrActions["viewPreviewMode"]);
	addAction(parent->scrActions["viewEditInPreview"]);
	addWidget(visualMenu);

	languageChange();
}

void ViewToolBar::setDoc(ScribusDoc* doc)
{
	visualMenu->blockSignals(true);
	visualMenu->setCurrentIndex(doc->previewVisual);
	visualMenu->blockSignals(false);
	previewQualitySwitcher->blockSignals(true);
	previewQualitySwitcher->setCurrentIndex(doc->previewQuality());
	previewQualitySwitcher->blockSignals(false);
}

void ViewToolBar::setViewPreviewMode(bool b)
{
	inPreview=b;
	visualMenu->setEnabled(b);
}

void ViewToolBar::languageChange()
{
	visualMenu->blockSignals(true);
	int iVM=visualMenu->currentIndex();
	visualMenu->clear();
	visualMenu->addItem(CommonStrings::trVisionNormal);
	visualMenu->addItem(CommonStrings::trVisionProtanopia);
	visualMenu->addItem(CommonStrings::trVisionDeuteranopia);
	visualMenu->addItem(CommonStrings::trVisionTritanopia);
	visualMenu->addItem(CommonStrings::trVisionFullColorBlind);
	visualMenu->setCurrentIndex(iVM);
	visualMenu->blockSignals(false);

	previewQualitySwitcher->blockSignals(true);
	int iPQM=previewQualitySwitcher->currentIndex();
	previewQualitySwitcher->clear();
	previewQualitySwitcher->addItem(tr("High"));
	previewQualitySwitcher->addItem(tr("Normal"));
	previewQualitySwitcher->addItem(tr("Low"));
	previewQualitySwitcher->setCurrentIndex(iPQM);
	previewQualitySwitcher->blockSignals(false);

	previewQualitySwitcher->setToolTip( tr("Select the image preview quality"));
	previewQualitySwitcher->setStatusTip( tr("Select image preview quality"));
	visualMenu->setToolTip( tr("Select the visual appearance of the display. You can choose between normal and several color blindness forms."));
	visualMenu->setStatusTip( tr("Select display visual appearance"));

}
