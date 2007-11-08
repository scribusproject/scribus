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

#include "lenseffects.h"
#include "lensdialog.h"
#include "scribuscore.h"

int lenseffects_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* lenseffects_getPlugin()
{
	LensEffectsPlugin* plug = new LensEffectsPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void lenseffects_freePlugin(ScPlugin* plugin)
{
	LensEffectsPlugin* plug = dynamic_cast<LensEffectsPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

LensEffectsPlugin::LensEffectsPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

LensEffectsPlugin::~LensEffectsPlugin() {};

void LensEffectsPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "LensEffects";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Lens Effects...");
	// Menu
	m_actionInfo.menu = "Item";
	m_actionInfo.enabledOnStartup = true;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.needsNumObjects = 3;
}

const QString LensEffectsPlugin::fullTrName() const
{
	return QObject::tr("Lens Effects");
}

const ScActionPlugin::AboutData* LensEffectsPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Lens Effects");
	about->description = tr("Apply fancy lens effects");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void LensEffectsPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool LensEffectsPlugin::run(ScribusDoc* doc, QString)
{
	ScribusDoc* currDoc = doc;
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc->m_Selection->count() > 0)
	{
		LensDialog *dia = new LensDialog(currDoc->scMW(), currDoc);
		if (dia->exec())
		{
			for (int a = 0; a < dia->origPathItem.count(); a++)
			{
				PageItem *currItem = currDoc->m_Selection->itemAt(a);
				if (currItem->itemType() == PageItem::Line)
					continue;
				QPainterPath path = dia->origPathItem[a]->path();
				FPointArray points;
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
			}
			currDoc->changed();
		}
		delete dia;
	}
	return true;
}
