/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  weldedit.h  -  description
							 -------------------
	begin                : Tue Nov  01 2011
	copyright            : (C) 2011 by Franz Schmid
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

#ifndef WELDEDIT_H
#define WELDEDIT_H

#include "ui_weldeditbase.h"

#include <QWidget>
#include <QLayout>
#include <QEvent>
#include "scribusapi.h"
#include "ui/scrpalettebase.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API WeldEditDialog :  public ScrPaletteBase, Ui::weldEditBase
{
	Q_OBJECT

public:
	WeldEditDialog( QWidget* parent);
	~WeldEditDialog() {};
	virtual void changeEvent(QEvent *e);

public slots:
	void languageChange();

signals:
	void modeMoveWeld();
	void modeMoveObject();
	void endEdit();

};
#endif // WELDEDIT_H
