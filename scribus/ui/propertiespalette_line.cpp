/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertiespalette_line.h"

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "arrowchooser.h"
#include "commonstrings.h"
#include "dasheditor.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
#include "sccolorengine.h"
#include "sccombobox.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scraction.h"
#include "scribusview.h"
#include "selection.h"
#include "units.h"
#include "undomanager.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"
#include "text/nlsconfig.h"
#include "ui/propertiespalette_utils.h"

//using namespace std;

PropertiesPalette_Line::PropertiesPalette_Line( QWidget* parent) : QWidget(parent)
{
	m_ScMW = 0;
	m_doc  = 0;
	m_haveDoc  = false;
	m_haveItem = false;
	m_lineMode = false;
	m_unitRatio = 1.0;

	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	lineType->addItem( tr("Custom"));

	lineModeLabel->setBuddy(lineMode);
	lineTypeLabel->setBuddy(lineType);

	startArrowLabel->setBuddy(startArrow);
	endArrowLabel->setBuddy(endArrow);

	startArrowScale->setMaximum( 300 );
	startArrowScale->setMinimum( 1 );

	endArrowScale->setMaximum( 300 );
	endArrowScale->setMinimum( 1 );

	lineWidthLabel->setBuddy(lineWidth);
	lineJoinLabel->setBuddy(lineJoinStyle);
	lineEndLabel->setBuddy(lineEndStyle);

	lineStyles->setItemDelegate(new LineFormatItemDelegate);
	lineStyles->addItem( "No Style" );

	tableLineLayout->setAlignment( Qt::AlignTop );

	languageChange();

	connect(lineWidth     , SIGNAL(valueChanged(double)), this, SLOT(handleLineWidth()));
	connect(lineType    , SIGNAL(activated(int))      , this, SLOT(handleLineStyle()));
	connect(lineJoinStyle, SIGNAL(activated(int))      , this, SLOT(handleLineJoin()));
	connect(lineEndStyle , SIGNAL(activated(int))      , this, SLOT(handleLineEnd()));
	connect(lineMode  , SIGNAL(activated(int))      , this, SLOT(handleLineMode()));
	connect(dashEditor, SIGNAL(dashChanged())       , this, SLOT(handleDashChange()));
	connect(topLine   , SIGNAL(clicked())           , this, SLOT(handleTLines()));
	connect(leftLine  , SIGNAL(clicked())           , this, SLOT(handleTLines()));
	connect(rightLine , SIGNAL(clicked())           , this, SLOT(handleTLines()));
	connect(bottomLine, SIGNAL(clicked())           , this, SLOT(handleTLines()));
	connect(startArrow, SIGNAL(activated(int))      , this, SLOT(handleStartArrow(int )));
	connect(endArrow  , SIGNAL(activated(int))      , this, SLOT(handleEndArrow(int )));
	connect(startArrowScale, SIGNAL(valueChanged(int)), this, SLOT(handleStartArrowScale(int )));
	connect(endArrowScale  , SIGNAL(valueChanged(int)), this, SLOT(handleEndArrowScale(int )));
	connect(lineStyles, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(handleLineStyle(QListWidgetItem*)));

	stackedWidget->setCurrentIndex(0);
}

void PropertiesPalette_Line::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

PageItem* PropertiesPalette_Line::currentItemFromSelection()
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

void PropertiesPalette_Line::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;

	connect(this  , SIGNAL(DocChanged())      , m_ScMW, SLOT(slotDocCh()));
	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this  , SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Line::setDoc(ScribusDoc *d)
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

	m_haveDoc  = true;
	m_haveItem = false;

	lineWidth->setMaximum( 300 );
	lineWidth->setMinimum( 0 );

	updateLineStyles(m_doc);
	startArrow->rebuildList(&m_doc->arrowStyles());
	endArrow->rebuildList(&m_doc->arrowStyles());

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Line::unsetDoc()
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

	updateLineStyles(0);

	setEnabled(false);
}

void PropertiesPalette_Line::unsetItem()
{
	m_haveItem = false;
	m_item     = NULL;
	dashEditor->hide();
	handleSelectionChanged();
}

