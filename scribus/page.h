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

#include "scribusstructs.h"
#include "pageitem.h"
/**
  *@author Franz Schmid
  */
class Page 
{
public:
	Page(double x, double y, double b, double h);
	~Page() {};
	double Xoffset;
	double Yoffset;
	double Width;
	double Height;
	MarginStruct Margins;
  /** Nummer der Seite */
	uint PageNr;
	bool LeftPg;
	QString MPageNam;
	QString PageNam;
	QValueList<double> XGuides;
	QValueList<double> YGuides;
	QPtrList<PageItem> FromMaster;
};

#endif
