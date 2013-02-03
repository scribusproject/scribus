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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#ifndef CMSETTINGS_H
#define CMSETTINGS_H

#include "scconfig.h"
#include "scribusapi.h"
#include <QString>

#include "colormgmt/sccolorprofile.h"
#include "colormgmt/sccolortransform.h"
class ScribusDoc;

class SCRIBUS_API CMSettings
{
public:
	CMSettings(ScribusDoc* doc, const QString& profileName, eRenderIntent intent);
	~CMSettings();
	
	ScribusDoc* doc() const {return m_Doc;}
	QString profileName() const {return m_ProfileName;}
	QString outputProfile() const { return m_outputProfile; }
	eRenderIntent intent() const {return m_Intent;}

	bool useOutputProfile() const { return !m_outputProfile.isEmpty(); }
	void setOutputProfile(const QString& prof) { m_outputProfile = prof; }

	bool useColorManagement() const;

	QString defaultMonitorProfile() const;
	QString defaultPrinterProfile() const;
	QString defaultImageRGBProfile() const;
	QString defaultImageCMYKProfile() const;
	QString defaultSolidColorRGBProfile() const;
	QString defaultSolidColorCMYKProfile() const;

	eRenderIntent colorRenderingIntent() const;
	eRenderIntent imageRenderingIntent() const;

	bool useBlackPoint() const;
	bool doSoftProofing() const;
	bool doGamutCheck() const;

	ScColorProfile monitorProfile() const;
	ScColorProfile printerProfile() const;

	ScColorTransform rgbColorDisplayTransform() const;   // stdTransRGBMonG
	ScColorTransform rgbColorProofingTransform() const;  // stdProofG
	ScColorTransform rgbImageDisplayTransform() const;   // stdTransImgG
	ScColorTransform rgbImageProofingTransform() const;  // stdProofImgG
	ScColorTransform rgbToCymkColorTransform() const;    // stdTransCMYKG
	ScColorTransform rgbGamutCheckTransform() const;     // stdProofGCG

	ScColorTransform cmykColorDisplayTransform() const;  // stdTransCMYKMonG
	ScColorTransform cmykColorProofingTransform() const; // stdProofCMYKG
	ScColorTransform cmykImageProofingTransform() const; // stdProofImgCMYK
	ScColorTransform cmykToRgbColorTransform() const;    // stdTransRGBG
	ScColorTransform cmykGamutCheckTransform() const;    //stdProofCMYKGCG

protected:
	ScribusDoc* m_Doc;
	QString m_ProfileName;
	QString m_outputProfile;
	eRenderIntent m_Intent;

};

#endif
