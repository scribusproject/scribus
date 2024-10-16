/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "propertywidget_paragraphstyle.h"

#include "appmodes.h"
#include "commonstrings.h"
#include "fontcombo.h"
#include "iconmanager.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribus.h"
#include "scribusapp.h"
#include "selection.h"
#include "stylecombos.h"
#include "stylemanager.h"
#include "undomanager.h"

PropertyWidget_ParagraphStyle::PropertyWidget_ParagraphStyle( QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));

	setEnabled(false);
}

void PropertyWidget_ParagraphStyle::hideParagraphStyle(bool hide)
{
	m_hasParagraph = !hide;
	labelParagraphStyle->setVisible(!hide);
}

void PropertyWidget_ParagraphStyle::hideCharacterStyle(bool hide)
{
	m_hasCharacter = !hide;
	labelCharacterStyle->setVisible(!hide);
}

void PropertyWidget_ParagraphStyle::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(UpdateRequest(int))     , this  , SLOT(handleUpdateRequest(int)));

	if (m_hasParagraph)
		connect(paraStyleCombo, SIGNAL(newStyle(QString)), m_ScMW, SLOT(setNewParStyle(QString)), Qt::UniqueConnection);

	if (m_hasCharacter)
		connect(charStyleCombo, SIGNAL(newStyle(QString)), m_ScMW, SLOT(setNewCharStyle(QString)), Qt::UniqueConnection);
}

void PropertyWidget_ParagraphStyle::setDoc(ScribusDoc *d)
{
	if ((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
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

	if (m_hasParagraph)
		paraStyleCombo->setDoc(m_doc);

	if (m_hasCharacter)
		charStyleCombo->setDoc(m_doc);

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

PageItem* PropertyWidget_ParagraphStyle::currentItemFromSelection()
{
	PageItem *currentItem = nullptr;

	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 1)
			currentItem = m_doc->m_Selection->itemAt(0);
		else if (m_doc->m_Selection->count() == 1)
			currentItem = m_doc->m_Selection->itemAt(0);
		if (currentItem  && currentItem->isTable() && m_doc->appMode == modeEditTable)
			currentItem = currentItem->asTable()->activeCell().textFrame();
	}

	return currentItem;
}

void PropertyWidget_ParagraphStyle::connectSignals()
{
	if (m_hasCharacter)
	{
		connect(charStyleClear, SIGNAL(clicked()), this, SLOT(doClearCStyle()));
		connect(charStyleNew, SIGNAL(clicked()), this, SLOT(newCStyle()));
		connect(charStyleEdit, SIGNAL(clicked()), this, SLOT(editCStyle()));
	}

	if (m_hasParagraph)
	{
		connect(paraStyleClear, SIGNAL(clicked()), this, SLOT(doClearPStyle()));
		connect(paraStyleNew, SIGNAL(clicked()), this, SLOT(newPStyle()));
		connect(paraStyleEdit, SIGNAL(clicked()), this, SLOT(editPStyle()));
	}
}

void PropertyWidget_ParagraphStyle::disconnectSignals()
{
	disconnect(charStyleClear, SIGNAL(clicked()), this, SLOT(doClearCStyle()));
	disconnect(paraStyleClear, SIGNAL(clicked()), this, SLOT(doClearPStyle()));
	disconnect(charStyleNew, SIGNAL(clicked()), this, SLOT(newCStyle()));
	disconnect(paraStyleNew, SIGNAL(clicked()), this, SLOT(newPStyle()));
	disconnect(charStyleEdit, SIGNAL(clicked()), this, SLOT(editCStyle()));
	disconnect(paraStyleEdit, SIGNAL(clicked()), this, SLOT(editPStyle()));
}

void PropertyWidget_ParagraphStyle::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		setEnabled(false);
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

void PropertyWidget_ParagraphStyle::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqCharStylesUpdate && m_hasCharacter)
		charStyleCombo->updateStyleList();

	if (updateFlags & reqParaStylesUpdate && m_hasParagraph)
		paraStyleCombo->updateStyleList();

	if (updateFlags & reqStyleComboDocUpdate)
	{
		if (m_hasParagraph)
			paraStyleCombo->setDoc(m_haveDoc ? m_doc : nullptr);

		if (m_hasCharacter)
			charStyleCombo->setDoc(m_haveDoc ? m_doc : nullptr);
	}
}

