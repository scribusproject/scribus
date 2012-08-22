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
#include "commonstrings.h"
#include "colorlistbox.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "propertiespalette_utils.h"
#include "propertywidget_advanced.h"
#include "propertywidget_distance.h"
#include "propertywidget_dropcap.h"
#include "propertywidget_flop.h"
#include "propertywidget_optmargins.h"
#include "propertywidget_orphans.h"
#include "propertywidget_pathtext.h"
#include "propertywidget_textcolor.h"
#include "sccombobox.h"
#include "scfonts.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scraction.h"
#include "scribusview.h"
#include "selection.h"
#include "spalette.h"
#include "styleselect.h"
#include "tabmanager.h"
#include "units.h"
#include "undomanager.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"
#include "text/nlsconfig.h"
#include "fontcombo.h"
#include "colorcombo.h"

//using namespace std;

PropertiesPalette_Text::PropertiesPalette_Text( QWidget* parent) : QWidget(parent)
{
	m_ScMW=0;
	m_doc=0;
	m_haveDoc = false;
	m_haveItem = false;
	m_unitRatio = 1.0;


	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	fontSize->setPrefix( "" );
	fontSizeLabel->setPixmap(loadIcon("Zeichen.xpm"));
	lineSpacingLabel->setPixmap(loadIcon("linespacing2.png"));

	paraStyleLabel->setBuddy(paraStyleCombo);
	paraStyleClear->setIcon(loadIcon("16/edit-clear.png"));
	charStyleLabel->setBuddy(charStyleCombo);
	charStyleClear->setIcon(loadIcon("16/edit-clear.png"));

	colorWidgets = new PropertyWidget_TextColor(textTree);
	colorWidgetsItem = textTree->addWidget( tr("Color && Effects"), colorWidgets);

	flopBox = new PropertyWidget_Flop(textTree);
	flopItem = textTree->addWidget( tr("First Line Offset"), flopBox);

	orphanBox = new PropertyWidget_Orphans(textTree);
	orphanItem = textTree->addWidget( tr("Orphans and Widows"), orphanBox);

	dropcapsBox = new PropertyWidget_DropCap(textTree);
	dropcapsItem = textTree->addWidget( tr("Drop Cap"), dropcapsBox);

	distanceWidgets = new PropertyWidget_Distance(textTree);
	distanceItem = textTree->addWidget( tr("Columns && Text Distances"), distanceWidgets);

	//<< Optical Margins
	optMargins = new PropertyWidget_OptMargins(textTree);
	optMarginsItem = textTree->addWidget( tr("Optical Margins"), optMargins);
	//>> Optical Margins

	//<<Advanced Settings
	advancedWidgets = new PropertyWidget_Advanced(textTree);
	advancedWidgetsItem = textTree->addWidget( tr("Advanced Settings"), advancedWidgets);
	//>>Advanced Settings

	pathTextWidgets = new PropertyWidget_PathText(textTree);
	pathTextItem = textTree->addWidget( tr("Path Text Properties"), pathTextWidgets);
	
	languageChange();

	connect(lineSpacing   , SIGNAL(valueChanged(double)), this, SLOT(handleLineSpacing()));
	connect(fonts         , SIGNAL(fontSelected(QString )), this, SLOT(handleTextFont(QString)));
	connect(fontSize      , SIGNAL(valueChanged(double)), this, SLOT(handleFontSize()));
	connect(textAlignment , SIGNAL(State(int))   , this, SLOT(handleAlignement(int)));
	connect(charStyleClear, SIGNAL(clicked()), this, SLOT(doClearCStyle()));
	connect(paraStyleClear, SIGNAL(clicked()), this, SLOT(doClearPStyle()));

	connect(flopBox->flopGroup, SIGNAL(buttonClicked( int )), this, SLOT(handleFirstLinePolicy(int)));

	connect(lineSpacingModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(handleLineSpacingMode(int)));

	m_haveItem = false;
	setEnabled(false);
}

