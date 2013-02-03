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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.              *
****************************************************************************/

#include <QMessageBox>
#include <QPixmap>
#include "pageitem_polygon.h"
#include "pathfinder.h"
#include "pathfinderdialog.h"
#include "propertiespalette.h"
#include "scribuscore.h"
#include "sccolorengine.h"
#include "undomanager.h"
#include "undostate.h"
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
	ScribusDoc* currDoc = doc;
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc->m_Selection->count() <= 1)
		return true;

	//<<#9046
	UndoTransaction* activeTransaction = NULL;
	UndoManager* undoManager = UndoManager::instance();
	if (UndoManager::undoEnabled())
		activeTransaction = new UndoTransaction(undoManager->beginTransaction(Um::SelectionGroup, Um::IDocument, Um::PathOperation, "", Um::IPolygon));
	//>>#9046

	PageItem *Item1 = currDoc->m_Selection->itemAt(0);
	PageItem *Item2 = currDoc->m_Selection->itemAt(1);
	PathFinderDialog *dia = new PathFinderDialog(currDoc->scMW(), currDoc, Item1, Item2);
	if (dia->exec())
	{
		int opMode=dia->opMode;
		if (dia->keepItem1)
		{
			PageItem *newItem;
			if (dia->swapped)
			{
				newItem = new PageItem_Polygon(*Item2);
				newItem->setSelected(false);
				currDoc->Items->insert(Item2->ItemNr, newItem);
			}
			else
			{
				newItem = new PageItem_Polygon(*Item1);
				newItem->setSelected(false);
				currDoc->Items->insert(Item1->ItemNr, newItem);
			}
			if (UndoManager::undoEnabled())
			{
				ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
				is->set("CREATE_ITEM", "create_item");
				is->setItem(newItem);
				UndoObject *target = currDoc->Pages->at(Item1->OwnPage);
				undoManager->action(target, is);
			}
		}
		if (dia->keepItem2)
		{
			PageItem *newItem;
			if (dia->swapped)
			{
				newItem = new PageItem_Polygon(*Item1);
				newItem->setSelected(false);
				currDoc->Items->insert(Item1->ItemNr, newItem);
			}
			else
			{
				newItem = new PageItem_Polygon(*Item2);
				newItem->setSelected(false);
				currDoc->Items->insert(Item2->ItemNr, newItem);
			}
			if (UndoManager::undoEnabled())
			{
				ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
				is->set("CREATE_ITEM", "create_item");
				is->setItem(newItem);
				UndoObject *target = currDoc->Pages->at(Item1->OwnPage);
				undoManager->action(target, is);
			}
		}
		if (dia->keepItem1 || dia->keepItem2)
			currDoc->renumberItemsInListOrder();
		if (opMode != 4)
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

			//<<#9046
			FPointArray oldPOLine=currItem->PoLine;
			FPointArray oldContourLine=currItem->ContourLine;
			ItemState<QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> > >* state = NULL;
			if (UndoManager::undoEnabled())
			{
				state = new ItemState<QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> > >(Um::PathOperation);
				state->set("PATH_OPERATION", "path_operation");
				state->set("PATH_OP_OLD_FRAME", currItem->Frame);
				state->set("PATH_OP_OLD_CLIPEDITED", currItem->ClipEdited);
				state->set("PATH_OP_OLD_FRAMETYPE", currItem->FrameType);
				state->set("PATH_OP_OLD_OLDB2", currItem->OldB2);
				state->set("PATH_OP_OLD_OLDH2", currItem->OldH2);
				state->set("PATH_OP_NEW_FRAME", false);
				state->set("PATH_OP_NEW_CLIPEDITED", true);
				state->set("PATH_OP_NEW_FRAMETYPE", 3);
			}
			//>>#9046

			currItem->PoLine = points;
			currItem->Frame = false;
			currItem->ClipEdited = true;
			currItem->FrameType = 3;
			currDoc->AdjustItemSize(currItem);
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			currItem->updateClip();
			currItem->ContourLine = currItem->PoLine.copy();

			//<<#9046
			if (UndoManager::undoEnabled())
			{
				state->set("PATH_OP_NEW_OLDB2", currItem->OldB2);
				state->set("PATH_OP_NEW_OLDH2", currItem->OldH2);
				state->setItem(QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> >(QPair<FPointArray, FPointArray>(oldPOLine, oldContourLine), QPair<FPointArray, FPointArray>(points, currItem->ContourLine)));
				undoManager->action(currItem, state);
			}
			//>>#9046

			currDoc->m_Selection->removeItem(currItem);
			currDoc->itemSelection_DeleteItem();

		}
		else
		{
			QPainterPath path;
			FPointArray points;
			PageItem *newItem;
			double i1x = Item1->xPos();
			double i1y = Item1->yPos();
			path = dia->result;
			if (!path.isEmpty())
			{
				points.fromQPainterPath(path);
				//<<#9046
				FPointArray oldPOLine=Item1->PoLine;
				FPointArray oldContourLine=Item1->ContourLine;
				ItemState<QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> > >* state = NULL;
				if (UndoManager::undoEnabled())
				{
					state = new ItemState<QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> > >(Um::PathOperation);
					state->set("PATH_OPERATION", "path_operation");
					state->set("PATH_OP_OLD_FRAME", Item1->Frame);
					state->set("PATH_OP_OLD_CLIPEDITED", Item1->ClipEdited);
					state->set("PATH_OP_OLD_FRAMETYPE", Item1->FrameType);
					state->set("PATH_OP_OLD_OLDB2", Item1->OldB2);
					state->set("PATH_OP_OLD_OLDH2", Item1->OldH2);
					state->set("PATH_OP_NEW_FRAME", false);
					state->set("PATH_OP_NEW_CLIPEDITED", true);
					state->set("PATH_OP_NEW_FRAMETYPE", 3);
				}
				//>>#9046
				Item1->PoLine = points;
				Item1->Frame = false;
				Item1->ClipEdited = true;
				Item1->FrameType = 3;
				currDoc->AdjustItemSize(Item1);
				Item1->OldB2 = Item1->width();
				Item1->OldH2 = Item1->height();
				Item1->updateClip();
				Item1->ContourLine = Item1->PoLine.copy();
				//<<#9046
				if (UndoManager::undoEnabled())
				{
					state->set("PATH_OP_NEW_OLDB2", Item1->OldB2);
					state->set("PATH_OP_NEW_OLDH2", Item1->OldH2);
					state->setItem(QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> >(QPair<FPointArray, FPointArray>(oldPOLine, oldContourLine), QPair<FPointArray, FPointArray>(Item1->PoLine, Item1->ContourLine)));
					undoManager->action(Item1, state);
				}
				//>>#9046
			}

			path = QPainterPath();
			path = dia->result1;
			if (!path.isEmpty())
			{
				points.fromQPainterPath(path);
				//<<#9046
				FPointArray oldPOLine=Item2->PoLine;
				FPointArray oldContourLine=Item2->ContourLine;
				ItemState<QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> > >* state = NULL;
				if (UndoManager::undoEnabled())
				{
					state = new ItemState<QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> > >(Um::PathOperation);
					state->set("PATH_OPERATION", "path_operation");
					state->set("PATH_OP_OLD_FRAME", Item2->Frame);
					state->set("PATH_OP_OLD_CLIPEDITED", Item2->ClipEdited);
					state->set("PATH_OP_OLD_FRAMETYPE", Item2->FrameType);
					state->set("PATH_OP_OLD_OLDB2", Item2->OldB2);
					state->set("PATH_OP_OLD_OLDH2", Item2->OldH2);
					state->set("PATH_OP_NEW_FRAME", false);
					state->set("PATH_OP_NEW_CLIPEDITED", true);
					state->set("PATH_OP_NEW_FRAMETYPE", 3);
				}
				//>>#9046
				Item2->setXYPos(i1x, i1y);
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
				//<<#9046
				if (UndoManager::undoEnabled())
				{
					state->set("PATH_OP_NEW_OLDB2", Item2->OldB2);
					state->set("PATH_OP_NEW_OLDH2", Item2->OldH2);
					state->setItem(QPair<QPair<FPointArray, FPointArray>, QPair<FPointArray, FPointArray> >(QPair<FPointArray, FPointArray>(oldPOLine, oldContourLine), QPair<FPointArray, FPointArray>(Item2->PoLine, Item2->ContourLine)));
					undoManager->action(Item2, state);
				}
				//>>#9046
			}

			path = QPainterPath();
			path = dia->result2;
			if (!path.isEmpty())
			{
				if (dia->targetColor == 0)
				{
					newItem = new PageItem_Polygon(*Item1);
					newItem->setXYPos(i1x, i1y);
				}
				else
				{
					newItem = new PageItem_Polygon(*Item2);
					newItem->setXYPos(i1x, i1y);
					newItem->setRotation(0.0);
				}
				currDoc->Items->append(newItem);
				newItem->ItemNr = currDoc->Items->count()-1;
				newItem->setSelected(false);
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
				if (UndoManager::undoEnabled())
				{
					ItemState<PageItem*> *is = new ItemState<PageItem*>("Create PageItem");
					is->set("CREATE_ITEM", "create_item");
					is->setItem(newItem);
					UndoObject *target = currDoc->Pages->at(Item1->OwnPage);
					undoManager->action(target, is);
				}
			}
			currDoc->m_Selection->clear();
			currDoc->view()->Deselect(true);
		}
		currDoc->changed();
		currDoc->view()->DrawNew();
	}
	delete dia;

	//<<#9046
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	//>>#9046
	return true;
}
