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



#ifndef CANVAS_MODE_NODEEDIT_H
#define CANVAS_MODE_NODEEDIT_H

#include "canvasmode.h"
#include "nodeeditcontext.h"

#include <QTransform>
#include <QRect>

class QPainter;

class PageItem;
class RectSelect;
class ScribusMainWindow;

class SCRIBUS_API CanvasMode_NodeEdit : public CanvasMode
{
	public:
		explicit CanvasMode_NodeEdit (ScribusView* view);
		~CanvasMode_NodeEdit() override = default;

		void activate(bool fromgesture) override;
		void deactivate(bool forGesture) override;

		void enterEvent(QEvent *) override {};
		void leaveEvent(QEvent *) override {};

		void mouseDoubleClickEvent(QMouseEvent *m) override;
		void mouseReleaseEvent(QMouseEvent *m) override;
		void mouseMoveEvent(QMouseEvent *m) override;
		void mousePressEvent(QMouseEvent *m) override;

		void keyPressEvent(QKeyEvent *e) override;
		void keyReleaseEvent(QKeyEvent *e) override;

		/**
		 * Draws the controls for this mode
		 */
		void drawControls(QPainter* p) override;

	private:

		enum MouseMode { None = 0, NodeHandle, ControlHandle, PathSegment };

		struct StateManager {
			MouseMode clickedOn { None };
			MouseMode hoveredOver { None };
			bool isSameLength {false};
			bool isSameAngle {false};
			bool wasNewlySelected { false };
			NodeEditContext::ControlMoveMode originalMoveMode { NodeEditContext::ControlMoveMode::Auto };
			NodeEditContext::ControlMoveMode moveMode { originalMoveMode }; // we need this for converted auto mode

			void reset()
			{
				clickedOn = None;
				isSameLength = false;
				isSameAngle = false;
				wasNewlySelected = false;
				moveMode = NodeEditContext::ControlMoveMode::Auto;
			}
		};

		inline bool GetItem(PageItem** pi);
		QMap<int, int> selectedNodesList(FPointArray points);
		void handleNodeEditPress(QMouseEvent*, QRect r);
		void handleNodeEditDrag(QMouseEvent*, PageItem*);
		bool handleNodeEditMove(QMouseEvent*, QRect r, PageItem*, const QTransform&);
		FPoint constraintControlAngle(FPoint anchor, FPoint mouse, double angleStep);
		FPoint snapControlAngle(FPoint anchor, FPoint mouse, double angle);
		FPoint snapControlLength(FPoint anchor, FPoint mouse, double length);
		FPoint snapControlOrigin(FPoint anchor, FPoint mouse);
		bool deleteNodes();

		void updateItem(PageItem* currItem);
		void refreshPath(bool edited);

		ScribusMainWindow* m_ScMW { nullptr };
		RectSelect* m_rectangleSelect { nullptr };
		StateManager state;

		double m_Mxp { -1.0 }; // last mouse position
		double m_Myp { -1.0 };
		double m_Dxp { -1.0 }; // last mouse press position for rectangle select
		double m_Dyp { -1.0 };
		double m_GxM { -1.0 }; // guide position
		double m_GyM { -1.0 };
		bool m_MoveGX { false };
		bool m_MoveGY { false };

};


#endif
