/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
 
#include <iostream>

#include <qmap.h>
#include <qobject.h>
#include <qstringlist.h> 

#include <cmath>

#include "pagesize.h"
#include "units.h"

PageSize::PageSize(const QString sizeName)
{
	bool valuesSet=false;
	generatePageSizeList();
	//Build based on untranslated key value
	if (pageSizeList.contains(sizeName))
	{
		PageSizeInfoMap::Iterator it = pageSizeList.find(sizeName);
		pageSizeName=it.key();
		width=it.data().width;
		height=it.data().height;
		pageUnitIndex=it.data().pageUnitIndex;
		trPageSizeName=it.data().trSizeName;
		valuesSet=true;
	}
	else //build based on translated value.
	{
		PageSizeInfoMap::Iterator it;
		for (it=pageSizeList.begin();it!=pageSizeList.end() && valuesSet!=true;++it)
		{
			if (sizeName==it.data().trSizeName)
			{
				pageSizeName=it.key();
				width=it.data().width;
				height=it.data().height;
				pageUnitIndex=it.data().pageUnitIndex;
				trPageSizeName=it.data().trSizeName;
				valuesSet=true;
			}
		}
	}
	
	if (!valuesSet)
	{
		//qDebug("Non-existant page size selected");
		width=height=0.0;
		pageUnitIndex=-1;
		pageSizeName="Custom";
		trPageSizeName=QObject::tr("Custom");	
	}
}

PageSize::PageSize(const double w, const double h)
{
	width=w;
	height=h;
	pageSizeName="Custom";
	trPageSizeName=QObject::tr("Custom");	
}

QString PageSize::getPageName()
{
	return pageSizeName;
}

QString PageSize::getPageText()
{
	return trPageSizeName;
}

double PageSize::getPageWidth()
{
	return width;
}

double PageSize::getPageHeight()
{
	return height;
}

double PageSize::getOriginalPageWidth()
{
	return width*unitGetRatioFromIndex(pageUnitIndex);
}

double PageSize::getOriginalPageHeight()
{
	return height*unitGetRatioFromIndex(pageUnitIndex);
}

QStringList PageSize::getPageSizeList(void)
{
	QStringList pageSizes;
	pageSizes.clear();
	PageSizeInfoMap::Iterator it;
	for (it=pageSizeList.begin();it!=pageSizeList.end();++it)
		pageSizes.append(it.key());
	return QStringList(pageSizes);
}

QStringList PageSize::getTrPageSizeList(void)
{
	QStringList pageSizes;
	pageSizes.clear();
	PageSizeInfoMap::Iterator it;
	for (it=pageSizeList.begin();it!=pageSizeList.end();++it)
		pageSizes.append(it.data().trSizeName);
	return QStringList(pageSizes);
}


