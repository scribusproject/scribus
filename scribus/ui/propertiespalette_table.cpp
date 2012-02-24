/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QColor>
#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>

#include "colorcombo.h"
#include "commonstrings.h"
#include "pageitem_table.h"
#include "sccolorengine.h"
#include "scribus.h"
#include "selection.h"
#include "tableborder.h"
#include "tablesideselector.h"
#include "util.h"
#include "util_color.h"
#include "util_icon.h"

#include "propertiespalette_table.h"

PropertiesPalette_Table::PropertiesPalette_Table(QWidget* parent) : QWidget(parent),
	m_mainWindow(0), m_doc(0), m_item(0), m_previousItem(0)
{
	setupUi(this);

	addBorderLineButton->setIcon(QIcon(loadIcon("penciladd.png")));
	removeBorderLineButton->setIcon(QIcon(loadIcon("pencilsub.png")));
}

void PropertiesPalette_Table::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		updateColorList();
}

void PropertiesPalette_Table::updateColorList()
{
	if (!m_doc)
		return;

	borderLineColor->updateBox(m_doc->PageColors, ColorCombo::smallPixmaps);
	fillColor->updateBox(m_doc->PageColors, ColorCombo::smallPixmaps);
}

void PropertiesPalette_Table::setMainWindow(ScribusMainWindow* mainWindow)
{
	m_mainWindow = mainWindow;

	connect(m_mainWindow, SIGNAL(UpdateRequest(int)), SLOT(handleUpdateRequest(int)));
	connect(m_mainWindow, SIGNAL(AppModeChanged(int,int)), this, SLOT(updateFillControls()));
}

void PropertiesPalette_Table::setDocument(ScribusDoc *doc)
{
	m_doc = doc;
}

void PropertiesPalette_Table::unsetDocument()
{
	m_doc = 0;
}

void PropertiesPalette_Table::setItem(PageItem* item)
{
	m_item = item;
	if (item->isTable())
		connect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleCellSelectionChanged()));
}

void PropertiesPalette_Table::unsetItem()
{
	if ((m_item) && (m_item->isTable()))
		disconnect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleCellSelectionChanged()));
	m_item = 0;
}

void PropertiesPalette_Table::handleSelectionChanged()
{
	if (!m_doc)
		return;

	// We only handle a single item for now.
	if (m_doc->m_Selection->count() >= 1 && m_doc->m_Selection->itemAt(0)->isTable())
		m_item = m_doc->m_Selection->itemAt(0);
	else
		m_item = 0;

	// HACK: Guard against "false" re-selections resulting from m_item->update().
	if (m_item == m_previousItem)
		return;
	m_previousItem = m_item;

	sideSelector->setSelection(TableSideSelector::All);

	updateFillControls();
}

void PropertiesPalette_Table::handleCellSelectionChanged()
{
	if (!m_doc)
		return;
	if (!m_item)
		return;
	updateFillControls();
}

void PropertiesPalette_Table::on_sideSelector_selectionChanged()
{
	if (!m_item || !m_item->isTable())
		return;

	/*
	 * Figure out the selection state. Either
	 *
	 * 1) Some sides are selected and they all have the same border, or
	 * 2) Some sides are selected but they have different borders, or
	 * 3) No sides are selected.
	 */
	State borderState = Unset;
	m_currentBorder = TableBorder();
	TableSideSelector::Sides selectedSides = sideSelector->selection();
	PageItem_Table* table = m_item->asTable();

	if (selectedSides & TableSideSelector::Left)
	{
		if (borderState == Unset && !table->leftBorder().isNull())
		{
			m_currentBorder = table->leftBorder();
			borderState = Set;
		}
		else if (m_currentBorder != table->leftBorder())
			borderState = TriState;
	}

	if (selectedSides & TableSideSelector::Right)
	{
		if (borderState == Unset && !table->rightBorder().isNull())
		{
			m_currentBorder = table->rightBorder();
			borderState = Set;
		}
		else if (m_currentBorder != table->rightBorder())
			borderState = TriState;
	}

	if (selectedSides & TableSideSelector::Top)
	{
		if (borderState == Unset && !table->topBorder().isNull())
		{
			m_currentBorder = table->topBorder();
			borderState = Set;
		}
		else if (m_currentBorder != table->topBorder())
			borderState = TriState;
	}

	if (selectedSides & TableSideSelector::Bottom)
	{
		if (borderState == Unset && !table->bottomBorder().isNull())
		{
			m_currentBorder = table->bottomBorder();
			borderState = Set;
		}
		else if (m_currentBorder != table->bottomBorder())
			borderState = TriState;
	}

	if (borderState == Set)
	{
		/// Some sides selected and they have same border.
		addBorderLineButton->setEnabled(true);
		removeBorderLineButton->setEnabled(true);
		borderLineList->setEnabled(true);
	}
	else if (borderState == TriState)
	{
		/// Some sides selected but they have different border.
		m_currentBorder = TableBorder();
		addBorderLineButton->setEnabled(true);
		removeBorderLineButton->setEnabled(true);
		borderLineList->setEnabled(true);
	}
	else
	{
		/// No sides selected.
		m_currentBorder = TableBorder();
		addBorderLineButton->setEnabled(false);
		removeBorderLineButton->setEnabled(false);
		borderLineList->setEnabled(false);
	}

	updateBorderLineList();
}