void PropertiesPalette_Text::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	advancedWidgets->setMainWindow(mw);
	colorWidgets->setMainWindow(mw);
	distanceWidgets->setMainWindow(mw);
	dropcapsBox->setMainWindow(mw);
	optMargins->setMainWindow(mw);
	pathTextWidgets->setMainWindow(mw);

	connect(m_ScMW, SIGNAL(UpdateRequest(int))     , this  , SLOT(handleUpdateRequest(int)));

	connect(paraStyleCombo, SIGNAL(newStyle(const QString&)), m_ScMW, SLOT(setNewParStyle(const QString&)), Qt::UniqueConnection);
	connect(charStyleCombo, SIGNAL(newStyle(const QString&)), m_ScMW, SLOT(setNewCharStyle(const QString&)), Qt::UniqueConnection);
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
	m_item = NULL;

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	m_haveDoc  = true;
	m_haveItem = false;

	fontSize->setValues( 0.5, 2048, 2, 1);
	lineSpacing->setValues( 1, 2048, 2, 1);

	advancedWidgets->setDoc(m_doc);
	colorWidgets->setDoc(m_doc);
	distanceWidgets->setDoc(m_doc);
	dropcapsBox->setDoc(m_doc);
	flopBox->setDoc(m_doc);
	optMargins->setDoc(m_doc);
	orphanBox->setDoc(m_doc);
	pathTextWidgets->setDoc(m_doc);

	fonts->RebuildList(m_doc);
	paraStyleCombo->setDoc(m_doc);
	charStyleCombo->setDoc(m_doc);

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
	m_doc      = NULL;
	m_item     = NULL;

	paraStyleCombo->setDoc(0);
	charStyleCombo->setDoc(0);

	advancedWidgets->setDoc(0);
	colorWidgets->setDoc(0);
	distanceWidgets->setDoc(0);
	flopBox->setDoc(0);
	optMargins->setDoc(0);
	orphanBox->setDoc(0);
	dropcapsBox->setDoc(0);
	pathTextWidgets->setDoc(0);

	m_haveItem = false;

	setEnabled(false);
}

void PropertiesPalette_Text::unsetItem()
{
	m_haveItem = false;
	m_item     = NULL;
	handleSelectionChanged();
}

PageItem* PropertiesPalette_Text::currentItemFromSelection()
{
	PageItem *currentItem = NULL;

	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 1)
		{
			currentItem = m_doc->m_Selection->itemAt(0);
		}
		else if (m_doc->m_Selection->count() == 1)
		{
			currentItem = m_doc->m_Selection->itemAt(0);
		}
	}

	return currentItem;
}

void PropertiesPalette_Text::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1 )
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
	repaint();
}

void PropertiesPalette_Text::handleUpdateRequest(int updateFlags)
{
	// ColorWidget will handle its update itself
	/*if (updateFlags & reqColorsUpdate)
		updateColorList();*/
	if (updateFlags & reqCharStylesUpdate)
	{
		charStyleCombo->updateFormatList();
		dropcapsBox->updateCharStyles();
	}
	if (updateFlags & reqParaStylesUpdate)
		paraStyleCombo->updateFormatList();
	if (updateFlags & reqDefFontListUpdate)
		fonts->RebuildList(0);
	if (updateFlags & reqDocFontListUpdate)
		fonts->RebuildList(m_haveDoc ? m_doc : 0);
	if (updateFlags & reqStyleComboDocUpdate)
	{
		paraStyleCombo->setDoc(m_haveDoc ? m_doc : 0);
		charStyleCombo->setDoc(m_haveDoc ? m_doc : 0);
		dropcapsBox->setDoc(m_haveDoc ? m_doc : 0);
	}
}

void PropertiesPalette_Text::setCurrentItem(PageItem *i)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldnt really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This wont work until when a canvas deselect happens, m_item must be NULL.
	//if (m_item == i)
	//	return;

	if (!m_doc)
		setDoc(i->doc());

	m_haveItem = false;
	m_item = i;

	displayFirstLinePolicy(m_item->firstLineOffset());

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		setEnabled(false);
	}
	if (m_item->asPathText())
	{
		flopItem->setHidden(true);
		distanceItem->setHidden(true);
		orphanItem->setHidden(true);
		dropcapsItem->setHidden(true);
		pathTextItem->setHidden(false);
	}
	else if (m_item->asTextFrame() || m_item->asTable())
	{
		flopItem->setHidden(false);
		distanceItem->setHidden(false);
		orphanItem->setHidden(false);
		dropcapsItem->setHidden(false);
		pathTextItem->setHidden(true);
	}
	else
	{
		flopItem->setHidden(false);
		distanceItem->setHidden(false);
		orphanItem->setHidden(false);
		dropcapsItem->setHidden(false);
		pathTextItem->setHidden(true);
	}

	m_haveItem = true;

	if (m_item->asTextFrame() || m_item->asPathText() || m_item->asTable())
	{
		ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
		if (m_doc->appMode == modeEdit)
			m_item->currentTextProps(parStyle);
		else if (m_doc->appMode == modeEditTable)
			m_item->asTable()->activeCell().textFrame()->currentTextProps(parStyle);
		updateStyle(parStyle);
	}
	if (m_item->asOSGFrame())
	{
		setEnabled(false);
	}
	if (m_item->asSymbolFrame())
	{
		setEnabled(false);
	}
}

