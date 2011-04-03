/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "propertiespalette_text.h"

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "commonstrings.h"
#include "colorlistbox.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
#include "propertiespalette_utils.h"
#include "propertywidget_advanced.h"
#include "propertywidget_distance.h"
#include "propertywidget_flop.h"
#include "propertywidget_pathtext.h"
#include "propertywidget_optmargins.h"
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

	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	setupUi(this);

	fontSize->setPrefix( "" );
	fontSizeLabel->setPixmap(loadIcon("Zeichen.xpm"));
	lineSpacingLabel->setPixmap(loadIcon("linespacing2.png"));

	paraStyleLabel->setBuddy(paraStyleCombo);
	paraStyleClear->setIcon(loadIcon("16/edit-clear.png"));
	charStyleLabel->setBuddy(charStyleCombo);
	charStyleClear->setIcon(loadIcon("16/edit-clear.png"));

	colorWidgets = new PropertyWidget_TextColor(textTree);
	colorWidgetsItem = textTree->addWidget( tr("Color & Effects"), colorWidgets);

	flopBox = new PropertyWidget_Flop(textTree);
	flopItem = textTree->addWidget( tr("First Line Offset"), flopBox);

	distanceWidgets = new PropertyWidget_Distance(textTree);
    distanceItem = textTree->addWidget( tr("Columns & Text Distances"), distanceWidgets);

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

	connect(distanceWidgets->columns       , SIGNAL(valueChanged(int))   , this, SLOT(handleColumns()));
	connect(distanceWidgets->columnGap     , SIGNAL(valueChanged(double)), this, SLOT(handleColumnGap()));
	connect(distanceWidgets->columnGapLabel, SIGNAL(activated(int))      , this, SLOT(handleGapSwitch()));
	connect(distanceWidgets->topDistance   , SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()));
	connect(distanceWidgets->leftDistance  , SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()));
	connect(distanceWidgets->rightDistance , SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()));
	connect(distanceWidgets->bottomDistance, SIGNAL(valueChanged(double)), this, SLOT(handleTextDistances()));
	connect(distanceWidgets->tabsButton    , SIGNAL( clicked() )         , this, SLOT( handleTabs() ) );

	connect(flopBox->flopGroup, SIGNAL(buttonClicked( int )), this, SLOT(handleFirstLinePolicy(int)));

	connect(optMargins->optMarginRadioNone  , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	connect(optMargins->optMarginRadioBoth  , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	connect(optMargins->optMarginRadioLeft  , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	connect(optMargins->optMarginRadioRight , SIGNAL(clicked()), this, SLOT(handleOpticalMargins()) );
	connect(optMargins->optMarginResetButton, SIGNAL(clicked()), this, SLOT(resetOpticalMargins()) );
	
	connect(pathTextWidgets->showCurveCheckBox, SIGNAL(clicked())     , this, SLOT(handlePathLine()));
	connect(pathTextWidgets->pathTextType     , SIGNAL(activated(int)), this, SLOT(handlePathType()));
	connect(pathTextWidgets->flippedPathText  , SIGNAL(clicked())     , this, SLOT(handlePathFlip()));
	connect(pathTextWidgets->startOffset      , SIGNAL(valueChanged(double)), this, SLOT(handlePathDist()));
	connect(pathTextWidgets->distFromCurve    , SIGNAL(valueChanged(double)), this, SLOT(handlePathOffs()));

	connect(lineSpacingModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(handleLineSpacingMode(int)));

	m_haveItem = false;
	setEnabled(false);
}

void PropertiesPalette_Text::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW=mw;

	advancedWidgets->setMainWindow(mw);
	colorWidgets->setMainWindow(mw);

	connect(this  , SIGNAL(DocChanged())           , m_ScMW, SLOT(slotDocCh()));
	connect(this  , SIGNAL(NewAlignment(int))      , m_ScMW, SLOT(setNewAlignment(int)));
	connect(this  , SIGNAL(NewFont(const QString&)), m_ScMW, SLOT(SetNewFont(const QString&)));
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
	m_item = NULL;

	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();
	int precision = unitGetPrecisionFromIndex(m_unitIndex);
	double maxXYWHVal =  16777215 * m_unitRatio;
	double minXYVal   = -16777215 * m_unitRatio;

	m_haveDoc  = true;
	m_haveItem = false;

	fontSize->setValues( 0.5, 2048, 2, 1);
	lineSpacing->setValues( 1, 2048, 2, 1);

	advancedWidgets->setDoc(m_doc);
	colorWidgets->setDoc(m_doc);
	distanceWidgets->setDoc(m_doc);
	flopBox->setDoc(m_doc);
	optMargins->setDoc(m_doc);
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
		charStyleCombo->updateFormatList();
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

	QString tm;
	PageItem_TextFrame *i2=m_item->asTextFrame();
	if (i2!=0)
	{
		disconnect(distanceWidgets->columnGap, SIGNAL(valueChanged(double)), this, SLOT(handleColumnGap()));
		disconnect(distanceWidgets->columns  , SIGNAL(valueChanged(int)), this, SLOT(handleColumns()));
		distanceWidgets->columns->setMaximum(qMax(qRound(i2->width() / qMax(i2->ColGap, 10.0)), 1));
		distanceWidgets->columns->setMinimum(1);
		distanceWidgets->columns->setValue(i2->Cols);
		distanceWidgets->columnGap->setMinimum(0);
		if (distanceWidgets->columnGapLabel->currentIndex() == 0)
		{
			distanceWidgets->columnGap->setMaximum(qMax((i2->width() / i2->Cols - i2->textToFrameDistLeft() - i2->textToFrameDistRight())*m_unitRatio, 0.0));
			distanceWidgets->columnGap->setValue(i2->ColGap*m_unitRatio);
		}
		else
		{
			distanceWidgets->columnGap->setMaximum(qMax((i2->width() / i2->Cols)*m_unitRatio, 0.0));
			distanceWidgets->columnGap->setValue(i2->columnWidth()*m_unitRatio);
		}
		distanceWidgets->leftDistance->setValue(i2->textToFrameDistLeft()*m_unitRatio);
		distanceWidgets->topDistance->setValue(i2->textToFrameDistTop()*m_unitRatio);
		distanceWidgets->bottomDistance->setValue(i2->textToFrameDistBottom()*m_unitRatio);
		distanceWidgets->rightDistance->setValue(i2->textToFrameDistRight()*m_unitRatio);
		if (distanceWidgets->columns->value() == 1)
		{
			distanceWidgets->columnGap->setEnabled(false);
			distanceWidgets->columnGapLabel->setEnabled(false);
		}
		else
		{
			distanceWidgets->columnGap->setEnabled(true);
			distanceWidgets->columnGapLabel->setEnabled(true);
		}
		// I put it here because it’s visually grouped with these elements
		// but it’s a PageItem prop. and as such should be set without considering
		// the frame type.
		displayFirstLinePolicy(m_item->firstLineOffset());
		
		connect(distanceWidgets->columnGap, SIGNAL(valueChanged(double)), this, SLOT(handleColumnGap()));
		connect(distanceWidgets->columns, SIGNAL(valueChanged(int))   , this, SLOT(handleColumns()));
	}

	if (m_item->asTextFrame() || m_item->asPathText())
	{
		fonts->RebuildList(m_doc, m_item->isAnnotation());
	}

	displayTextDistances(i->textToFrameDistLeft(),i->textToFrameDistTop(),i->textToFrameDistBottom(),i->textToFrameDistRight());

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		setEnabled(false);
	}
	if (m_item->asPathText())
	{
		flopItem->setHidden(true);
		distanceItem->setHidden(true);
		pathTextItem->setHidden(false);
		pathTextWidgets->pathTextType->setCurrentIndex(m_item->textPathType);
		pathTextWidgets->flippedPathText->setChecked(m_item->textPathFlipped);
		pathTextWidgets->showCurveCheckBox->setChecked(m_item->PoShow);
		pathTextWidgets->distFromCurve->setValue(m_item->BaseOffs * -1);
		pathTextWidgets->startOffset->setValue(m_item->textToFrameDistLeft());
	}
	else if (m_item->asTextFrame())
	{
		flopItem->setHidden(false);
		distanceItem->setHidden(false);
		pathTextItem->setHidden(true);
	}
	else
	{
		flopItem->setHidden(false);
		distanceItem->setHidden(false);
		pathTextItem->setHidden(true);
	}

	m_haveItem = true;

	if (m_item->asTextFrame() || m_item->asPathText())
	{
		updateStyle(m_doc->appMode == modeEdit? m_item->currentStyle() : m_item->itemText.defaultStyle());
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

	m_haveItem = tmp;
}

void PropertiesPalette_Text::displayColumns(int r, double g)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	distanceWidgets->columns->setValue(r);
	distanceWidgets->columnGap->setValue(g*m_unitRatio);
	if (tmp)
	{
		PageItem_TextFrame *i2=m_item->asTextFrame();
		if (i2!=0)
		{
			distanceWidgets->columns->setMaximum(qMax(qRound(i2->width() / qMax(i2->ColGap, 10.0)), 1));
			if (distanceWidgets->columnGapLabel->currentIndex() == 0)
			{
				distanceWidgets->columnGap->setMaximum(qMax((i2->width() / i2->Cols - i2->textToFrameDistLeft() - i2->textToFrameDistRight())*m_unitRatio, 0.0));
				distanceWidgets->columnGap->setValue(i2->ColGap*m_unitRatio);
			}
			else
			{
				distanceWidgets->columnGap->setMaximum(qMax((i2->width() / i2->Cols)*m_unitRatio, 0.0));
				distanceWidgets->columnGap->setValue(i2->columnWidth()*m_unitRatio);
			}
		}
	}
	distanceWidgets->columns->setMinimum(1);
	distanceWidgets->columnGap->setMinimum(0);
	if (distanceWidgets->columns->value() == 1)
	{
		distanceWidgets->columnGap->setEnabled(false);
		distanceWidgets->columnGapLabel->setEnabled(false);
	}
	else
	{
		distanceWidgets->columnGap->setEnabled(true);
		distanceWidgets->columnGapLabel->setEnabled(true);
	}
	m_haveItem = tmp;
}

