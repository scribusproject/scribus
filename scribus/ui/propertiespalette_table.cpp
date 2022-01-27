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
#include <QScopedValueRollback>
#include <QWidget>

#include "appmodehelper.h"
#include "appmodes.h"
#include "colorcombo.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "pageitem_table.h"
#include "propertiespalette_table.h"
#include "sccolorengine.h"
#include "scribus.h"
#include "scribusapp.h"
#include "selection.h"
#include "tableborder.h"
#include "tablesideselector.h"
#include "undomanager.h"
#include "undotransaction.h"
#include "util.h"
#include "util_color.h"



PropertiesPalette_Table::PropertiesPalette_Table(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	iconSetChange();
	labelTable->setBuddy(tableStyleCombo);
	labelCells->setBuddy(cellStyleCombo);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

	connect(tableStyleCombo, SIGNAL(newStyle(const QString&)), this, SLOT(setTableStyle(const QString&)));
	connect(cellStyleCombo, SIGNAL(newStyle(const QString&)), this, SLOT(setCellStyle(const QString&)));
}

void PropertiesPalette_Table::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();

	addBorderLineButton->setIcon(iconManager.loadIcon("penciladd.png"));
	removeBorderLineButton->setIcon(iconManager.loadIcon("pencilsub.png"));
	buttonClearTableStyle->setIcon(iconManager.loadIcon("16/edit-clear.png"));
	buttonClearCellStyle->setIcon(iconManager.loadIcon("16/edit-clear.png"));
}

void PropertiesPalette_Table::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		updateColorList();
	tableStyleCombo->updateStyleList();
	cellStyleCombo->updateStyleList();
}

void PropertiesPalette_Table::updateColorList()
{
	if (!m_doc)
		return;

	borderLineColor->setColors(m_doc->PageColors, true);
	fillColor->setColors(m_doc->PageColors, true);
}

void PropertiesPalette_Table::setMainWindow(ScribusMainWindow* mainWindow)
{
	m_mainWindow = mainWindow;

	connect(m_mainWindow, SIGNAL(UpdateRequest(int)), SLOT(handleUpdateRequest(int)));
	connect(m_mainWindow->appModeHelper, SIGNAL(AppModeChanged(int,int)), this, SLOT(updateFillControls()));
	connect(m_mainWindow->appModeHelper, SIGNAL(AppModeChanged(int,int)), this, SLOT(updateStyleControls()));
}

void PropertiesPalette_Table::setDocument(ScribusDoc *doc)
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc = doc;

	tableStyleCombo->setDoc(m_doc);
	cellStyleCombo->setDoc(m_doc);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Table::unsetDocument()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc = nullptr;

	tableStyleCombo->setDoc(m_doc);
	cellStyleCombo->setDoc(m_doc);
}

void PropertiesPalette_Table::setItem(PageItem* item)
{
	m_item = item;
	if (item->isTable())
		connect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleCellSelectionChanged()), Qt::UniqueConnection);
}

void PropertiesPalette_Table::unsetItem()
{
	disconnect(this, SLOT(handleCellSelectionChanged()));

//	if ((m_item) && (m_item->isTable()))
//		disconnect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleCellSelectionChanged()));
	m_item = nullptr;
}

void PropertiesPalette_Table::handleSelectionChanged()
{
	if (!m_doc)
		return;

	// We only handle a single item for now.
	if (m_doc->m_Selection->count() >= 1 && m_doc->m_Selection->itemAt(0)->isTable())
		m_item = m_doc->m_Selection->itemAt(0);
	else
		m_item = nullptr;

	sideSelector->setSelection(TableSide::All);

	updateFillControls();
	updateStyleControls();
}

void PropertiesPalette_Table::handleCellSelectionChanged()
{
	if (!m_doc)
		return;
	if (!m_item)
		return;
	updateFillControls();
	updateStyleControls();
	on_sideSelector_selectionChanged();
}

void PropertiesPalette_Table::showTableStyle(const QString& name)
{
	bool blocked = tableStyleCombo->blockSignals(true);
	tableStyleCombo->setStyle(name);
	tableStyleCombo->blockSignals(blocked);
}

void PropertiesPalette_Table::showCellStyle(const QString& name)
{
	bool blocked = cellStyleCombo->blockSignals(true);
	cellStyleCombo->setStyle(name);
	cellStyleCombo->blockSignals(blocked);
}

