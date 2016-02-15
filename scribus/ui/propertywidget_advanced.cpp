/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_advanced.h"

#include "appmodes.h"
#include "iconmanager.h"
#include "pageitem_table.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "selection.h"
#include "units.h"

PropertyWidget_Advanced::PropertyWidget_Advanced(QWidget* parent) : QFrame(parent)
{
	m_item = NULL;
	m_ScMW = NULL;
	m_unitIndex = 0;
	m_unitRatio = 1.0;

	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);

	layout()->setAlignment( Qt::AlignLeft );

	textBase->setValue( 0 );
	textBaseLabel->setPixmap(IconManager::instance()->loadPixmap("textbase.png"));
	trackingLabel->setPixmap(IconManager::instance()->loadPixmap("textkern.png"));

	scaleH->setValues(10, 400, 2, 100 );
	scaleHLabel->setPixmap(IconManager::instance()->loadPixmap("textscaleh.png"));

	scaleV->setValues(10, 400, 2, 100 );
	scaleVLabel->setPixmap(IconManager::instance()->loadPixmap("textscalev.png"));

	minWordTrackingLabel->setBuddy(minWordTrackingSpinBox);
	normWordTrackingLabel->setBuddy(normWordTrackingSpinBox);

	minGlyphExtensionLabel->setBuddy(minGlyphExtSpinBox);
	maxGlyphExtensionLabel->setBuddy(maxGlyphExtSpinBox);

	languageChange();
}

void PropertyWidget_Advanced::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;
}

void PropertyWidget_Advanced::setDoc(ScribusDoc *d)
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
	//FIXME: This won't work until when a canvas deselect happens, m_item must be NULL.
	//if (m_item == i)
	//	return;

	if (item && m_doc.isNull())
		setDoc(item->doc());

	m_item = item;

	disconnectSignals();
	configureWidgets();

	if (m_item)
	{
		if (m_item->asTextFrame() || m_item->asPathText() || m_item->asTable())
		{
			ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
			if (m_doc->appMode == modeEdit)
				m_item->currentTextProps(parStyle);
			else if (m_doc->appMode == modeEditTable)
				m_item->asTable()->activeCell().textFrame()->currentTextProps(parStyle);
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
}

void PropertyWidget_Advanced::configureWidgets(void)
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		if (m_item->asPathText() || m_item->asTextFrame() || m_item->asTable())
			enabled = true;
		if ((m_item->isGroup()) && (!m_item->isSingleSel))
			enabled = false;
		if (m_item->asOSGFrame() || m_item->asSymbolFrame())
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
	repaint();
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

void PropertyWidget_Advanced::handleBaselineOffset()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != NULL)
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
	if (i2 != NULL)
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
	if (i2 != NULL)
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
	if (i2 != NULL)
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
	if (i2 != NULL)
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
	if (i2 != NULL)
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
	if (i2 != NULL)
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
	if (i2 != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_doc->itemSelection_SetTracking(qRound(tracking->value() * 10.0), &tempSelection);
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

void PropertyWidget_Advanced::languageChange()
{
	wordTrackingLabel->setText( tr("Word Tracking"));
	minWordTrackingLabel->setText( tr("Min:"));
	normWordTrackingLabel->setText( tr("Norm:"));
	glyphExtensionLabel->setText( tr("Glyph Extension"));
	minGlyphExtensionLabel->setText( tr("Min:"));
	maxGlyphExtensionLabel->setText( tr("Max:"));

	QString pctSuffix=tr(" %");
	textBase->setSuffix(pctSuffix);
	scaleH->setSuffix(pctSuffix);
	scaleV->setSuffix(pctSuffix);
	tracking->setSuffix(pctSuffix);
	minWordTrackingSpinBox->setSuffix(pctSuffix);
	normWordTrackingSpinBox->setSuffix(pctSuffix);
	minGlyphExtSpinBox->setSuffix(pctSuffix);
	maxGlyphExtSpinBox->setSuffix(pctSuffix);

	textBase->setToolTip( tr("Offset to baseline of characters"));
	tracking->setToolTip( tr("Manual Tracking"));
	scaleH->setToolTip( tr("Scaling width of characters"));
	scaleV->setToolTip( tr("Scaling height of characters"));

	minWordTrackingSpinBox->setToolTip( tr("Minimal width of spaces between words"));
	normWordTrackingSpinBox->setToolTip( tr("Normal width of spaces between words"));
	minGlyphExtSpinBox->setToolTip( tr("Minimal shrinkage of glyphs for justification"));
	maxGlyphExtSpinBox->setToolTip( tr("Maximal extension of glyphs for justification"));
}
