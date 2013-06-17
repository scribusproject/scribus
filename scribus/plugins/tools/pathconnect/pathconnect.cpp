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

#include "pathconnect.h"
#include "pathconnectdialog.h"
#include "selection.h"
#include "scribusdoc.h"
#include "undomanager.h"

int pathconnect_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* pathconnect_getPlugin()
{
	PathConnectPlugin* plug = new PathConnectPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void pathconnect_freePlugin(ScPlugin* plugin)
{
	PathConnectPlugin* plug = dynamic_cast<PathConnectPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

PathConnectPlugin::PathConnectPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

PathConnectPlugin::~PathConnectPlugin() {}

void PathConnectPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "PathConnect";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Connect Paths");
	m_actionInfo.helpText = tr("Connects two Paths.");
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
	m_actionInfo.notSuitableFor.append(PageItem::Spiral);
	m_actionInfo.notSuitableFor.append(PageItem::Arc);
	m_actionInfo.needsNumObjects = 2;
}

const QString PathConnectPlugin::fullTrName() const
{
	return QObject::tr("PathConnect");
}

const ScActionPlugin::AboutData* PathConnectPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Connect Paths");
	about->description = tr("Connect 2 Polylines.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void PathConnectPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool PathConnectPlugin::run(ScribusDoc* doc, QString)
{
	m_doc = doc;
	firstUpdate = true;
	if (m_doc == 0)
		m_doc = ScCore->primaryMainWindow()->doc;
	if (m_doc->m_Selection->count() > 1)
	{
		m_item1 = m_doc->m_Selection->itemAt(0);
		m_item2 = m_doc->m_Selection->itemAt(1);
		originalPath1 = m_item1->PoLine.copy();
		originalPath2 = m_item2->PoLine.copy();
		originalXPos = m_item1->xPos();
		originalYPos = m_item1->yPos();
		PathConnectDialog *dia = new PathConnectDialog(m_doc->scMW());
		connect(dia, SIGNAL(updateValues(int, int, int, int)), this, SLOT(updateEffect(int, int, int, int)));
		if (dia->exec())
		{
			int pointOne = dia->getFirstLinePoint();
			int pointTwo = dia->getSecondLinePoint();
			int mode = dia->getMode();
			UndoTransaction *trans = NULL;
			if(UndoManager::undoEnabled())
				trans = new UndoTransaction(UndoManager::instance()->beginTransaction(Um::BezierCurve,Um::ILine,Um::ConnectPath,"",Um::ILine));

			m_item1->PoLine = computePath(pointOne, pointTwo, mode, originalPath1, originalPath2);
			m_item1->ClipEdited = true;
			m_item1->FrameType = 3;
			m_doc->AdjustItemSize(m_item1);
			m_item1->OldB2 = m_item1->width();
			m_item1->OldH2 = m_item1->height();
			if(UndoManager::undoEnabled())
			{
				ScItemState<QPair<FPointArray,FPointArray> > *is = new ScItemState<QPair<FPointArray,FPointArray> >(Um::ConnectPath);
				is->set("CONNECT_PATH","connect_path");
				is->set("OLDX",originalXPos);
				is->set("OLDY",originalYPos);
				is->set("NEWX",m_item1->xPos());
				is->set("NEWY",m_item1->yPos());
				is->setItem(qMakePair(originalPath1, m_item1->PoLine));
				UndoManager::instance()->action(m_item1,is);
			}
			m_item1->updateClip();
			m_item1->ContourLine = m_item1->PoLine.copy();
			m_doc->m_Selection->removeItem(m_item1);
			m_doc->itemSelection_DeleteItem();
			m_doc->changed();
			if(trans)
			{
				trans->commit();
				delete trans;
				trans = NULL;
			}
		}
		else
		{
			m_item1->PoLine = originalPath1.copy();
			m_item1->ClipEdited = true;
			m_item1->FrameType = 3;
			m_item1->setXYPos(originalXPos, originalYPos);
			m_doc->AdjustItemSize(m_item1);
			m_item1->OldB2 = m_item1->width();
			m_item1->OldH2 = m_item1->height();
			m_item1->updateClip();
			m_item1->ContourLine = m_item1->PoLine.copy();
		}
		m_doc->view()->DrawNew();
		delete dia;
	}
	return true;
}

void PathConnectPlugin::updateEffect(int effectType, int pointOne, int pointTwo, int mode)
{
	if (effectType == -1)
	{
		m_item1->PoLine = originalPath1.copy();
		m_item1->ClipEdited = true;
		m_item1->FrameType = 3;
		m_item1->setXYPos(originalXPos, originalYPos);
		firstUpdate = true;
	}
	else
	{
		m_item1->setXYPos(originalXPos, originalYPos);
		m_item1->PoLine = computePath(pointOne, pointTwo, mode, originalPath1, originalPath2);
		m_item1->ClipEdited = true;
		m_item1->FrameType = 3;
	}
	m_doc->AdjustItemSize(m_item1);
	m_item1->OldB2 = m_item1->width();
	m_item1->OldH2 = m_item1->height();
	m_item1->updateClip();
	if (firstUpdate)
		m_doc->view()->DrawNew();
	else
	{
		QRectF oldR(m_item1->getBoundingRect());
		QRectF newR(m_item2->getBoundingRect());
		m_doc->regionsChanged()->update(newR.united(oldR));
	}
	if (effectType != -1)
		firstUpdate = false;
}

FPointArray PathConnectPlugin::computePath(int pointOne, int pointTwo, int mode, FPointArray &p1, FPointArray &p2)
{
	FPointArray result;
	FPointArray path1 = p1.copy();
	FPointArray path2 = p2.copy();
	QTransform ma;
	ma.translate(m_item2->xPos(), m_item2->yPos());
	ma.rotate(m_item2->rotation());
	path2.map(ma);
	QTransform ma2;
	ma2.translate(originalXPos, originalYPos);
	ma2.rotate(m_item1->rotation());
	ma2 = ma2.inverted();
	path2.map(ma2);
	FPoint startL1 = path1.point(0);
	FPoint startL2 = path2.point(0);
	FPoint endL1 = path1.point(path1.size() - 2);
	FPoint endL2 = path2.point(path2.size() - 2);
	if (pointOne == 0)
	{
		if (pointTwo == 0)
		{
			path2 = reversePath(path2);
			if (mode == 0)
				path2.addQuadPoint(startL2, startL2, startL1, startL1);
			else
			{
				FPoint midP = (startL2 + startL1) * 0.5;
				FPoint corr1 = path1.point(1) + (midP - startL1);
				FPoint corr2 = path2.point(path2.size() - 1) + (midP - startL2);
				path1.setPoint(1, corr1);
				path2.setPoint(path2.size() - 1, corr2);
				path2.setPoint(path2.size() - 2, midP);
				path1.setPoint(0, midP);
			}
			path2.putPoints(path2.size(), path1.size(), path1);
			result = path2.copy();
		}
		else if (pointTwo == 1)
		{
			if (mode == 0)
				path2.addQuadPoint(endL2, endL2, startL1, startL1);
			else
			{
				FPoint midP = (startL1 + endL2) * 0.5;
				FPoint corr1 = path1.point(1) + (midP - startL1);
				FPoint corr2 = path2.point(path2.size() - 1) + (midP - endL2);
				path1.setPoint(1, corr1);
				path2.setPoint(path2.size() - 1, corr2);
				path2.setPoint(path2.size() - 2, midP);
				path1.setPoint(0, midP);
			}
			path2.putPoints(path2.size(), path1.size(), path1);
			result = path2.copy();
		}
	}
	else if (pointOne == 1)
	{
		if (pointTwo == 0)
		{
			if (mode == 0)
				path1.addQuadPoint(endL1, endL1, startL2, startL2);
			else
			{
				FPoint midP = (startL2 + endL1) * 0.5;
				FPoint corr1 = path1.point(path1.size() - 1) + (midP - endL1);
				FPoint corr2 = path2.point(1) + (midP - startL2);
				path1.setPoint(path1.size() - 1, corr1);
				path2.setPoint(1, corr2);
				path1.setPoint(path1.size() - 2, midP);
				path2.setPoint(0, midP);
			}
			path1.putPoints(path1.size(), path2.size(), path2);
			result = path1.copy();
		}
		else if (pointTwo == 1)
		{
			path2 = reversePath(path2);
			if (mode == 0)
				path1.addQuadPoint(endL1, endL1, endL2, endL2);
			else
			{
				FPoint midP = (endL2 + endL1) * 0.5;
				FPoint corr1 = path1.point(path1.size() - 1) + (midP - endL1);
				FPoint corr2 = path2.point(1) + (midP - endL2);
				path1.setPoint(path1.size() - 1, corr1);
				path2.setPoint(1, corr2);
				path1.setPoint(path1.size() - 2, midP);
				path2.setPoint(0, midP);
			}
			path1.putPoints(path1.size(), path2.size(), path2);
			result = path1.copy();
		}
	}
	return result;
}

FPointArray PathConnectPlugin::reversePath(FPointArray &path)
{
	FPointArray result;
	for (int a = path.size()-4; a > -1; a -= 4)
	{
		FPoint p1 = path.point(a);
		FPoint p2 = path.point(a+1);
		FPoint p3 = path.point(a+2);
		FPoint p4 = path.point(a+3);
		result.addQuadPoint(p3, p4, p1, p2);
	}
	return result;
}
