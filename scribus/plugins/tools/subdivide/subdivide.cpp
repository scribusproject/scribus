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

#include "subdivide.h"
#include "scribuscore.h"
#include "scribusstructs.h"

int subdivide_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* subdivide_getPlugin()
{
	SubdividePlugin* plug = new SubdividePlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void subdivide_freePlugin(ScPlugin* plugin)
{
	SubdividePlugin* plug = dynamic_cast<SubdividePlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

SubdividePlugin::SubdividePlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

SubdividePlugin::~SubdividePlugin() {};

void SubdividePlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "Subdivide";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Subdivide Path");
	// Menu
	m_actionInfo.menu = "ItemPathOps";
	m_actionInfo.parentMenu = "Item";
	m_actionInfo.subMenuName = tr("Path Tools");
	m_actionInfo.enabledOnStartup = true;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.forAppMode.append(modeEditClip);
	m_actionInfo.needsNumObjects = 1;
}

const QString SubdividePlugin::fullTrName() const
{
	return QObject::tr("Subdivide");
}

const ScActionPlugin::AboutData* SubdividePlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Subdivide");
	about->description = tr("Subdivide selected Path");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void SubdividePlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool SubdividePlugin::run(ScribusDoc* doc, QString)
{
	ScribusDoc* currDoc = doc;
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	double nearT = 0.5;
	uint docSelectionCount = currDoc->m_Selection->count();
	if (docSelectionCount != 0)
	{
		for (uint aa = 0; aa < docSelectionCount; ++aa)
		{
			FPointArray points;
			PageItem *currItem = currDoc->m_Selection->itemAt(aa);
			uint psize = currItem->PoLine.size();
			for (uint a = 0; a < psize-3; a += 4)
			{
				if (currItem->PoLine.point(a).x() > 900000)
				{
					points.setMarker();
					continue;
				}
				FPoint base = currItem->PoLine.point(a);
				FPoint c1 = currItem->PoLine.point(a+1);
				FPoint base2 =  currItem->PoLine.point(a+2);
				FPoint c2 = currItem->PoLine.point(a+3);
				FPoint cn1 = (1.0 - nearT) * base + nearT * c1;
				FPoint cn2 = (1.0 - nearT) * cn1 + nearT * ((1.0 - nearT) * c1 + nearT * c2);
				FPoint cn3 = (1.0 - nearT) * ((1.0 - nearT) * c1 + nearT * c2) + nearT * ((1.0 - nearT) * c2 + nearT * base2);
				FPoint cn4 = (1.0 - nearT) * c2 + nearT * base2;
				FPoint bp1 = (1.0 - nearT) * cn2 + nearT * cn3;
				points.addPoint(base);
				points.addPoint(cn1);
				points.addPoint(bp1);
				points.addPoint(cn2);
				points.addPoint(bp1);
				points.addPoint(cn3);
				points.addPoint(base2);
				points.addPoint(cn4);
			}
			currItem->PoLine = points;
			currItem->Frame = false;
			currItem->ClipEdited = true;
			currItem->FrameType = 3;
			currDoc->AdjustItemSize(currItem);
			currItem->OldB2 = currItem->width();
			currItem->OldH2 = currItem->height();
			currItem->updateClip();
		}
		currDoc->changed();
	}
	return true;
}
