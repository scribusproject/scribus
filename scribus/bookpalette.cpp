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

/*!
 \fn BookPalette::BookPalette(QWidget* parent)
 \author Franz Schmid
 \date
 \brief Constructor for Bookmark Palette
 \param parent Parent Window
 \retval None
 */

BookPalette::BookPalette(QWidget* parent) : QWidget( parent, "Books")
{
	PaletteLayout = new QVBoxLayout( this, 0, 0, "PaletteLayout");
	setFont(qApp->font());
	BView = new BookMView(this);
	BView->setMinimumSize(QSize(100,150));
	PaletteLayout->addWidget( BView );
	clearWState( WState_Polished );
}

void BookPalette::keyPressEvent(QKeyEvent *k)
{
	if (k->key() == Key_F10)
		emit ToggleAllPalettes();
	QWidget::keyPressEvent(k);
}
