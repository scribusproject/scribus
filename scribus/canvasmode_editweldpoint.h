/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
/***************************************************************************
				 canvasmode_editweldpoint.h  -  description
							 -------------------
	begin                : Sun Okt 23 2011
	copyright            : (C) 2011 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef CANVASMODE_EDITWELDPOINT_H
#define CANVASMODE_EDITWELDPOINT_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"
#include "ui/weldedit.h"

class PageItem;
class PageItem_TextFrame;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditWeldPoint :  public CanvasMode
{
	Q_OBJECT
public:
	explicit CanvasMode_EditWeldPoint(ScribusView* view);
	~CanvasMode_EditWeldPoint() override = default;

	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;

	void activate(bool) override;
	void deactivate(bool) override;
	void mouseDoubleClickEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	void keyPressEvent(QKeyEvent *e) override;
	void drawControls(QPainter* p) override;
	void drawControlsWeldPoint(QPainter* psx, PageItem* currItem);

private:
	void snapToEdgePoints(double &x, double &y);
	QRectF getUpdateRect();

	inline bool GetItem(PageItem** pi);

	PageItem *m_currItem {nullptr};
	QList<PageItem*> m_weldToList;
	ScribusMainWindow* m_ScMW {nullptr};
	WeldEditDialog* m_ModeDialog {nullptr};
	bool m_editWeldMode {true};
	bool m_keyRepeat {false};
	double m_Mxp {-1.0};
	double m_Myp {-1.0};
	int m_selectedPoint {-1};

private slots:
	void endEditing(bool active);
	void endEditing();
	void setWeldMode();
	void setObjectMode();
};

#endif
