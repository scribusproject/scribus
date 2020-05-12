/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2008 by Franz Schmid                                     *
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

#include "meshdistortiondialog.h"
#include "meshdistortion.h"
#include "scribuscore.h"
#include "scribusview.h"
#include "appmodes.h"
#include "util.h"
#include "util_math.h"

int meshdistortion_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* meshdistortion_getPlugin()
{
	MeshDistortionPlugin* plug = new MeshDistortionPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void meshdistortion_freePlugin(ScPlugin* plugin)
{
	MeshDistortionPlugin* plug = dynamic_cast<MeshDistortionPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

MeshDistortionPlugin::MeshDistortionPlugin()
{
	m_doc = nullptr;
	m_patternItem = nullptr;
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

MeshDistortionPlugin::~MeshDistortionPlugin() {};

void MeshDistortionPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "MeshDistortion";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Mesh Distortion...");
	// Menu
	m_actionInfo.menu = "ItemPathOps";
	m_actionInfo.parentMenu = "Item";
	m_actionInfo.subMenuName = tr("Path Tools");
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.forAppMode.append(modeNormal);
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.notSuitableFor.append(PageItem::TextFrame);
	m_actionInfo.notSuitableFor.append(PageItem::ImageFrame);
	m_actionInfo.notSuitableFor.append(PageItem::PathText);
	m_actionInfo.notSuitableFor.append(PageItem::LatexFrame);
	m_actionInfo.notSuitableFor.append(PageItem::Symbol);
	m_actionInfo.notSuitableFor.append(PageItem::RegularPolygon);
	m_actionInfo.notSuitableFor.append(PageItem::Arc);
	m_actionInfo.notSuitableFor.append(PageItem::Spiral);
	m_actionInfo.needsNumObjects = 3;
}

QString MeshDistortionPlugin::fullTrName() const
{
	return QObject::tr("MeshDistortion");
}

const ScActionPlugin::AboutData* MeshDistortionPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Mesh Distortion of Polygons");
	about->description = tr("Mesh Distortion of Polygons");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void MeshDistortionPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool MeshDistortionPlugin::run(ScribusDoc* doc, const QString&)
{
	m_doc = doc;
	if (m_doc == nullptr)
		m_doc = ScCore->primaryMainWindow()->doc;
	if (m_doc->m_Selection->count() > 0)
	{
		m_patternItem = m_doc->m_Selection->itemAt(0);
		MeshDistortionDialog *dia = new MeshDistortionDialog(m_doc->scMW(), m_doc);
		if (dia->exec())
		{
			dia->updateAndExit();
			if (m_patternItem->isGroup())
			{
				m_doc->resizeGroupToContents(m_patternItem);
				m_patternItem->SetRectFrame();
			}
			m_doc->changed();
			m_doc->view()->DrawNew();
		}
		delete dia;
	}
	return true;
}
