/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QLocale>
#include "appmodes.h"
#include "iconmanager.h"
#include "localemgr.h"
#include "pageitem_table.h"
#include "propertywidget_textcolor.h"
#include "scribus.h"
#include "scribusapp.h"
#include "selection.h"

PropertyWidget_TextColor::PropertyWidget_TextColor(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	layout()->setAlignment(Qt::AlignLeft);

	fillLayout->setAlignment(Qt::AlignLeft);
	fillColor->setPixmapType(ColorCombo::fancyPixmaps);
	fillIcon->setScaledContents( false );

	strokeLayout->setAlignment(Qt::AlignLeft);
	strokeColor->setPixmapType(ColorCombo::fancyPixmaps);
	strokeIcon->setScaledContents( false );

	backLayout->setAlignment(Qt::AlignLeft);
	backColor->setPixmapType(ColorCombo::fancyPixmaps);
	backIcon->setScaledContents( false );

	effectsLayout->setAlignment(Qt::AlignLeft);

	iconSetChange();
	languageChange();

	strokeIcon->setEnabled(false);
	strokeColor->setEnabled(false);
	strokeShade->setEnabled(false);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));

}

void PropertyWidget_TextColor::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
}

void PropertyWidget_TextColor::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
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

	updateColorList();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_TextColor::setCurrentItem(PageItem *item)
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

	m_item = item;
	if (item && m_doc.isNull())
		setDoc(item->doc());

	configureWidgets();

	if (m_item == nullptr)
		return;
	if (!m_item->isTable() && !m_item->isTextFrame() && !m_item->isPathText())
		return;

	if (m_item->isTextFrame() || m_item->isPathText() || m_item->isTable())
	{
		ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
		if (m_doc->appMode == modeEdit || m_doc->appMode == modeEditTable)
			m_item->currentTextProps(parStyle);
		updateStyle(parStyle);
	}
	connectSignals();
}

void PropertyWidget_TextColor::connectSignals()
{
	connect(fillColor   , SIGNAL(activated(int)), this, SLOT(handleTextFill())     , Qt::UniqueConnection);
	connect(strokeColor , SIGNAL(activated(int)), this, SLOT(handleTextStroke())   , Qt::UniqueConnection);
	connect(fillShade   , SIGNAL(clicked())     , this, SLOT(handleTextShade())    , Qt::UniqueConnection);
	connect(strokeShade , SIGNAL(clicked())     , this, SLOT(handleTextShade())    , Qt::UniqueConnection);
	connect(backShade   , SIGNAL(clicked())     , this, SLOT(handleTextShade())    , Qt::UniqueConnection);
	connect(backColor   , SIGNAL(activated(int)), this, SLOT(handleTextBackground())     , Qt::UniqueConnection);

	connect(textEffects, SIGNAL(State(int))      , this, SLOT(handleTypeStyle(int)), Qt::UniqueConnection);
	connect(textEffects->ShadowVal->Xoffset  , SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()), Qt::UniqueConnection);
	connect(textEffects->ShadowVal->Yoffset  , SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()), Qt::UniqueConnection);
	connect(textEffects->OutlineVal->LWidth  , SIGNAL(valueChanged(double)), this, SLOT(handleOutlineWidth()), Qt::UniqueConnection);
	connect(textEffects->UnderlineVal->LPos  , SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()) , Qt::UniqueConnection);
	connect(textEffects->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()) , Qt::UniqueConnection);
	connect(textEffects->StrikeVal->LPos     , SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()), Qt::UniqueConnection);
	connect(textEffects->StrikeVal->LWidth   , SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()), Qt::UniqueConnection);
}

void PropertyWidget_TextColor::disconnectSignals()
{
	disconnect(fillColor   , SIGNAL(activated(int)), this, SLOT(handleTextFill()));
	disconnect(strokeColor , SIGNAL(activated(int)), this, SLOT(handleTextStroke()));
	disconnect(fillShade   , SIGNAL(clicked())     , this, SLOT(handleTextShade()));
	disconnect(strokeShade , SIGNAL(clicked())     , this, SLOT(handleTextShade()));
	disconnect(backShade   , SIGNAL(clicked())     , this, SLOT(handleTextShade()));
	disconnect(backColor   , SIGNAL(activated(int)), this, SLOT(handleTextBackground()));

	disconnect(textEffects, SIGNAL(State(int))      , this, SLOT(handleTypeStyle(int)));
	disconnect(textEffects->ShadowVal->Xoffset  , SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()));
	disconnect(textEffects->ShadowVal->Yoffset  , SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()));
	disconnect(textEffects->OutlineVal->LWidth  , SIGNAL(valueChanged(double)), this, SLOT(handleOutlineWidth()));
	disconnect(textEffects->UnderlineVal->LPos  , SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()));
	disconnect(textEffects->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()));
	disconnect(textEffects->StrikeVal->LPos     , SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()));
	disconnect(textEffects->StrikeVal->LWidth   , SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()));
}

