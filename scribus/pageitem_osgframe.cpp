/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						pageitem_osgframe.cpp  -  description
						-------------------
	begin                : Wed July 29 2009
	copyright            : (C) 2009 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "pageitem_osgframe.h"

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include <QDebug>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include "prefsmanager.h"
#include "scpainter.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "undostate.h"
#include "util.h"

PageItem_OSGFrame::PageItem_OSGFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
		: PageItem_ImageFrame(pa, x, y, w, h, w2, fill, outline)
{
	setUPixmap(Um::ILatexFrame);
	AnName = tr("OSG") + QString::number(m_Doc->TotalItems);
	setUName(AnName);
	struct viewDefinition defaultView;
	defaultView.trackerCenter = osg::Vec3d();
	defaultView.cameraPosition = osg::Vec3d();
	defaultView.cameraUp = osg::Vec3d();
	defaultView.trackerMatrix = osg::Matrixd();
	defaultView.angleFOV = 30.0;
	defaultView.trackerDist = 0.0;
	defaultView.trackerSize = 0.0;
	defaultView.illumination = Headlamp;
	defaultView.rendermode = Solid;
	defaultView.addedTransparency = 0.5;
	defaultView.colorAC = QColor(Qt::black);
	defaultView.colorFC = QColor(Qt::white);
	viewMap.insert( tr("Default"), defaultView);
	currentView = tr("Default");
	loadedModel = NULL;
	distanceToObj = 0.0;
	modelFile = "";
}

PageItem_OSGFrame::~PageItem_OSGFrame()
{
}

void PageItem_OSGFrame::setExternalModelFile(QString val)
{
	modelFile = val;
	if (!modelFile.isEmpty())
	{
		QString test = modelFile;
		if (QDir::isRelativePath(test))
			test = QDir::homePath()+"/"+modelFile;
		test = QDir::cleanPath(QDir::convertSeparators(test));
		QFileInfo pfi2(test);
		modelFile = pfi2.absoluteFilePath();
	}
}

void PageItem_OSGFrame::loadModel()
{
	if (!modelFile.isEmpty())
	{
		loadedModel = osgDB::readNodeFile ( modelFile.toStdString() );
		if (loadedModel)
		{
			osgUtil::Optimizer optimzer;
			optimzer.optimize(loadedModel);
		}
	}
}

void PageItem_OSGFrame::setImage(QImage &image)
{
	isInlineImage = true;
	if (PictureIsAvailable)
	{
		image.save(Pfile, "PNG");
		m_Doc->LoadPict(Pfile, ItemNr, true);
	}
	else
	{
		tempImageFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_osg_XXXXXX.png");
		tempImageFile->open();
		QString imgName = getLongPathName(tempImageFile->fileName());
		tempImageFile->close();
		image.save(imgName, "PNG");
		m_Doc->LoadPict(imgName, ItemNr);
	}
	setImageScalingMode(false, true);
}

void PageItem_OSGFrame::clearContents()
{
	viewMap.clear();
	struct viewDefinition defaultView;
	defaultView.trackerCenter = osg::Vec3d();
	defaultView.cameraPosition = osg::Vec3d();
	defaultView.cameraUp = osg::Vec3d();
	defaultView.trackerMatrix = osg::Matrixd();
	defaultView.angleFOV = 30.0;
	defaultView.trackerDist = 0.0;
	defaultView.trackerSize = 0.0;
	defaultView.illumination = Headlamp;
	defaultView.rendermode = Solid;
	defaultView.addedTransparency = 0.5;
	defaultView.colorAC = QColor(Qt::black);
	defaultView.colorFC = QColor(Qt::white);
	viewMap.insert( tr("Default"), defaultView);
	currentView = tr("Default");
	loadedModel = NULL;
	distanceToObj = 0.0;
	modelFile = "";
	PageItem_ImageFrame::clearContents();
}

void PageItem_OSGFrame::applicableActions(QStringList & actionList)
{
	actionList << "itemImageIsVisible";
	actionList << "editEditRenderSource";
	if (PictureIsAvailable)
	{
		actionList << "editClearContents";
	}
}

QString PageItem_OSGFrame::infoDescription()
{
	QString htmlText;
	htmlText.append( tr("3D Model") + "<br/>");
	QFileInfo fi = QFileInfo(modelFile);
	htmlText.append( tr("File:") + " " + fi.fileName() + "<br/>");
	htmlText.append(PageItem::infoDescription());
	return htmlText;
}