void PropertyWidget_ParagraphStyle::setCurrentItem(PageItem *item)
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
	m_haveItem = true;

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		setEnabled(false);
	}

	if (m_item->isTextFrame() || m_item->isPathText() || m_item->isTable())
	{
		ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
		if (m_doc->appMode == modeEdit || m_doc->appMode == modeEditTable)
			m_item->currentTextProps(parStyle);
		updateStyle(parStyle);

		setEnabled(true);
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

void PropertyWidget_ParagraphStyle::iconSetChange()
{
	IconManager& im = IconManager::instance();

	labelParagraphStyle->setPixmap(im.loadPixmap("paragraph-style"));
	paraStyleClear->setIcon(im.loadPixmap("reset"));
	paraStyleNew->setIcon(im.loadPixmap("paragraph-style-new"));
	paraStyleEdit->setIcon(im.loadPixmap("paragraph-style-edit"));

	labelCharacterStyle->setPixmap(im.loadPixmap("character-style"));
	charStyleClear->setIcon(im.loadPixmap("reset"));
	charStyleNew->setIcon(im.loadPixmap("character-style-new"));
	charStyleEdit->setIcon(im.loadPixmap("character-style-edit"));

}

void PropertyWidget_ParagraphStyle::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || ! m_item || m_ScMW->scriptIsRunning())
		return;

	if (m_hasParagraph)
	{
		QString defaultParStyle = m_item->itemText.defaultStyle().parent();
		if (defaultParStyle.isEmpty())
			defaultParStyle = CommonStrings::DefaultParagraphStyle;
		paraStyleCombo->setDefaultStyle(defaultParStyle);
		showParStyle(newCurrent.parent());

	}

	if (m_hasCharacter)
	{		
		QString defaultCharStyle;
		if (!newCurrent.parent().isEmpty())
		{
			const ParagraphStyle* paraStyle = m_doc->paragraphStyles().getPointer(newCurrent.parent());
			if (paraStyle)
				defaultCharStyle = paraStyle->charStyle().parent();
			if (defaultCharStyle.isEmpty())
				defaultCharStyle = CommonStrings::DefaultCharacterStyle;
		}
		if (defaultCharStyle.isEmpty())
			defaultCharStyle = m_item->itemText.defaultStyle().charStyle().parent();
		if (defaultCharStyle.isEmpty())
			defaultCharStyle = CommonStrings::DefaultCharacterStyle;
		charStyleCombo->setDefaultStyle(defaultCharStyle);

		const CharStyle& charStyle = newCurrent.charStyle();
		showCharStyle(charStyle.parent());
	}

}

void PropertyWidget_ParagraphStyle::updateTextStyles()
{
	if (m_hasParagraph)
		paraStyleCombo->updateStyleList();

	if (m_hasCharacter)
		charStyleCombo->updateStyleList();
}

void PropertyWidget_ParagraphStyle::showCharStyle(const QString& name)
{
	if (!m_ScMW || !m_hasCharacter || m_ScMW->scriptIsRunning())
		return;
	bool blocked = charStyleCombo->blockSignals(true);
	charStyleCombo->setStyle(name);
	charStyleCombo->blockSignals(blocked);

	charStyleEdit->setEnabled(!charStyleCombo->currentStyle().isEmpty());
}

void PropertyWidget_ParagraphStyle::showParStyle(const QString& name)
{
	if (!m_ScMW || !m_hasParagraph || m_ScMW->scriptIsRunning())
		return;
	bool blocked = paraStyleCombo->blockSignals(true);
	paraStyleCombo->setStyle(name);
	paraStyleCombo->blockSignals(blocked);

	paraStyleEdit->setEnabled(!paraStyleCombo->currentStyle().isEmpty());
}

void PropertyWidget_ParagraphStyle::doClearCStyle()
{
	if (!m_ScMW || !m_hasCharacter || m_ScMW->scriptIsRunning() || !m_haveDoc || !m_haveItem)
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_EraseCharStyle(&tempSelection);
}

void PropertyWidget_ParagraphStyle::doClearPStyle()
{
	if (!m_ScMW || !m_hasParagraph || m_ScMW->scriptIsRunning() || !m_haveDoc || !m_haveItem)
		return;

	UndoTransaction activeTransaction;
	if (UndoManager::undoEnabled())
		activeTransaction = UndoManager::instance()->beginTransaction(Um::SelectionGroup, Um::IGroup, Um::RemoveTextStyle, tr( "remove direct paragraph formatting" ), Um::IFont);

	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_ClearBulNumStrings(&tempSelection);
	m_doc->itemSelection_EraseParagraphStyle(&tempSelection);
	CharStyle emptyCStyle;
	m_doc->itemSelection_SetCharStyle(emptyCStyle, &tempSelection);

	if (activeTransaction)
		activeTransaction.commit();
}

void PropertyWidget_ParagraphStyle::newPStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	m_ScMW->styleMgr()->showAsNewParagraphStyle();
}

void PropertyWidget_ParagraphStyle::newCStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	m_ScMW->styleMgr()->showAsNewCharacterStyle();
}

void PropertyWidget_ParagraphStyle::editPStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (!paraStyleCombo->currentStyle().isEmpty())
		m_ScMW->styleMgr()->showAsEditParagraphStyle(paraStyleCombo->currentStyle());
}

void PropertyWidget_ParagraphStyle::editCStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (!charStyleCombo->currentStyle().isEmpty())
		m_ScMW->styleMgr()->showAsEditCharacterStyle(charStyleCombo->currentStyle());
}

void PropertyWidget_ParagraphStyle::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_ParagraphStyle::languageChange()
{
	retranslateUi(this);
}
