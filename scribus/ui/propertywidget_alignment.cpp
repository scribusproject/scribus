/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_alignment.h"

#include <QSignalBlocker>

#include "appmodehelper.h"
#include "appmodes.h"
#include "iconmanager.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"
#include "tabmanager.h"

PropertyWidget_Alignment::PropertyWidget_Alignment(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	layout()->setAlignment(Qt::AlignTop);

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));
}

void PropertyWidget_Alignment::updateStyle(const ParagraphStyle &newCurrent)
{
	bool tmp = m_haveItem;
	m_haveItem = false;

	textAlignment->setStyle(newCurrent.alignment(), newCurrent.direction());
	textDirection->setStyle(newCurrent.direction());

	m_haveItem = tmp;

	showOpticalMargins(newCurrent.opticalMargins());
}

void PropertyWidget_Alignment::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW->appModeHelper, SIGNAL(AppModeChanged(int,int)), this, SLOT(handleAppModeChanged(int,int)));
	connect(m_ScMW, SIGNAL(UpdateRequest(int))      , this, SLOT(handleUpdateRequest(int)));
}

void PropertyWidget_Alignment::setDoc(ScribusDoc *d)
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
	m_haveDoc  = true;
	m_haveItem = false;

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_Alignment::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be nullptr.
	//if (m_item == i)
	//	return;

	disconnectSignals();

	if (!m_doc)
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	configureWidgets();

	if (!m_item) return;


	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		setEnabled(false);
	}

	m_haveItem = true;

	textVerticalAlignment->setEnabled(m_item->isTextFrame() || m_item->isTable());

	if (m_item->isTextFrame() || m_item->isPathText() || m_item->isTable())
	{
		PageItem_TextFrame *textItem = m_item->asTextFrame();
		if (m_doc->appMode == modeEditTable)
			textItem = m_item->asTable()->activeCell().textFrame();
		if (textItem)
			textVerticalAlignment->setStyle(textItem->verticalAlignment());

		ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
		if (m_doc->appMode == modeEdit || m_doc->appMode == modeEditTable)
			m_item->currentTextProps(parStyle);

		updateStyle(parStyle);
		showFirstLinePolicy(m_item->firstLineOffset());
	}
	if (m_item->isOSGFrame())
	{
		setEnabled(false);
	}
	if (m_item->asSymbol())
	{
		setEnabled(false);
	}

	connectSignals();

}

void PropertyWidget_Alignment::connectSignals()
{
	connect(flopWidget->flopGroup, SIGNAL(idClicked(int)), this, SLOT(handleFirstLinePolicy(int)));
	connect(opticalMarginsWidget, SIGNAL(opticalMarginChanged()), this, SLOT(handleOpticalMargins()) );
	connect(textAlignment , SIGNAL(State(int))   , this, SLOT(handleAlignment(int)));
	connect(textVerticalAlignment, SIGNAL(State(int)), this, SLOT(handleVAlignment(int)));
	connect(textDirection , SIGNAL(State(int))   , this, SLOT(handleDirection(int)));
}

void PropertyWidget_Alignment::disconnectSignals()
{
	disconnect(flopWidget->flopGroup, SIGNAL(idClicked(int)), this, SLOT(handleFirstLinePolicy(int)));
	disconnect(opticalMarginsWidget, SIGNAL(opticalMarginChanged()), this, SLOT(handleOpticalMargins()) );
	disconnect(textAlignment , SIGNAL(State(int))   , this, SLOT(handleAlignment(int)));
	disconnect(textVerticalAlignment, SIGNAL(State(int)), this, SLOT(handleVAlignment(int)));
	disconnect(textDirection , SIGNAL(State(int))   , this, SLOT(handleDirection(int)));
}

void PropertyWidget_Alignment::configureWidgets()
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		PageItem_TextFrame *textItem = m_item->asTextFrame();
		if (m_doc->appMode == modeEditTable)
			textItem = m_item->asTable()->activeCell().textFrame();

		enabled  = (textItem != nullptr);

		if (m_item->isPathText())
			enabled = true;

		enabled &= (m_doc->m_Selection->count() == 1);

		// disable all options that don't work for path text
		textVerticalAlignment->setEnabled(!m_item->isPathText());
		flopWidget->setEnabled(!m_item->isPathText());
		opticalMarginsWidget->setEnabled(!m_item->isPathText());
	}
	setEnabled(enabled);
}

void PropertyWidget_Alignment::handleAppModeChanged(int oldMode, int mode)
{
	if (oldMode == modeEditTable || mode == modeEditTable)
	{
		setCurrentItem(m_item);
	}
}

void PropertyWidget_Alignment::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		//setEnabled(false);
		flopWidget->flopRealHeight->setChecked(true);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;
		m_haveItem = (itemType != -1);

		switch (itemType)
		{
		case -1:
			m_haveItem = false;
			m_item = nullptr;
			setEnabled(false);
			break;
		case PageItem::TextFrame:
		case PageItem::PathText:
			setEnabled(true);
			break;
		case PageItem::Table:
			setEnabled(m_doc->appMode == modeEditTable);
			break;
		default:
			setEnabled(false);
			break;
		}
	}
	if (currItem)
	{
		setCurrentItem(currItem);
	}
	updateGeometry();
}