void PropertiesPalette_Text::handleLineSpacingMode(int id)
{
	if ((m_haveDoc) && (m_haveItem))
	{
		m_doc->itemSelection_SetLineSpacingMode(id);
		updateStyle(m_doc->appMode == modeEdit? m_item->currentStyle() : m_item->itemText.defaultStyle());
	}
}

void PropertiesPalette_Text::displayLineSpacing(double r)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	lineSpacing->setValue(r);
	const ParagraphStyle& curStyle(tmp && m_doc->appMode == modeEdit? m_item->currentStyle() : m_item->itemText.defaultStyle());
	if (tmp)
	{
		setupLineSpacingSpinbox(curStyle.lineSpacingMode(), r);
		lineSpacingModeCombo->setCurrentIndex(curStyle.lineSpacingMode());
	}
	m_haveItem = tmp;
}

void PropertiesPalette_Text::displayTextDistances(double left, double top, double bottom, double right)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	distanceWidgets->displayTextDistances(left, top, bottom, right);
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
	else
		flopBox->flopRealHeight->setChecked(true); //It’s historical behaviour.
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

	displayFontFace(charStyle.font().scName());
	displayFontSize(charStyle.fontSize());

	bool tmp = m_haveItem;
	m_haveItem = false;

	setupLineSpacingSpinbox(newCurrent.lineSpacingMode(), newCurrent.lineSpacing());
	lineSpacingModeCombo->setCurrentIndex(newCurrent.lineSpacingMode());
	textAlignment->setStyle(newCurrent.alignment());
	
	displayOpticalMargins(newCurrent);
	
	m_haveItem = tmp;
}

