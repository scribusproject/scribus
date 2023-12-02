/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "propertiespalette_text.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <QSignalBlocker>

#include "appmodes.h"
#include "commonstrings.h"
#include "fontcombo.h"
#include "iconmanager.h"
#include "localemgr.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "propertywidget_advanced.h"
#include "propertywidget_distance.h"
#include "propertywidget_flop.h"
#include "propertywidget_fontfeatures.h"
#include "propertywidget_hyphenation.h"
#include "propertywidget_optmargins.h"
#include "propertywidget_orphans.h"
#include "propertywidget_pareffect.h"
#include "propertywidget_pathtext.h"
#include "propertywidget_text.h"
#include "propertywidget_textcolor.h"
#include "scribus.h"
#include "scribusapp.h"
#include "selection.h"
#include "stylecombos.h"
#include "undomanager.h"
#include "langmgr.h"
#include "widgets/section_container.h"

//using namespace std;

PropertiesPalette_Text::PropertiesPalette_Text( QWidget* parent) : QWidget(parent)
{

	advancedWidgets = new PropertyWidget_Advanced(this);
	scAdvanced = new SectionContainer(tr("Advanced Settings"));
	scAdvanced->setWidget(advancedWidgets);
	scAdvanced->setIsCollapsed(true);

	distanceWidgets = new PropertyWidget_Distance(this);
	scDistance = new SectionContainer(tr("Columns && Text Distances"));
	scDistance->setWidget(distanceWidgets);
	scDistance->setIsCollapsed(true);

	flopBox = new PropertyWidget_Flop(this);
	scFlop = new SectionContainer(tr("First Line Offset"));
	scFlop->setWidget(flopBox);
	scFlop->setIsCollapsed(true);

	fontfeaturesWidget = new PropertyWidget_FontFeatures(this);
	scFontFeatures = new SectionContainer(tr("Font Features"));
	scFontFeatures->setWidget(fontfeaturesWidget);
	scFontFeatures->setIsCollapsed(true);

	hyphenationWidget = new PropertyWidget_Hyphenation(this);
	scHyphenation = new SectionContainer(tr("Hyphenation"));
	scHyphenation->setWidget(hyphenationWidget);
	scHyphenation->setIsCollapsed(true);

	optMargins = new PropertyWidget_OptMargins(this);
	scOptMargins = new SectionContainer(tr("Optical Margins"));
	scOptMargins->setWidget(optMargins);
	scOptMargins->setIsCollapsed(true);

	orphanBox = new PropertyWidget_Orphans(this);
	scOrphans = new SectionContainer(tr("Orphans and Widows"));
	scOrphans->setWidget(orphanBox);
	scOrphans->setIsCollapsed(true);

	parEffectWidgets = new PropertyWidget_ParEffect(this);
	scParEffect = new SectionContainer(tr("Paragraph Effects"));
	scParEffect->setWidget(parEffectWidgets);
	scParEffect->setIsCollapsed(true);

	pathTextWidgets = new PropertyWidget_PathText(this);
	scPathText = new SectionContainer(tr("Path Text Properties"));
	scPathText->setWidget(pathTextWidgets);
	scPathText->setIsCollapsed(true);

	textWidgets = new PropertyWidget_Text(this);
	scText = new SectionContainer(tr("Text"));
	scText->setWidget(textWidgets);
	scText->setIsCollapsed(false);

	colorWidgets = new PropertyWidget_TextColor(this);
	scTextColor = new SectionContainer(tr("Color && Effects"));
	scTextColor->setWidget(colorWidgets);
	scTextColor->setIsCollapsed(true);

	// Layout stack
	QVBoxLayout * lyt = new QVBoxLayout();
	lyt->setContentsMargins(0, 0, 0, 0);
	lyt->setSpacing(0);
	lyt->addWidget(scText);
	lyt->addWidget(scTextColor);
	lyt->addWidget(scFlop);
	lyt->addWidget(scOrphans);
	lyt->addWidget(scParEffect);
	lyt->addWidget(scDistance);
	lyt->addWidget(scOptMargins);
	lyt->addWidget(scHyphenation);
	lyt->addWidget(scAdvanced);
	lyt->addWidget(scFontFeatures);
	lyt->addWidget(scPathText);
	lyt->addStretch(1);
	setLayout(lyt);

	iconSetChange();
	languageChange();

	connect(flopBox->flopGroup, SIGNAL(idClicked(int)), this, SLOT(handleFirstLinePolicy(int)));

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));

	setEnabled(false);
}

