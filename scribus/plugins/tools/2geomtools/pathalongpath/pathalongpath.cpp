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

bool PathAlongPathPlugin::handleSelection(ScribusDoc* doc, int SelectedType)
{
	bool ret = ScActionPlugin::handleSelection(doc, SelectedType);
	if (!ret)
	{
		if (doc->m_Selection->count() > 1)
		{
			bool isGroup = true;
			int firstElem = -1;
			PageItem *currItem = doc->m_Selection->itemAt(0);
			if (currItem->Groups.count() != 0)
			{
				firstElem = currItem->Groups.top();
				for (int bx = 0; bx < doc->m_Selection->count() - 1; ++bx)
				{
					PageItem* bxi = doc->m_Selection->itemAt(bx);
					if (bxi->Groups.count() != 0)
					{
						if (bxi->Groups.top() != firstElem)
							isGroup = false;
						if (currItem->itemType() == PageItem::Line)
							isGroup = false;
					}
					else
						isGroup = false;
				}
				currItem = doc->m_Selection->itemAt(doc->m_Selection->count() - 1);
				if (currItem->itemType() != PageItem::PolyLine)
					ret = false;
				else
					ret = isGroup;
			}
			else
			{
				if (currItem->itemType() != PageItem::PolyLine)
					ret = false;
				else
				{
					currItem = doc->m_Selection->itemAt(1);
					if (currItem->Groups.count() != 0)
					{
						firstElem = currItem->Groups.top();
						for (int bx = 1; bx < doc->m_Selection->count(); ++bx)
						{
							PageItem* bxi = doc->m_Selection->itemAt(bx);
							if (bxi->Groups.count() != 0)
							{
								if (bxi->Groups.top() != firstElem)
									isGroup = false;
								if (currItem->itemType() == PageItem::Line)
									isGroup = false;
							}
							else
								isGroup = false;
						}
						ret = isGroup;
					}
				}
			}
		}
	}
	return ret;
}

bool PathAlongPathPlugin::run(ScribusDoc* doc, QString)
{
	firstUpdate = true;
	currDoc = doc;
	originalPathG.clear();
	originalRotG.clear();
	originalXPosG.clear();
	originalYPosG.clear();
	patternItemG.clear();
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc->m_Selection->count() > 1)
	{
		if ((currDoc->m_Selection->itemAt(0)->Groups.count() != 0) || (currDoc->m_Selection->itemAt(1)->Groups.count() != 0))
		{
			selOffs = 0;
			selCount = currDoc->m_Selection->count() - 1;
			if (currDoc->m_Selection->itemAt(0)->Groups.count() == 0)
			{
				pathItem = currDoc->m_Selection->itemAt(0);
				selOffs = 1;
			}
			else
				pathItem = currDoc->m_Selection->itemAt(selCount);
			effectPath = pathItem->PoLine.copy();
			QMatrix mp;
			mp.rotate(pathItem->rotation());
			effectPath.map(mp);
			for (int bx = 0; bx < selCount; ++bx)
			{
				PageItem* bxi = currDoc->m_Selection->itemAt(bx + selOffs);
				originalPathG.append(bxi->PoLine.copy());
				originalXPosG.append(bxi->xPos());
				originalYPosG.append(bxi->yPos());
				originalRotG.append(bxi->rotation());
				patternItemG.append(bxi);
			}
			PathDialog *dia = new PathDialog(currDoc->scMW(), currDoc->unitIndex(), true);
			connect(dia, SIGNAL(updateValues(int, double, double, double, int)), this, SLOT(updateEffectG(int, double, double, double, int)));
			if (dia->exec())
			{
				updateEffectG(dia->effectType, dia->offset, dia->offsetY, dia->gap, dia->rotate);
				currDoc->changed();
			}
			else
			{
				updateEffectG(-1, dia->offset, dia->offsetY, dia->gap, dia->rotate);
				currDoc->view()->DrawNew();
			}
			delete dia;
		}
		else
		{
			patternItem = currDoc->m_Selection->itemAt(0);
			pathItem = currDoc->m_Selection->itemAt(1);
			if (pathItem->itemType() != PageItem::PolyLine)
			{
				patternItem = currDoc->m_Selection->itemAt(1);
				pathItem = currDoc->m_Selection->itemAt(0);
			}
			effectPath = pathItem->PoLine.copy();
			QMatrix mp;
			mp.rotate(pathItem->rotation());
			effectPath.map(mp);
			originalPath = patternItem->PoLine.copy();
			originalXPos = patternItem->xPos();
			originalYPos = patternItem->yPos();
			originalRot = patternItem->rotation();
			PathDialog *dia = new PathDialog(currDoc->scMW(), currDoc->unitIndex(), false);
			connect(dia, SIGNAL(updateValues(int, double, double, double, int)), this, SLOT(updateEffect(int, double, double, double, int)));
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
				patternItem->setRotation(originalRot);
				currDoc->AdjustItemSize(patternItem);
				patternItem->OldB2 = patternItem->width();
				patternItem->OldH2 = patternItem->height();
				patternItem->updateClip();
				currDoc->view()->DrawNew();
			}
			delete dia;
		}
	}
	return true;
}

