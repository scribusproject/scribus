/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_distance.h"

#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "selection.h"
#include "tabmanager.h"
#include "units.h"
#include "util_icon.h"

PropertyWidget_Distance::PropertyWidget_Distance(QWidget* parent) : QFrame(parent)
{
	m_item = 0;
	m_ScMW = 0;

	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);

	layout()->setAlignment( Qt::AlignTop );

	columnsLabel->setBuddy(columns);
	columnGap->setValues(0, 300, 2, 0);

	topDistance->setValues(0, 300, 2, 0);
	topLabel->setBuddy(topDistance);

	bottomDistance->setValues(0, 300, 2, 0);
	bottomLabel->setBuddy(bottomDistance);

	leftDistance->setValues(0, 300, 2, 0);
	leftLabel->setBuddy(leftDistance);

	rightDistance->setValues(0, 300, 2, 0);
	rightLabel->setBuddy(rightDistance);

	columns->setDecimals(0);

	languageChange();

	columnGapLabel->setCurrentIndex(0);
}

void PropertyWidget_Distance::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(AppModeChanged(int, int)), this, SLOT(handleAppModeChanged(int, int)));
	connect(m_ScMW, SIGNAL(UpdateRequest(int))      , this, SLOT(handleUpdateRequest(int)));
}

void PropertyWidget_Distance::setDoc(ScribusDoc *d)
{
	if(d == (ScribusDoc*) m_doc)
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = NULL;

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	columns->setDecimals(0);
	columnGap->setDecimals(2);
	topDistance->setDecimals(2);
	leftDistance->setDecimals(2);
	bottomDistance->setDecimals(2);
	rightDistance->setDecimals(2);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_Distance::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldnt really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This wont work until when a canvas deselect happens, m_item must be NULL.
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

	columns->setMaximum(qMax(qRound(textItem->width() / qMax(textItem->ColGap, 10.0)), 1));
	columns->setMinimum(1);
	columns->setValue(textItem->Cols);
	columnGap->setMinimum(0);
	if (columnGapLabel->currentIndex() == 0)
	{
		columnGap->setMaximum(qMax((textItem->width() / textItem->Cols - textItem->textToFrameDistLeft() - textItem->textToFrameDistRight()) * m_unitRatio, 0.0));
		columnGap->setValue(textItem->ColGap*m_unitRatio);
	}
	else
	{
		columnGap->setMaximum(qMax((textItem->width() / textItem->Cols) * m_unitRatio, 0.0));
		columnGap->setValue(textItem->columnWidth() * m_unitRatio);
	}
	leftDistance->setValue(textItem->textToFrameDistLeft()*m_unitRatio);
	topDistance->setValue(textItem->textToFrameDistTop()*m_unitRatio);
	bottomDistance->setValue(textItem->textToFrameDistBottom()*m_unitRatio);
	rightDistance->setValue(textItem->textToFrameDistRight()*m_unitRatio);
	if (columns->value() == 1)
	{
		columnGap->setEnabled(false);
		columnGapLabel->setEnabled(false);
	}
	else
	{
		columnGap->setEnabled(true);
		columnGapLabel->setEnabled(true);
	}

	displayTextDistances(textItem->textToFrameDistLeft(), textItem->textToFrameDistTop(), textItem->textToFrameDistBottom(), textItem->textToFrameDistRight());
	connectSignals();
}

void PropertyWidget_Distance::connectSignals()
{
	connect(columns       , SIGNAL(valueChanged(double))   , this, SLOT(handleColumns()), Qt::UniqueConnection);
	connect(columnGap     , SIGNAL(valueChanged(double)), this, SLOT(handleColumnGap()), Qt::UniqueConnection);
	connect(columnGapLabel, SIGNAL(activated(int))      , this, SLOT(handleGapSwitch()), Qt::UniqueConnection);
	connect(topDistance   , SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()), Qt::UniqueConnection);
	connect(leftDistance  , SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()), Qt::UniqueConnection);
	connect(rightDistance , SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()), Qt::UniqueConnection);
	connect(bottomDistance, SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()), Qt::UniqueConnection);
	connect(tabsButton    , SIGNAL(clicked())           , this, SLOT(handleTabs()), Qt::UniqueConnection);
}