void PropertiesPalette_Text::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	textWidgets->setMainWindow(mw);
	advancedWidgets->setMainWindow(mw);
	fontfeaturesWidget->setMainWindow(mw);
	colorWidgets->setMainWindow(mw);
	distanceWidgets->setMainWindow(mw);
	hyphenationWidget->setMainWindow(mw);
	parEffectWidgets->setMainWindow(mw);
	optMargins->setMainWindow(mw);
	pathTextWidgets->setMainWindow(mw);

	connect(m_ScMW, SIGNAL(UpdateRequest(int))     , this  , SLOT(handleUpdateRequest(int)));

}

void PropertiesPalette_Text::setDoc(ScribusDoc *d)
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

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	m_haveDoc  = true;
	m_haveItem = false;

	textWidgets->setDoc(m_doc);
	advancedWidgets->setDoc(m_doc);
	fontfeaturesWidget->setDoc(m_doc);
	colorWidgets->setDoc(m_doc);
	distanceWidgets->setDoc(m_doc);
	parEffectWidgets->setDoc(m_doc);
	flopBox->setDoc(m_doc);
	hyphenationWidget->setDoc(m_doc);
	optMargins->setDoc(m_doc);
	orphanBox->setDoc(m_doc);
	pathTextWidgets->setDoc(m_doc);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Text::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_haveDoc  = false;
	m_haveItem = false;
	m_doc      = nullptr;
	m_item     = nullptr;

	textWidgets->setDoc(nullptr);
	advancedWidgets->setDoc(nullptr);
	fontfeaturesWidget->setDoc(nullptr);
	colorWidgets->setDoc(nullptr);
	distanceWidgets->setDoc(nullptr);
	flopBox->setDoc(nullptr);
	hyphenationWidget->setDoc(nullptr);
	optMargins->setDoc(nullptr);
	orphanBox->setDoc(nullptr);
	parEffectWidgets->setDoc(nullptr);
	pathTextWidgets->setDoc(nullptr);

	setEnabled(false);
}

void PropertiesPalette_Text::unsetItem()
{
	m_haveItem = false;
	m_item     = nullptr;
	colorWidgets->setCurrentItem(m_item);
	handleSelectionChanged();
}

PageItem* PropertiesPalette_Text::currentItemFromSelection()
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

void PropertiesPalette_Text::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		setEnabled(false);
		flopBox->flopRealHeight->setChecked(true);
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

void PropertiesPalette_Text::handleUpdateRequest(int updateFlags)
{
	// ColorWidget will handle its update itself
	/*if (updateFlags & reqColorsUpdate)
		updateColorList();*/

	textWidgets->handleUpdateRequest(updateFlags);

	if (updateFlags & reqCharStylesUpdate)
	{
//		charStyleCombo->updateStyleList();
		parEffectWidgets->updateCharStyles();
	}
//	if (updateFlags & reqParaStylesUpdate)
//		paraStyleCombo->updateStyleList();
//	if (updateFlags & reqDefFontListUpdate)
//		fonts->rebuildList(nullptr);
//	if (updateFlags & reqDocFontListUpdate)
//		fonts->rebuildList(m_haveDoc ? m_doc : nullptr);
	if (updateFlags & reqStyleComboDocUpdate)
	{
//		paraStyleCombo->setDoc(m_haveDoc ? m_doc : nullptr);
//		charStyleCombo->setDoc(m_haveDoc ? m_doc : nullptr);
		parEffectWidgets->setDoc(m_haveDoc ? m_doc : nullptr);
	}
}

void PropertiesPalette_Text::setCurrentItem(PageItem *item)
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

	showFirstLinePolicy(m_item->firstLineOffset());

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		setEnabled(false);
	}

	m_haveItem = true;

	if (!sender())
	{
		colorWidgets->handleSelectionChanged();
		distanceWidgets->handleSelectionChanged();
		parEffectWidgets->handleSelectionChanged();
	}

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

void PropertiesPalette_Text::iconSetChange()
{
	textWidgets->iconSetChange();
}

void PropertiesPalette_Text::unitChange()
{
	if (!m_haveDoc)
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;

	advancedWidgets->unitChange();
	fontfeaturesWidget->unitChange();
	colorWidgets->unitChange();
	distanceWidgets->unitChange();
	flopBox->unitChange();
	optMargins->unitChange();
	pathTextWidgets->unitChange();
	parEffectWidgets->unitChange();
	textWidgets->unitChange();

	m_haveItem = tmp;
}

