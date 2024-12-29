/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_advanced.h"

#include "appmodes.h"
#include "iconmanager.h"
#include "localemgr.h"
#include "pageitem_table.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"

PropertyWidget_Advanced::PropertyWidget_Advanced(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	layout()->setAlignment(Qt::AlignLeft);

	textBase->setValue(0);

	scaleH->setValues(10, 400, 2, 100);
	scaleV->setValues(10, 400, 2, 100);

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));
}

void PropertyWidget_Advanced::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;
}

void PropertyWidget_Advanced::setDoc(ScribusDoc *d)
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

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	tracking->setValues( -300, 300, 2, 0);
	minWordTrackingSpinBox->setValues(1, 100, 2, 100);
	normWordTrackingSpinBox->setValues(1, 2000, 2, 100);
	minGlyphExtSpinBox->setValues(90, 110, 2, 100);
	maxGlyphExtSpinBox->setValues(90, 110, 2, 100);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_Advanced::setCurrentItem(PageItem *item)
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

	if (m_item)
	{
		if (m_item->isTextFrame() || m_item->isPathText() || m_item->isTable())
		{
			ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
			if (m_doc->appMode == modeEdit || m_doc->appMode == modeEditTable)
				m_item->currentTextProps(parStyle);
			updateStyle(parStyle);
		}
		connectSignals();
	}
}

void PropertyWidget_Advanced::connectSignals()
{
	connect(textBase, SIGNAL(valueChanged(double)), this, SLOT(handleBaselineOffset()));
	connect(tracking, SIGNAL(valueChanged(double)), this, SLOT(handleTracking()));
	connect(scaleH  , SIGNAL(valueChanged(double)), this, SLOT(handleTextScaleH()));
	connect(scaleV  , SIGNAL(valueChanged(double)), this, SLOT(handleTextScaleV()));
	connect(minWordTrackingSpinBox , SIGNAL(valueChanged(double)), this, SLOT(handleMinWordTracking()) );
	connect(normWordTrackingSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleNormWordTracking()) );
	connect(minGlyphExtSpinBox     , SIGNAL(valueChanged(double)), this, SLOT(handleMinGlyphExtension()) );
	connect(maxGlyphExtSpinBox     , SIGNAL(valueChanged(double)), this, SLOT(handleMaxGlyphExtension()) );

	connect(textEffects, SIGNAL(State(int))		, this, SLOT(handleTypeStyle(int)), Qt::UniqueConnection);
	connect(textEffects->ShadowVal->Xoffset		, SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()), Qt::UniqueConnection);
	connect(textEffects->ShadowVal->Yoffset		, SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()), Qt::UniqueConnection);
	connect(textEffects->OutlineVal->LWidth		, SIGNAL(valueChanged(double)), this, SLOT(handleOutlineWidth()), Qt::UniqueConnection);
	connect(textEffects->UnderlineVal->LPos		, SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()) , Qt::UniqueConnection);
	connect(textEffects->UnderlineVal->LWidth	, SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()) , Qt::UniqueConnection);
	connect(textEffects->StrikeVal->LPos		, SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()), Qt::UniqueConnection);
	connect(textEffects->StrikeVal->LWidth		, SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()), Qt::UniqueConnection);
}

void PropertyWidget_Advanced::disconnectSignals()
{
	disconnect(textBase, SIGNAL(valueChanged(double)), this, SLOT(handleBaselineOffset()));
	disconnect(tracking, SIGNAL(valueChanged(double)), this, SLOT(handleTracking()));
	disconnect(scaleH  , SIGNAL(valueChanged(double)), this, SLOT(handleTextScaleH()));
	disconnect(scaleV  , SIGNAL(valueChanged(double)), this, SLOT(handleTextScaleV()));
	disconnect(minWordTrackingSpinBox , SIGNAL(valueChanged(double)), this, SLOT(handleMinWordTracking()) );
	disconnect(normWordTrackingSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleNormWordTracking()) );
	disconnect(minGlyphExtSpinBox     , SIGNAL(valueChanged(double)), this, SLOT(handleMinGlyphExtension()) );
	disconnect(maxGlyphExtSpinBox     , SIGNAL(valueChanged(double)), this, SLOT(handleMaxGlyphExtension()) );

	disconnect(textEffects, SIGNAL(State(int))	, this, SLOT(handleTypeStyle(int)));
	disconnect(textEffects->ShadowVal->Xoffset  , SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()));
	disconnect(textEffects->ShadowVal->Yoffset  , SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()));
	disconnect(textEffects->OutlineVal->LWidth  , SIGNAL(valueChanged(double)), this, SLOT(handleOutlineWidth()));
	disconnect(textEffects->UnderlineVal->LPos  , SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()));
	disconnect(textEffects->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()));
	disconnect(textEffects->StrikeVal->LPos     , SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()));
	disconnect(textEffects->StrikeVal->LWidth   , SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()));
}