void PropertiesPalette_Line::handleSelectionChanged()
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

		lineMode->setEnabled(false);
		switch (itemType)
		{
		case -1:
			setEnabled(false);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
			setEnabled(currItem->asOSGFrame() == NULL);
		case PageItem::Line:
			setEnabled(true);
			lineMode->setEnabled(true);
			break;
		case PageItem::Arc:
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::PolyLine:
		case PageItem::PathText:
		case PageItem::RegularPolygon:
		case PageItem::TextFrame:
			setEnabled(true);
			break;
		case PageItem::Symbol:
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

void PropertiesPalette_Line::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqArrowStylesUpdate)
		updateArrowStyles();
	if (updateFlags & reqLineStylesUpdate)
		updateLineStyles();
}

void PropertiesPalette_Line::setCurrentItem(PageItem *item)
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
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	lineStyles->blockSignals(true);
	startArrow->blockSignals(true);
	endArrow->blockSignals(true);
	startArrowScale->blockSignals(true);
	endArrowScale->blockSignals(true);
	lineMode->blockSignals(true);

	if ((m_item->asLine()) || (m_item->asPolyLine()) || (m_item->asSpiral()))
	{
		startArrow->setEnabled(true);
		endArrow->setEnabled(true);
		startArrow->setCurrentIndex(m_item->startArrowIndex());
		endArrow->setCurrentIndex(m_item->endArrowIndex());
		startArrowScale->setEnabled(true);
		endArrowScale->setEnabled(true);
		endArrowScale->setValue(m_item->endArrowScale());
		startArrowScale->setValue(m_item->startArrowScale());
	}
	else
	{
		startArrow->setEnabled(false);
		endArrow->setEnabled(false);
		startArrowScale->setEnabled(false);
		endArrowScale->setEnabled(false);
	}

	if (lineStyles->currentItem())
		lineStyles->currentItem()->setSelected(false);

	bool setter = false;
	if (m_item->NamedLStyle.isEmpty())
	{
		setter = true;
		QListWidgetItem *itemStl = NULL;
		itemStl = lineStyles->item(0);
		if (itemStl != NULL)
			itemStl->setSelected(true);
	}
	else
	{
		QList<QListWidgetItem*> results (lineStyles->findItems(m_item->NamedLStyle, Qt::MatchFixedString|Qt::MatchCaseSensitive));
		if (results.count() > 0)
			results[0]->setSelected(true);
		setter = false;
	}

	lineType->setEnabled(setter);
	lineWidth->setEnabled(setter);
	lineJoinStyle->setEnabled(setter);
	lineEndStyle->setEnabled(setter);

	if (m_item->dashes().count() == 0)
		dashEditor->hide();
	else
	{
		lineType->setCurrentIndex(37);
		dashEditor->setDashValues(m_item->dashes(), qMax(m_item->lineWidth(), 0.001), m_item->dashOffset());
		dashEditor->show();
	}

	if (m_lineMode)
		lineMode->setCurrentIndex(1);
	else
		lineMode->setCurrentIndex(0);

	lineStyles->blockSignals(false);
	startArrow->blockSignals(false);
	endArrow->blockSignals(false);
	startArrowScale->blockSignals(false);
	endArrowScale->blockSignals(false);
	lineMode->blockSignals(false);

	if ((m_item->isTableItem) && (m_item->isSingleSel))
	{
		setter = true;
		stackedWidget->setCurrentIndex(1);
		topLine->setChecked(m_item->TopLine);
		leftLine->setChecked(m_item->LeftLine);
		rightLine->setChecked(m_item->RightLine);
		bottomLine->setChecked(m_item->BottomLine);
	}
	else
	{
		setter = false;
		stackedWidget->setCurrentIndex(0);
	}

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		setEnabled(false);
	}

	m_haveItem = true;

	displayLineWidth(m_item->lineWidth());
	displayLineValues(m_item->lineStyle(), m_item->lineEnd(), m_item->lineJoin());

	if (m_item->asOSGFrame())
	{
		setEnabled(false);
	}
	if (m_item->asSymbolFrame())
	{
		setEnabled(false);
	}
}

void PropertiesPalette_Line::updateArrowStyles()
{
	updateArrowStyles(m_doc);
}

