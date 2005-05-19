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

#include <qtoolbar.h>
#include <qmainwindow.h>
class QToolButton;
class QPopupMenu;
class AutoformButtonGroup;

/**
  *@author Franz Schmid
  */

class WerkToolB : public QToolBar
{
	Q_OBJECT

public: 
	WerkToolB(QMainWindow* parent);
	~WerkToolB() {};

	bool Sichtbar;
	int SubMode;
	int ValCount;
	double *ShapeVals;

public slots:
	void Docken(QDockWindow::Place p);
	void Verbergen(bool vis);
	void GetPolyProps();
	void SelShape(int s, int c, double *vals);
	void languageChange();
		
protected:
	AutoformButtonGroup* Rechteck;
	QPopupMenu* insertShapeButtonMenu;
	QPopupMenu* insertPolygonButtonMenu;
	int idInsertPolygonButtonMenu;
	
signals:
	void Schliessen();
};

class WerkToolBP : public QToolBar  
{
	Q_OBJECT

public:
	WerkToolBP(QMainWindow* parent);
	~WerkToolBP() {};
	QToolButton* PDFTool;
	QPopupMenu* PDFM;
	QToolButton* PDFaTool;
	QPopupMenu* PDFA;
	bool Sichtbar;
	int PDFwerkz;
	int PDFnotiz;

public slots:
	void setPDFnotiz(int id);
	void setPDFtool(int id);
	void Docken(QDockWindow::Place p);
	void Verbergen(bool vis);
	void ModeFromTB();
	void languageChange();

signals:
	void Schliessen();
	void NewMode(int);
};

#endif