void PathAlongPathPlugin::updateEffectG(int effectType, double offset, double offsetY, double gap, int rotate)
{
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	if (effectType == -1)
	{
		for (int bx = 0; bx < patternItemG.count(); ++bx)
		{
			PageItem* bxi = patternItemG[bx];
			bxi->PoLine = originalPathG[bx];
			bxi->Frame = false;
			bxi->ClipEdited = true;
			bxi->FrameType = 3;
			bxi->setXYPos(originalXPosG[bx], originalYPosG[bx]);
			bxi->setRotation(originalRotG[bx]);
			currDoc->AdjustItemSize(bxi);
			bxi->OldB2 = bxi->width();
			bxi->OldH2 = bxi->height();
			bxi->updateClip();
			bxi->ContourLine = bxi->PoLine.copy();
		}
		firstUpdate = true;
	}
	else
	{
		Geom::Piecewise<Geom::D2<Geom::SBasis> > originaldpwd2 = FPointArray2Piecewise(effectPath, false);
		Geom::Piecewise<Geom::D2<Geom::SBasis> > patternpwd2;
		PageItem* bxi = patternItemG[0];
		double originX = originalXPosG[0];
		double originY = originalYPosG[0];
		if (bxi->itemType() == PageItem::PolyLine)
			patternpwd2 = FPointArray2Piecewise(originalPathG[0], false);
		else
			patternpwd2 = FPointArray2Piecewise(originalPathG[0], true);
		setUpEffect(originaldpwd2, patternpwd2, effectType, offset / currDoc->unitRatio(), offsetY / currDoc->unitRatio(), gap / currDoc->unitRatio(), rotate);
		for (int bx = 0; bx < patternItemG.count(); ++bx)
		{
			PageItem* bxi = patternItemG[bx];
			FPointArray pathP = originalPathG[bx].copy();
			double deltaX = originalXPosG[bx] - originX;
			double deltaY = originalYPosG[bx] - originY;
			QMatrix mm;
			mm.rotate(originalRotG[bx]);
			pathP.map(mm);
			pathP.translate(deltaX, deltaY);
			if (bxi->itemType() == PageItem::PolyLine)
				patternpwd2 = FPointArray2Piecewise(pathP, false);
			else
				patternpwd2 = FPointArray2Piecewise(pathP, true);
			bxi->PoLine = doEffect_pwd2(patternpwd2);
			bxi->PoLine.translate(-deltaX, -deltaY);
			QMatrix mm2;
			mm2.rotate(-originalRotG[bx]);
			bxi->PoLine.map(mm2);
			bxi->Frame = false;
			bxi->ClipEdited = true;
			bxi->FrameType = 3;
			bxi->setXYPos(pathItem->xPos()+deltaX, pathItem->yPos()+deltaY);
			currDoc->AdjustItemSize(bxi);
			bxi->OldB2 = bxi->width();
			bxi->OldH2 = bxi->height();
			bxi->updateClip();
			bxi->ContourLine = bxi->PoLine.copy();
		}
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	if (firstUpdate)
		currDoc->view()->DrawNew();
	else
	{
		double gx, gy, gh, gw;
		currDoc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		QRectF oldR(pathItem->getBoundingRect());
		QRectF newR = QRectF(gx, gy, gw, gh);
		currDoc->regionsChanged()->update(newR.unite(oldR));
	}
	if (effectType != -1)
		firstUpdate = false;
}

void PathAlongPathPlugin::updateEffect(int effectType, double offset, double offsetY, double gap, int rotate)
{
	if (effectType == -1)
	{
		patternItem->PoLine = originalPath;
		patternItem->Frame = false;
		patternItem->ClipEdited = true;
		patternItem->FrameType = 3;
		patternItem->setXYPos(originalXPos, originalYPos);
		patternItem->setRotation(originalRot);
		firstUpdate = true;
	}
	else
	{
		Geom::Piecewise<Geom::D2<Geom::SBasis> > originaldpwd2 = FPointArray2Piecewise(effectPath, false);
		Geom::Piecewise<Geom::D2<Geom::SBasis> > patternpwd2;
		if (patternItem->itemType() == PageItem::PolyLine)
			patternpwd2 = FPointArray2Piecewise(originalPath, false);
		else
			patternpwd2 = FPointArray2Piecewise(originalPath, true);
		setUpEffect(originaldpwd2, patternpwd2, effectType, offset / currDoc->unitRatio(), offsetY / currDoc->unitRatio(), gap / currDoc->unitRatio(), rotate);
		patternItem->PoLine = doEffect_pwd2(patternpwd2);
		patternItem->Frame = false;
		patternItem->ClipEdited = true;
		patternItem->FrameType = 3;
		patternItem->setXYPos(pathItem->xPos(), pathItem->yPos());
		patternItem->setRotation(0);
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

void PathAlongPathPlugin::setUpEffect(Geom::Piecewise<Geom::D2<Geom::SBasis> > &pwd2_in, Geom::Piecewise<Geom::D2<Geom::SBasis> > &pattern, int effect, double offset, double offsetY, double gap, int rotate)
{
	m_offsetX = offset;
	m_offsetY = offsetY;
	m_gapval = gap;
	m_rotate = rotate;
	uskeleton = arc_length_parametrization(pwd2_in, 2, .1);
	uskeleton = remove_short_cuts(uskeleton,.01);
	n = rot90(derivative(uskeleton));
	n = force_continuity(remove_short_cuts(n,.1));
	D2<Piecewise<SBasis> > patternd2;
	if (rotate == 1)
		patternd2 = make_cuts_independant(rot90(pattern));
	else if (rotate == 2)
		patternd2 = make_cuts_independant(rot90(rot90(pattern)));
	else if (rotate == 3)
		patternd2 = make_cuts_independant(rot90(rot90(rot90(pattern))));
	else
		patternd2 = make_cuts_independant(pattern);
	Piecewise<SBasis> x = Piecewise<SBasis>(patternd2[0]);
	Piecewise<SBasis> y = Piecewise<SBasis>(patternd2[1]);
	pattBnds = bounds_exact(x);
	x -= pattBnds.min();
	pattBndsY = bounds_exact(y);
	y -= (pattBndsY.max()+pattBndsY.min()) / 2.0;
	y -= offsetY;
	m_scaling = 1.0;
	nbCopies = int(uskeleton.cuts.back()/pattBnds.extent());
	if (effect == 0)
	{
		nbCopies = 1;
		m_scaling = 1.0;
	}
	else if (effect == 1)
	{
		nbCopies = 1;
		m_scaling = (uskeleton.cuts.back()-m_offsetX)/pattBnds.extent();
	}
	else if (effect == 2)
	{
		nbCopies = int((uskeleton.cuts.back()-m_offsetX)/(pattBnds.extent()+m_gapval));
		m_scaling = 1.0;
	}
	else if (effect == 3)
	{
		nbCopies = int((uskeleton.cuts.back()-m_offsetX)/(pattBnds.extent()+m_gapval));
		m_scaling = (uskeleton.cuts.back()-m_offsetX)/((((double)nbCopies)*pattBnds.extent()) + (((double)nbCopies-1)*m_gapval));
	}
	pattWidth = pattBnds.extent() * m_scaling;
}

FPointArray PathAlongPathPlugin::doEffect_pwd2(Geom::Piecewise<Geom::D2<Geom::SBasis> > &pattern)
{
	double offs = m_offsetX;
	D2<Piecewise<SBasis> > patternd2;
	if (m_rotate == 1)
		patternd2 = make_cuts_independant(rot90(pattern));
	else if (m_rotate == 2)
		patternd2 = make_cuts_independant(rot90(rot90(pattern)));
	else if (m_rotate == 3)
		patternd2 = make_cuts_independant(rot90(rot90(rot90(pattern))));
	else
		patternd2 = make_cuts_independant(pattern);
	Piecewise<SBasis> x = Piecewise<SBasis>(patternd2[0]);
	Piecewise<SBasis> y = Piecewise<SBasis>(patternd2[1]);
	x -= pattBnds.min();
	y -= (pattBndsY.max()+pattBndsY.min()) / 2.0;
	y -= m_offsetY;
	if (m_scaling != 1.0)
		x*=m_scaling;
	FPointArray pathP;
	for (int i=0; i<nbCopies; i++)
	{
		Piecewise<D2<SBasis> > output;
		output.concat(compose(uskeleton,x+offs)+y*compose(n,x+offs));
		offs+=pattWidth+m_gapval;
		Piecewise2FPointArray(&pathP, output);
		if (nbCopies > 1)
			pathP.setMarker();
	}
	return pathP;
}