void PropertyWidget_Distance::disconnectSignals()
{
	disconnect(columns       , SIGNAL(valueChanged(double))   , this, SLOT(handleColumns()));
	disconnect(columnGap     , SIGNAL(valueChanged(double)), this, SLOT(handleColumnGap()));
	disconnect(columnGapLabel, SIGNAL(activated(int))      , this, SLOT(handleGapSwitch()));
	disconnect(topDistance   , SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()));
	disconnect(leftDistance  , SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()));
	disconnect(rightDistance , SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()));
	disconnect(bottomDistance, SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()));
	disconnect(tabsButton    , SIGNAL(clicked())           , this, SLOT(handleTabs()));
}

void PropertyWidget_Distance::configureWidgets(void)
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		PageItem_TextFrame *textItem = m_item->asTextFrame();
		if (m_doc->appMode == modeEditTable)
			textItem = m_item->asTable()->activeCell().textFrame();

		enabled  = (textItem != NULL);
		enabled &= (m_doc->m_Selection->count() == 1);

		if (textItem)
		{
			int numCols = textItem->Cols;
			
			columnGap->setEnabled(numCols != 1);
			columnGapLabel->setEnabled(numCols != 1);
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
	repaint();
}

void PropertyWidget_Distance::handleUpdateRequest(int /*updateFlags*/)
{
	// Nothing to do in this widget
}

void PropertyWidget_Distance::displayColumns(int r, double g)
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

		if (textItem != 0)
		{
			columns->setMaximum(qMax(qRound(textItem->width() / qMax(textItem->ColGap, 10.0)), 1));
			if (columnGapLabel->currentIndex() == 0)
			{
				columnGap->setMaximum(qMax((textItem->width() / textItem->Cols - textItem->textToFrameDistLeft() - textItem->textToFrameDistRight()) * m_unitRatio, 0.0));
				columnGap->setValue(textItem->ColGap * m_unitRatio);
			}
			else
			{
				columnGap->setMaximum(qMax((textItem->width() / textItem->Cols) * m_unitRatio, 0.0));
				columnGap->setValue(textItem->columnWidth() * m_unitRatio);
			}
		}
	}
	columns->setMinimum(1);
	columnGap->setMinimum(0);
	columnGap->setEnabled(columns->value() != 1);
	columnGapLabel->setEnabled(columns->value() != 1);

	columns->blockSignals(cSigWasBlocked);
	columnGap->blockSignals(cGapSigWasBlocked);
}

