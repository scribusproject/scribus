/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_distance.h"

#include <QSignalBlocker>

#include "appmodehelper.h"
#include "appmodes.h"
#include "iconmanager.h"
#include "localemgr.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"
#include "tabmanager.h"

PropertyWidget_Distance::PropertyWidget_Distance(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	layout()->setAlignment(Qt::AlignTop);

	columnGap->setValues(0, 300, 2, 0);
	columns->setDecimals(0);
	columns->setSuffix("");

	MarginStruct distances;
	distances.resetToZero();

	distanceWidget->setup(distances, 0, m_unitIndex, NewMarginWidget::DistanceWidgetFlags);
	distanceWidget->toggleLabelVisibility(false);

	firstLineIndentSpin->setValues(-3000, 4000, 3, 0);
	leftIndentSpin->setValues(0, 4000, 3, 0);
	rightIndentSpin->setValues(0, 4000, 3, 0);

	gapBeforeSpin->setValues(0, 300, 2, 0);
	gapBeforeSpin->setSuffix(unitGetSuffixFromIndex(0));
	gapAfterSpin->setValues(0, 300, 2, 0);
	gapAfterSpin->setSuffix(unitGetSuffixFromIndex(0));

	iconSetChange();
	languageChange();

	columnGapCombo->setCurrentIndex(0);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));
}

void PropertyWidget_Distance::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW->appModeHelper, SIGNAL(AppModeChanged(int,int)), this, SLOT(handleAppModeChanged(int,int)));
	connect(m_ScMW, SIGNAL(UpdateRequest(int))      , this, SLOT(handleUpdateRequest(int)));
}

void PropertyWidget_Distance::setDoc(ScribusDoc *d)
{
	if (d == (ScribusDoc*) m_doc)
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = nullptr;

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	columns->setDecimals(0);
	columnGap->setDecimals(2);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_Distance::updateStyle(const ParagraphStyle &newCurrent)
{
	if (!m_item)
		return;

	firstLineIndentSpin->setValue(newCurrent.firstIndent() * m_unitRatio);
	leftIndentSpin->setValue(newCurrent.leftMargin() * m_unitRatio);
	rightIndentSpin->setValue(newCurrent.rightMargin() * m_unitRatio);
	gapBeforeSpin->setValue(newCurrent.gapBefore());
	gapAfterSpin->setValue(newCurrent.gapAfter());
}

void PropertyWidget_Distance::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be nullptr.
	//if (m_item == i)
	//	return;

	if (item && m_doc.isNull())
		setDoc(item->doc());

	m_item = item;

	disconnectSignals();
	configureWidgets();

	if (!m_item) return;

	PageItem_TextFrame *textItem = m_item->asTextFrame();
	if (m_doc->appMode == modeEditTable)
		textItem = m_item->asTable()->activeCell().textFrame();
	if (!textItem) return;

	ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
	if (m_doc->appMode == modeEdit || m_doc->appMode == modeEditTable)
		m_item->currentTextProps(parStyle);

	//#14427: columns->setMaximum(qMax(qRound(textItem->width() / qMax(textItem->ColGap, 10.0)), 1));
	columns->setMinimum(1);
	columns->setValue(textItem->m_columns);
	columnGap->setMinimum(0);
	if (columnGapCombo->currentIndex() == 0)
	{
		columnGap->setMaximum(qMax((textItem->width() / textItem->m_columns - textItem->textToFrameDistLeft() - textItem->textToFrameDistRight()) * m_unitRatio, 0.0));
		columnGap->setValue(textItem->m_columnGap * m_unitRatio);
	}
	else
	{
		columnGap->setMaximum(qMax((textItem->width() / textItem->m_columns) * m_unitRatio, 0.0));
		columnGap->setValue(textItem->columnWidth() * m_unitRatio);
	}

	MarginStruct distances;
	distances.set(
		textItem->textToFrameDistTop(),
		textItem->textToFrameDistLeft(),
		textItem->textToFrameDistBottom(),
		textItem->textToFrameDistRight()
	);
	distanceWidget->setPageHeight(m_item->height());
	distanceWidget->setPageWidth(m_item->width());
	distanceWidget->setNewValues(distances);

	columnGapLabel->setVisible(columns->value() != 1);
	columnGapSizeLabel->setVisible(columns->value() != 1);

	updateStyle(parStyle);

	connectSignals();
}

