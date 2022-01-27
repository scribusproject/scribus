/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef CXFTYPES_H
#define CXFTYPES_H

enum CxfIlluminant
{
	cxfIlluminantUnknown,
	cxfIlluminantA,
	cxfIlluminantB,
	cxfIlluminantC,
	cxfIlluminantD50,
	cxfIlluminantD55,
	cxfIlluminantD60,
	cxfIlluminantD65,
	cxfIlluminantD75,
	cxfIlluminantE,
	cxfIlluminantF2,
	cxfIlluminantF3,
	cxfIlluminantF7,
	cxfIlluminantF9,
	cxfIlluminantF10,
	cxfIlluminantF11,
	cxfIlluminantF12,
	cxfIlluminant9300,
	cxfIlluminantTL84,
	cxfIlluminantTL83,
	cxfIlluminantUL30,
	cxfIlluminantUL35,
	cxfIlluminantUL50,
	cxfIlluminantCustom
};

enum CxfObserver
{
	cxfObserverUnknown,
	cxfObserver2_Degree,
	cxfObserver10_Degree,
	cxfObserverCustom_CMF
};

enum CxfMeasurementType
{
	cxfMeasurementUnknown,
	cxfMeasurementSpectrum_Reflectance,
	cxfMeasurementSpectrum_Transmittance,
	cxfMeasurementSpectrum_TotalTransmittance,
	cxfMeasurementSpectrum_Emissive,
	cxfMeasurementColorimetric_Reflectance,
	cxfMeasurementColorimetric_Transmittance,
	cxfMeasurementColorimetric_Emissive,
	cxfMeasurementSpectrum_Custom
};

enum CxfCIEConversionMethod
{
	cxfMethodUnknown,
	cxfMethodE308_Table5,
	cxfMethodE308_Table6,
	cxfMethodE308_1nm
};

enum CxfColorType
{
	cxfColorUnknown,
	cxfColorRGB,
	cxfColorHTML,
	cxfColorHSL,
	cxfColorSRGB,
	cxfColorAdobeRGB,
	cxfColorCMYK,
	cxfColorCMYKPlusN,
	cxfColorCIELab,
	cxfColorCIELCh,
	cxfColorCIEXYZ,
	cxfColorEmissiveCIEXYZ,
	cxfColorCIExyY,
	cxfColorEmissiveCIExyY,
	cxfColorCIELuv,
	cxfReflectanceSpectrum,
	cxfTransmittanceSpectrum,
	cxfEmissiveSpectrum
};

#endif
