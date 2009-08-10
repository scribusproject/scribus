/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2009 by Franz Schmid                                    *
*   franz.schmid@altmuehlnet.de                                           *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgUtil/Optimizer>
#include "osgeditor.h"
#include "commonstrings.h"
#include "sccolorengine.h"
#include "ui/customfdialog.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "units.h"
#include "util.h"
#include <QFileDialog>
#include <QInputDialog>

OSGEditorDialog::OSGEditorDialog(QWidget* parent, PageItem_OSGFrame *frame) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	currItem = frame;
	int wd = 300;
	int hd = 300;
	double asp = currItem->width() / currItem->height();
	if (asp > 1.0)
		hd = qRound(300 / asp);
	else
		wd = qRound(300 * asp);
	drawingarea->setFixedSize(wd, hd);
	drawingarea->resize(wd, hd);
	updateGeometry();
	viewMap = currItem->viewMap;
	currentViewName = currItem->currentView;
	currentView = viewMap[currentViewName];
	viewCombo->addItems(viewMap.keys());
	setCurrentComboItem(viewCombo, currentViewName);
	fovAngle->setValue(30.0);
	fovAngle->setNewUnit(SC_DEGREES);
	if (currItem->fillColor() != CommonStrings::None)
	{
		const ScColor& col = currItem->doc()->PageColors[currItem->fillColor()];
		QColor fillColor = ScColorEngine::getShadeColorProof(col, currItem->doc(), currItem->fillShade());
		drawingarea->getCamera()->setClearColor(osg::Vec4(fillColor.redF(), fillColor.greenF(), fillColor.blueF(), 0.0));
	}
	if (currItem->loadedModel)
	{
		loadedModel = currItem->loadedModel;
		modelFile = currItem->modelFile;
		renderStyleCombo->setCurrentIndex(currentView.rendermode);
		lightStyleCombo->setCurrentIndex(currentView.illumination);
		fovAngle->setValue(currentView.angleFOV);
		drawingarea->setCameraManipulator ( new osgGA::TrackballManipulator );
		drawingarea->setSceneData(loadedModel.get());
		osgGA::TrackballManipulator *trb = dynamic_cast<osgGA::TrackballManipulator*>(drawingarea->getCameraManipulator());
		trb->setByMatrix(currentView.trackerMatrix);
		trb->setCenter(currentView.trackerCenter);
		trb->setDistance(currentView.trackerDist);
		trb->setTrackballSize(currentView.trackerSize);
		drawingarea->updateTraversal();
		double fovy, aspectRatio, zNear, zFar;
		drawingarea->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
		drawingarea->getCamera()->setProjectionMatrixAsPerspective(currentView.angleFOV, aspectRatio, zNear, zFar);
		drawingarea->updateGL();
		connect(drawingarea, SIGNAL(mouseMoved()), this, SLOT(reportCamera()));
		connect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
		connect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));
		connect(buttonRemoveView, SIGNAL(clicked()), this, SLOT(removeView()));
		groupBox_5->setEnabled(true);
		buttonRemoveView->setEnabled(true);
		buttonAddView->setEnabled(true);
		clearButton->setEnabled(true);
	}
	else
	{
		groupBox_5->setEnabled(false);
		buttonRemoveView->setEnabled(false);
		buttonAddView->setEnabled(false);
		clearButton->setEnabled(false);
	}
	connect(fovAngle, SIGNAL(valueChanged(double)), this, SLOT(setCameraValues()));
	connect(loadButton, SIGNAL(clicked()), this, SLOT(openFile()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clearDisplay()));
	connect(renderStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRenderMode(int)));
	connect(lightStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLightMode(int)));
}

