/***************************************************************************
    begin                : Feb 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#ifndef PAGESIZE_H
#define PAGESIZE_H

#include <qmap.h>
#include <qstring.h>
#include "scribusapi.h"

struct PageSizeInfo
{
	double width;
	double height;
	QString trSizeName;
	int pageUnitIndex;
};

typedef QMap<QString, PageSizeInfo > PageSizeInfoMap;

class SCRIBUS_API PageSize
{
public:
	PageSize(const QString);
	PageSize(const double, const double);
	
	QString getPageName();
	QString getPageText();
	double getPageWidth();
	double getPageHeight();
	double getOriginalPageWidth();
	double getOriginalPageHeight();
	QStringList getPageSizeList();
	QStringList getTrPageSizeList();
	void generatePageSizeList();
	void printPageSizeList();

private:
	QMap<QString, PageSizeInfo > pageSizeList;
	double width;
	double height;
	int pageUnitIndex;
	QString pageSizeName;
	QString trPageSizeName;
};

#endif