void PropertiesPalette_Text::updateCharStyles()
{
	charStyleCombo->updateFormatList();
}

void PropertiesPalette_Text::updateParagraphStyles()
{
	paraStyleCombo->updateFormatList();
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

void PropertiesPalette_Text::handleOpticalMargins()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	int omt(ParagraphStyle::OM_None);
	if (optMargins->optMarginRadioBoth->isChecked())
		omt = ParagraphStyle::OM_Default;
	else if (optMargins->optMarginRadioLeft->isChecked())
		omt = ParagraphStyle::OM_LeftHangingPunct;
	else if (optMargins->optMarginRadioRight->isChecked())
		omt = ParagraphStyle::OM_RightHangingPunct;

	m_doc->itemSelection_SetOpticalMargins(omt);
}

void PropertiesPalette_Text::resetOpticalMargins()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_resetOpticalMargins();
}

void PropertiesPalette_Text::displayOpticalMargins(const ParagraphStyle & pStyle)
{
	ParagraphStyle::OpticalMarginType omt(static_cast<ParagraphStyle::OpticalMarginType>(pStyle.opticalMargins()));
	bool blocked = optMargins->optMarginRadioBoth->blockSignals(true);
	if (omt == ParagraphStyle::OM_Default)
		optMargins->optMarginRadioBoth->setChecked(true);
	else if (omt == ParagraphStyle::OM_LeftHangingPunct)
		optMargins->optMarginRadioLeft->setChecked(true);
	else if (omt == ParagraphStyle::OM_RightHangingPunct)
		optMargins->optMarginRadioRight->setChecked(true);
	else
		optMargins->optMarginRadioNone->setChecked(true);
	optMargins->optMarginRadioBoth->blockSignals(blocked);
}

