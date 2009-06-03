/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QVBoxLayout>

class QEvent;

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "bookmwin.h"
/**
  *@author Franz Schmid
  */

/*! \brief A Bookmark Palette */
class SCRIBUS_API BookPalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	/*!
	\fn BookPalette::BookPalette(QWidget* parent)
	\author Franz Schmid
	\date
	\brief Constructor for Bookmark Palette
	\param parent Parent Window
	\retval None
	*/
	BookPalette(QWidget* parent);
	~BookPalette() {};
	
	virtual void changeEvent(QEvent *e);
	
	BookMView	*BView;
	
public slots:
	void languageChange();

protected:
	QVBoxLayout* PaletteLayout;
};

#endif
