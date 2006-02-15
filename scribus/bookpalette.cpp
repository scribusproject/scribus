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

#include "bookpalette.h"
#include "bookpalette.moc"
extern QPixmap loadIcon(QString nam);

BookPalette::BookPalette(QWidget* parent) : ScrPaletteBase( parent, "Books", false, 0 )
{
	setIcon(loadIcon("AppIcon.png"));
	PaletteLayout = new QVBoxLayout( this, 0, 0, "PaletteLayout");
	BView = new BookMView(this);
	BView->setMinimumSize(QSize(100,150));
	PaletteLayout->addWidget( BView );
	languageChange();
}

/*
 \fn void BookPalette::closeEvent(QCloseEvent *ce)
 \author Franz Schmid
 \date
 \brief Emits the Signal Schliessen and accepts close event. The Signal is used in ScribusMainWindow to adjust the Menues.
 \param ce Close Event
 \retval None
			  */
			 /*
void BookPalette::keyPressEvent(QKeyEvent *k)
{
	QDialog::keyPressEvent(k);
}

void BookPalette::closeEvent(QCloseEvent *ce)
{
	emit Schliessen();
	ce->accept();
}

void BookPalette::reject()
{
	emit Schliessen();
	QDialog::reject();
}
*/

void BookPalette::languageChange()
{
	setCaption( tr( "Bookmarks" ) );	
}
