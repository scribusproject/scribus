/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "propertywidget_text.h"

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

PropertyWidget_Text::PropertyWidget_Text( QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	fontSize->setPrefix("");

	buttonTextColor->setContext(Context::Text);
	buttonTextColor->setMenuContextType(ColorButton::Floating);
	buttonTextColor->setColor( CommonStrings::tr_NoneColor);
	buttonTextColor->setIconSize(QSize(20, 20));
	buttonTextColor->setApplyColorOnIcon(true);

	buttonBackgroundColor->setContext(Context::TextBackground);
	buttonBackgroundColor->setMenuContextType(ColorButton::Floating);
	buttonBackgroundColor->setColor(CommonStrings::tr_NoneColor);
//	buttonBackgroundColor->setIconSize(QSize(16, 16));

	buttonStrokeColor->setContext(Context::TextOutline);
	buttonStrokeColor->setMenuContextType(ColorButton::Floating);
	buttonStrokeColor->setColor(CommonStrings::tr_NoneColor);
	buttonStrokeColor->setIconSize(QSize(20, 20));

	labelFontSize->setLabelVisibility(false);
	labelLineSpacing->setLabelVisibility(false);
	labelLineHeight->setLabelVisibility(false);

	fonts->toggleLabelVisibility(false);
	fonts->setGuestWidget(labelFontSize);

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));

	setEnabled(false);
}

