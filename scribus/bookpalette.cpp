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

BookPalette::BookPalette(QWidget* parent)
					 : QDialog( parent, "Books", false, 0 )
//					 : QDialog( parent, "Books", false, Qt::WStyle_Customize | Qt::WStyle_Title | Qt::WStyle_Tool )
{
	setIcon(loadIcon("AppIcon.xpm"));
	setCaption( tr( "Bookmarks" ) );
  PaletteLayout = new QVBoxLayout( this, 0, 0, "PaletteLayout");
	BView = new BookMView(this);
	BView->setMinimumSize(QSize(100,150));
  PaletteLayout->addWidget( BView );
}

void BookPalette::closeEvent(QCloseEvent *ce)
{
	emit Schliessen();
	ce->accept();
}