void OSGEditorDialog::clearDisplay()
{
	disconnect(fovAngle, SIGNAL(valueChanged(double)), this, SLOT(setCameraValues()));
	disconnect(renderStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRenderMode(int)));
	disconnect(lightStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLightMode(int)));
	disconnect(drawingarea, SIGNAL(mouseMoved()), this, SLOT(reportCamera()));
	disconnect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
	disconnect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));
	disconnect(buttonRemoveView, SIGNAL(clicked()), this, SLOT(removeView()));
	currentView.trackerCenter = osg::Vec3d();
	currentView.cameraPosition = osg::Vec3d();
	currentView.trackerMatrix = osg::Matrixd();
	drawingarea->setSceneData(0);
	loadedModel = 0;
	currentView.angleFOV = 30.0;
	fovAngle->setValue(currentView.angleFOV);
	currentView.illumination = PageItem_OSGFrame::Headlamp;
	currentView.rendermode = PageItem_OSGFrame::Solid;
	renderStyleCombo->setCurrentIndex(currentView.rendermode);
	lightStyleCombo->setCurrentIndex(currentView.illumination);
	modelFile = "";
	viewMap.clear();
	currentView.trackerDist = 0.0;
	currentView.trackerSize = 0.0;
	viewMap.insert( tr("Default"), currentView);
	currentViewName = tr("Default");
	viewCombo->addItems(viewMap.keys());
	setCurrentComboItem(viewCombo, currentViewName);
	groupBox_5->setEnabled(false);
	buttonRemoveView->setEnabled(false);
	buttonAddView->setEnabled(false);
	clearButton->setEnabled(false);
	connect(fovAngle, SIGNAL(valueChanged(double)), this, SLOT(setCameraValues()));
	connect(renderStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRenderMode(int)));
	connect(lightStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLightMode(int)));
}

void OSGEditorDialog::setCameraValues()
{
	drawingarea->getCamera()->setProjectionMatrixAsPerspective(fovAngle->value(), static_cast<double>(drawingarea->width()) / static_cast<double>(drawingarea->height()), 1.0f, 10000.0f );
	drawingarea->updateGL();
	currentView.angleFOV = fovAngle->value();
}

void OSGEditorDialog::changeView(QString viewName)
{
	disconnect(fovAngle, SIGNAL(valueChanged(double)), this, SLOT(setCameraValues()));
	disconnect(renderStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRenderMode(int)));
	disconnect(lightStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLightMode(int)));
	viewMap[currentViewName] = currentView;
	currentView = viewMap[viewName];
	currentViewName = viewName;
	renderStyleCombo->setCurrentIndex(currentView.rendermode);
	lightStyleCombo->setCurrentIndex(currentView.illumination);
	fovAngle->setValue(currentView.angleFOV);
	osgGA::TrackballManipulator *trb = dynamic_cast<osgGA::TrackballManipulator*>(drawingarea->getCameraManipulator());
	trb->setByMatrix(currentView.trackerMatrix);
	trb->setCenter(currentView.trackerCenter);
	trb->setDistance(currentView.trackerDist);
	trb->setTrackballSize(currentView.trackerSize);
	drawingarea->updateTraversal();
	double fovy, aspectRatio, zNear, zFar;
	drawingarea->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	drawingarea->getCamera()->setProjectionMatrixAsPerspective(currentView.angleFOV, aspectRatio, zNear, zFar);
	drawingarea->updateGL();
	connect(fovAngle, SIGNAL(valueChanged(double)), this, SLOT(setCameraValues()));
	connect(renderStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRenderMode(int)));
	connect(lightStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLightMode(int)));
}

void OSGEditorDialog::addView()
{
	disconnect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
	viewMap[currentViewName] = currentView;
	bool ok;
	QString newName = currentViewName;
	while (viewMap.contains(newName))
	{
		newName = QInputDialog::getText( this, tr("Add a new View"), tr("Name:"), QLineEdit::Normal, tr("New View"), &ok);
	}
	if (ok && !newName.isEmpty())
	{
		currentViewName = newName;
		viewMap.insert(currentViewName, currentView);
		viewCombo->addItem(currentViewName);
		setCurrentComboItem(viewCombo, currentViewName);
	}
	connect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
}

void OSGEditorDialog::removeView()
{
	disconnect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
	if (viewMap.size() > 1)
	{
		viewMap.remove(currentViewName);
		viewCombo->clear();
		QStringList vKey = viewMap.keys();
		viewCombo->addItems(vKey);
		currentViewName = vKey[0];
		setCurrentComboItem(viewCombo, currentViewName);
		currentView = viewMap[currentViewName];
		changeView(currentViewName);
	}
	connect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
}