void PropertyWidget_Advanced::configureWidgets()
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

void PropertyWidget_Advanced::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
}

void PropertyWidget_Advanced::showBaseLineOffset(double e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textBase->showValue(e / 10.0);
}

void PropertyWidget_Advanced::showTextScaleH(double e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	scaleH->showValue(e / 10.0);
}

void PropertyWidget_Advanced::showTextScaleV(double e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	scaleV->showValue(e / 10.0);
}

void PropertyWidget_Advanced::showTracking(double e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	tracking->showValue(e / 10.0);
}

void PropertyWidget_Advanced::showOutlineW(double x)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->OutlineVal->LWidth->showValue(x / 10.0);
}

void PropertyWidget_Advanced::showShadowOffset(double x, double y)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->ShadowVal->Xoffset->showValue(x / 10.0);
	textEffects->ShadowVal->Yoffset->showValue(y / 10.0);
}

void PropertyWidget_Advanced::showStrikeThru(double p, double w)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->StrikeVal->LPos->showValue(p / 10.0);
	textEffects->StrikeVal->LWidth->showValue(w / 10.0);
}

void PropertyWidget_Advanced::showTextEffects(int s)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	textEffects->setStyle(s);
}

void PropertyWidget_Advanced::showUnderline(double p, double w)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->UnderlineVal->LPos->showValue(p / 10.0);
	textEffects->UnderlineVal->LWidth->showValue(w / 10.0);
}

void PropertyWidget_Advanced::handleBaselineOffset()
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
		m_doc->itemSelection_SetBaselineOffset(qRound(textBase->value() * 10), &tempSelection);
	}
}

void PropertyWidget_Advanced::handleMinWordTracking()
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
		ParagraphStyle newStyle;
		newStyle.setMinWordTracking(minWordTrackingSpinBox->value() / 100.0);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
	}
}

void PropertyWidget_Advanced::handleNormWordTracking()
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
		ParagraphStyle newStyle;
		newStyle.charStyle().setWordTracking(normWordTrackingSpinBox->value() / 100.0);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
	}
}

void PropertyWidget_Advanced::handleMinGlyphExtension()
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
		ParagraphStyle newStyle;
		newStyle.setMinGlyphExtension(minGlyphExtSpinBox->value() / 100.0);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
	}
}

void PropertyWidget_Advanced::handleMaxGlyphExtension()
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
		ParagraphStyle newStyle;
		newStyle.setMaxGlyphExtension(maxGlyphExtSpinBox->value() / 100.0);
		m_doc->itemSelection_ApplyParagraphStyle(newStyle, &tempSelection);
	}
}

void PropertyWidget_Advanced::handleTextScaleH()
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
		m_doc->itemSelection_SetScaleH(qRound(scaleH->value() * 10), &tempSelection);
	}
}

void PropertyWidget_Advanced::handleTextScaleV()
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
		m_doc->itemSelection_SetScaleV(qRound(scaleV->value() * 10), &tempSelection);
	}
}

void PropertyWidget_Advanced::handleTracking()
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
		m_doc->itemSelection_SetTracking(qRound(tracking->value() * 10.0), &tempSelection);
	}
}

void PropertyWidget_Advanced::handleUnderline()
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

void PropertyWidget_Advanced::handleOutlineWidth()
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

void PropertyWidget_Advanced::handleShadowOffs()
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

void PropertyWidget_Advanced::handleTypeStyle(int s)
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

void PropertyWidget_Advanced::handleStrikeThru()
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

