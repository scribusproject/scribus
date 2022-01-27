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



#ifndef CANVAS_MODE_EDITGRADIENT_H
#define CANVAS_MODE_EDITGRADIENT_H

#include <QObject>
#include <QTime>

#include "canvasmode.h"
#include "fpointarray.h"

class PageItem;
class PageItem_TextFrame;
class ScribusMainWindow;
class ScribusView;


// This class encapsulate the old code for mouse interaction from scribusview.cpp

class CanvasMode_EditGradient :  public CanvasMode
{
	public:
		explicit CanvasMode_EditGradient(ScribusView* view);
		~CanvasMode_EditGradient() override = default;

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
		void drawControlsGradientVectors(QPainter* psx, PageItem *currItem);

	private:

		typedef enum
		{
			noPointDefined,
			useGradientStart,
			useGradientEnd,
			useGradientFocal,
			useGradientSkew,
			useControl1,
			useControl2,
			useControl3,
			useControl4,
			useControl5
		} eGradientPoint;

		inline bool GetItem(PageItem** pi);


		//save of previous state
		FPoint OldGrControl1;
		FPoint OldGrControl2;
		FPoint OldGrControl3;
		FPoint OldGrControl4;
		FPoint OldGrControl5;
		ScribusMainWindow* m_ScMW {nullptr};
		double OldGrEndX {0.0};
		double OldGrEndY {0.0};
		double OldGrFocalX {0.0};
		double OldGrFocalY {0.0};
		double OldGrMaskEndX {0.0};
		double OldGrMaskEndY {0.0};
		double OldGrMaskFocalX {0.0};
		double OldGrMaskFocalY {0.0};
		double OldGrMaskScale {0.0};
		double OldGrMaskSkew {0.0};
		double OldGrMaskStartX {0.0};
		double OldGrMaskStartY {0.0};
		double OldGrScale {0.0};
		double OldGrSkew {0.0};
		double OldGrStartX {0.0};
		double OldGrStartY {0.0};
		double OldGrStrokeEndX {0.0};
		double OldGrStrokeEndY {0.0};
		double OldGrStrokeFocalX {0.0};
		double OldGrStrokeFocalY {0.0};
		double OldGrStrokeScale {0.0};
		double OldGrStrokeSkew {0.0};
		double OldGrStrokeStartX {0.0};
		double OldGrStrokeStartY {0.0};
		double m_Mxp {-1.0};
		double m_Myp {-1.0};
		eGradientPoint m_gradientPoint {noPointDefined};
};


#endif
