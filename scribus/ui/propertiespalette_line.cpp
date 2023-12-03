/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertiespalette_line.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <QButtonGroup>
#include <cmath>
#include <QSignalBlocker>

#include "dasheditor.h"
#include "iconmanager.h"
#include "linemarkerselector.h"
#include "localemgr.h"
#include "pageitem.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"
#include "ui/propertiespalette_utils.h"
#include "ui/widgets/popup_menu.h"
#include "units.h"
#include "util.h"


//using namespace std;

const double HAIRLINE = 0.001;

PropertiesPalette_Line::PropertiesPalette_Line( QWidget* parent) : QWidget(parent)
{
	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	buttonsCaps = new QButtonGroup();
	buttonsCaps->addButton(buttonCapFlat, 0);
	buttonsCaps->addButton(buttonCapSquare, 1);
	buttonsCaps->addButton(buttonCapRound, 2);

	buttonsJoins = new QButtonGroup();
	buttonsJoins->addButton(buttonJoinMiter, 0);
	buttonsJoins->addButton(buttonJoinBevel, 1);
	buttonsJoins->addButton(buttonJoinRound, 2);

	numberOpacity->setDecimals(0);

	lineType->addItem( tr("Custom"));

	lineMarkerSelectorStart = new LineMarkerSelector();
	lineMarkerSelectorStart->setArrowDirection(ArrowDirection::StartArrow);
	PopupMenu *menuLineMarkerStart = new PopupMenu(lineMarkerSelectorStart);
	buttonMarkerStart->setMenu(menuLineMarkerStart);

	lineMarkerSelectorEnd = new LineMarkerSelector();
	lineMarkerSelectorEnd->setArrowDirection(ArrowDirection::EndArrow);
	PopupMenu *menuLineMarkerEnd = new PopupMenu(lineMarkerSelectorEnd);
	buttonMarkerEnd->setMenu(menuLineMarkerEnd);

	lineStyles->setItemDelegate(new LineStyleItemDelegate());
	lineStyles->addItem( "No Style" );

	languageChange();
	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));

	connect(lineWidth, SIGNAL(valueChanged(double)), this, SLOT(handleLineWidth()));
	connect(lineType, SIGNAL(activated(int)), this, SLOT(handleLineStyle()));
	connect(buttonsJoins, SIGNAL(idClicked(int)), this, SLOT(handleLineJoin()));
	connect(buttonsCaps, SIGNAL(idClicked(int)), this, SLOT(handleLineEnd()));
	connect(dashEditor, SIGNAL(dashChanged()), this, SLOT(handleDashChange()));
	connect(lineMarkerSelectorStart, SIGNAL(markerChanged(int)), this, SLOT(handleStartArrow(int)));
	connect(lineMarkerSelectorStart, SIGNAL(scaleChanged(double)), this, SLOT(handleStartArrowScale(double)));
	connect(lineMarkerSelectorEnd, SIGNAL(markerChanged(int)), this, SLOT(handleEndArrow(int)));
	connect(lineMarkerSelectorEnd, SIGNAL(scaleChanged(double)), this, SLOT(handleEndArrowScale(double)));
	connect(lineStyles, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(handleLineStyle(QListWidgetItem*)));
	connect(buttonSwapMarker, SIGNAL(clicked(bool)), this, SLOT(swapLineMarker()));
	connect(comboBlendmode, SIGNAL(currentIndexChanged(int)), this, SLOT(handleLineBlendmode(int)));
	connect(numberOpacity, SIGNAL(valueChanged(double)), this, SLOT(handleLineOpacity(double)));

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
	PageItem *currentItem = nullptr;

	if (m_doc)
		if (m_doc->m_Selection->count() >= 1)
			currentItem = m_doc->m_Selection->itemAt(0);		

	return currentItem;
}

void PropertiesPalette_Line::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Line::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = nullptr;
	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();

	m_haveDoc  = true;
	m_haveItem = false;

	lineWidth->setMaximum( 300 );
	lineWidth->setMinimum( 0 );

	updateLineStyles(m_doc);

	lineMarkerSelectorStart->rebuildList(&m_doc->arrowStyles());
	buttonMarkerStart->setIcon(lineMarkerSelectorStart->currentIcon());

	lineMarkerSelectorEnd->rebuildList(&m_doc->arrowStyles());
	buttonMarkerEnd->setIcon(lineMarkerSelectorEnd->currentIcon());

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Line::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));
	}

	m_haveDoc  = false;
	m_haveItem = false;
	m_doc   = nullptr;
	m_item  = nullptr;

	updateLineStyles(nullptr);

	setEnabled(false);
}