void PropertyWidget_Alignment::showFirstLinePolicy(FirstLineOffsetPolicy f)
{
	if (f == FLOPFontAscent)
		flopWidget->flopFontAscent->setChecked(true);
	else if (f == FLOPLineSpacing)
		flopWidget->flopLineSpacing->setChecked(true);
	else if (f == FLOPRealGlyphHeight)
		flopWidget->flopRealHeight->setChecked(true); //It’s historical behaviour.
	else // if (f == FLOPBaseline)
		flopWidget->flopBaselineGrid->setChecked(true);
}

void PropertyWidget_Alignment::showOpticalMargins(int e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	opticalMarginsWidget->setOpticalMargin(e);
	m_haveItem = tmp;
}

void PropertyWidget_Alignment::showAlignment(int e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	textAlignment->setEnabled(true);
	textAlignment->setStyle(e, textDirection->getStyle());
	m_haveItem = tmp;
}

void PropertyWidget_Alignment::showDirection(int e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	textDirection->setEnabled(true);
	textDirection->setStyle(e);
	m_haveItem = tmp;
}

void PropertyWidget_Alignment::handleAlignment(int a)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetAlignment(a, &tempSelection);

}

void PropertyWidget_Alignment::handleVAlignment(int a)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	PageItem *textItem = m_item;
	if (m_doc->appMode == modeEditTable)
		textItem = m_item->asTable()->activeCell().textFrame();
	if (textItem != nullptr)
	{
		textItem->setVerticalAlignment(a);
		textItem->update();
		if (m_doc->appMode == modeEditTable)
			m_item->asTable()->update();
		m_doc->regionsChanged()->update(QRect());
	}
}

void PropertyWidget_Alignment::handleDirection(int d)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetDirection(d, &tempSelection);
	// If current text alignment is left or right, change it to match direction
	if (d == ParagraphStyle::RTL && textAlignment->selectedId() == ParagraphStyle::LeftAligned)
	{
		m_doc->itemSelection_SetAlignment(ParagraphStyle::RightAligned, &tempSelection);
		textAlignment->setTypeStyle(ParagraphStyle::RightAligned);
	}
	else if (d == ParagraphStyle::LTR && textAlignment->selectedId() == ParagraphStyle::RightAligned)
	{
		m_doc->itemSelection_SetAlignment(ParagraphStyle::LeftAligned, &tempSelection);
		textAlignment->setTypeStyle(ParagraphStyle::LeftAligned);
	}
}

void PropertyWidget_Alignment::handleFirstLinePolicy(int radioFlop)
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (radioFlop == FlopWidget::RealHeightID)
		m_item->setFirstLineOffset(FLOPRealGlyphHeight);
	else if (radioFlop == FlopWidget::FontAscentID)
		m_item->setFirstLineOffset(FLOPFontAscent);
	else if (radioFlop == FlopWidget::LineSpacingID)
		m_item->setFirstLineOffset(FLOPLineSpacing);
	else if (radioFlop == FlopWidget::BaselineGridID)
		m_item->setFirstLineOffset(FLOPBaselineGrid);
	m_item->update();
	if (m_doc->appMode == modeEditTable)
		m_item->parentTable()->update();
	else
		m_item->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertyWidget_Alignment::handleOpticalMargins()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	int omt(opticalMarginsWidget->opticalMargin());

	PageItem *item = m_item;
	if (m_doc->appMode == modeEditTable)
		item = m_item->asTable()->activeCell().textFrame();
	if (item != nullptr)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(item, true);
		m_doc->itemSelection_SetOpticalMargins(omt, &tempSelection);
	}
}

// void PropertyWidget_Alignment::resetOpticalMargins()
// {
// 	// if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
// 	// 	return;
// 	// PageItem *item = m_item;
// 	// if (m_doc->appMode == modeEditTable)
// 	// 	item = m_item->asTable()->activeCell().textFrame();
// 	// if (item != nullptr)
// 	// {
// 	// 	Selection tempSelection(this, false);
// 	// 	tempSelection.addItem(item, true);
// 	// 	m_doc->itemSelection_resetOpticalMargins(&tempSelection);
// 	// }
// }

void PropertyWidget_Alignment::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_Alignment::languageChange()
{
	retranslateUi(this);

	textAlignment->languageChange();
	textDirection->languageChange();
}

void PropertyWidget_Alignment::unitChange()
{
	if (!m_doc)
		return;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	flopWidget->unitChange();
}

void PropertyWidget_Alignment::toggleLabelVisibility(bool v)
{
	flopWidget->setLabelVisibility(v);
	opticalMarginsWidget->setLabelVisibility(v);
	textAlignment->setLabelVisibility(v);
	textVerticalAlignment->setLabelVisibility(v);
	textDirection->setLabelVisibility(v);
}
