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
				 canvasmode_editmeshpatch.h  -  description
							 -------------------
	begin                : Sun Sep 25 2011
	copyright            : (C) 2011 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef CANVASMODE_EDITMESHPATCH_H
#define CANVASMODE_EDITMESHPATCH_H

#include <QObject>
#include <QTime>
#include <QPolygonF>

#include "canvasmode.h"
#include "fpointarray.h"

class MeshPoint;
class PageItem;
class PageItem_TextFrame;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditMeshPatch :  public CanvasMode
{
	public:
		explicit CanvasMode_EditMeshPatch(ScribusView* view);
		~CanvasMode_EditMeshPatch() override;

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
		void drawControlsMeshPoint(QPainter* psx, const MeshPoint& mp, bool isSelected);
		void drawControlsMeshPatch(QPainter* psx, PageItem* currItem);

	private:
		void snapToOtherPatch(double &x, double &y);
		typedef enum
		{
			noPointDefined,
			useTL,
			useTR,
			useBR,
			useBL
		} eMPatchPoint;

		typedef enum
		{
			noControlPointDefined,
			useControlT,
			useControlB,
			useControlL,
			useControlR,
			useControlC
		} eMGradientPoint;

		inline bool GetItem(PageItem** pi);

		MeshPoint* m_old_mesh {nullptr};
		PageItem *m_currItem {nullptr};
		QPointF m_currentPoint;
		QPolygonF m_clickPointPolygon;
		ScribusMainWindow* m_ScMW {nullptr};
		bool m_keyRepeat {false};
		double m_Mxp {-1.0};
		double m_Myp {-1.0};
		eMGradientPoint m_gradientPoint {noControlPointDefined};
		eMPatchPoint m_patchPoint {noPointDefined};
		int m_click_count {0};
};

#endif
