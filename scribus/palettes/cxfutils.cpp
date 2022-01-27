/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "cxfutils.h"

CxfIlluminant qstrToCxfIlluminant(const QString& str)
{
	if (str == QLatin1String("A"))
		return cxfIlluminantA;

	if (str == QLatin1String("B"))
		return cxfIlluminantB;

	if (str == QLatin1String("C"))
		return cxfIlluminantC;

	if (str == QLatin1String("D50"))
		return cxfIlluminantD50;

	if (str == QLatin1String("D55"))
		return cxfIlluminantD55;

	if (str == QLatin1String("D60"))
		return cxfIlluminantD60;

	if (str == QLatin1String("D65"))
		return cxfIlluminantD65;

	if (str == QLatin1String("D75"))
		return cxfIlluminantD75;

	if (str == QLatin1String("E"))
		return cxfIlluminantE;

	if (str == QLatin1String("F2"))
		return cxfIlluminantF2;

	if (str == QLatin1String("F3"))
		return cxfIlluminantF3;

	if (str == QLatin1String("F7"))
		return cxfIlluminantF7;

	if (str == QLatin1String("F9"))
		return cxfIlluminantF9;

	if (str == QLatin1String("F10"))
		return cxfIlluminantF10;

	if (str == QLatin1String("F11"))
		return cxfIlluminantF11;

	if (str == QLatin1String("F12"))
		return cxfIlluminantF12;

	if (str == QLatin1String("9300"))
		return cxfIlluminant9300;

	if (str == QLatin1String("TL83"))
		return cxfIlluminantTL83;

	if (str == QLatin1String("TL84"))
		return cxfIlluminantTL84;

	if (str == QLatin1String("UL30"))
		return cxfIlluminantUL30;

	if (str == QLatin1String("UL35"))
		return cxfIlluminantUL35;

	if (str == QLatin1String("UL50"))
		return cxfIlluminantUL50;

	if (str == QLatin1String("Custom"))
		return cxfIlluminantCustom;

	return cxfIlluminantUnknown;
}

CxfObserver qstrToCxfObserver(const QString& str)
{
	if (str == QLatin1String("2_Degree"))
		return cxfObserver2_Degree;

	if (str == QLatin1String("10_Degree"))
		return cxfObserver10_Degree;

	if (str == QLatin1String("Custom_CMF"))
		return cxfObserverCustom_CMF;

	return cxfObserverUnknown;
}

CxfMeasurementType qstrToCxfMeasurementType(const QString& str)
{
	if (str == QLatin1String("Spectrum_Reflectance"))
		return cxfMeasurementSpectrum_Reflectance;

	if (str == QLatin1String("Spectrum_Transmittance"))
		return cxfMeasurementSpectrum_Transmittance;

	if (str == QLatin1String("Spectrum_TotalTransmittance"))
		return cxfMeasurementSpectrum_TotalTransmittance;

	if (str == QLatin1String("Spectrum_Emissive"))
		return cxfMeasurementSpectrum_Emissive;

	if (str == QLatin1String("Colorimetric_Reflectance"))
		return cxfMeasurementColorimetric_Reflectance;

	if (str == QLatin1String("Colorimetric_Transmittance"))
		return cxfMeasurementColorimetric_Transmittance;

	if (str == QLatin1String("Colorimetric_Emissive"))
		return cxfMeasurementColorimetric_Emissive;

	if (str == QLatin1String("Spectrum_Custom"))
		return cxfMeasurementSpectrum_Custom;

	return cxfMeasurementUnknown;
}

CxfCIEConversionMethod qstrToCxfCIEConversionMethod(const QString& str)
{
	if (str == QLatin1String("E308_Table5"))
		return cxfMethodE308_Table5;

	if (str == QLatin1String("E308_Table6"))
		return cxfMethodE308_Table6;

	if (str == QLatin1String("E308_1nm"))
		return cxfMethodE308_1nm;

	if (str == QLatin1String("unknown"))
		return cxfMethodUnknown;

	return cxfMethodUnknown;
}

CxfColorType qstrToCxfColorType(const QString& str)
{
	if (str == QLatin1String("ColorRGB"))
		return cxfColorRGB;

	if (str == QLatin1String("ColorHTML"))
		return cxfColorHTML;

	if (str == QLatin1String("ColorHSL"))
		return cxfColorHSL;

	if (str == QLatin1String("ColorSRGB"))
		return cxfColorSRGB;

	if (str == QLatin1String("ColorAdobeRGB"))
		return cxfColorAdobeRGB;

	if (str == QLatin1String("ColorCMYK"))
		return cxfColorCMYK;

	if (str == QLatin1String("ColorCMYKPlusN"))
		return cxfColorCMYKPlusN;

	if (str == QLatin1String("ColorCIELab"))
		return cxfColorCIELab;

	if (str == QLatin1String("ColorCIELCh"))
		return cxfColorCIELCh;

	if (str == QLatin1String("ColorCIEXYZ"))
		return cxfColorCIEXYZ;

	if (str == QLatin1String("ColorEmissiveCIEXYZ"))
		return cxfColorEmissiveCIEXYZ;

	if (str == QLatin1String("ColorCIExyY"))
		return cxfColorCIExyY;

	if (str == QLatin1String("ColorEmissiveCIExyY"))
		return cxfColorEmissiveCIExyY;

	if (str == QLatin1String("cxfColorCIELuv"))
		return cxfColorCIELuv;

	if (str == QLatin1String("ReflectanceSpectrum"))
		return cxfReflectanceSpectrum;

	if (str == QLatin1String("TransmittanceSpectrum"))
		return cxfTransmittanceSpectrum;

	if (str == QLatin1String("EmissiveSpectrum"))
		return cxfEmissiveSpectrum;

	return cxfColorUnknown;
}