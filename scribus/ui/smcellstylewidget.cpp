/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "colorcombo.h"
#include "iconmanager.h"
#include "sccolorengine.h"
#include "scribus.h"
#include "scribusapp.h"
#include "smcellstylewidget.h"
#include "util.h"
#include "util_color.h"

SMCellStyleWidget::SMCellStyleWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	buttonLineColor->colorButton->setContext(Context::TableCellStroke);
	buttonLineColor->setColor(CommonStrings::tr_NoneColor);
	buttonLineColor->setText(tr("Color"));

	buttonFillColor->colorButton->setContext(Context::TableCellFill);
	buttonFillColor->setColor(CommonStrings::tr_NoneColor);
	buttonFillColor->setText(tr("Color"));

	sideSelector->setInnerActive(false);
	sideSelector->setStyle(TableSideSelector::CellStyle);

	MarginStruct distances;
	cellPaddingWidget->setup(distances, 0, m_unitIndex, NewMarginWidget::DistanceWidgetFlags);
	cellPaddingWidget->toggleLabelVisibility(false);

	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(buttonLineColor->colorButton, &ColorButton::changed, this, &SMCellStyleWidget::borderLineColorChanged);
}

void SMCellStyleWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void SMCellStyleWidget::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();
	buttonFillColor->setPixmap(iconManager.loadPixmap("color-fill"));
	buttonLineColor->setPixmap(iconManager.loadPixmap("color-stroke"));
	addBorderLineButton->setIcon(iconManager.loadIcon("stroke-add"));
	removeBorderLineButton->setIcon(iconManager.loadIcon("stroke-remove"));
}

void SMCellStyleWidget::languageChange()
{
	retranslateUi(this);

	buttonLineColor->colorButton->setPersistentToolTip( tr("Border color of table cell"));
	buttonLineColor->setText(tr("Color"));

	buttonFillColor->colorButton->setPersistentToolTip( tr("Fill color of table cell"));
	buttonFillColor->setText(tr("Color"));

	borderLineStyleLabel->setText(tr("Type"));
	borderLineWidthLabel->setText(tr("Width"));

	verticalAlignmentSelect->languageChange();
}

void SMCellStyleWidget::updateBorderLineList()
{
	borderLineList->clear();
	for (const TableBorderLine& line : m_currentBorder.borderLines())
	{
		QString text = QString(" %1%2 %3").arg(line.width()).arg(borderLineWidth->suffix(), CommonStrings::translatePenStyleName(line.style()));
		if (line.color() != CommonStrings::None)
		{
			QPixmap icon = getWidePixmap(getColor(line.color(), line.shade()));
			borderLineList->addItem(new QListWidgetItem(icon, text, borderLineList));
		}
		else
		{
			borderLineList->addItem(new QListWidgetItem(text, borderLineList));
		}
	}
	removeBorderLineButton->setEnabled(borderLineList->count() > 1);
}

void SMCellStyleWidget::updateBorderLineListItem()
{
	QListWidgetItem* item = borderLineList->currentItem();
	if (!item)
		return;

	QString text = QString(" %1%2 %3").arg(borderLineWidth->getValue()).arg(borderLineWidth->suffix(), CommonStrings::translatePenStyleName(static_cast<Qt::PenStyle>(borderLineStyle->currentIndex() + 1)));
	if (buttonLineColor->colorButton->colorName() != CommonStrings::None)
	{
		QPixmap icon = getWidePixmap(getColor(buttonLineColor->colorButton->colorName(), buttonLineColor->colorButton->colorData().Shade));
		item->setIcon(icon);
	}
	item->setText(text);
}

QColor SMCellStyleWidget::getColor(const QString& colorName, int shade) const
{
	if (!m_Doc)
		return QColor();
	return ScColorEngine::getDisplayColor(m_Doc->PageColors.value(colorName), m_Doc, shade);
}

void SMCellStyleWidget::mirrorCurrentBorderToSelectedSides()
{
	TableSides sides = sideSelector->selection();
	if (sides & TableSide::Left)
		m_leftBorder = m_currentBorder;
	if (sides & TableSide::Right)
		m_rightBorder = m_currentBorder;
	if (sides & TableSide::Top)
		m_topBorder = m_currentBorder;
	if (sides & TableSide::Bottom)
		m_bottomBorder = m_currentBorder;
}

void SMCellStyleWidget::handleUpdateRequest(int updateFlags)
{
	if (!m_Doc)
		return;
}

void SMCellStyleWidget::setDoc(ScribusDoc* doc)
{
	if (m_Doc)
		disconnect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));

	m_Doc = doc;
	if (!m_Doc)
		return;

	m_unitRatio = m_Doc->unitRatio();
	m_unitIndex = m_Doc->unitIndex();
	cellPaddingWidget->setNewUnit(m_unitIndex);

	buttonLineColor->colorButton->setDoc(m_Doc);
	buttonFillColor->colorButton->setDoc(m_Doc);

	connect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));
}

