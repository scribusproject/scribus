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
#include <QSignalBlocker>
#include <QWidget>

#include "appmodehelper.h"
#include "appmodes.h"
#include "colorcombo.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "newmarginwidget.h"
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

	MarginStruct distances;

	cellPaddingWidget->setup(distances, 0, m_unitIndex, NewMarginWidget::DistanceWidgetFlags);
	cellPaddingWidget->toggleLabelVisibility(false);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));

	connect(tableStyleCombo, SIGNAL(newStyle(QString)), this, SLOT(setTableStyle(QString)));
	connect(cellStyleCombo, SIGNAL(newStyle(QString)), this, SLOT(setCellStyle(QString)));
	connect(tableDirectionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_tableDirectionComboBox_currentIndexChanged(int)));
}

void PropertiesPalette_Table::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();

	addBorderLineButton->setIcon(iconManager.loadIcon("stroke-add"));
	removeBorderLineButton->setIcon(iconManager.loadIcon("stroke-remove"));
	buttonClearTableStyle->setIcon(iconManager.loadIcon("reset"));
	buttonClearCellStyle->setIcon(iconManager.loadIcon("reset"));
	labelTable->setPixmap(iconManager.loadPixmap("table-style"));
	labelCells->setPixmap(iconManager.loadPixmap("table-cell-style"));
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

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

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

	if (!m_item) return;

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

	syncSideSelectorToCells();
	on_sideSelector_selectionChanged();

	updateFillControls();
	updateStyleControls();
	updatePaddingControls();
	updateAlignmentControls();
}

void PropertiesPalette_Table::handleCellSelectionChanged()
{
	if (!m_doc)
		return;
	if (!m_item)
		return;
	updateFillControls();
	updateStyleControls();
	updateAlignmentControls();
	syncSideSelectorToCells();
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
		bool blocked = tableDirectionComboBox->blockSignals(true);
		tableDirectionComboBox->setCurrentIndex(table->isRTL() ? 1 : 0);
		tableDirectionComboBox->blockSignals(blocked);
	}
	else
	{
		tableStyleCombo->setEnabled(false);
		cellStyleCombo->setEnabled(false);
		buttonClearTableStyle->setEnabled(false);
		buttonClearCellStyle->setEnabled(false);
	}
}

void PropertiesPalette_Table::updatePaddingControls()
{
	if (!m_doc || !m_item || !m_item->isTable())
		return;

	QSet<TableCell> cells = effectiveCells();
	if (cells.isEmpty())
	{
		PageItem_Table* table = m_item->asTable();
		TableCell active = table->activeCell();
		if (active.isValid())
			cells.insert(active);
	}
	if (cells.isEmpty())
		return;

	// For multi-cell selections, show the first cell's values.
	// (Mixed-value display could be added later.)
	const TableCell& cell = *cells.cbegin();

	// Set the widget's "page" dimensions to the cell's bounding rect so
	// padding clamps stay sane.
	QSignalBlocker blocker(cellPaddingWidget);
	QRectF cellRect = cell.boundingRect();
	cellPaddingWidget->setPageWidth(cellRect.width());
	cellPaddingWidget->setPageHeight(cellRect.height());

	MarginStruct padding;
	padding.set(cell.topPadding(), cell.leftPadding(), cell.bottomPadding(), cell.rightPadding());
	cellPaddingWidget->setNewValues(padding);
}

void PropertiesPalette_Table::updateAlignmentControls()
{
	if (!m_doc || !m_item || !m_item->isTable())
		return;

	QSet<TableCell> cells = effectiveCells();
	if (cells.isEmpty())
	{
		TableCell active = m_item->asTable()->activeCell();
		if (active.isValid())
			cells.insert(active);
	}
	if (cells.isEmpty())
		return;

	// For multi-cell selections, show the first cell's value, matching how
	// updatePaddingControls() behaves. AlignVerticalSelect::setStyle() only
	// checks a button, and QButtonGroup::idClicked does not fire for
	// programmatic checking, so no signal blocking is needed here.
	const TableCell& cell = *cells.cbegin();
	cellVerticalAlignment->setStyle(cell.verticalAlignment());
}