void OSGEditorDialog::renameView(QString newName)
{
	viewMap.remove(currentViewName);
	currentViewName = newName;
	viewMap.insert(currentViewName, currentView);
}

void OSGEditorDialog::changeRenderMode(int mode)
{
	currentView.rendermode = static_cast<PageItem_OSGFrame::RenderType>(mode);
}

void OSGEditorDialog::changeLightMode(int mode)
{
	currentView.illumination = static_cast<PageItem_OSGFrame::LightType>(mode);
}

void OSGEditorDialog::reportCamera()
{
	disconnect(fovAngle, SIGNAL(valueChanged(double)), this, SLOT(setCameraValues()));
	osg::Vec3d vecEye, vecCenter, vecUp;
	drawingarea->getCamera()->getViewMatrixAsLookAt(vecEye, vecCenter, vecUp);
	currentView.cameraPosition = vecEye;
	currentView.cameraUp = vecUp;
	osgGA::TrackballManipulator *trb = dynamic_cast<osgGA::TrackballManipulator*>(drawingarea->getCameraManipulator());
	currentView.trackerCenter = trb->getCenter();
	currentView.trackerMatrix = trb->getMatrix();
	currentView.trackerDist = trb->getDistance();
	currentView.trackerSize = trb->getTrackballSize();
	double fovy, aspectRatio, zNear, zFar;
	drawingarea->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	currentView.angleFOV = fovy;
	fovAngle->setValue(currentView.angleFOV);
	connect(fovAngle, SIGNAL(valueChanged(double)), this, SLOT(setCameraValues()));
}

void OSGEditorDialog::openFile()
{
	QString fileName;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("models", ".");
	QString docexts("*.ac");
	QString filter = tr("All Supported Formats (%1);;All Files (*)").arg(docexts);
	CustomFDialog dia(this, wdir, tr("Import 3-D Model"), filter, fdHidePreviewCheckBox);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		loadedModel = osgDB::readNodeFile ( fileName.toStdString() );
		if (loadedModel)
		{
			osgUtil::Optimizer optimzer;
			optimzer.optimize(loadedModel);
			modelFile = fileName;
			drawingarea->setCameraManipulator ( new osgGA::TrackballManipulator );
			drawingarea->setSceneData(loadedModel.get());
			reportCamera();
			groupBox_5->setEnabled(true);
			buttonRemoveView->setEnabled(true);
			buttonAddView->setEnabled(true);
			clearButton->setEnabled(true);
			connect(drawingarea, SIGNAL(mouseMoved()), this, SLOT(reportCamera()));
			connect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
			connect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));
			connect(buttonRemoveView, SIGNAL(clicked()), this, SLOT(removeView()));
		}
		dirs->set("models", fileName.left(fileName.lastIndexOf("/")));
	}
}

void OSGEditorDialog::accept()
{
	if (loadedModel)
	{
		currItem->loadedModel = loadedModel;
		osg::Vec3d vecEye, vecCenter, vecUp;
		drawingarea->getCamera()->getViewMatrixAsLookAt(vecEye, vecCenter, vecUp);
		currentView.cameraPosition = vecEye;
		currentView.cameraUp = vecUp;
		osgGA::TrackballManipulator *trb = dynamic_cast<osgGA::TrackballManipulator*>(drawingarea->getCameraManipulator());
		currentView.trackerCenter = trb->getCenter();
		currentView.trackerMatrix = trb->getMatrix();
		currentView.trackerDist = trb->getDistance();
		currentView.trackerSize = trb->getTrackballSize();
		double fovy, aspectRatio, zNear, zFar;
		drawingarea->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
		currentView.angleFOV = fovy;
		currentView.illumination = static_cast<PageItem_OSGFrame::LightType>(lightStyleCombo->currentIndex());
		currentView.rendermode = static_cast<PageItem_OSGFrame::RenderType>(renderStyleCombo->currentIndex());
		currItem->modelFile = modelFile;
		QImage image = drawingarea->grabFrameBuffer();
		currItem->setImage(image);
		viewMap[currentViewName] = currentView;
		currItem->viewMap = viewMap;
		currItem->currentView = currentViewName;
	}
	else
	{
		currItem->clearContents();
	}
	QDialog::accept();
}