void PropertiesPalette_Line::unsetItem()
{
	m_haveItem = false;
	m_item     = nullptr;
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
		setEnabled(true);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;
		m_haveItem = (itemType != -1);

		switch (itemType)
		{
		case -1:
			setEnabled(false);
			lineMarkerSelectorStart->setMarker(0);
			buttonMarkerStart->setIcon(lineMarkerSelectorStart->currentIcon());
			lineMarkerSelectorEnd->setMarker(0);
			buttonMarkerEnd->setIcon(lineMarkerSelectorEnd->currentIcon());
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
			setEnabled(currItem->asOSGFrame() == nullptr);
			break;
		case PageItem::Line:
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

	lineStyles->blockSignals(true);
	lineMarkerSelectorStart->blockSignals(true);
	lineMarkerSelectorEnd->blockSignals(true);
	if ((m_item->asLine()) || (m_item->asPolyLine()) || (m_item->asSpiral()))
	{
		lineMarkerSelectorStart->setMarker(m_item->startArrowIndex());
		lineMarkerSelectorStart->setScale(m_item->startArrowScale());
		buttonMarkerStart->setIcon(lineMarkerSelectorStart->currentIcon());
		lineMarkerSelectorEnd->setMarker(m_item->endArrowIndex());
		lineMarkerSelectorEnd->setScale(m_item->endArrowScale());
		buttonMarkerEnd->setIcon(lineMarkerSelectorEnd->currentIcon());
		lineMarkerLabel->setVisible(true);
	}
	else
	{
		lineMarkerLabel->setVisible(false);
	}

	if (lineStyles->currentItem())
		lineStyles->currentItem()->setSelected(false);

	bool setter = false;
	if (m_item->NamedLStyle.isEmpty())
	{
		setter = true;
		QListWidgetItem *itemStl = nullptr;
		itemStl = lineStyles->item(0);
		if (itemStl != nullptr)
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
	lineJoinLabel->setEnabled(setter);
	lineEndLabel->setEnabled(setter);

	if (m_item->dashes().count() == 0)
		dashEditor->hide();
	else
	{
		lineType->setCurrentIndex(37);
		dashEditor->setDashValues(m_item->dashes(), qMax(m_item->lineWidth(), HAIRLINE), m_item->dashOffset());
		dashEditor->show();
	}

	lineStyles->blockSignals(false);
	lineMarkerSelectorStart->blockSignals(false);
	lineMarkerSelectorEnd->blockSignals(false);

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
		setEnabled(false);

	m_haveItem = true;

	showLineWidth(m_item->lineWidth());
	showLineValues(m_item->lineStyle(), m_item->lineEnd(), m_item->lineJoin());

	if (m_item->isOSGFrame())
		setEnabled(false);
	if (m_item->isSymbol())
		setEnabled(false);
}

void PropertiesPalette_Line::updateArrowStyles()
{
	updateArrowStyles(m_doc);
}

void PropertiesPalette_Line::updateArrowStyles(ScribusDoc *doc)
{
	if (!doc)
		return;
	lineMarkerSelectorStart->rebuildList(&doc->arrowStyles());
	buttonMarkerStart->setIcon(lineMarkerSelectorStart->currentIcon());
	lineMarkerSelectorEnd->rebuildList(&doc->arrowStyles());
	buttonMarkerEnd->setIcon(lineMarkerSelectorEnd->currentIcon());
}

void PropertiesPalette_Line::updateLineStyles()
{
	updateLineStyles(m_doc);
}

void PropertiesPalette_Line::updateLineStyles(ScribusDoc *doc)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	
	lineStyles->blockSignals(true);
	lineStyles->clear();
	if (doc != nullptr)
	{
		QHash<QString,multiLine>::Iterator it;
		for (it = doc->docLineStyles.begin(); it != doc->docLineStyles.end(); ++it)
			lineStyles->addItem( new LineStyleItem(doc, it.value(), it.key()) );
		lineStyles->sortItems();
		lineStyles->insertItem( 0, tr("No Style"));
		if (lineStyles->currentItem())
			lineStyles->currentItem()->setSelected(false);
	}
	lineStyles->blockSignals(false);
}

void PropertiesPalette_Line::showLineWidth(double s)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	lineWidth->showValue(s * m_unitRatio);
	if (m_haveItem)
	{
		if (m_item->dashes().count() != 0)
		{
			dashEditor->blockSignals(true);
			if (m_item->lineWidth() > HAIRLINE)
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

void PropertiesPalette_Line::showLineValues(Qt::PenStyle p, Qt::PenCapStyle pc, Qt::PenJoinStyle pj)
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
			dashEditor->setDashValues(m_item->dashes(), qMax(m_item->lineWidth(), HAIRLINE), m_item->dashOffset());
		}
		else
			lineType->setCurrentIndex(static_cast<int>(p) - 1);
	}
	else
		lineType->setCurrentIndex(static_cast<int>(p) - 1);
	dashEditor->blockSignals(false);
	lineType->blockSignals(false);

	buttonsCaps->blockSignals(true);
	switch (pc)
	{
	case Qt::FlatCap:
	default:
		buttonCapFlat->setChecked(true);
		break;
	case Qt::SquareCap:
		buttonCapSquare->setChecked(true);
		break;
	case Qt::RoundCap:
		buttonCapRound->setChecked(true);
		break;
	}
	buttonsCaps->blockSignals(false);

	buttonsJoins->blockSignals(true);
	switch (pj)
	{
	case Qt::MiterJoin:
	default:
		buttonJoinMiter->setChecked(true);
		break;
	case Qt::BevelJoin:
		buttonJoinBevel->setChecked(true);
		break;
	case Qt::RoundJoin:
		buttonJoinRound->setChecked(true);
		break;
	}
	buttonsJoins->blockSignals(false);
}

