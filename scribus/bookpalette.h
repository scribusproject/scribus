/***************************************************************************
                          bookpalette.h  -  description
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

#ifndef BOOKPALETTE_H
#define BOOKPALETTE_H

#include <qwidget.h>
#include <qlayout.h>
#include <bookmwin.h>
/**
  *@author Franz Schmid
  */

class BookPalette : public QWidget
{
	Q_OBJECT

public:
	BookPalette(QWidget* parent);
	~BookPalette() {};
	void keyPressEvent(QKeyEvent *k);
	BookMView	*BView;

signals:
	void ToggleAllPalettes();

protected:
	QVBoxLayout* PaletteLayout;

};

#endif