void PropertyWidget_TextColor::configureWidgets()
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		if (m_item->isPathText() || m_item->isTextFrame() || m_item->isTable())
			enabled = true;
		if ((m_item->isGroup()) && (!m_item->isSingleSel))
			enabled = false;
		if (m_item->isOSGFrame() || m_item->isSymbol())
			enabled = false;
		if (m_doc->m_Selection->count() > 1)
			enabled = false;
	}
	setEnabled(enabled);
}

void PropertyWidget_TextColor::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
}

void PropertyWidget_TextColor::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		updateColorList();
}

void PropertyWidget_TextColor::updateColorList()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (m_item)
		disconnectSignals();

	fillColor->setColors(m_doc->PageColors, true);
	strokeColor->setColors(m_doc->PageColors, false);
	backColor->setColors(m_doc->PageColors, true);
	fillColor->view()->setMinimumWidth(fillColor->view()->maximumViewportSize().width() + 24);
	strokeColor->view()->setMinimumWidth(strokeColor->view()->maximumViewportSize().width() + 24);
	backColor->view()->setMinimumWidth(backColor->view()->maximumViewportSize().width() + 24);

	if (m_item)
		setCurrentItem(m_item);
}

void PropertyWidget_TextColor::updateCharStyle(const CharStyle& charStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	showOutlineW  (charStyle.outlineWidth());
	showShadowOffset(charStyle.shadowXOffset(), charStyle.shadowYOffset());
	showTextColors(charStyle.strokeColor(), charStyle.fillColor(), charStyle.backColor(), charStyle.strokeShade(), charStyle.fillShade(), charStyle.backShade());
	showTextEffects(charStyle.effects());
	showStrikeThru(charStyle.strikethruOffset()  , charStyle.strikethruWidth());
	showUnderline (charStyle.underlineOffset(), charStyle.underlineWidth());
}

void PropertyWidget_TextColor::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	const CharStyle& charStyle = newCurrent.charStyle();

	showOutlineW  (charStyle.outlineWidth());
	showShadowOffset(charStyle.shadowXOffset(), charStyle.shadowYOffset());
	showTextColors(charStyle.strokeColor(), charStyle.fillColor(), charStyle.backColor(), charStyle.strokeShade(), charStyle.fillShade(), charStyle.backShade());
	showTextEffects(charStyle.effects());
	showStrikeThru(charStyle.strikethruOffset()  , charStyle.strikethruWidth());
	showUnderline (charStyle.underlineOffset(), charStyle.underlineWidth());
}

void PropertyWidget_TextColor::showOutlineW(double x)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->OutlineVal->LWidth->showValue(x / 10.0);
}

void PropertyWidget_TextColor::showShadowOffset(double x, double y)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->ShadowVal->Xoffset->showValue(x / 10.0);
	textEffects->ShadowVal->Yoffset->showValue(y / 10.0);
}

void PropertyWidget_TextColor::showStrikeThru(double p, double w)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->StrikeVal->LPos->showValue(p / 10.0);
	textEffects->StrikeVal->LWidth->showValue(w / 10.0);
}

void PropertyWidget_TextColor::showTextColors(const QString& strokeCol, const QString& fillCol, const QString& backCol, double strokeShd, double fillShd, double backShd)
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	QSignalBlocker fillShadeBlocker(fillShade);
	QSignalBlocker strokeShadeBlocker(strokeShade);
	QSignalBlocker backShadeBlocker(backShade);
	QSignalBlocker fillColorBlocker(fillColor);
	QSignalBlocker strokeColorBlocker(strokeColor);
	QSignalBlocker backColorBlocker(backColor);

	fillShade->setValue(qRound(fillShd));
	strokeShade->setValue(qRound(strokeShd));
	backShade->setValue(qRound(backShd));

	if (!fillCol.isEmpty())
		fillColor->setCurrentColor(fillCol);
	if (!strokeCol.isEmpty())
		strokeColor->setCurrentColor(strokeCol);
	if (!backCol.isEmpty())
		backColor->setCurrentColor(backCol);
}

void PropertyWidget_TextColor::showTextEffects(int s)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	strokeIcon->setEnabled(false);
	strokeColor->setEnabled(false);
	strokeShade->setEnabled(false);
	textEffects->setStyle(s);
	if ((s & 4) || (s & 256))
	{
		strokeIcon->setEnabled(true);
		strokeColor->setEnabled(true);
		strokeShade->setEnabled(true);
	}
}

void PropertyWidget_TextColor::handleOutlineWidth()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	int x = qRound(textEffects->OutlineVal->LWidth->value() * 10.0);
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != nullptr)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_doc->itemSelection_SetOutlineWidth(x, &tempSelection);
	}
}

