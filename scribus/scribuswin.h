/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scribuswin.h  -  description
                             -------------------
    begin                : Mit Nov 6 2002
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

#ifndef SCRIBUSWIN_H
#define SCRIBUSWIN_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QMdiSubWindow>

class ScribusDoc;
class ScribusMainWindow;
class ScribusView;

#include "scribusapi.h"
/**
  *@author Franz Schmid
  */

class SCRIBUS_API ScribusWin : public QMainWindow
{
	Q_OBJECT

public: 
	ScribusWin(QWidget* parent, ScribusDoc* doc);
	~ScribusWin() {};
	void closeEvent(QCloseEvent *ce);
	void setView(ScribusView* newView);
	void setMainWindow(ScribusMainWindow *);
	ScribusView* view() const { return m_View;}
	ScribusDoc* doc() const { return m_Doc;}
	QMdiSubWindow* getSubWin() { return m_subWindow; }
	void setSubWin(QMdiSubWindow *win) { m_subWindow = win; }

protected:
	QMdiSubWindow *m_subWindow;
	ScribusDoc *m_Doc;
	ScribusMainWindow *m_ScMW;
	ScribusView *m_View;
	int m_winIndex;

};

#endif