void SMCellStyleWidget::show(CellStyle *cellStyle, QList<CellStyle> &cellStyles, const QString &defaultLanguage, int unitIndex)
{
	Q_ASSERT(cellStyle);
	if (!cellStyle)
		return;

	parentCombo->setEnabled(!cellStyle->isDefaultStyle());

	const CellStyle *parent = dynamic_cast<const CellStyle*>(cellStyle->parentStyle());
	bool hasParent =  cellStyle->hasParent() && parent != nullptr && parent->hasName() && cellStyle->parent() != "";
	if (hasParent)
		buttonFillColor->setColor(cellStyle->fillColor(), qRound(cellStyle->fillShade()), parent->fillColor(), qRound(parent->fillShade()), cellStyle->isInhFillColor() && cellStyle->isInhFillShade());
	else
		buttonFillColor->setColor(cellStyle->fillColor(), qRound(cellStyle->fillShade()));

	setBorders(cellStyle->leftBorder(), cellStyle->rightBorder(), cellStyle->topBorder(), cellStyle->bottomBorder());

	// Padding
	MarginStruct padding;
	padding.set(cellStyle->topPadding(), cellStyle->leftPadding(), cellStyle->bottomPadding(), cellStyle->rightPadding());

	QSignalBlocker blocker(cellPaddingWidget);
	cellPaddingWidget->setPageWidth(99999.0);
	cellPaddingWidget->setPageHeight(99999.0);
	cellPaddingWidget->setNewValues(padding);

	// Vertical alignment. setParentItem() must follow setStyle(): the latter
	// records the value passed to it as the fallback, so setting the parent
	// first would be overwritten and the reset button would restore the
	// override instead of the inherited value.
	verticalAlignmentSelect->setEnabled(true);
	if (hasParent)
	{
		verticalAlignmentSelect->setStyle(cellStyle->verticalAlignment(), cellStyle->isInhVerticalAlignment());
		verticalAlignmentSelect->setParentItem(parent->verticalAlignment());
	}
	else
		verticalAlignmentSelect->setStyle(cellStyle->verticalAlignment());

	parentCombo->clear();
	parentCombo->addItem( cellStyle->isDefaultStyle()? tr("A default style cannot be assigned a parent style") : "");
	if (!cellStyle->isDefaultStyle())
	{
		QStringList styleNames;
		for (int i = 0; i < cellStyles.count(); ++i)
		{
			if (cellStyles[i].name() != cellStyle->name())
				styleNames.append(cellStyles[i].name());
		}
		styleNames.sort(Qt::CaseSensitive);
		parentCombo->addItems(styleNames);
	}

	if (cellStyle->isDefaultStyle() || !hasParent)
		parentCombo->setCurrentIndex(0);
	else if (hasParent)
	{
		int index = parentCombo->findText(cellStyle->parentStyle()->name());
		if (index < 0)
			index = 0;
		parentCombo->setCurrentIndex(index);
	}
}

void SMCellStyleWidget::show(QList<CellStyle*> &cellStyles, QList<CellStyle> &cellStylesAll, const QString &defaultLanguage, int unitIndex)
{
	if (cellStyles.count() == 1)
		show(cellStyles[0], cellStylesAll, defaultLanguage, unitIndex);
	else if (cellStyles.count() > 1)
	{
		showColors(cellStyles);
		parentCombo->setEnabled(false);
		verticalAlignmentSelect->setEnabled(false);
	}
}

void SMCellStyleWidget::showColors(const QList<CellStyle*> &cellStyles)
{
	double d = -30000;
	for (int i = 0; i < cellStyles.count(); ++i)
	{
		if (d != -30000 && cellStyles[i]->fillShade() != d)
		{
			d = -30000;
			break;
		}
		d = cellStyles[i]->fillShade();
	}
	QString s;
	for (int i = 0; i < cellStyles.count(); ++i)
	{
		if (!s.isEmpty() && s != cellStyles[i]->fillColor())
		{
			s.clear();
			break;
		}
		s = cellStyles[i]->fillColor();
	}

	buttonFillColor->setColor(s, qRound(d));
}

void SMCellStyleWidget::setBorders(const TableBorder& left, const TableBorder& right,
									const TableBorder& top, const TableBorder& bottom)
{
	m_leftBorder = left;
	m_rightBorder = right;
	m_topBorder = top;
	m_bottomBorder = bottom;
	on_sideSelector_selectionChanged();
}