void PropertiesPalette_Table::updateBorderLineList()
{
	borderLineList->clear();

	foreach (const TableBorderLine& borderLine, m_currentBorder.borderLines())
	{
		QPixmap *icon = getWidePixmap(getColor(borderLine.color(), borderLine.shade()));
		QString text = QString(" %1%2 %3")
			.arg(borderLine.width())
			.arg(borderLineWidth->suffix())
			.arg(CommonStrings::translatePenStyleName(borderLine.style()));
		borderLineList->addItem(new QListWidgetItem(*icon, text, borderLineList));
	}

	/*if (borderLineList->count() > 0)
		borderLineList->setCurrentItem(borderLineList->item(0));
	else
		on_borderLineList_currentRowChanged(-1);*/
}

void PropertiesPalette_Table::updateBorderLineListItem()
{
	QListWidgetItem* item = borderLineList->currentItem();
	QString color = borderLineColor->currentColor();
	if (color == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;
	QPixmap *icon = getWidePixmap(getColor(color, borderLineShade->value()));
	QString text = QString(" %1%2 %3")
		.arg(borderLineWidth->getValue())
		.arg(borderLineWidth->suffix())
		.arg(CommonStrings::translatePenStyleName(static_cast<Qt::PenStyle>(borderLineStyle->currentIndex() + 1)));

	item->setIcon(*icon);
	item->setText(text);
}

void PropertiesPalette_Table::updateFillControls()
{
	if (m_item && m_item->isTable())
	{
		PageItem_Table* table = m_item->asTable();
		// Enable fill editing controls.
		fillColor->setEnabled(true);
		fillColorLabel->setEnabled(true);
		fillShade->setEnabled(true);
		fillShadeLabel->setEnabled(true);
		// Fill in values.
		if (m_doc->appMode != modeEditTable)
		{
			QString color = table->fillColor();
			if (color == CommonStrings::None)
				color = CommonStrings::tr_NoneColor;
			setCurrentComboItem(fillColor, color);
			fillShade->setValue(table->fillShade());
		}
		else
		{
			TableCell cell = table->activeCell();
			QString color = cell.fillColor();
			if (color == CommonStrings::None)
				color = CommonStrings::tr_NoneColor;
			setCurrentComboItem(fillColor, color);
			fillShade->setValue(cell.fillShade());
		}
	}
	else
	{
		// Disable fill editing controls.
		fillColor->setEnabled(false);
		fillColorLabel->setEnabled(false);
		fillShade->setEnabled(false);
		fillShadeLabel->setEnabled(false);
	}
}

QColor PropertiesPalette_Table::getColor(const QString& colorName, int shade) const
{
	if (!m_doc)
		return QColor();

	return ScColorEngine::getDisplayColor(m_doc->PageColors[colorName], m_doc, shade);
}

void PropertiesPalette_Table::on_borderLineList_currentRowChanged(int row)
{
	if (row == -1)
	{
		// No list item selected, so disable editing widgets.
		borderLineWidth->setEnabled(false);
		borderLineWidthLabel->setEnabled(false);
		borderLineColor->setEnabled(false);
		borderLineColorLabel->setEnabled(false);
		borderLineStyle->setEnabled(false);
		borderLineStyleLabel->setEnabled(false);
		borderLineShade->setEnabled(false);
		borderLineShadeLabel->setEnabled(false);
	}
	else
	{
		QList<TableBorderLine> borderLines = m_currentBorder.borderLines();
		Q_ASSERT(borderLineList->count() == borderLines.size());
		TableBorderLine line = borderLines.at(row);

		// Enable editing widgets.
		borderLineWidth->setEnabled(true);
		borderLineWidthLabel->setEnabled(true);
		borderLineColor->setEnabled(true);
		borderLineColorLabel->setEnabled(true);
		borderLineStyle->setEnabled(true);
		borderLineStyleLabel->setEnabled(true);
		borderLineShade->setEnabled(true);
		borderLineShadeLabel->setEnabled(true);

		// Fill in values.
		borderLineWidth->showValue(line.width());
		setCurrentComboItem(borderLineColor, line.color());
		borderLineStyle->setCurrentIndex(static_cast<int>(line.style()) - 1);
		borderLineShade->setValue(line.shade());
	}
}

/// Handles adding of a new border line.
void PropertiesPalette_Table::on_addBorderLineButton_clicked()
{
	if (!m_item || !m_item->isTable())
		return;

	m_currentBorder.addBorderLine(TableBorderLine());
	updateBorderLineList();
}

/// Handles removing of a border line.
void PropertiesPalette_Table::on_removeBorderLineButton_clicked()
{
	int index = borderLineList->currentRow();

	borderLineList->removeItemWidget(borderLineList->currentItem());
	m_currentBorder.removeBorderLine(index);

	updateBorderLineList();
}

void PropertiesPalette_Table::on_borderLineWidth_valueChanged(double width)
{
	int index = borderLineList->currentRow();
	TableBorderLine borderLine = m_currentBorder.borderLines().at(index);
	borderLine.setWidth(width);
	m_currentBorder.replaceBorderLine(index, borderLine);

	updateBorders();
	updateBorderLineListItem();
}

void PropertiesPalette_Table::on_borderLineShade_valueChanged(int shade)
{
	int index = borderLineList->currentRow();
	TableBorderLine borderLine = m_currentBorder.borderLines().at(index);
	borderLine.setShade(shade);
	m_currentBorder.replaceBorderLine(index, borderLine);

	updateBorders();
	updateBorderLineListItem();
}

void PropertiesPalette_Table::on_borderLineColor_activated(const QString& colorName)
{
	int index = borderLineList->currentRow();
	TableBorderLine borderLine = m_currentBorder.borderLines().at(index);
	borderLine.setColor(colorName);
	m_currentBorder.replaceBorderLine(index, borderLine);

	updateBorders();
	updateBorderLineListItem();
}

void PropertiesPalette_Table::on_borderLineStyle_activated(int style)
{
	int index = borderLineList->currentRow();
	TableBorderLine borderLine = m_currentBorder.borderLines().at(index);
	borderLine.setStyle(static_cast<Qt::PenStyle>(style + 1));
	m_currentBorder.replaceBorderLine(index, borderLine);

	updateBorders();
	updateBorderLineListItem();
}

void PropertiesPalette_Table::on_fillColor_activated(const QString& colorName)
{
	if (!m_item || !m_item->isTable())
		return;
	QString color = colorName;
	if (colorName == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;
	PageItem_Table* table = m_item->asTable();
	if (m_doc->appMode != modeEditTable)
	{
		table->setFillColor(color);
		table->setFillShade(fillShade->value());
	}
	else
	{
		TableCell cell = table->activeCell();
		cell.setFillColor(color);
		cell.setFillShade(fillShade->value());
	}

	table->update();
}

void PropertiesPalette_Table::on_fillShade_valueChanged(int shade)
{
	if (!m_item || !m_item->isTable())
		return;

	QString color = fillColor->currentColor();
	if (color == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;
	PageItem_Table* table = m_item->asTable();
	if (m_doc->appMode != modeEditTable)
	{
		table->setFillColor(color);
		table->setFillShade(shade);
	}
	else
	{
		TableCell cell = table->activeCell();
		cell.setFillColor(color);
		cell.setFillShade(shade);
	}
	table->update();
}

void PropertiesPalette_Table::updateBorders()
{
	if (!m_doc || !m_item || !m_item->isTable())
		return;

	PageItem_Table* table = m_item->asTable();
	TableSideSelector::Sides selectedSides = sideSelector->selection();

	m_doc->dontResize = true;
	if (selectedSides & TableSideSelector::Left)
		table->setLeftBorder(m_currentBorder);
	if (selectedSides & TableSideSelector::Right)
		table->setRightBorder(m_currentBorder);
	if (selectedSides & TableSideSelector::Top)
		table->setTopBorder(m_currentBorder);
	if (selectedSides & TableSideSelector::Bottom)
		table->setBottomBorder(m_currentBorder);
	table->adjustTable();
	table->update();
}

void PropertiesPalette_Table::languageChange()
{
	// Not implemented.
}

void PropertiesPalette_Table::unitChange()
{
	// Not implemented.
}
