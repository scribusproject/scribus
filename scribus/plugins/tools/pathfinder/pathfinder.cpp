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

#include <QMessageBox>
#include <QPixmap>
#include "pageitem_polygon.h"
#include "pathfinder.h"
#include "pathfinderdialog.h"
#include "propertiespalette.h"
#include "scribuscore.h"
#include "sccolorengine.h"
#include "util_color.h"
#include "util_math.h"
#include "util_icon.h"

int pathfinder_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* pathfinder_getPlugin()
{
	PathFinderPlugin* plug = new PathFinderPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void pathfinder_freePlugin(ScPlugin* plugin)
{
	PathFinderPlugin* plug = dynamic_cast<PathFinderPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

PathFinderPlugin::PathFinderPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

PathFinderPlugin::~PathFinderPlugin() {};

void PathFinderPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "PathFinder";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Path Operations...");
	// Menu
	m_actionInfo.menu = "ItemPathOps";
	m_actionInfo.parentMenu = "Item";
	m_actionInfo.subMenuName = tr("Path Tools");
//	m_actionInfo.toolbar = "Vector";
//	m_actionInfo.toolBarName = tr("Vector-Tools");
//	m_actionInfo.icon1 = loadIcon("pathintersection.png");
//	m_actionInfo.icon2 = loadIcon("pathintersection.png");
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.notSuitableFor.append(PageItem::TextFrame);
	m_actionInfo.notSuitableFor.append(PageItem::ImageFrame);
	m_actionInfo.notSuitableFor.append(PageItem::PolyLine);
	m_actionInfo.notSuitableFor.append(PageItem::PathText);
	m_actionInfo.notSuitableFor.append(PageItem::LatexFrame);
	m_actionInfo.needsNumObjects = 2;
}

const QString PathFinderPlugin::fullTrName() const
{
	return QObject::tr("PathFinder");
}

const ScActionPlugin::AboutData* PathFinderPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Path Operations");
	about->description = tr("Apply fancy boolean operations to paths.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void PathFinderPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool PathFinderPlugin::run(ScribusDoc* doc, QString)
{
	QString vers = QString(qVersion()).left(5);
	if (vers < "4.3.3")
	{
		QMessageBox::information(doc->scMW(), tr("Qt Version too old"), tr("This plugin requires at least version 4.3.3 of the Qt library"));
		return true;
	}
	ScribusDoc* currDoc = doc;
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc->m_Selection->count() > 1)
	{
		PageItem *Item1 = currDoc->m_Selection->itemAt(0);
		PageItem *Item2 = currDoc->m_Selection->itemAt(1);
		PathFinderDialog *dia = new PathFinderDialog(currDoc->scMW(), currDoc, Item1, Item2);
		if (dia->exec())
		{
			if (dia->keepItem1)
			{
				if (dia->swapped)
					currDoc->Items->insert(Item2->ItemNr, new PageItem_Polygon(*Item2));
				else
					currDoc->Items->insert(Item1->ItemNr, new PageItem_Polygon(*Item1));
			}
			if (dia->keepItem2)
			{
				if (dia->swapped)
					currDoc->Items->insert(Item1->ItemNr, new PageItem_Polygon(*Item1));
				else
					currDoc->Items->insert(Item2->ItemNr, new PageItem_Polygon(*Item2));
			}
			if (dia->keepItem1 || dia->keepItem2)
				currDoc->renumberItemsInListOrder();
			if (dia->opMode != 4)
			{
				PageItem *currItem;
				QPainterPath path;
				FPointArray points;
				if (dia->targetColor == 0)
				{
					currItem = Item1;
					if (dia->swapped)
					{
						currItem = Item2;
						currItem->setXYPos(Item1->xPos(), Item1->yPos());
						currItem->setRotation(0.0);
					}
				}
				else
				{
					if (dia->swapped)
						currItem = Item1;
					else
					{
						currItem = Item2;
						currItem->setXYPos(Item1->xPos(), Item1->yPos());
						currItem->setRotation(0.0);
					}
				}
				path = dia->result;
				points.fromQPainterPath(path);
				currItem->PoLine = points;
				currItem->Frame = false;
				currItem->ClipEdited = true;
				currItem->FrameType = 3;
				currDoc->AdjustItemSize(currItem);
				currItem->OldB2 = currItem->width();
				currItem->OldH2 = currItem->height();
				currItem->updateClip();
				currItem->ContourLine = currItem->PoLine.copy();
				currDoc->m_Selection->removeItem(currItem);
				currDoc->itemSelection_DeleteItem();
			}
			else
			{
				QPainterPath path;
				FPointArray points;
				PageItem *newItem;
				path = dia->result;
				if (!path.isEmpty())
				{
					points.fromQPainterPath(path);
					Item1->PoLine = points;
					Item1->Frame = false;
					Item1->ClipEdited = true;
					Item1->FrameType = 3;
					currDoc->AdjustItemSize(Item1);
					Item1->OldB2 = Item1->width();
					Item1->OldH2 = Item1->height();
					Item1->updateClip();
					Item1->ContourLine = Item1->PoLine.copy();
				}

				path = QPainterPath();
				path = dia->result1;
				if (!path.isEmpty())
				{
					points.fromQPainterPath(path);
					Item2->setXYPos(Item1->xPos(), Item1->yPos());
					Item2->setRotation(0.0);
					Item2->PoLine = points;
					Item2->Frame = false;
					Item2->ClipEdited = true;
					Item2->FrameType = 3;
					currDoc->AdjustItemSize(Item2);
					Item2->OldB2 = Item2->width();
					Item2->OldH2 = Item2->height();
					Item2->updateClip();
					Item2->ContourLine = Item2->PoLine.copy();
				}
				
				path = QPainterPath();
				path = dia->result2;
				if (!path.isEmpty())
				{
					if (dia->targetColor == 0)
						newItem = new PageItem_Polygon(*Item1);
					else
					{
						newItem = new PageItem_Polygon(*Item2);
						newItem->setXYPos(Item1->xPos(), Item1->yPos());
						newItem->setRotation(0.0);
					}
					currDoc->Items->append(newItem);
					newItem->ItemNr = currDoc->Items->count()-1;
					points.fromQPainterPath(path);
					newItem->PoLine = points;
					newItem->Frame = false;
					newItem->ClipEdited = true;
					newItem->FrameType = 3;
					currDoc->AdjustItemSize(newItem);
					newItem->OldB2 = newItem->width();
					newItem->OldH2 = newItem->height();
					newItem->updateClip();
					newItem->ContourLine = newItem->PoLine.copy();
					if (dia->targetColor == 2)
					{
						QString fill = dia->getOtherFillColor();
						if (fill == CommonStrings::tr_NoneColor)
							fill = CommonStrings::None;
						newItem->setFillColor(fill);
						QString stroke = dia->getOtherLineColor();
						if (stroke == CommonStrings::tr_NoneColor)
							stroke = CommonStrings::None;
						newItem->setLineColor(stroke);
					}
				}
				currDoc->m_Selection->clear();
				currDoc->view()->Deselect(true);
			}
			currDoc->changed();
			currDoc->view()->DrawNew();
		}
		delete dia;
	}
	return true;
}
