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
#include "prefsmanager.h"

#include <QStringList>

#include <cmath>
#include <iostream>

#include "commonstrings.h"


PageSize::PageSize(const QString& sizeName)
{
	init(sizeName);
}

PageSize::PageSize(const double w, const double h)
{
	m_width=w;
	m_height=h;
	m_pageUnitIndex=-1;
	m_pageSizeName=CommonStrings::customPageSize;
	m_trPageSizeName=CommonStrings::trCustomPageSize;
}

PageSize& PageSize::operator=(const PageSize& other)
{
	init(other.name());
	return *this;
}

void PageSize::init(const QString& sizeName)
{
	m_width=m_height=0.0;
	m_pageUnitIndex=-1;
	m_pageSizeName.clear();
	m_trPageSizeName.clear();

	bool valuesSet=false;
	generateSizeList();
	//Build based on untranslated key value
	if (m_pageSizeList.contains(sizeName))
	{
		PageSizeInfoMap::Iterator it = m_pageSizeList.find(sizeName);
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
		for (it=m_pageSizeList.begin();it!=m_pageSizeList.end() && !valuesSet;++it)
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

QStringList PageSize::sizeList() const
{
	QStringList pageSizes;
	pageSizes.clear();
	PageSizeInfoMap::ConstIterator it;
	for (it=m_pageSizeList.begin();it!=m_pageSizeList.end();++it)
		pageSizes.append(it.key());
	return QStringList(pageSizes);
}

QStringList PageSize::sizeTRList() const
{
	QStringList pageSizes;
	pageSizes.clear();
	PageSizeInfoMap::ConstIterator it;
	for (it=m_pageSizeList.begin();it!=m_pageSizeList.end();++it)
		pageSizes.append(it.value().trSizeName);
	return QStringList(pageSizes);
}

QStringList PageSize::activeSizeList() const
{
	QStringList pageSizes=sizeList();
	if (PrefsManager::instance().appPrefs.activePageSizes.count()==0)
		return QStringList(pageSizes);
	QStringList activePageSizes(PrefsManager::instance().appPrefs.activePageSizes);
	QStringList activeSizes;
	for (int i = 0; i < activePageSizes.size(); ++i)
	{
		if (pageSizes.contains(activePageSizes.at(i)))
			activeSizes << activePageSizes.at(i);
	}
	return QStringList(activeSizes);
}

QStringList PageSize::activeSizeTRList() const
{
	QStringList pageTRSizes=sizeTRList();
	QStringList pageSizes=sizeList();
	if (PrefsManager::instance().appPrefs.activePageSizes.count()==0)
		return QStringList(pageTRSizes);
	QStringList activePageSizes(PrefsManager::instance().appPrefs.activePageSizes);
	QStringList activeTRSizes;
	for (int i = 0; i < activePageSizes.size(); ++i)
	{
		int j=pageSizes.indexOf(activePageSizes.at(i));
		if (j!=-1)
		{
			activeTRSizes << pageTRSizes.at(j);
		}
	}
	return QStringList(activeTRSizes);
}

QStringList PageSize::untransPageSizeList(const QStringList &transList)
{
	QStringList pageTRSizes=sizeTRList();
	QStringList pageSizes=sizeList();
	QStringList untranslatedList;
	for (int i = 0; i < transList.size(); ++i)
	{
		int j=pageTRSizes.indexOf(transList.at(i));
		if (j!=-1)
		{
			untranslatedList << pageSizes.at(j);
		}
	}
	return QStringList(untranslatedList);
}


void PageSize::generateSizeList()
{
	/*
	{"A5", "A6", "A7", "A8", "A9", "B0", "B1", "B2", "B3",
	"B4", "B5", "B6", "B7", "B8", "B9", "B10", "C5E", "Comm10E", "DLE", QObject::tr("Executive"),
	QObject::tr("Folio"), QObject::tr("Ledger"), QObject::tr("Legal"), QObject::tr("Letter"), QObject::tr("Tabloid")
	};
	*/

//	double width, height, tmp;
	int format;
	QString name;
	struct PageSizeInfo info;
	/* A series */
//	width  = floor(1000.0 / sqrt(sqrt(2.0)) + 0.5);
//	height = floor(sqrt(sqrt(2.0))*1000.0 + 0.5);
	//4A0
	info.width = mm2pts(1682.0);
	info.height = mm2pts(2378.0);
	info.pageUnitIndex = SC_MM;
	info.trSizeName = QObject::tr("4A0");
	m_pageSizeList.insert("4A0", info); 
	//2A0
	info.width = mm2pts(1189.0);
	info.height = mm2pts(1682.0);
	info.pageUnitIndex = SC_MM;
	info.trSizeName = QObject::tr("2A0");
	m_pageSizeList.insert("2A0", info); 
	//Continue with rest of A series
	// do not use the formula below, as it is correct in theory,
	// but due to rounding errors it gives incorrect results.
/*	for (format = 0; format <= 10; format++) {
		info.width=mm2pts(width);
		info.height=mm2pts(height);
		info.pageUnitIndex=SC_MM;
		name=QString("A%1").arg(format);
		info.trSizeName=name;
		pageSizeList.insert(name, info);
		tmp = height;
		height = width;
		width = floor(tmp / 2.0);
	} */
	// all of the following data is taken from the Ghostscript manual
	// "Appendix: Paper sizes known to Ghostscript"
	QList<double> Awidths;
	QList<double> Aheights;
	Awidths  <<  841.0 << 594.0 << 420.0 << 297.0 << 210.0 << 148.0 << 105.0 <<  74.0 << 52.0 << 37.0 << 26.0;
	Aheights << 1189.0 << 841.0 << 594.0 << 420.0 << 297.0 << 210.0 << 148.0 << 105.0 << 74.0 << 52.0 << 37.0;
	for (format = 0; format < Awidths.count(); format++)
	{
		info.width = mm2pts(Awidths[format]);
		info.height = mm2pts(Aheights[format]);
		info.pageUnitIndex = SC_MM;
		name = QString("A%1").arg(format);
		info.trSizeName = name;
		m_pageSizeList.insert(name, info);
	}
	
	/* B series */
	Awidths.clear();
	Aheights.clear();
	Awidths  << 1000.0 <<  707.0 << 500.0 << 353.0 << 250.0 << 176.0 << 125.0 <<  88.0 << 62.0 << 44.0 << 31.0;
	Aheights << 1414.0 << 1000.0 << 707.0 << 500.0 << 353.0 << 250.0 << 176.0 << 125.0 << 88.0 << 62.0 << 44.0;
	for (format = 0; format < Awidths.count(); format++)
	{
		info.width = mm2pts(Awidths[format]);
		info.height = mm2pts(Aheights[format]);
		info.pageUnitIndex=SC_MM;
		name = QString("B%1").arg(format);
		info.trSizeName = name;
		m_pageSizeList.insert(name, info);
	}
	/* C series */
	Awidths.clear();
	Aheights.clear();
	Awidths  <<  917.0 << 648.0 << 458.0 << 324.0 << 229.0 << 162.0 << 114.0 <<  81.0 << 57.0 << 40.0 << 28.0;
	Aheights << 1297.0 << 917.0 << 648.0 << 458.0 << 324.0 << 229.0 << 162.0 << 114.0 << 81.0 << 57.0 << 40.0;
	for (format = 0; format < Awidths.count(); format++)
	{
		info.width = mm2pts(Awidths[format]);
		info.height = mm2pts(Aheights[format]);
		info.pageUnitIndex=SC_MM;
		name = QString("C%1").arg(format);
		info.trSizeName = name;
		m_pageSizeList.insert(name, info);
	}
/*
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

	// C series
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
*/
	/* PA series */
	double tmp;
	double width  = 840;
	double height = 1120;
	for (format = 0; format <= 10; format++)
	{
		info.width=mm2pts(width);
		info.height=mm2pts(height);
		info.pageUnitIndex=SC_MM;
		name=QString("PA%1").arg(format);
		info.trSizeName=name;
		m_pageSizeList.insert(name, info);
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
		m_pageSizeList.insert(impNames[i], info);
	}
	//Comm10E
	info.width=in2pts(4.125);
	info.height=in2pts(9.5);
	info.pageUnitIndex=SC_IN;
	info.trSizeName=QObject::tr("Comm10E");
	m_pageSizeList.insert("Comm10E", info); 
	//DLE
	info.width=mm2pts(110);
	info.height=mm2pts(220);
	info.pageUnitIndex=SC_MM;
	info.trSizeName=QObject::tr("DLE");
	m_pageSizeList.insert("DLE", info); 
	// additional page sizes used by Viva Designer
	//Compact Disc
	info.width=mm2pts(119.9);
	info.height=mm2pts(120.7);
	info.pageUnitIndex=SC_MM;
	info.trSizeName=QObject::tr("Compact Disc");
	m_pageSizeList.insert("Compact Disc", info);
	//Letter Half
	info.width=mm2pts(139.7);
	info.height=mm2pts(215.9);
	info.pageUnitIndex=SC_MM;
	info.trSizeName=QObject::tr("Letter Half");
	m_pageSizeList.insert("Letter Half", info);
	//US Letter
	info.width=mm2pts(215.9);
	info.height=mm2pts(279.4);
	info.pageUnitIndex=SC_MM;
	info.trSizeName=QObject::tr("US Letter");
	m_pageSizeList.insert("US Letter", info);
	//US Legal
	info.width=mm2pts(215.9);
	info.height=mm2pts(355.6);
	info.pageUnitIndex=SC_MM;
	info.trSizeName=QObject::tr("US Legal");
	m_pageSizeList.insert("US Legal", info);
	//11x17
	info.width=in2pts(11);
	info.height=in2pts(17);
	info.pageUnitIndex=SC_IN;
	info.trSizeName=QObject::tr("11x17");
	m_pageSizeList.insert("11x17", info);

	// Arch A 9 × 12
	info.width = in2pts(9);
	info.height = in2pts(12);
	info.pageUnitIndex = SC_IN;
	info.trSizeName = QObject::tr("9x12 (Arch A)");
	m_pageSizeList.insert("9x12 (Arch A)", info);
	// Arch B 12 × 18
	info.width = in2pts(12);
	info.height = in2pts(18);
	info.pageUnitIndex = SC_IN;
	info.trSizeName = QObject::tr("12x18 (Arch B)");
	m_pageSizeList.insert("12x18 (Arch B)", info);
	// Arch C 18 × 24
	info.width = in2pts(18);
	info.height = in2pts(24);
	info.pageUnitIndex = SC_IN;
	info.trSizeName = QObject::tr("18x24 (Arch C)");
	m_pageSizeList.insert("18x (Arch C)", info);
	// Arch D 24 × 36
	info.width = in2pts(24);
	info.height = in2pts(36);
	info.pageUnitIndex = SC_IN;
	info.trSizeName = QObject::tr("24x36 (Arch D)");
	m_pageSizeList.insert("24x36 (Arch D)", info);
	// Arch E 36 × 48
	info.width = in2pts(36);
	info.height = in2pts(48);
	info.pageUnitIndex = SC_IN;
	info.trSizeName = QObject::tr("36x48 (Arch E)");
	m_pageSizeList.insert("36x48 (Arch E)", info);
	// Arch E1 30 × 42
	info.width = in2pts(30);
	info.height = in2pts(42);
	info.pageUnitIndex = SC_IN;
	info.trSizeName = QObject::tr("30x42 (Arch E1)");
	m_pageSizeList.insert("30x42 (Arch E1)", info);
	// Arch E2 26 × 38
	info.width = in2pts(26);
	info.height = in2pts(38);
	info.pageUnitIndex = SC_IN;
	info.trSizeName = QObject::tr("26x38 (Arch E2)");
	m_pageSizeList.insert("26x38 (Arch E2)", info);
	// Arch E3 27 × 39
	info.width = in2pts(27);
	info.height = in2pts(39);
	info.pageUnitIndex = SC_IN;
	info.trSizeName = QObject::tr("27x39 (Arch E3)");
	m_pageSizeList.insert("27x39 (Arch E3)", info);

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
	for (it=m_pageSizeList.begin();it!=m_pageSizeList.end();++it)
		std::cout << it.key().leftJustified(6).toStdString() << ": " << it.value().width << " x " << it.value().height << ",  " << it.value().width*unitGetRatioFromIndex(it.value().pageUnitIndex) << " x " << it.value().height*unitGetRatioFromIndex(it.value().pageUnitIndex) << ",  " << it.value().trSizeName.toStdString() << std::endl;
}

