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

#include "pathfinder.h"

#include "pageitem_polygon.h"
#include "pathfinderdialog.h"
#include "selection.h"
#include "scribuscore.h"
#include "scribusview.h"
#include "sccolorengine.h"
#include "ui/propertiespalette.h"
#include "undomanager.h"
#include "undostate.h"
#include "util_color.h"
#include "util_math.h"

#include <QMessageBox>
#include <QPixmap>
#include <QScopedPointer>

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
	PathFinderPlugin* plug = qobject_cast<PathFinderPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

PathFinderPlugin::PathFinderPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

PathFinderPlugin::~PathFinderPlugin() {}

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
//	m_actionInfo.iconPath1 = "pathintersection.png";
//	m_actionInfo.iconPath2 = "pathintersection.png";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.notSuitableFor.append(PageItem::TextFrame);
	m_actionInfo.notSuitableFor.append(PageItem::ImageFrame);
	m_actionInfo.notSuitableFor.append(PageItem::PolyLine);
	m_actionInfo.notSuitableFor.append(PageItem::PathText);
	m_actionInfo.notSuitableFor.append(PageItem::LatexFrame);
	m_actionInfo.notSuitableFor.append(PageItem::Symbol);
	m_actionInfo.notSuitableFor.append(PageItem::RegularPolygon);
	m_actionInfo.notSuitableFor.append(PageItem::Arc);
	m_actionInfo.notSuitableFor.append(PageItem::Spiral);
	m_actionInfo.needsNumObjects = 2;
}

QString PathFinderPlugin::fullTrName() const
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

