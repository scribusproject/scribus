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

#ifndef PATHALONGPATH_H
#define PATHALONGPATH_H

#include "pluginapi.h"
#include "scplugin.h"
#include "selection.h"

#include "../lib2geom/sbasis.h"
#include "../lib2geom/sbasis-geometric.h"
#include "../lib2geom/bezier-to-sbasis.h"
#include "../lib2geom/sbasis-to-bezier.h"
#include "../lib2geom/d2.h"
#include "../lib2geom/piecewise.h"
#include "../lib2geom/utils.h"
#include "../lib2geom/path.h"
#include "../lib2geom/sbasis-2d.h"
#include "../lib2geom/scribushelper.h"
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
		virtual ~PathAlongPathPlugin();
		virtual bool run(ScribusDoc* doc, QString target = QString::null);
		virtual const QString fullTrName() const;
		virtual const AboutData* getAboutData() const;
		virtual void deleteAboutData(const AboutData* about) const;
		virtual void languageChange();
		virtual void addToMainWindowMenu(ScribusMainWindow *) {};
		bool handleSelection(ScribusDoc* doc, int SelectedType);
	private:
		void setUpEffect(Geom::Piecewise<Geom::D2<Geom::SBasis> > &pwd2_in, Geom::Piecewise<Geom::D2<Geom::SBasis> > &pattern, int effect, double offset, double offsetY, double gap, int rotate);
		FPointArray doEffect_pwd2 (Geom::Piecewise<Geom::D2<Geom::SBasis> > & pattern);
		PageItem *patternItem;
		PageItem *pathItem;
		FPointArray originalPath;
		double originalRot;
		double originalXPos;
		double originalYPos;
		ScribusDoc* currDoc;
		bool firstUpdate;
		Piecewise<D2<SBasis> > uskeleton;
		Piecewise<D2<SBasis> > n;
		double m_scaling;
		int nbCopies;
		double pattWidth;
		double m_offsetX;
		double m_offsetY;
		double m_gapval;
		int m_rotate;
		Interval pattBnds;
		Interval pattBndsY;
		int selOffs;
		int selCount;
		QList<FPointArray> originalPathG;
		QList<double> originalRotG;
		QList<double> originalXPosG;
		QList<double> originalYPosG;
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
