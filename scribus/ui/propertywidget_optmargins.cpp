/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pageitem_table.h"
#include "propertywidget_optmargins.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "selection.h"
#include "units.h"
#include "util_icon.h"

PropertyWidget_OptMargins::PropertyWidget_OptMargins(QWidget* parent) : QFrame(parent)
{
	m_item = 0;
	m_ScMW = 0;

	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);
	layout()->setAlignment( Qt::AlignTop );

	languageChange();
}

void PropertyWidget_OptMargins::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(AppModeChanged(int, int)), this, SLOT(handleAppModeChanged(int, int)));
	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this  , SLOT(handleUpdateRequest(int)));
}

void PropertyWidget_OptMargins::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
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

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_OptMargins::setCurrentItem(PageItem *item)
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

	if (m_item)
	{
		if (m_item->asTextFrame() || m_item->asPathText())
		{
			ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
			if (m_doc->appMode == modeEdit)
				m_item->currentTextProps(parStyle);
			displayOpticalMargins(parStyle);
		}

		connectSignals();
	}
}

void PropertyWidget_OptMargins::connectSignals()
{
	connect(optMarginRadioNone  , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	connect(optMarginRadioBoth  , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	connect(optMarginRadioLeft  , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	connect(optMarginRadioRight , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	connect(optMarginResetButton, SIGNAL(clicked()), this, SLOT(resetOpticalMargins()) );
}

void PropertyWidget_OptMargins::disconnectSignals()
{
	disconnect(optMarginRadioNone  , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	disconnect(optMarginRadioBoth  , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	disconnect(optMarginRadioLeft  , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	disconnect(optMarginRadioRight , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	disconnect(optMarginResetButton, SIGNAL(clicked()), this, SLOT(resetOpticalMargins()) );
}

void PropertyWidget_OptMargins::configureWidgets(void)
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		PageItem_TextFrame* textItem = m_item->asTextFrame();
		if (m_doc->appMode == modeEditTable)
			textItem = m_item->asTable()->activeCell().textFrame();

		enabled  = (m_item->isPathText() || (textItem != NULL));
		enabled &= (m_doc->m_Selection->count() == 1);
	}
	setEnabled(enabled);
}

void PropertyWidget_OptMargins::handleAppModeChanged(int oldMode, int mode)
{
	if (oldMode == modeEditTable || mode == modeEditTable)
	{
		setCurrentItem(m_item);
	}
}

void PropertyWidget_OptMargins::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
	repaint();
}

void PropertyWidget_OptMargins::displayOpticalMargins(const ParagraphStyle & pStyle)
{
	ParagraphStyle::OpticalMarginType omt(static_cast<ParagraphStyle::OpticalMarginType>(pStyle.opticalMargins()));
	bool blocked = optMarginRadioBoth->blockSignals(true);
	if (omt == ParagraphStyle::OM_Default)
		optMarginRadioBoth->setChecked(true);
	else if (omt == ParagraphStyle::OM_LeftHangingPunct)
		optMarginRadioLeft->setChecked(true);
	else if (omt == ParagraphStyle::OM_RightHangingPunct)
		optMarginRadioRight->setChecked(true);
	else
		optMarginRadioNone->setChecked(true);
	optMarginRadioBoth->blockSignals(blocked);
}

void PropertyWidget_OptMargins::updateStyle(const ParagraphStyle& newCurrent)
{
	displayOpticalMargins(newCurrent);
}

void PropertyWidget_OptMargins::handleOpticalMargins()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	int omt(ParagraphStyle::OM_None);
	if (optMarginRadioBoth->isChecked())
		omt = ParagraphStyle::OM_Default;
	else if (optMarginRadioLeft->isChecked())
		omt = ParagraphStyle::OM_LeftHangingPunct;
	else if (optMarginRadioRight->isChecked())
		omt = ParagraphStyle::OM_RightHangingPunct;

	PageItem *item = m_item;
	if (m_doc->appMode == modeEditTable)
		item = m_item->asTable()->activeCell().textFrame();
	if (item != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(item, true);
		m_doc->itemSelection_SetOpticalMargins(omt, &tempSelection);
	}
}

void PropertyWidget_OptMargins::resetOpticalMargins()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	PageItem *item = m_item;
	if (m_doc->appMode == modeEditTable)
		item = m_item->asTable()->activeCell().textFrame();
	if (item != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(item, true);
		m_doc->itemSelection_resetOpticalMargins(&tempSelection);
	}
}

void PropertyWidget_OptMargins::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_OptMargins::languageChange()
{
	optMarginRadioNone->setText( tr("None","optical margins") );
	optMarginRadioBoth->setText( tr("Both Sides","optical margins") );
	optMarginRadioLeft->setText( tr("Left Only","optical margins") );
	optMarginRadioRight->setText( tr("Right Only","optical margins") );
	optMarginResetButton->setText( tr("Reset") );
}