void PropertyWidget_Text::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(UpdateRequest(int))     , this  , SLOT(handleUpdateRequest(int)));
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
	buttonTextColor->setDoc(m_doc);
	buttonBackgroundColor->setDoc(m_doc);
	buttonStrokeColor->setDoc(m_doc);

	if (m_doc.isNull())
	{
		disconnectSignals();
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

void PropertyWidget_Text::connectSignals()
{
	connect(lineSpacing   , SIGNAL(valueChanged(double)), this, SLOT(handleLineSpacing()));
	connect(fonts         , SIGNAL(fontSelected(QString)), this, SLOT(handleTextFont(QString)));
	connect(fontSize      , SIGNAL(valueChanged(double)), this, SLOT(handleFontSize()));
	connect(lineSpacingModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(handleLineSpacingMode(int)));
	connect(buttonTextColor			, SIGNAL(changed()), this, SLOT(handleTextFill()));
	connect(buttonBackgroundColor	, SIGNAL(changed()), this, SLOT(handleTextBackground()));
	connect(buttonStrokeColor, SIGNAL(changed()), this, SLOT(handleTextStroke()));
}

void PropertyWidget_Text::disconnectSignals()
{
	disconnect(lineSpacing   , SIGNAL(valueChanged(double)), this, SLOT(handleLineSpacing()));
	disconnect(fonts         , SIGNAL(fontSelected(QString)), this, SLOT(handleTextFont(QString)));
	disconnect(fontSize      , SIGNAL(valueChanged(double)), this, SLOT(handleFontSize()));
	disconnect(lineSpacingModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(handleLineSpacingMode(int)));
	disconnect(buttonTextColor			, SIGNAL(changed()), this, SLOT(handleTextFill()));
	disconnect(buttonBackgroundColor	, SIGNAL(changed()), this, SLOT(handleTextBackground()));
	disconnect(buttonStrokeColor, SIGNAL(changed()), this, SLOT(handleTextStroke()));
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
	if (updateFlags & reqDefFontListUpdate)
		fonts->rebuildList(nullptr);

	if (updateFlags & reqDocFontListUpdate)
		fonts->rebuildList(m_haveDoc ? m_doc : nullptr);
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

void PropertyWidget_Text::iconSetChange()
{
	IconManager& im = IconManager::instance();

	labelFontSize->setPixmap(im.loadPixmap("font-size"));
	labelLineSpacing->setPixmap(im.loadPixmap("paragraph-line-height"));
	buttonTextColor->setIcon(im.loadIcon("text-color", buttonTextColor->iconSize()));
	buttonStrokeColor->setIcon(im.loadIcon("text-outline-color", buttonStrokeColor->iconSize()));
}


void PropertyWidget_Text::toggleLabelVisibility(bool v)
{
	labelTextColor->setLabelVisibility(v);
	labelBackgroundColor->setLabelVisibility(v);
	labelStrokeColor->setLabelVisibility(v);
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
	m_doc->regionsChanged()->update(QRect());
}

void PropertyWidget_Text::showLineSpacing(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || !m_item || m_ScMW->scriptIsRunning())
		return;

	bool tmp = m_haveItem;
	double r = newCurrent.lineSpacing();
	m_haveItem = false;

	if (tmp)
	{
		if (newCurrent.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
		{
			double autoLS = static_cast<double>(m_item->doc()->typographicPrefs().autoLineSpacing) / 100.0;
			r = newCurrent.charStyle().font().height(newCurrent.charStyle().fontSize() / 10.0) * autoLS;
		}
		else if (newCurrent.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
			r = m_item->doc()->guidesPrefs().valueBaselineGrid;

		setupLineSpacingSpinbox(newCurrent.lineSpacingMode(), r);
		lineSpacingModeCombo->setCurrentIndex(newCurrent.lineSpacingMode());
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

void PropertyWidget_Text::setupLineSpacingSpinbox(int mode, double value)
{
	bool blocked = lineSpacing->blockSignals(true);
	lineSpacing->setValue(value);
	lineSpacing->setEnabled((mode == 0));
	lineSpacing->blockSignals(blocked);
}

void PropertyWidget_Text::updateCharStyle(const CharStyle& charStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	showFontFace(charStyle.font().scName());
	showFontSize(charStyle.fontSize());
	showTextColors(charStyle.fillColor(), charStyle.backColor(), charStyle.strokeColor(), charStyle.fillShade(), charStyle.backShade(), charStyle.strokeShade());
	labelStrokeColor->setEnabled( (charStyle.effects() & 4) || (charStyle.effects() & 256) );
}

void PropertyWidget_Text::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	const CharStyle& charStyle = newCurrent.charStyle();
	updateCharStyle(charStyle);
	showLineSpacing(newCurrent);
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

void PropertyWidget_Text::handleTextFont(const QString& font)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetFont(font, &tempSelection);
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
	lineSpacingModeCombo->addItem( tr("Fixed"));
	lineSpacingModeCombo->addItem( tr("Automatic"));
	lineSpacingModeCombo->addItem( tr("Baseline"));
	lineSpacingModeCombo->setCurrentIndex(oldLineSpacingMode);

	buttonTextColor->setPersistentToolTip( tr("Color of selected text"));
	buttonBackgroundColor->setPersistentToolTip( tr("Background color of selected text"));
	buttonStrokeColor->setPersistentToolTip( tr("Color of text stroke and/or drop shadow, depending which is chosen. If both are chosen, then they share the same color."));

}

void PropertyWidget_Text::handleTextFill()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != nullptr)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_blockUpdate = true;
		m_doc->itemSelection_SetFillColor(buttonTextColor->colorName(), &tempSelection);
		m_doc->itemSelection_SetFillShade(buttonTextColor->colorData().Shade, &tempSelection);
		m_blockUpdate = false;

	}
}

void PropertyWidget_Text::handleTextBackground()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != nullptr)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_blockUpdate = true;
		m_doc->itemSelection_SetBackgroundColor(buttonBackgroundColor->colorName(), &tempSelection);
		m_doc->itemSelection_SetBackgroundShade(buttonBackgroundColor->colorData().Shade, &tempSelection);
		m_blockUpdate = false;
	}
}

void PropertyWidget_Text::handleTextStroke()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != nullptr)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_blockUpdate = true;
		m_doc->itemSelection_SetStrokeColor(buttonStrokeColor->colorName(), &tempSelection);
		m_doc->itemSelection_SetStrokeShade(buttonStrokeColor->colorData().Shade, &tempSelection);
		m_blockUpdate = false;
	}
}

void PropertyWidget_Text::showTextColors(const QString& fillCol, const QString& backCol, const QString& strokeCol, double fillShd, double backShd, double strokeShd)
{

	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning() || m_blockUpdate)
		return;

	buttonTextColor->setColor(fillCol, qRound(fillShd));
	buttonTextColor->updatePreview();
	buttonTextColor->updateFloatingContext();

	buttonBackgroundColor->setColor(backCol, qRound(backShd));
	buttonBackgroundColor->updatePreview();
	buttonBackgroundColor->updateFloatingContext();

	buttonStrokeColor->setColor(strokeCol, qRound(strokeShd));
	buttonStrokeColor->updatePreview();
	buttonStrokeColor->updateFloatingContext();

}
