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
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osg/Point>
#include <osg/Material>
#include <osg/BlendFunc>
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
#include <QColorDialog>

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
	QPixmap pm(54, 14);
	pm.fill(currentView.colorAC);
	buttonACcolor->setIcon(pm);
	buttonACcolor->setText( QString::null );
	pm.fill(currentView.colorFC);
	buttonFCcolor->setIcon(pm);
	buttonFCcolor->setText( QString::null );
	transpSpin->setValue(qRound(currentView.addedTransparency * 100));
	rootnode = new osg::Group;
	decorator = new osg::Group;
	if (currItem->fillColor() != CommonStrings::None)
	{
		const ScColor& col = currItem->doc()->PageColors[currItem->fillColor()];
		QColor fillColor = ScColorEngine::getShadeColorProof(col, currItem->doc(), currItem->fillShade());
		drawingarea->getCamera()->setClearColor(osg::Vec4(fillColor.redF(), fillColor.greenF(), fillColor.blueF(), 0.0));
	}
	if (currItem->loadedModel)
	{
		loadedModel = currItem->loadedModel;
		default_stateset = loadedModel->getOrCreateStateSet();
		usedModel = dynamic_cast<osg::Node*>(loadedModel->clone(osg::CopyOp::DEEP_COPY_ALL));
		usedModel2 = dynamic_cast<osg::Node*>(loadedModel->clone(osg::CopyOp::DEEP_COPY_ALL));
		modelFile = currItem->modelFile;
		renderStyleCombo->setCurrentIndex(currentView.rendermode);
		lightStyleCombo->setCurrentIndex(currentView.illumination);
		fovAngle->setValue(currentView.angleFOV);
		transpSpin->setValue(qRound(currentView.addedTransparency * 100));
		drawingarea->setCameraManipulator ( new osgGA::TrackballManipulator );
		rootnode->addChild(usedModel);
		drawingarea->setSceneData(rootnode);
		osgGA::TrackballManipulator *trb = dynamic_cast<osgGA::TrackballManipulator*>(drawingarea->getCameraManipulator());
		trb->setByMatrix(currentView.trackerMatrix);
		trb->setCenter(currentView.trackerCenter);
		trb->setDistance(currentView.trackerDist);
		trb->setTrackballSize(currentView.trackerSize);
		drawingarea->updateTraversal();
		double fovy, aspectRatio, zNear, zFar;
		drawingarea->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
		drawingarea->getCamera()->setProjectionMatrixAsPerspective(currentView.angleFOV, aspectRatio, zNear, zFar);
		changeRenderMode(static_cast<int>(currentView.rendermode));
		drawingarea->updateGL();
		connect(drawingarea, SIGNAL(mouseMoved()), this, SLOT(reportCamera()));
		connect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
		connect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));
		connect(buttonRemoveView, SIGNAL(clicked()), this, SLOT(removeView()));
		connect(buttonACcolor, SIGNAL(clicked()), this, SLOT(changeACcolor()));
		connect(buttonFCcolor, SIGNAL(clicked()), this, SLOT(changeFCcolor()));
		connect(transpSpin, SIGNAL(valueChanged(int)), this, SLOT(changeTransparency(int)));
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
	rootnode->removeChild(usedModel);
	if (rootnode->getNumChildren() > 1)
	{
		rootnode->removeChild(decorator);
		decorator->removeChild(usedModel2);
	}
	drawingarea->setSceneData(0);
	loadedModel = 0;
	usedModel = 0;
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
	disconnect(transpSpin, SIGNAL(valueChanged(int)), this, SLOT(changeTransparency(int)));
	viewMap[currentViewName] = currentView;
	currentView = viewMap[viewName];
	currentViewName = viewName;
	renderStyleCombo->setCurrentIndex(currentView.rendermode);
	lightStyleCombo->setCurrentIndex(currentView.illumination);
	fovAngle->setValue(currentView.angleFOV);
	transpSpin->setValue(qRound(currentView.addedTransparency * 100));
	QPixmap pm(54, 14);
	pm.fill(currentView.colorAC);
	buttonACcolor->setIcon(pm);
	buttonACcolor->setText( QString::null );
	pm.fill(currentView.colorFC);
	buttonFCcolor->setIcon(pm);
	buttonFCcolor->setText( QString::null );
	osgGA::TrackballManipulator *trb = dynamic_cast<osgGA::TrackballManipulator*>(drawingarea->getCameraManipulator());
	trb->setByMatrix(currentView.trackerMatrix);
	trb->setCenter(currentView.trackerCenter);
	trb->setDistance(currentView.trackerDist);
	trb->setTrackballSize(currentView.trackerSize);
	drawingarea->updateTraversal();
	double fovy, aspectRatio, zNear, zFar;
	drawingarea->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	drawingarea->getCamera()->setProjectionMatrixAsPerspective(currentView.angleFOV, aspectRatio, zNear, zFar);
	changeRenderMode(static_cast<int>(currentView.rendermode));
	drawingarea->updateGL();
	connect(fovAngle, SIGNAL(valueChanged(double)), this, SLOT(setCameraValues()));
	connect(renderStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRenderMode(int)));
	connect(lightStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLightMode(int)));
	connect(transpSpin, SIGNAL(valueChanged(int)), this, SLOT(changeTransparency(int)));
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