void PropertyWidget_Distance::connectSignals()
{
	connect(columns       , SIGNAL(valueChanged(double))   , this, SLOT(handleColumns()), Qt::UniqueConnection);
	connect(columnGap     , SIGNAL(valueChanged(double)), this, SLOT(handleColumnGap()), Qt::UniqueConnection);
	connect(columnGapCombo, SIGNAL(activated(int))      , this, SLOT(handleGapSwitch()), Qt::UniqueConnection);
	connect(distanceWidget, SIGNAL(valuesChanged(MarginStruct)), this, SLOT(handleTextDistances()), Qt::UniqueConnection);
	connect(tabsButton    , SIGNAL(clicked())           , this, SLOT(handleTabs()), Qt::UniqueConnection);
	connect(firstLineIndentSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()), Qt::UniqueConnection);
	connect(rightIndentSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()), Qt::UniqueConnection);
	connect(leftIndentSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()), Qt::UniqueConnection);
	connect(gapAfterSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()), Qt::UniqueConnection);
	connect(gapBeforeSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()), Qt::UniqueConnection);

}

void PropertyWidget_Distance::disconnectSignals()
{
	disconnect(columns       , SIGNAL(valueChanged(double))   , this, SLOT(handleColumns()));
	disconnect(columnGap     , SIGNAL(valueChanged(double)), this, SLOT(handleColumnGap()));
	disconnect(columnGapCombo, SIGNAL(activated(int))      , this, SLOT(handleGapSwitch()));
	disconnect(distanceWidget, SIGNAL(valuesChanged(MarginStruct)), this, SLOT(handleTextDistances()));
	disconnect(tabsButton    , SIGNAL(clicked())           , this, SLOT(handleTabs()));
	disconnect(firstLineIndentSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()));
	disconnect(rightIndentSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()));
	disconnect(leftIndentSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()));
	disconnect(gapAfterSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()));
	disconnect(gapBeforeSpin, SIGNAL(valueChanged(double)), this, SLOT(handleIndents()));
}

void PropertyWidget_Distance::configureWidgets()
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		PageItem_TextFrame *textItem = m_item->asTextFrame();
		if (m_doc->appMode == modeEditTable)
			textItem = m_item->asTable()->activeCell().textFrame();

		enabled  = (textItem != nullptr);
		enabled &= (m_doc->m_Selection->count() == 1);

		if (textItem)
		{
			int numCols = textItem->m_columns;
			
			columnGapLabel->setVisible(numCols != 1);
			columnGapSizeLabel->setVisible(numCols != 1);
		}
	}
	setEnabled(enabled);
}

void PropertyWidget_Distance::handleAppModeChanged(int oldMode, int mode)
{
	if (oldMode == modeEditTable || mode == modeEditTable)
	{
		setCurrentItem(m_item);
	}
}

void PropertyWidget_Distance::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
}

void PropertyWidget_Distance::handleUpdateRequest(int /*updateFlags*/)
{
	// Nothing to do in this widget
}

void PropertyWidget_Distance::iconSetChange()
{
	IconManager &im = IconManager::instance();

	columnsLabel->setPixmap(im.loadPixmap("paragraph-columns"));
	firstLineIndentLabel->setPixmap(im.loadPixmap("paragraph-indent-firstline"));
	leftIndentLabel->setPixmap(im.loadPixmap("paragraph-indent-left"));
	rightIndentLabel->setPixmap(im.loadPixmap("paragraph-indent-right"));
	gapBeforeLabel->setPixmap(im.loadPixmap("paragraph-space-above") );
	gapAfterLabel->setPixmap(im.loadPixmap("paragraph-space-below") );
}

void PropertyWidget_Distance::showColumns(int r, double g)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	bool cSigWasBlocked    = columns->blockSignals(true);
	bool cGapSigWasBlocked = columnGap->blockSignals(true);

	columns->setValue(r);
	columnGap->setValue(g * m_unitRatio);
	if (m_item)
	{
		PageItem_TextFrame *textItem = m_item->asTextFrame();
		if (m_doc->appMode == modeEditTable)
			textItem = m_item->asTable()->activeCell().textFrame();

		if (textItem != nullptr)
		{
//#14427: columns->setMaximum(qMax(qRound(textItem->width() / qMax(textItem->ColGap, 10.0)), 1));
			if (columnGapCombo->currentIndex() == 0)
			{
				columnGap->setMaximum(qMax((textItem->width() / textItem->m_columns - textItem->textToFrameDistLeft() - textItem->textToFrameDistRight()) * m_unitRatio, 0.0));
				columnGap->setValue(textItem->m_columnGap * m_unitRatio);
			}
			else
			{
				columnGap->setMaximum(qMax((textItem->width() / textItem->m_columns) * m_unitRatio, 0.0));
				columnGap->setValue(textItem->columnWidth() * m_unitRatio);
			}
		}
	}
	columns->setMinimum(1);
	columnGap->setMinimum(0);
	columnGapLabel->setVisible(columns->value() != 1);
	columnGapSizeLabel->setVisible(columns->value() != 1);

	columns->blockSignals(cSigWasBlocked);
	columnGap->blockSignals(cGapSigWasBlocked);
}

