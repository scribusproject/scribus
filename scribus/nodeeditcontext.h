/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						  noeeditcontext.h  -  description
                             -------------------
	begin                : 2015
	copyright            : (C) 2015 by Scribus Team
	email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef NODEEDITCONTEXT_H
#define NODEEDITCONTEXT_H

#ifdef HAVE_CONFIG_H
#include "scconfig.h"
#endif

#include <QList>
#include <QPair>

#include "fpoint.h"
#include "fpointarray.h"
#include "observable.h"
#include "undostate.h"
#include "undotransaction.h"

class SCRIBUS_API NodeEditContext : public MassObservable<QPointF>
{
	public:
		enum SubMode
		{
			MOVE_POINT = 0,
			ADD_POINT,
			DEL_POINT,
			SPLIT_PATH,
			EDIT_POINT
		};

		enum NodeType
		{
			ControlLeft = 0,
			NodeLeft,
			NodeRight,
			ControlRight,
			Opposite, // opposite control/node index of input
			Sibling, // sibling of node is control, sibling of control is node
			OppositeSibling, // combination of opposite and sibling
			Self
		};

		enum ControlMoveMode
		{
			Auto = 0,
			Symmetric,
			Asymmetric,
			Independent
		};

		struct OppositeControl {
			FPoint relativePos;
			bool isValid {false};
			int controlIndex {-1};
			int nodeIndex {-1};
			double distance {0};
			double angle {0};
			bool sameAngle {false};
			bool sameLength {false};
		};

		NodeEditContext();

		bool isContourLine() { return m_isContourLine; }
		void setIsContourLine(bool b) { m_isContourLine = b; }
		int clre() { return m_ClRe; }
		void setClre(int i) { m_ClRe = i; }
		int clre2() { return m_ClRe2; }
		void setClre2(int i) { m_ClRe2 = i; }
		int segP1() { return m_SegP1; }
		void setSegP1(int i) { m_SegP1 = i; }
		int segP2() { return m_SegP2; }
		void setSegP2(int i) { m_SegP2 = i; }
		ControlMoveMode moveMode() { return m_moveMode; }
		void setMoveMode(ControlMoveMode b) { m_moveMode = b; }
		double scale() { return m_scale; };
		void setScale(double scale) {m_scale = scale; }

		QList<int>& selNode() { return m_SelNode; }
		bool hasNodeSelected() const;
		void deselect();
		void selectNode(int i);
		void deselectNode(int i);
		void deselectNodeControls();
		int  selectionCount() const;

		void getPoints(PageItem* currItem, int i, FPoint& ctrlLeft, FPoint& nodeLeft, FPoint& nodeRight, FPoint& ctrlRight);
		FPoint getPoint(PageItem* currItem, int i, NodeType nodeType = NodeType::Self);
		bool isSymetric(PageItem* currItem, int i = -1);
		bool sameAngle(PageItem* currItem, int i = -1);
		bool sameLength(PageItem* currItem, int i = -1);

		static bool isNode(int i) { return i % 2 == 0; }
		static bool isLeftControl(int i);
		static bool isLeftNode(int i);
		static bool isRightNode(int i);
		static bool isRightControl(int i);
		static bool hasTwoNodes(int i, FPointArray clip);
		static bool isSharpNode(int i, FPointArray clip);
		static int indexOfNode(int i, NodeType type = NodeType::NodeRight, int size = -1);

		void reset();
		void setPreviewMode(bool mode);
		bool previewMode() { return m_preview; }
		int submode() { return m_submode; }
		void setSubMode(int i) { m_submode = i; }
		void reset1Control(PageItem* currItem);
		void resetControl(PageItem* currItem);
		FPointArray clipFromItem(PageItem* currItem);
		FPointArray beginTransaction(PageItem* currItem);
		void finishTransaction(PageItem* currItem);
		ScOldNewState<FPointArray>* finishTransaction1(PageItem* currItem);
		void finishTransaction2(PageItem* currItem, ScOldNewState<FPointArray>* state);
		void moveClipPoint(PageItem *currItem, const FPoint& ip);
		void equalizeControls(PageItem *currItem);

		FPointArray *oldClip { nullptr };
		UndoTransaction nodeTransaction;
		OppositeControl oppositeControl;

	protected:
		int m_submode { MOVE_POINT };
		bool m_isContourLine { false };

		int m_ClRe { -1 };
		int m_ClRe2 { -1 };
		int m_SegP1 { -1 };
		int m_SegP2 { -1 };
		ControlMoveMode m_moveMode { Auto };
		QList<int> m_SelNode;
		double m_oldItemX { 0.0 };
		double m_oldItemY { 0.0 };
		double m_scale { 1.0 };
		bool m_preview { false };
};
#endif