void SMCellStyleWidget::on_sideSelector_selectionChanged()
{
	TableSides newSelection = sideSelector->selection();
	TableSides changedSides = newSelection ^ m_lastSelection;
	bool turnedOn = (newSelection & changedSides) != 0;

	m_lastSelection = newSelection;

	// When sides are toggled on/off, mirror m_currentBorder (or null) to those sides
	// and emit a bordersChanged so the controller persists into the style.
	if (changedSides != TableSide::None)
	{
		TableBorder borderToApply;
		if (turnedOn)
		{
			if (m_currentBorder.isNull())
				m_currentBorder = TableBorder(1.0, Qt::SolidLine, "Black", 100);
			borderToApply = m_currentBorder;
		}
		// else borderToApply stays a null TableBorder, meaning "remove the border"

		if (changedSides & TableSide::Left)
			m_leftBorder = borderToApply;
		if (changedSides & TableSide::Right)
			m_rightBorder = borderToApply;
		if (changedSides & TableSide::Top)
			m_topBorder = borderToApply;
		if (changedSides & TableSide::Bottom)
			m_bottomBorder = borderToApply;

		emit bordersChanged(changedSides, borderToApply);
	}

	// Refresh the border-line list to show the border on the currently-selected sides.
	State borderState = Unset;
	m_currentBorder = TableBorder();

	auto considerSide = [&](TableSide side, const TableBorder& border)
	{
		if (!(newSelection & side))
			return;
		if (borderState == Unset && !border.isNull())
		{
			m_currentBorder = border;
			borderState = Set;
		}
		else if (m_currentBorder != border)
		{
			borderState = TriState;
		}
	};
	considerSide(TableSide::Left,   m_leftBorder);
	considerSide(TableSide::Right,  m_rightBorder);
	considerSide(TableSide::Top,    m_topBorder);
	considerSide(TableSide::Bottom, m_bottomBorder);

	const bool enable = (borderState != Unset);
	addBorderLineButton->setEnabled(enable);
	removeBorderLineButton->setEnabled(enable);
	borderLineList->setEnabled(enable);

	if (borderState == TriState)
		m_currentBorder = TableBorder();

	int previousRow = borderLineList->currentRow();
	updateBorderLineList();
	if (previousRow >= 0 && previousRow < borderLineList->count())
		borderLineList->setCurrentRow(previousRow);
}

void SMCellStyleWidget::on_borderLineList_currentRowChanged(int row)
{
	if (row == -1)
	{
		borderLineWidth->setEnabled(false);
		borderLineWidthLabel->setEnabled(false);
		buttonLineColor->setEnabled(false);
		borderLineStyle->setEnabled(false);
		borderLineStyleLabel->setEnabled(false);
		return;
	}

	const QList<TableBorderLine>& lines = m_currentBorder.borderLines();
	if (row >= lines.size())
		return;
	const TableBorderLine& line = lines.at(row);

	borderLineWidth->setEnabled(true);
	borderLineWidthLabel->setEnabled(true);
	buttonLineColor->setEnabled(true);
	borderLineStyle->setEnabled(true);
	borderLineStyleLabel->setEnabled(true);

	borderLineWidth->showValue(line.width());
	buttonLineColor->setColor(line.color(), line.shade());
	borderLineStyle->setCurrentIndex(static_cast<int>(line.style()) - 1);
}

void SMCellStyleWidget::on_addBorderLineButton_clicked()
{
	m_currentBorder.addBorderLine(TableBorderLine(1.0, Qt::SolidLine, "Black", 100));
	mirrorCurrentBorderToSelectedSides();
	updateBorderLineList();
	emit bordersChanged(sideSelector->selection(), m_currentBorder);
}

void SMCellStyleWidget::on_removeBorderLineButton_clicked()
{
	int index = borderLineList->currentRow();
	if (index < 0)
		return;
	m_currentBorder.removeBorderLine(index);
	mirrorCurrentBorderToSelectedSides();
	updateBorderLineList();
	emit bordersChanged(sideSelector->selection(), m_currentBorder);
}

void SMCellStyleWidget::on_borderLineWidth_valueChanged(double width)
{
	int index = borderLineList->currentRow();
	if (index < 0)
		return;
	TableBorderLine line = m_currentBorder.borderLines().at(index);
	line.setWidth(width);
	m_currentBorder.replaceBorderLine(index, line);
	mirrorCurrentBorderToSelectedSides();
	updateBorderLineListItem();
	emit bordersChanged(sideSelector->selection(), m_currentBorder);
}

void SMCellStyleWidget::borderLineColorChanged()
{
	int index = borderLineList->currentRow();
	if (index < 0)
		return;
	TableBorderLine line = m_currentBorder.borderLines().at(index);
	QString color = buttonLineColor->colorButton->colorName();
	if (color == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;
	line.setColor(color);
	line.setShade(buttonLineColor->colorButton->colorData().Shade);
	m_currentBorder.replaceBorderLine(index, line);
	mirrorCurrentBorderToSelectedSides();
	updateBorderLineListItem();
	emit bordersChanged(sideSelector->selection(), m_currentBorder);
}

void SMCellStyleWidget::on_borderLineStyle_activated(int style)
{
	int index = borderLineList->currentRow();
	if (index < 0)
		return;
	TableBorderLine line = m_currentBorder.borderLines().at(index);
	line.setStyle(static_cast<Qt::PenStyle>(style + 1));
	m_currentBorder.replaceBorderLine(index, line);
	mirrorCurrentBorderToSelectedSides();
	updateBorderLineListItem();
	emit bordersChanged(sideSelector->selection(), m_currentBorder);
}

