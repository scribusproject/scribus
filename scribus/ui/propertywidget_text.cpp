/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "propertywidget_text.h"
#include "ui_propertywidget_textbase.h"

#include "appmodes.h"
#include "commonstrings.h"
#include "fontcombo.h"
#include "iconmanager.h"
#include "localemgr.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "scribus.h"
#include "scribusapp.h"
#include "selection.h"
#include "stylecombos.h"
#include "undomanager.h"
#include "langmgr.h"


PropertyWidget_Text::PropertyWidget_Text( QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	fontSize->setPrefix("");

	iconSetChange();
	languageChange();

	connect(lineSpacing   , SIGNAL(valueChanged(double)), this, SLOT(handleLineSpacing()));
	connect(fonts         , SIGNAL(fontSelected(QString)), this, SLOT(handleTextFont(QString)));
	connect(fontSize      , SIGNAL(valueChanged(double)), this, SLOT(handleFontSize()));
	connect(textAlignment , SIGNAL(State(int))   , this, SLOT(handleAlignment(int)));
	connect(textDirection , SIGNAL(State(int))   , this, SLOT(handleDirection(int)));
	connect(charStyleClear, SIGNAL(clicked()), this, SLOT(doClearCStyle()));
	connect(paraStyleClear, SIGNAL(clicked()), this, SLOT(doClearPStyle()));

	connect(lineSpacingModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(handleLineSpacingMode(int)));
	connect(langCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLang(int)));

//	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
//	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));

	setEnabled(false);
}

void PropertyWidget_Text::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

//	connect(m_ScMW, SIGNAL(UpdateRequest(int))     , this  , SLOT(handleUpdateRequest(int)));

	connect(paraStyleCombo, SIGNAL(newStyle(QString)), m_ScMW, SLOT(setNewParStyle(QString)), Qt::UniqueConnection);
	connect(charStyleCombo, SIGNAL(newStyle(QString)), m_ScMW, SLOT(setNewCharStyle(QString)), Qt::UniqueConnection);
}

void PropertyWidget_Text::setDoc(ScribusDoc *d)
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

	fontSize->setValues(0.5, 2048, 2, 1);
	lineSpacing->setValues(1, 2048, 2, 1);

	fonts->rebuildList(m_doc);
	paraStyleCombo->setDoc(m_doc);
	charStyleCombo->setDoc(m_doc);

	if (m_doc.isNull())
	{
		return;
	}

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

PageItem* PropertyWidget_Text::currentItemFromSelection()
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

void PropertyWidget_Text::handleSelectionChanged()
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

void PropertyWidget_Text::handleUpdateRequest(int updateFlags)
{
	// ColorWidget will handle its update itself
	/*if (updateFlags & reqColorsUpdate)
		updateColorList();*/
	if (updateFlags & reqCharStylesUpdate)
	{
		charStyleCombo->updateStyleList();
	}
	if (updateFlags & reqParaStylesUpdate)
		paraStyleCombo->updateStyleList();
	if (updateFlags & reqDefFontListUpdate)
		fonts->rebuildList(nullptr);
	if (updateFlags & reqDocFontListUpdate)
		fonts->rebuildList(m_haveDoc ? m_doc : nullptr);
	if (updateFlags & reqStyleComboDocUpdate)
	{
		paraStyleCombo->setDoc(m_haveDoc ? m_doc : nullptr);
		charStyleCombo->setDoc(m_haveDoc ? m_doc : nullptr);
	}
}

void PropertyWidget_Text::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be nullptr.
	//if (m_item == i)
	//	return;

	if (!m_doc)
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		setEnabled(false);
	}

	m_haveItem = true;


	if (m_item->isTextFrame() || m_item->isPathText() || m_item->isTable())
	{
		ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
		if (m_doc->appMode == modeEdit || m_doc->appMode == modeEditTable)
			m_item->currentTextProps(parStyle);
		updateStyle(parStyle);
	}
	if (m_item->isOSGFrame())
	{
		setEnabled(false);
	}
	if (m_item->asSymbol())
	{
		setEnabled(false);
	}
}

