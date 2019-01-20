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
	~Hruler() {}

private:
	bool   m_textEditMode;
	double m_colGap;
	double m_lineCorr;
	int    m_cols;
	double m_distLeft;
	double m_distRight;
	double m_firstIndent;
	double m_leftMargin;
	double m_rightMargin;
	bool   m_reverse;
	QList<ParagraphStyle::TabRecord> m_tabValues;
	PageItem * m_currItem;

	double m_itemPos;
	double m_itemEndPos;
	double m_offset;
	double m_itemScale;

public:
	double ruleSpacing();
	void   setItem(PageItem * item);
	void   textMode(bool state) { m_textEditMode = state; }
	double textBase() const; // left text edge in canvas coord
	double textWidth() const;
	double textPosToCanvas(double x) const;
	int    textPosToLocal(double x) const;
	double localToTextPos(int x) const;
	void   shift(double pos); // using canvas coord
	void   shiftRel(double dist); // using canvas coord
	double offset() const { return m_offset; }
	
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
	void drawNumber(const QString& num, int startx, int starty, QPainter & p);
	void updateTabList();

	int  m_oldMark;
	int  m_currCol;
	int  m_currTab;
	double m_scaling;
	int m_rulerCode;

	bool m_mousePressed;
	int  m_mouseX;
	
	double m_iter, m_iter2;
	double m_cor;
	int  m_whereToDraw;
	bool m_drawMark;

	ScribusDoc *m_doc;
	ScribusView *m_view;
	
public slots: // Public slots
	/** \brief draw mark
	\param where where to draw */
	void draw(int where);
	void unitChange();

signals:
	void DocChanged(bool);
	void MarkerMoved(double base, double xp);

private:
	PrefsManager *prefsManager;
	RulerGesture* rulerGesture;
};

#endif