void OSGEditorDialog::analyse(osg::Node *nd, double transparency)
{
	osg::Geode *geode = dynamic_cast<osg::Geode *> ( nd );
	if ( geode )
	{
		analyseGeode ( geode, transparency );
	}
	else
	{
		osg::Group *gp = dynamic_cast<osg::Group *> ( nd );
		if ( gp )
		{
			for ( unsigned int ic=0; ic<gp->getNumChildren(); ic++ )
			{
				analyse ( gp->getChild ( ic ), transparency );
			}
		}
	}
}

void OSGEditorDialog::analyseGeode(osg::Geode *geode, double transparency)
{
	osg::StateAttribute* pRAP;
	osg::StateSet* theState = geode->getStateSet();
	if (theState)
	{
		pRAP = theState->getAttribute(osg::StateAttribute::MATERIAL);
		if (pRAP != NULL)
		{
			osg::Material *material = dynamic_cast<osg::Material*>(pRAP);
			if (material != NULL)
			{
				material->setColorMode(osg::Material::OFF);
				material->setAlpha(osg::Material::FRONT_AND_BACK, transparency);
				osg::ref_ptr<osg::BlendFunc> bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
				theState->setAttributeAndModes(bf);
				theState->setMode(GL_BLEND,osg::StateAttribute::ON);
			}
		}
	}
	for ( unsigned int i=0; i<geode->getNumDrawables(); i++ )
	{
		osg::Drawable *drawable = geode->getDrawable ( i );
		if (drawable)
		{
			osg::StateSet* theState = drawable->getStateSet();
			if (theState)
			{
				pRAP = theState->getAttribute(osg::StateAttribute::MATERIAL);
				if (pRAP != NULL)
				{
					osg::Material *material = dynamic_cast<osg::Material*>(pRAP);
					if (material != NULL)
					{
						material->setColorMode(osg::Material::OFF);
						material->setAlpha(osg::Material::FRONT_AND_BACK, transparency);
					}
				}
			}
		}
	}
}

