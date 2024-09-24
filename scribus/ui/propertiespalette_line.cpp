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
#include "ui/colorpicker/colorpicker_enum.h"
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

	lineType->addItem( tr("Custom"));

	lineMarkerSelectorStart = new LineMarkerSelector();
	lineMarkerSelectorStart->setArrowDirection(ArrowDirection::StartArrow);
	PopupMenu *menuLineMarkerStart = new PopupMenu(lineMarkerSelectorStart);
	buttonMarkerStart->setMenu(menuLineMarkerStart);

	lineMarkerSelectorEnd = new LineMarkerSelector();
	lineMarkerSelectorEnd->setArrowDirection(ArrowDirection::EndArrow);
	PopupMenu *menuLineMarkerEnd = new PopupMenu(lineMarkerSelectorEnd);
	buttonMarkerEnd->setMenu(menuLineMarkerEnd);

	buttonLineColor->setContext(Context::Line);
	buttonLineColor->setMenuContextType(ColorButton::Floating);
	buttonLineMask->setContext(Context::LineMask);
	buttonLineMask->setMenuContextType(ColorButton::Floating);

	languageChange();
	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));

	connect(lineWidth, SIGNAL(valueChanged(double)), this, SLOT(handleLineWidth()));
	connect(lineType, SIGNAL(activated(int)), this, SLOT(handleLineType()));
	connect(buttonsJoins, SIGNAL(idClicked(int)), this, SLOT(handleLineJoin()));
	connect(buttonsCaps, SIGNAL(idClicked(int)), this, SLOT(handleLineEnd()));
	connect(dashEditor, SIGNAL(dashChanged()), this, SLOT(handleDashChange()));
	connect(lineMarkerSelectorStart, SIGNAL(markerChanged(int)), this, SLOT(handleStartArrow(int)));
	connect(lineMarkerSelectorStart, SIGNAL(scaleChanged(double)), this, SLOT(handleStartArrowScale(double)));
	connect(lineMarkerSelectorEnd, SIGNAL(markerChanged(int)), this, SLOT(handleEndArrow(int)));
	connect(lineMarkerSelectorEnd, SIGNAL(scaleChanged(double)), this, SLOT(handleEndArrowScale(double)));
	connect(buttonSwapMarker, SIGNAL(clicked(bool)), this, SLOT(swapLineMarker()));
	connect(comboBlendmode, SIGNAL(currentIndexChanged(int)), this, SLOT(handleLineBlendmode(int)));
	connect(comboLineStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(handleLineStyle(int)));
	connect(buttonLineColor, SIGNAL(changed()), this, SLOT(handleLineColor()));
	connect(buttonLineColor, SIGNAL(gradientVectorChanged()), this, SLOT(handleLineColorVector()));
	connect(buttonLineMask, SIGNAL(changed()), this, SLOT(handleLineOpacity()));

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
	if ((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
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

	comboLineStyle->setDoc(m_doc);
	updateLineStyles(m_doc);

	lineMarkerSelectorStart->rebuildList(&m_doc->arrowStyles());
	buttonMarkerStart->setIcon(lineMarkerSelectorStart->currentIcon());

	lineMarkerSelectorEnd->rebuildList(&m_doc->arrowStyles());
	buttonMarkerEnd->setIcon(lineMarkerSelectorEnd->currentIcon());

	buttonLineColor->setDoc(m_doc);
	buttonLineMask->setDoc(m_doc);

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

	comboLineStyle->setDoc(m_doc);
	buttonLineColor->unsetDoc();
	buttonLineMask->unsetDoc();
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

	if (updatesBlocked())
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
		case PageItem::Spiral:
		case PageItem::Table:
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

	QSignalBlocker sigLineStyle(comboLineStyle);
	QSignalBlocker sigMarkerSelectorStart(lineMarkerSelectorStart);
	QSignalBlocker sigMarkerSelectorEnd(lineMarkerSelectorEnd);
	QSignalBlocker sigLineBlendMode(comboBlendmode);

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
		lineMarkerLabel->setVisible(false);

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
		setEnabled(false);
	if (m_item->isOSGFrame())
		setEnabled(false);
	if (m_item->isSymbol())
		setEnabled(false);

	bool hasStyle = false;
	if (m_item->NamedLStyle.isEmpty())
	{
		hasStyle = false;
		comboLineStyle->setCurrentIndex(0);
	}
	else
	{
		hasStyle = true;
		comboLineStyle->setCurrentText(m_item->NamedLStyle);
	}

	lineTypeLabel->setVisible(!hasStyle);
	lineWidthLabel->setVisible(!hasStyle);
	lineJoinLabel->setVisible(!hasStyle);
	lineEndLabel->setVisible(!hasStyle);
	lineColorLabel->setVisible(!hasStyle);

	if (m_item->dashes().count() == 0 || hasStyle)
		dashEditor->hide();
	else
	{
		lineType->setCurrentIndex(37);
		dashEditor->setDashValues(m_item->dashes(), qMax(m_item->lineWidth(), HAIRLINE), m_item->dashOffset());
		dashEditor->show();
	}

	comboBlendmode->setCurrentIndex(m_item->lineBlendmode());

	m_haveItem = true;

	setCurrentItem_Line(m_item);
	setCurrentItem_LineMask(m_item);

	showLineWidth(m_item->lineWidth());
	showLineValues(m_item->lineStyle(), m_item->lineEnd(), m_item->lineJoin());

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
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !doc)
		return;

	comboLineStyle->updateLineStyles();
}

