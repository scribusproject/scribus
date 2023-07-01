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


enum ruler_code
{
	rc_none = 0,
	rc_leftFrameDist = 1,
	rc_rightFrameDist = 2,
	rc_indentFirst = 3,
	rc_leftMargin = 4,
	rc_tab = 5,
	rc_rightMargin = 6
};

class SCRIBUS_API Hruler : public QWidget
{
	Q_OBJECT

public:
	Hruler(ScribusView *pa, ScribusDoc *doc);
	~Hruler() {}

	double ruleSpacing() const;
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
	int findRulerHandle(QPoint mp, int grabRadius);
	
	void paintEvent(QPaintEvent *e) override;
	void mousePressEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void enterEvent(QEvent *m) override;
	void leaveEvent(QEvent *m) override;

	void drawMarks(QPainter& p) const;
	void drawTextMarks(double pos, double endPos, QPainter& p) const;
	void drawMarker(QPainter& p) const;
	void drawNumber(const QString& num, int startx, int starty, QPainter & p) const;
	void updateTabList();

	PageItem* m_currItem {nullptr};
	QList<ParagraphStyle::TabRecord> m_tabValues;
	ScribusDoc *m_doc {nullptr};
	ScribusView *m_view {nullptr};
	bool m_drawMark {false};
	bool m_mousePressed {false};
	bool m_reverse {false};
	bool m_textEditMode {false};
	double m_colGap {0.0};
	double m_cor {0.0};
	double m_distLeft {0.0};
	double m_distRight {0.0};
	double m_firstIndent {0.0};
	double m_itemEndPos {0.0};
	double m_itemPos {0.0};
	double m_itemScale {1.0};
	double m_iter {0.0};
	double m_iter2 {0.0};
	double m_leftMargin {0.0};
	double m_lineCorr {0.0};
	double m_offset {0.0};
	double m_rightMargin {0.0};
	double m_scaling {0.0};
	int m_cols {0};
	int m_currCol {1};
	int m_currTab {0};
	int m_mouseX {0};
	int m_oldMark {0};
	int m_rulerCode {rc_none};
	int m_whereToDraw {0};
	
public slots: // Public slots
	/** \brief draw mark
	\param where where to draw */
	void draw(int where);
	void unitChange();

signals:
	void DocChanged(bool);
	void MarkerMoved(double base, double xp);

private:
	RulerGesture* rulerGesture;
};

#endif
