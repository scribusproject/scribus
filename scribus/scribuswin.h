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

#include <q3mainwindow.h>
//Added by qt3to4:
#include <Q3Frame>
#include <QHBoxLayout>
#include <QCloseEvent>
class ScribusView;
class ScribusDoc;
#include "scribusapi.h"
#include "muster.h"
/**
  *@author Franz Schmid
  */

class SCRIBUS_API ScribusWin : public Q3MainWindow
{
	Q_OBJECT

public: 
	ScribusWin(QWidget* parent, ScribusDoc* ddoc);
	~ScribusWin() {};
	void setView(ScribusView* dview);
	void closeEvent(QCloseEvent *ce);
	ScribusView* view;
	ScribusDoc* doc;
	MasterPagesPalette* muster;
	Q3Frame *statusFrame;
	bool MenuStat[7];
	int winIndex;
	
public slots:
	void slotAutoSave();
	
signals:
	void AutoSaved();

protected:
	QHBoxLayout* statusFrameLayout;
};

#endif