void PropertiesPalette_Line::setCurrentItem_Line(PageItem *item)
{
	buttonLineColor->setType(item->strokeGradientType());
	// Gradient
	buttonLineColor->setGradient(
				item->strokeGradient(),
				item->stroke_gradient,
				item->getStrokeGradientExtend()
				);

	// Gradient Vector
	buttonLineColor->setGradientVector(
				item->gradientStrokeStartX(),
				item->gradientStrokeStartY(),
				item->gradientStrokeEndX(),
				item->gradientStrokeEndY(),
				item->gradientStrokeFocalX(),
				item->gradientStrokeFocalY(),
				item->gradientStrokeScale(),
				item->gradientStrokeSkew()
				);

	// Pattern
	bool mirrorX, mirrorY;
	item->strokePatternFlip(mirrorX, mirrorY);
	buttonLineColor->setPattern(
				item->strokePattern(),
				item->strokePatternTransform().offsetX,
				item->strokePatternTransform().offsetY,
				item->strokePatternTransform().scaleX,
				item->strokePatternTransform().scaleY,
				item->strokePatternTransform().skewX,
				item->strokePatternTransform().skewY,
				item->strokePatternTransform().rotation,
				item->strokePatternTransform().space,
				mirrorX, mirrorY,
				item->isStrokePatternToPath()
				);
	// Color
	buttonLineColor->setColor(
				item->lineColor(),
				item->lineShade()
				);
	// General
	buttonLineColor->setGeneral(item->overprint());

	buttonLineColor->updatePreview();
	buttonLineColor->updateFloatingContext();

}

void PropertiesPalette_Line::setCurrentItem_LineMask(PageItem *item)
{
	buttonLineMask->setType(Gradient_None);
	// Color
	buttonLineMask->setColor(
				"",
				100,
				item->lineTransparency()
				);

	buttonLineMask->updatePreview();
	buttonLineMask->updateFloatingContext();

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

void PropertiesPalette_Line::handleLineType()
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

void PropertiesPalette_Line::handleLineStyle(int c)
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool setter = (c == 0);
	m_doc->itemSelection_SetNamedLineStyle(setter ? QString("") : comboLineStyle->currentText());
}

void PropertiesPalette_Line::handleLineOpacity(/*double opacity*/)
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	blockUpdates(true);

	switch (buttonLineMask->mode())
	{
	default:
	case Mode::Solid:
		m_doc->itemSelection_SetItemLineTransparency(buttonLineMask->colorData().Opacity);
		break;
	case Mode::Gradient:
	case Mode::Hatch:
	case Mode::Pattern:
		// Do nothing
		break;
	}
	blockUpdates(false);
}

void PropertiesPalette_Line::handleLineBlendmode(int mode)
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	m_doc->itemSelection_SetItemLineBlend(comboBlendmode->currentIndex());
}

void PropertiesPalette_Line::handleLineColor()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	blockUpdates(true);
	m_doc->itemSelection_SetItemGradStroke(buttonLineColor->type());

	switch(buttonLineColor->mode())
	{
	default:
	case Mode::Solid:
		m_doc->itemSelection_SetItemStrokePattern(""); // reset pattern
		m_doc->itemSelection_SetItemPen(buttonLineColor->colorName());
		m_doc->itemSelection_SetItemPenShade(static_cast<int>(buttonLineColor->colorData().Shade));
		break;
	case Mode::Gradient:

		m_doc->itemSelection_SetItemStrokePattern(""); // reset pattern

		if (buttonLineColor->gradientData().Name.isEmpty())
		{
			m_item->setStrokeGradient("");
			m_doc->itemSelection_SetLineGradient(buttonLineColor->gradientData().Gradient);
		}
		else
		{
			m_item->setStrokeGradient(buttonLineColor->gradientData().Name);
		}

		m_item->setStrokeGradientExtend(buttonLineColor->gradientData().repeatMethod);
		m_item->update();
		m_doc->regionsChanged()->update(QRect());

		break;
	case Mode::Hatch:
		// Do nothing
		break;
	case Mode::Pattern:
		m_doc->itemSelection_SetItemStrokePattern(buttonLineColor->patternData().Name);
		m_doc->itemSelection_SetItemStrokePatternType(buttonLineColor->patternData().OnPath);
		m_doc->itemSelection_SetItemStrokePatternProps(
					buttonLineColor->patternData().ScaleX,
					buttonLineColor->patternData().ScaleY,
					buttonLineColor->patternData().OffsetX,
					buttonLineColor->patternData().OffsetY,
					buttonLineColor->patternData().Angle,
					buttonLineColor->patternData().SkewX,
					buttonLineColor->patternData().SkewY,
					buttonLineColor->patternData().Spacing,
					buttonLineColor->patternData().MirrorX,
					buttonLineColor->patternData().MirrorY
					);
		break;
	}

	m_doc->itemSelection_SetOverprint(buttonLineColor->generalData().overprint);

	//	m_item->update();
	//	m_doc->regionsChanged()->update(QRect());
	blockUpdates(false);

}

