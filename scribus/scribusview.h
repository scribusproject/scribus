/***************************************************************************
                          scribusview.h  -  description
                             -------------------
    begin                : Fre Apr  6 21:47:55 CEST 2001
    copyright            : (C) 2001 by Franz Schmid
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

#ifndef SCRIBUSVIEW_H
#define SCRIBUSVIEW_H

// include files for QT
#include <qscrollview.h>
#include <qptrlist.h>
#include <qlineedit.h>
#include <qscrollbar.h>
#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qprogressdialog.h>
#include <qspinbox.h>
// application specific includes
#include "scribusdoc.h"
#include "page.h"
#include "hruler.h"
#include "vruler.h"
#include "libpostscript/pslib.h"
#include "mspinbox.h"
#include "pageselector.h"

extern QPixmap loadIcon(QString nam);

/**
 * This class provides an incomplete base for your application view. 
 */

class ScribusView : public QScrollView
{
	Q_OBJECT

public:
    ScribusView(QWidget *parent=0, ScribusDoc *doc=0, preV *prefs=0);
    ~ScribusView() {};
  /** Vergroesserungseingabefeld */
	MSpinBox* LE;
	PageSelector* PGS;
	QToolButton *UN;
	Hruler *HR;
	Vruler *VR;
  /** Liste der Seiten */
	QPtrList<Page> Pages;
  /** Liste der Musterseiten */
	QPtrList<Page> MasterPages;
  /** Liste der Dokumentseiten */
	QPtrList<Page> DocPages;
  /** Mapping Namen zu Musterseiten */
	QMap<QString,int> MasterNames;
  /** Fgt eine Seite hinzu */
	Page* addPage(int nr);
  /** L�cht eine Seite */
	void delPage(int Nr);
  /** verschiebt Seiten */
	void movePage(int from, int to, int ziel, int art);
	void reformPages();
	void LaMenu();
	void ShowTemplate(int nr);
	void HideTemplate();
	int CountElements();
	QPixmap PageToPixmap(int Nr, int maxGr);
	QPixmap MPageToPixmap(QString name, int maxGr);
	void RecalcPictures(ProfilesL *Pr, QProgressBar *dia = 0);
	void CreatePS(PSLib *p, uint von, uint bis, int step, bool sep, QString SepNam, bool farb, 
					bool Hm, bool Vm, bool Ic);
	void ProcessPage(PSLib *p, Page* a, uint PNr, bool sep = false, bool farb = true, bool ic = false);
	void SetClipPath(PSLib *p, FPointArray *c);
	void SetFarbe(QString farb, int shade, int *h, int *s, int *v, int *k);
	void rememberPreviousSettings(int mx=0, int my=0);
  /** Dokument zu dem die Seite geh�t */
	ScribusDoc *Doc;
	preV *Prefs;

public slots: // Public slots
  /** Fhrt die Vergr�erung/Verkleinerung aus; */
	void slotDoZoom();
  /** Vergr�ert die Ansicht */
	void slotZoomIn(int mx=0,int my=0);
  /** Verkleinert die Ansicht */
	void slotZoomOut(int mx=0,int my=0);
  /** Vergr�ert die Ansicht */
	void slotZoomIn2(int mx=0,int my=0);
  /** Verkleinert die Ansicht */
	void slotZoomOut2(int mx=0,int my=0);
  /** Zeichnet alle Seiten neu */
	void DrawNew();
	void setMenTxt(int Seite);
	void setLayMenTxt(int l);
	void GotoPa(int Seite);
	void GotoLa(int l);
	void GotoPage(int Seite);
	void ChgUnit(int art);
	void SetCPo(int x, int y);
	void SetCCPo(int x, int y);

private: // Private attributes
	QPushButton *SB1;
	QPushButton *SB2;
	QPushButton *LY;
	QPopupMenu *Unitmen;
	QPopupMenu *Laymen;
	bool Ready;
	int    oldX;
	int    oldY;
	int    oldSbx;
	int    oldSby;
	double oldScale;
	bool doZooming;
	
private slots:
	void Zval();
	void setRulerPos(int x, int y);

protected: // Protected methods
  /** Waagrechter Scrollbalken */
	virtual void contentsWheelEvent ( QWheelEvent *ev );
	virtual void setHBarGeometry(QScrollBar &bar, int x, int y, int w, int h);
	virtual void setVBarGeometry(QScrollBar &bar, int x, int y, int w, int h);

signals:
	void changeUN(int);
	void changeLA(int);
};
#endif
