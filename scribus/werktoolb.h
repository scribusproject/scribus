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

#ifndef WERKTOOLB_H
#define WERKTOOLB_H

#include <q3toolbar.h>
#include <q3mainwindow.h>
//Added by qt3to4:
#include <Q3PopupMenu>
#include "scribusapi.h"
class QToolButton;
class Q3PopupMenu;
class AutoformButtonGroup;

/**
  *@author Franz Schmid
  */

class SCRIBUS_API WerkToolB : public Q3ToolBar
{
	Q_OBJECT

public: 
	WerkToolB(Q3MainWindow* parent);
	~WerkToolB() {};

	bool Sichtbar;
	int SubMode;
	int ValCount;
	double *ShapeVals;

public slots:
	void Docken(Q3DockWindow::Place p);
	void Verbergen(bool vis);
	void GetPolyProps();
	void SelShape(int s, int c, double *vals);
	void languageChange();
		
protected:
	AutoformButtonGroup* Rechteck;
	Q3PopupMenu* insertShapeButtonMenu;
	Q3PopupMenu* insertPolygonButtonMenu;
	int idInsertPolygonButtonMenu;
	
signals:
	void Schliessen();
};

class SCRIBUS_API WerkToolBP : public Q3ToolBar  
{
	Q_OBJECT

public:
	WerkToolBP(Q3MainWindow* parent);
	~WerkToolBP() {};
	QToolButton* PDFTool;
	Q3PopupMenu* PDFM;
	QToolButton* PDFaTool;
	Q3PopupMenu* PDFA;
	bool Sichtbar;
	int PDFwerkz;
	int PDFnotiz;

public slots:
	void setPDFnotiz(int id);
	void setPDFtool(int id);
	void Docken(Q3DockWindow::Place p);
	void Verbergen(bool vis);
	void ModeFromTB();
	void languageChange();

signals:
	void Schliessen();
	void NewMode(int);
};

#endif
