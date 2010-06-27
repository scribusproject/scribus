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
#include <osgDB/ReadFile>
#include <osg/Material>
#include <osgUtil/Optimizer>

#include "scconfig.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <cstdlib>
#include <string>
#include "exportPRC.h"

void PRCExporter::getCurrentMaterial ( osg::Geode *geode )
{
	osg::StateAttribute* pRAP;
	osg::StateSet* theState = geode->getStateSet();
	if ( theState )
	{
		pRAP = theState->getAttribute ( osg::StateAttribute::MATERIAL );
		if ( pRAP != NULL )
		{
			osg::Material *material = dynamic_cast<osg::Material*> ( pRAP );
			if ( material != NULL )
			{
				const osg::Vec4& Diffuse = material->getDiffuse ( osg::Material::FRONT_AND_BACK );
				const osg::Vec4& Ambient = material->getAmbient ( osg::Material::FRONT_AND_BACK );
				const osg::Vec4& Emissive = material->getEmission ( osg::Material::FRONT_AND_BACK );
				const osg::Vec4& Specular = material->getSpecular ( osg::Material::FRONT_AND_BACK );
				RGBAColour amb = RGBAColour ( Ambient[0], Ambient[1], Ambient[2], Ambient[3] );
				RGBAColour dif = RGBAColour ( Diffuse[0], Diffuse[1], Diffuse[2], Diffuse[3] );
				RGBAColour emi = RGBAColour ( Emissive[0], Emissive[1], Emissive[2], Emissive[3] );
				RGBAColour spe = RGBAColour ( Specular[0], Specular[1], Specular[2], Specular[3] );
				currentMaterial = PRCmaterial ( amb, dif, emi, spe, Diffuse[3], material->getShininess ( osg::Material::FRONT_AND_BACK ) );
			}
		}
	}
}

void PRCExporter::getCurrentMaterial ( osg::Drawable *geode )
{
	osg::StateAttribute* pRAP;
	osg::StateSet* theState = geode->getStateSet();
	if ( theState )
	{
		pRAP = theState->getAttribute ( osg::StateAttribute::MATERIAL );
		if ( pRAP != NULL )
		{
			osg::Material *material = dynamic_cast<osg::Material*> ( pRAP );
			if ( material != NULL )
			{
				const osg::Vec4& Diffuse = material->getDiffuse ( osg::Material::FRONT_AND_BACK );
				const osg::Vec4& Ambient = material->getAmbient ( osg::Material::FRONT_AND_BACK );
				const osg::Vec4& Emissive = material->getEmission ( osg::Material::FRONT_AND_BACK );
				const osg::Vec4& Specular = material->getSpecular ( osg::Material::FRONT_AND_BACK );
				RGBAColour amb = RGBAColour ( Ambient[0], Ambient[1], Ambient[2], Ambient[3] );
				RGBAColour dif = RGBAColour ( Diffuse[0], Diffuse[1], Diffuse[2], Diffuse[3] );
				RGBAColour emi = RGBAColour ( Emissive[0], Emissive[1], Emissive[2], Emissive[3] );
				RGBAColour spe = RGBAColour ( Specular[0], Specular[1], Specular[2], Specular[3] );
				currentMaterial = PRCmaterial ( amb, dif, emi, spe, Diffuse[3], material->getShininess ( osg::Material::FRONT_AND_BACK ) );
			}
		}
	}
}

void PRCExporter::analyse ( osg::Node *nd, oPRCFile *out )
{
	/// here you have found a group.
	osg::Geode *geode = dynamic_cast<osg::Geode *> ( nd );
	if ( geode )
	{
		// analyse the geode. If it isnt a geode the dynamic cast gives NULL.
		QString nam = QString::fromStdString(geode->getName());
		out->begingroup(nam.toLatin1());
		analyseGeode ( geode, out );
		out->endgroup();
	}
	else
	{
		osg::Group *gp = dynamic_cast<osg::Group *> ( nd );
		if ( gp )
		{
			QString nam = QString::fromStdString(gp->getName());
			out->begingroup(nam.toLatin1());
//			osg::notify ( osg::WARN ) << "Group "<<  gp->getName() <<std::endl;
			for ( unsigned int ic=0; ic<gp->getNumChildren(); ic++ )
			{
				analyse ( gp->getChild ( ic ), out );
			}
			out->endgroup();
		}
//		else
//		{
//			osg::notify ( osg::WARN ) << "Unknown node "<<  nd <<std::endl;
//		}
	}
}
// divide the geode into its drawables and primitivesets:

void PRCExporter::analyseGeode ( osg::Geode *geode, oPRCFile *out )
{
	getCurrentMaterial ( geode );
	for ( unsigned int i=0; i<geode->getNumDrawables(); i++ )
	{
		osg::Drawable *drawable=geode->getDrawable ( i );
		osg::Geometry *geom=dynamic_cast<osg::Geometry *> ( drawable );
		getCurrentMaterial ( drawable );
		QString nam = QString::fromStdString(drawable->getName());
		out->begingroup(nam.toLatin1());
		for ( unsigned int ipr=0; ipr<geom->getNumPrimitiveSets(); ipr++ )
		{
			osg::PrimitiveSet* prset=geom->getPrimitiveSet ( ipr );
			analysePrimSet ( prset, out, geom, dynamic_cast<const osg::Vec3Array*> ( geom->getVertexArray() ) );
		}
		out->endgroup();
	}
}