void PropertiesPalette_Text::handleLineSpacing()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetLineSpacing(lineSpacing->value());
}

void PropertiesPalette_Text::handleGapSwitch()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	displayColumns(m_item->Cols, m_item->ColGap);
	distanceWidgets->columnGap->setToolTip("");
	if (distanceWidgets->columnGapLabel->currentIndex() == 0)
		distanceWidgets->columnGap->setToolTip( tr( "Distance between columns" ) );
	else
		distanceWidgets->columnGap->setToolTip( tr( "Column width" ) );
}

void PropertiesPalette_Text::handleColumns()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->Cols = distanceWidgets->columns->value();
	displayColumns(m_item->Cols, m_item->ColGap);
	m_item->update();
	if (distanceWidgets->columns->value() == 1)
	{
		distanceWidgets->columnGap->setEnabled(false);
		distanceWidgets->columnGapLabel->setEnabled(false);
	}
	else
	{
		distanceWidgets->columnGap->setEnabled(true);
		distanceWidgets->columnGapLabel->setEnabled(true);
	}
	emit DocChanged();
}

void PropertiesPalette_Text::handleColumnGap()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (distanceWidgets->columnGapLabel->currentIndex() == 0)
		m_item->ColGap = distanceWidgets->columnGap->value() / m_unitRatio;
	else
	{
		double lineCorr;
		if ((m_item->lineColor() != CommonStrings::None) || (!m_item->strokePattern().isEmpty()))
			lineCorr = m_item->lineWidth();
		else
			lineCorr = 0;
		double newWidth = distanceWidgets->columnGap->value() / m_unitRatio;
		double newGap = qMax(((m_item->width() - m_item->textToFrameDistLeft() - m_item->textToFrameDistRight() - lineCorr) - (newWidth * m_item->Cols)) / (m_item->Cols - 1), 0.0);
		m_item->ColGap = newGap;
	}
	m_item->update();
	emit DocChanged();
}

void PropertiesPalette_Text::handleFontSize()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetFontSize(qRound(fontSize->value()*10.0));
}

void PropertiesPalette_Text::handleAlignement(int a)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetAlignment(a);
}

void PropertiesPalette_Text::handleTextDistances()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		double left   = distanceWidgets->leftDistance->value() / m_unitRatio;
		double right  = distanceWidgets->rightDistance->value() / m_unitRatio;
		double top    = distanceWidgets->topDistance->value() / m_unitRatio;
		double bottom = distanceWidgets->bottomDistance->value() / m_unitRatio;
		m_item->setTextToFrameDist(left, right, top, bottom);
		displayColumns(m_item->Cols, m_item->ColGap);
		m_item->update();
		emit DocChanged();
	}
}

