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
#include <qtoolbutton.h>
#include <qtooltip.h>
#include <qpopupmenu.h>

/**
  *@author Franz Schmid
  */

class WerkToolB : public QToolBar  {

Q_OBJECT

public: 
	WerkToolB(QMainWindow* parent);
	~WerkToolB() {};
	QToolButton* Select;
  QToolButton* Rotiere;
	QToolButton* Textedit;
  QToolButton* Zoom;
  QToolButton* Texte;
  QToolButton* BildB;
  QToolButton* Linien;
  QToolButton* Kreis;
  QToolButton* Rechteck;
	QToolButton* Polygon;
	QToolButton* PolyLin;
	QPopupMenu* PolyM;
  QToolButton* KetteEin;
  QToolButton* KetteAus;
  QToolButton* PDFTool;
	QPopupMenu* PDFM;
  QToolButton* PDFaTool;
	QPopupMenu* PDFA;
	bool Sichtbar;
	int EdPo;
	int PDFwerkz;
	int PDFnotiz;

public slots:
	void setPDFnotiz(int id);
	void setPDFtool(int id);
	void Docken(QDockWindow::Place p);
	void Verbergen(bool vis);
	void GetPolyProps();
	void ModeFromTB();
		
signals:
	void Schliessen();
	void NewMode(int);
};

#endif