void PropertiesPalette_Line::handleLineColorVector()
{
	if (!m_doc ||!m_item || !m_ScMW || m_ScMW->scriptIsRunning() || buttonLineColor->mode() != Mode::Gradient)
		return;

	CPGradientVectorData gvd = buttonLineColor->gradientVectorData();

	QRectF upRect;

	switch (buttonLineColor->type())
	{
	case Gradient_Linear:
	case Gradient_Radial:
		m_item->setGradientStrokeStart(gvd.StartX, gvd.StartY);
		m_item->setGradientStrokeEnd(gvd.EndX, gvd.EndY);
		m_item->setGradientStrokeFocal(gvd.FocalX, gvd.FocalY);
		m_item->setGradientStrokeScale(gvd.Scale);
		m_item->setGradientStrokeSkew(gvd.Skew);
		if (m_item->strokeGradientType() == Gradient_Linear)
		{
			m_item->setGradientStrokeFocal(m_item->gradientStrokeStartX(), m_item->gradientStrokeStartY());
		}
		m_item->update();
		upRect = QRectF(m_item->gradientStrokeStart(), m_item->gradientStrokeEnd());
		double radEnd = distance(m_item->gradientStrokeEndX() - m_item->gradientStrokeStartX(), m_item->gradientStrokeEndY() - m_item->gradientStrokeStartY());
		double rotEnd = xy2Deg(m_item->gradientStrokeEndX() - m_item->gradientStrokeStartX(), m_item->gradientStrokeEndY() - m_item->gradientStrokeStartY());
		QTransform m;
		m.translate(m_item->gradientStrokeStartX(), m_item->gradientStrokeStartY());
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(m_item->gradientStrokeSkew());
		m.translate(radEnd * m_item->gradientStrokeScale(), 0);
		QPointF shP = m.map(QPointF(0,0));
		upRect = upRect.united(QRectF(shP, m_item->gradientStrokeEnd()).normalized());
		upRect = upRect.united(QRectF(shP, m_item->gradientStrokeStart()).normalized());
		upRect |= QRectF(shP, QPointF(0, 0)).normalized();
		upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		break;
	}

	upRect.translate(m_item->xPos(), m_item->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
	m_doc->changed();
	m_doc->changedPagePreview();
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
	buttonLineMask->setDotIcon(im.loadIcon("mask", 8));

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
	comboLineStyle->languageChange();

	QString ptSuffix = tr(" pt");
	QString suffix = (m_doc) ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;

	lineWidth->setSuffix(suffix);
	lineWidth->setSpecialValueText( tr("Hairline"));

	lineType->setToolTip( tr("Pattern of line"));
	lineWidth->setToolTip( tr("Thickness of line"));
	buttonCapFlat->setToolTip( tr("Line end is flat"));
	buttonCapSquare->setToolTip( tr("Line end is squared"));
	buttonCapRound->setToolTip( tr("Line end is rounded"));
	buttonJoinMiter->setToolTip( tr("Line join is mitered"));
	buttonJoinRound->setToolTip( tr("Line join is rounded"));
	buttonJoinBevel->setToolTip( tr("Line join is beveled"));
	buttonMarkerStart->setToolTip( tr("Marker style for start of line"));
	buttonMarkerEnd->setToolTip( tr("Marker style for end of line"));
	buttonSwapMarker->setToolTip( tr("Swap start and end markers"));
	comboBlendmode->setToolTip( tr("Blendmode of line"));
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
	lineColorLabel->setLabelVisibility(v);
	lineWidthLabel->setLabelVisibility(v);
	lineJoinLabel->setLabelVisibility(v);
	lineTypeLabel->setLabelVisibility(v);
	lineEndLabel->setLabelVisibility(v);
	lineMarkerLabel->setLabelVisibility(v);
	lineMaskLabel->setLabelVisibility(v);
	lineBlendmodeLabel->setLabelVisibility(v);
	buttonLineMask->setHasDot(!v);
}