void PropertiesPalette_Text::unitChange()
{
	if (!m_haveDoc)
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;

	advancedWidgets->unitChange();
	colorWidgets->unitChange();
	distanceWidgets->unitChange();
	flopBox->unitChange();
	optMargins->unitChange();
	pathTextWidgets->unitChange();
	dropcapsBox->unitChange();

	m_haveItem = tmp;
}

void PropertiesPalette_Text::handleLineSpacingMode(int id)
{
	if ((m_haveDoc) && (m_haveItem))
	{
		PageItem *i2 = m_item;
		if (m_doc->appMode == modeEditTable)
			i2 = m_item->asTable()->activeCell().textFrame();
		if (i2 != NULL)
		{
			Selection tempSelection(this, false);
			tempSelection.addItem(i2, true);
			m_doc->itemSelection_SetLineSpacingMode(id, &tempSelection);
			updateStyle(((m_doc->appMode == modeEdit) || (m_doc->appMode == modeEditTable)) ? i2->currentStyle() : i2->itemText.defaultStyle());
			m_doc->regionsChanged()->update(QRect());
		}
	}
}

void PropertiesPalette_Text::displayLineSpacing(double r)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	lineSpacing->setValue(r);
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != NULL)
	{
		const ParagraphStyle& curStyle(tmp && m_doc->appMode == modeEdit? i2->currentStyle() : i2->itemText.defaultStyle());
		if (tmp)
		{
			setupLineSpacingSpinbox(curStyle.lineSpacingMode(), r);
			lineSpacingModeCombo->setCurrentIndex(curStyle.lineSpacingMode());
		}
	}
	m_haveItem = tmp;
}

void PropertiesPalette_Text::displayFontFace(const QString& newFont)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	if (m_item != NULL)
		fonts->RebuildList(m_doc, m_item->isAnnotation());
	fonts->setCurrentFont(newFont);
	m_haveItem = tmp;
}

void PropertiesPalette_Text::displayFontSize(double s)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	fontSize->showValue(s / 10.0);
}

void PropertiesPalette_Text::displayFirstLinePolicy( FirstLineOffsetPolicy f )
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

void PropertiesPalette_Text::setupLineSpacingSpinbox(int mode, double value)
{
	bool blocked = lineSpacing->blockSignals(true);
	if (mode > 0)
	{
		if (mode==1)
			lineSpacing->setSpecialValueText( tr( "Auto" ) );
		if (mode==2)
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

void PropertiesPalette_Text::updateCharStyle(const CharStyle& charStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	advancedWidgets->updateCharStyle(charStyle);
	colorWidgets->updateCharStyle(charStyle);

	displayFontFace(charStyle.font().scName());
	displayFontSize(charStyle.fontSize());
}

void PropertiesPalette_Text::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	const CharStyle& charStyle = newCurrent.charStyle();

	advancedWidgets->updateStyle(newCurrent);
	colorWidgets->updateStyle(newCurrent);
	optMargins->updateStyle(newCurrent);
	orphanBox->updateStyle (newCurrent);
	dropcapsBox->updateStyle(newCurrent);

	displayFontFace(charStyle.font().scName());
	displayFontSize(charStyle.fontSize());

	bool tmp = m_haveItem;
	m_haveItem = false;

	setupLineSpacingSpinbox(newCurrent.lineSpacingMode(), newCurrent.lineSpacing());
	lineSpacingModeCombo->setCurrentIndex(newCurrent.lineSpacingMode());
	textAlignment->setStyle(newCurrent.alignment());
	
	m_haveItem = tmp;
}

void PropertiesPalette_Text::updateCharStyles()
{
	charStyleCombo->updateFormatList();
	dropcapsBox->updateCharStyles();
}

void PropertiesPalette_Text::updateParagraphStyles()
{
	paraStyleCombo->updateFormatList();
	charStyleCombo->updateFormatList();
	dropcapsBox->updateCharStyles();
}

void PropertiesPalette_Text::updateTextStyles()
{
	paraStyleCombo->updateFormatList();
	charStyleCombo->updateFormatList();
}

void PropertiesPalette_Text::displayAlignment(int e)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	textAlignment->setEnabled(true);
	textAlignment->setStyle(e);
	m_haveItem = tmp;
}

void PropertiesPalette_Text::displayCharStyle(const QString& name)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool blocked = charStyleCombo->blockSignals(true);
	charStyleCombo->setFormat(name);
	charStyleCombo->blockSignals(blocked);
}

void PropertiesPalette_Text::displayParStyle(const QString& name)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool blocked = paraStyleCombo->blockSignals(true);
	paraStyleCombo->setFormat(name);
	paraStyleCombo->blockSignals(blocked);
}

void PropertiesPalette_Text::handleLineSpacing()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_doc->itemSelection_SetLineSpacing(lineSpacing->value(), &tempSelection);
	}
}

