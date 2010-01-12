/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						pageitem_osgframe.h  -  description
						-------------------
copyright            : Scribus Team
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef PAGEITEM_OSGFRAME_H
#define PAGEITEM_OSGFRAME_H

#include <osg/Node>
#include <osg/Matrixd>
#include <osg/Vec3>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QHash>

#include "scribusapi.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"


class SCRIBUS_API PageItem_OSGFrame : public PageItem_ImageFrame
{
	Q_OBJECT

	public:
		enum LightType
		{
			None		= 0,
			Headlamp	= 1,
			White		= 2,
			Day			= 3,
			Night		= 4,
			Hard		= 5,
			Primary		= 6,
			Blue		= 7,
			Red			= 8,
			Cube		= 9,
			CAD			= 10,
			Artwork		= 11
		};
		enum RenderType
		{
			Solid							= 0,
			SolidWireframe					= 1,
			Transparent						= 2,
			TransparentWireframe			= 3,
			BoundingBox						= 4,
			TransparentBoundingBox			= 5,
			TransparentBoundingBoxOutline	= 6,
			Wireframe						= 7,
			ShadedWireframe					= 8,
			HiddenWireframe					= 9,
			Vertices						= 10,
			ShadedVertices					= 11,
			Illustration					= 12,
			SolidOutline					= 13,
			ShadedIllustration				= 14
		};
		PageItem_OSGFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
		~PageItem_OSGFrame();
		
		virtual PageItem_OSGFrame * asOSGFrame() { return this; }
		virtual bool isOSGFrame() const { return true; }

		virtual ItemType realItemType() const { return PageItem::OSGFrame; }
		virtual void clearContents();
		void setImage(QImage &image);
		void setExternalModelFile(QString val);
		void loadModel();
		virtual void applicableActions(QStringList& actionList);
		virtual QString infoDescription();
		double getDistance(osg::Vec3d pos1, osg::Vec3d pos2);
		QString getPDFMatrix(QString viewName);

		osg::ref_ptr<osg::Node> loadedModel;
		struct viewDefinition
		{
			osg::Matrixd trackerMatrix;
			osg::Vec3d trackerCenter;
			osg::Vec3d cameraPosition;
			osg::Vec3d cameraUp;
			double angleFOV;
			double trackerDist;
			double trackerSize;
			LightType illumination;
			RenderType rendermode;
			double addedTransparency;
			QColor colorAC;
			QColor colorFC;
		};
		QHash<QString, viewDefinition> viewMap;
		QString currentView;
		double distanceToObj;
		QString modelFile;
};

#endif
