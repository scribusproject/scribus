/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2006 by Craig Bradney                                   *
 *   mrb@scribus.info                                                      *
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

#ifndef CMSETTINGS_H
#define CMSETTINGS_H

#include "scconfig.h"
#include "scribusapi.h"
#include <qstring.h>
class ScribusDoc;

#ifdef HAVE_CMS
#include "lcms.h"
#endif

class SCRIBUS_API CMSettings
{
public:
	CMSettings(ScribusDoc* doc, const QString& profileName);
	~CMSettings();
	
	ScribusDoc* doc() const {return m_Doc;}
	QString profileName() const {return m_ProfileName;}

	bool useColorManagement() const;

	QString defaultMonitorProfile();
	QString defaultPrinterProfile();
	QString defaultImageRGBProfile();
	QString defaultImageCMYKProfile();
	QString defaultSolidColorRGBProfile();
	QString defaultSolidColorCMYKProfile();

	int colorRenderingIntent() const;
	int imageRenderingIntent() const;

	bool useBlackPoint() const;
	bool doSoftProofing() const;
	bool doGamutCheck() const;

#ifdef HAVE_CMS
	cmsHPROFILE monitorProfile() const;
	cmsHPROFILE printerProfile() const;

	cmsHTRANSFORM rgbColorDisplayTransform() const;   // stdTransRGBMonG
	cmsHTRANSFORM rgbColorProofingTransform() const;  // stdProofG
	cmsHTRANSFORM rgbImageDisplayTransform() const;   // stdTransImgG
	cmsHTRANSFORM rgbImageProofingTransform() const;  // stdProofImgG
	cmsHTRANSFORM rgbToCymkColorTransform() const;    // stdTransCMYKG
	cmsHTRANSFORM rgbGamutCheckTransform() const;     // stdProofGCG

	cmsHTRANSFORM cmykColorDisplayTransform() const;  // stdTransCMYKMonG
	cmsHTRANSFORM cmykColorProofingTransform() const; // stdProofCMYKG
	cmsHTRANSFORM cmykToRgbColorTransform() const;    // stdTransRGBG
	cmsHTRANSFORM cmykGamutCheckTransform() const;    //stdProofCMYKGCG
#endif

protected:
	ScribusDoc* m_Doc;
	QString m_ProfileName;

};

#endif
