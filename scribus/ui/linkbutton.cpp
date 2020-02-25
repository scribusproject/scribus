/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          linkbutton.cpp  -  description
                             -------------------
    begin                : Wed Jul 11 2001
    copyright            : (C) 2001 by Franz Schmid
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

#include <QIcon>
#include <QPixmap>

#include "linkbutton.h"
#include "iconmanager.h"
#include "scribusapp.h"
#include "scribuscore.h"

LinkButton::LinkButton(QWidget *pa) : QToolButton(pa)
{
	setBackgroundRole(QPalette::Window);
	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void LinkButton::iconSetChange()
{
	QIcon a = QIcon();
	IconManager& iconManager = IconManager::instance();
	bool rtlLoad = QLocale(ScCore->getGuiLanguage()).textDirection() == Qt::RightToLeft;

	QPixmap closedPixmap(iconManager.loadPixmap("chain-closed.png", false, rtlLoad));
	a.addPixmap(closedPixmap, QIcon::Normal, QIcon::On);
	a.addPixmap(iconManager.loadPixmap("chain-open.png", false, rtlLoad), QIcon::Normal, QIcon::Off);
	setIcon(a);
	setIconSize(closedPixmap.size());
}

QSize LinkButton::sizeHint() const
{
	return iconSize();
}

QSize LinkButton::minimumSizeHint() const
{
	return iconSize();
}
