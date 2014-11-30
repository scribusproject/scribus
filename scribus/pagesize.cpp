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

#include "pagesize.h"
 

#include <QStringList>

#include <cmath>
#include <iostream>

#include "commonstrings.h"


PageSize::PageSize(const QString sizeName)
{
	bool valuesSet=false;
	generateSizeList();
	//Build based on untranslated key value
	if (pageSizeList.contains(sizeName))
	{
		PageSizeInfoMap::Iterator it = pageSizeList.find(sizeName);
		m_pageSizeName=it.key();
		m_width=it.value().width;
		m_height=it.value().height;
		m_pageUnitIndex=it.value().pageUnitIndex;
		m_trPageSizeName=it.value().trSizeName;
		valuesSet=true;
	}
	else //build based on translated value.
	{
		PageSizeInfoMap::Iterator it;
		for (it=pageSizeList.begin();it!=pageSizeList.end() && valuesSet!=true;++it)
		{
			if (sizeName==it.value().trSizeName)
			{
				m_pageSizeName=it.key();
				m_width=it.value().width;
				m_height=it.value().height;
				m_pageUnitIndex=it.value().pageUnitIndex;
				m_trPageSizeName=it.value().trSizeName;
				valuesSet=true;
			}
		}
	}
	
	if (!valuesSet)
	{
		//qDebug("Non-existant page size selected");
		m_width=m_height=0.0;
		m_pageUnitIndex=-1;
		m_pageSizeName=CommonStrings::customPageSize;
		m_trPageSizeName=CommonStrings::trCustomPageSize;	
	}
}

PageSize::PageSize(const double w, const double h)
{
	m_width=w;
	m_height=h;
	m_pageSizeName=CommonStrings::customPageSize;
	m_trPageSizeName=CommonStrings::trCustomPageSize;	
}

QStringList PageSize::sizeList(void) const
{
	QStringList pageSizes;
	pageSizes.clear();
	PageSizeInfoMap::ConstIterator it;
	for (it=pageSizeList.begin();it!=pageSizeList.end();++it)
		pageSizes.append(it.key());
	return QStringList(pageSizes);
}

QStringList PageSize::sizeTRList(void) const
{
	QStringList pageSizes;
	pageSizes.clear();
	PageSizeInfoMap::ConstIterator it;
	for (it=pageSizeList.begin();it!=pageSizeList.end();++it)
		pageSizes.append(it.value().trSizeName);
	return QStringList(pageSizes);
}


void PageSize::generateSizeList()
{
	/*
	{"A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3",
	"B4", "B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", QObject::tr("Executive"),
	QObject::tr("Folio"), QObject::tr("Ledger"), QObject::tr("Legal"), QObject::tr("Letter"), QObject::tr("Tabloid")
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

	/* PA series */
	width  = 840;
	height = 1120;
	for (format = 0; format <= 10; format++) {
		info.width=mm2pts(width);
		info.height=mm2pts(height);
		info.pageUnitIndex=SC_MM;
		name=QString("PA%1").arg(format);
		info.trSizeName=name;
		pageSizeList.insert(name, info);
		tmp = height;
		height = width;
		width = floor(tmp / 2.0);
	}
	
	//Imperial
	//http://en.wikipedia.org/wiki/Paper_sizes
	double impWidths[]={8.0,8.0,8.5,8.0,8.5,11.0,7.25,15.5,15.0,16.5,17.5,18.0,20.0,23.0,23.5,35.0,5.5,8.5,11.0,17.0,22.0,34.0};
	double impHeights[]={10.0,13.0,11.0,10.5,14.0,17.0,10.5,19.25,20.0,21.0,22.5,23.0,25.0,28.0,35.0,45.0,8.5,11.0,17.0,22.0,34.0,44.0};
	QString impNames[]={"Quarto","Foolscap","Letter","Govt. Letter","Legal","Ledger","Executive","Post","Crown","Large Post","Demy","Medium","Royal","Elephant","Double Demy","Quad Demy","STMT","A","B","C","D","E"};
	QString impTrNames[]={QObject::tr("Quarto"), QObject::tr("Foolscap"), QObject::tr("Letter"),
		QObject::tr("Govt. Letter"), QObject::tr("Legal"), QObject::tr("Ledger"), QObject::tr("Executive"),
		QObject::tr("Post"), QObject::tr("Crown"), QObject::tr("Large Post"), QObject::tr("Demy"),
		QObject::tr("Medium"), QObject::tr("Royal"), QObject::tr("Elephant"), QObject::tr("Double Demy"),
		QObject::tr("Quad Demy"), QObject::tr("STMT"), QObject::tr("A"), QObject::tr("B"), QObject::tr("C"),
		QObject::tr("D"), QObject::tr("E")};
	
	size_t const num_mappings_widths = (sizeof impWidths)/(sizeof *impWidths);
	
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

void PageSize::printSizeList()
{
	PageSizeInfoMap::Iterator it;
	for (it=pageSizeList.begin();it!=pageSizeList.end();++it)
		std::cout << it.key().leftJustified(6).toStdString() << ": " << it.value().width << " x " << it.value().height << ",  " << it.value().width*unitGetRatioFromIndex(it.value().pageUnitIndex) << " x " << it.value().height*unitGetRatioFromIndex(it.value().pageUnitIndex) << ",  " << it.value().trSizeName.toStdString() << std::endl;
}

