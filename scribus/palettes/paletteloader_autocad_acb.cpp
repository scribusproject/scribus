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

#include "paletteloader_autocad_acb.h"
#include "sccolor.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "util.h"
#include "util_formats.h"

bool PaletteLoader_Autocad_acb::isFileSupported(const QString & fileName) const
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	
	QByteArray bytes = file.read(1024);
	return (bytes.indexOf("<?xml") >= 0);
}

bool PaletteLoader_Autocad_acb::importFile(const QString& fileName, bool /*merge*/)
{
	QByteArray docBytes;
	loadRawText(fileName, docBytes);
	QString docText = QString::fromUtf8(docBytes);
	QDomDocument docu("scridoc");
	if (!docu.setContent(docText))
		return false;

	ScColor lf;
	int oldCount = m_colors->count();

	QDomElement elem = docu.documentElement();
	QDomNode PAGE = elem.firstChild();
	while (!PAGE.isNull())
	{
		QDomElement pg = PAGE.toElement();
		if (pg.tagName() == "colorPage")
		{
			QDomNode colNode = pg.firstChild();
			while (!colNode.isNull())
			{
				QDomElement cg = colNode.toElement();
				if (cg.tagName() == "colorEntry")
				{
					int r (0), g(0), b(0);
					QString colorName = "";
					QDomNode colEntry = cg.firstChild();
					while (!colEntry.isNull())
					{
						QDomElement cc = colEntry.toElement();
						if (cc.tagName() == "colorName")
							colorName = cc.text();
						else if (cc.tagName() == "RGB8")
						{
							QDomNode colVal = cc.firstChild();
							while (!colVal.isNull())
							{
								QDomElement cv = colVal.toElement();
								if (cv.tagName() == "red")
									r = cv.text().toInt();
								else if (cv.tagName() == "green")
									g = cv.text().toInt();
								else if (cv.tagName() == "blue")
									b = cv.text().toInt();
								colVal = colVal.nextSibling();
							}
						}
						colEntry = colEntry.nextSibling();
					}
					if (!colorName.isEmpty())
					{
						lf.setRgbColor(r, g, b);
						lf.setSpotColor(false);
						lf.setRegistrationColor(false);
						m_colors->tryAddColor(colorName, lf);
					}
				}
				colNode = colNode.nextSibling();
			}
		}
		PAGE = PAGE.nextSibling();
	}
	
	return (m_colors->count() != oldCount);
}
