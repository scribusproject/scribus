/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PAGESIZESELECTOR_H
#define PAGESIZESELECTOR_H

#include <QComboBox>
#include <QWidget>

#include "pagesize.h"

class PageSizeSelector : public QWidget
{
	Q_OBJECT

public:
	explicit PageSizeSelector(QWidget *parent = nullptr);

	void setPageSize(QString name);
	void setHasFormatSelector(bool isVisble );
	void setHasCustom(bool hasCustom);
	bool hasCustom() const { return m_hasCustom; };
	bool hasFormatSelector() const { return m_hasFormatSelector; };
	void setCurrentCategory(PageSizeInfo::Category category);

	PageSizeInfo::Category category() const { return m_sizeCategory; };
	QString pageSize() const { return m_sizeName; };
	QString pageSizeTR() const { return m_trSizeName; };

private:
	QComboBox* comboCategory { nullptr };
	QComboBox* comboFormat { nullptr };

	QString m_sizeName;
	QString m_trSizeName;
	PageSizeInfo::Category m_sizeCategory;
	bool m_hasFormatSelector {true};
	bool m_hasCustom {true};

	void setFormat(PageSizeInfo::Category category, QString name);

signals:
	void pageSizeChanged(QString);
	void pageCategoryChanged(PageSizeInfo::Category);

private slots:
	void categorySelected(int index);
	void formatSelected(int index);

};

#endif // PAGESIZESELECTOR_H
