/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          gradientvectordialog.h  -  description
                             -------------------
    begin                : Tue Nov 17 2009
    copyright            : (C) 2009 by Franz Schmid
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

#ifndef GRADVECTOR_H
#define GRADVECTOR_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QEvent>
#include "scribusapi.h"
#include "ui/scrpalettebase.h"
#include "scrspinbox.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API GradientVectorDialog : public ScrPaletteBase
{
	Q_OBJECT

public:
	GradientVectorDialog( QWidget* parent);
	~GradientVectorDialog() {};
	virtual void changeEvent(QEvent *e);
	void hideExtraWidgets();
	void showExtraWidgets();

public slots:
	void languageChange();
	void setValues(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk);
	void changeSpecial();
	void unitChange(int unitIndex);

signals:
	void NewSpecial(double, double, double, double, double, double, double, double);

protected:
	QGridLayout* freeGradientLayout;
	QLabel* GTextX1;
	QLabel* GTextY1;
	QLabel* GTextX2;
	QLabel* GTextY2;
	QLabel* GTextFX;
	QLabel* GTextFY;
	QLabel* GTextSK;
	QLabel* GTextSC;
	ScrSpinBox* gY1;
	ScrSpinBox* gX2;
	ScrSpinBox* gX1;
	ScrSpinBox* gY2;
	ScrSpinBox* gFX;
	ScrSpinBox* gFY;
	ScrSpinBox* gSk;
	ScrSpinBox* gSc;
};
#endif