void PropertiesPalette_Table::toggleLabelVisibility(bool v)
{
	borderLineColorLabel->setLabelVisibility(v);
	borderLineShadeLabel->setLabelVisibility(v);
	borderLineStyleLabel->setLabelVisibility(v);
	borderLineWidthLabel->setLabelVisibility(v);
	fillColorLabel->setLabelVisibility(v);
	fillShadeLabel->setLabelVisibility(v);
	cellVerticalAlignment->setLabelVisibility(v);
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

	TableSides newSelection = sideSelector->selection();
	TableSides changedSides = newSelection ^ m_lastSelection;
	bool turnedOn = (newSelection & changedSides) != 0;

	m_lastSelection = newSelection;

	TableSides outerChanged = changedSides & TableSide::AllOuter;
	bool didChange = false;

	PageItem_Table* table = m_item->asTable();
	QScopedValueRollback<bool> dontResizeRb(m_doc->dontResize, true);

	// Outer-side handling: only when outer bits changed.
	if (outerChanged != TableSide::None)
	{
		didChange = true;
		if (turnedOn)
		{
			if (m_currentBorder.isNull())
				m_currentBorder = TableBorder(1.0, Qt::SolidLine, "Black", 100);

			QSet<TableCell> cells = effectiveCells();
			table->setCellBorders(cells, m_currentBorder, outerChanged);

			if (m_doc->appMode != modeEditTable)
				table->setBorders(m_currentBorder, outerChanged);
		}
		else
		{
			QSet<TableCell> cells = effectiveCells();
			TableBorder nullBorder;
			table->setCellBorders(cells, nullBorder, outerChanged);

			if (m_doc->appMode != modeEditTable)
				table->setBorders(nullBorder, outerChanged);
		}
	}

	// Inner-horizontal handling: independent of outer changes.
	if (changedSides & TableSide::InnerHorizontal)
	{
		didChange = true;
		QSet<TableCell> cells = effectiveCells();
		bool innerHOn = (newSelection & TableSide::InnerHorizontal) != 0;

		if (innerHOn && m_currentBorder.isNull())
			m_currentBorder = TableBorder(1.0, Qt::SolidLine, "Black", 100);

		TableBorder borderToApply = innerHOn ? m_currentBorder : TableBorder();
		applyToInnerHorizontal(cells, borderToApply);
	}

	// Inner-vertical handling: independent of outer changes.
	if (changedSides & TableSide::InnerVertical)
	{
		didChange = true;
		QSet<TableCell> cells = effectiveCells();
		bool innerVOn = (newSelection & TableSide::InnerVertical) != 0;

		if (innerVOn && m_currentBorder.isNull())
			m_currentBorder = TableBorder(1.0, Qt::SolidLine, "Black", 100);

		TableBorder borderToApply = innerVOn ? m_currentBorder : TableBorder();
		applyToInnerVertical(cells, borderToApply);
	}

	if (didChange)
		table->update();

	// Now refresh the border-line list to show what's on the currently-bold sides.
	// Reuse the existing logic that figures out borderState across selected sides.
	State borderState = Unset;
	m_currentBorder = TableBorder();
	bool tableEditMode = (m_doc->appMode == modeEditTable);

	if (newSelection & TableSide::Left)
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
	if (newSelection & TableSide::Right)
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
	if (newSelection & TableSide::Top)
	{
		TableBorder topBorder = tableEditMode ? table->activeCell().topBorder() : table->topBorder();
		if (borderState == Unset && !topBorder.isNull())
		{
			m_currentBorder = topBorder;
			borderState = Set;
		}
		else if (m_currentBorder != topBorder)
			borderState = TriState;
	}
	if (newSelection & TableSide::Bottom)
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
		addBorderLineButton->setEnabled(true);
		removeBorderLineButton->setEnabled(true);
		borderLineList->setEnabled(true);
	}
	else if (borderState == TriState)
	{
		m_currentBorder = TableBorder();
		addBorderLineButton->setEnabled(true);
		removeBorderLineButton->setEnabled(true);
		borderLineList->setEnabled(true);
	}
	else
	{
		m_currentBorder = TableBorder();
		addBorderLineButton->setEnabled(false);
		removeBorderLineButton->setEnabled(false);
		borderLineList->setEnabled(false);
	}

	int previousRow = borderLineList->currentRow();
	updateBorderLineList();
	if (previousRow >= 0 && previousRow < borderLineList->count())
		borderLineList->setCurrentRow(previousRow);
}