void OSGEditorDialog::changeRenderMode(int mode)
{
	osg::ref_ptr<osg::StateSet> stateset;
	osg::ref_ptr<osg::StateSet> stateset2;
	osg::ref_ptr<osg::Material> material;
	osg::ref_ptr<osg::Material> material2;
	osg::ref_ptr<osg::PolygonMode> polymode;
	osg::ref_ptr<osg::PolygonOffset> polyoffset;
	osg::ref_ptr<osg::Point> point;
	osg::ref_ptr<osg::BlendFunc> bf;
	osg::Vec4 colAC = osg::Vec4(currentView.colorAC.redF(), currentView.colorAC.greenF(), currentView.colorAC.blueF(), 1.0f);
	osg::Vec4 colFC = osg::Vec4(currentView.colorFC.redF(), currentView.colorFC.greenF(), currentView.colorFC.blueF(), 1.0f);
	currentView.rendermode = static_cast<PageItem_OSGFrame::RenderType>(mode);
	stateset = new osg::StateSet;
	rootnode->removeChild(usedModel);
	usedModel->releaseGLObjects();
	usedModel = dynamic_cast<osg::Node*>(loadedModel->clone(osg::CopyOp::DEEP_COPY_ALL));
	if (rootnode->getNumChildren() > 0)
	{
		rootnode->removeChild(decorator);
		decorator->removeChild(usedModel2);
		decorator->setStateSet(default_stateset);
		usedModel2->releaseGLObjects();
		usedModel2 = dynamic_cast<osg::Node*>(loadedModel->clone(osg::CopyOp::DEEP_COPY_ALL));
	}
	rootnode->addChild(usedModel);
	label->setEnabled(false);
	buttonACcolor->setEnabled(false);
	label_2->setEnabled(false);
	buttonFCcolor->setEnabled(false);
	label_3->setEnabled(false);
	transpSpin->setEnabled(false);
	switch (currentView.rendermode)
	{
		case PageItem_OSGFrame::Transparent:
			analyse(usedModel.get(), currentView.addedTransparency);
			stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
			bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
			stateset->setAttributeAndModes(bf);
			usedModel.get()->setStateSet(stateset);
			label_3->setEnabled(true);
			transpSpin->setEnabled(true);
			break;
		case PageItem_OSGFrame::Wireframe:
			label->setEnabled(true);
			buttonACcolor->setEnabled(true);
			polymode = new osg::PolygonMode;
			material = new osg::Material;
			polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
			stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
			material->setColorMode(osg::Material::OFF);
			material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setEmission(osg::Material::FRONT_AND_BACK, colAC);
			stateset->setAttributeAndModes(material,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			usedModel.get()->setStateSet(stateset);
			break;
		case PageItem_OSGFrame::ShadedWireframe:
			polymode = new osg::PolygonMode;
			polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
			stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
			stateset->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
			usedModel.get()->setStateSet(stateset);
			break;
		case PageItem_OSGFrame::Vertices:
			label->setEnabled(true);
			buttonACcolor->setEnabled(true);
			polymode = new osg::PolygonMode;
			material = new osg::Material;
			polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::POINT);
			stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			point = new osg::Point();
			point->setSize(3);
			stateset->setAttribute(point);
			stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
			material->setColorMode(osg::Material::OFF);
			material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setEmission(osg::Material::FRONT_AND_BACK, colAC);
			stateset->setAttributeAndModes(material,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			usedModel.get()->setStateSet(stateset);
			break;
		case PageItem_OSGFrame::ShadedVertices:
			polymode = new osg::PolygonMode;
			polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::POINT);
			stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			point = new osg::Point();
			point->setSize(3);
			stateset->setAttribute(point);
			stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
			stateset->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
			usedModel.get()->setStateSet(stateset);
			break;
		case PageItem_OSGFrame::Illustration:
			label->setEnabled(true);
			buttonACcolor->setEnabled(true);
			label_2->setEnabled(true);
			buttonFCcolor->setEnabled(true);
			polymode = new osg::PolygonMode;
			polyoffset = new osg::PolygonOffset;
			material = new osg::Material;
			polyoffset->setFactor(-1.0f);
			polyoffset->setUnits(-1.0f);
			polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
			stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setAttributeAndModes(polyoffset,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
			material->setColorMode(osg::Material::OFF);
			material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setEmission(osg::Material::FRONT_AND_BACK, colAC);
			stateset->setAttributeAndModes(material,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			rootnode->insertChild(0, decorator);
			stateset2 = new osg::StateSet;
			material2 = new osg::Material;
			material2->setColorMode(osg::Material::OFF);
			material2->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material2->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material2->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material2->setEmission(osg::Material::FRONT_AND_BACK, colFC);
			stateset2->setAttributeAndModes(material2,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset2->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			decorator->addChild(usedModel2);
			decorator->setStateSet(stateset2);
			usedModel.get()->setStateSet(stateset);
			break;
		case PageItem_OSGFrame::SolidWireframe:
		case PageItem_OSGFrame::SolidOutline:
		case PageItem_OSGFrame::TransparentWireframe:
			label->setEnabled(true);
			buttonACcolor->setEnabled(true);
			if (currentView.rendermode == PageItem_OSGFrame::TransparentWireframe)
			{
				label_3->setEnabled(true);
				transpSpin->setEnabled(true);
				analyse(usedModel2.get(), currentView.addedTransparency);
				stateset2 = new osg::StateSet;
				stateset2->setMode(GL_BLEND,osg::StateAttribute::ON);
				bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
				stateset2->setAttributeAndModes(bf);
				usedModel2.get()->setStateSet(stateset2);
			}
			polymode = new osg::PolygonMode;
			polyoffset = new osg::PolygonOffset;
			material = new osg::Material;
			polyoffset->setFactor(-1.0f);
			polyoffset->setUnits(-1.0f);
			polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
			stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setAttributeAndModes(polyoffset,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
			material->setColorMode(osg::Material::OFF);
			material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0,0.0f,0.0f,1.0f));
			material->setEmission(osg::Material::FRONT_AND_BACK, colAC);
			stateset->setAttributeAndModes(material,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			stateset->setMode(GL_LIGHTING,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
			rootnode->insertChild(0, decorator);
			decorator->addChild(usedModel2);
			usedModel.get()->setStateSet(stateset);
			break;
		default:
			stateset = default_stateset;
			usedModel.get()->setStateSet(stateset);
			break;
	}
}

void OSGEditorDialog::changeACcolor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(currentView.colorAC, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		currentView.colorAC = neu;
		buttonACcolor->setIcon(pm);
		changeRenderMode(static_cast<int>(currentView.rendermode));
	}
}

void OSGEditorDialog::changeFCcolor()
{
	QColor neu = QColor();
	neu = QColorDialog::getColor(currentView.colorAC, this);
	if (neu.isValid())
	{
		QPixmap pm(54, 14);
		pm.fill(neu);
		currentView.colorFC = neu;
		buttonFCcolor->setIcon(pm);
		changeRenderMode(static_cast<int>(currentView.rendermode));
	}
}

void OSGEditorDialog::changeTransparency(int value)
{
	currentView.addedTransparency = static_cast<double>(value) / 100.0;
	changeRenderMode(static_cast<int>(currentView.rendermode));
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
	QString docexts("*.ac *.osg");
	QString filter = tr("All Supported Formats (%1);;All Files (*)").arg(docexts);
	CustomFDialog dia(this, wdir, tr("Import 3-D Model"), filter, fdHidePreviewCheckBox);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (loadedModel)
	{
		if (rootnode->getNumChildren() > 1)
		{
			rootnode->removeChild(decorator);
			decorator->removeChild(usedModel2);
		}
		rootnode->removeChild(usedModel);
		drawingarea->setSceneData(0);
		loadedModel = 0;
		usedModel = 0;
		disconnect(drawingarea, SIGNAL(mouseMoved()), this, SLOT(reportCamera()));
		disconnect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
		disconnect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));
		disconnect(buttonRemoveView, SIGNAL(clicked()), this, SLOT(removeView()));
		disconnect(buttonACcolor, SIGNAL(clicked()), this, SLOT(changeACcolor()));
		disconnect(buttonFCcolor, SIGNAL(clicked()), this, SLOT(changeFCcolor()));
		disconnect(transpSpin, SIGNAL(valueChanged(int)), this, SLOT(changeTransparency(int)));
	}
	if (!fileName.isEmpty())
	{
		loadedModel = osgDB::readNodeFile ( fileName.toStdString() );
		if (loadedModel)
		{
			osgUtil::Optimizer optimzer;
			optimzer.optimize(loadedModel);
			modelFile = fileName;
			default_stateset = loadedModel->getOrCreateStateSet();
			usedModel = dynamic_cast<osg::Node*>(loadedModel->clone(osg::CopyOp::DEEP_COPY_ALL));
			usedModel2 = dynamic_cast<osg::Node*>(loadedModel->clone(osg::CopyOp::DEEP_COPY_ALL));
			drawingarea->setCameraManipulator ( new osgGA::TrackballManipulator );
			rootnode->setStateSet(default_stateset);
			rootnode->addChild(usedModel);
			drawingarea->setSceneData(rootnode);
			reportCamera();
			groupBox_5->setEnabled(true);
			buttonRemoveView->setEnabled(true);
			buttonAddView->setEnabled(true);
			clearButton->setEnabled(true);
			changeRenderMode(static_cast<int>(currentView.rendermode));
			connect(drawingarea, SIGNAL(mouseMoved()), this, SLOT(reportCamera()));
			connect(viewCombo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(changeView(QString)));
			connect(buttonAddView, SIGNAL(clicked()), this, SLOT(addView()));
			connect(buttonRemoveView, SIGNAL(clicked()), this, SLOT(removeView()));
			connect(buttonACcolor, SIGNAL(clicked()), this, SLOT(changeACcolor()));
			connect(buttonFCcolor, SIGNAL(clicked()), this, SLOT(changeFCcolor()));
			connect(transpSpin, SIGNAL(valueChanged(int)), this, SLOT(changeTransparency(int)));
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
