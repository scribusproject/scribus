/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          linecombo.h  -  description
                             -------------------
    begin                : Thu Jul 12 2001
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

#ifndef LINECOMBO_H
#define LINECOMBO_H

#include <QComboBox>
#include <QPixmap>

class QEvent;

#include "scribusapi.h"
#include "sclistboxpixmap.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API LineCombo : public QComboBox
{
	Q_OBJECT

public: 
	LineCombo(QWidget* pa);
	~LineCombo() {}

	void updateList();

protected:
	QPixmap createIcon(int type);

	void changeEvent(QEvent *e) override;
	void styleChange();
	void themeChange();
};

#endif