void PropertiesPalette_Text::localeChange()
{
	textWidgets->localeChange();
	advancedWidgets->localeChange();
	colorWidgets->localeChange();
	distanceWidgets->localeChange();
	parEffectWidgets->localeChange();
	pathTextWidgets->localeChange();
}

void PropertiesPalette_Text::showFontSize(double s)
{
	textWidgets->showFontSize(s);
}

void PropertiesPalette_Text::showLanguage(const QString& w)
{
	textWidgets->showLanguage(w);
}

void PropertiesPalette_Text::showFirstLinePolicy( FirstLineOffsetPolicy f )
{
	if(f == FLOPFontAscent)
		flopBox->flopFontAscent->setChecked(true);
	else if(f == FLOPLineSpacing)
		flopBox->flopLineSpacing->setChecked(true);
	else if (f == FLOPRealGlyphHeight)
		flopBox->flopRealHeight->setChecked(true); //It’s historical behaviour.
	else // if (f == FLOPBaseline)
		flopBox->flopBaselineGrid->setChecked(true);
}

void PropertiesPalette_Text::updateCharStyle(const CharStyle& charStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	advancedWidgets->updateCharStyle(charStyle);
	fontfeaturesWidget->updateCharStyle(charStyle);
	colorWidgets->updateCharStyle(charStyle);
	hyphenationWidget->updateCharStyle(charStyle);
	textWidgets->updateCharStyle(charStyle);
}

void PropertiesPalette_Text::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	advancedWidgets->updateStyle(newCurrent);
	fontfeaturesWidget->updateStyle(newCurrent);
	colorWidgets->updateStyle(newCurrent);
	optMargins->updateStyle(newCurrent);
	orphanBox->updateStyle (newCurrent);
	parEffectWidgets->updateStyle(newCurrent);
	hyphenationWidget->updateStyle(newCurrent);
	textWidgets->updateStyle(newCurrent);
}

void PropertiesPalette_Text::updateCharStyles()
{
	textWidgets->updateCharStyles();
	parEffectWidgets->updateCharStyles();
}

void PropertiesPalette_Text::updateParagraphStyles()
{
	textWidgets->updateParagraphStyles();
	parEffectWidgets->updateCharStyles();
}

void PropertiesPalette_Text::updateTextStyles()
{
	textWidgets->updateTextStyles();
}

void PropertiesPalette_Text::showAlignment(int e)
{
	textWidgets->showAlignment(e);
}

void PropertiesPalette_Text::showDirection(int e)
{
	textWidgets->showDirection(e);
}

void PropertiesPalette_Text::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	colorWidgets->updateColorList();
}

void PropertiesPalette_Text::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertiesPalette_Text::languageChange()
{
	scAdvanced->setText(tr("Advanced Settings"));
	scDistance->setText(tr("Columns && Text Distances"));
	scFlop->setText(tr("First Line Offset"));
	scFontFeatures->setText(tr("Font Features"));
	scHyphenation->setText(tr("Hyphenation"));
	scOptMargins->setText(tr("Optical Margins"));
	scOrphans->setText(tr("Orphans and Widows"));
	scParEffect->setText(tr("Paragraph Effects"));
	scPathText->setText(tr("Path Text Properties"));
	scText->setText(tr("Text"));
	scTextColor->setText(tr("Color && Effects"));

	colorWidgets->languageChange();
	flopBox->languageChange();
	orphanBox->languageChange();
	distanceWidgets->languageChange();
	optMargins->languageChange();
	advancedWidgets->languageChange();
	pathTextWidgets->languageChange();
	fontfeaturesWidget->languageChange();
	hyphenationWidget->languageChange();
	textWidgets->languageChange();
}

void PropertiesPalette_Text::handleFirstLinePolicy(int radioFlop)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveDoc || !m_haveItem)
		return;
	if( radioFlop == PropertyWidget_Flop::RealHeightID)
		m_item->setFirstLineOffset(FLOPRealGlyphHeight);
	else if( radioFlop == PropertyWidget_Flop::FontAscentID)
		m_item->setFirstLineOffset(FLOPFontAscent);
	else if( radioFlop == PropertyWidget_Flop::LineSpacingID)
		m_item->setFirstLineOffset(FLOPLineSpacing);
	else if( radioFlop == PropertyWidget_Flop::BaselineGridID)
		m_item->setFirstLineOffset(FLOPBaselineGrid);
	m_item->update();
	if (m_doc->appMode == modeEditTable)
		m_item->parentTable()->update();
	else
		m_item->update();
	m_doc->regionsChanged()->update(QRect());
}
