/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertiespalette_shape.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "appmodes.h"
#include "autoform.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "pageitem_arc.h"
#include "pageitem_regularpolygon.h"
#include "pageitem_textframe.h"
#include "polyprops.h"
#include "sccolorengine.h"
#include "sccombobox.h"
#include "scraction.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/nodeeditpalette.h"
#include "ui/propertiespalette_utils.h"
#include "ui/propertiespalette_xyz.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"


PropertiesPalette_Shape::PropertiesPalette_Shape( QWidget* parent) : QWidget(parent)
{
	m_ScMW = 0;
	m_doc  = 0;
	m_haveDoc    = false;
	m_haveItem   = false;
	m_unitRatio  = 1.0;

	m_tmpSelection = new Selection(this, false);
	m_tmpSelection->clear();

	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	textFlowDisabled->setIcon(IconManager::instance()->loadIcon("flow-none.png"));
	textFlowUsesFrameShape->setIcon(IconManager::instance()->loadIcon("flow-frame.png"));
	textFlowUsesBoundingBox->setIcon(IconManager::instance()->loadIcon("flow-bounding.png"));
	textFlowUsesContourLine->setIcon(IconManager::instance()->loadIcon("flow-contour.png"));
	textFlowUsesImageClipping->setIcon(IconManager::instance()->loadIcon("flow-contour.png"));

	languageChange();

	connect(textFlowBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(handleTextFlow()));
	connect(editShape  , SIGNAL(clicked())                 , this, SLOT(handleShapeEdit()));
	connect(roundRect  , SIGNAL(valueChanged(double))      , this, SLOT(handleCornerRadius()));
	connect(evenOdd    , SIGNAL(clicked())                 , this, SLOT(handleFillRule()) );
	connect(nonZero    , SIGNAL(clicked())                 , this, SLOT(handleFillRule()) );
	connect(customShape, SIGNAL(FormSel(int, int, qreal *)), this, SLOT(handleNewShape(int, int, qreal *)));

	roundRect->showValue(0);
	stackedWidget->setCurrentIndex(0);
}

void PropertiesPalette_Shape::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}


void PropertiesPalette_Shape::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this  , SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Shape::setDoc(ScribusDoc *d)
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

	roundRect->setValues( -300, 300, 2, 0);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Shape::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_haveDoc  = false;
	m_haveItem = false;
	m_doc   = NULL;
	m_item  = NULL;

	editShape->setEnabled(false);
	roundRect->setEnabled(false);
	roundRect->showValue(0);
	setEnabled(false);
}

void PropertiesPalette_Shape::unsetItem()
{
	m_haveItem = false;
	m_item     = NULL;
	handleSelectionChanged();
}

PageItem* PropertiesPalette_Shape::currentItemFromSelection()
{
	PageItem *currentItem = NULL;

	if (m_doc && m_doc->m_Selection->count()>0)
		currentItem = m_doc->m_Selection->itemAt(0);

	return currentItem;
}

void PropertiesPalette_Shape::setCustomShapeIcon(int submode)
{
	customShape->setIcon( customShape->getIconPixmap(submode) );
}

void PropertiesPalette_Shape::setLocked(bool isLocked)
{
	QPalette pal(qApp->palette());
	if (isLocked)
		pal.setCurrentColorGroup(QPalette::Disabled);
	if ((m_haveDoc) && (m_haveItem))
	{
		enableCustomShape();
		enableEditShape();
		if (((m_item->asTextFrame()) || (m_item->asImageFrame()) || (m_item->asPolygon())) &&  (!m_item->ClipEdited) && ((m_item->FrameType == 0) || (m_item->FrameType == 2)))
			roundRect->setEnabled(!isLocked);
		else
			roundRect->setEnabled(false);
	}
}

void PropertiesPalette_Shape::setSizeLocked(bool )
{
	enableEditShape();
}

void PropertiesPalette_Shape::setRoundRectEnabled(bool enabled)
{
	roundRect->setEnabled(enabled);
}

void PropertiesPalette_Shape::enableCustomShape()
{
	bool enabled = false;
	if (m_item)
	{
		enabled  = true;
		enabled &= !m_item->isArc();
		enabled &= !m_item->isLine();
		enabled &= !m_item->isOSGFrame();
		enabled &= !m_item->isPathText();
		enabled &= !m_item->isPolyLine();
		enabled &= !m_item->isSpiral();
		enabled &= !m_item->isRegularPolygon();
		enabled &= !m_item->locked();
		enabled &= !m_item->sizeLocked();
		enabled &= !m_item->isTable();
	}
	customShape->setEnabled(enabled);
}

void PropertiesPalette_Shape::enableEditShape()
{
	bool enabled = false;
	if (m_item)
	{
		enabled  = true;
		enabled &= !m_item->locked();
		enabled &= !m_item->sizeLocked();
		enabled &= !m_item->isOSGFrame();
		enabled &= !m_item->isTable();
	}
	editShape->setEnabled(enabled);
}

