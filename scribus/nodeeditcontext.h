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
		bool edPoints() { return m_EdPoints; }
		void setEdPoints(bool b) { m_EdPoints = b; }
		bool moveSym() { return m_MoveSym; }
		void setMoveSym(bool b) { m_MoveSym = b; }
		QList<int>& selNode() { return m_SelNode; }
		bool hasNodeSelected();
		void deselect();
		void reset();
		void setPreviewMode(bool mode);
		bool previewMode() { return m_preview; }
		int submode() { return m_submode; }
		void setSubMode(int i) { m_submode = i; }
		void reset1Control(PageItem* currItem);
		void resetControl(PageItem* currItem);
		FPointArray beginTransaction(PageItem* currItem);
		void finishTransaction(PageItem* currItem);
		ScItemState<QPair<FPointArray, FPointArray> >* finishTransaction1(PageItem* currItem);
		void finishTransaction2(PageItem* currItem, ScItemState<QPair<FPointArray, FPointArray> >* state);
		void moveClipPoint(PageItem *currItem, FPoint ip);

		FPointArray *oldClip;
		UndoTransaction nodeTransaction;

		enum SubMode { MOVE_POINT = 0, ADD_POINT = 1, DEL_POINT = 2, SPLIT_PATH = 3 };

	protected:
		int m_submode;
		bool m_isContourLine;

		int m_ClRe;
		int m_ClRe2;
		int m_SegP1;
		int m_SegP2;
		bool m_EdPoints;
		bool m_MoveSym;
		QList<int> m_SelNode;
		double m_oldItemX;
		double m_oldItemY;
		bool m_preview;
};
#endif
