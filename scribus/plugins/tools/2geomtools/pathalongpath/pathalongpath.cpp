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

#include "pathdialog.h"
#include "pathalongpath.h"
#include "scribuscore.h"
#include "scribusstructs.h"
#include "util.h"
#include "util_math.h"

int pathalongpath_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* pathalongpath_getPlugin()
{
	PathAlongPathPlugin* plug = new PathAlongPathPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void pathalongpath_freePlugin(ScPlugin* plugin)
{
	PathAlongPathPlugin* plug = dynamic_cast<PathAlongPathPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

PathAlongPathPlugin::PathAlongPathPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

PathAlongPathPlugin::~PathAlongPathPlugin() {};

void PathAlongPathPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "PathAlongPath";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Path Along Path...");
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

const QString PathAlongPathPlugin::fullTrName() const
{
	return QObject::tr("PathAlongPath");
}

const ScActionPlugin::AboutData* PathAlongPathPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Bends a Polygon along a Polyline");
	about->description = tr("This plugin bends a Polygon with the help of a Polyline.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void PathAlongPathPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool PathAlongPathPlugin::run(ScribusDoc* doc, QString)
{
	firstUpdate = true;
	currDoc = doc;
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc->m_Selection->count() > 1)
	{
		patternItem = currDoc->m_Selection->itemAt(0);
		pathItem = currDoc->m_Selection->itemAt(1);
		if (pathItem->itemType() != PageItem::PolyLine)
		{
			patternItem = currDoc->m_Selection->itemAt(1);
			pathItem = currDoc->m_Selection->itemAt(0);
		}
		originalPath = patternItem->PoLine.copy();
		originalXPos = patternItem->xPos();
		originalYPos = patternItem->yPos();
		PathDialog *dia = new PathDialog(currDoc->scMW(), currDoc->unitIndex());
		connect(dia, SIGNAL(updateValues(int, double, double, double, bool)), this, SLOT(updateEffect(int, double, double, double, bool)));
		if (dia->exec())
		{
			updateEffect(dia->effectType, dia->offset, dia->offsetY, dia->gap, dia->rotate);
			patternItem->ContourLine = patternItem->PoLine.copy();
			currDoc->changed();
		}
		else
		{
			patternItem->PoLine = originalPath;
			patternItem->Frame = false;
			patternItem->ClipEdited = true;
			patternItem->FrameType = 3;
			patternItem->setXYPos(originalXPos, originalYPos);
			currDoc->AdjustItemSize(patternItem);
			patternItem->OldB2 = patternItem->width();
			patternItem->OldH2 = patternItem->height();
			patternItem->updateClip();
			currDoc->view()->DrawNew();
		}
		delete dia;
	}
	return true;
}

void PathAlongPathPlugin::updateEffect(int effectType, double offset, double offsetY, double gap, bool rotate)
{
	if (effectType == -1)
	{
		patternItem->PoLine = originalPath;
		patternItem->Frame = false;
		patternItem->ClipEdited = true;
		patternItem->FrameType = 3;
		patternItem->setXYPos(originalXPos, originalYPos);
		firstUpdate = true;
	}
	else
	{
		Geom::Piecewise<Geom::D2<Geom::SBasis> > originaldpwd2 = FPointArray2Piecewise(pathItem->PoLine, false);
		Geom::Piecewise<Geom::D2<Geom::SBasis> > patternpwd2;
		if (patternItem->itemType() == PageItem::PolyLine)
			patternpwd2 = FPointArray2Piecewise(originalPath, false);
		else
			patternpwd2 = FPointArray2Piecewise(originalPath, true);
		patternItem->PoLine = doEffect_pwd2(originaldpwd2, patternpwd2, effectType, offset / currDoc->unitRatio(), offsetY / currDoc->unitRatio(), gap / currDoc->unitRatio(), rotate);
		patternItem->Frame = false;
		patternItem->ClipEdited = true;
		patternItem->FrameType = 3;
		patternItem->setXYPos(pathItem->xPos(), pathItem->yPos());
	}
	currDoc->AdjustItemSize(patternItem);
	patternItem->OldB2 = patternItem->width();
	patternItem->OldH2 = patternItem->height();
	patternItem->updateClip();
	if (firstUpdate)
		currDoc->view()->DrawNew();
	else
	{
		QRectF oldR(pathItem->getBoundingRect());
		QRectF newR(patternItem->getBoundingRect());
		currDoc->regionsChanged()->update(newR.unite(oldR));
	}
	if (effectType != -1)
		firstUpdate = false;
}

FPointArray
PathAlongPathPlugin::doEffect_pwd2(Geom::Piecewise<Geom::D2<Geom::SBasis> > &pwd2_in, Geom::Piecewise<Geom::D2<Geom::SBasis> > &pattern, int effect, double offset, double offsetY, double gap, bool rotate)
{
	double offs = offset;
	double gapval = gap;
	Piecewise<D2<SBasis> > uskeleton = arc_length_parametrization(pwd2_in, 2, .1);
	uskeleton = remove_short_cuts(uskeleton,.01);
	Piecewise<D2<SBasis> > n = rot90(derivative(uskeleton));
	n = force_continuity(remove_short_cuts(n,.1));
	D2<Piecewise<SBasis> > patternd2 = make_cuts_independant(pattern);
	Piecewise<SBasis> x;
	Piecewise<SBasis> y;
	if (rotate)
	{
		x = Piecewise<SBasis>(patternd2[1]);
		y = Piecewise<SBasis>(patternd2[0]);
	}
	else
	{
		x = Piecewise<SBasis>(patternd2[0]);
		y = Piecewise<SBasis>(patternd2[1]);
	}
	Interval pattBnds = bounds_exact(x);
	x -= pattBnds.min();
	Interval pattBndsY = bounds_exact(y);
	y -= (pattBndsY.max()+pattBndsY.min()) / 2.0;
	y -= offsetY;
	double scaling = 1.0;
	int nbCopies = int(uskeleton.cuts.back()/pattBnds.extent());
	if (effect == 0)
	{
		nbCopies = 1;
		scaling = 1.0;
	}
	else if (effect == 1)
	{
		nbCopies = 1;
		scaling = (uskeleton.cuts.back()-offs)/pattBnds.extent();
	}
	else if (effect == 2)
	{
		nbCopies = int((uskeleton.cuts.back()-offs)/(pattBnds.extent()+gapval));
		scaling = 1.0;
	}
	else if (effect == 3)
	{
		nbCopies = int((uskeleton.cuts.back()-offs)/(pattBnds.extent()+gapval));
		scaling = (uskeleton.cuts.back()-offs)/((((double)nbCopies)*pattBnds.extent()) + (((double)nbCopies-1)*gapval));
	}
	double pattWidth = pattBnds.extent() * scaling;
	if (scaling != 1.0)
		x*=scaling;
	FPointArray pathP;
	for (int i=0; i<nbCopies; i++)
	{
		Piecewise<D2<SBasis> > output;
		output.concat(compose(uskeleton,x+offs)+y*compose(n,x+offs));
		offs+=pattWidth+gapval;
		Piecewise2FPointArray(&pathP, output);
		if (nbCopies > 1)
			pathP.setMarker();
	}
	return pathP;
}
