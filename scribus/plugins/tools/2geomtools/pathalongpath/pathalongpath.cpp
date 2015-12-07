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

#include "pathalongpath.h"

#include "appmodes.h"
#include "pageitem_group.h"
#include "pathdialog.h"
#include "scribuscore.h"
#include "scribusview.h"
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
	m_actionInfo.helpText = tr("Bends a Polygon along a Path.");
	// Menu
	m_actionInfo.menu = "ItemPathOps";
	m_actionInfo.parentMenu = "Item";
	m_actionInfo.subMenuName = tr("Path Tools");
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.notSuitableFor.append(PageItem::TextFrame);
	m_actionInfo.notSuitableFor.append(PageItem::ImageFrame);
	m_actionInfo.notSuitableFor.append(PageItem::PathText);
	m_actionInfo.notSuitableFor.append(PageItem::LatexFrame);
	m_actionInfo.notSuitableFor.append(PageItem::Symbol);
	m_actionInfo.notSuitableFor.append(PageItem::RegularPolygon);
	m_actionInfo.notSuitableFor.append(PageItem::Arc);
	m_actionInfo.notSuitableFor.append(PageItem::Spiral);
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
		if (doc->m_Selection->count() == 2)
		{
			PageItem *currItem = doc->m_Selection->itemAt(0);
			if (currItem->isGroup())
			{
				currItem = doc->m_Selection->itemAt(1);
				if (currItem->itemType() != PageItem::PolyLine)
					ret = false;
				else
					ret = true;
			}
			else
			{
				if (currItem->itemType() != PageItem::PolyLine)
					ret = false;
				else
				{
					currItem = doc->m_Selection->itemAt(1);
					if (currItem->isGroup())
					{
						ret = true;
					}
				}
			}
		}
	}
	return ret;
}

