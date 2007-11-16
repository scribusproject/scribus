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

#include "pathcut.h"
#include "scribuscore.h"
#include "scribusstructs.h"
#include "util.h"
#include <QPainterPathStroker>
#include <QMessageBox>

int pathcut_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* pathcut_getPlugin()
{
	PathCutPlugin* plug = new PathCutPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void pathcut_freePlugin(ScPlugin* plugin)
{
	PathCutPlugin* plug = dynamic_cast<PathCutPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

PathCutPlugin::PathCutPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

PathCutPlugin::~PathCutPlugin() {};

void PathCutPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "PathCutter";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Cut Polygon");
	// Menu
	m_actionInfo.menu = "ItemPathOps";
	m_actionInfo.parentMenu = "Item";
	m_actionInfo.subMenuName = tr("Path Tools");
	m_actionInfo.enabledOnStartup = true;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.notSuitableFor.append(PageItem::TextFrame);
	m_actionInfo.notSuitableFor.append(PageItem::ImageFrame);
	m_actionInfo.notSuitableFor.append(PageItem::PathText);
	m_actionInfo.notSuitableFor.append(PageItem::LatexFrame);
	m_actionInfo.forAppMode.append(modeNormal);
	m_actionInfo.needsNumObjects = 2;
	m_actionInfo.firstObjectType.append(PageItem::PolyLine);
	m_actionInfo.secondObjectType.append(PageItem::Polygon);
}

const QString PathCutPlugin::fullTrName() const
{
	return QObject::tr("PathCutter");
}

const ScActionPlugin::AboutData* PathCutPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Cuts a Polygon by a Polyline");
	about->description = tr("Cuts a Polygon by a Polyline");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void PathCutPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool PathCutPlugin::run(ScribusDoc* doc, QString)
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
		if (Item1->itemType() != PageItem::PolyLine)
		{
			Item1 = currDoc->m_Selection->itemAt(1);
			Item2 = currDoc->m_Selection->itemAt(0);
		}
		FPointArray path = Item1->PoLine;
		QPainterPathStroker stroke;
		stroke.setWidth(Item1->lineWidth());
		QPainterPath cutter = stroke.createStroke(path.toQPainterPath(false));
		QMatrix ms;
		ms.translate(Item1->xPos() - Item2->xPos(), Item1->yPos() - Item2->yPos());
		ms.rotate(Item1->rotation());
		cutter = ms.map(cutter);
		path.map(ms);
		FPoint start = path.point(0);
		FPoint end = path.point(path.size()-2);
		QMatrix mm;
		mm.rotate(Item2->rotation());
		QPainterPath objekt = mm.map(Item2->PoLine.toQPainterPath(true));
		if ((objekt.contains(QPointF(start.x(), start.y()))) || (objekt.contains(QPointF(end.x(), end.y()))))
		{
			QMessageBox::information(doc->scMW(), tr("Error"), tr("The cutting line must cross the polygon and\nboth end points must lie outside of the polygon"));
			return true;
		}
		QPainterPath result = objekt.subtracted(cutter);
		FPointArray points;
		points.fromQPainterPath(result);
		Item2->PoLine = points;
		Item2->Frame = false;
		Item2->ClipEdited = true;
		Item2->FrameType = 3;
		currDoc->AdjustItemSize(Item2);
		Item2->OldB2 = Item2->width();
		Item2->OldH2 = Item2->height();
		Item2->updateClip();
		Item2->ContourLine = Item2->PoLine.copy();
		currDoc->m_Selection->clear();
		currDoc->m_Selection->addItem(Item1);
		currDoc->itemSelection_DeleteItem();
		currDoc->m_Selection->clear();
		currDoc->m_Selection->addItem(Item2);
		currDoc->itemSelection_SplitItems();
		currDoc->changed();
	}
	return true;
}
