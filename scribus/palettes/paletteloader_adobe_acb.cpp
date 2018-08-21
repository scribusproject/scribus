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

#include "paletteloader_adobe_acb.h"
#include "sccolor.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "util.h"
#include "util_formats.h"

PaletteLoader_Adobe_acb::PaletteLoader_Adobe_acb()
{

}

bool PaletteLoader_Adobe_acb::isFileSupported(const QString & fileName) const
{
	QFileInfo fi = QFileInfo(fileName);
	QString ext = fi.suffix().toLower();
	if (ext != "acb")
		return false;
	
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	
	quint16 vers = 0;
	quint32 signature;
	QDataStream ts(&file);
	ts.setByteOrder(QDataStream::BigEndian);
	ts >> signature;
	ts >> vers;
	file.close();
	
	return (signature == 0x38424342) && (vers == 1);
}

bool PaletteLoader_Adobe_acb::importFile(const QString& fileName, bool /*merge*/)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	int oldCount = m_colors->count();

	ScColor lf;
	quint16 vers = 0;
	quint32 signature;
	QDataStream ts(&file);
	ts.setByteOrder(QDataStream::BigEndian);
	ts >> signature;
	ts >> vers;
	if ((signature != 0x38424342) || (vers != 1))
	{
		file.close();
		return false;
	}

	quint16 vendor, numcolors, colType;
	ts >> vendor;
//	QString title		= readAdobeUniCodeString(ts);
//	QString prefix		= readAdobeUniCodeString(ts);
//	QString postfix		= readAdobeUniCodeString(ts);
//	QString description	= readAdobeUniCodeString(ts);
	ts >> numcolors;
	ts.skipRawData(4);
	ts >> colType;
	for (quint16 cc = 0; cc < numcolors; cc++)
	{
		QString name = readAdobeUniCodeString(ts);
		if (vendor == 3000)
			name.prepend("ANPA");
		else if (vendor == 3001)
			name.prepend("Focoltone");
		else if (vendor == 3002)
			name.prepend("PantoneCoated");
		else if (vendor == 3003)
			name.prepend("PantoneProcess");
		else if (vendor == 3004)
			name.prepend("PantoneProSlim");
		else if (vendor == 3005)
			name.prepend("PantoneUncoated");
		else if (vendor == 3006)
			name.prepend("Toyo");
		else if (vendor == 3007)
			name.prepend("Trumatch");
		else if (vendor == 3008)
			name.prepend("HKSE");
		else if (vendor == 3009)
			name.prepend("HKSK");
		else if (vendor == 3010)
			name.prepend("HKSN");
		else if (vendor == 3011)
			name.prepend("HKSZ");
		else if (vendor == 3012)
			name.prepend("DIC");
		else if (vendor == 3020)
			name.prepend("PantonePastelCoated");
		else if (vendor == 3021)
			name.prepend("PantonePastelUncoated");
		else if (vendor == 3022)
			name.prepend("PantoneMetallic");
		ts.skipRawData(6);
		quint8 componentR, componentG, componentB, componentK;
		ts >> componentR >> componentG >> componentB;
		if (colType == 2)
			ts >> componentK;
		if (!name.isEmpty())
		{
			bool validColor = false;
			if (colType == 0)			// RBG
			{
				lf.setRgbColor(componentR, componentG, componentB);
				validColor = true;
			}
			else if (colType == 2)		// CMYK
			{
				lf.setColor(255 - componentR, 255 - componentG, 255 - componentB, 255 - componentK);
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
	file.close();
	
	return (m_colors->count() != oldCount);
}