void PropertyWidget_Distance::handleColumns()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem *textItem = m_item;
	if (m_doc->appMode == modeEditTable)
		textItem = m_item->asTable()->activeCell().textFrame();

	if (textItem)
	{
		textItem->setColumns(static_cast<int>(columns->value()));
		showColumns(textItem->m_columns, textItem->m_columnGap);
		textItem->update();
		if (m_doc->appMode == modeEditTable)
			m_item->asTable()->update();
		m_doc->regionsChanged()->update(QRect());
	}
}

void PropertyWidget_Distance::handleColumnGap()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem *textItem = m_item;
	if (m_doc->appMode == modeEditTable)
		textItem = m_item->asTable()->activeCell().textFrame();
	if (!textItem)
		return;

	if (columnGapCombo->currentIndex() == 0)
		textItem->setColumnGap(columnGap->value() / m_unitRatio);
	else
	{
		double lineCorr=0.0;
		if ((textItem->lineColor() != CommonStrings::None) || (!textItem->strokePattern().isEmpty()))
			lineCorr = textItem->lineWidth();
		double newWidth = columnGap->value() / m_unitRatio;
		double newGap = qMax(((textItem->width() - textItem->textToFrameDistLeft() - textItem->textToFrameDistRight() - lineCorr) - (newWidth * textItem->m_columns)) / (textItem->m_columns - 1), 0.0);
		textItem->setColumnGap(newGap);
	}
	textItem->update();
	if (m_doc->appMode == modeEditTable)
		m_item->asTable()->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertyWidget_Distance::handleGapSwitch()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	PageItem *textItem = m_item;
	if (m_doc->appMode == modeEditTable)
		textItem = m_item->asTable()->activeCell().textFrame();
	if (textItem != nullptr)
		showColumns(textItem->m_columns, textItem->m_columnGap);

	int index = columnGapCombo->currentIndex();
	columnGap->setToolTip((index == 0) ? tr( "Distance between columns" ) : tr( "Column width" ));
}

void PropertyWidget_Distance::handleTabs()
{
	if (!m_doc || !m_item)
		return;

	PageItem_TextFrame *tItem = m_item->asTextFrame();
	if (tItem == nullptr)
		return;
	const ParagraphStyle& style(m_doc->appMode == modeEdit ? tItem->currentStyle() : tItem->itemText.defaultStyle());
	TabManager *dia = new TabManager(this, m_doc->unitIndex(), style.tabValues(), tItem->columnWidth());
	if (dia->exec())
	{
		if (m_doc->appMode != modeEdit)
		{
			ParagraphStyle newStyle(m_item->itemText.defaultStyle());
			newStyle.setTabValues(dia->tabList());
			Selection tempSelection(this, false);
			tempSelection.addItem(m_item, true);
			m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
		}
		else
		{
			ParagraphStyle newStyle;
			newStyle.setTabValues(dia->tabList());
			m_doc->itemSelection_ApplyParagraphStyle(newStyle);
		}
		m_item->update();
	}
	delete dia;
}