void PropertiesPalette_Text::handleFontSize()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_doc->itemSelection_SetFontSize(qRound(fontSize->value()*10.0), &tempSelection);
	}
}

void PropertiesPalette_Text::handleAlignement(int a)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != NULL)
	{
		Selection tempSelection(this, false);
		tempSelection.addItem(i2, true);
		m_doc->itemSelection_SetAlignment(a, &tempSelection);
	}
}

void PropertiesPalette_Text::handleTextFont(QString c)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_ScMW->SetNewFont(c);
}

void PropertiesPalette_Text::doClearCStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc)
	{
		PageItem *i2 = m_item;
		if (m_doc->appMode == modeEditTable)
			i2 = m_item->asTable()->activeCell().textFrame();
		if (i2 != NULL)
		{
			Selection tempSelection(this, false);
			tempSelection.addItem(i2, true);
			m_doc->itemSelection_EraseCharStyle(&tempSelection);
		}
	}
}


void PropertiesPalette_Text::doClearPStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc)
	{
		PageItem *i2 = m_item;
		if (m_doc->appMode == modeEditTable)
			i2 = m_item->asTable()->activeCell().textFrame();
		if (i2 != NULL)
		{
			Selection tempSelection(this, false);
			tempSelection.addItem(i2, true);
			m_doc->itemSelection_EraseParagraphStyle(&tempSelection);
			CharStyle emptyCStyle;
			m_doc->itemSelection_SetCharStyle(emptyCStyle, &tempSelection);
		}
	}
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
	paraStyleLabel->setText( tr("Paragraph St&yle:"));
	charStyleLabel->setText( tr("Character St&yle:"));
	
	colorWidgetsItem->setText(0, tr("Color && Effects"));
	advancedWidgetsItem->setText(0, tr("Advanced Settings"));
	flopItem->setText(0, tr("First Line Offset"));
	distanceItem->setText(0, tr("Columns && Text Distances"));
	optMarginsItem->setText(0, tr("Optical Margins"));
	orphanItem->setText(0, tr("Orphans and Widows"));
	pathTextItem->setText(0, tr("Path Text Properties"));

	int oldLineSpacingMode = lineSpacingModeCombo->currentIndex();
	lineSpacingModeCombo->clear();
	lineSpacingModeCombo->addItem( tr("Fixed Linespacing"));
	lineSpacingModeCombo->addItem( tr("Automatic Linespacing"));
	lineSpacingModeCombo->addItem( tr("Align to Baseline Grid"));
	lineSpacingModeCombo->setCurrentIndex(oldLineSpacingMode);
	
	QString ptSuffix = tr(" pt");
	fontSize->setSuffix(ptSuffix);
	lineSpacing->setSuffix(ptSuffix);

	advancedWidgets->languageChange();
	colorWidgets->languageChange();
	distanceWidgets->languageChange();
	flopBox->languageChange();
	optMargins->languageChange();
	orphanBox->languageChange();
	pathTextWidgets->languageChange();

	textAlignment->languageChange();

	fontSize->setToolTip( tr("Font Size"));
	
	lineSpacing->setToolTip( tr("Line Spacing"));
	lineSpacingModeCombo->setToolTip( tr("Select the line spacing mode.") );
	paraStyleCombo->setToolTip( tr("Paragraph style of currently selected text or paragraph"));
	charStyleCombo->setToolTip( tr("Character style of currently selected text or paragraph"));
	paraStyleClear->setToolTip( tr("Remove Direct Paragraph Formatting"));
	charStyleClear->setToolTip( tr("Remove Direct Character Formatting"));
}

void PropertiesPalette_Text::handleFirstLinePolicy(int radioFlop)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveDoc || !m_haveItem)
		return;
	PageItem *i2 = m_item;
	if (m_doc->appMode == modeEditTable)
		i2 = m_item->asTable()->activeCell().textFrame();
	if (i2 != NULL)
	{
		if( radioFlop == PropertyWidget_Flop::RealHeightID)
			i2->setFirstLineOffset(FLOPRealGlyphHeight);
		else if( radioFlop == PropertyWidget_Flop::FontAscentID)
			i2->setFirstLineOffset(FLOPFontAscent);
		else if( radioFlop == PropertyWidget_Flop::LineSpacingID)
			i2->setFirstLineOffset(FLOPLineSpacing);
		else if( radioFlop == PropertyWidget_Flop::BaselineGridID)
			i2->setFirstLineOffset(FLOPBaselineGrid);
		i2->update();
		if (m_doc->appMode == modeEditTable)
			m_item->asTable()->update();
		else
			m_item->update();
		m_doc->regionsChanged()->update(QRect());
	}
}