void PropertiesPalette_Line::updateArrowStyles(ScribusDoc *doc)
{
	if (doc)
	{
		startArrow->rebuildList(&doc->arrowStyles());
		endArrow->rebuildList(&doc->arrowStyles());
	}
}

void PropertiesPalette_Line::updateLineStyles()
{
	updateLineStyles(m_doc);
}

void PropertiesPalette_Line::updateLineStyles(ScribusDoc *dd)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	
	lineStyles->blockSignals(true);
	lineStyles->clear();
	if (dd != 0)
	{
		QMap<QString,multiLine>::Iterator it;
		for (it = dd->MLineStyles.begin(); it != dd->MLineStyles.end(); ++it)
			lineStyles->addItem( new LineFormatItem(dd, it.value(), it.key()) );
		lineStyles->sortItems();
		lineStyles->insertItem( 0, tr("No Style"));
		if (lineStyles->currentItem())
			lineStyles->currentItem()->setSelected(false);
	}
	lineStyles->blockSignals(false);
}

void PropertiesPalette_Line::displayLineWidth(double s)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	lineWidth->showValue(s * m_unitRatio);
	if (m_haveItem)
	{
		if (m_item->dashes().count() != 0)
		{
			dashEditor->blockSignals(true);
			if (m_item->lineWidth() != 0.0)
			{
				dashEditor->setDashValues(m_item->dashes(), m_item->lineWidth(), m_item->dashOffset());
				dashEditor->setEnabled(true);
			}
			else
				dashEditor->setEnabled(false);
			dashEditor->blockSignals(false);
		}
	}
}

void PropertiesPalette_Line::displayLineValues(Qt::PenStyle p, Qt::PenCapStyle pc, Qt::PenJoinStyle pj)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	lineType->blockSignals(true);
	dashEditor->blockSignals(true);
	if (m_haveItem)
	{
		if (m_item->dashes().count() != 0)
		{
			lineType->setCurrentIndex(37);
			dashEditor->setDashValues(m_item->dashes(), qMax(m_item->lineWidth(), 0.001), m_item->dashOffset());
		}
		else
			lineType->setCurrentIndex(static_cast<int>(p) - 1);
	}
	else
		lineType->setCurrentIndex(static_cast<int>(p) - 1);
	dashEditor->blockSignals(false);
	lineType->blockSignals(false);

	lineEndStyle->blockSignals(true);
	switch (pc)
	{
	case Qt::FlatCap:
		lineEndStyle->setCurrentIndex(0);
		break;
	case Qt::SquareCap:
		lineEndStyle->setCurrentIndex(1);
		break;
	case Qt::RoundCap:
		lineEndStyle->setCurrentIndex(2);
		break;
	default:
		lineEndStyle->setCurrentIndex(0);
		break;
	}
	lineEndStyle->blockSignals(false);

	lineJoinStyle->blockSignals(true);
	switch (pj)
	{
	case Qt::MiterJoin:
		lineJoinStyle->setCurrentIndex(0);
		break;
	case Qt::BevelJoin:
		lineJoinStyle->setCurrentIndex(1);
		break;
	case Qt::RoundJoin:
		lineJoinStyle->setCurrentIndex(2);
		break;
	default:
		lineJoinStyle->setCurrentIndex(0);
		break;
	}
	lineJoinStyle->blockSignals(false);
}

void PropertiesPalette_Line::handleLineWidth()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		double oldL = m_item->lineWidth();
		m_doc->itemSelection_SetLineWidth(lineWidth->value() / m_unitRatio);
		if (m_item->dashes().count() != 0)
		{
			if ((oldL != 0.0) && (m_item->lineWidth() != 0.0))
			{
				for (int a = 0; a < m_item->DashValues.count(); a++)
				{
					m_item->DashValues[a] = m_item->DashValues[a] / oldL * m_item->lineWidth();
				}
				m_item->setDashOffset(m_item->dashOffset() / oldL * m_item->lineWidth());
			}
			if (m_item->lineWidth() != 0.0)
			{
				dashEditor->setDashValues(m_item->dashes(), m_item->lineWidth(), m_item->dashOffset());
				dashEditor->setEnabled((m_item->lineWidth() != 0.0));
			}
			else
				dashEditor->setEnabled(false);
		}
	}
}

