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

class QString;
#include "qvaluelist.h"
#include "qptrlist.h"

#include "undoobject.h"
#include "scribusstructs.h"
#include "pageitem.h"

class UndoManager;
class UndoState;

/**
  *@author Franz Schmid
  */
class Page : public UndoObject
{
public:
	Page(double x, double y, double b, double h);
	~Page() {};
	double Xoffset;
	double Yoffset;
	double Width;
	double Height;
	double initialWidth;
	double initialHeight;
	MarginStruct Margins;
	MarginStruct initialMargins;
  /** Nummer der Seite */
	uint PageNr;
	bool LeftPg;
	QString MPageNam;
	QString PageNam;
	QString PageSize;
	int PageOri;
	QValueList<double> XGuides;
	QValueList<double> YGuides;
	QPtrList<PageItem> FromMaster;
	void setPageNr(int pageNr);
	uint getPageNr();
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
	void restoreMasterPageApplying(SimpleState *state, bool isUndo);
};

#endif
