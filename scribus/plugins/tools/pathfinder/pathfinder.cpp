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

#include "pathfinder.h"
#include "pathfinderdialog.h"
#include "scribuscore.h"

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
	m_actionInfo.menu = "Item";
	m_actionInfo.enabledOnStartup = true;
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
	if (currDoc->m_Selection->count() > 1)
	{
		PageItem *Item1 = currDoc->m_Selection->itemAt(0);
		PageItem *Item2 = currDoc->m_Selection->itemAt(1);
		PathFinderDialog *dia = new PathFinderDialog(currDoc->scMW(), Item1, Item2);
		if (dia->exec())
		{
			PageItem *currItem;
			if (dia->swapped)
				currItem = Item2;
			else
				currItem = Item1;
			QPainterPath path = dia->result;
			FPointArray points;
			points.fromQPainterPath(path);
			currItem->PoLine = points;
			currItem->Frame = false;
			currItem->ClipEdited = true;
			currItem->FrameType = 3;
			currDoc->AdjustItemSize(currItem);
			currItem->ContourLine = currItem->PoLine.copy();
			currDoc->m_Selection->removeItem(currItem);
			currDoc->itemSelection_DeleteItem();
			currDoc->changed();
		}
		delete dia;
	}
	return true;
}
