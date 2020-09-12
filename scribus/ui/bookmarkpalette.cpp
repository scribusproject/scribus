/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          bookpalette.cpp  -  description
                             -------------------
    begin                : Son Jan 19 2003
    copyright            : (C) 2003 by Franz Schmid
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

#include <QEvent>

#include "bookmarkpalette.h"

BookPalette::BookPalette(QWidget* parent) : ScDockPalette( parent, "Books" )
{
	setObjectName(QString::fromLocal8Bit("Books"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	BView = new BookmarkView(this);
	BView->setMinimumSize(QSize(100,150));
	setWidget( BView );
	languageChange();
}

void BookPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		ScDockPalette::changeEvent(e);
}

void BookPalette::languageChange()
{
	setWindowTitle( tr( "Bookmarks" ) );
}
