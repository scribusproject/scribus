/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <cassert>

#include <QDataStream>
#include <QFile>
#include <QFileInfo>

#include "paletteloader_ps.h"
#include "sccolor.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "util.h"
#include "util_formats.h"

bool PaletteLoader_PS::isFileSupported(const QString & fileName) const
{
	QFileInfo fInfo(fileName);
	QString ext = fInfo.suffix().toLower();
	return extensionIndicatesEPSorPS(ext);
}

bool PaletteLoader_PS::importFile(const QString& fileName, bool /*merge*/)
{
	QString tmp, colorName;
	double c, m, y, k;
	ScColor cc;
	
	QFile f(fileName);
	if (!f.open(QIODevice::ReadOnly))
		return false;
	int oldCount = m_colors->count();

	bool isAtend = false;
	QDataStream ts(&f);
	while (!ts.atEnd())
	{
		tmp = readLineFromDataStream(ts);
		if ((tmp.startsWith("%%CMYKCustomColor")) || (tmp.startsWith("%%CMYKProcessColor")))
		{
			if (tmp.contains("(atend)"))
				isAtend = true;
			else
			{
				if (tmp.startsWith("%%CMYKCustomColor"))
					tmp = tmp.remove(0,18);
				else if (tmp.startsWith("%%CMYKProcessColor"))
					tmp = tmp.remove(0,19);
				ScTextStream ts2(&tmp, QIODevice::ReadOnly);
				ts2 >> c >> m >> y >> k;
				colorName = ts2.readAll();
				colorName = colorName.trimmed();
				colorName = colorName.remove(0,1);
				colorName = colorName.remove(colorName.length()-1,1);
				colorName = colorName.simplified();
				cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * y), qRound(255 * k));
				cc.setSpotColor(true);
				if (!colorName.isEmpty())
					m_colors->tryAddColor(colorName, cc);
				while (!ts.atEnd())
				{
					quint64 oldPos = ts.device()->pos();
					tmp = readLineFromDataStream(ts);
					if (!tmp.startsWith("%%+"))
					{
						ts.device()->seek(oldPos);
						break;
					}
					tmp = tmp.remove(0,3);
					ScTextStream ts2(&tmp, QIODevice::ReadOnly);
					ts2 >> c >> m >> y >> k;
					colorName = ts2.readAll();
					colorName = colorName.trimmed();
					colorName = colorName.remove(0,1);
					colorName = colorName.remove(colorName.length()-1,1);
					colorName = colorName.simplified();
					cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * y), qRound(255 * k));
					cc.setSpotColor(true);
					if (!colorName.isEmpty())
						m_colors->tryAddColor(colorName, cc);
				}
			}
		}
		if ((tmp.startsWith("%%RGBCustomColor")) || (tmp.startsWith("%%RGBProcessColor")))
		{
			if (tmp.contains("(atend)"))
				isAtend = true;
			else
			{
				if (tmp.startsWith("%%RGBCustomColor"))
					tmp = tmp.remove(0,17);
				else if (tmp.startsWith("%%RGBProcessColor"))
					tmp = tmp.remove(0,18);
				ScTextStream ts2(&tmp, QIODevice::ReadOnly);
				ts2 >> c >> m >> y;
				colorName = ts2.readAll();
				colorName = colorName.trimmed();
				colorName = colorName.remove(0,1);
				colorName = colorName.remove(colorName.length()-1,1);
				colorName = colorName.simplified();
				cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * y));
				if (!colorName.isEmpty())
					m_colors->tryAddColor(colorName, cc);
				while (!ts.atEnd())
				{
					quint64 oldPos = ts.device()->pos();
					tmp = readLineFromDataStream(ts);
					if (!tmp.startsWith("%%+"))
					{
						ts.device()->seek(oldPos);
						break;
					}
					tmp = tmp.remove(0,3);
					ScTextStream ts2(&tmp, QIODevice::ReadOnly);
					ts2 >> c >> m >> y;
					colorName = ts2.readAll();
					colorName = colorName.trimmed();
					colorName = colorName.remove(0,1);
					colorName = colorName.remove(colorName.length()-1,1);
					colorName = colorName.simplified();
					cc = ScColor(qRound(255 * c), qRound(255 * m), qRound(255 * y));
					if (!colorName.isEmpty())
						m_colors->tryAddColor(colorName, cc);
				}
			}
		}
		if (tmp.startsWith("%%EndComments"))
		{
			if (!isAtend)
				break;
		}
	}
	f.close();
	
	return (m_colors->count() != oldCount);
}
