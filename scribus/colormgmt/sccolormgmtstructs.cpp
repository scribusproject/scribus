/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolormgmtstructs.h"

bool operator==(const ScColorTransformInfo& v1, const ScColorTransformInfo& v2)
{
	return ((v1.inputProfile  == v2.inputProfile) && 
	        (v1.outputProfile == v2.outputProfile) &&
	        (v1.proofingProfile == v2.proofingProfile) &&
	        (v1.inputFormat   == v2.inputFormat) &&
	        (v1.outputFormat  == v2.outputFormat) &&
	        (v1.renderIntent  == v2.renderIntent) &&
            (v1.proofingIntent  == v2.proofingIntent) &&
			(v1.flags  == v2.flags));
}

eColorType colorFormatType(eColorFormat format)
{
	eColorType type = Color_Unknown;
	switch (format)
	{
	case Format_RGB_8:
	case Format_RGB_16:
	case Format_RGBA_8:
	case Format_RGBA_16:
	case Format_ARGB_8:
	case Format_ARGB_16:
	case Format_BGRA_8:
	case Format_BGRA_16:
		type = Color_RGB;
		break;
	case Format_CMYK_8:
	case Format_CMYK_16:
	case Format_CMYKA_8:
	case Format_CMYKA_16:
	case Format_YMCK_8:
	case Format_YMCK_16:
		type = Color_CMYK;
		break;
	case Format_GRAY_8:
	case Format_GRAY_16:
		type = Color_Gray;
		break;
	case Format_LabA_8:
		type = Color_Lab;
		break;
	}
	return type;
}

uint colorFormatNumChannels(eColorFormat format)
{
	uint channels = 0;
	switch (format)
	{
	case Format_RGB_8:
	case Format_RGB_16:
		channels = 3;
		break;
	case Format_RGBA_8:
	case Format_RGBA_16:
	case Format_ARGB_8:
	case Format_ARGB_16:
	case Format_BGRA_8:
	case Format_BGRA_16:
	case Format_CMYK_8:
	case Format_CMYK_16:
		channels = 4;
		break;
	case Format_CMYKA_8:
	case Format_CMYKA_16:
		channels = 5;
		break;
	case Format_YMCK_8:
	case Format_YMCK_16:
		channels = 4;
		break;
	case Format_GRAY_8:
	case Format_GRAY_16:
		channels = 1;
		break;
	case Format_LabA_8:
		channels = 4;
		break;
	default: // For the pure sake of shutting gcc up
		channels = 0;
		break;
	}
	return channels;
}

uint colorFormatBytesPerChannel(eColorFormat format)
{
	uint bytes = 0;
	switch (format)
	{
	case Format_RGB_8:
	case Format_RGBA_8:
	case Format_ARGB_8:
	case Format_BGRA_8:
	case Format_CMYK_8:
	case Format_CMYKA_8:
	case Format_YMCK_8:
	case Format_GRAY_8:
	case Format_LabA_8:
		bytes = 1;
		break;
	case Format_RGB_16:
	case Format_RGBA_16:
	case Format_ARGB_16:
	case Format_BGRA_16:
	case Format_CMYK_16:
	case Format_CMYKA_16:
	case Format_YMCK_16:
	case Format_GRAY_16:
		bytes = 2;
		break;
	default: // For the pure sake of shutting gcc up
		bytes = 0;
		break;
	}
	return bytes;
}

bool colorFormatHasAlpha(eColorFormat format)
{
	bool hasAlpha = false;
	switch (format)
	{
	case Format_RGBA_8:
	case Format_RGBA_16:
	case Format_ARGB_8:
	case Format_ARGB_16:
	case Format_BGRA_8:
	case Format_BGRA_16:
	case Format_CMYKA_8:
	case Format_CMYKA_16:
	case Format_LabA_8:
		hasAlpha = true;
		break;
	default: // For the pure sake of shutting gcc up
		hasAlpha = false;
		break;
	}
	return hasAlpha;
}