void PropertyWidget_Distance::displayTextDistances(double left, double top, double bottom, double right)
{
	leftDistance->showValue(left * m_unitRatio);
	topDistance->showValue(top * m_unitRatio);
	bottomDistance->showValue(bottom * m_unitRatio);
	rightDistance->showValue(right * m_unitRatio);
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
		textItem->Cols = static_cast<int>(columns->value());
		displayColumns(textItem->Cols, textItem->ColGap);
		if (static_cast<int>(columns->value()) == 1)
		{
			columnGap->setEnabled(false);
			columnGapLabel->setEnabled(false);
		}
		else
		{
			columnGap->setEnabled(true);
			columnGapLabel->setEnabled(true);
		}
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
	if (!textItem) return;

	if (columnGapLabel->currentIndex() == 0)
		textItem->ColGap = columnGap->value() / m_unitRatio;
	else
	{
		double lineCorr;
		if ((textItem->lineColor() != CommonStrings::None) || (!textItem->strokePattern().isEmpty()))
			lineCorr = textItem->lineWidth();
		else
			lineCorr = 0;
		double newWidth = columnGap->value() / m_unitRatio;
		double newGap = qMax(((textItem->width() - textItem->textToFrameDistLeft() - textItem->textToFrameDistRight() - lineCorr) - (newWidth * textItem->Cols)) / (textItem->Cols - 1), 0.0);
		textItem->ColGap = newGap;
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
	if (textItem != NULL)
		displayColumns(textItem->Cols, textItem->ColGap);

	int index = columnGapLabel->currentIndex();
	columnGap->setToolTip((index == 0) ? tr( "Distance between columns" ) : tr( "Column width" ));
}

void PropertyWidget_Distance::handleTabs()
{
	if (m_doc && m_item)
	{
		PageItem_TextFrame *tItem = m_item->asTextFrame();
		if (tItem == 0)
			return;
		const ParagraphStyle& style(m_doc->appMode == modeEdit ? tItem->currentStyle() : tItem->itemText.defaultStyle());
		TabManager *dia = new TabManager(this, m_doc->unitIndex(), style.tabValues(), tItem->columnWidth());
		if (dia->exec())
		{
			if (m_doc->appMode != modeEdit)
			{
				ParagraphStyle newStyle(m_item->itemText.defaultStyle());
				newStyle.setTabValues(dia->tmpTab);
				m_item->itemText.setDefaultStyle(newStyle);
			}
			else
			{
				ParagraphStyle newStyle;
				newStyle.setTabValues(dia->tmpTab);
				m_doc->itemSelection_ApplyParagraphStyle(newStyle);
			}
			m_item->update();
		}
		delete dia;
	}
}

void PropertyWidget_Distance::handleTextDistances()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem *textItem = m_item;
	if (m_doc->appMode == modeEditTable)
		textItem = m_item->asTable()->activeCell().textFrame();
	if (!textItem) return;

	double left   = leftDistance->value() / m_unitRatio;
	double right  = rightDistance->value() / m_unitRatio;
	double top    = topDistance->value() / m_unitRatio;
	double bottom = bottomDistance->value() / m_unitRatio;
	textItem->setTextToFrameDist(left, right, top, bottom);
	displayColumns(textItem->Cols, textItem->ColGap);

	textItem->update();
	if (m_doc->appMode == modeEditTable)
		m_item->asTable()->update();
	m_doc->regionsChanged()->update(QRect());
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
	columnsLabel->setText( tr("Colu&mns:"));

	int oldcolgapLabel = columnGapLabel->currentIndex();
	columnGapLabel->clear();
	columnGapLabel->addItem( tr("Gap:"));
	columnGapLabel->addItem( tr("Width:"));
	columnGapLabel->setCurrentIndex(oldcolgapLabel);

	topLabel->setText( tr("To&p:"));
	bottomLabel->setText( tr("&Bottom:"));
	leftLabel->setText( tr("&Left:"));
	rightLabel->setText( tr("&Right:"));

	tabsButton->setText( tr("T&abulators..."));

	QString ptSuffix = tr(" pt");

	QString suffix = (m_doc) ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;

	columnGap->setSuffix(suffix);
	leftDistance->setSuffix(suffix);
	topDistance->setSuffix(suffix);
	bottomDistance->setSuffix(suffix);
	rightDistance->setSuffix(suffix);

	columns->setToolTip( tr("Number of columns in text frame"));
	columnGapLabel->setToolTip( tr("Switches between Gap or Column width"));
	columnGap->setToolTip( tr("Distance between columns"));
	topDistance->setToolTip( tr("Distance of text from top of frame"));
	bottomDistance->setToolTip( tr("Distance of text from bottom of frame"));
	leftDistance->setToolTip( tr("Distance of text from left of frame"));
	rightDistance->setToolTip( tr("Distance of text from right of frame"));
	tabsButton->setToolTip( tr("Edit tab settings of text frame..."));
}

void PropertyWidget_Distance::unitChange()
{
	if (!m_doc)
		return;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	columnGap->blockSignals(true);
	leftDistance->blockSignals(true);
	topDistance->blockSignals(true);
	bottomDistance->blockSignals(true);
	rightDistance->blockSignals(true);

	columnGap->setNewUnit( m_unitIndex );
	leftDistance->setNewUnit( m_unitIndex );
	topDistance->setNewUnit( m_unitIndex );
	bottomDistance->setNewUnit( m_unitIndex );
	rightDistance->setNewUnit( m_unitIndex );

	columnGap->blockSignals(false);
	leftDistance->blockSignals(false);
	topDistance->blockSignals(false);
	bottomDistance->blockSignals(false);
	rightDistance->blockSignals(false);
}