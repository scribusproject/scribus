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

#include "ui_gradientvectorbase.h"

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QEvent>
#include "scribusapi.h"
#include "ui/scrpalettebase.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API GradientVectorDialog :  public ScrPaletteBase, Ui::GradientVectorBase
{
	Q_OBJECT

public:
	GradientVectorDialog( QWidget* parent);
	~GradientVectorDialog() {};
	virtual void changeEvent(QEvent *e);
	void selectLinear();
	void selectRadial();
	void selectFourColor();
	void selectDiamond();
	void selectMesh();

public slots:
	void languageChange();
	void handleEditButton();
	void handleEditControlButton();
	void setValues(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk, double cx, double cy);
	void changeSpecialL();
	void changeSpecialR();
	void changeSpecialF();
	void changeSpecialD();
	void unitChange(int unitIndex);

signals:
	void NewSpecial(double, double, double, double, double, double, double, double, double, double);
	void editGradient(int);
	void createNewMesh();
	void resetMesh();
	void meshToShape();
	void reset1Control();
	void resetAllControl();

};
#endif
