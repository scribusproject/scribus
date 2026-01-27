/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
    begin                : Feb 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@scribus.info
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "commonstrings.h"
#include "pagesize.h"
#include "prefsmanager.h"
#include <QStringList>
#include <QObject>

PageSize::PageSize(const QString& sizeName)
{
	init(sizeName);
}

PageSize::PageSize(double w, double h)
        : m_width(w),
          m_height(h)
{
	m_pageSizeName = CommonStrings::customPageSize;
	m_trPageSizeName = CommonStrings::trCustomPageSize;
}

PageSize& PageSize::operator=(const PageSize& other)
{
	init(other.name());
	return *this;
}

void PageSize::init(const QString& sizeName)
{
	m_width = 0.0;
	m_height = 0.0;
	m_pageUnitIndex = -1;
	m_pageSizeName.clear();
	m_trPageSizeName.clear();

	bool valuesSet = false;
	generateSizeList();
	//Build based on untranslated key value
	if (m_pageSizeList.contains(sizeName))
	{
		auto it = m_pageSizeList.constFind(sizeName);
		m_pageSizeName = it.key();
		m_width = it.value().width;
		m_height = it.value().height;
		m_pageUnitIndex = it.value().pageUnitIndex;
		m_trPageSizeName = it.value().trSizeName;
		m_category = it.value().category;
		valuesSet = true;
	}
	else //build based on translated value.
	{
		PageSizeInfoMap::Iterator it;
		for (it = m_pageSizeList.begin(); it != m_pageSizeList.end() && !valuesSet; ++it)
		{
			if (sizeName == it.value().trSizeName)
			{
				m_pageSizeName = it.key();
				m_width = it.value().width;
				m_height = it.value().height;
				m_pageUnitIndex = it.value().pageUnitIndex;
				m_trPageSizeName = it.value().trSizeName;
				m_category = it.value().category;
				valuesSet = true;
			}
		}
	}

	if (!valuesSet)
	{
		//qDebug("Non-existent page size selected");
		m_width = 0.0;
		m_height = 0.0;
		m_pageUnitIndex = -1;
		m_pageSizeName = CommonStrings::customPageSize;
		m_trPageSizeName = CommonStrings::trCustomPageSize;
		m_category = PageSizeInfo::Custom;
	}
}

QStringList PageSize::defaultSizesList()
{
	return { "IsoA_A03", "IsoA_A04", "IsoA_A05", "IsoA_A06", "US_Letter" };
}

PageSizeCategoriesMap PageSize::categories() const
{
	PageSizeCategoriesMap map;

	for (auto it = m_pageSizeList.begin(); it != m_pageSizeList.end(); ++it)
		map.insert(it.value().category, categoryToString(it.value().category));

	return map;
}

PageSizeInfoMap PageSize::sizesByCategory(PageSizeInfo::Category category) const
{
	PageSizeInfoMap map;

	for (auto it = m_pageSizeList.begin(); it != m_pageSizeList.end(); ++it)
	{
		if (it.value().category == category)
			map.insert(it.value().sizeName, it.value());
	}

	return map;
}

PageSizeInfoMap PageSize::sizesByDimensions(QSize sizePt) const
{
	PageSizeInfoMap map;

	for (auto it = m_pageSizeList.begin(); it != m_pageSizeList.end(); ++it)
	{
		if (it.value().width == sizePt.width() && it.value().height == sizePt.height())
			map.insert(it.value().sizeName, it.value());
	}

	return map;
}

PageSizeInfoMap PageSize::activePageSizes() const
{
	PageSizeInfoMap map;
	if (PrefsManager::instance().appPrefs.activePageSizes.count() == 0)
		return pageSizes();

	QStringList activeList(PrefsManager::instance().appPrefs.activePageSizes);

	for (auto it = m_pageSizeList.begin(); it != m_pageSizeList.end(); ++it)
	{
		if (activeList.contains(it.value().sizeName))
			map.insert(it.value().sizeName, it.value());
	}

	return map;
}

void PageSize::addPageSize(const QString id, const QString name, double width, double height, int unitIndex, PageSizeInfo::Category category)
{

	struct PageSizeInfo info;
	QString unit;

	switch (unitIndex)
	{
	case SC_C: // cicero
		info.width = c2pts(width);
		info.height = c2pts(height);
		unit = "c";
		break;
	case SC_CM: // centimeter
		info.width = cm2pts(width);
		info.height = cm2pts(height);
		unit = "cm";
		break;
	case SC_IN: // inch
		info.width = in2pts(width);
		info.height = in2pts(height);
		unit = "in";
		break;
	case SC_MM: // millimeter
	default:
		info.width = mm2pts(width);
		info.height = mm2pts(height);
		unit = "mm";
		break;
	case SC_P: // picas
		info.width = p2pts(width);
		info.height = p2pts(height);
		unit = "p";
		break;
	}

	info.pageUnitIndex = unitIndex;
	info.sizeName = id;
	info.trSizeName = name;
	info.category = category;
	info.sizeLabel = QString("%1 x %2 %3").arg(width).arg(height).arg(unit);
	m_pageSizeList.insert(info.sizeName, info);
}

void PageSize::addPageSize(const QString id, double width, double height, int unitIndex, PageSizeInfo::Category category)
{
	addPageSize(id, id, width, height, unitIndex, category);
}

