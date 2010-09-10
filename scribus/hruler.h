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

#include <QWidget>

#include "scribusapi.h"
#include "styles/paragraphstyle.h"

class PageItem;

class QPaintEvent;
class QMouseEvent;
class PrefsManager;
class RulerGesture;
class ScribusView;
class ScribusDoc;
class ScribusMainWindow;

/** \brief Horizontal ruler
\author Franz Schmid
*/
class SCRIBUS_API Hruler : public QWidget
{
	Q_OBJECT

public:
	Hruler(ScribusView *pa, ScribusDoc *doc);
	~Hruler() {};

private:
	bool textEditMode;
	double ColGap;
	double lineCorr;
	int Cols;
	double RExtra;
	double Extra;
	double Indent;
	double First;
	double RMargin;
	bool Revers;
	QList<ParagraphStyle::TabRecord> TabValues;
	PageItem * currItem;

	double ItemPos;
	double ItemEndPos;
	double offs;

public:	
	double ruleSpacing();
	void setItem(PageItem * item);
	void textMode(bool state) { textEditMode = state; }
	double textBase() const; // left text edge in canvas coord
	double textWidth() const;
	double textPosToCanvas(double x) const;
	int textPosToLocal(double x) const;
	double localToTextPos(int x) const;
	void shift(double pos); // using canvas coord
	void shiftRel(double dist); // using canvas coord
	double offset() const { return offs; }

private:
	int findRulerHandle(QPoint mp, double grabRadius);

	virtual void paintEvent(QPaintEvent *e);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void enterEvent(QEvent *m);
	virtual void leaveEvent(QEvent *m);
 
	void drawMarks(QPainter& p);
	void drawTextMarks(double pos, double endPos, QPainter& p);
	void drawMarker(QPainter& p);

	void drawNumber(QString num, int startx, int starty, QPainter & p);
	void UpdateTabList();

	int Markp;
	int oldMark;
	bool Mpressed;
	int ActCol;
	int ActTab;
	double Scaling;
	int RulerCode;
	int MouseX;

	ScribusDoc *currDoc;
	ScribusView *currView;
	
public slots: // Public slots
	/** \brief draw mark
	\param where where to draw */
	void Draw(int where);
	void unitChange();

signals:
	void DocChanged(bool);
	void MarkerMoved(double base, double xp);

private:
	double iter, iter2;
	double cor;
	int whereToDraw;
	bool drawMark;
	PrefsManager *prefsManager;
	RulerGesture* rulerGesture;
};

#endif
