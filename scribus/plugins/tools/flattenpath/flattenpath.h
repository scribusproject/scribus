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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.              *
****************************************************************************/

#ifndef FLATTENPATH_H
#define FLATTENPATH_H

#include "pluginapi.h"
#include "scplugin.h"

class PLUGIN_API FlattenPathPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		FlattenPathPlugin();
		virtual ~FlattenPathPlugin();
		bool run(ScribusDoc* doc, const QString& target = QString()) override;
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		void addToMainWindowMenu(ScribusMainWindow *) override {};

		// Special features (none)
};

extern "C" PLUGIN_API int flattenpath_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* flattenpath_getPlugin();
extern "C" PLUGIN_API void flattenpath_freePlugin(ScPlugin* plugin);

#endif
