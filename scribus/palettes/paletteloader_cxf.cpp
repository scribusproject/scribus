/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "lcms2.h"
#include "paletteloader_cxf.h"

#include "cxfcolor.h"
#include "cxfobject.h"
#include "sccolor.h"
#include "scribusdoc.h"
#include "util.h"
#include "util_formats.h"

PaletteLoader_CxF::PaletteLoader_CxF()
                 : PaletteLoader(), m_spectrumConvertor(Illuminant_D50, Observer_2deg)
{

}

bool PaletteLoader_CxF::isFileSupported(const QString & fileName) const
{
	QFileInfo fi = QFileInfo(fileName);
	QString ext = fi.suffix().toLower();
	if (ext != "cxf")
		return false;
	
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	
	QByteArray bytes = file.read(1024);
	if (bytes.indexOf("<?xml") < 0)
		return false;
	return (bytes.indexOf("CxF") >= 0);
}

bool PaletteLoader_CxF::importFile(const QString& fileName, bool /*merge*/)
{
	if (!m_cxfDoc.parse(fileName))
		return false;

	int importCount = 0;
	int objectCount = m_cxfDoc.objectCount();

	for (int i = 0; i < objectCount; ++i)
	{
		const CxfObject* object = m_cxfDoc.objectAt(i);

		if (importObjectAsLabColor(object))
			++importCount;
	}

	return (importCount > 0);
}

bool PaletteLoader_CxF::importObjectAsLabColor(const CxfObject* object)
{
	ScColor color;

	const CxfReflectanceSpectrum* spectrum = dynamic_cast<const CxfReflectanceSpectrum*>(object->color(cxfReflectanceSpectrum));
	if (spectrum)
	{
		ScLab labVal = m_spectrumConvertor.toLab(spectrum->wavelengths(), spectrum->values());
		color.setLabColor(labVal.L, labVal.a, labVal.b);
		color.setSpotColor(false);
		color.setRegistrationColor(false);
		m_colors->tryAddColor(object->name(), color);
		return true;
	}

	const CxfColorCIELab* cxfLabColor = dynamic_cast<const CxfColorCIELab*>(object->color(cxfColorCIELab));
	if (cxfLabColor)
	{
		const CxfColorSpecification* colorSpec = cxfLabColor->colorSpecification();
		const CxfTristimulusSpec& tristimulusSpec = colorSpec->tristimulusSpec();
		if (tristimulusSpec.isD50_2degree())
		{
			color.setLabColor(cxfLabColor->L(), cxfLabColor->a(), cxfLabColor->b());
			color.setSpotColor(false);
			color.setRegistrationColor(false);
			m_colors->tryAddColor(object->name(), color);
			return true;
		}
	}

	const CxfColorCIELCh* cxfLChColor = dynamic_cast<const CxfColorCIELCh*>(object->color(cxfColorCIELCh));
	if (cxfLChColor)
	{
		const CxfColorSpecification* colorSpec = cxfLabColor->colorSpecification();
		const CxfTristimulusSpec& tristimulusSpec = colorSpec->tristimulusSpec();
		if (tristimulusSpec.isD50_2degree())
		{
			ScLab labVal = cxfLChColor->lab();
			color.setLabColor(labVal.L, labVal.a, labVal.b);
			color.setSpotColor(false);
			color.setRegistrationColor(false);
			m_colors->tryAddColor(object->name(), color);
			return true;
		}
	}

	const CxfColorCIEXYZ* cxfXYZColor = dynamic_cast<const CxfColorCIEXYZ*>(object->color(cxfColorCIEXYZ));
	if (cxfXYZColor)
	{
		const CxfColorSpecification* colorSpec = cxfLabColor->colorSpecification();
		const CxfTristimulusSpec& tristimulusSpec = colorSpec->tristimulusSpec();
		if (tristimulusSpec.isD50_2degree())
		{
			cmsCIELab cmsLab;
			cmsCIEXYZ cmsXYZ = { cxfXYZColor->X(),  cxfXYZColor->Y(), cxfXYZColor->Z() };
			cmsXYZ2Lab(cmsD50_XYZ(), &cmsLab, &cmsXYZ);
			color.setLabColor(cmsLab.L, cmsLab.a, cmsLab.b);
			color.setSpotColor(false);
			color.setRegistrationColor(false);
			m_colors->tryAddColor(object->name(), color);
			return true;
		}
	}

	return false;
}