void PropertiesPalette_Shape::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		roundRect->showValue(0);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;

		m_haveItem = (itemType != -1);
		switch (itemType)
		{
		case -1:
			setEnabled(false);
			editShape->setEnabled(false);
			customShape->setEnabled(false);
			roundRect->setEnabled(false);
			roundRect->showValue(0);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
			if (currItem->asOSGFrame())
			{
				setEnabled(false);
				roundRect->setEnabled(false);
				editShape->setEnabled(false);
				customShape->setEnabled(false);
			}
			else
			{
				setEnabled(true);
				if ((!currItem->ClipEdited) && ((currItem->FrameType == 0) || (currItem->FrameType == 2)))
					roundRect->setEnabled(!currItem->locked());
				else
					roundRect->setEnabled(false);
				if ((m_doc->m_Selection->itemAt(0)->FrameType == 0) || (m_doc->m_Selection->itemAt(0)->FrameType == 2))
					roundRect->setEnabled(!currItem->locked());
			}
			break;
		case PageItem::TextFrame:
			setEnabled(true);
			if ((!currItem->ClipEdited) && ((currItem->FrameType == 0) || (currItem->FrameType == 2)))
				roundRect->setEnabled(!currItem->locked());
			else
				roundRect->setEnabled(false);
			break;
		case PageItem::Line:
			setEnabled(false);
			roundRect->setEnabled(false);
			break;
		case PageItem::Arc:
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::RegularPolygon:
			setEnabled(true);
			if ((!currItem->ClipEdited) && ((currItem->FrameType == 0) || (currItem->FrameType == 2)))
				roundRect->setEnabled(!currItem->locked());
			else
				roundRect->setEnabled(false);
			break;
		case PageItem::PolyLine:
		case PageItem::Spiral:
			setEnabled(true);
			roundRect->setEnabled(false);
			break;
		case PageItem::PathText:
			setEnabled(true);
			roundRect->setEnabled(false);
			break;
		case PageItem::Symbol:
			setEnabled(false);
			if ((!currItem->ClipEdited) && ((currItem->FrameType == 0) || (currItem->FrameType == 2)))
				roundRect->setEnabled(!currItem->locked());
			else
				roundRect->setEnabled(false);
			break;
		case PageItem::Table:
			setEnabled(true);
			roundRect->setEnabled(false);
			editShape->setEnabled(false);
			customShape->setEnabled(false);
			break;
		}
	}
	setCurrentItem(currItem);
	updateGeometry();
	//repaint();
}

void PropertiesPalette_Shape::handleUpdateRequest(int updateFlags)
{
	if ((updateFlags & reqCustomShapeUpdate) && (m_haveDoc && m_doc))
	{
		setCustomShapeIcon(m_doc->SubMode);
	}
}

void PropertiesPalette_Shape::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be NULL.
	//if (m_item == i)
	//	return;

	if (item && !m_doc)
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	if (!m_item) return;

	enableEditShape();
	enableCustomShape();

	if (m_item->FrameType == 0)
		customShape->setIcon(customShape->getIconPixmap(0));
	if (m_item->FrameType == 1)
		customShape->setIcon(customShape->getIconPixmap(1));
	if (m_item->FrameType > 3)
		customShape->setIcon(customShape->getIconPixmap(m_item->FrameType-2));

	roundRect->setValue(m_item->cornerRadius()*m_unitRatio);
	showTextFlowMode(m_item->textFlowMode());

	if (m_item->asPathText())
	{
		stackedWidget->setCurrentIndex(0);
	}
	else if (m_item->asTextFrame())
	{
		stackedWidget->setCurrentIndex(0);
	}
	else
	{
		stackedWidget->setCurrentIndex(1);
		fillRuleGroup->setVisible(m_item->itemType() != PageItem::ImageFrame);
	}
	setLocked(m_item->locked());
	setSizeLocked(m_item->sizeLocked());
	nonZero->setChecked(!m_item->fillRule);
	evenOdd->setChecked(m_item->fillRule);

	// Frame type 3 is obsolete: CR 2005-02-06
	//if (((i->itemType() == PageItem::TextFrame) || (i->itemType() == PageItem::ImageFrame) || (i->itemType() == 3)) &&  (!i->ClipEdited))
	if (((m_item->asTextFrame()) || (m_item->asImageFrame())) &&  (!m_item->ClipEdited) && ((m_item->FrameType == 0) || (m_item->FrameType == 2)))
		roundRect->setEnabled(true);
	else
	{
		roundRect->setEnabled ((m_item->asPolygon()) &&  (!m_item->ClipEdited)  && ((m_item->FrameType == 0) || (m_item->FrameType == 2)));
	}
	if (m_item->asOSGFrame())
	{
		setEnabled(false);
		roundRect->setEnabled(false);
		editShape->setEnabled(false);
		customShape->setEnabled(false);
	}
	if (m_item->asSymbolFrame())
	{
		setEnabled(false);
	}
	if (m_item->asTable())
	{
		setEnabled(true);
		roundRect->setEnabled(false);
		editShape->setEnabled(false);
		customShape->setEnabled(false);
	}
	m_haveItem = true;
	showTextFlowMode(m_item->textFlowMode());
}

