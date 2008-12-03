/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2007 by Franz Schmid                                     *
*   franz.schmid@altmuehlnet.de                                            *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
*                                                                          *
*   This program is distributed in the hope that it will be useful,        *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*   GNU General Public License for more details.                           *
*                                                                          *
*   You should have received a copy of the GNU General Public License      *
*   along with this program; if not, write to the                          *
*   Free Software Foundation, Inc.,                                        *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.              *
****************************************************************************/

#include "pathstroker.h"
#include "pageitem_polygon.h"
#include "commonstrings.h"
#include "scribuscore.h"
#include "scribusstructs.h"
#include "util.h"
#include <QPainterPathStroker>

int pathstroker_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* pathstroker_getPlugin()
{
	PathStrokerPlugin* plug = new PathStrokerPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void pathstroker_freePlugin(ScPlugin* plugin)
{
	PathStrokerPlugin* plug = dynamic_cast<PathStrokerPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

PathStrokerPlugin::PathStrokerPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

PathStrokerPlugin::~PathStrokerPlugin() {};

void PathStrokerPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "PathStroker";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Create Path from Stroke");
	// Menu
	m_actionInfo.menu = "ItemPathOps";
	m_actionInfo.parentMenu = "Item";
	m_actionInfo.subMenuName = tr("Path Tools");
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.notSuitableFor.append(PageItem::TextFrame);
	m_actionInfo.notSuitableFor.append(PageItem::ImageFrame);
	m_actionInfo.notSuitableFor.append(PageItem::PathText);
	m_actionInfo.notSuitableFor.append(PageItem::LatexFrame);
	m_actionInfo.forAppMode.append(modeNormal);
	m_actionInfo.needsNumObjects = 1;
}

const QString PathStrokerPlugin::fullTrName() const
{
	return QObject::tr("PathStroker");
}

const ScActionPlugin::AboutData* PathStrokerPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Create Path from Stroke");
	about->description = tr("Converts the stroke of a Path to a filled Path.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void PathStrokerPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool PathStrokerPlugin::run(ScribusDoc* doc, QString)
{
	ScribusDoc* currDoc = doc;
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc->m_Selection->count() > 0)
	{
		QVector<double> m_array;
		PageItem *currItem = currDoc->m_Selection->itemAt(0);
		FPointArray path = currItem->PoLine;
		QPainterPath pp;
		if (currItem->itemType() == PageItem::PolyLine)
			pp = path.toQPainterPath(false);
		else
			pp = path.toQPainterPath(true);
		if (currItem->NamedLStyle.isEmpty())
		{
			QPainterPathStroker stroke;
			stroke.setCapStyle(currItem->lineEnd());
			stroke.setJoinStyle(currItem->lineJoin());
			if (currItem->lineStyle() == Qt::SolidLine)
				stroke.setDashPattern(currItem->lineStyle());
			else
			{
				getDashArray(currItem->lineStyle(), 1, m_array);
				stroke.setDashPattern(m_array);
			}
			stroke.setWidth(currItem->lineWidth());
			QPainterPath result = stroke.createStroke(pp);
			if (currItem->startArrowIndex() != 0)
			{
				FPoint Start = currItem->PoLine.point(0);
				for (uint xx = 1; xx < currItem->PoLine.size(); xx += 2)
				{
					FPoint Vector = currItem->PoLine.point(xx);
					if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
					{
						double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
						QMatrix arrowTrans;
						FPointArray arrow = currDoc->arrowStyles.at(currItem->startArrowIndex()-1).points.copy();
						arrowTrans.translate(Start.x(), Start.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(currItem->lineWidth(), currItem->lineWidth());
						arrow.map(arrowTrans);
						result.addPath(arrow.toQPainterPath(true));
						break;
					}
				}
			}
			if (currItem->endArrowIndex() != 0)
			{
				FPoint End = currItem->PoLine.point(currItem->PoLine.size()-2);
				for (uint xx = currItem->PoLine.size()-1; xx > 0; xx -= 2)
				{
					FPoint Vector = currItem->PoLine.point(xx);
					if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
					{
						double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
						QMatrix arrowTrans;
						FPointArray arrow = currDoc->arrowStyles.at(currItem->endArrowIndex()-1).points.copy();
						arrowTrans.translate(End.x(), End.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(currItem->lineWidth(), currItem->lineWidth());
						arrow.map(arrowTrans);
						result.addPath(arrow.toQPainterPath(true));
						break;
					}
				}
			}
			currDoc->m_Selection->clear();
			PageItem* newItem = currDoc->convertItemTo(currItem, PageItem::Polygon);
			newItem->setLineWidth(0);
			newItem->setLineStyle(Qt::SolidLine);
			newItem->setFillColor(newItem->lineColor());
			newItem->setFillShade(newItem->lineShade());
			newItem->setFillTransparency(newItem->lineTransparency());
			newItem->setFillBlendmode(newItem->lineBlendmode());
			FPointArray points;
			points.fromQPainterPath(result);
			newItem->PoLine = points;
			newItem->Frame = false;
			newItem->ClipEdited = true;
			newItem->FrameType = 3;
			currDoc->AdjustItemSize(newItem);
			newItem->OldB2 = newItem->width();
			newItem->OldH2 = newItem->height();
			newItem->updateClip();
			newItem->ContourLine = newItem->PoLine.copy();
			newItem->setFillEvenOdd(true);
			currDoc->m_Selection->addItem(newItem);
		}
		else
		{
			currDoc->m_Selection->clear();
			multiLine ml = currDoc->MLineStyles[currItem->NamedLStyle];
			bool first = true;
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QPainterPathStroker stroke;
					stroke.setCapStyle(static_cast<Qt::PenCapStyle>(ml[it].LineEnd));
					stroke.setJoinStyle(static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
					if (static_cast<Qt::PenStyle>(ml[it].Dash) == Qt::SolidLine)
						stroke.setDashPattern(static_cast<Qt::PenStyle>(ml[it].Dash));
					else
					{
						getDashArray(static_cast<Qt::PenStyle>(ml[it].Dash), 1, m_array);
						stroke.setDashPattern(m_array);
					}
					stroke.setWidth(ml[it].Width);
					QPainterPath result = stroke.createStroke(pp);
					PageItem* newItem;
					if (first)
					{
						newItem = currDoc->convertItemTo(currItem, PageItem::Polygon);
					}
					else
					{
						newItem = new PageItem_Polygon(*currItem);
						newItem->convertTo(PageItem::Polygon);
						currDoc->Items->append(newItem);
					}
					first = false;
					newItem->ItemNr = currDoc->Items->count()-1;
					newItem->setLineStyle(Qt::SolidLine);
					newItem->setFillColor(ml[it].Color);
					newItem->setFillShade(ml[it].Shade);
					newItem->setFillTransparency(newItem->lineTransparency());
					newItem->setFillBlendmode(newItem->lineBlendmode());
					newItem->setLineColor(CommonStrings::None);
					newItem->setCustomLineStyle("");
					FPointArray points;
					points.fromQPainterPath(result);
					newItem->PoLine = points;
					newItem->Frame = false;
					newItem->ClipEdited = true;
					newItem->FrameType = 3;
					currDoc->AdjustItemSize(newItem);
					newItem->OldB2 = newItem->width();
					newItem->OldH2 = newItem->height();
					newItem->updateClip();
					newItem->ContourLine = newItem->PoLine.copy();
					newItem->setFillEvenOdd(true);
					currDoc->m_Selection->addItem(newItem);
				}
			}
			if (currItem->startArrowIndex() != 0)
			{
				FPoint Start = currItem->PoLine.point(0);
				for (uint xx = 1; xx < currItem->PoLine.size(); xx += 2)
				{
					FPoint Vector = currItem->PoLine.point(xx);
					if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
					{
						double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
						QMatrix arrowTrans;
						FPointArray arrow = currDoc->arrowStyles.at(currItem->startArrowIndex()-1).points.copy();
						arrowTrans.translate(Start.x(), Start.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(currItem->lineWidth(), currItem->lineWidth());
						arrow.map(arrowTrans);
						PageItem* newItem = new PageItem_Polygon(*currItem);
						currDoc->Items->append(newItem);
						newItem->ItemNr = currDoc->Items->count()-1;
						newItem->setLineWidth(0);
						newItem->setLineStyle(Qt::SolidLine);
						newItem->setCustomLineStyle("");
						newItem->setFillColor(newItem->lineColor());
						newItem->setFillShade(newItem->lineShade());
						newItem->setFillTransparency(newItem->lineTransparency());
						newItem->setFillBlendmode(newItem->lineBlendmode());
						newItem->PoLine = arrow;
						newItem->Frame = false;
						newItem->ClipEdited = true;
						newItem->FrameType = 3;
						currDoc->AdjustItemSize(newItem);
						newItem->OldB2 = newItem->width();
						newItem->OldH2 = newItem->height();
						newItem->updateClip();
						newItem->ContourLine = newItem->PoLine.copy();
						newItem->setFillEvenOdd(true);
						currDoc->m_Selection->addItem(newItem);
						break;
					}
				}
			}
			if (currItem->endArrowIndex() != 0)
			{
				FPoint End = currItem->PoLine.point(currItem->PoLine.size()-2);
				for (uint xx = currItem->PoLine.size()-1; xx > 0; xx -= 2)
				{
					FPoint Vector = currItem->PoLine.point(xx);
					if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
					{
						double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
						QMatrix arrowTrans;
						FPointArray arrow = currDoc->arrowStyles.at(currItem->endArrowIndex()-1).points.copy();
						arrowTrans.translate(End.x(), End.y());
						arrowTrans.rotate(r);
						arrowTrans.scale(currItem->lineWidth(), currItem->lineWidth());
						arrow.map(arrowTrans);
						PageItem* newItem = new PageItem_Polygon(*currItem);
						currDoc->Items->append(newItem);
						newItem->ItemNr = currDoc->Items->count()-1;
						newItem->setLineWidth(0);
						newItem->setLineStyle(Qt::SolidLine);
						newItem->setCustomLineStyle("");
						newItem->setFillColor(newItem->lineColor());
						newItem->setFillShade(newItem->lineShade());
						newItem->setFillTransparency(newItem->lineTransparency());
						newItem->setFillBlendmode(newItem->lineBlendmode());
						newItem->PoLine = arrow;
						newItem->Frame = false;
						newItem->ClipEdited = true;
						newItem->FrameType = 3;
						currDoc->AdjustItemSize(newItem);
						newItem->OldB2 = newItem->width();
						newItem->OldH2 = newItem->height();
						newItem->updateClip();
						newItem->ContourLine = newItem->PoLine.copy();
						newItem->setFillEvenOdd(true);
						currDoc->m_Selection->addItem(newItem);
						break;
					}
				}
			}
			if (currDoc->m_Selection->count() > 1)
				currDoc->itemSelection_GroupObjects(false, false);
			currDoc->m_Selection->itemAt(0)->emitAllToGUI();
		}
		currDoc->changed();
	}
	return true;
}
