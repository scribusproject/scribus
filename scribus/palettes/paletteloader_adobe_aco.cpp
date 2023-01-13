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

#include "paletteloader_adobe_aco.h"
#include "sccolor.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"

bool PaletteLoader_Adobe_aco::isFileSupported(const QString & fileName) const
{
	QFileInfo fi(fileName);
	QString ext = fi.suffix().toLower();
	if (ext != "aco")
		return false;
	
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	
	QDataStream ts(&file);
	ts.setByteOrder(QDataStream::BigEndian);
	quint16 vers = 0;
	ts >> vers;
	file.close();
	
	return vers == 1;
}

bool PaletteLoader_Adobe_aco::importFile(const QString& fileName, bool /*merge*/)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	int oldCount = m_colors->count();

	ScColor lf;
	QDataStream ts(&file);
	ts.setByteOrder(QDataStream::BigEndian);
	quint16 vers = 0;
	ts >> vers;
	if (vers != 1)
	{
		file.close();
		return false;
	}

	quint16 count1 = 0;
	quint16 count2 = 0;
	ts >> count1;
	qint64 pos = ts.device()->pos();
	if (pos + count1 * 10 < file.size())
	{
		ts.skipRawData(count1 * 10);
		ts >> vers >> count2;
		if ((vers == 2) && (count1 == count2))
		{
			for (quint16 cc = 0; cc < count2; cc++)
			{
				quint16 colType;
				quint16 componentR, componentG, componentB, componentK;
				ts >> colType >> componentR >> componentG >> componentB >> componentK;
				QString name = readAdobeUniCodeString(ts);
				if (!name.isEmpty())
				{
					bool validColor = false;
					if (colType == 0)			// RBG
					{
						lf.setRgbColor(componentR >> 8, componentG >> 8, componentB >> 8);
						validColor = true;
					}
					else if (colType == 1)		// HSB
					{
						uchar hc = componentR >> 8;
						uchar sc = componentG >> 8;
						uchar bc = componentB >> 8;
						HSVTORGB(hc, sc, bc);
						lf.setRgbColor(hc, sc, bc);
						validColor = true;
					}
					else if (colType == 2)		// CMYK
					{
						lf.setColor(255 - (componentR >> 8), 255 - (componentG >> 8), 255 - (componentB >> 8), 255 - (componentK >> 8));
						validColor = true;
					}
					else if (colType == 8)		// Grayscale
					{
						lf.setColor(0, 0, 0, qRound((componentK / 10000.0) * 255));
						validColor = true;
					}
					if (validColor)
					{
						lf.setSpotColor(false);
						lf.setRegistrationColor(false);
						m_colors->tryAddColor(name, lf);
					}
				}
			}
		}
	}
	else
	{
		QFileInfo fileInfo(fileName);
		QString baseName = fileInfo.baseName();
		baseName.replace(" ", "_");
		for (quint16 cc = 0; cc < count1; cc++)
		{
			quint16 colType;
			quint16 componentR, componentG, componentB, componentK;
			ts >> colType >> componentR >> componentG >> componentB >> componentK;
			bool validColor = false;
			if (colType == 0)			// RBG
			{
				lf.setRgbColor(componentR >> 8, componentG >> 8, componentB >> 8);
				validColor = true;
			}
			else if (colType == 1)		// HSB
			{
				uchar hc = componentR >> 8;
				uchar sc = componentG >> 8;
				uchar bc = componentB >> 8;
				HSVTORGB(hc, sc, bc);
				lf.setRgbColor(hc, sc, bc);
				validColor = true;
			}
			else if (colType == 2)		// CMYK
			{
				lf.setColor(255 - (componentR >> 8), 255 - (componentG >> 8), 255 - (componentB >> 8), 255 - (componentK >> 8));
				validColor = true;
			}
			else if (colType == 8)		// Grayscale
			{
				lf.setColor(0, 0, 0, qRound((componentK / 10000.0) * 255));
				validColor = true;
			}
			if (validColor)
			{
				lf.setSpotColor(false);
				lf.setRegistrationColor(false);
				QString name = baseName+lf.name().toUpper();
				m_colors->tryAddColor(name, lf);
			}
		}
	}
	file.close();
	
	return (m_colors->count() != oldCount);
}
