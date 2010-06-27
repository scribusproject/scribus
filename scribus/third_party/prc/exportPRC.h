/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#ifndef ADAPT_H
#define ADAPT_H

#include <QtCore>
#include <osg/Vec3>
#include <osg/Texture2D>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Vec4>
#include <cassert>
#include "oPRCFile.h"
#include "pageitem_osgframe.h"
/*
class prcfile : public oPRCFile
{
		std::list<PRCentity *> entities;
	public:
		prcfile ( std::string name ) : oPRCFile ( name.c_str() ) {}
		~prcfile()
		{
			for ( std::list<PRCentity *>::iterator p=entities.begin(); p != entities.end(); ++p )
			{
				assert ( *p );
				delete *p;
			}
		}

		void add ( PRCentity* e )
		{
			entities.push_back ( e );
			oPRCFile::add ( e );
		}
};
*/
class PRCExporter
{
	public:
		PRCExporter() {}
		virtual ~PRCExporter() {}
		void convertFile(QString fileName, PageItem_OSGFrame *frame);
	private:
		void analyse ( osg::Node *nd, oPRCFile *out );
		void getCurrentMaterial(osg::Geode *geode);
		void getCurrentMaterial(osg::Drawable *geode);
		void analyseGeode ( osg::Geode *geode, oPRCFile *out );
		void analysePrimSet ( osg::PrimitiveSet*prset, oPRCFile *out, osg::Geometry *geom, const osg::Vec3Array *verts );
		PRCmaterial currentMaterial;
};

#endif
