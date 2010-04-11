/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolormgmtenginedata.h"
#include "sccolorspacedata_cmyk.h"
#include "sccolorspacedata_cmyka.h"
#include "sccolorspacedata_gray.h"
#include "sccolorspacedata_laba.h"
#include "sccolorspacedata_rgb.h"
#include "sccolorspacedata_rgba.h"

ScColorSpace ScColorMgmtEngineData::createColorSpace(ScColorProfile& profile, eColorFormat colorFormat)
{
	ScColorSpace colorSpace;
	eColorSpaceType profileCSpace = profile.colorSpace();
	if (profileCSpace == ColorSpace_Rgb)
	{
		if (colorFormat == Format_RGB_8)
			colorSpace = ScColorSpace(new ScColorSpaceData_RGB8(profile));
		else if (colorFormat == Format_RGB_16)
			colorSpace = ScColorSpace(new ScColorSpaceData_RGB16(profile));
		else if (colorFormat == Format_RGBA_8)
			colorSpace = ScColorSpace(new ScColorSpaceData_RGBA8(profile));
		else if (colorFormat == Format_RGBA_16)
			colorSpace = ScColorSpace(new ScColorSpaceData_RGBA16(profile));
		else if (colorFormat == Format_ARGB_8)
			colorSpace = ScColorSpace(new ScColorSpaceData_ARGB8(profile));
		else if (colorFormat == Format_ARGB_16)
			colorSpace = ScColorSpace(new ScColorSpaceData_ARGB16(profile));
		else if (colorFormat == Format_BGRA_8)
			colorSpace = ScColorSpace(new ScColorSpaceData_BGRA8(profile));
		else if (colorFormat == Format_BGRA_16)
			colorSpace = ScColorSpace(new ScColorSpaceData_BGRA16(profile));
	}
	else if (profileCSpace == ColorSpace_Cmyk)
	{
		if (colorFormat == Format_CMYK_8)
			colorSpace = ScColorSpace(new ScColorSpaceData_CMYK8(profile));
		else if (colorFormat == Format_CMYK_16)
			colorSpace = ScColorSpace(new ScColorSpaceData_CMYK16(profile));
		else if (colorFormat == Format_CMYKA_8)
			colorSpace = ScColorSpace(new ScColorSpaceData_CMYKA8(profile));
		else if (colorFormat == Format_CMYKA_16)
			colorSpace = ScColorSpace(new ScColorSpaceData_CMYKA16(profile));
		else if (colorFormat == Format_YMCK_8)
			colorSpace = ScColorSpace(new ScColorSpaceData_YMCK8(profile));
		else if (colorFormat == Format_YMCK_16)
			colorSpace = ScColorSpace(new ScColorSpaceData_YMCK16(profile));
	}
	else if (profileCSpace == ColorSpace_Gray)
	{
		if (colorFormat == Format_GRAY_8)
			colorSpace = ScColorSpace(new ScColorSpaceData_GRAY8(profile));
		else if (colorFormat == Format_GRAY_16)
			colorSpace = ScColorSpace(new ScColorSpaceData_GRAY16(profile));
	}
	else if (profileCSpace == ColorSpace_Lab)
	{
		if (colorFormat == Format_LabA_8)
			colorSpace = ScColorSpace(new ScColorSpaceData_LabA8(profile));
	}
	return colorSpace;
}
