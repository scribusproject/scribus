/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          patternpropsdialog.h  -  description
                             -------------------
    begin                : Thu Nov 19 2009
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

#ifndef PATTERNPROPSDIALOG_H
#define PATTERNPROPSDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QGroupBox>
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QEvent>
#include "scribusapi.h"
#include "scrspinbox.h"
#include "linkbutton.h"

class SCRIBUS_API PatternPropsDialog : public QDialog
{
	Q_OBJECT

public:
	PatternPropsDialog(QWidget* parent, int unitIndex, bool isStroke);
	~PatternPropsDialog() {};

	ScrSpinBox* spinXoffset { nullptr };
	ScrSpinBox* spinYoffset { nullptr };
	ScrSpinBox* spinXscaling { nullptr };
	ScrSpinBox* spinYscaling { nullptr };
	ScrSpinBox* spinAngle { nullptr };
	ScrSpinBox* spinXSkew { nullptr };
	ScrSpinBox* spinYSkew { nullptr };
	ScrSpinBox* spinSpacing { nullptr };
	QToolButton* FlipH { nullptr };
	QToolButton* FlipV { nullptr };
	LinkButton* keepScaleRatio { nullptr };
	bool forStroke { false };

public slots:
	void languageChange();
	void changePatternProps();
	void ToggleKette();
	void HChange();
	void VChange();

signals:
	void NewPatternProps(double, double, double, double, double, double, double, bool, bool);
	void NewPatternPropsS(double, double, double, double, double, double, double, double, bool, bool);

protected:
	QGroupBox* groupOffset { nullptr };
	QLabel* textLabel1 { nullptr };
	QLabel* textLabel2 { nullptr };
	QGroupBox* groupScale { nullptr };
	QLabel* textLabel5 { nullptr };
	QLabel* textLabel6 { nullptr };
	QGroupBox* groupRotation { nullptr };
	QLabel* textLabel7 { nullptr };
	QGroupBox* groupSkew { nullptr };
	QLabel* textLabel8 { nullptr };
	QLabel* textLabel9 { nullptr };
	QLabel* textLabel15 { nullptr };
	QGroupBox* groupSpace { nullptr };
	QLabel* textLabel20 { nullptr };
	QVBoxLayout* frame3Layout { nullptr };
	QHBoxLayout* groupOffsetLayout { nullptr };
	QGridLayout* groupScaleLayout { nullptr };
	QGridLayout* groupSkewLayout { nullptr };
	QHBoxLayout* groupRotationLayout { nullptr };
	QHBoxLayout* groupSpaceLayout { nullptr };
	QPushButton* buttonOk { nullptr };
	QHBoxLayout* buttonLayout { nullptr };
	QHBoxLayout* groupFlipLayout { nullptr };

	void changeEvent(QEvent *e) override;
};
#endif
