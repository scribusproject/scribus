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
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.              *
****************************************************************************/

#include "transformeffect.h"
#include "transformdialog.h"
#include "scribuscore.h"

int transformeffect_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* transformeffect_getPlugin()
{
	TransformEffectPlugin* plug = new TransformEffectPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void transformeffect_freePlugin(ScPlugin* plugin)
{
	TransformEffectPlugin* plug = dynamic_cast<TransformEffectPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

TransformEffectPlugin::TransformEffectPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

TransformEffectPlugin::~TransformEffectPlugin() {};

void TransformEffectPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "TransformEffect";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Transform...");
	// Menu
	m_actionInfo.menu = "Item";
	m_actionInfo.menuAfterName = "MulDuplicate";
	m_actionInfo.enabledOnStartup = true;
	m_actionInfo.needsNumObjects = 3;
	m_actionInfo.notSuitableFor.append(PageItem::Line);
	m_actionInfo.forAppMode.append(modeNormal);
}

const QString TransformEffectPlugin::fullTrName() const
{
	return QObject::tr("Transform Effect");
}

const ScActionPlugin::AboutData* TransformEffectPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Franz Schmid <Franz.Schmid@altmuehlnet.de>");
	about->shortDescription = tr("Transform Effect");
	about->description = tr("Apply multiple transformations at once");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void TransformEffectPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool TransformEffectPlugin::run(ScribusDoc* doc, QString)
{
	ScribusDoc* currDoc = doc;
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc->m_Selection->count() > 0)
	{
		TransformDialog *dia = new TransformDialog(currDoc->scMW(), currDoc);
		if (dia->exec())
		{
			qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
			int nrOfCopies = dia->getCount();
			QMatrix matrix = dia->getTransformMatrix();
			int baseP = dia->getBasepoint();
			if (nrOfCopies == 0)
			{
				double gx, gy, gh, gw;
				PageItem *currItem = currDoc->m_Selection->itemAt(0);
				if (currDoc->m_Selection->count() == 1)
				{
					gx = currItem->xPos();
					gy = currItem->yPos();
					gw = currItem->width();
					gh = currItem->height();
				}
				else
					currDoc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
				for (int a = 0; a < currDoc->m_Selection->count(); ++a)
				{
					PageItem *currItem = currDoc->m_Selection->itemAt(a);
					double deltaX = currItem->xPos() - gx;
					double deltaY = currItem->yPos() - gy;
					QMatrix matrixPre = QMatrix();
					QMatrix matrixAft = QMatrix();
					switch (baseP)
					{
						case 2:
							matrixPre.translate(-gw/2.0, -gh/2.0);
							matrixAft.translate(gw/2.0, gh/2.0);
							break;
						case 4:
							matrixPre.translate(-gw, -gh);
							matrixAft.translate(gw, gh);
							break;
						case 3:
							matrixPre.translate(0, -gh);
							matrixAft.translate(0, gh);
							break;
						case 1:
							matrixPre.translate(-gw, 0);
							matrixAft.translate(gw, 0);
							break;
					}
					currItem->PoLine.translate(deltaX, deltaY);
					currItem->PoLine.map(matrixPre);
					currItem->PoLine.map(matrix);
					currItem->PoLine.map(matrixAft);
					currItem->PoLine.translate(-deltaX, -deltaY);
					currItem->ContourLine.translate(deltaX, deltaY);
					currItem->ContourLine.map(matrixPre);
					currItem->ContourLine.map(matrix);
					currItem->ContourLine.map(matrixAft);
					currItem->ContourLine.translate(-deltaX, -deltaY);
					currItem->Frame = false;
					currItem->ClipEdited = true;
					currItem->FrameType = 3;
					currDoc->AdjustItemSize(currItem);
				}
			}
			else
			{
				QList<PageItem*> Elements;
				bool savedAlignGrid = currDoc->useRaster;
				bool savedAlignGuides = currDoc->SnapGuides;
				currDoc->useRaster = false;
				currDoc->SnapGuides = false;
				currDoc->DoDrawing = false;
				currDoc->view()->updatesOn(false);
				currDoc->m_Selection->delaySignalsOn();
				currDoc->scMW()->ScriptRunning = true;
				QMatrix comulatedMatrix = matrix;
				PageItem *currItem = currDoc->m_Selection->itemAt(0);
				Elements.append(currItem);
				int rotBack = currDoc->RotMode;
				currDoc->RotMode = 0;
				currDoc->scMW()->slotEditCopy();
				currDoc->view()->Deselect(true);
				for (int b = 0; b < nrOfCopies; b++)
				{
					currDoc->scMW()->slotEditPaste();
					double gx, gy, gh, gw;
					currItem = currDoc->m_Selection->itemAt(0);
					if (currDoc->m_Selection->count() == 1)
					{
						gx = currItem->xPos();
						gy = currItem->yPos();
						gw = currItem->width();
						gh = currItem->height();
					}
					else
						currDoc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
					for (int a = 0; a < currDoc->m_Selection->count(); ++a)
					{
						currItem = currDoc->m_Selection->itemAt(a);
						double deltaX = currItem->xPos() - gx;
						double deltaY = currItem->yPos() - gy;
						QMatrix matrixPre = QMatrix();
						QMatrix matrixAft = QMatrix();
						switch (baseP)
						{
							case 2:
								matrixPre.translate(-gw/2.0, -gh/2.0);
								matrixAft.translate(gw/2.0, gh/2.0);
								break;
							case 4:
								matrixPre.translate(-gw, -gh);
								matrixAft.translate(gw, gh);
								break;
							case 3:
								matrixPre.translate(0, -gh);
								matrixAft.translate(0, gh);
								break;
							case 1:
								matrixPre.translate(-gw, 0);
								matrixAft.translate(gw, 0);
								break;
						}
						currItem->PoLine.translate(deltaX, deltaY);
						currItem->PoLine.map(matrixPre);
						currItem->PoLine.map(comulatedMatrix);
						currItem->PoLine.map(matrixAft);
						currItem->PoLine.translate(-deltaX, -deltaY);
						currItem->ContourLine.translate(deltaX, deltaY);
						currItem->ContourLine.map(matrixPre);
						currItem->ContourLine.map(comulatedMatrix);
						currItem->ContourLine.map(matrixAft);
						currItem->ContourLine.translate(-deltaX, -deltaY);
						currItem->Frame = false;
						currItem->ClipEdited = true;
						currItem->FrameType = 3;
						currDoc->AdjustItemSize(currItem);
						Elements.append(currItem);
					}
					comulatedMatrix *= matrix;
				}
				for (int c = 0; c < Elements.count(); ++c)
				{
					currDoc->m_Selection->addItem(Elements.at(c), true);
				}
				currDoc->m_Selection->setGroupRect();
				currDoc->RotMode = rotBack;
				currDoc->useRaster = savedAlignGrid;
				currDoc->SnapGuides = savedAlignGuides;
				currDoc->DoDrawing = true;
				currDoc->m_Selection->delaySignalsOff();
				currDoc->view()->updatesOn(true);
				currDoc->scMW()->ScriptRunning = false;
				currDoc->m_Selection->connectItemToGUI();
			}
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			currDoc->view()->DrawNew();
			currDoc->changed();
		}
		delete dia;
	}
	return true;
}
