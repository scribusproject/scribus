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

#ifndef CANVASMODE_OBJIMPORT_H
#define CANVASMODE_OBJIMPORT_H

#include <QEvent>
#include <QMimeData>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>

#include "canvasmode.h"
#include "fpointarray.h"

class QMimeData;

class PageItem;
class ScribusMainWindow;
class ScribusView;
class TransactionSettings;

// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_ObjImport :  public CanvasMode
{
	Q_OBJECT

public:
	explicit CanvasMode_ObjImport(ScribusView* view);
	~CanvasMode_ObjImport() override;

	void setMimeData(QMimeData* mimeData);
	void setTransactionSettings(TransactionSettings* settings);

	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;
	
	void activate(bool) override;
	void deactivate(bool) override;
	void keyPressEvent(QKeyEvent *e) override;
	void keyReleaseEvent(QKeyEvent *e) override;
	void mouseDoubleClickEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *m) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void mousePressEvent(QMouseEvent *m) override;
	void drawControls(QPainter* p) override;

// protected:
// 	void setResizeCursor(int);

private:
	QMimeData* m_mimeData {nullptr};
	TransactionSettings* m_trSettings {nullptr};
	double m_Mxp {-1.0};
	double m_Myp {-1.0};
};


#endif