QString PageSize::categoryToString(PageSizeInfo::Category category) const
{
	switch (category)
	{
	case PageSizeInfo::Preferred:
		return QObject::tr("Preferred");
	case PageSizeInfo::Custom:
		return CommonStrings::trCustomPageSize;
	case PageSizeInfo::Book:
		return QObject::tr("Books");
	case PageSizeInfo::BusinessCards:
		return QObject::tr("Business Cards");
	case PageSizeInfo::Canadian:
		return QObject::tr("Canadian");
	case PageSizeInfo::Chinese:
		return QObject::tr("Chinese");
	case PageSizeInfo::Colombian:
		return QObject::tr("Colombian");	
	case PageSizeInfo::French:
		return QObject::tr("French");
	case PageSizeInfo::German:
		return QObject::tr("German");
	case PageSizeInfo::Imperial:
		return QObject::tr("Imperial");
	case PageSizeInfo::IsoA:
		return QObject::tr("ISO A Paper");
	case PageSizeInfo::IsoB:
		return QObject::tr("ISO B Paper");
	case PageSizeInfo::IsoC:
		return QObject::tr("ISO C Envelope");
	case PageSizeInfo::IsoEnvelope:
		return QObject::tr("International Envelopes");
	case PageSizeInfo::Japanese:
		return QObject::tr("Japanese");
	case PageSizeInfo::Newspaper:
		return QObject::tr("Newspapers");
	case PageSizeInfo::Other:
		return QObject::tr("Others");
	case PageSizeInfo::Swedish:
		return QObject::tr("Swedish");
	case PageSizeInfo::Transitional:
		return QObject::tr("Transitional");
	case PageSizeInfo::USStandard:
		return QObject::tr("US Paper");
	case PageSizeInfo::USPress:
		return QObject::tr("US Press");
	case PageSizeInfo::USEnvelope:
		return QObject::tr("US Envelopes");
	default:
		return "undefined";
		break;
	}
}


