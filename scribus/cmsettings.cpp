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

#include "cmsettings.h"
#include "scribusdoc.h"

CMSettings::CMSettings(ScribusDoc* doc, const QString& profileName) :
m_Doc(doc),
m_ProfileName(profileName)
{
}

CMSettings::~CMSettings()
{
}

bool CMSettings::useColorManagement() const
{
	if (m_Doc)
		return m_Doc->HasCMS;
	return false;
}

QString CMSettings::defaultMonitorProfile()
{
	if (m_Doc)
		return m_Doc->CMSSettings.DefaultMonitorProfile;
	return QString();
}

QString CMSettings::defaultPrinterProfile()
{
	if (m_Doc)
		return m_Doc->CMSSettings.DefaultPrinterProfile;
	return QString();
}

QString CMSettings::defaultImageRGBProfile()
{
	if (m_Doc)
		return m_Doc->CMSSettings.DefaultImageRGBProfile;
	return QString();
}

QString CMSettings::defaultImageCMYKProfile()
{
	if (m_Doc)
		return m_Doc->CMSSettings.DefaultImageCMYKProfile;
	return QString();
}

QString CMSettings::defaultSolidColorRGBProfile()
{
	if (m_Doc)
		return m_Doc->CMSSettings.DefaultSolidColorRGBProfile;
	return QString();
}

QString CMSettings::defaultSolidColorCMYKProfile()
{
	if (m_Doc)
		return m_Doc->CMSSettings.DefaultSolidColorCMYKProfile;
	return QString();
}

int CMSettings::colorRenderingIntent() const
{
	if (m_Doc)
		return m_Doc->IntentColors;
	return -1;
}

int CMSettings::imageRenderingIntent() const
{
	if (m_Doc)
		return m_Doc->IntentImages;
	return -1;
}

bool CMSettings::useBlackPoint() const
{
	if (m_Doc)
		return m_Doc->CMSSettings.BlackPoint;
	return false;
}

bool CMSettings::doSoftProofing() const
{
	if (m_Doc)
		return m_Doc->CMSSettings.SoftProofOn;
	return false;
}

bool CMSettings::doGamutCheck() const
{
	if (m_Doc)
		return m_Doc->CMSSettings.GamutCheck;
	return false;
}

#ifdef HAVE_CMS
cmsHPROFILE CMSettings::monitorProfile() const
{
	if (m_Doc->HasCMS)
		return m_Doc->DocOutputProf;
	return NULL;
}

cmsHPROFILE CMSettings::printerProfile() const
{
	if (m_Doc->HasCMS)
		return m_Doc->DocPrinterProf;
	return NULL;
}

cmsHTRANSFORM CMSettings::rgbColorDisplayTransform() const  // stdTransRGBMonG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdTransRGBMon;
	return NULL;
}

cmsHTRANSFORM CMSettings::rgbColorProofingTransform() const  // stdProofG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdProof;
	return NULL;
}

cmsHTRANSFORM CMSettings::rgbImageDisplayTransform() const   // stdTransImgG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdTransImg;
	return NULL;
}

cmsHTRANSFORM CMSettings::rgbImageProofingTransform() const  // stdProofImgG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdProofImg;
	return NULL;
}

cmsHTRANSFORM CMSettings::rgbToCymkColorTransform() const // stdTransCMYKG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdTransCMYK;
	return NULL;
}

cmsHTRANSFORM CMSettings::rgbGamutCheckTransform() const // stdProofGCG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdProofGC;
	return NULL;
}

cmsHTRANSFORM CMSettings::cmykColorDisplayTransform() const // stdTransCMYKMonG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdTransCMYKMon;
	return NULL;
}

cmsHTRANSFORM CMSettings::cmykColorProofingTransform() const // stdProofCMYKG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdProofCMYK;
	return NULL;
}

cmsHTRANSFORM CMSettings::cmykToRgbColorTransform() const  // stdTransRGBG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdTransRGB;
	return NULL;
}

cmsHTRANSFORM CMSettings::cmykGamutCheckTransform() const //stdProofCMYKGCG
{
	if (m_Doc->HasCMS)
		return m_Doc->stdProofCMYKGC;
	return NULL;
}

#endif
