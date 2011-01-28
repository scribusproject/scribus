/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          spiralvectordialog.h  -  description
                             -------------------
    begin                : Wed Jan 27 2011
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

#ifndef SPIRALVECTOR_H
#define SPIRALVECTOR_H

#include "ui_spiralvectorbase.h"

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QEvent>
#include "scribusapi.h"
#include "ui/scrpalettebase.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API SpiralVectorDialog :  public ScrPaletteBase, Ui::SpiralVectorBase
{
	Q_OBJECT

public:
	SpiralVectorDialog( QWidget* parent);
	~SpiralVectorDialog() {};
	virtual void changeEvent(QEvent *e);

public slots:
	void languageChange();
	void changeVectors();
	void setValues(double start, double sweep, double factor);

signals:
	void NewVectors(double, double, double);
	void endEdit();

};
#endif