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
		osg::Drawable *drawable = geode->getDrawable ( i );
		if (drawable)
		{
			osg::Geometry *geom = dynamic_cast<osg::Geometry *> ( drawable );
			if (geom)
			{
				getCurrentMaterial ( drawable );
				QString nam = QString::fromStdString(drawable->getName());
				PRCoptions grpopt;
				grpopt.no_break = true;
				grpopt.do_break = false;
				grpopt.tess = true;
				out->begingroup(nam.toLatin1(), &grpopt);
				for ( unsigned int ipr=0; ipr < geom->getNumPrimitiveSets(); ipr++ )
				{
					osg::PrimitiveSet* prset=geom->getPrimitiveSet ( ipr );
					analysePrimSet ( prset, out, geom, dynamic_cast<const osg::Vec3Array*> ( geom->getVertexArray() ) );
				}
				out->endgroup();
			}
		}
	}
}

void PRCExporter::analysePrimSet ( osg::PrimitiveSet*prset, oPRCFile *out, osg::Geometry *geom, const osg::Vec3Array *verts )
{
	unsigned int ic;
	unsigned int evenodd=0;
	switch ( prset->getMode() )
	{
		case osg::PrimitiveSet::TRIANGLES: // get vertices of triangle
			for ( ic=0; ic<prset->getNumIndices()-2; ic+=3 )
			{
				double ( *points ) [3] = new double[4][3];
				double knotsU[] = {1,1,2,2};
				double knotsV[] = {1,1,2,2};
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
				out->addSurface(1, 1, 2, 2, points, knotsU, knotsV, currentMaterial, NULL );
				delete[] points;
			}
			break;
		case osg::PrimitiveSet::TRIANGLE_STRIP: // look up how tristrips are coded
			if (prset->getType() == osg::PrimitiveSet::DrawArrayLengthsPrimitiveType)
			{
				const osg::DrawArrayLengths* drawArrayLengths = static_cast<const osg::DrawArrayLengths*>(prset);
				QList<uint> lenList;
				for(osg::DrawArrayLengths::const_iterator primItr = drawArrayLengths->begin(); primItr !=drawArrayLengths->end(); ++primItr)
				{
					unsigned int localPrimLength = *primItr;
					lenList.append(localPrimLength);
				}
				int lenInd = 0;
				int actLen = lenList[lenInd]-2;
				int currLen = 0;
				evenodd = 0;
				for(unsigned int vindex = 0; vindex < prset->getNumIndices()-2; ++vindex, evenodd++)
				{
					double ( *points ) [3] = new double[4][3];
					double knotsU[] = {1,1,2,2};
					double knotsV[] = {1,1,2,2};
					if (evenodd % 2 == 0)
					{
						points[0][0] = ( * verts ) [prset->index ( vindex ) ].x();
						points[0][1] = ( * verts ) [prset->index ( vindex ) ].y();
						points[0][2] = ( * verts ) [prset->index ( vindex ) ].z();
						points[1][0] = ( * verts ) [prset->index ( vindex+1 ) ].x();
						points[1][1] = ( * verts ) [prset->index ( vindex+1 ) ].y();
						points[1][2] = ( * verts ) [prset->index ( vindex+1 ) ].z();
					}
					else
					{
						points[0][0] = ( * verts ) [prset->index ( vindex+1 ) ].x();
						points[0][1] = ( * verts ) [prset->index ( vindex+1 ) ].y();
						points[0][2] = ( * verts ) [prset->index ( vindex+1 ) ].z();
						points[1][0] = ( * verts ) [prset->index ( vindex ) ].x();
						points[1][1] = ( * verts ) [prset->index ( vindex ) ].y();
						points[1][2] = ( * verts ) [prset->index ( vindex ) ].z();
					}
					points[2][0] = ( * verts ) [prset->index ( vindex+2 ) ].x();
					points[2][1] = ( * verts ) [prset->index ( vindex+2 ) ].y();
					points[2][2] = ( * verts ) [prset->index ( vindex+2 ) ].z();
					points[3][0] = ( * verts ) [prset->index ( vindex+2 ) ].x();
					points[3][1] = ( * verts ) [prset->index ( vindex+2 ) ].y();
					points[3][2] = ( * verts ) [prset->index ( vindex+2 ) ].z();
					out->addSurface(1, 1, 2, 2, points, knotsU, knotsV, currentMaterial, NULL );
					currLen++;
					if (currLen >= actLen)
					{
						lenInd++;
						if (lenInd == lenList.count())
							break;
						actLen = lenList[lenInd]-2;
						currLen = 0;
						vindex += 2;
						evenodd = 0;
						if (vindex >= prset->getNumIndices())
							break;
					}
					delete[] points;
				}
			}
			else
			{
				evenodd = 0;
				for(unsigned int vindex = 0; vindex < prset->getNumIndices()-2; ++vindex, evenodd++)
				{
					double ( *points ) [3] = new double[4][3];
					double knotsU[] = {1,1,2,2};
					double knotsV[] = {1,1,2,2};
					if (evenodd % 2 == 0)
					{
						points[0][0] = ( * verts ) [prset->index ( vindex ) ].x();
						points[0][1] = ( * verts ) [prset->index ( vindex ) ].y();
						points[0][2] = ( * verts ) [prset->index ( vindex ) ].z();
						points[1][0] = ( * verts ) [prset->index ( vindex+1 ) ].x();
						points[1][1] = ( * verts ) [prset->index ( vindex+1 ) ].y();
						points[1][2] = ( * verts ) [prset->index ( vindex+1 ) ].z();
					}
					else
					{
						points[0][0] = ( * verts ) [prset->index ( vindex+1 ) ].x();
						points[0][1] = ( * verts ) [prset->index ( vindex+1 ) ].y();
						points[0][2] = ( * verts ) [prset->index ( vindex+1 ) ].z();
						points[1][0] = ( * verts ) [prset->index ( vindex ) ].x();
						points[1][1] = ( * verts ) [prset->index ( vindex ) ].y();
						points[1][2] = ( * verts ) [prset->index ( vindex ) ].z();
					}
					points[2][0] = ( * verts ) [prset->index ( vindex+2 ) ].x();
					points[2][1] = ( * verts ) [prset->index ( vindex+2 ) ].y();
					points[2][2] = ( * verts ) [prset->index ( vindex+2 ) ].z();
					points[3][0] = ( * verts ) [prset->index ( vindex+2 ) ].x();
					points[3][1] = ( * verts ) [prset->index ( vindex+2 ) ].y();
					points[3][2] = ( * verts ) [prset->index ( vindex+2 ) ].z();
					out->addSurface(1, 1, 2, 2, points, knotsU, knotsV, currentMaterial, NULL );
					delete[] points;
				}
			}
			break;
		case osg::PrimitiveSet::QUADS: // get vertices of quad
			for ( ic=0; ic<prset->getNumIndices()-3; ic+=4 )
			{
				double ( *points ) [3] = new double[4][3];
				double knotsU[] = {1,1,2,2};
				double knotsV[] = {1,1,2,2};
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
				out->addSurface(1, 1, 2, 2, points, knotsU, knotsV, currentMaterial, NULL );
				delete[] points;
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