bool PathAlongPathPlugin::run(ScribusDoc* doc, QString)
{
	m_firstUpdate = true;
	m_doc = doc;
	m_originalPathG.clear();
	m_originalRotG.clear();
	m_originalXPosG.clear();
	m_originalYPosG.clear();
	m_patternItemG.clear();
	if (m_doc == 0)
		m_doc = ScCore->primaryMainWindow()->doc;
	if (m_doc->m_Selection->count() > 1)
	{
		if ((m_doc->m_Selection->itemAt(0)->isGroup()) || (m_doc->m_Selection->itemAt(1)->isGroup()))
		{
			m_selOffs = 0;
			m_selCount = m_doc->m_Selection->count() - 1;
			if (!m_doc->m_Selection->itemAt(0)->isGroup())
			{
				m_pathItem = m_doc->m_Selection->itemAt(0);
				m_selOffs = 1;
			}
			else
				m_pathItem = m_doc->m_Selection->itemAt(m_selCount);
			m_effectPath = m_pathItem->PoLine.copy();
			QTransform mp;
			mp.rotate(m_pathItem->rotation());
			m_effectPath.map(mp);
			PageItem* bxi = m_doc->m_Selection->itemAt(m_selOffs);
			bxi->asGroupFrame()->adjustXYPosition();
			m_originalPathG.append(bxi->PoLine.copy());
			m_originalXPosG.append(bxi->xPos());
			m_originalYPosG.append(bxi->yPos());
			m_originalXPosGi.append(bxi->gXpos);
			m_originalYPosGi.append(bxi->gYpos);
			m_originalRotG.append(bxi->rotation());
			m_originalWidth.append(bxi->width());
			m_originalHeight.append(bxi->height());
			m_originalWidthG.append(bxi->groupWidth);
			m_originalHeightG.append(bxi->groupHeight);
			m_patternItemG.append(bxi);
			QList<PageItem*> bxiL = bxi->getItemList();
			for (int bx = 0; bx < bxiL.count(); ++bx)
			{
				PageItem* cIte = bxiL.at(bx);
				m_originalPathG.append(cIte->PoLine.copy());
				m_originalXPosG.append(cIte->xPos());
				m_originalYPosG.append(cIte->yPos());
				m_originalWidth.append(cIte->width());
				m_originalHeight.append(cIte->height());
				m_originalWidthG.append(cIte->groupWidth);
				m_originalHeightG.append(cIte->groupHeight);
				m_originalXPosGi.append(cIte->gXpos);
				m_originalYPosGi.append(cIte->gYpos);
				m_originalRotG.append(cIte->rotation());
				m_patternItemG.append(cIte);
			}
			QPainterPath tmpPath = m_effectPath.toQPainterPath(false);
			PathDialog *dia = new PathDialog(m_doc->scMW(), m_doc->unitIndex(), tmpPath.length(), true);
			connect(dia, SIGNAL(updateValues(int, double, double, double, int)), this, SLOT(updateEffectG(int, double, double, double, int)));
			if (dia->exec())
			{
				updateEffectG(dia->effectType, dia->offset, dia->offsetY, dia->gap, dia->rotate);
				m_doc->changed();
			}
			else
			{
				updateEffectG(-1, dia->offset, dia->offsetY, dia->gap, dia->rotate);
				m_doc->view()->DrawNew();
			}
			delete dia;
		}
		else
		{
			m_patternItem = m_doc->m_Selection->itemAt(0);
			m_pathItem = m_doc->m_Selection->itemAt(1);
			if (m_pathItem->itemType() != PageItem::PolyLine)
			{
				m_patternItem = m_doc->m_Selection->itemAt(1);
				m_pathItem = m_doc->m_Selection->itemAt(0);
			}
			m_effectPath = m_pathItem->PoLine.copy();
			QTransform mp;
			mp.rotate(m_pathItem->rotation());
			m_effectPath.map(mp);
			m_originalPath = m_patternItem->PoLine.copy();
			m_originalXPos = m_patternItem->xPos();
			m_originalYPos = m_patternItem->yPos();
			m_originalRot = m_patternItem->rotation();
			QPainterPath tmpPath = m_effectPath.toQPainterPath(false);
			PathDialog *dia = new PathDialog(m_doc->scMW(), m_doc->unitIndex(), tmpPath.length(), false);
			connect(dia, SIGNAL(updateValues(int, double, double, double, int)), this, SLOT(updateEffect(int, double, double, double, int)));
			if (dia->exec())
			{
				updateEffect(dia->effectType, dia->offset, dia->offsetY, dia->gap, dia->rotate);
				m_patternItem->ContourLine = m_patternItem->PoLine.copy();
				m_doc->changed();
			}
			else
			{
				m_patternItem->PoLine = m_originalPath;
				m_patternItem->ClipEdited = true;
				m_patternItem->FrameType = 3;
				m_patternItem->setXYPos(m_originalXPos, m_originalYPos);
				m_patternItem->setRotation(m_originalRot);
				m_doc->AdjustItemSize(m_patternItem);
				m_patternItem->OldB2 = m_patternItem->width();
				m_patternItem->OldH2 = m_patternItem->height();
				m_patternItem->updateClip();
				m_doc->view()->DrawNew();
			}
			delete dia;
		}
	}
	return true;
}

