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
 
#ifndef PAGESIZE_H
#define PAGESIZE_H

#include <QMap>
#include <QString>
#include <QSize>
#include "scribusapi.h"
#include "units.h"

struct PageSizeInfo
{
	enum Category {

		Custom = 0, // don't use for presets, it is reserved for custom sizes
		Preferred = 1, // don't use for presets, it is reserved for user favorite sizes

		IsoA = 10,
		IsoB = 11,
		IsoC = 12,
		IsoEnvelope = 13,

		USStandard = 20,
		USPress = 21,
		USEnvelope = 22,

		Book = 30,
		BusinessCards = 31,
		Newspaper = 32,
		Transitional = 33,

		Other = 40,

		Canadian = 50,
		Chinese = 51,
		Colombian = 52,
		French = 53,
		German = 54,
		Imperial = 55,
		Japanese = 56,
		Swedish = 57,
	};

	double width;
	double height;
	QString trSizeName;
	QString sizeName;
	QString sizeLabel;
	int pageUnitIndex;
	Category category;
};

using PageSizeInfoMap = QMap<QString, PageSizeInfo>;
using PageSizeCategoriesMap = QMap<PageSizeInfo::Category, QString>;

class SCRIBUS_API PageSize
{
public:
	PageSize(const QString&);
	PageSize(double, double);
	PageSize& operator=(const PageSize& other);

	void init(const QString&);
	const QString& name() const { return m_pageSizeName; }
	const QString& nameTR() const { return m_trPageSizeName; }
	PageSizeInfo::Category category() const { return m_category; };
	QString categoryToString(PageSizeInfo::Category category) const;
	double width() const { return m_width; }
	double height() const { return m_height; }
	double originalWidth() const { return m_width * unitGetRatioFromIndex(m_pageUnitIndex); }
	double originalHeight() const { return m_height * unitGetRatioFromIndex(m_pageUnitIndex); }
	QString originalUnit() const { return unitGetSuffixFromIndex(m_pageUnitIndex); }
	static QStringList defaultSizesList();
	PageSizeCategoriesMap categories() const;
	PageSizeInfoMap sizesByCategory(PageSizeInfo::Category category) const;
	PageSizeInfoMap sizesByDimensions(QSize sizePt) const;
	PageSizeInfoMap activePageSizes() const;
	const PageSizeInfoMap& pageSizes() const { return m_pageSizeList; };
	void printSizeList() const;

private:
	PageSizeInfoMap m_pageSizeList;
	double m_width { 0.0 };
	double m_height { 0.0 };
	int m_pageUnitIndex { -1 };
	QString m_pageSizeName;
	QString m_trPageSizeName;
	PageSizeInfo::Category m_category {PageSizeInfo::Custom};

	void generateSizeList();
	void addPageSize(const QString id, double width, double height, int unitIndex, PageSizeInfo::Category category);
	void addPageSize(const QString id, const QString name, double width, double height, int unitIndex, PageSizeInfo::Category category);
};

#endif