bool PathFinderPlugin::run(ScribusDoc* doc, const QString&)
{
	ScribusDoc* currDoc = doc;
	if (currDoc == nullptr)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc->m_Selection->count() <= 1)
		return true;

	PageItem *Item1 = currDoc->m_Selection->itemAt(0);
	PageItem *Item2 = currDoc->m_Selection->itemAt(1);
	QScopedPointer<PathFinderDialog> dia(new PathFinderDialog(currDoc->scMW(), currDoc, Item1, Item2));
	if (dia->exec() != QDialog::Accepted)
		return true;
	
	//<<#9046
	UndoTransaction activeTransaction;
	UndoManager* undoManager = UndoManager::instance();
	if (UndoManager::undoEnabled())
		activeTransaction = undoManager->beginTransaction(Um::SelectionGroup, Um::IDocument, Um::PathOperation, "", Um::IPolygon);
	//>>#9046
	
	int opMode = dia->opMode;
	if (dia->keepItem1)
	{
		PageItem *newItem;
		if (dia->swapped)
		{
			newItem = new PageItem_Polygon(*Item2);
			newItem->setSelected(false);
			currDoc->Items->insert(currDoc->Items->indexOf(Item2), newItem);
		}
		else
		{
			newItem = new PageItem_Polygon(*Item1);
			newItem->setSelected(false);
			currDoc->Items->insert(currDoc->Items->indexOf(Item1), newItem);
		}
		if (UndoManager::undoEnabled())
		{
			int targetIndex = (newItem->OwnPage >= 0) ? newItem->OwnPage : 0;
			ScItemState<PageItem*> *is = new ScItemState<PageItem*>("Create PageItem");
			is->set("CREATE_ITEM");
			is->setItem(newItem);
			UndoObject *target = currDoc->Pages->at(targetIndex);
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
			currDoc->Items->insert(currDoc->Items->indexOf(Item1), newItem);
		}
		else
		{
			newItem = new PageItem_Polygon(*Item2);
			newItem->setSelected(false);
			currDoc->Items->insert(currDoc->Items->indexOf(Item2), newItem);
		}
		if (UndoManager::undoEnabled())
		{
			int targetIndex = (newItem->OwnPage >= 0) ? newItem->OwnPage : 0;
			ScItemState<PageItem*> *is = new ScItemState<PageItem*>("Create PageItem");
			is->set("CREATE_ITEM");
			is->setItem(newItem);
			UndoObject *target = currDoc->Pages->at(targetIndex);
			undoManager->action(target, is);
		}
	}
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
		FPointArray oldPOLine = currItem->PoLine;
		FPointArray oldContourLine = currItem->ContourLine;
		ScOldNewState< QPair<FPointArray, FPointArray> >* state = nullptr;
		if (UndoManager::undoEnabled())
		{
			state = new ScOldNewState< QPair<FPointArray, FPointArray> >(Um::PathOperation);
			state->set("PATH_OPERATION");
			state->set("PATH_OP_OLD_CLIPEDITED", currItem->ClipEdited);
			state->set("PATH_OP_OLD_FRAMETYPE", currItem->FrameType);
			state->set("PATH_OP_OLD_OLDB2", currItem->OldB2);
			state->set("PATH_OP_OLD_OLDH2", currItem->OldH2);
			state->set("PATH_OP_NEW_FRAME", false);
			state->set("PATH_OP_NEW_CLIPEDITED", true);
			state->set("PATH_OP_NEW_FRAMETYPE", 3);
		}
		//>>#9046
		
		int oldRotMode = currDoc->rotationMode();
		currItem->setRotation(0.0);
		currItem->PoLine = points;
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		currDoc->setRotationMode(0);
		currDoc->adjustItemSize(currItem);
		currDoc->setRotationMode(oldRotMode);
		currItem->OldB2 = currItem->width();
		currItem->OldH2 = currItem->height();
		currItem->updateClip();
		currItem->ContourLine = currItem->PoLine.copy();
			
		//<<#9046
		if (UndoManager::undoEnabled())
		{
			state->set("PATH_OP_NEW_OLDB2", currItem->OldB2);
			state->set("PATH_OP_NEW_OLDH2", currItem->OldH2);
			state->setStates(qMakePair(oldPOLine, oldContourLine), qMakePair(currItem->PoLine, currItem->ContourLine));
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
			FPointArray oldPOLine = Item1->PoLine;
			FPointArray oldContourLine = Item1->ContourLine;
			ScOldNewState< QPair<FPointArray, FPointArray> >* state = nullptr;
			if (UndoManager::undoEnabled())
			{
				state = new ScOldNewState< QPair<FPointArray, FPointArray> >(Um::PathOperation);
				state->set("PATH_OPERATION");
				state->set("PATH_OP_OLD_CLIPEDITED", Item1->ClipEdited);
				state->set("PATH_OP_OLD_FRAMETYPE", Item1->FrameType);
				state->set("PATH_OP_OLD_OLDB2", Item1->OldB2);
				state->set("PATH_OP_OLD_OLDH2", Item1->OldH2);
				state->set("PATH_OP_NEW_FRAME", false);
				state->set("PATH_OP_NEW_CLIPEDITED", true);
				state->set("PATH_OP_NEW_FRAMETYPE", 3);
			}
			//>>#9046

			int oldRotMode = currDoc->rotationMode();
			Item1->setRotation(0.0);
			Item1->PoLine = points;
			Item1->ClipEdited = true;
			Item1->FrameType = 3;
			currDoc->setRotationMode(0);
			currDoc->adjustItemSize(Item1);
			currDoc->setRotationMode(oldRotMode);
			Item1->OldB2 = Item1->width();
			Item1->OldH2 = Item1->height();
			Item1->updateClip();
			Item1->ContourLine = Item1->PoLine.copy();

			//<<#9046
			if (UndoManager::undoEnabled())
			{
				state->set("PATH_OP_NEW_OLDB2", Item1->OldB2);
				state->set("PATH_OP_NEW_OLDH2", Item1->OldH2);
				state->setStates(qMakePair(oldPOLine, oldContourLine), qMakePair(Item1->PoLine, Item1->ContourLine));
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
			FPointArray oldPOLine = Item2->PoLine;
			FPointArray oldContourLine = Item2->ContourLine;
			ScOldNewState< QPair<FPointArray, FPointArray> >* state = nullptr;
			if (UndoManager::undoEnabled())
			{
				state = new ScOldNewState< QPair<FPointArray, FPointArray> >(Um::PathOperation);
				state->set("PATH_OPERATION");
				state->set("PATH_OP_OLD_CLIPEDITED", Item2->ClipEdited);
				state->set("PATH_OP_OLD_FRAMETYPE", Item2->FrameType);
				state->set("PATH_OP_OLD_OLDB2", Item2->OldB2);
				state->set("PATH_OP_OLD_OLDH2", Item2->OldH2);
				state->set("PATH_OP_NEW_FRAME", false);
				state->set("PATH_OP_NEW_CLIPEDITED", true);
				state->set("PATH_OP_NEW_FRAMETYPE", 3);
			}
			//>>#9046

			int oldRotMode = currDoc->rotationMode();
			Item2->setXYPos(i1x, i1y);
			Item2->setRotation(0.0);
			Item2->PoLine = points;
			Item2->ClipEdited = true;
			Item2->FrameType = 3;
			currDoc->setRotationMode(0);
			currDoc->adjustItemSize(Item2);
			currDoc->setRotationMode(oldRotMode);
			Item2->OldB2 = Item2->width();
			Item2->OldH2 = Item2->height();
			Item2->updateClip();
			Item2->ContourLine = Item2->PoLine.copy();

			//<<#9046
			if (UndoManager::undoEnabled())
			{
				state->set("PATH_OP_NEW_OLDB2", Item2->OldB2);
				state->set("PATH_OP_NEW_OLDH2", Item2->OldH2);
				state->setStates(qMakePair(oldPOLine, oldContourLine), qMakePair(Item2->PoLine, Item2->ContourLine));
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
			newItem->setSelected(false);

			points.fromQPainterPath(path);

			int oldRotMode = currDoc->rotationMode();
			newItem->PoLine = points;
			newItem->ClipEdited = true;
			newItem->FrameType = 3;
			currDoc->setRotationMode(0);
			currDoc->adjustItemSize(newItem);
			currDoc->setRotationMode(oldRotMode);
			newItem->OldB2 = newItem->width();
			newItem->OldH2 = newItem->height();
			newItem->updateClip();
			newItem->ContourLine = newItem->PoLine.copy();

			if (UndoManager::undoEnabled())
			{
				int targetIndex = (newItem->OwnPage >= 0) ? newItem->OwnPage : 0;
				ScItemState<PageItem*> *is = new ScItemState<PageItem*>("Create PageItem");
				is->set("CREATE_ITEM");
				is->setItem(newItem);
				UndoObject *target = currDoc->Pages->at(targetIndex);
				undoManager->action(target, is);
			}

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
		currDoc->view()->deselectItems(true);
	}

	currDoc->changed();
	currDoc->view()->DrawNew();
	
	//<<#9046
	if (activeTransaction)
		activeTransaction.commit();
	//>>#9046

	return true;
}
