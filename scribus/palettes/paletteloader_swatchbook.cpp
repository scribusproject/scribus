/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <cassert>

#include <QByteArray>
#include <QDataStream>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QScopedPointer>

#include "paletteloader_swatchbook.h"
#include "sccolor.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "third_party/zip/scribus_zip.h"
#include "util.h"
#include "util_formats.h"

bool PaletteLoader_Swatchbook::isFileSupported(const QString & fileName) const
{
	QFileInfo fi(fileName);
	QString ext = fi.suffix().toLower();
	return ext == "sbz";
}

bool PaletteLoader_Swatchbook::importFile(const QString& fileName, bool /*merge*/)
{
	ScColor lf;
	int oldCount = m_colors->count();
	
	QScopedPointer<ScZipHandler> uz(new ScZipHandler());
	if (!uz->open(fileName))
		return false;
	if (!uz->contains("swatchbook.xml"))
		return false;
	
	QByteArray docBytes;
	if (!uz->read("swatchbook.xml", docBytes))
		return false;

	QString docText = QString::fromUtf8(docBytes);
	QDomDocument docu("scridoc");
	if (!docu.setContent(docText))
		return false;
	
	QDomElement docElem = docu.documentElement();
	for (QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() != "materials")
			continue;

		for (QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
		{
			if (spf.tagName() != "color")
				continue;

			bool isSpot = spf.attribute("usage") == "spot";
			QString colorName;
			ScColor tmp;
			tmp.setRegistrationColor(false);
			for (QDomElement spp = spf.firstChildElement(); !spp.isNull(); spp = spp.nextSiblingElement() )
			{
				if (spp.tagName() == "metadata")
				{
					for (QDomElement spm = spp.firstChildElement(); !spm.isNull(); spm = spm.nextSiblingElement() )
					{
						if (spm.tagName() == "dc:identifier")
							colorName = spm.text();
					}
				}
				else if (spp.tagName() == "values")
				{
					QString colorVals = spp.text();
					ScTextStream CoE(&colorVals, QIODevice::ReadOnly);
					if (spp.attribute("model") == "Lab")
					{
						double inC[3];
						CoE >> inC[0];
						CoE >> inC[1];
						CoE >> inC[2];
						tmp.setLabColor(inC[0], inC[1], inC[2]);
						tmp.setSpotColor(isSpot);
					}
					else if (spp.attribute("model") == "CMYK")
					{
						double c, m, y, k;
						CoE >> c >> m >> y >> k;
						tmp.setColorF(c, m, y, k);
						tmp.setSpotColor(isSpot);
					}
					else if (spp.attribute("model") == "RGB")
					{
						double r, g, b;
						CoE >> r >> g >> b;
						tmp.setRgbColorF(r, g, b);
						tmp.setSpotColor(false);
					}
				}
			}

			if (!colorName.isEmpty())
				m_colors->tryAddColor(colorName, tmp);
		}
	}
	
	return (m_colors->count() != oldCount);
}
