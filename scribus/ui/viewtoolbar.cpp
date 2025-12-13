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
#include <QToolButton>

#include "iconmanager.h"
#include "scraction.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "scribuscore.h"
#include "scribusview.h"

ViewToolBar::ViewToolBar(ScribusMainWindow* parent) : ScToolBar( tr("View Tools"), "View_Tools", parent)
{
	QFont fo = QFont(font());
	int posi = fo.pointSize() - (ScCore->isWinGUI() ? 1 : 2);
	fo.setPointSize(posi);

	previewQualitySwitcher = new DropdownButton( this );
	previewQualitySwitcher->setFocusPolicy(Qt::NoFocus);
//	previewQualitySwitcher->setFont(fo);

	visualMenu = new DropdownButton( this );
	visualMenu->setFocusPolicy(Qt::NoFocus);
	visualMenu->setEnabled(false);

	inPreview = visualMenu->isEnabled();

	addWidget(previewQualitySwitcher);
	addAction(parent->scrActions["viewToggleWhiteSpaceMode"]);
	addAction(parent->scrActions["viewToggleCMS"]);
	addSeparator();
	addAction(parent->scrActions["viewPreviewMode"]);
	addAction(parent->scrActions["viewEditInPreview"]);
	addWidget(visualMenu);
	addSeparator();
	addAction(parent->scrActions["viewSnapToGrid"]);
	addAction(parent->scrActions["viewSnapToGuides"]);
	addAction(parent->scrActions["viewSnapToItems"]);

	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(languageChange()));
}

void ViewToolBar::setDoc(ScribusDoc* doc)
{
	if (m_doc)
	{
		disconnect(previewQualitySwitcher, SIGNAL(activated(int)), ScCore->primaryMainWindow(), SLOT(changePreviewQuality(int)));
		disconnect(visualMenu, SIGNAL(activated(int)), m_doc->view(), SLOT(switchPreviewVisual(int)));
	}

	m_doc = doc;
	if (!m_doc)
		return;

	QSignalBlocker sigColorVision(visualMenu);
	visualMenu->setCurrentIndex(m_doc->previewVisual);

	QSignalBlocker sigImageRes(previewQualitySwitcher);
	previewQualitySwitcher->setCurrentIndex(m_doc->previewQuality());

	if (m_doc)
	{
		connect(previewQualitySwitcher, SIGNAL(activated(int)), ScCore->primaryMainWindow(), SLOT(changePreviewQuality(int)), Qt::UniqueConnection);
		connect(visualMenu, SIGNAL(activated(int)), m_doc->view(), SLOT(switchPreviewVisual(int)), Qt::UniqueConnection);
	}
}

void ViewToolBar::setViewPreviewMode(bool b)
{
	inPreview = b;
	visualMenu->setEnabled(b);
}

void ViewToolBar::languageChange()
{
	IconManager &im = IconManager::instance();

	QSignalBlocker sigColorVision(visualMenu);
	visualMenu->clear();
	visualMenu->addAction(im.loadIcon("color-vision-normal", iconSize()), CommonStrings::trVisionNormal);
	visualMenu->addAction(im.loadIcon("color-vision-protanopia", iconSize()), CommonStrings::trVisionProtanopia);
	visualMenu->addAction(im.loadIcon("color-vision-deuteranopia", iconSize()), CommonStrings::trVisionDeuteranopia);
	visualMenu->addAction(im.loadIcon("color-vision-tritanopia", iconSize()), CommonStrings::trVisionTritanopia);
	visualMenu->addAction(im.loadIcon("color-vision-colorblind", iconSize()), CommonStrings::trVisionFullColorBlind);
	visualMenu->setToolTip( tr("Select the visual appearance of the display. You can choose between normal and several color blindness forms."));
	visualMenu->setStatusTip( tr("Select display visual appearance"));
	visualMenu->setCurrentIndex(visualMenu->currentIndex());

	QSignalBlocker sigImageRes(previewQualitySwitcher);
	previewQualitySwitcher->clear();
	previewQualitySwitcher->addAction(im.loadIcon("image-resolution-high", iconSize()), tr("High"));
	previewQualitySwitcher->addAction(im.loadIcon("image-resolution-normal", iconSize()), tr("Normal"));
	previewQualitySwitcher->addAction(im.loadIcon("image-resolution-low", iconSize()), tr("Low"));
	previewQualitySwitcher->setToolTip( tr("Select the image preview quality"));
	previewQualitySwitcher->setStatusTip( tr("Select image preview quality"));
	previewQualitySwitcher->setCurrentIndex(previewQualitySwitcher->currentIndex());

}