void PathAlongPathPlugin::updateEffectG(int effectType, double offset, double offsetY, double gap, int rotate)
{
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	for (int bx = 0; bx < m_patternItemG.count(); ++bx)
	{
		PageItem* bxi = m_patternItemG[bx];
		bxi->PoLine = m_originalPathG[bx];
		bxi->ClipEdited = true;
		bxi->FrameType = 3;
		bxi->setXYPos(m_originalXPosG[bx], m_originalYPosG[bx], true);
		bxi->setRotation(m_originalRotG[bx]);
		bxi->gXpos = m_originalXPosGi[bx];
		bxi->gYpos = m_originalYPosGi[bx];
		bxi->setWidthHeight(m_originalWidth[bx], m_originalHeight[bx], true);
		bxi->groupWidth = m_originalWidthG[bx];
		bxi->groupHeight = m_originalHeightG[bx];
		bxi->OldB2 = bxi->width();
		bxi->OldH2 = bxi->height();
		bxi->updateClip();
		bxi->ContourLine = bxi->PoLine.copy();
	}
	m_firstUpdate = true;
	if (effectType != -1)
	{
		Geom::Piecewise<Geom::D2<Geom::SBasis> > originaldpwd2 = FPointArray2Piecewise(m_effectPath, false);
		Geom::Piecewise<Geom::D2<Geom::SBasis> > patternpwd2;
		PageItem* bxi = m_patternItemG[0];
		double originX = m_originalXPosG[0];
		double originY = m_originalYPosG[0];
		if (bxi->itemType() == PageItem::PolyLine)
			patternpwd2 = FPointArray2Piecewise(m_originalPathG[0], false);
		else
			patternpwd2 = FPointArray2Piecewise(m_originalPathG[0], true);
		setUpEffect(originaldpwd2, patternpwd2, effectType, offset / m_doc->unitRatio(), offsetY / m_doc->unitRatio(), gap / m_doc->unitRatio(), rotate);
		for (int bx = 0; bx < m_patternItemG.count(); ++bx)
		{
			PageItem* bxi = m_patternItemG[bx];
			FPointArray pathP = m_originalPathG[bx].copy();
			FPoint tp(getMinClipF(&pathP));
			double deltaX, deltaY;
			deltaX = m_originalXPosG[bx] - originX;
			deltaY = m_originalYPosG[bx] - originY;
			QTransform mm;
			mm.rotate(m_originalRotG[bx]);
			pathP.map(mm);
			pathP.translate(deltaX, deltaY);
			if (bxi->itemType() == PageItem::PolyLine)
				patternpwd2 = FPointArray2Piecewise(pathP, false);
			else
				patternpwd2 = FPointArray2Piecewise(pathP, true);
			bxi->PoLine = doEffect_pwd2(patternpwd2);
			bxi->PoLine.translate(-deltaX, -deltaY);
			QTransform mm2;
			mm2.rotate(-m_originalRotG[bx]);
			bxi->PoLine.map(mm2);
			bxi->ClipEdited = true;
			bxi->FrameType = 3;
			bxi->setXYPos(m_pathItem->xPos()+deltaX, m_pathItem->yPos()+deltaY, true);
			double oW = bxi->width();
			double oH = bxi->height();
			m_doc->AdjustItemSize(bxi, true);
			bxi->OldB2 = bxi->width();
			bxi->OldH2 = bxi->height();
			if (bxi->isGroup())
			{
				bxi->groupWidth = bxi->groupWidth * (bxi->OldB2 / oW);
				bxi->groupHeight = bxi->groupHeight * (bxi->OldH2 / oH);
			}
			bxi->updateClip();
			bxi->ContourLine = bxi->PoLine.copy();
		}
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	if (m_firstUpdate)
		m_doc->view()->DrawNew();
	else
	{
		double gx, gy, gh, gw;
		m_doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
		QRectF oldR(m_pathItem->getBoundingRect());
		QRectF newR = QRectF(gx, gy, gw, gh);
		m_doc->regionsChanged()->update(newR.united(oldR));
	}
	if (effectType != -1)
		m_firstUpdate = false;
}

void PathAlongPathPlugin::updateEffect(int effectType, double offset, double offsetY, double gap, int rotate)
{
	if (effectType == -1)
	{
		m_patternItem->PoLine = m_originalPath;
		m_patternItem->ClipEdited = true;
		m_patternItem->FrameType = 3;
		m_patternItem->setXYPos(m_originalXPos, m_originalYPos);
		m_patternItem->setRotation(m_originalRot);
		m_firstUpdate = true;
	}
	else
	{
		Geom::Piecewise<Geom::D2<Geom::SBasis> > originaldpwd2 = FPointArray2Piecewise(m_effectPath, false);
		Geom::Piecewise<Geom::D2<Geom::SBasis> > patternpwd2;
		if (m_patternItem->itemType() == PageItem::PolyLine)
			patternpwd2 = FPointArray2Piecewise(m_originalPath, false);
		else
			patternpwd2 = FPointArray2Piecewise(m_originalPath, true);
		setUpEffect(originaldpwd2, patternpwd2, effectType, offset / m_doc->unitRatio(), offsetY / m_doc->unitRatio(), gap / m_doc->unitRatio(), rotate);
		m_patternItem->PoLine = doEffect_pwd2(patternpwd2);
		m_patternItem->ClipEdited = true;
		m_patternItem->FrameType = 3;
		m_patternItem->setXYPos(m_pathItem->xPos(), m_pathItem->yPos());
		m_patternItem->setRotation(0);
	}
	m_doc->AdjustItemSize(m_patternItem, true);
	m_patternItem->OldB2 = m_patternItem->width();
	m_patternItem->OldH2 = m_patternItem->height();
	m_patternItem->updateClip();
	if (m_firstUpdate)
		m_doc->view()->DrawNew();
	else
	{
		QRectF oldR(m_pathItem->getBoundingRect());
		QRectF newR(m_patternItem->getBoundingRect());
		m_doc->regionsChanged()->update(newR.united(oldR));
	}
	if (effectType != -1)
		m_firstUpdate = false;
}

void PathAlongPathPlugin::setUpEffect(Geom::Piecewise<Geom::D2<Geom::SBasis> > &pwd2_in, Geom::Piecewise<Geom::D2<Geom::SBasis> > &pattern, int effect, double offset, double offsetY, double gap, int rotate)
{
	m_offsetX = offset;
	m_offsetY = offsetY;
	m_gapval = gap;
	m_rotate = rotate;
	m_uskeleton = arc_length_parametrization(pwd2_in, 2, .1);
	m_uskeleton = remove_short_cuts(m_uskeleton,.01);
	m_n = rot90(derivative(m_uskeleton));
	m_n = force_continuity(remove_short_cuts(m_n,.1));
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
	m_pattBnds = bounds_exact(x);
	x -= m_pattBnds.min();
	m_pattBndsY = bounds_exact(y);
	y -= (m_pattBndsY.max()+m_pattBndsY.min()) / 2.0;
	y -= offsetY;
	m_scaling = 1.0;
	m_nbCopies = int(m_uskeleton.cuts.back()/m_pattBnds.extent());
	if (effect == 0)
	{
		m_nbCopies = 1;
		m_scaling = 1.0;
	}
	else if (effect == 1)
	{
		m_nbCopies = 1;
		m_scaling = (m_uskeleton.cuts.back()-m_offsetX)/m_pattBnds.extent();
	}
	else if (effect == 2)
	{
		m_nbCopies = int((m_uskeleton.cuts.back()-m_offsetX)/(m_pattBnds.extent()+m_gapval));
		m_scaling = 1.0;
	}
	else if (effect == 3)
	{
		m_nbCopies = int((m_uskeleton.cuts.back()-m_offsetX)/(m_pattBnds.extent()+m_gapval));
		m_scaling = (m_uskeleton.cuts.back()-m_offsetX)/((((double)m_nbCopies)*m_pattBnds.extent()) + (((double)m_nbCopies-1)*m_gapval));
	}
	m_pattWidth = m_pattBnds.extent() * m_scaling;
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
	x -= m_pattBnds.min();
	y -= (m_pattBndsY.max()+m_pattBndsY.min()) / 2.0;
	y -= m_offsetY;
	if (m_scaling != 1.0)
		x*=m_scaling;
	FPointArray pathP;
	for (int i=0; i<m_nbCopies; i++)
	{
		Piecewise<D2<SBasis> > output;
		output.concat(compose(m_uskeleton,x+offs)+y*compose(m_n,x+offs));
		offs+=m_pattWidth+m_gapval;
		Piecewise2FPointArray(&pathP, output);
		if (m_nbCopies > 1)
			pathP.setMarker();
	}
	return pathP;
}