void PropertyWidget_Advanced::updateCharStyle(const CharStyle& charStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	showTextScaleH(charStyle.scaleH());
	showTextScaleV(charStyle.scaleV());
	showTracking(charStyle.tracking());
	showBaseLineOffset(charStyle.baselineOffset());
	showOutlineW(charStyle.outlineWidth());
	showShadowOffset(charStyle.shadowXOffset(), charStyle.shadowYOffset());
	showTextEffects(charStyle.effects());
	showStrikeThru(charStyle.strikethruOffset(), charStyle.strikethruWidth());
	showUnderline(charStyle.underlineOffset(), charStyle.underlineWidth());

	normWordTrackingSpinBox->showValue(charStyle.wordTracking() * 100.0);
}

void PropertyWidget_Advanced::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	const CharStyle& charStyle = newCurrent.charStyle();

	showTextScaleH(charStyle.scaleH());
	showTextScaleV(charStyle.scaleV());
	showTracking(charStyle.tracking());
	showBaseLineOffset(charStyle.baselineOffset());
	showOutlineW(charStyle.outlineWidth());
	showShadowOffset(charStyle.shadowXOffset(), charStyle.shadowYOffset());
	showTextEffects(charStyle.effects());
	showStrikeThru(charStyle.strikethruOffset(), charStyle.strikethruWidth());
	showUnderline(charStyle.underlineOffset(), charStyle.underlineWidth());

	minWordTrackingSpinBox->showValue(newCurrent.minWordTracking() * 100.0);
	normWordTrackingSpinBox->showValue(newCurrent.charStyle().wordTracking() * 100.0);
	minGlyphExtSpinBox->showValue(newCurrent.minGlyphExtension() * 100.0);
	maxGlyphExtSpinBox->showValue(newCurrent.maxGlyphExtension() * 100.0);
}

void PropertyWidget_Advanced::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_Advanced::iconSetChange()
{
	IconManager &im = IconManager::instance();

	textBaseLabel->setPixmap(im.loadPixmap("character-offset-baseline"));
	trackingLabel->setPixmap(im.loadPixmap("character-letter-tracking"));

	scaleHLabel->setPixmap(im.loadPixmap("character-scale-width"));
	scaleVLabel->setPixmap(im.loadPixmap("character-scale-height"));

	minWordTrackingLabel->setPixmap(im.loadPixmap("character-space-width-min"));
	normWordTrackingLabel->setPixmap(im.loadPixmap("character-space-width"));

	minGlyphExtensionLabel->setPixmap(im.loadPixmap("character-scale-width-min"));
	maxGlyphExtensionLabel->setPixmap(im.loadPixmap("character-scale-width-max"));

}

void PropertyWidget_Advanced::languageChange()
{
	retranslateUi(this);

	textEffects->languageChange();
}

void PropertyWidget_Advanced::localeChange()
{
	const QLocale& l(LocaleManager::instance().userPreferredLocale());
	textBase->setLocale(l);
	tracking->setLocale(l);
	scaleH->setLocale(l);
	scaleV->setLocale(l);
	minWordTrackingSpinBox->setLocale(l);
	normWordTrackingSpinBox->setLocale(l);
	minGlyphExtSpinBox->setLocale(l);
	maxGlyphExtSpinBox->setLocale(l);

	textEffects->ShadowVal->Xoffset->setLocale(l);
	textEffects->ShadowVal->Yoffset->setLocale(l);
	textEffects->OutlineVal->LWidth->setLocale(l);
	textEffects->UnderlineVal->LPos->setLocale(l);
	textEffects->UnderlineVal->LWidth->setLocale(l);
	textEffects->StrikeVal->LPos->setLocale(l);
	textEffects->StrikeVal->LWidth->setLocale(l);
}

void PropertyWidget_Advanced::toggleLabelVisibility(bool v)
{	
	minWordTrackingLabel->setLabelVisibility(v);
	normWordTrackingLabel->setLabelVisibility(v);
	minGlyphExtensionLabel->setLabelVisibility(v);
	maxGlyphExtensionLabel->setLabelVisibility(v);
	glyphLabel->setLabelVisibility(v);
	spaceLabel->setLabelVisibility(v);

}
