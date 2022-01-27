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

#include "paletteloader_sk1.h"
#include "sccolor.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "util.h"
#include "util_formats.h"

PaletteLoader_sK1::PaletteLoader_sK1()
{

}

bool PaletteLoader_sK1::isFileSupported(const QString & fileName) const
{
	QFileInfo fi = QFileInfo(fileName);
	QString ext = fi.suffix().toLower();
	if (ext != "skp")
		return false;
	
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	
	QByteArray bytes = file.read(1024);
	return (bytes.indexOf("<?xml") >= 0);
}

bool PaletteLoader_sK1::importFile(const QString& fileName, bool /*merge*/)
{
	QByteArray docBytes;
	loadRawText(fileName, docBytes);
	QString docText = QString::fromUtf8(docBytes);
	QDomDocument docu("scridoc");
	if (!docu.setContent(docText))
		return false;

	ScColor lf;
	bool isCMYK = false;
	int oldCount = m_colors->count();

	QDomElement elem = docu.documentElement();
	QDomNode page = elem.firstChild();
	while (!page.isNull())
	{
		QDomElement pg = page.toElement();
		if (pg.tagName() == "description")
		{
			if (pg.attribute("type", "") == "CMYK")
				isCMYK = true;
		}
		if (pg.tagName() == "color")
		{
			QString colorName;
			if (isCMYK)
			{
				double c, m, y, k;
				c = pg.attribute("c", "0").toDouble();
				m = pg.attribute("m", "0").toDouble();
				y = pg.attribute("y", "0").toDouble();
				k = pg.attribute("k", "0").toDouble();
				colorName = pg.attribute("name", "");
				if (!colorName.isEmpty())
				{
					lf.setColorF(c, m, y, k);
					lf.setSpotColor(false);
					lf.setRegistrationColor(false);
					m_colors->tryAddColor(colorName, lf);
				}
			}
			else
			{
				double r, g, b;
				r = pg.attribute("r", "0").toDouble();
				g = pg.attribute("g", "0").toDouble();
				b = pg.attribute("b", "0").toDouble();
				colorName = pg.attribute("name", "");
				if (!colorName.isEmpty())
				{
					lf.setRgbColorF(r, g, b);
					lf.setSpotColor(false);
					lf.setRegistrationColor(false);
					m_colors->tryAddColor(colorName, lf);
				}
			}
		}
		page = page.nextSibling();
	}
	
	return (m_colors->count() != oldCount);
}
