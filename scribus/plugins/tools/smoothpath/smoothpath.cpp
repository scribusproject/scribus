/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2011 by Franz Schmid                                     *
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

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "smoothpath.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "selection.h"
#include "util.h"
#include <QPainterPath>
#include <QPolygonF>
#include <QList>
#include "KarbonCurveFit.h"

int smoothpath_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* smoothpath_getPlugin()
{
	SmoothPathPlugin* plug = new SmoothPathPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void smoothpath_freePlugin(ScPlugin* plugin)
{
	SmoothPathPlugin* plug = dynamic_cast<SmoothPathPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

SmoothPathPlugin::SmoothPathPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

SmoothPathPlugin::~SmoothPathPlugin() {};

void SmoothPathPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "SmoothPath";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Smooth Path");
	// Menu
	m_actionInfo.menu = "ItemPathOps";
	m_actionInfo.parentMenu = "Item";
	m_actionInfo.subMenuName = tr("Path Tools");
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.notSuitableFor.append(PageItem::TextFrame);
	m_actionInfo.notSuitableFor.append(PageItem::ImageFrame);
	m_actionInfo.notSuitableFor.append(PageItem::Polygon);
	m_actionInfo.notSuitableFor.append(PageItem::PathText);
	m_actionInfo.notSuitableFor.append(PageItem::LatexFrame);
	m_actionInfo.notSuitableFor.append(PageItem::Symbol);
	m_actionInfo.notSuitableFor.append(PageItem::RegularPolygon);
	m_actionInfo.notSuitableFor.append(PageItem::Arc);
	m_actionInfo.notSuitableFor.append(PageItem::Spiral);
	m_actionInfo.forAppMode.append(modeNormal);
	m_actionInfo.needsNumObjects = 1;
}

const QString SmoothPathPlugin::fullTrName() const
{
	return QObject::tr("SmoothPath");
}

const ScActionPlugin::AboutData* SmoothPathPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Smoothes a Path");
	about->description = tr("Converts a Polyline made of straight Lines to a smoothed Bezier Curve.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void SmoothPathPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool SmoothPathPlugin::run(ScribusDoc* doc, QString)
{
	ScribusDoc* currDoc = doc;
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc->m_Selection->count() > 0)
	{
		PageItem *currItem = currDoc->m_Selection->itemAt(0);
		QPainterPath pp;
		if (currItem->itemType() == PageItem::PolyLine)
			pp = currItem->PoLine.toQPainterPath(false);
		else
			pp = currItem->PoLine.toQPainterPath(true);
		QList<QPolygonF> polyList = pp.toSubpathPolygons();
		QPainterPath result;
		for (int a = 0; a < polyList.count(); a++)
		{
			result.addPath(bezierFit(polyList[a], 5.0));
		}
		currItem->PoLine.fromQPainterPath(result);
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		currDoc->AdjustItemSize(currItem);
		currItem->OldB2 = currItem->width();
		currItem->OldH2 = currItem->height();
		currItem->updateClip();
		currDoc->regionsChanged()->update(QRectF());
		currDoc->changed();
	}
	return true;
}
