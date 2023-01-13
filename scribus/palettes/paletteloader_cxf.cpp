/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QScopedPointer>

#include "lcms2.h"
#include "paletteloader_cxf.h"

#include "cxfcolor.h"
#include "cxfobject.h"
#include "sccolor.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "util.h"
#include "util_formats.h"

#include "colormgmt/sce308tables.h"
#include "ui/cxfimportdialog.h"

PaletteLoader_CxF::PaletteLoader_CxF()
				 : m_spectrumConvertor(ScE308Table5_D50_2deg())
{
	
}

bool PaletteLoader_CxF::isFileSupported(const QString & fileName) const
{
	QFileInfo fi(fileName);
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

QList<eColorSpaceType> PaletteLoader_CxF::getAvailableColorspaces() const
{
	bool importableAsRgb  = false;
	bool importableAsCmyk = false;
	bool importableAsLab  = false;

	int objectCount = m_cxfDoc.objectCount();
	for (int i = 0; i < objectCount; ++i)
	{
		const CxfObject* object = m_cxfDoc.objectAt(i);

		if (!importableAsRgb)
			importableAsRgb = canImportObjectAsRgb(object);
		if (!importableAsCmyk)
			importableAsCmyk = canImportObjectAsCmyk(object);
		if (!importableAsLab)
			importableAsLab = canImportObjectAsLab(object);

		if (importableAsRgb && importableAsCmyk && importableAsLab)
			break;
	}

	QList<eColorSpaceType> colorspaces;
	if (importableAsLab)
		colorspaces.append(ColorSpace_Lab);
	if (importableAsCmyk)
		colorspaces.append(ColorSpace_Cmyk);
	if (importableAsRgb)
		colorspaces.append(ColorSpace_Rgb);
	return colorspaces;
}

bool PaletteLoader_CxF::importFile(const QString& fileName, bool /*merge*/)
{
	if (!m_cxfDoc.parse(fileName))
		return false;

	QList<eColorSpaceType> colorspaces = getAvailableColorspaces();
	if (colorspaces.count() <= 0)
		return false;

	if (colorspaces.count() > 1)
	{
		QScopedPointer<CxfImportDialog> cxfImportDia(new CxfImportDialog(ScCore->primaryMainWindow()));
		cxfImportDia->setPriorities(colorspaces);
		if (cxfImportDia->exec() != QDialog::Accepted)
			return true;
		colorspaces = cxfImportDia->priorities();
	}

	QList<PaletteLoader_CxF::ColorImportFunction> importFunctions;
	for (int i = 0; i < colorspaces.count(); ++i)
	{
		eColorSpaceType colorspace = colorspaces.at(i);
		if (colorspace == ColorSpace_Rgb)
			importFunctions.append(&PaletteLoader_CxF::importObjectAsRgbColor);
		else if (colorspace == ColorSpace_Cmyk)
			importFunctions.append(&PaletteLoader_CxF::importObjectAsCmykColor);
		else if (colorspace == ColorSpace_Lab)
			importFunctions.append(&PaletteLoader_CxF::importObjectAsLabColor);
	}

	ColorImportFunction singleImportFunc = nullptr;
	if (importFunctions.count() == 1)
		singleImportFunc = importFunctions.at(0);

	int importCount = 0;
	int objectCount = m_cxfDoc.objectCount();

	for (int i = 0; i < objectCount; ++i)
	{
		const CxfObject* object = m_cxfDoc.objectAt(i);

		if (singleImportFunc)
		{
			if ((this->*singleImportFunc)(object))
				++importCount;
			continue;
		}

		for (int j = 0; j < importFunctions.count(); ++j)
		{
			ColorImportFunction importFunc = importFunctions.at(j);
			if ((this->*importFunc)(object))
			{
				++importCount;
				break;
			}	
		}
	}

	return (importCount > 0);
}

bool PaletteLoader_CxF::canImportObjectAsRgb(const CxfObject* object) const
{
	if (object->hasColor(cxfColorRGB))
		return true;
	if (object->hasColor(cxfColorSRGB))
		return true;
	if (object->hasColor(cxfColorAdobeRGB))
		return true;
	if (object->hasColor(cxfColorHTML))
		return true;
	return false;
}

bool PaletteLoader_CxF::canImportObjectAsCmyk(const CxfObject* object) const
{
	return object->hasColor(cxfColorCMYK);
}

bool PaletteLoader_CxF::canImportObjectAsLab(const CxfObject* object) const
{
	if (object->hasColor(cxfReflectanceSpectrum))
		return true;
	if (object->hasColor(cxfColorCIELab))
		return true;
	if (object->hasColor(cxfColorCIELCh))
		return true;
	if (object->hasColor(cxfColorCIEXYZ))
		return true;
	return false;
}

bool PaletteLoader_CxF::importObjectAsRgbColor(const CxfObject* object)
{
	ScColor color;

	const CxfColorRGB* cxfRgbColor = dynamic_cast<const CxfColorRGB*>(object->color(cxfColorRGB));
	if (cxfRgbColor)
	{
		double range = cxfRgbColor->maxRange();
		double red   = cxfRgbColor->red() / range;
		double green = cxfRgbColor->green() / range;
		double blue  = cxfRgbColor->blue() / range;
		color.setRgbColorF(red, green, blue);
		color.setSpotColor(false);
		color.setRegistrationColor(false);
		m_colors->tryAddColor(object->name(), color);
		return true;
	}

	const CxfColorSRGB* cxfsRgbColor = dynamic_cast<const CxfColorSRGB*>(object->color(cxfColorSRGB));
	if (cxfsRgbColor)
	{
		double range = cxfsRgbColor->maxRange();
		double red = cxfsRgbColor->red() / range;
		double green = cxfsRgbColor->green() / range;
		double blue = cxfsRgbColor->blue() / range;
		color.setRgbColorF(red, green, blue);
		color.setSpotColor(false);
		color.setRegistrationColor(false);
		m_colors->tryAddColor(object->name(), color);
		return true;
	}

	const CxfColorAdobeRGB* cxfAdobeRgbColor = dynamic_cast<const CxfColorAdobeRGB*>(object->color(cxfColorAdobeRGB));
	if (cxfAdobeRgbColor)
	{
		double range = cxfAdobeRgbColor->maxRange();
		double red = cxfAdobeRgbColor->red() / range;
		double green = cxfAdobeRgbColor->green() / range;
		double blue = cxfAdobeRgbColor->blue() / range;
		color.setRgbColorF(red, green, blue);
		color.setSpotColor(false);
		color.setRegistrationColor(false);
		m_colors->tryAddColor(object->name(), color);
		return true;
	}

	const CxfColorHTML* cxfHTMLColor = dynamic_cast<const CxfColorHTML*>(object->color(cxfColorHTML));
	if (cxfHTMLColor)
	{
		double red = cxfHTMLColor->red() / 255.0;
		double green = cxfHTMLColor->green() / 255.0;
		double blue = cxfHTMLColor->blue() / 255.0;
		color.setRgbColorF(red, green, blue);
		color.setSpotColor(false);
		color.setRegistrationColor(false);
		m_colors->tryAddColor(object->name(), color);
		return true;
	}

	return false;
}

bool PaletteLoader_CxF::importObjectAsCmykColor(const CxfObject* object)
{
	ScColor color;

	const CxfColorCMYK* cxfCmykColor = dynamic_cast<const CxfColorCMYK*>(object->color(cxfColorCMYK));
	if (cxfCmykColor)
	{
		double c = cxfCmykColor->cyan() / 100.0;
		double m = cxfCmykColor->magenta() / 100.0;
		double y = cxfCmykColor->yellow() / 100.0;
		double k = cxfCmykColor->black() / 100.0;
		color.setCmykColorF(c, m, y, k);
		color.setSpotColor(false);
		color.setRegistrationColor(false);
		m_colors->tryAddColor(object->name(), color);
		return true;
	}

	return false;
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
		const CxfColorSpecification* colorSpec = cxfLChColor->colorSpecification();
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
		const CxfColorSpecification* colorSpec = cxfXYZColor->colorSpecification();
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
