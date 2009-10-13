/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          gradienteditor  -  description
                             -------------------
    begin                : Mit Mai 26 2004
    copyright            : (C) 2004 by Franz Schmid
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

#ifndef GRADEDITOR_H
#define GRADEDITOR_H

#include <QLabel>
#include <QSpinBox>
#include <QLayout>
#include <QList>
#include <QFrame>

class QEvent;

#include "scribusapi.h"
#include "vgradient.h"
#include "gradientpreview.h"
#include "ui_gradienteditor.h"
#include "sccolorengine.h"

class SCRIBUS_API GradientEditor : public QFrame, Ui::GradientEditorBase
{
	Q_OBJECT

public:
	GradientEditor(QWidget *pa);
	~GradientEditor() {};
	void setGradient(VGradient grad);
	const VGradient gradient();
	void setColors(ColorList &colorList);
	QColor setColor(QString colorName, int shad);
	void setGradientEditable(bool val);
	virtual void changeEvent(QEvent *e);

public slots:
	void setPos(double);
	void changePos(int);
	void slotColor(QString name, int shade);
	void setGradTrans(double val);
	void setStopColor(const QString &);
	void setStopTrans(int val);
	void setStopShade(int val);
	void languageChange();

signals:
	void gradientChanged();

protected:
	ColorList m_colorList;
	virtual bool event ( QEvent * event );
};

#endif