void PropertiesPalette_Shape::handleTextFlow()
{
	PageItem::TextFlowMode mode = PageItem::TextFlowDisabled;
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if (!m_item->isGroup())
		{
			if (textFlowDisabled->isChecked())
				mode = PageItem::TextFlowDisabled;
			if (textFlowUsesFrameShape->isChecked())
				mode = PageItem::TextFlowUsesFrameShape;
			if (textFlowUsesBoundingBox->isChecked())
				mode = PageItem::TextFlowUsesBoundingBox;
			if (textFlowUsesContourLine->isChecked())
				mode = PageItem::TextFlowUsesContourLine;
			if (textFlowUsesImageClipping->isChecked())
				mode = PageItem::TextFlowUsesImageClipping;
		}
		m_item->setTextFlowMode(mode);
		m_doc->changed();
		m_doc->invalidateAll();
		m_doc->regionsChanged()->update(QRect());
	}
}

void PropertiesPalette_Shape::handleShapeEdit()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		m_tmpSelection->clear();
		if (m_item->asRegularPolygon())
		{
			m_ScMW->view->requestMode(modeEditPolygon);
			roundRect->setEnabled(false);
		}
		else if (m_item->asArc())
		{
			m_ScMW->view->requestMode(modeEditArc);
			roundRect->setEnabled(false);
		}
		else if (m_item->asSpiral())
		{
			m_ScMW->view->requestMode(modeEditSpiral);
			roundRect->setEnabled(false);
		}
		else
		{
			m_ScMW->view->requestMode(modeEditClip);
			roundRect->setEnabled(false);
			connect(m_ScMW->nodePalette, SIGNAL(paletteClosed()), this, SLOT(handleShapeEditEnded()));
		}
		m_doc->changed();
	}
}

void PropertiesPalette_Shape::handleShapeEditEnded()
{
	disconnect(m_ScMW->nodePalette, SIGNAL(paletteClosed()), this, SLOT(handleShapeEditEnded()));
	if ((m_haveDoc) && (m_haveItem))
	{
		if (m_tmpSelection->count() > 0)
		{
			m_doc->m_Selection->copy(*m_tmpSelection, false);
			m_doc->m_Selection->connectItemToGUI();
		}
		m_tmpSelection->clear();
	}
}

void PropertiesPalette_Shape::handleCornerRadius()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->setCornerRadius(roundRect->value() / m_unitRatio);
	m_doc->setFrameRounded();
	m_doc->changed();
	//called from setFrameRounded already!
	m_doc->regionsChanged()->update(QRect());
	m_item->update();
}

void PropertiesPalette_Shape::handleFillRule()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->fillRule = evenOdd->isChecked();
	m_item->update();
}

void PropertiesPalette_Shape::handleNewShape(int f, int c, qreal *vals)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if ((m_item->itemType() == PageItem::PolyLine) || (m_item->itemType() == PageItem::PathText))
			return;
		bool b = roundRect->blockSignals(true);
		m_doc->item_setFrameShape(m_item, f, c, vals);
		roundRect->setValue(m_item->cornerRadius()*m_unitRatio);
		roundRect->setEnabled(f == 0);
		roundRect->blockSignals(b);
		if (f == 0)
			m_doc->setFrameRounded();
		if ((m_item->itemType() == PageItem::ImageFrame) || (m_item->itemType() == PageItem::TextFrame))
			return;
		m_doc->invalidateAll();
		m_doc->regionsChanged()->update(QRect());
	}
}

void PropertiesPalette_Shape::showTextFlowMode(PageItem::TextFlowMode mode)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveItem)
		return;
	if (!m_item->isGroup())
	{
		if (mode == PageItem::TextFlowDisabled)
			textFlowDisabled->setChecked(true);
		else if (mode == PageItem::TextFlowUsesFrameShape)
			textFlowUsesFrameShape->setChecked(true);
		else if (mode == PageItem::TextFlowUsesBoundingBox)
			textFlowUsesBoundingBox->setChecked(true);
		else if (mode == PageItem::TextFlowUsesContourLine)
			textFlowUsesContourLine->setChecked(true);
		else if (mode == PageItem::TextFlowUsesImageClipping)
			textFlowUsesImageClipping->setChecked(true);
		if ((m_item->asImageFrame()) && (m_item->imageClip.size() != 0))
			textFlowUsesImageClipping->setEnabled(true);
		else
			textFlowUsesImageClipping->setEnabled(false);
	}
}

void PropertiesPalette_Shape::languageChange()
{
	QString ptSuffix = unitGetSuffixFromIndex(SC_PT);
	QString suffix   = m_doc ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;
	roundRect->setSuffix(suffix);

	retranslateUi(this);
}

void PropertiesPalette_Shape::unitChange()
{
	if (!m_doc)
		return;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	roundRect->blockSignals(true);
	roundRect->setNewUnit( m_unitIndex );
	roundRect->blockSignals(false);
}