void PropertyWidget_Text::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();

	fontSizeLabel->setPixmap(iconManager.loadPixmap("zeichen.png"));
	lineSpacingLabel->setPixmap(iconManager.loadPixmap("linespacing2.png"));
	textAlignmentLabel->setPixmap(iconManager.loadPixmap("22/text-align.png"));
	langLabel->setPixmap(iconManager.loadPixmap("22/language.png"));
	paraStyleLabel->setPixmap(iconManager.loadPixmap("22/paragraph-style.png"));
	charStyleLabel->setPixmap(iconManager.loadPixmap("22/character-style.png"));

	paraStyleClear->setIcon(iconManager.loadPixmap("16/edit-clear.png"));
	charStyleClear->setIcon(iconManager.loadPixmap("16/edit-clear.png"));
}


void PropertyWidget_Text::localeChange()
{
	const QLocale& l(LocaleManager::instance().userPreferredLocale());
	fontSize->setLocale(l);
	lineSpacing->setLocale(l);
}

void PropertyWidget_Text::handleLineSpacingMode(int id)
{
	if (!m_haveDoc || !m_haveItem)
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetLineSpacingMode(id, &tempSelection);
	updateStyle(((m_doc->appMode == modeEdit) || (m_doc->appMode == modeEditTable)) ? m_item->currentStyle() : m_item->itemText.defaultStyle());
	m_doc->regionsChanged()->update(QRect());
}

void PropertyWidget_Text::changeLang(int id)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	QStringList languageList;
	LanguageManager::instance()->fillInstalledStringList(&languageList);
	QString abrv = LanguageManager::instance()->getAbbrevFromLang(languageList.value(id),false);
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetLanguage(abrv, &tempSelection);
}

void PropertyWidget_Text::showLineSpacing(double r)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool inEditMode = (m_doc->appMode == modeEdit || m_doc->appMode == modeEditTable);
	bool tmp = m_haveItem;
	m_haveItem = false;
	lineSpacing->showValue(r);
	const ParagraphStyle& curStyle(m_haveItem && inEditMode ? m_item->currentStyle() : m_item->itemText.defaultStyle());
	if (tmp)
	{
		setupLineSpacingSpinbox(curStyle.lineSpacingMode(), r);
		lineSpacingModeCombo->setCurrentIndex(curStyle.lineSpacingMode());
	}
	m_haveItem = tmp;
}

void PropertyWidget_Text::showFontFace(const QString& newFont)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	if (m_item != nullptr)
		fonts->rebuildList(m_doc, m_item->isAnnotation());
	fonts->setCurrentFont(newFont);
	m_haveItem = tmp;
}

void PropertyWidget_Text::showFontSize(double s)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	fontSize->showValue(s / 10.0);
}

void PropertyWidget_Text::showLanguage(const QString& w)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	QStringList lang;
	LanguageManager::instance()->fillInstalledStringList(&lang);
	QString langName = LanguageManager::instance()->getLangFromAbbrev(w, true);

	bool sigBlocked  = langCombo->blockSignals(true);
	langCombo->setCurrentIndex(lang.indexOf(langName));
	langCombo->blockSignals(sigBlocked);
}

void PropertyWidget_Text::setupLineSpacingSpinbox(int mode, double value)
{
	bool blocked = lineSpacing->blockSignals(true);
	if (mode > 0)
	{
		if (mode == 1)
			lineSpacing->setSpecialValueText( tr( "Auto" ) );
		if (mode == 2)
			lineSpacing->setSpecialValueText( tr( "Baseline" ) );
		lineSpacing->setMinimum(0);
		lineSpacing->setValue(0);
		lineSpacing->setEnabled(false);
	}
	else
	{
		lineSpacing->setSpecialValueText("");
		lineSpacing->setMinimum(1);
		lineSpacing->setValue(value);
		lineSpacing->setEnabled(true);
	}
	lineSpacing->blockSignals(blocked);
}

void PropertyWidget_Text::updateCharStyle(const CharStyle& charStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	showFontFace(charStyle.font().scName());
	showFontSize(charStyle.fontSize());
	showLanguage(charStyle.language());
}

