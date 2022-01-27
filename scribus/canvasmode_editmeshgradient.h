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
                 canvasmode_editmeshgradient.h  -  description
                             -------------------
    begin                : Sat Apr 24 2010
    copyright            : (C) 2010 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef CANVAS_MODE_EDITMESHGRADIENT_H
#define CANVAS_MODE_EDITMESHGRADIENT_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"

class PageItem;
class PageItem_TextFrame;
class ScribusMainWindow;
class ScribusView;
class MeshPoint;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditMeshGradient :  public CanvasMode
{
	public:
		explicit CanvasMode_EditMeshGradient(ScribusView* view);
		~CanvasMode_EditMeshGradient() override;

		void enterEvent(QEvent *) override;
		void leaveEvent(QEvent *) override;

		void activate(bool) override;
		void deactivate(bool) override;
		void keyPressEvent(QKeyEvent *e) override;
		void mouseDoubleClickEvent(QMouseEvent *m) override;
		void mouseReleaseEvent(QMouseEvent *m) override;
		void mouseMoveEvent(QMouseEvent *m) override;
		void mousePressEvent(QMouseEvent *m) override;
		void drawControls(QPainter* p) override;
		void drawControlsMeshGradient(QPainter* psx, PageItem* currItem);

	private:

		typedef enum
		{
			noPointDefined,
			useControlT,
			useControlB,
			useControlL,
			useControlR
		} eMGradientPoint;

		inline bool GetItem(PageItem** pi);

		MeshPoint *m_old_mesh {nullptr};
		QList<QPair<int, int> > m_selectedMeshPoints;
		ScribusMainWindow* m_ScMW {nullptr};
		bool m_keyRepeat {false};
		double m_Mxp {-1.0};
		double m_Myp {-1.0};
		eMGradientPoint m_gradientPoint {noPointDefined};
};


#endif
