/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
	~Page();
	double xOffset() const { return m_xOffset; }
	double yOffset() const { return m_yOffset; }
	double width() const { return m_width; }
	double height() const { return m_height; }
	double initialWidth() const { return m_initialWidth; }
	double initialHeight() const { return m_initialHeight; }
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
	uint pageNr() const { return m_pageNr; }
	void setPageNr(const int pageNr);
	const QString& pageSectionNumber() const { return m_pageSectionNumber; }
	void setPageSectionNumber(const QString&);
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

	// As a bit of a dirty hack, we declare this mutable so it can be altered
	// even while the object is `const'. That's normally only for internal
	// implementation, but in this case it at least lets us guarantee the rest
	// of the object is unchanged in (eg) pdflib. This should be replaced with
	// proper access methods later.
	mutable QPtrList<PageItem> FromMaster;

private:
	UndoManager * const undoManager;
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
	QString m_pageSectionNumber;
};

#endif