void PropertiesPalette_Line::handleLineStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if (lineType->currentIndex() == 37)
		{
			if (m_item->dashes().count() == 0)
			{
				if ((m_item->lineStyle() == 0) || (m_item->lineStyle() == 1))
				{
					m_item->DashValues.append(4.0 * qMax(m_item->lineWidth(), 1.0));
					m_item->DashValues.append(2.0 * qMax(m_item->lineWidth(), 1.0));
				}
				else
					getDashArray(m_item->lineStyle(), qMax(m_item->lineWidth(), 1.0), m_item->DashValues);
			}
			if (m_item->lineWidth() != 0.0)
				dashEditor->setDashValues(m_item->dashes(), m_item->lineWidth(), m_item->dashOffset());
			else
			{
				dashEditor->setEnabled(false);
				dashEditor->setDashValues(m_item->dashes(), 1.0, m_item->dashOffset());
			}
			dashEditor->show();
			m_item->update();
		}
		else
		{
			m_item->DashValues.clear();
			dashEditor->hide();
			m_doc->itemSelection_SetLineArt(static_cast<Qt::PenStyle>(lineType->currentIndex()+1));
		}
	}
}

void PropertiesPalette_Line::handleLineJoin()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		Qt::PenJoinStyle c = Qt::MiterJoin;
		switch (lineJoinStyle->currentIndex())
		{
		case 0:
			c = Qt::MiterJoin;
			break;
		case 1:
			c = Qt::BevelJoin;
			break;
		case 2:
			c = Qt::RoundJoin;
			break;
		}
		m_doc->itemSelection_SetLineJoin(c);
	}
}

void PropertiesPalette_Line::handleLineEnd()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		Qt::PenCapStyle c = Qt::FlatCap;
		switch (lineEndStyle->currentIndex())
		{
		case 0:
			c = Qt::FlatCap;
			break;
		case 1:
			c = Qt::SquareCap;
			break;
		case 2:
			c = Qt::RoundCap;
			break;
		}
		m_doc->itemSelection_SetLineEnd(c);
	}
}

void PropertiesPalette_Line::handleLineMode()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_lineMode = (lineMode->currentIndex() == 1); 
	emit lineModeChanged(lineMode->currentIndex());
}

void PropertiesPalette_Line::handleTLines()
{
	if ((m_haveDoc) && (m_haveItem))
	{
		m_item->TopLine    = topLine->isChecked();
		m_item->LeftLine   = leftLine->isChecked();
		m_item->RightLine  = rightLine->isChecked();
		m_item->BottomLine = bottomLine->isChecked();
		m_item->update();
		emit DocChanged();
	}
}

void PropertiesPalette_Line::handleStartArrow(int id)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ApplyArrowHead(id,-1);
}

void PropertiesPalette_Line::handleEndArrow(int id)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ApplyArrowHead(-1, id);
}

void PropertiesPalette_Line::handleStartArrowScale(int sc)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ApplyArrowScale(sc, -1, NULL);
}

void PropertiesPalette_Line::handleEndArrowScale(int sc)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ApplyArrowScale(-1, sc, NULL);
}

void PropertiesPalette_Line::handleDashChange()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if (m_item->lineWidth() != 0.0)
		{
			m_item->setDashes(dashEditor->getDashValues(m_item->lineWidth()));
			m_item->setDashOffset(dashEditor->Offset->value() * m_item->lineWidth());
		}
		m_item->update();
		emit DocChanged();
	}
}

void PropertiesPalette_Line::handleLineStyle(QListWidgetItem *widgetItem)
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning() || !widgetItem)
		return;
	bool setter = (widgetItem->listWidget()->currentRow() == 0);
	m_doc->itemSelection_SetNamedLineStyle(setter ? QString("") : widgetItem->text());
	lineType->setEnabled(setter);
	lineWidth->setEnabled(setter);
	lineJoinStyle->setEnabled(setter);
	lineEndStyle->setEnabled(setter);
}