void PageSize::generatePageSizeList()
{
	/*
	{"A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3",
	"B4", "B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", tr("Executive"),
	tr("Folio"), tr("Ledger"), tr("Legal"), tr("Letter"), tr("Tabloid")
	};
	*/

	double width, height, tmp;
	int format;
	QString name;
	struct PageSizeInfo info;
	/* A series */
	width  = floor(1000.0 / sqrt(sqrt(2.0)) + 0.5);
	height = floor(sqrt(sqrt(2.0))*1000.0 + 0.5);
	//4A0
	info.width=mm2pts(width*2.0);
	info.height=mm2pts(height*2.0);
	info.pageUnitIndex=SC_MM;
	info.trSizeName=QObject::tr("4A0");
	pageSizeList.insert("4A0", info); 
	//2A0
	info.width=mm2pts(height);
	info.height=mm2pts(width*2.0);
	info.pageUnitIndex=SC_MM;
	info.trSizeName=QObject::tr("2A0");
	pageSizeList.insert("2A0", info); 
	//Continue with rest of A series
	for (format = 0; format <= 10; format++) {
		info.width=mm2pts(width);
		info.height=mm2pts(height);
		info.pageUnitIndex=SC_MM;
		name=QString("A%1").arg(format);
		info.trSizeName=name;
		pageSizeList.insert(name, info);
		tmp = height;
		height = width;
		width = floor(tmp / 2.0);
	}
	
	/* B series */
	width  = 1000.0;
	height = floor(sqrt(2.0)*1000.0 + 0.5);

	for (format = 0; format <= 10; format++) {
		info.width=mm2pts(width);
		info.height=mm2pts(height);
		info.pageUnitIndex=SC_MM;
		name=QString("B%1").arg(format);
		info.trSizeName=name;
		pageSizeList.insert(name, info);

		tmp = height;
		height = width;
		width = floor(tmp / 2.0);
	}

	/* C series */
	width  = floor(1000.0 / sqrt(sqrt(sqrt(2.0)))+0.5);
	height = floor(sqrt(sqrt(sqrt(8.0)))*1000.0 + 0.5);

	for (format = 0; format <= 10; format++) {
		info.width=mm2pts(width);
		info.height=mm2pts(height);
		info.pageUnitIndex=SC_MM;
		name=QString("C%1").arg(format);
		info.trSizeName=name;
		pageSizeList.insert(name, info);

		tmp = height;
		height = width;
		width = floor(tmp / 2.0);
	}
	
	//Imperial
	//http://en.wikipedia.org/wiki/Paper_sizes
	double impWidths[]={10.0,13.0,8.5,8.0,8.5,11.0,7.25,15.5,15.0,16.5,17.5,18.0,20.0,23.0,23.5,35.0,5.5,8.5,11.0,17.0,22.0,34.0};
	double impHeights[]={8.0,8.0,11.0,10.5,14.0,17.0,10.5,19.25,20.0,21.0,22.5,23.0,25.0,28.0,35.0,45.0,8.5,11.0,17.0,22.0,34.0,44.0};
	QString impNames[]={"Quarto","Foolscap","Letter","Government Letter","Legal","Ledger","Executive","Post","Crown","Large Post","Demy","Medium","Royal","Elephant","Double Demy","Quad Demy","STMT","A","B","C","D","E"};
	QString impTrNames[]={QT_TR_NOOP("Quarto"),QT_TR_NOOP("Foolscap"),QT_TR_NOOP("Letter"),QT_TR_NOOP("Government Letter"),QT_TR_NOOP("Legal"),QT_TR_NOOP("Ledger"),QT_TR_NOOP("Executive"),QT_TR_NOOP("Post"),QT_TR_NOOP("Crown"),QT_TR_NOOP("Large Post"),QT_TR_NOOP("Demy"),QT_TR_NOOP("Medium"),QT_TR_NOOP("Royal"),QT_TR_NOOP("Elephant"),QT_TR_NOOP("Double Demy"),QT_TR_NOOP("Quad Demy"),QT_TR_NOOP("STMT"),QT_TR_NOOP("A"),QT_TR_NOOP("B"),QT_TR_NOOP("C"),QT_TR_NOOP("D"),QT_TR_NOOP("E")};
	
	size_t const num_mappings_widths = (sizeof impWidths)/(sizeof *impWidths);
/*	size_t const num_mappings_heights = (sizeof impHeights)/(sizeof *impHeights);
	size_t const num_mappings_names = (sizeof impNames)/(sizeof *impNames);
	size_t const num_mappings_trnames = (sizeof impTrNames)/(sizeof *impTrNames); */
	
	//Q_ASSERT(num_mappings_widths == num_mappings_heights == num_mappings_names == num_mappings_trnames );
	
	for (uint i = 0; i < num_mappings_widths; ++i)
	{
		info.width=in2pts(impWidths[i]);
		info.height=in2pts(impHeights[i]);
		info.pageUnitIndex=SC_IN;
		info.trSizeName=impTrNames[i];
		pageSizeList.insert(impNames[i], info);
	}
	//Comm10E
	info.width=in2pts(4.125);
	info.height=in2pts(9.5);
	info.pageUnitIndex=SC_IN;
	info.trSizeName=QObject::tr("Comm10E");
	pageSizeList.insert("Comm10E", info); 
	//DLE
	info.width=mm2pts(110);
	info.height=mm2pts(220);
	info.pageUnitIndex=SC_MM;
	info.trSizeName=QObject::tr("DLE");
	pageSizeList.insert("DLE", info); 
	//Executive
	//Folio
	//Ledger
	//Legal
	//Letter
	//Tabloid
}

void PageSize::printPageSizeList()
{
	PageSizeInfoMap::Iterator it;
	for (it=pageSizeList.begin();it!=pageSizeList.end();++it)
		std::cout << it.key().leftJustify(6) << ": " << it.data().width << " x " << it.data().height << ",  " << it.data().width*unitGetRatioFromIndex(it.data().pageUnitIndex) << " x " << it.data().height*unitGetRatioFromIndex(it.data().pageUnitIndex) << ",  " << it.data().trSizeName << std::endl;
}