double PageItem_OSGFrame::getDistance(osg::Vec3d pos1, osg::Vec3d pos2)
{
	double dist2D = sqrt((pos1[0]-pos2[0])*(pos1[0]-pos2[0]) + (pos1[1]-pos2[1])*(pos1[1]-pos2[1]));
	return sqrt(dist2D*dist2D + (pos1[2]-pos2[2])*(pos1[2]-pos2[2]));
}

#define normalized(x, y, z)		\
{					\
	double modulo;			\
	modulo = sqrtf(x*x + y*y + z*z);	\
	if (modulo != 0.0)			\
	{					\
		x = x/modulo;			\
		y = y/modulo;			\
		z = z/modulo;			\
	}					\
}

QString PageItem_OSGFrame::getPDFMatrix(QString viewName)
{
	struct viewDefinition defaultView;
	defaultView = viewMap[viewName];
	osg::Vec3d trackerCenter = defaultView.trackerCenter;
	osg::Vec3d cameraPosition = defaultView.cameraPosition;
	osg::Vec3d cameraUp = defaultView.cameraUp;
	double viewx, viewy, viewz;
	double leftx, lefty, leftz;
	double upx, upy, upz;
	double transx, transy, transz;
	double roll = 0.0;
	double roo = getDistance(cameraPosition, trackerCenter);
	cameraPosition.normalize();
	cameraUp.normalize();
	viewx = -cameraPosition[0];
	viewy = -cameraPosition[1];
	viewz = -cameraPosition[2];
	if (viewx == 0.0 && viewy == 0.0 && viewz == 0.0)
	{
		viewy = 1.0;
	}
	leftx = -1.0f;
	lefty =  0.0f;
	leftz =  0.0f;
	if (viewz < 0.0) /* top view*/
	{
		upx = 0.0f;
		upy = 1.0f;
		upz = 0.0f;
	}
	else /* bottom view*/
	{
		upx = 0.0f;
		upy =-1.0f;
		upz = 0.0f;
	}
	if ( fabs(viewx) + fabs(viewy) != 0.0f) /* other views than top and bottom*/
	{
		cameraUp.normalize();
		upx = cameraUp[0];
		upy = cameraUp[1];
		upz = cameraUp[2];
		leftx = viewz*upy - viewy*upz;
		lefty = viewx*upz - viewz*upx;
		leftz = viewy*upx - viewx*upy;
		normalized(leftx, lefty, leftz);
	}
	/* apply camera roll*/
	{
		double leftxprime, leftyprime, leftzprime;
		double upxprime, upyprime, upzprime;
		double sinroll, cosroll;
		sinroll =  sin((roll/180.0f)*M_PI);
		cosroll =  cos((roll/180.0f)*M_PI);
		leftxprime = leftx*cosroll + upx*sinroll;
		leftyprime = lefty*cosroll + upy*sinroll;
		leftzprime = leftz*cosroll + upz*sinroll;
		upxprime = upx*cosroll + leftx*sinroll;
		upyprime = upy*cosroll + lefty*sinroll;
		upzprime = upz*cosroll + leftz*sinroll;
		leftx = leftxprime;
		lefty = leftyprime;
		leftz = leftzprime;
		upx = upxprime;
		upy = upyprime;
		upz = upzprime;
	}
	/* translation vector*/
	roo = fabs(roo);
	if (roo == 0.0)
	{
		roo = 0.000000000000000001;
	}
	transx = trackerCenter[0] - roo*viewx;
	transy = trackerCenter[1] - roo*viewy;
	transz = trackerCenter[2] - roo*viewz;
	if (fabs(leftx) < 0.0000001)
		leftx = 0.0;
	if (fabs(lefty) < 0.0000001)
		lefty = 0.0;
	if (fabs(leftz) < 0.0000001)
		leftz = 0.0;
	if (fabs(upx) < 0.0000001)
		upx = 0.0;
	if (fabs(upy) < 0.0000001)
		upy = 0.0;
	if (fabs(upz) < 0.0000001)
		upz = 0.0;
	if (fabs(transx) < 0.0000001)
		transx = 0.0;
	if (fabs(transy) < 0.0000001)
		transy = 0.0;
	if (fabs(transz) < 0.0000001)
		transz = 0.0;
	QString ret = "";
	QString desc4 = "%1 %2 %3 ";
	ret += desc4.arg(leftx).arg(lefty).arg(leftz);
	ret += desc4.arg(upx).arg(upy).arg(upz);
	ret += desc4.arg(viewx).arg(viewy).arg(viewz);
	ret += desc4.arg(transx).arg(transy).arg(transz);
	distanceToObj = roo;
	return ret;
}