void PageSize::generateSizeList()
{
	// Books
	// https://papersizes.io/books/	
	// https://paper-size.com/size/12mo-books-sizes.html
	// https://en.wikipedia.org/wiki/Book_size
	// https://blissetts.com/sizes-stocks-more
	PageSizeInfo::Category catBooks =  PageSizeInfo::Book;
	QString prefix = "Book_";
	addPageSize(prefix + "Folio", QObject::tr("Folio"), 12.0, 19.0, SC_IN, catBooks);
	addPageSize(prefix + "4to Quarto", QObject::tr("4to (Quarto)"), 9.5, 12.0, SC_IN, catBooks);
	addPageSize(prefix + "8vo Imperial Octavo", QObject::tr("8vo (Imperial Octavo)"), 8.25, 11.5, SC_IN, catBooks);
	addPageSize(prefix + "8vo Super Octavo", QObject::tr("8vo (Super Octavo)"), 7.0, 11.0, SC_IN, catBooks);
	addPageSize(prefix + "8vo Royal Octavo", QObject::tr("8vo (Royal Octavo)"), 6.25, 10.0, SC_IN, catBooks);
	addPageSize(prefix + "8vo Medium Octavo", QObject::tr("8vo (Medium Octavo)"), 6.5, 9.25, SC_IN, catBooks);
	addPageSize(prefix + "8vo Octavo", QObject::tr("8vo (Octavo)"), 6.0, 9.0, SC_IN, catBooks);
	addPageSize(prefix + "8vo Crown Octavo", QObject::tr("8vo (Crown Octavo)"), 5.375, 8.0, SC_IN, catBooks);
	addPageSize(prefix + "12mo", QObject::tr("12mo (Duodecimo)"), 5.0, 7.375, SC_IN, catBooks);
	addPageSize(prefix + "16mo", QObject::tr("16mo (Sextodecimo)"), 4.0, 6.75, SC_IN, catBooks);
	addPageSize(prefix + "18mo", QObject::tr("18mo (Octodecimo)"), 4.0, 6.5, SC_IN, catBooks);
	addPageSize(prefix + "32mo", QObject::tr("32mo (Tricesimo Secondo)"), 3.5, 5.5, SC_IN, catBooks);
	addPageSize(prefix + "48mo", QObject::tr("48mo (Quadragesimo Octavo)"), 2.5, 4.0, SC_IN, catBooks);
	addPageSize(prefix + "64mo", QObject::tr("64mo (Sexagesimo Quarto)"), 2.0, 3.0, SC_IN, catBooks);
	addPageSize(prefix + "A Format", QObject::tr("A Format"), 4.25, 7.0, SC_IN, catBooks);
	addPageSize(prefix + "B Format", QObject::tr("B Format"), 5.1, 7.75, SC_IN, catBooks);
	addPageSize(prefix + "C Format", QObject::tr("C Format"), 5.25, 8.5, SC_IN, catBooks);
	addPageSize(prefix + "Comic Book", QObject::tr("Comic Book"), 6.63, 10.25, SC_IN, catBooks);
	addPageSize(prefix + "Crown Quarto", QObject::tr("Crown Quarto"), 189, 246, SC_MM, catBooks);

	// Business Cards
	// https://papersizes.io/business-card/	
	PageSizeInfo::Category catBusinessCards = PageSizeInfo::BusinessCards;
	prefix = "Cards_";
	addPageSize(prefix + "Europe", QObject::tr("Europe"), 55.0, 85.0, SC_MM, catBusinessCards);
	addPageSize(prefix + "ISO 7810 ID-1", QObject::tr("ISO 7810 ID-1"), 54.0, 85.6, SC_MM, catBusinessCards);
	addPageSize(prefix + "US/Canada", QObject::tr("US/Canada"), 2.0, 3.5, SC_IN, catBusinessCards);
	addPageSize(prefix + "China", QObject::tr("China"), 54.0, 90.0, SC_MM, catBusinessCards);
	addPageSize(prefix + "Scandinavia", QObject::tr("Scandinavia"), 55.0, 90.0, SC_MM, catBusinessCards);
	addPageSize(prefix + "Japan", QObject::tr("Japan"), 55.0, 91.0, SC_MM, catBusinessCards);
	addPageSize(prefix + "Hungary", QObject::tr("Hungary"), 50.0, 90.0, SC_MM, catBusinessCards);
	addPageSize(prefix + "Iran", QObject::tr("Iran"), 48.0, 85.0, SC_MM, catBusinessCards);
	addPageSize(prefix + "ISO 216", QObject::tr("ISO 216"), 52.0, 74.0, SC_MM, catBusinessCards);

	// Canadian
	// https://papersizes.io/canadian/	
	PageSizeInfo::Category catCanadian = PageSizeInfo::Canadian;
	prefix = "CA_";
	addPageSize(prefix + "P1", "P1", 560.0, 860.0, SC_MM, catCanadian);
	addPageSize(prefix + "P2", "P2", 430.0, 560.0, SC_MM, catCanadian);
	addPageSize(prefix + "P3", "P3", 280.0, 430.0, SC_MM, catCanadian);
	addPageSize(prefix + "P4", "P4", 215.0, 280.0, SC_MM, catCanadian);
	addPageSize(prefix + "P5", "P5", 140.0, 215.0, SC_MM, catCanadian);
	addPageSize(prefix + "P6", "P6", 107.0, 140.0, SC_MM, catCanadian);

	// Chinese
	// https://papersizes.io/chinese/	
	PageSizeInfo::Category catChinese = PageSizeInfo::Chinese;
	prefix = "CN_";
	addPageSize(prefix + "D0", "D0", 764.0, 1064.0, SC_MM, catChinese);
	addPageSize(prefix + "D1", "D1", 532.0, 760.0, SC_MM, catChinese);
	addPageSize(prefix + "D2", "D2", 380.0, 528.0, SC_MM, catChinese);
	addPageSize(prefix + "D3", "D3", 264.0, 375.0, SC_MM, catChinese);
	addPageSize(prefix + "D4", "D4", 188.0, 260.0, SC_MM, catChinese);
	addPageSize(prefix + "D5", "D5", 130.0, 184.0, SC_MM, catChinese);
	addPageSize(prefix + "D6", "D6", 92.0, 126.0, SC_MM, catChinese);
	addPageSize(prefix + "RD0", "RD0", 787.0, 1092.0, SC_MM, catChinese);
	addPageSize(prefix + "RD1", "RD1", 546.0, 787.0, SC_MM, catChinese);
	addPageSize(prefix + "RD2", "RD2", 393.0, 546.0, SC_MM, catChinese);
	addPageSize(prefix + "RD3", "RD3", 273.0, 393.0, SC_MM, catChinese);
	addPageSize(prefix + "RD4", "RD4", 196.0, 273.0, SC_MM, catChinese);
	addPageSize(prefix + "RD5", "RD5", 136.0, 196.0, SC_MM, catChinese);
	addPageSize(prefix + "RD6", "RD6", 98.0, 136.0, SC_MM, catChinese);

	// Colombian
	// https://papersizes.io/colombian/
	PageSizeInfo::Category catColombian = PageSizeInfo::Colombian;
	prefix = "CO_";
	addPageSize(prefix + "Carta", "Carta", 216.0, 279.0, SC_MM, catColombian);
	addPageSize(prefix + "Extra Tabloide", "Extra Tabloide", 304.0, 457.2, SC_MM, catColombian);
	addPageSize(prefix + "Oficio", "Oficio", 216.0, 330.0, SC_MM, catColombian);
	addPageSize(prefix + "Pliego", "Pliego", 700.0, 1000.0, SC_MM, catColombian);
	addPageSize(prefix + "1/2 Pliego", "1/2 Pliego", 500.0, 700.0, SC_MM, catColombian);
	addPageSize(prefix + "1/4 Pliego", "1/4 Pliego", 350.0, 500.0, SC_MM, catColombian);
	addPageSize(prefix + "1/8 Pliego", "1/8 Pliego", 250.0, 350.0, SC_MM, catColombian);

	// French
	// https://papersizes.io/french/	
	PageSizeInfo::Category catFrench = PageSizeInfo::French;
	prefix = "FR_";
	addPageSize(prefix + "Cloche", "Cloche", 300.0, 400.0, SC_MM, catFrench);
	addPageSize(prefix + "Pot ecolier", "Pot, écolier", 310.0, 400.0, SC_MM, catFrench);
	addPageSize(prefix + "Telliere", "Tellière", 340.0, 440.0, SC_MM, catFrench);
	addPageSize(prefix + "Couronne ecriture", "Couronne écriture", 360.0, 360.0, SC_MM, catFrench);
	addPageSize(prefix + "Couronne edition", "Couronne édition", 370.0, 470.0, SC_MM, catFrench);
	addPageSize(prefix + "Roberto", "Roberto", 390.0, 500.0, SC_MM, catFrench);
	addPageSize(prefix + "Ecu", "Écu", 400.0, 520.0, SC_MM, catFrench);
	addPageSize(prefix + "Coquille", "Coquille", 440.0, 560.0, SC_MM, catFrench);
	addPageSize(prefix + "Carre", "Carré", 450.0, 560.0, SC_MM, catFrench);
	addPageSize(prefix + "Cavalier", "Cavalier", 460.0, 620.0, SC_MM, catFrench);
	addPageSize(prefix + "Demi-raisin", "Demi-raisin", 325.0, 500.0, SC_MM, catFrench);
	addPageSize(prefix + "Raisin", "Raisin", 500.0, 650.0, SC_MM, catFrench);
	addPageSize(prefix + "Double Raisin", "Double Raisin", 650.0, 1000.0, SC_MM, catFrench);
	addPageSize(prefix + "Jesus", "Jésus", 560.0, 760.0, SC_MM, catFrench);
	addPageSize(prefix + "Soleil", "Soleil", 600.0, 800.0, SC_MM, catFrench);
	addPageSize(prefix + "Colombier affiche", "Colombier affiche", 600.0, 800.0, SC_MM, catFrench);
	addPageSize(prefix + "Colombier commercial", "Colombier commercial", 630.0, 900.0, SC_MM, catFrench);
	addPageSize(prefix + "Petit Aigle", "Petit Aigle", 700.0, 940.0, SC_MM, catFrench);
	addPageSize(prefix + "Grand Aigle", "Grand Aigle", 750.0, 1050.0, SC_MM, catFrench);
	addPageSize(prefix + "Grand Monde", "Grand Monde", 900.0, 1260.0, SC_MM, catFrench);
	addPageSize(prefix + "Univers", "Univers", 1000.0, 1130.0, SC_MM, catFrench);

	// German
	// https://papersizes.io/german/
	// https://www.saxoprint.de/blog/papier/papierformate
	PageSizeInfo::Category catGerman = PageSizeInfo::German;
	prefix = "DE_";
	addPageSize(prefix + "DIN D00", "DIN D0", 771.0, 1090.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D01", "DIN D1", 545.0, 771.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D02", "DIN D2", 385.0, 545.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D03", "DIN D3", 272.0, 385.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D04", "DIN D4", 192.0, 272.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D05", "DIN D5", 136.0, 192.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D06", "DIN D6", 96.0, 136.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D07", "DIN D7", 68.0, 96.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D08", "DIN D8", 48.0, 68.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D09", "DIN D9", 34.0, 48.0, SC_MM, catGerman);
	addPageSize(prefix + "DIN D10", "DIN D10", 24.0, 34.0, SC_MM, catGerman);

	// Imperial
	// https://papersdb.com/imperial
	// https://en.wikipedia.org/wiki/Paper_size#Traditional_inch-based_paper_sizes
	PageSizeInfo::Category catImperial = PageSizeInfo::Imperial;
	prefix = "Imp_";
	addPageSize(prefix + "Quarto", QObject::tr("Quarto"), 8.0, 10.0, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Foolscap", QObject::tr("Foolscap"), 8.0, 13.0, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Executive", QObject::tr("Executive"), 7.25, 10.5, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Post", QObject::tr("Post"), 15.5, 19.25, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Crown", QObject::tr("Crown"), 15.0, 20.0, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Large Post", QObject::tr("Large Post"), 16.5, 21.0, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Demy", QObject::tr("Demy"), 17.5, 22.5, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Medium", QObject::tr("Medium"), 18.0, 23.0, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Royal", QObject::tr("Royal"), 20.0, 25.0, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Elephant", QObject::tr("Elephant"), 23.0, 28.0, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Double Demy", QObject::tr("Double Demy"), 22.5, 35.0, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "Quad Demy", QObject::tr("Quad Demy"), 35.0, 45.0, SC_IN, catImperial); // from 1.6.2
	addPageSize(prefix + "STMT", QObject::tr("STMT"), 5.5, 8.5, SC_IN, catImperial); // from 1.6.2

	// ISO A series
	// https://papersizes.io/a/	
	PageSizeInfo::Category catIsoA = PageSizeInfo::IsoA;
	prefix = "IsoA_";
	addPageSize(prefix + "2A0", "2A0", 1189.0, 1682.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "4A0", "4A0", 1682.0, 2378.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A00+", "A0+", 914.0, 1292.0, SC_MM, catIsoA);
	addPageSize(prefix + "A00", "A0", 841.0, 1189.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A01+", "A1+", 609.0, 914.0, SC_MM, catIsoA);
	addPageSize(prefix + "A01", "A1", 594.0, 841.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A02", "A2", 420.0, 594.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A03+", "A3+", 329.0, 483.0, SC_MM, catIsoA);
	addPageSize(prefix + "A03", "A3", 297.0, 420.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A04", "A4", 210.0, 297.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A05", "A5", 148.0, 210.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A06", "A6", 105.0, 148.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A07", "A7", 74.0, 105.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A08", "A8", 52.0, 74.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A09", "A9", 37.0, 52.0, SC_MM, catIsoA); // from 1.6.2
	addPageSize(prefix + "A10", "A10", 26.0, 37.0, SC_MM, catIsoA); // from 1.6.2

	// ISO B series
	// https://papersizes.io/b/	
	PageSizeInfo::Category catIsoB = PageSizeInfo::IsoB;
	prefix = "IsoB_";
	addPageSize(prefix + "B00+", "B0+", 1118.0, 1580.0, SC_MM, catIsoB);
	addPageSize(prefix + "B00", "B0", 1000.0, 1414.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B01+", "B1+", 720.0, 1020.0, SC_MM, catIsoB);
	addPageSize(prefix + "B01", "B1", 707.0, 1000.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B02+", "B2+", 520.0, 720.0, SC_MM, catIsoB);
	addPageSize(prefix + "B02", "B2", 500.0, 707.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B03", "B3", 353.0, 500.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B04", "B4", 250.0, 353.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B05", "B5", 176.0, 250.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B06", "B6", 125.0, 176.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B07", "B7", 88.0, 125.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B08", "B8", 62.0, 88.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B09", "B9", 44.0, 62.0, SC_MM, catIsoB); // from 1.6.2
	addPageSize(prefix + "B10", "B10", 31.0, 44.0, SC_MM, catIsoB); // from 1.6.2

	// ISO C series
	// https://papersizes.io/c-envelope/	
	PageSizeInfo::Category catIsoC = PageSizeInfo::IsoC;
	prefix = "IsoC_";
	addPageSize(prefix + "C00", "C0", 917.0, 1297.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C01", "C1", 648.0, 917.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C02", "C2", 458.0, 648.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C03", "C3", 324.0, 458.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C04", "C4", 229.0, 324.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C05", "C5", 162.0, 229.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C06", "C6", 114.0, 162.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C07", "C7", 81.0, 114.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C08", "C8", 57.0, 81.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C09", "C9", 40.0, 57.0, SC_MM, catIsoC); // from 1.6.2
	addPageSize(prefix + "C10", "C10", 28.0, 40.0, SC_MM, catIsoC); // from 1.6.2

	// ISO Envelopes
	// https://papersizes.io/international-envelope/	
	PageSizeInfo::Category catIsoEnvelope = PageSizeInfo::IsoEnvelope;
	prefix = "IsoEnv_";
	addPageSize(prefix + "DL/E65", "DL, E65", 110.0, 220.0, SC_MM, catIsoEnvelope); // from 1.6.2
	// addPageSize(prefix + "B4", "B4", 250.0, 353.0, SC_MM, catIsoEnvelope);
	// addPageSize(prefix + "B5", "B5", 176.0, 250.0, SC_MM, catIsoEnvelope);
	// addPageSize(prefix + "B6", "B6", 125.0, 176.0, SC_MM, catIsoEnvelope);
	// addPageSize(prefix + "C3", "C3", 324.0, 458.0, SC_MM, catIsoEnvelope);
	// addPageSize(prefix + "C4", "C4", 229.0, 324.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "C4M", "C4M", 229.0, 318.0, SC_MM, catIsoEnvelope);
	// addPageSize(prefix + "C5", "C5", 162.0, 229.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "C6/C5", "C6/C5", 114.0, 229.0, SC_MM, catIsoEnvelope);
	// addPageSize(prefix + "C6", "C6", 114.0, 162.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "C64M", "C64M", 114.0, 318.0, SC_MM, catIsoEnvelope);
	// addPageSize(prefix + "C7", "C7", 81.0, 114.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "C7/C6", "C7/C6", 81.0, 162.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "B6/C4", "B6/C4", 125.0, 324.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "E4", "E4", 220.0, 312.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "E5", "E5", 115.0, 220.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "E56", "E56", 115.0, 115.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "E6", "E6", 110.0, 155.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "EC45", "EC45", 220.0, 229.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "EC5", "EC5", 155.0, 229.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "R7", "R7", 120.0, 135.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "S4", "S4", 250.0, 330.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "S5", "S5", 185.0, 255.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "S65", "S65", 110.0, 225.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "X5", "X5", 105.0, 216.0, SC_MM, catIsoEnvelope);
	addPageSize(prefix + "EX5", "EX5", 115.0, 216.0, SC_MM, catIsoEnvelope);

	// Japanese
	// https://papersizes.io/japanese/	
	PageSizeInfo::Category catJapanese = PageSizeInfo::Japanese;
	prefix = "JP_";
	addPageSize(prefix + "JB00", "JB0", 1030.0, 1456.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB01", "JB1", 728.0, 1030.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB02", "JB2", 515.0, 728.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB03", "JB3", 364.0, 515.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB04", "JB4", 257.0, 364.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB05", "JB5", 182.0, 257.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB06", "JB6", 128.0, 182.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB07", "JB7", 91.0, 128.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB08", "JB8", 64.0, 91.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB09", "JB9", 45.0, 64.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB10", "JB10", 32.0, 45.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB11", "JB11", 22.0, 32.0, SC_MM, catJapanese);
	addPageSize(prefix + "JB12", "JB12", 16.0, 22.0, SC_MM, catJapanese);
	addPageSize(prefix + "Shiroku ban 4", "Shiroku ban 4", 264.0, 379.0, SC_MM, catJapanese);
	addPageSize(prefix + "Shiroku ban 5", "Shiroku ban 5", 189.0, 262.0, SC_MM, catJapanese);
	addPageSize(prefix + "Shiroku ban 6", "Shiroku ban 6", 127.0, 188.0, SC_MM, catJapanese);
	addPageSize(prefix + "Kiku 4", "Kiku 4", 227.0, 306.0, SC_MM, catJapanese);
	addPageSize(prefix + "Kiku 5", "Kiku 5", 151.0, 227.0, SC_MM, catJapanese);

	// Newspaper
	// https://papersizes.io/newspaper/
	// https://en.wikipedia.org/wiki/Broadsheet
	// https://www.papersizes.org/
	PageSizeInfo::Category catNewspaper = PageSizeInfo::Newspaper;
	prefix = "News_";
	// Broadsheet
	addPageSize(prefix + "AU/NZ Broadsheet", QObject::tr("AU/NZ Broadsheet"), 420.0, 594.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Berliner Broadsheet", QObject::tr("Berliner Broadsheet"), 315.0, 470.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Ciner", QObject::tr("Ciner"), 350.0, 500.0, SC_MM, catNewspaper);
	addPageSize(prefix + "New York Times", QObject::tr("New York Times"), 12.0, 22.0, SC_IN, catNewspaper);
	addPageSize(prefix + "Nordisch Broadsheet", QObject::tr("Nordisch Broadsheet"), 400.0, 570.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Rhenish Broadsheet", QObject::tr("Rhenish Broadsheet"), 350.0, 520.0, SC_MM, catNewspaper);
	addPageSize(prefix + "SA Broadsheet", QObject::tr("SA Broadsheet"), 410.0, 578.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Swiss Broadsheet", QObject::tr("Swiss Broadsheet"), 320.0, 475.0, SC_MM, catNewspaper);
	addPageSize(prefix + "UK Broadsheet", QObject::tr("UK Broadsheet"), 375.0, 597.0, SC_MM, catNewspaper);
	addPageSize(prefix + "US Broadsheet", QObject::tr("US Broadsheet"), 15.0, 22.75, SC_IN, catNewspaper);
	addPageSize(prefix + "Wall Street Journal", QObject::tr("Wall Street Journal"), 12.0, 22.75, SC_IN, catNewspaper);
	// Tabloids
	addPageSize(prefix + "Berliner Tabloid", QObject::tr("Berliner Tabloid"), 235.0, 315.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Canadian Tabloid", QObject::tr("Canadian Tabloid"), 260.0, 368.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Canadian Tall Tabloid", QObject::tr("Canadian Tall Tabloid"), 260.0, 413.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Nordisch Tabloid", QObject::tr("Nordisch Tabloid"), 285.0, 400.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Norwegian Tabloid", QObject::tr("Norwegian Tabloid"), 280.0, 400.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Rhenish Tabloid", QObject::tr("Rhenish Tabloid"), 260.0, 350.0, SC_MM, catNewspaper);
	addPageSize(prefix + "UK Tabloid", QObject::tr("UK Tabloid"), 280.0, 430.0, SC_MM, catNewspaper);
	addPageSize(prefix + "Compact", QObject::tr("Compact"), 280.0, 430.0, SC_MM, catNewspaper);

	// Swedish
	// https://papersizes.io/swedish/
	PageSizeInfo::Category catSwedish = PageSizeInfo::Swedish;
	prefix = "SE_";
	addPageSize(prefix + "SIS D00", "SIS D0", 1091.0, 1542.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D01", "SIS D1", 771.0, 1091.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D02", "SIS D2", 545.0, 771.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D03", "SIS D3", 386.0, 545.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D04", "SIS D4", 273.0, 386.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D05", "SIS D5", 193.0, 273.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D06", "SIS D6", 136.0, 193.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D07", "SIS D7", 96.0, 136.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D08", "SIS D8", 68.0, 96.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D09", "SIS D9", 48.0, 68.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS D10", "SIS D10", 34.0, 48.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E00", "SIS E0", 878.0, 1242.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E01", "SIS E1", 621.0, 878.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E02", "SIS E2", 439.0, 621.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E03", "SIS E3", 310.0, 439.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E04", "SIS E4", 220.0, 310.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E05", "SIS E5", 155.0, 220.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E06", "SIS E6", 110.0, 155.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E07", "SIS E7", 78.0, 110.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E08", "SIS E8", 55.0, 78.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E09", "SIS E9", 39.0, 55.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS E10", "SIS E10", 27.0, 39.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F00", "SIS F0", 958.0, 1354.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F01", "SIS F1", 677.0, 958.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F02", "SIS F2", 479.0, 677.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F03", "SIS F3", 339.0, 479.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F04", "SIS F4", 239.0, 339.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F05", "SIS F5", 169.0, 239.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F06", "SIS F6", 120.0, 169.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F07", "SIS F7", 85.0, 120.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F08", "SIS F8", 60.0, 85.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F09", "SIS F9", 42.0, 60.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS F10", "SIS F10", 30.0, 42.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G00", "SIS G0", 1044.0, 1477.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G01", "SIS G1", 738.0, 1044.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G02", "SIS G2", 522.0, 738.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G03", "SIS G3", 369.0, 522.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G04", "SIS G4", 261.0, 369.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G05", "SIS G5", 185.0, 261.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G06", "SIS G6", 131.0, 185.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G07", "SIS G7", 92.0, 131.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G08", "SIS G8", 65.0, 92.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G09", "SIS G9", 46.0, 65.0, SC_MM, catSwedish);
	addPageSize(prefix + "SIS G10", "SIS G10", 33.0, 46.0, SC_MM, catSwedish);

	// Transitional
	PageSizeInfo::Category catTransitional = PageSizeInfo::Transitional;
	prefix = "Trans_";
	addPageSize(prefix + "F00", "F0", 841.0, 1321.0, SC_MM, catTransitional);
	addPageSize(prefix + "F01", "F1", 660.0, 841.0, SC_MM, catTransitional);
	addPageSize(prefix + "F02", "F2", 420.0, 660.0, SC_MM, catTransitional);
	addPageSize(prefix + "F03", "F3", 330.0, 420.0, SC_MM, catTransitional);
	addPageSize(prefix + "F04", "F4", 210.0, 330.0, SC_MM, catTransitional);
	addPageSize(prefix + "F05", "F5", 165.0, 210.0, SC_MM, catTransitional);
	addPageSize(prefix + "F06", "F6", 105.0, 165.0, SC_MM, catTransitional);
	addPageSize(prefix + "F07", "F7", 82.0, 105.0, SC_MM, catTransitional);
	addPageSize(prefix + "F08", "F8", 52.0, 82.0, SC_MM, catTransitional);
	addPageSize(prefix + "F09", "F9", 41.0, 52.0, SC_MM, catTransitional);
	addPageSize(prefix + "F10", "F10", 26.0, 41.0, SC_MM, catTransitional);
	addPageSize(prefix + "PA00", "PA0", 840.0, 1120.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA01", "PA1", 560.0, 840.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA02", "PA2", 420.0, 560.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA03", "PA3", 280.0, 420.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA04", "PA4", 210.0, 280.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA05", "PA5", 140.0, 210.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA06", "PA6", 105.0, 140.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA07", "PA7", 70.0, 105.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA08", "PA8", 52.0, 70.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA09", "PA9", 35.0, 52.0, SC_MM, catTransitional); // from 1.6.2
	addPageSize(prefix + "PA10", "PA10", 26.0, 35.0, SC_MM, catTransitional); // from 1.6.2


	// US Paper Size
	// https://papersizes.io/us/
	PageSizeInfo::Category catUSStandard = PageSizeInfo::USStandard;
	prefix = "US_";
	addPageSize(prefix + "Government Legal", "Government Legal", 8.5, 13.0, SC_IN, catUSStandard);
	addPageSize(prefix + "Legal", "Legal", 8.5, 14.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Legal Half", "Legal Half", 7.0, 8.5, SC_IN, catUSStandard);
	addPageSize(prefix + "Junior Legal", "Junior Legal", 5.0, 8.0, SC_IN, catUSStandard);
	addPageSize(prefix + "Letter", "Letter", 8.5, 11.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Letter Half", "Letter Half", 5.5, 8.5, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Government Letter", "Government Letter", 8.0, 10.5, SC_IN, catUSStandard); // from 1.6.2	
	addPageSize(prefix + "Ledger", "Ledger, Tabloid", 11.0, 17.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "ANSI A", "ANSI A", 8.5, 11.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "ANSI B", "ANSI B", 11.0, 17.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "ANSI C", "ANSI C", 17.0, 22.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "ANSI D", "ANSI D", 22.0, 34.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "ANSI E", "ANSI E", 34.0, 44.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Arch A", "Arch A", 9.0, 12.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Arch B", "Arch B", 12.0, 18.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Arch C", "Arch C", 18.0, 24.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Arch D", "Arch D", 24.0, 36.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Arch E", "Arch E", 36.0, 48.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Arch E1", "Arch E1", 30.0, 42.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Arch E2", "Arch E2", 26.0, 38.0, SC_IN, catUSStandard); // from 1.6.2
	addPageSize(prefix + "Arch E3", "Arch E3", 27.0, 39.0, SC_IN, catUSStandard); // from 1.6.2

	// US Common Press
	// https://papersdb.com/common-us-press-sheet
	PageSizeInfo::Category catUSPress = PageSizeInfo::USPress;
	prefix = "USPress_";
	addPageSize(prefix + "11x17", "11x17", 11.0, 17.0, SC_IN, catUSPress); // from 1.6.2
	addPageSize(prefix + "12x18", "12x18", 12.0, 18.0, SC_IN, catUSPress);
	addPageSize(prefix + "17x22", "17x22", 17.0, 22.0, SC_IN, catUSPress);
	addPageSize(prefix + "19x25", "19x25", 19.0, 25.0, SC_IN, catUSPress);
	addPageSize(prefix + "20x26", "20x26", 20.0, 26.0, SC_IN, catUSPress);
	addPageSize(prefix + "23x29", "23x29", 23.0, 29.0, SC_IN, catUSPress);
	addPageSize(prefix + "23x35", "23x35", 23.0, 35.0, SC_IN, catUSPress);
//	addPageSize(prefix + "24x36", "24x36", 24.0, 36.0, SC_IN, catUSPress);
	addPageSize(prefix + "25x38", "25x38", 25.0, 38.0, SC_IN, catUSPress);
	addPageSize(prefix + "26x40", "26x40", 26.0, 40.0, SC_IN, catUSPress);
	addPageSize(prefix + "28x40", "28x40", 28.0, 40.0, SC_IN, catUSPress);
	addPageSize(prefix + "35x45", "38x45", 35.0, 45.0, SC_IN, catUSPress);
	addPageSize(prefix + "38x50", "38x50", 38.0, 50.0, SC_IN, catUSPress);

	// US Envelope
	// https://papersdb.com/us-baronial-envelope
	// https://www.wsel.com/envelopes/standard-sizes
	PageSizeInfo::Category catUSEnvelope = PageSizeInfo::USEnvelope;
	prefix = "USEnv_";
	addPageSize(prefix + "A01", "A-1", 3.625, 5.125, SC_IN, catUSEnvelope);
	addPageSize(prefix + "A02", "A-2 (Lady Grey)", 4.375, 5.75, SC_IN, catUSEnvelope);
	addPageSize(prefix + "A04", "A-4", 4.25, 6.25, SC_IN, catUSEnvelope);
	addPageSize(prefix + "A06", "A-6 (Thompson Standard)", 4.75, 6.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "A07", "A-7 (Besselheim)", 5.25, 7.25, SC_IN, catUSEnvelope);
	addPageSize(prefix + "A08", "A-8 (Carrs)", 5.5, 8.125, SC_IN, catUSEnvelope);
	addPageSize(prefix + "A09", "A-9 (Diplomat)", 5.75, 8.75, SC_IN, catUSEnvelope);
	addPageSize(prefix + "A10", "A-10 (Willow)", 6.0, 9.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Baronial 04", "Baronial 4", 3.625, 5.125, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Baronial 05 1/2", "Baronial 5 1/2", 4.375, 5.75, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Baronial 06", "Baronial 6", 4.75, 6.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Baronial Lee", "Lee", 5.25, 7.25, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 03", "Booklet 3", 4.75, 6.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 04 1/2", "Booklet 4 1/2", 5.5, 7.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 06", "Booklet 6", 5.75, 8.875, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 06 1/2", "Booklet 6 1/2", 6.0, 9.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 06 7/8", "Booklet 6 7/8", 6.0, 9.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 06 3/4", "Booklet 6 3/4", 6.5, 9.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 07 1/2", "Booklet 7 1/2", 7.5, 10.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 09", "Booklet 9", 8.75, 11.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 09 1/2", "Booklet 9 1/2", 9.0, 12.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 10", "Booklet 10", 9.5, 12.625, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Booklet 13", "Booklet 13", 10.0, 13.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 01", "Catalog 1", 6.0, 9.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 01 3/4", "Catalog 1 3/4", 6.5, 9.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 03", "Catalog 3", 7.0, 10.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 06", "Catalog 6", 7.5, 10.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 08", "Catalog 8", 8.25, 11.25, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 09 3/4", "Catalog 9 3/4", 8.75, 11.25, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 10 1/2", "Catalog 10 1/2", 9.0, 12.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 12 1/5", "Catalog 12 1/5", 9.5, 12.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 13 1/2", "Catalog 13 1/2", 10.0, 13.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 14 1/2", "Catalog 14 1/2", 11.5, 14.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 15", "Catalog 15", 10.0, 15.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Catalog 15 1/2", "Catalog 15 1/2", 12.0, 15.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 05", "Commerical 5", 3.125, 5.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 06 1/4", "Commerical 6 1/4", 3.5, 6.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 06 3/4", "Commerical 6 3/4", 3.625, 6.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 07", "Commerical 7", 3.75, 6.75, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 07 1/2", "Commerical 7 1/2", 3.9375, 7.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 08 5/8", "Commerical 8 5/8", 3.625, 8.625, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 09", "Commerical 9", 3.875, 8.875, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 10", "Commerical 10", 4.125, 9.5, SC_IN, catUSEnvelope); // from 1.6.2
	addPageSize(prefix + "Commerical 11", "Commerical 11", 4.5, 10.375, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 12", "Commerical 12", 4.75, 11, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Commerical 14", "Commerical 14", 5.0, 11.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Square 06", "Square 6", 6.0, 6.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Square 06 1/2", "Square 6 1/2", 6.5, 6.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Square 07", "Square 7", 7.0, 7.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Square 07 1/2", "Square 7 1/2", 7.5, 7.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Square 08", "Square 8", 8.0, 8.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Square 08 1/2", "Square 8 1/2", 8.5, 8.5, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Square 09", "Square 9", 9.0, 9.0, SC_IN, catUSEnvelope);
	addPageSize(prefix + "Square 09 1/2", "Square 9 1/2", 9.5, 9.5, SC_IN, catUSEnvelope);

	// Other Formats
	// https://www.template.net/graphic-design/album-cover-sizes/
	// https://print.dvdcover.com/dvd-blu-ray-cover-size.php
	// https://www.duplication.com/printspecs/k7insertspecs.htm
	// https://tapemuzik.de/shop/paper-products/cover-without-flap/?lang=en
	PageSizeInfo::Category catOther = PageSizeInfo::Other;
	prefix = "Other_";
	addPageSize(prefix + "Blu-Ray Cover (5 mm)", QObject::tr("Blu-Ray Cover (5 mm)"), 148.0, 257.0, SC_MM, catOther);
	addPageSize(prefix + "Blu-Ray Cover (12 mm)", QObject::tr("Blu-Ray Cover (12 mm)"), 148.0, 264.0, SC_MM, catOther);
	addPageSize(prefix + "Blu-Ray Cover (14 mm)", QObject::tr("Blu-Ray Cover (14 mm)"), 148.0, 266.0, SC_MM, catOther);
	addPageSize(prefix + "Blu-Ray Cover (24 mm)", QObject::tr("Blu-Ray Cover (24 mm)"), 148.0, 276.0, SC_MM, catOther);
	addPageSize(prefix + "Cassette Cover (J-Card)", QObject::tr("Cassette Cover (J-Card)"), 101.5, 105.0, SC_MM, catOther);
	addPageSize(prefix + "Cassette Cover (U-Card)", QObject::tr("Cassette Cover (U-Card)"), 101.5, 140.9, SC_MM, catOther);
	addPageSize(prefix + "Compact Disc (Front)", QObject::tr("Compact Disc (Front)"), 120.0, 120.0, SC_MM, catOther); // from 1.6.2
	addPageSize(prefix + "Compact Disc (Back)", QObject::tr("Compact Disc (Back)"), 118.0, 151.0, SC_MM, catOther);
	addPageSize(prefix + "Compact Disc (Front Double)", QObject::tr("Compact Disc (Front Double)"), 120.0, 240.0, SC_MM, catOther);
	addPageSize(prefix + "Vinyl LP", QObject::tr("Vinyl LP"), 314.3, 314.3, SC_MM, catOther);
	addPageSize(prefix + "DVD Cover Normal", QObject::tr("DVD Cover Normal"), 183.0, 273.0, SC_MM, catOther);
	addPageSize(prefix + "DVD Cover Slim", QObject::tr("DVD Cover Slim"), 183.0, 266.0, SC_MM, catOther);
}

void PageSize::printSizeList() const
{
	for (auto it = m_pageSizeList.begin(); it != m_pageSizeList.end(); ++it)
	{
		std::cout << it.key().leftJustified(6).toStdString() << ": ";
		std::cout << it.value().width << " x " << it.value().height << ",  ";
		std::cout << it.value().width * unitGetRatioFromIndex(it.value().pageUnitIndex) << " x " << it.value().height * unitGetRatioFromIndex(it.value().pageUnitIndex) << ",  ";
		std::cout << it.value().trSizeName.toStdString() << categoryToString(it.value().category).toStdString() << std::endl;
	}
}

