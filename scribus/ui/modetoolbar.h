/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          texttoolb.h  -  description
                             -------------------
    begin                : Sun Mar 10 2002
    copyright            : (C) 2002 by Franz Schmid
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

#ifndef MODETOOLBAR_H
#define MODETOOLBAR_H

#include "scribusapi.h"
#include "ui/sctoolbar.h"
#include <QAction>
#include <QMenu>
#include <QWidgetAction>

class QEvent;

class QToolButton;
class AutoformButtonGroup;
class ScribusMainWindow;
class WidgetPopupMenu2;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API ModeToolBar : public ScToolBar
{
	Q_OBJECT

public: 
	ModeToolBar(ScribusMainWindow* parent);
	~ModeToolBar() {};

	int SubMode;
	int ValCount;
	double *ShapeVals;
	
	virtual void changeEvent(QEvent *e);

public slots:
	void GetPolyProps();
	void SelShape(int s, int c, qreal *vals);
	void languageChange();
		
protected:
	AutoformButtonGroup* Rechteck;
	QWidgetAction* insertShapeButtonAct;
	QMenu* insertShapeButtonMenu;
	QMenu* insertPolygonButtonMenu;
	QAction* idInsertPolygonButtonMenu;
	ScribusMainWindow* m_ScMW;
};

#endif