void PropertiesPalette_Line::handleLineWidth()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc && m_haveItem)
	{
		double oldL = m_item->lineWidth();
		m_doc->itemSelection_SetLineWidth(lineWidth->value() / m_unitRatio);

		if (m_item->dashes().count() != 0)
		{
			if ((oldL > HAIRLINE) && (m_item->lineWidth() > HAIRLINE))
			{
				for (int a = 0; a < m_item->DashValues.count(); a++)
				{
					m_item->DashValues[a] = m_item->DashValues[a] / oldL * m_item->lineWidth();
				}
				m_item->setDashOffset(m_item->dashOffset() / oldL * m_item->lineWidth());
			}
			if (m_item->lineWidth() > HAIRLINE)
			{
				dashEditor->setDashValues(m_item->dashes(), m_item->lineWidth(), m_item->dashOffset());
				dashEditor->setEnabled((m_item->lineWidth() > HAIRLINE));
			}
			else
				dashEditor->setEnabled(false);
		}
		m_doc->invalidateAll();
		m_doc->regionsChanged()->update(QRect());
	}
}

void PropertiesPalette_Line::handleLineStyle()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc && m_haveItem)
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
			if (m_item->lineWidth() > HAIRLINE)
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
	if (m_haveDoc && m_haveItem)
	{
		Qt::PenJoinStyle c = Qt::MiterJoin;
		switch (buttonsJoins->checkedId())
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
	if (m_haveDoc && m_haveItem)
	{
		Qt::PenCapStyle c = Qt::FlatCap;

		switch (buttonsCaps->checkedId())
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

void PropertiesPalette_Line::handleStartArrow(int id)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ApplyArrowHead(id,-1);
	buttonMarkerStart->setIcon(lineMarkerSelectorStart->currentIcon());
}

void PropertiesPalette_Line::handleEndArrow(int id)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ApplyArrowHead(-1, id);
	buttonMarkerEnd->setIcon(lineMarkerSelectorEnd->currentIcon());
}

void PropertiesPalette_Line::handleStartArrowScale(double sc)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ApplyArrowScale(static_cast<int>(sc), -1, nullptr);
}

void PropertiesPalette_Line::handleEndArrowScale(double sc)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_ApplyArrowScale(-1, static_cast<int>(sc), nullptr);
}

void PropertiesPalette_Line::handleDashChange()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc && m_haveItem)
	{
		if (m_item->lineWidth() > HAIRLINE)
		{
			m_item->setDashes(dashEditor->getDashValues(m_item->lineWidth()));
			m_item->setDashOffset(dashEditor->Offset->value() * m_item->lineWidth());
		}
		m_item->update();
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
	lineJoinLabel->setEnabled(setter);
	lineEndLabel->setEnabled(setter);

}

void PropertiesPalette_Line::handleLineOpacity(double opacity)
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	m_doc->itemSelection_SetItemLineTransparency((100 - numberOpacity->value()) / 100.0);
}

void PropertiesPalette_Line::handleLineBlendmode(int mode)
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	m_doc->itemSelection_SetItemLineBlend(comboBlendmode->currentIndex());
}

