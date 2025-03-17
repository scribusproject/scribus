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

#include <QVBoxLayout>

#include "pagesizeselector.h"
#include "pagesize.h"
#include "commonstrings.h"

PageSizeSelector::PageSizeSelector(QWidget *parent)
	: QWidget{parent}
{
	comboCategory = new QComboBox();
	comboFormat = new QComboBox();

	QVBoxLayout* lytMain = new QVBoxLayout();
	lytMain->setContentsMargins(0, 0, 0, 0);
	lytMain->setSpacing(4);
	lytMain->addWidget(comboCategory);
	lytMain->addWidget(comboFormat);
	this->setLayout(lytMain);

	connect(comboCategory, &QComboBox::currentIndexChanged, this, &PageSizeSelector::categorySelected);
	connect(comboFormat, &QComboBox::currentIndexChanged, this, &PageSizeSelector::formatSelected);
}


void PageSizeSelector::setHasFormatSelector(bool isVisble)
{
	m_hasFormatSelector = isVisble;
	comboFormat->setVisible(isVisble);
}

void PageSizeSelector::setHasCustom(bool hasCustom)
{
	m_hasCustom = hasCustom;

	if (!m_sizeName.isEmpty())
		setPageSize(m_sizeName);
}

void PageSizeSelector::setCurrentCategory(PageSizeInfo::Category category)
{
	int index = comboCategory->findData(category);
	if (index != -1)
		comboCategory->setCurrentIndex(index);
}

void PageSizeSelector::setPageSize(QString name)
{
	PageSize ps(name);

	m_sizeName = ps.name();
	m_sizeCategory = ps.category();
	m_trSizeName = ps.nameTR();

	// Load category list
	int index = -1;
	QSignalBlocker sigCat(comboCategory);
	comboCategory->clear();

	// Add Custom
	if (hasCustom())
	{
		comboCategory->addItem(CommonStrings::trCustomPageSize, PageSizeInfo::Custom);
		if (m_sizeName == CommonStrings::customPageSize || m_sizeName == CommonStrings::trCustomPageSize)
			index = comboCategory->count() - 1;
	}

	// Add Preferred
	comboCategory->addItem(ps.categoryToString(PageSizeInfo::Preferred), PageSizeInfo::Preferred);

	// Add all available categories
	PageSizeCategoriesMap categories = ps.categories();
	for (auto it = categories.begin(); it != categories.end(); ++it)
	{
		comboCategory->addItem(it.value(), it.key());
		if (it.key() == m_sizeCategory)
			index = comboCategory->count() - 1;			
	}

	comboCategory->setCurrentIndex(index);
	comboCategory->insertSeparator(comboCategory->findData(PageSizeInfo::Preferred) + 1);
	comboCategory->insertSeparator(comboCategory->findData(PageSizeInfo::IsoEnvelope) + 1);
	comboCategory->insertSeparator(comboCategory->findData(PageSizeInfo::USEnvelope) + 1);
	comboCategory->insertSeparator(comboCategory->findData(PageSizeInfo::Other) + 1);

	// Load size format list
	setFormat(m_sizeCategory, m_sizeName);
}

void PageSizeSelector::setFormat(PageSizeInfo::Category category, QString name)
{
	if (!hasFormatSelector())
		return;

	QSignalBlocker sigFormat(comboFormat);
	comboFormat->clear();

	if (category == PageSizeInfo::Custom)
	{
		comboFormat->setEnabled(false);
		m_sizeName = CommonStrings::customPageSize;
		m_trSizeName = CommonStrings::trCustomPageSize;
		return;
	}
	else
	{
		comboFormat->setEnabled(true);
	}

	PageSize ps(name);
	int index = -1;
	for (const auto &item : ps.pageSizes())
	{
		if (item.category == category || (category == PageSizeInfo::Preferred && ps.activePageSizes().contains(item.sizeName)))
		{
			comboFormat->addItem(item.trSizeName, item.sizeName);
			if (item.sizeName == name)
				index = comboFormat->count() - 1;
		}
	}

	if (index == -1)
		index = 0;

	comboFormat->setCurrentIndex(index);

	m_sizeName = comboFormat->currentData().toString();
	m_trSizeName = comboFormat->currentText();
}

void PageSizeSelector::categorySelected(int index)
{
	m_sizeCategory = static_cast<PageSizeInfo::Category>(comboCategory->itemData(index).toInt());

	setFormat(m_sizeCategory, m_sizeName);
	emit pageCategoryChanged(m_sizeCategory);
	emit pageSizeChanged(m_trSizeName);
}

void PageSizeSelector::formatSelected(int index)
{
	m_sizeName = comboFormat->itemData(index).toString();
	m_trSizeName = comboFormat->itemText(index);

	emit pageSizeChanged(m_trSizeName);
}
