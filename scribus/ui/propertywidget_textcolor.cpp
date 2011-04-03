/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertywidget_textcolor.h"

#include "scribus.h"
#include "selection.h"
#include "util_icon.h"

PropertyWidget_TextColor::PropertyWidget_TextColor(QWidget* parent) : QFrame(parent)
{
	m_item = NULL;
	m_ScMW = NULL;

	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);

	layout()->setAlignment( Qt::AlignLeft );

	fillLayout->setAlignment( Qt::AlignLeft );
	fillIcon->setPixmap(loadIcon("16/color-fill.png"));
	fillIcon->setScaledContents( false );
	fillShadeLabel->setPixmap(loadIcon("shade.png"));

	strokeLayout->setAlignment( Qt::AlignLeft );
	strokeIcon->setPixmap(loadIcon("16/color-stroke.png"));
	strokeIcon->setScaledContents( false );
	strokeShadeLabel->setPixmap(loadIcon("shade.png"));

	effectsLayout->setAlignment( Qt::AlignLeft );
	revertButton->setIcon(loadIcon("Revers.png"));
	revertButton->setCheckable( true );

	languageChange();

	strokeIcon->setEnabled(false);
	strokeColor->setEnabled(false);
	strokeShade->setEnabled(false);
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
	m_item = NULL;

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
	enableFromSelection();

	if (m_item)
	{
		revertButton->setChecked(m_item->reversed());
		if (m_item->asTextFrame() || m_item->asPathText())
		{
			updateStyle(m_doc->appMode == modeEdit? m_item->currentStyle() : m_item->itemText.defaultStyle());
		}
		connectSignals();
	}
}

void PropertyWidget_TextColor::connectSignals()
{
	connect(revertButton, SIGNAL(clicked())     , this, SLOT(handleTextDirection()), Qt::UniqueConnection);
	connect(fillColor   , SIGNAL(activated(int)), this, SLOT(handleTextFill())     , Qt::UniqueConnection);
	connect(strokeColor , SIGNAL(activated(int)), this, SLOT(handleTextStroke())   , Qt::UniqueConnection);
	connect(fillShade   , SIGNAL(clicked())     , this, SLOT(handleTextShade())    , Qt::UniqueConnection);
	connect(strokeShade , SIGNAL(clicked())     , this, SLOT(handleTextShade())    , Qt::UniqueConnection);

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
	disconnect(revertButton, SIGNAL(clicked())     , this, SLOT(handleTextDirection()));
	disconnect(fillColor   , SIGNAL(activated(int)), this, SLOT(handleTextFill()));
	disconnect(strokeColor , SIGNAL(activated(int)), this, SLOT(handleTextStroke()));
	disconnect(fillShade   , SIGNAL(clicked())     , this, SLOT(handleTextShade()));
	disconnect(strokeShade , SIGNAL(clicked())     , this, SLOT(handleTextShade()));

	disconnect(textEffects, SIGNAL(State(int))      , this, SLOT(handleTypeStyle(int)));
	disconnect(textEffects->ShadowVal->Xoffset  , SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()));
	disconnect(textEffects->ShadowVal->Yoffset  , SIGNAL(valueChanged(double)), this, SLOT(handleShadowOffs()));
	disconnect(textEffects->OutlineVal->LWidth  , SIGNAL(valueChanged(double)), this, SLOT(handleOutlineWidth()));
	disconnect(textEffects->UnderlineVal->LPos  , SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()));
	disconnect(textEffects->UnderlineVal->LWidth, SIGNAL(valueChanged(double)), this, SLOT(handleUnderline()));
	disconnect(textEffects->StrikeVal->LPos     , SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()));
	disconnect(textEffects->StrikeVal->LWidth   , SIGNAL(valueChanged(double)), this, SLOT(handleStrikeThru()));
}

void PropertyWidget_TextColor::enableFromSelection(void)
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		if (m_item->asPathText() || m_item->asTextFrame())
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

void PropertyWidget_TextColor::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
	repaint();
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

	fillColor->updateBox(m_doc->PageColors, ColorCombo::fancyPixmaps, true);
	strokeColor->updateBox(m_doc->PageColors, ColorCombo::fancyPixmaps, false);
	fillColor->view()->setMinimumWidth(fillColor->view()->maximumViewportSize().width() + 24);
	strokeColor->view()->setMinimumWidth(strokeColor->view()->maximumViewportSize().width() + 24);
}

void PropertyWidget_TextColor::updateCharStyle(const CharStyle& charStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	displayOutlineW  (charStyle.outlineWidth());
	displayShadowOffset(charStyle.shadowXOffset(), charStyle.shadowYOffset());
	displayTextColors(charStyle.strokeColor(), charStyle.fillColor(), charStyle.strokeShade(), charStyle.fillShade());
	displayTextEffects(charStyle.effects());
	displayStrikeThru(charStyle.strikethruOffset()  , charStyle.strikethruWidth());
	displayUnderline (charStyle.underlineOffset(), charStyle.underlineWidth());
}