void PropertiesPalette_Table::updateBorderLineList()
{
	borderLineList->clear();
	for (const TableBorderLine& borderLine : m_currentBorder.borderLines())
	{
		QString text = QString(" %1%2 %3").arg(borderLine.width()).arg(borderLineWidth->suffix(), CommonStrings::translatePenStyleName(borderLine.style()));
		if (borderLine.color() != CommonStrings::None)
		{
			QPixmap icon = getWidePixmap(getColor(borderLine.color(), borderLine.shade()));
			borderLineList->addItem(new QListWidgetItem(icon, text, borderLineList));
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
		QPixmap icon = getWidePixmap(getColor(borderLineColor->currentColor(), borderLineShade->value()));
		item->setIcon(icon);
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

	return ScColorEngine::getDisplayColor(m_doc->PageColors.value(colorName), m_doc, shade);
}

void PropertiesPalette_Table::applyToInnerHorizontal(const QSet<TableCell>& cells, const TableBorder& border)
{
	PageItem_Table* table = m_item->asTable();
	for (TableCell cell : cells)
	{
		TableCell below = table->cellAt(cell.row() + cell.rowSpan(), cell.column());
		if (cells.contains(below))
		{
			cell.setBottomBorder(border);
			below.setTopBorder(border);
		}
	}
}

void PropertiesPalette_Table::applyToInnerVertical(const QSet<TableCell>& cells, const TableBorder& border)
{
	PageItem_Table* table = m_item->asTable();
	for (TableCell cell : cells)
	{
		TableCell right = table->cellAt(cell.row(), cell.column() + cell.columnSpan());
		if (cells.contains(right))
		{
			cell.setRightBorder(border);
			right.setLeftBorder(border);
		}
	}
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

void PropertiesPalette_Table::on_borderLineColor_textActivated(const QString& colorName)
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

void PropertiesPalette_Table::on_fillColor_textActivated(const QString& colorName)
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

	// Table-level direct formatting (fill, shade, table borders).
	table->unsetDirectFormatting();

	// Normal-mode edits also write direct formatting to every cell
	// (updateBorders -> setCellBorders), so those must be cleared too or
	// the cell borders keep rendering.
	if (m_doc->appMode != modeEditTable)
	{
		QScopedValueRollback<bool> dontResizeRb(m_doc->dontResize, true);
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
		table->adjustTable();
	}

	table->update();

	// Reflect the cleared values in the palette (mirrors handleSelectionChanged).
	syncSideSelectorToCells();
	on_sideSelector_selectionChanged();
	updateFillControls();
	updateStyleControls();
	updatePaddingControls();
	updateAlignmentControls();
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

	// Reflect the cleared values in the palette (mirrors handleSelectionChanged).
	syncSideSelectorToCells();
	on_sideSelector_selectionChanged();
	updateFillControls();
	updateStyleControls();
	updatePaddingControls();
	updateAlignmentControls();
}

void PropertiesPalette_Table::on_cellPaddingWidget_valuesChanged(const MarginStruct& padding)
{
	if (!m_doc || !m_item || !m_item->isTable())
		return;

	QScopedValueRollback<bool> dontResizeRb(m_doc->dontResize, true);

	QSet<TableCell> cells = effectiveCells();
	for (const TableCell& cell : cells)
	{
		TableCell c = cell;
		c.setLeftPadding(padding.left());
		c.setRightPadding(padding.right());
		c.setTopPadding(padding.top());
		c.setBottomPadding(padding.bottom());
	}

	m_item->asTable()->adjustTable();
	m_item->asTable()->update();
}

void PropertiesPalette_Table::on_cellVerticalAlignment_State(int alignment)
{
	if (!m_doc || !m_item || !m_item->isTable())
		return;

	QScopedValueRollback<bool> dontResizeRb(m_doc->dontResize, true);

	// Direct cell formatting, like padding and borders: in edit mode this hits
	// the selected cells, in normal mode every cell in the table.
	const QSet<TableCell> cells = effectiveCells();
	for (const TableCell& cell : cells)
	{
		TableCell c = cell;
		c.setVerticalAlignment(alignment);
	}

	m_item->asTable()->adjustTable();
	m_item->asTable()->update();
}

void PropertiesPalette_Table::on_tableDirectionComboBox_currentIndexChanged(int index)
{
	if (!m_item || !m_item->isTable())
		return;
	m_item->setRTL(index == 1);
	m_item->asTable()->update();
	if (m_doc)
		m_doc->changed();
}

void PropertiesPalette_Table::syncSideSelectorToCells()
{
	if (!m_item || !m_item->isTable())
		return;

	QSet<TableCell> cells = effectiveCells();
	if (cells.isEmpty())
		return;

	PageItem_Table* table = m_item->asTable();

	bool anyLeft = false, anyRight = false, anyTop = false, anyBottom = false;
	bool anyInnerH = false, anyInnerV = false;
	bool hasInteriorEdges = false;

	for (const TableCell& cell : cells)
	{
		if (!cell.leftBorder().isNull())
			anyLeft = true;
		if (!cell.rightBorder().isNull())
			anyRight = true;
		if (!cell.topBorder().isNull())
			anyTop = true;
		if (!cell.bottomBorder().isNull())
			anyBottom = true;

		// Check vertical adjacency for inner-horizontal edges.
		TableCell below = table->cellAt(cell.row() + cell.rowSpan(), cell.column());
		if (cells.contains(below))
		{
			hasInteriorEdges = true;
			if (!cell.bottomBorder().isNull() || !below.topBorder().isNull())
				anyInnerH = true;
		}
		// Check horizontal adjacency for inner-vertical edges.
		TableCell right = table->cellAt(cell.row(), cell.column() + cell.columnSpan());
		if (cells.contains(right))
		{
			hasInteriorEdges = true;
			if (!cell.rightBorder().isNull() || !right.leftBorder().isNull())
				anyInnerV = true;
		}
	}

	TableSides sides;
	if (anyLeft)
		sides |= TableSide::Left;
	if (anyRight)
		sides |= TableSide::Right;
	if (anyTop)
		sides |= TableSide::Top;
	if (anyBottom)
		sides |= TableSide::Bottom;
	if (anyInnerH)
		sides |= TableSide::InnerHorizontal;
	if (anyInnerV)
		sides |= TableSide::InnerVertical;

	QSignalBlocker blocker(sideSelector);
	sideSelector->setSelection(sides);
	sideSelector->setInnerActive(hasInteriorEdges);
	m_lastSelection = sides;
}

QSet<TableCell> PropertiesPalette_Table::effectiveCells() const
{
	PageItem_Table* table = m_item->asTable();
	if (m_doc->appMode == modeEditTable)
	{
		const QSet<TableCell>& selected = table->selectedCells();
		if (!selected.isEmpty())
			return selected;
		QSet<TableCell> single;
		single.insert(table->activeCell());
		return single;
	}
	return table->cells();
}

void PropertiesPalette_Table::updateBorders()
{
	if (!m_doc || !m_item || !m_item->isTable())
		return;
	QScopedValueRollback<bool> dontResizeRb(m_doc->dontResize, true);

	PageItem_Table* table = m_item->asTable();
	TableSides selectedSides = sideSelector->selection();

	if (m_doc->appMode == modeEditTable)
	{
		// Edit mode: apply to selected cells. If no cells are selected,
		// do nothing -- the user hasn't told us what to edit.
		const QSet<TableCell>& selection = table->selectedCells();
		if (!selection.isEmpty())
			table->setCellBorders(selection, m_currentBorder, selectedSides);
	}
	else
	{
		// Normal mode: implicit whole-table selection. Apply to all cells
		// and keep the table-level fallback in sync.
		table->setBorders(m_currentBorder, selectedSides);
		table->setCellBorders(table->cells(), m_currentBorder, selectedSides);
	}
	table->update();
}

void PropertiesPalette_Table::languageChange()
{
	retranslateUi(this);

	scTableStyles->setText(tr("Styles"));
	scTableFills->setText(tr("Fill"));
	scTableBorders->setText(tr("Borders"));
	scTableCellPadding->setText(tr("Cell Padding"));
	scTableCellAlignment->setText(tr("Cell Alignment"));

	borderLineColorLabel->setText(tr("C&olor"));
	borderLineShadeLabel->setText(tr("S&hade"));
	borderLineStyleLabel->setText(tr("&Type"));
	borderLineWidthLabel->setText(tr("&Width"));
	fillColorLabel->setText(tr("&Color"));
	fillShadeLabel->setText(tr("&Shade"));

	cellVerticalAlignment->languageChange();

}

void PropertiesPalette_Table::unitChange()
{
	if (m_doc)
		cellPaddingWidget->setNewUnit(m_doc->unitIndex());
}