void PropertiesPalette_Line::languageChange()
{
	int oldLineStyle = lineType->currentIndex();
	lineType->clear();
	lineType->updateList();
	lineType->addItem( tr("Custom"));
	lineType->setCurrentIndex(oldLineStyle);
	int oldLineMode=lineMode->currentIndex();
	lineMode->clear();
	lineMode->addItem( tr("Left Point"));
	lineMode->addItem( tr("End Points"));
	lineMode->setCurrentIndex(oldLineMode);
	lineModeLabel->setText( tr("&Basepoint:"));
	lineTypeLabel->setText( tr("T&ype of Line:"));
	startArrowLabel->setText( tr("Start Arrow:"));
	endArrowLabel->setText( tr("End Arrow:"));
	startArrowScaleLabel->setText( tr("Scaling:"));
	endArrowScaleLabel->setText( tr("Scaling:"));
	if (m_haveDoc)
	{
		int arrowItem = startArrow->currentIndex();
		startArrow->rebuildList(&m_doc->arrowStyles());
		startArrow->setCurrentIndex(arrowItem);
		arrowItem = endArrow->currentIndex();
		endArrow->rebuildList(&m_doc->arrowStyles());
		endArrow->setCurrentIndex(arrowItem);
	}
	lineWidthLabel->setText( tr("Line &Width:"));
	lineJoinLabel->setText( tr("Ed&ges:"));

	int oldLJoinStyle=lineJoinStyle->currentIndex();
	lineJoinStyle->clear();
	lineJoinStyle->addItem(loadIcon("16/stroke-join-miter.png"), tr("Miter Join"));
	lineJoinStyle->addItem(loadIcon("16/stroke-join-bevel.png"), tr("Bevel Join"));
	lineJoinStyle->addItem(loadIcon("16/stroke-join-round.png"), tr("Round Join"));
	lineJoinStyle->setCurrentIndex(oldLJoinStyle);

	int oldLEndStyle=lineEndStyle->currentIndex();
	lineEndStyle->clear();
	lineEndStyle->addItem(loadIcon("16/stroke-cap-butt.png"), tr("Flat Cap"));
	lineEndStyle->addItem(loadIcon("16/stroke-cap-square.png"), tr("Square Cap"));
	lineEndStyle->addItem(loadIcon("16/stroke-cap-round.png"), tr("Round Cap"));
	lineEndStyle->setCurrentIndex(oldLEndStyle);
	lineEndLabel->setText( tr("&Endings:"));

	tableLineGroup->setTitle( tr("Cell Lines"));
	topLine->setText( tr("Line at Top"));
	leftLine->setText( tr("Line at the Left"));
	rightLine->setText( tr("Line at the Right "));
	bottomLine->setText( tr("Line at Bottom"));

	QString pctSuffix = tr(" %");
	startArrowScale->setSuffix(pctSuffix);
	endArrowScale->setSuffix(pctSuffix);

	QString ptSuffix = tr(" pt");
	QString suffix = (m_doc) ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;

	lineWidth->setSuffix(suffix);
	lineWidth->setSpecialValueText( tr("Hairline"));

	if(lineStyles->count() > 0)
		lineStyles->item(0)->setText( tr("No Style") );

	lineMode->setToolTip( tr("Change settings for left or end points"));
	lineType->setToolTip( tr("Pattern of line"));
	lineWidth->setToolTip( tr("Thickness of line"));
	lineJoinStyle->setToolTip( tr("Type of line joins"));
	lineEndStyle->setToolTip( tr("Type of line end"));
	lineStyles->setToolTip( tr("Line style of current object"));
	startArrow->setToolTip( tr("Arrow head style for start of line"));
	endArrow->setToolTip( tr("Arrow head style for end of line"));
	startArrowScale->setToolTip( tr("Arrow head scale for start of line"));
	endArrowScale->setToolTip( tr("Arrow head scale for end of line"));
}

void PropertiesPalette_Line::unitChange()
{
	if (!m_doc)
		return;

	double oldRatio = m_unitRatio;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	lineWidth->blockSignals(true);
	lineWidth->setNewUnit( m_unitIndex );
	lineWidth->blockSignals(false);
}