void PRCExporter::analysePrimSet ( osg::PrimitiveSet*prset, oPRCFile *out, osg::Geometry *geom, const osg::Vec3Array *verts )
{
	unsigned int ic;
//	const osg::Vec4Array *Diffuse = dynamic_cast<const osg::Vec4Array*> ( geom->getColorArray() );
//	double r = ( *Diffuse ) [0].x();
//	double g = ( *Diffuse ) [0].y();
//	double b = ( *Diffuse ) [0].z();
//	double a = ( *Diffuse ) [0].w();
	// you might want to handle each type of primset differently: such as:
	switch ( prset->getMode() )
	{
		case osg::PrimitiveSet::TRIANGLES: // get vertices of triangle
//			osg::notify ( osg::WARN ) << "Triangles " << std::endl;
			for ( ic=0; ic<prset->getNumIndices()-2; ic+=3 )
			{
				double ( *points ) [3] = new double[4][3];
//				double *knotsU = new double[4];
//				double *knotsV = new double[4];
				double knotsU[] = {1,1,2,2};
				double knotsV[] = {1,1,2,2};
//				knotsU[0] = 1.0;
//				knotsU[1] = 1.0;
//				knotsU[2] = 2.0;
//				knotsU[3] = 2.0;
//				knotsV[0] = 1.0;
//				knotsV[1] = 1.0;
//				knotsV[2] = 2.0;
//				knotsV[3] = 2.0;
				points[0][0] = ( * verts ) [prset->index ( ic ) ].x();
				points[0][1] = ( * verts ) [prset->index ( ic ) ].y();
				points[0][2] = ( * verts ) [prset->index ( ic ) ].z();
				points[1][0] = ( * verts ) [prset->index ( ic+1 ) ].x();
				points[1][1] = ( * verts ) [prset->index ( ic+1 ) ].y();
				points[1][2] = ( * verts ) [prset->index ( ic+1 ) ].z();
				points[2][0] = ( * verts ) [prset->index ( ic+2 ) ].x();
				points[2][1] = ( * verts ) [prset->index ( ic+2 ) ].y();
				points[2][2] = ( * verts ) [prset->index ( ic+2 ) ].z();
				points[3][0] = ( * verts ) [prset->index ( ic+2 ) ].x();
				points[3][1] = ( * verts ) [prset->index ( ic+2 ) ].y();
				points[3][2] = ( * verts ) [prset->index ( ic+2 ) ].z();
//				out->addRectangle(points, currentMaterial);
				out->addSurface(1, 1, 2, 2, points, knotsU, knotsV, currentMaterial, NULL );
//				out->add ( new PRCsurface ( out, 1, 1, 2, 2, points, knotsU, knotsV, *new RGBAColour ( r,g,b,a ) ) );
			}
			break;
		case osg::PrimitiveSet::TRIANGLE_STRIP: // look up how tristrips are coded
//			osg::notify ( osg::WARN ) << "Triangle Strip " << std::endl;
			break;
		case osg::PrimitiveSet::QUADS: // get vertices of quad
//			osg::notify ( osg::WARN ) << "Quad " << std::endl;
			for ( ic=0; ic<prset->getNumIndices()-3; ic+=4 )
			{
				double ( *points ) [3] = new double[4][3];
//				double *knotsU = new double[4];
//				double *knotsV = new double[4];
				double knotsU[] = {1,1,2,2};
				double knotsV[] = {1,1,2,2};
//				knotsU[0] = 1.0;
//				knotsU[1] = 1.0;
//				knotsU[2] = 2.0;
//				knotsU[3] = 2.0;
//				knotsV[0] = 1.0;
//				knotsV[1] = 1.0;
//				knotsV[2] = 2.0;
//				knotsV[3] = 2.0;
				points[0][0] = ( * verts ) [prset->index ( ic ) ].x();
				points[0][1] = ( * verts ) [prset->index ( ic ) ].y();
				points[0][2] = ( * verts ) [prset->index ( ic ) ].z();
				points[1][0] = ( * verts ) [prset->index ( ic+1 ) ].x();
				points[1][1] = ( * verts ) [prset->index ( ic+1 ) ].y();
				points[1][2] = ( * verts ) [prset->index ( ic+1 ) ].z();
				points[2][0] = ( * verts ) [prset->index ( ic+3 ) ].x();
				points[2][1] = ( * verts ) [prset->index ( ic+3 ) ].y();
				points[2][2] = ( * verts ) [prset->index ( ic+3 ) ].z();
				points[3][0] = ( * verts ) [prset->index ( ic+2 ) ].x();
				points[3][1] = ( * verts ) [prset->index ( ic+2 ) ].y();
				points[3][2] = ( * verts ) [prset->index ( ic+2 ) ].z();
//				out->addRectangle(points, currentMaterial);
				out->addSurface(1, 1, 2, 2, points, knotsU, knotsV, currentMaterial, NULL );
//				out->add ( new PRCsurface ( out, 1, 1, 2, 2, points, knotsU, knotsV, *new RGBAColour ( r,g,b,a ) ) );
			}
			break;
		case osg::PrimitiveSet::QUAD_STRIP: // look up how tristrips are coded
//			osg::notify ( osg::WARN ) << "Quad Strip " << std::endl;
			break;
			// etc for all the primitive types you expect. EG quads, quadstrips lines line loops....
	}
}

void PRCExporter::convertFile ( QString fileName, PageItem_OSGFrame *frame )
{
	if ( !fileName.isEmpty() )
	{
		if ( frame->loadedModel )
		{
			oPRCFile oPRC ( fileName.toStdString() );
			analyse ( frame->loadedModel.get(), &oPRC );
			oPRC.finish();
		}
	}
}