void PropertiesPalette_Line::swapLineMarker()
{
	int startID = lineMarkerSelectorStart->marker();
	double startScale = lineMarkerSelectorStart->scale();
	int endID = lineMarkerSelectorEnd->marker();
	double endScale = lineMarkerSelectorEnd->scale();

	// Info: Setter trigger signals like a change by user.
	lineMarkerSelectorStart->setMarker(endID);
	lineMarkerSelectorStart->setScale(endScale);
	lineMarkerSelectorEnd->setMarker(startID);
	lineMarkerSelectorEnd->setScale(startScale);

}

void PropertiesPalette_Line::iconSetChange()
{
	IconManager& im = IconManager::instance();

	buttonJoinMiter->setIcon(im.loadIcon("16/stroke-join-miter.png"));
	buttonJoinBevel->setIcon(im.loadIcon("16/stroke-join-bevel.png"));
	buttonJoinRound->setIcon(im.loadIcon("16/stroke-join-round.png"));

	buttonCapFlat->setIcon(im.loadIcon("16/stroke-cap-butt.png"));
	buttonCapRound->setIcon(im.loadIcon("16/stroke-cap-round.png"));
	buttonCapSquare->setIcon(im.loadIcon("16/stroke-cap-square.png"));

	buttonSwapMarker->setIcon(im.loadIcon("swap"));
}

void PropertiesPalette_Line::languageChange()
{
	QSignalBlocker lineTypeBlocker(lineType);
	int oldLineStyle = lineType->currentIndex();
	lineType->clear();
	lineType->updateList();
	lineType->addItem( tr("Custom"));
	lineType->setCurrentIndex(oldLineStyle);

	lineTypeLabel->setText( tr("T&ype"));
	lineMarkerLabel->setText( tr("&Marker"));

	if (m_haveDoc)
	{
		QSignalBlocker startArrowBlocker(lineMarkerSelectorStart);
		lineMarkerSelectorStart->rebuildList(&m_doc->arrowStyles());
		buttonMarkerStart->setIcon(lineMarkerSelectorStart->currentIcon());

		QSignalBlocker endArrowBlocker(lineMarkerSelectorEnd);
		lineMarkerSelectorEnd->rebuildList(&m_doc->arrowStyles());
		buttonMarkerEnd->setIcon(lineMarkerSelectorEnd->currentIcon());
	}
	lineWidthLabel->setText( tr("&Width"));
	lineJoinLabel->setText( tr("&Joins"));
	lineEndLabel->setText( tr("&Caps"));

	lineMarkerSelectorStart->languageChange();
	lineMarkerSelectorEnd->languageChange();

	QString ptSuffix = tr(" pt");
	QString suffix = (m_doc) ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;

	lineWidth->setSuffix(suffix);
	lineWidth->setSpecialValueText( tr("Hairline"));

	if(lineStyles->count() > 0)
		lineStyles->item(0)->setText( tr("No Style") );

	lineType->setToolTip( tr("Pattern of line"));
	lineWidth->setToolTip( tr("Thickness of line"));
	lineStyles->setToolTip( tr("Line style of current object"));
	buttonCapFlat->setToolTip( tr("Line end is flat"));
	buttonCapSquare->setToolTip( tr("Line end is squared"));
	buttonCapRound->setToolTip( tr("Line end is rounded"));
	buttonJoinMiter->setToolTip( tr("Line join is mitered"));
	buttonJoinRound->setToolTip( tr("Line join is rounded"));
	buttonJoinBevel->setToolTip( tr("Line join is beveled"));
	buttonMarkerStart->setToolTip( tr("Arrow head style for start of line"));
	buttonMarkerEnd->setToolTip( tr("Arrow head style for end of line"));
}

void PropertiesPalette_Line::unitChange()
{
	if (!m_doc)
		return;

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	lineWidth->blockSignals(true);
	lineWidth->setNewUnit( m_unitIndex );
	lineWidth->blockSignals(false);
}

void PropertiesPalette_Line::localeChange()
{
	const QLocale& l(LocaleManager::instance().userPreferredLocale());
	lineWidth->setLocale(l);
}

void PropertiesPalette_Line::toggleLabelVisibility(bool v)
{
	lineWidthLabel->setLabelVisibility(v);
	lineJoinLabel->setLabelVisibility(v);
	lineTypeLabel->setLabelVisibility(v);
	lineEndLabel->setLabelVisibility(v);
	lineMarkerLabel->setLabelVisibility(v);
	lineOpacityLabel->setLabelVisibility(v);
	lineBlendmodeLabel->setLabelVisibility(v);
}