void PropertyWidget_Text::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	const CharStyle& charStyle = newCurrent.charStyle();

	showFontFace(charStyle.font().scName());
	showFontSize(charStyle.fontSize());
	showLanguage(charStyle.language());

	if (m_item)
	{
		QString defaultParStyle = m_item->itemText.defaultStyle().parent();
		if (defaultParStyle.isEmpty())
			defaultParStyle = CommonStrings::DefaultParagraphStyle;
		paraStyleCombo->setDefaultStyle(defaultParStyle);
	}
	showParStyle(newCurrent.parent());

	if (m_item)
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
	}
	showCharStyle(charStyle.parent());

	bool tmp = m_haveItem;
	m_haveItem = false;

	setupLineSpacingSpinbox(newCurrent.lineSpacingMode(), newCurrent.lineSpacing());
	lineSpacingModeCombo->setCurrentIndex(newCurrent.lineSpacingMode());
	textAlignment->setStyle(newCurrent.alignment(), newCurrent.direction());
	textDirection->setStyle(newCurrent.direction());

	m_haveItem = tmp;
}

void PropertyWidget_Text::updateCharStyles()
{
	charStyleCombo->updateStyleList();
}

void PropertyWidget_Text::updateParagraphStyles()
{
	paraStyleCombo->updateStyleList();
	charStyleCombo->updateStyleList();
}

void PropertyWidget_Text::updateTextStyles()
{
	paraStyleCombo->updateStyleList();
	charStyleCombo->updateStyleList();
}

void PropertyWidget_Text::showAlignment(int e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	textAlignment->setEnabled(true);
	textAlignment->setStyle(e, textDirection->getStyle());
	m_haveItem = tmp;
}

void PropertyWidget_Text::showDirection(int e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	textDirection->setEnabled(true);
	textDirection->setStyle(e);
	m_haveItem = tmp;
}

void PropertyWidget_Text::showCharStyle(const QString& name)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool blocked = charStyleCombo->blockSignals(true);
	charStyleCombo->setStyle(name);
	charStyleCombo->blockSignals(blocked);
}

void PropertyWidget_Text::showParStyle(const QString& name)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool blocked = paraStyleCombo->blockSignals(true);
	paraStyleCombo->setStyle(name);
	paraStyleCombo->blockSignals(blocked);
}

void PropertyWidget_Text::handleLineSpacing()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetLineSpacing(lineSpacing->value(), &tempSelection);
}

void PropertyWidget_Text::handleFontSize()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetFontSize(qRound(fontSize->value()*10.0), &tempSelection);
}

void PropertyWidget_Text::handleAlignment(int a)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetAlignment(a, &tempSelection);

	emit alignmentHandled();

//	if (m_item->isPathText())
//		pathTextWidgets->handleSelectionChanged();
}

void PropertyWidget_Text::handleDirection(int d)
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

void PropertyWidget_Text::handleTextFont(const QString& font)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetFont(font, &tempSelection);
}

void PropertyWidget_Text::doClearCStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveDoc || !m_haveItem)
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_EraseCharStyle(&tempSelection);
}


void PropertyWidget_Text::doClearPStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveDoc || !m_haveItem)
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

void PropertyWidget_Text::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_Text::languageChange()
{
	retranslateUi(this);

	QSignalBlocker lineSpacingModeBlocker(lineSpacingModeCombo);
	int oldLineSpacingMode = lineSpacingModeCombo->currentIndex();
	lineSpacingModeCombo->clear();
	lineSpacingModeCombo->addItem( tr("Fixed Linespacing"));
	lineSpacingModeCombo->addItem( tr("Automatic Linespacing"));
	lineSpacingModeCombo->addItem( tr("Align to Baseline Grid"));
	lineSpacingModeCombo->setCurrentIndex(oldLineSpacingMode);

	QSignalBlocker langComboBlocker(langCombo);
	QStringList languageList;
	LanguageManager::instance()->fillInstalledStringList(&languageList);
	int oldLang = langCombo->currentIndex();
	langCombo->clear();
	langCombo->addItems(languageList);
	langCombo->setCurrentIndex(oldLang);

	textAlignment->languageChange();
	textDirection->languageChange();
}
