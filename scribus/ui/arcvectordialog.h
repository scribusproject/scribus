/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          arcvectordialog.h  -  description
                             -------------------
    begin                : Mon Jan 17 2011
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

#ifndef ARCVECTOR_H
#define ARcVECTOR_H

#include "ui_arcvectorbase.h"

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QEvent>
#include "scribusapi.h"
#include "ui/scrpalettebase.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API ArcVectorDialog :  public ScrPaletteBase, Ui::ArcVectorBase
{
	Q_OBJECT

public:
	ArcVectorDialog( QWidget* parent);
	~ArcVectorDialog() {};
	virtual void changeEvent(QEvent *e);

public slots:
	void languageChange();
	void changeVectors();
	void setValues(double start, double sweep, double height, double width);
	void unitChange(int unitIndex);

signals:
	void NewVectors(double, double, double, double);
	void endEdit();

};
#endif