void PropertyWidget_Distance::handleTextDistances()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem *textItem = m_item;
	if (m_doc->appMode == modeEditTable)
		textItem = m_item->asTable()->activeCell().textFrame();
	if (!textItem) return;

	double left   = distanceWidget->margins().left();
	double right  = distanceWidget->margins().right();
	double top    = distanceWidget->margins().top();
	double bottom = distanceWidget->margins().bottom();
	textItem->setTextToFrameDist(left, right, top, bottom);
	showColumns(textItem->m_columns, textItem->m_columnGap);

	textItem->update();
	if (m_doc->appMode == modeEditTable)
		m_item->asTable()->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertyWidget_Distance::handleIndents()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;


	PageItem_TextFrame *tItem = m_item->asTextFrame();
	if (tItem == nullptr)
		return;

	if (m_doc->appMode != modeEdit)
	{
		ParagraphStyle newStyle(m_item->itemText.defaultStyle());
		newStyle.setFirstIndent(firstLineIndentSpin->value() / m_unitRatio);
		newStyle.setLeftMargin(leftIndentSpin->value() / m_unitRatio);
		newStyle.setRightMargin(rightIndentSpin->value() / m_unitRatio);
		newStyle.setGapBefore(gapBeforeSpin->value());
		newStyle.setGapAfter(gapAfterSpin->value());
		Selection tempSelection(this, false);
		tempSelection.addItem(m_item, true);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
	}
	else
	{
		ParagraphStyle newStyle;
		newStyle.setFirstIndent(firstLineIndentSpin->value() / m_unitRatio);
		newStyle.setLeftMargin(leftIndentSpin->value() / m_unitRatio);
		newStyle.setRightMargin(rightIndentSpin->value() / m_unitRatio);
		newStyle.setGapBefore(gapBeforeSpin->value());
		newStyle.setGapAfter(gapAfterSpin->value());
		m_doc->itemSelection_ApplyParagraphStyle(newStyle);
	}
	m_item->update();
}

void PropertyWidget_Distance::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_Distance::languageChange()
{
	retranslateUi(this);

	QSignalBlocker columnGapLabelBlocker(columnGapCombo);
	int oldColGapLabel = columnGapCombo->currentIndex();
	columnGapCombo->clear();
	columnGapCombo->addItem( tr("Gap:"));
	columnGapCombo->addItem( tr("Width:"));
	columnGapCombo->setCurrentIndex(oldColGapLabel);

	QString ptSuffix = tr(" pt");
	QString suffix = (m_doc) ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;

	columnGap->setSuffix(suffix);
	firstLineIndentSpin->setSuffix(suffix);
	leftIndentSpin->setSuffix(suffix);
	rightIndentSpin->setSuffix(suffix);

	firstLineIndentSpin->setToolTip(tr("Indentation for first line of the paragraph"));
	leftIndentSpin->setToolTip(tr("Indentation from the left for the whole paragraph"));
	rightIndentSpin->setToolTip(tr("Indentation from the right for the whole paragraph"));
	firstLineIndentLabel->setToolTip(firstLineIndentSpin->toolTip());
	leftIndentLabel->setToolTip(leftIndentSpin->toolTip());
	rightIndentLabel->setToolTip(rightIndentSpin->toolTip());
	tabsButton->setToolTip(tr("Edit tab settings of text frame..."));
	gapBeforeLabel->setToolTip(tr("Space above a paragraph"));
	gapAfterLabel->setToolTip(tr("Space below a paragraph"));
}

void PropertyWidget_Distance::unitChange()
{
	if (!m_doc)
		return;

	QSignalBlocker columnGapBlocker(columnGap);
	QSignalBlocker distancesBlocker(distanceWidget);
	QSignalBlocker numberFirstLineIndentBlocker(firstLineIndentSpin);
	QSignalBlocker numberLeftIndentBlocker(leftIndentSpin);
	QSignalBlocker numberRightIndentBlocker(rightIndentSpin);

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	columnGap->setNewUnit(m_unitIndex);
	distanceWidget->setNewUnit(m_unitIndex);
	firstLineIndentSpin->setNewUnit(m_unitIndex);
	leftIndentSpin->setNewUnit(m_unitIndex);
	rightIndentSpin->setNewUnit(m_unitIndex);
}

void PropertyWidget_Distance::localeChange()
{
	const QLocale& l(LocaleManager::instance().userPreferredLocale());
	columnGap->setLocale(l);
	distanceWidget->setLocale(l);
	firstLineIndentSpin->setLocale(l);
	leftIndentSpin->setLocale(l);
	rightIndentSpin->setLocale(l);
	gapAfterSpin->setLocale(l);
	gapBeforeSpin->setLocale(l);
}

void PropertyWidget_Distance::toggleLabelVisibility(bool v)
{
	columnGapSizeLabel->setLabelVisibility(v);
	columnGapLabel->setLabelVisibility(v);
	columnsLabel->setLabelVisibility(v);
	firstLineIndentLabel->setLabelVisibility(v);
	leftIndentLabel->setLabelVisibility(v);
	rightIndentLabel->setLabelVisibility(v);
	gapBeforeLabel->setLabelVisibility(v);
	gapAfterLabel->setLabelVisibility(v);
}
