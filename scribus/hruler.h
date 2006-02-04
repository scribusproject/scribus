/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						hruler.h  -  description
							-------------------
	begin                : Tue Apr 10 2001
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

#ifndef HRULER_H
#define HRULER_H

#include <qwidget.h>
#include <qpainter.h>

#include "scribusapi.h"
#include "pageitem.h"

class ScribusView;
class ScribusDoc;
class ScribusMainWindow;
class PrefsManager;

/** \brief Horizontal ruler
\author Franz Schmid
*/
class SCRIBUS_API Hruler : public QWidget
{
	Q_OBJECT

public:
	Hruler(ScribusView *pa, ScribusDoc *doc);
	~Hruler() {};
	void paintEvent(QPaintEvent *e);
	void drawNumber(QString num, int startx, int starty, QPainter & p);
	void mousePressEvent(QMouseEvent *m);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *m);
	void UpdateTabList();
	void setItemPosition(double pos, double width);
	double offs;
	int Markp;
	int oldMark;
	bool Mpressed;
	QValueList<PageItem::TabRecord> TabValues;
	double ItemPos;
	double ItemEndPos;
	int Cols;
	int ActCol;
	int ActTab;
	double ColGap;
	double RExtra;
	double Extra;
	double lineCorr;
	double Indent;
	double First;
	double Scaling;
	int RulerCode;
	int MouseX;
	bool Revers;
	bool ItemPosValid;

private: // Private attributes
	ScribusDoc *currDoc;
	ScribusView *currView;

public slots: // Public slots
	/** \brief draw mark
	\param where where to draw */
	void Draw(int where);
	void unitChange();

signals:
	void DocChanged(bool);
	void MarkerMoved(double, double);

private:
	double iter, iter2;
	int cor;
	PrefsManager *prefsManager;

	void drawGuides();
};

#endif