void PropertyWidget_TextColor::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	const CharStyle& charStyle = newCurrent.charStyle();

	displayOutlineW  (charStyle.outlineWidth());
	displayShadowOffset(charStyle.shadowXOffset(), charStyle.shadowYOffset());
	displayTextColors(charStyle.strokeColor(), charStyle.fillColor(), charStyle.strokeShade(), charStyle.fillShade());
	displayTextEffects(charStyle.effects());
	displayStrikeThru(charStyle.strikethruOffset()  , charStyle.strikethruWidth());
	displayUnderline (charStyle.underlineOffset(), charStyle.underlineWidth());
}

void PropertyWidget_TextColor::displayOutlineW(double x)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->OutlineVal->LWidth->showValue(x / 10.0);
}

void PropertyWidget_TextColor::displayShadowOffset(double x, double y)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->ShadowVal->Xoffset->showValue(x / 10.0);
	textEffects->ShadowVal->Yoffset->showValue(y / 10.0);
}

void PropertyWidget_TextColor::displayStrikeThru(double p, double w)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	textEffects->StrikeVal->LPos->showValue(p / 10.0);
	textEffects->StrikeVal->LWidth->showValue(w / 10.0);
}

void PropertyWidget_TextColor::displayTextColors(QString p, QString b, double shp, double shb)
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	ColorList::Iterator it;
	int c = 0;
	fillShade->setValue(qRound(shb));
	strokeShade->setValue(qRound(shp));
	if ((b != CommonStrings::None) && (!b.isEmpty()))
	{
		c++;
		for (it = m_doc->PageColors.begin(); it != m_doc->PageColors.end(); ++it)
		{
			if (it.key() == b)
				break;
			c++;
		}
	}
	fillColor->setCurrentIndex(c);
	c = 0;
	if ((p != CommonStrings::None) && (!p.isEmpty()))
	{
		for (it = m_doc->PageColors.begin(); it != m_doc->PageColors.end(); ++it)
		{
			if (it.key() == p)
				break;
			c++;
		}
	}
	strokeColor->setCurrentIndex(c);
}

void PropertyWidget_TextColor::displayTextEffects(int s)
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
	int x = qRound(textEffects->OutlineVal->LWidth->value() * 10.0);
	if ((m_doc) && (m_item))
		m_doc->itemSelection_SetOutlineWidth(x);
}

void PropertyWidget_TextColor::handleShadowOffs()
{
	if ((m_doc) && (m_item))
	{
		int x = qRound(textEffects->ShadowVal->Xoffset->value() * 10.0);
		int y = qRound(textEffects->ShadowVal->Yoffset->value() * 10.0);
		m_doc->itemSelection_SetShadowOffsets(x, y);
	}
}

void PropertyWidget_TextColor::handleStrikeThru()
{
	if ((m_doc) && (m_item))
	{
		int x = qRound(textEffects->StrikeVal->LPos->value() * 10.0);
		int y = qRound(textEffects->StrikeVal->LWidth->value() * 10.0);
		m_doc->itemSelection_SetStrikethru(x, y);
	}
}

void PropertyWidget_TextColor::handleTextDirection()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool setter = revertButton->isChecked();
	m_item->setImageFlippedH(setter);
	m_item->setReversed(setter);
	m_item->update();
	//emit DocChanged();
}

void PropertyWidget_TextColor::handleTextFill()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetFillColor(fillColor->currentColor());
}

void PropertyWidget_TextColor::handleTextStroke()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetStrokeColor(strokeColor->currentColor());
}

void PropertyWidget_TextColor::handleTextShade()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (strokeShade == sender())
	{
		int b = strokeShade->getValue();
		m_doc->itemSelection_SetStrokeShade(b);
	}
	else
	{
		int b = fillShade->getValue();
		m_doc->itemSelection_SetFillShade(b);
	}
}

void PropertyWidget_TextColor::handleTypeStyle(int s)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_ScMW->setItemEffects(s);
}

void PropertyWidget_TextColor::displayUnderline(double p, double w)
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
		m_doc->itemSelection_SetUnderline(x, y);
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

void PropertyWidget_TextColor::languageChange()
{
	retranslateUi(this);

	textEffects->languageChange();

	fillColor->setToolTip( "<qt>" + tr("Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.") + "</qt>" );
	strokeColor->setToolTip( "<qt>" + tr("Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.") + "</qt>" );
	fillShade->setToolTip( tr("Saturation of color of text fill"));
	strokeShade->setToolTip( tr("Saturation of color of text stroke"));

	revertButton->setToolTip( tr("Right to Left Writing"));
}