void PropertiesPalette_Text::handleTextFont(QString c)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	emit NewFont(c);
}

void PropertiesPalette_Text::doClearCStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc)
	{
		m_doc->itemSelection_EraseCharStyle();
	}
}


void PropertiesPalette_Text::doClearPStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc)
	{
		m_doc->itemSelection_EraseParagraphStyle();
		CharStyle emptyCStyle;
		m_doc->itemSelection_SetCharStyle(emptyCStyle);
	}
}

void PropertiesPalette_Text::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	colorWidgets->updateColorList();
}

void PropertiesPalette_Text::handlePathType()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->textPathType = pathTextType->currentIndex();
	m_item->update();
	emit DocChanged();
}

void PropertiesPalette_Text::handlePathFlip()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->textPathFlipped = pathTextWidgets->flippedPathText->isChecked();
	m_item->updatePolyClip();
	m_item->update();
	emit DocChanged();
}

void PropertiesPalette_Text::handlePathLine()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->PoShow = pathTextWidgets->showCurveCheckBox->isChecked();
	m_item->update();
	emit DocChanged();
}

void PropertiesPalette_Text::handlePathDist()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->setTextToFrameDistLeft(pathTextWidgets->startOffset->value());
	m_doc->AdjustItemSize(m_item);
	m_item->updatePolyClip();
	m_item->update();
	emit DocChanged();
}

void PropertiesPalette_Text::handlePathOffs()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->BaseOffs = -pathTextWidgets->distFromCurve->value();
	m_doc->AdjustItemSize(m_item);
	m_item->updatePolyClip();
	m_item->update();
	emit DocChanged();
}

void PropertiesPalette_Text::fillLangCombo(QMap<QString,QString> langMap)
{
	QStringList sortList;
	QMap<QString,QString>::Iterator it;
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	langCombo->clear();
	for (it = langMap.begin(); it != langMap.end(); ++it)
		sortList.push_back(it.value());
	langCombo->addItems(sortQStringList(sortList));
	QListView *tmpView = dynamic_cast<QListView*>(langCombo->view()); Q_ASSERT(tmpView);
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
}

void PropertiesPalette_Text::handleHyphLanguage()
{
	if ((m_haveDoc) && (m_haveItem))
	{
		m_item->doc()->setHyphLanguage(m_ScMW->GetLang(langCombo->currentText()));
		emit DocChanged();
	}
}

void PropertiesPalette_Text::handleTabs()
{
	if ((m_haveDoc) && (m_haveItem))
	{
		PageItem_TextFrame *i2=m_item->asTextFrame();
		if (i2==0)
			return;
		const ParagraphStyle& style(m_doc->appMode == modeEdit ? i2->currentStyle() : i2->itemText.defaultStyle());
		TabManager *dia = new TabManager(this, m_doc->unitIndex(), style.tabValues(), i2->columnWidth());
		if (dia->exec())
		{
			if (m_doc->appMode != modeEdit)
			{
				ParagraphStyle newStyle(m_item->itemText.defaultStyle());
				newStyle.setTabValues(dia->tmpTab);
				m_item->itemText.setDefaultStyle(newStyle);
			}
			else
			{
				ParagraphStyle newStyle;
				newStyle.setTabValues(dia->tmpTab);
				m_doc->itemSelection_ApplyParagraphStyle(newStyle);
			}
			m_item->update();
			emit DocChanged();
		}
		delete dia;
	}
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
	
	colorWidgetsItem->setText(0, tr("Color & Effects"));
	advancedWidgetsItem->setText(0, tr("Advanced Settings"));
	flopItem->setText(0, tr("First Line Offset"));
    distanceItem->setText(0, tr("Columns & Text Distances"));
	optMarginsItem->setText(0, tr("Optical Margins"));
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
	if( radioFlop == 0)
		m_item->setFirstLineOffset(FLOPRealGlyphHeight);
	else if( radioFlop == 1)
		m_item->setFirstLineOffset(FLOPFontAscent);
	else if( radioFlop == 2)
		m_item->setFirstLineOffset(FLOPLineSpacing);
	m_item->update();
	emit DocChanged();
}