void PropertyWidget_TextColor::handleShadowOffs()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	int x = qRound(textEffects->ShadowVal->Xoffset->value() * 10.0);
	int y = qRound(textEffects->ShadowVal->Yoffset->value() * 10.0);
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != nullptr)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_doc->itemSelection_SetShadowOffsets(x, y, &tempSelection);
	}
}

void PropertyWidget_TextColor::handleStrikeThru()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	int x = qRound(textEffects->StrikeVal->LPos->value() * 10.0);
	int y = qRound(textEffects->StrikeVal->LWidth->value() * 10.0);
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != nullptr)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_doc->itemSelection_SetStrikethru(x, y, &tempSelection);
	}
}

void PropertyWidget_TextColor::handleTextFill()
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
		m_doc->itemSelection_SetFillColor(fillColor->currentColor(), &tempSelection);
	}
}

void PropertyWidget_TextColor::handleTextStroke()
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
		m_doc->itemSelection_SetStrokeColor(strokeColor->currentColor(), &tempSelection);
	}
}

void PropertyWidget_TextColor::handleTextBackground()
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
		m_doc->itemSelection_SetBackgroundColor(backColor->currentColor(), &tempSelection);
	}
}

void PropertyWidget_TextColor::handleTextShade()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (strokeShade == sender())
	{
		int b = strokeShade->getValue();
		PageItem *i2 = m_item;
		if (m_doc->appMode == modeEditTable)
			i2 = m_item->asTable()->activeCell().textFrame();
		if (i2 != nullptr)
		{
			Selection tempSelection(this, false);
			tempSelection.addItem(i2, true);
			m_doc->itemSelection_SetStrokeShade(b, &tempSelection);
		}
	}
	else if (fillShade == sender())
	{
		int b = fillShade->getValue();
		PageItem *i2 = m_item;
		if (m_doc->appMode == modeEditTable)
			i2 = m_item->asTable()->activeCell().textFrame();
		if (i2 != nullptr)
		{
			Selection tempSelection(this, false);
			tempSelection.addItem(i2, true);
			m_doc->itemSelection_SetFillShade(b, &tempSelection);
		}
	}
	else if (backShade == sender())
	{
		int b = backShade->getValue();
		PageItem *i2 = m_item;
		if (m_doc->appMode == modeEditTable)
			i2 = m_item->asTable()->activeCell().textFrame();
		if (i2 != nullptr)
		{
			Selection tempSelection(this, false);
			tempSelection.addItem(i2, true);
			m_doc->itemSelection_SetBackgroundShade(b, &tempSelection);
		}
	}
}

void PropertyWidget_TextColor::handleTypeStyle(int s)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != nullptr)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_doc->itemSelection_SetEffects(s, &tempSelection);
		m_ScMW->setStyleEffects(s);
	}
}

void PropertyWidget_TextColor::showUnderline(double p, double w)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->UnderlineVal->LPos->showValue(p / 10.0);
	textEffects->UnderlineVal->LWidth->showValue(w / 10.0);
}

void PropertyWidget_TextColor::handleUnderline()
{
	if ((m_doc) && (m_item))
	{
		int x = qRound(textEffects->UnderlineVal->LPos->value() * 10.0);
		int y = qRound(textEffects->UnderlineVal->LWidth->value() * 10.0);
		PageItem *i2 = m_item;
		if (m_doc->appMode == modeEditTable)
			i2 = m_item->asTable()->activeCell().textFrame();
		if (i2 != nullptr)
		{
			Selection tempSelection(this, false);
			tempSelection.addItem(i2, true);
			m_doc->itemSelection_SetUnderline(x, y, &tempSelection);
		}
	}
}

void PropertyWidget_TextColor::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_TextColor::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();

	fillIcon->setPixmap(iconManager.loadPixmap("16/color-fill.png"));
	fillShadeLabel->setPixmap(iconManager.loadPixmap("shade.png"));

	strokeIcon->setPixmap(iconManager.loadPixmap("16/color-stroke.png"));
	strokeShadeLabel->setPixmap(iconManager.loadPixmap("shade.png"));

	backIcon->setPixmap(iconManager.loadPixmap("16/color-fill.png"));
	backShadeLabel->setPixmap(iconManager.loadPixmap("shade.png"));
}

void PropertyWidget_TextColor::languageChange()
{
	retranslateUi(this);
	textEffects->languageChange();
}

void PropertyWidget_TextColor::localeChange()
{
	const QLocale& l(LocaleManager::instance().userPreferredLocale());
	textEffects->ShadowVal->Xoffset->setLocale(l);
	textEffects->ShadowVal->Yoffset->setLocale(l);
	textEffects->OutlineVal->LWidth->setLocale(l);
	textEffects->UnderlineVal->LPos->setLocale(l);
	textEffects->UnderlineVal->LWidth->setLocale(l);
	textEffects->StrikeVal->LPos->setLocale(l);
	textEffects->StrikeVal->LWidth->setLocale(l);
}
