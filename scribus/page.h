/***************************************************************************
                          page.h  -  description
                             -------------------
    begin                : Sat Apr 7 2001
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

#ifndef PAGE_H
#define PAGE_H

#include <utility>

class QString;
#include "qvaluelist.h"
#include "qptrlist.h"

#include "scribusapi.h"
#include "undoobject.h"
#include "scribusstructs.h"
#include "pageitem.h"

class UndoManager;
class UndoState;

/**
  *@author Franz Schmid
  */
class SCRIBUS_API Page : public UndoObject
{
public:
	Page(const double x, const double y, const double b, const double h);
	~Page() {};
	const double xOffset();
	const double yOffset();
	const double width();
	const double height();
	const double initialWidth();
	const double initialHeight();
	void setXOffset(const double);
	void setYOffset(const double);
	void setWidth(const double);
	void setHeight(const double);
	void setInitialWidth(const double);
	void setInitialHeight(const double);
	MarginStruct Margins;
	MarginStruct initialMargins;
  /** Nummer der Seite */
	int LeftPg;
	QString MPageNam;
	QString PageNam;
	QString PageSize;
	int PageOri;
	QValueList<double> XGuides;
	QValueList<double> YGuides;
	QPtrList<PageItem> FromMaster;
	void setPageNr(const int pageNr);
	uint pageNr();
	void addXGuide(double position);
	void addYGuide(double position);
	void addXGuides(QValueList<double>& guides);
	void addYGuides(QValueList<double>& guides);
	void removeXGuide(double position);
	void removeXGuide(int index);
	void removeYGuide(double position);
	void removeYGuide(int index);
	void moveXGuide(int fromIndex, double to);
	void moveYGuide(int fromIndex, double to);
	void setPageName(const QString& newName);
	void restore(UndoState* state, bool isUndo);

private:
	UndoManager* undoManager;
	void restorePageItemCreation(ItemState<PageItem*> *state, bool isUndo);
	void restorePageItemDeletion(ItemState<PageItem*> *state, bool isUndo);
	void restorePageItemConversion(ItemState<std::pair<PageItem*, PageItem*> >*state, bool isUndo);
	
	double m_xOffset;
	double m_yOffset;
	double m_width;
	double m_height;
	double m_initialWidth;
	double m_initialHeight;
	uint m_pageNr;
};

#endif
