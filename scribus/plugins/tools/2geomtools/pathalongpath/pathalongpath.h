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

#ifndef PATHALONGPATH_H
#define PATHALONGPATH_H

#include "pluginapi.h"
#include "scplugin.h"
#include "selection.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include "third_party/lib2geom/d2.h"
#include "third_party/lib2geom/piecewise.h"
#include "third_party/lib2geom/sbasis.h"
#include "third_party/lib2geom/sbasis-geometric.h"
#include "third_party/lib2geom/bezier-to-sbasis.h"
#include "third_party/lib2geom/sbasis-to-bezier.h"
#include "third_party/lib2geom/utils.h"
#include "third_party/lib2geom/path.h"
#include "third_party/lib2geom/sbasis-2d.h"
#include "third_party/lib2geom/scribushelper.h"
#include <vector>
using namespace Geom;

/** \brief This is a simple "PathAlongPath" plugin for Scribus 1.3 and later.
\author Franz Schmid
\date November 2007
*/
class PLUGIN_API PathAlongPathPlugin : public ScActionPlugin
{
	Q_OBJECT

	public:
		// Standard plugin implementation
		PathAlongPathPlugin();
		~PathAlongPathPlugin() override;

		bool run(ScribusDoc* doc, const QString& target = QString()) override;
		QString fullTrName() const override;
		const AboutData* getAboutData() const override;
		void deleteAboutData(const AboutData* about) const override;
		void languageChange() override;
		void addToMainWindowMenu(ScribusMainWindow *) override {};
		bool handleSelection(ScribusDoc* doc, int SelectedType) override;

	private:
		void setUpEffect(const Geom::Piecewise<Geom::D2<Geom::SBasis> > &pwd2_in, const Geom::Piecewise<Geom::D2<Geom::SBasis> > &pattern, int effect, double offset, double offsetY, double gap, int rotate);
		FPointArray doEffect_pwd2 (const Geom::Piecewise<Geom::D2<Geom::SBasis> > & pattern) const;

		PageItem *patternItem { nullptr };
		PageItem *pathItem { nullptr };
		FPointArray originalPath;
		FPointArray effectPath;
		double originalRot { 0.0 };
		double originalXPos { 0.0 };
		double originalYPos { 0.0 };
		ScribusDoc* m_doc { nullptr };
		bool firstUpdate { false };
		Geom::Piecewise<Geom::D2<Geom::SBasis> > uskeleton;
		Geom::Piecewise<Geom::D2<Geom::SBasis> > n;
		double m_scaling { 0.0 };
		int nbCopies { 0 };
		double pattWidth { 0.0 };
		double m_offsetX { 0.0 };
		double m_offsetY { 0.0 };
		double m_gapval { 0.0 };
		int m_rotate { 0 };
		Geom::Interval pattBnds;
		Geom::Interval pattBndsY;
		int selOffs { 0 };
		int selCount { 0 };
		QList<FPointArray> originalPathG;
		QList<double> originalRotG;
		QList<double> originalXPosG;
		QList<double> originalYPosG;
		QList<double> originalXPosGi;
		QList<double> originalYPosGi;
		QList<double> originalWidthG;
		QList<double> originalHeightG;
		QList<double> originalWidth;
		QList<double> originalHeight;
		QList<PageItem *> patternItemG;

	private slots:
		void updateEffectG(int effectType, double offset, double offsetY, double gap, int rotate);
		void updateEffect(int effectType, double offset, double offsetY, double gap, int rotate);

		// Special features (none)
};

extern "C" PLUGIN_API int pathalongpath_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* pathalongpath_getPlugin();
extern "C" PLUGIN_API void pathalongpath_freePlugin(ScPlugin* plugin);

#endif