void PropertiesPalette_Table::updateStyleControls()
{
	if (m_item && m_item->isTable())
	{
		PageItem_Table* table = m_item->asTable();
		tableStyleCombo->setEnabled(true);
		cellStyleCombo->setEnabled(true);
		buttonClearTableStyle->setEnabled(true);
		buttonClearCellStyle->setEnabled(true);
		// Fill in values.
		if (m_doc->appMode != modeEditTable)
		{
			showTableStyle(table->styleName());
			cellStyleCombo->setEnabled(false);
			buttonClearCellStyle->setEnabled(false);
		}
		else
		{
//			showTableStyle(table->style());
			showCellStyle(table->activeCell().styleName());
		}
	}
	else
	{
		tableStyleCombo->setEnabled(false);
		cellStyleCombo->setEnabled(false);
		buttonClearTableStyle->setEnabled(false);
		buttonClearCellStyle->setEnabled(false);
	}
}

void PropertiesPalette_Table::setTableStyle(const QString &name)
{
	if (!m_item || !m_item->isTable())
		return;
	m_item->asTable()->setStyle(name);
	m_item->asTable()->update();
	showTableStyle(name);
}

void PropertiesPalette_Table::setCellStyle(const QString &name)
{
	if (!m_item || !m_item->isTable())
		return;
	QScopedValueRollback<bool> dontResizeRb(m_doc->dontResize, true);

	m_doc->itemSelection_SetNamedCellStyle(name);

	m_item->asTable()->update();
	showCellStyle(name);
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
	TableSides selectedSides = sideSelector->selection();
	PageItem_Table* table = m_item->asTable();
	bool tableEditMode = (m_doc->appMode == modeEditTable);

	if (selectedSides & TableSide::Left)
	{
		TableBorder leftBorder = tableEditMode ? table->activeCell().leftBorder() : table->leftBorder();
		if (borderState == Unset && !leftBorder.isNull())
		{
			m_currentBorder = leftBorder;
			borderState = Set;
		}
		else if (m_currentBorder != leftBorder)
			borderState = TriState;
	}

	if (selectedSides & TableSide::Right)
	{
		TableBorder rightBorder = tableEditMode ? table->activeCell().rightBorder() : table->rightBorder();
		if (borderState == Unset && !rightBorder.isNull())
		{
			m_currentBorder = rightBorder;
			borderState = Set;
		}
		else if (m_currentBorder != rightBorder)
			borderState = TriState;
	}

	if (selectedSides & TableSide::Top)
	{
		TableBorder topBorder = tableEditMode ? table->activeCell().topBorder() : table->topBorder();
		if (borderState == Unset && !table->topBorder().isNull())
		{
			m_currentBorder = topBorder;
			borderState = Set;
		}
		else if (m_currentBorder != topBorder)
			borderState = TriState;
	}

	if (selectedSides & TableSide::Bottom)
	{
		TableBorder bottomBorder = tableEditMode ? table->activeCell().bottomBorder() : table->bottomBorder();
		if (borderState == Unset && !bottomBorder.isNull())
		{
			m_currentBorder = bottomBorder;
			borderState = Set;
		}
		else if (m_currentBorder != bottomBorder)
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
	for (const TableBorderLine& borderLine : m_currentBorder.borderLines())
	{
		QString text = QString(" %1%2 %3").arg(borderLine.width()).arg(borderLineWidth->suffix(), CommonStrings::translatePenStyleName(borderLine.style()));
		if (borderLine.color() != CommonStrings::None)
		{
			QPixmap *icon = getWidePixmap(getColor(borderLine.color(), borderLine.shade()));
			borderLineList->addItem(new QListWidgetItem(*icon, text, borderLineList));
		}
		else
			borderLineList->addItem(new QListWidgetItem(text, borderLineList));
	}
	removeBorderLineButton->setEnabled(borderLineList->count() > 1);
}

void PropertiesPalette_Table::updateBorderLineList(const TableBorderLine& current)
{
	updateBorderLineList();

	const QList<TableBorderLine>& borderLines = m_currentBorder.borderLines();
	for (int i = 0; i < borderLines.count(); ++i)
	{
		const TableBorderLine& borderLine = borderLines.at(i);
		if (borderLine == current)
		{
			borderLineList->setCurrentRow(i);
			break;
		}
	}
}

void PropertiesPalette_Table::updateBorderLineListItem()
{
	QListWidgetItem* item = borderLineList->currentItem();
	if (!item)
		return;

	QString text = QString(" %1%2 %3").arg(borderLineWidth->getValue()).arg(borderLineWidth->suffix(), CommonStrings::translatePenStyleName(static_cast<Qt::PenStyle>(borderLineStyle->currentIndex() + 1)));
	if (borderLineColor->currentColor() != CommonStrings::None)
	{
		QPixmap *icon = getWidePixmap(getColor(borderLineColor->currentColor(), borderLineShade->value()));
		item->setIcon(*icon);
	}
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
			bool sigBlocked = fillShade->blockSignals(true);
			fillShade->setValue(table->fillShade());
			fillShade->blockSignals(sigBlocked);
		}
		else
		{
			TableCell cell = table->activeCell();
			QString color = cell.fillColor();
			if (color == CommonStrings::None)
				color = CommonStrings::tr_NoneColor;
			setCurrentComboItem(fillColor, color);
			bool sigBlocked = fillShade->blockSignals(true);
			fillShade->setValue(cell.fillShade());
			fillShade->blockSignals(sigBlocked);
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
		const TableBorderLine& line = borderLines.at(row);

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
	
	updateBorders();
	updateBorderLineList();
}

void PropertiesPalette_Table::on_borderLineWidth_valueChanged(double width)
{
	int index = borderLineList->currentRow();
	TableBorderLine borderLine = m_currentBorder.borderLines().at(index);
	borderLine.setWidth(width);
	m_currentBorder.replaceBorderLine(index, borderLine);

	updateBorders();
	updateBorderLineList(borderLine);
}

void PropertiesPalette_Table::on_borderLineShade_valueChanged(double shade)
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
	QString color = colorName;
	if (colorName == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;
	borderLine.setColor(color);
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

	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = UndoManager::instance()->beginTransaction(m_item->getUName(), m_item->getUPixmap(), Um::CellFillColor, QString(), Um::ITable);

	QString color = colorName;
	if (colorName == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;

	PageItem_Table* table = m_item->asTable();
	if (m_doc->appMode != modeEditTable)
		table->setFillColor(color);
	else
		table->setCellFillColor(color);

	if (activeTransaction)
		activeTransaction.commit();

	table->update();
}

void PropertiesPalette_Table::on_fillShade_valueChanged(double shade)
{
	if (!m_item || !m_item->isTable())
		return;

	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = UndoManager::instance()->beginTransaction(m_item->getUName(), m_item->getUPixmap(), Um::CellFillShade, QString(), Um::ITable);

	QString color = fillColor->currentColor();
	if (color == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;

	PageItem_Table* table = m_item->asTable();
	if (m_doc->appMode != modeEditTable)
		table->setFillShade(shade);
	else
		table->setCellFillShade(fillShade->value());

	if (activeTransaction)
		activeTransaction.commit();

	table->update();
}

void PropertiesPalette_Table::on_buttonClearTableStyle_clicked()
{
	if (!m_item || !m_item->isTable())
		return;
	PageItem_Table* table = m_item->asTable();
	table->unsetDirectFormatting();
	table->update();
}

void PropertiesPalette_Table::on_buttonClearCellStyle_clicked()
{
	if (!m_item || !m_item->isTable())
		return;
	QScopedValueRollback<bool> dontResizeRb(m_doc->dontResize, true);

	PageItem_Table* table = m_item->asTable();
	if (m_doc->appMode != modeEditTable)
	{
		for (int row = 0; row < table->rows(); ++row)
		{
			int colSpan = 0;
			for (int col = 0; col < table->columns(); col += colSpan)
			{
				TableCell currentCell = table->cellAt(row, col);
				if (row == currentCell.row())
					currentCell.unsetDirectFormatting();
				colSpan = currentCell.columnSpan();
			}
		}
	}
	else
	{
		QSet<TableCell> cells = table->selectedCells();
		if (cells.isEmpty())
			cells.insert(table->activeCell());
		for (auto cellIter = cells.begin(); cellIter != cells.end(); cellIter++)
		{
			TableCell currentCell(*cellIter);
			currentCell.unsetDirectFormatting();
		}
	}

	table->adjustTable();
	table->update();
}

void PropertiesPalette_Table::updateBorders()
{
	if (!m_doc || !m_item || !m_item->isTable())
		return;
	QScopedValueRollback<bool> dontResizeRb(m_doc->dontResize, true);

	PageItem_Table* table = m_item->asTable();
	TableSides selectedSides = sideSelector->selection();
	if (m_doc->appMode != modeEditTable)
	{
		table->setBorders(m_currentBorder, selectedSides);
	}
	else
	{
		QSet<TableCell> cells = table->selectedCells();
		if (cells.isEmpty())
			cells.insert(table->activeCell());
		table->setCellBorders(cells, m_currentBorder, selectedSides);
	}
	table->update();
}

void PropertiesPalette_Table::languageChange()
{
	retranslateUi(this);
}

void PropertiesPalette_Table::unitChange()
{
	// Not implemented.
}
