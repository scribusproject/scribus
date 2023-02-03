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

bool PaletteLoader_Adobe_acb::isFileSupported(const QString & fileName) const
{
	QFileInfo fi(fileName);
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

	QString vendorStr;
	if (vendor == 3000)
		vendorStr = "ANPA";
	else if (vendor == 3001)
		vendorStr = "Focoltone";
	else if (vendor == 3002)
		vendorStr = "PantoneCoated";
	else if (vendor == 3003)
		vendorStr = "PantoneProcess";
	else if (vendor == 3004)
		vendorStr = "PantoneProSlim";
	else if (vendor == 3005)
		vendorStr = "PantoneUncoated";
	else if (vendor == 3006)
		vendorStr = "Toyo";
	else if (vendor == 3007)
		vendorStr = "Trumatch";
	else if (vendor == 3008)
		vendorStr = "HKSE";
	else if (vendor == 3009)
		vendorStr = "HKSK";
	else if (vendor == 3010)
		vendorStr = "HKSN";
	else if (vendor == 3011)
		vendorStr = "HKSZ";
	else if (vendor == 3012)
		vendorStr = "DIC";
	else if (vendor == 3020)
		vendorStr = "PantonePastelCoated";
	else if (vendor == 3021)
		vendorStr = "PantonePastelUncoated";
	else if (vendor == 3022)
		vendorStr = "PantoneMetallic";

	QStringList metadata;
	for (int i = 0; i < 4; i++)
	{
		QString metadataString = readAdobeUniCodeString(ts);
		if (metadataString.startsWith("\""))
			metadataString = metadataString.remove(0, 1);
		if (metadataString.endsWith("\""))
			metadataString.chop(1);
		if (metadataString.startsWith("$$$/"))
		{
			if (metadataString.contains("="))
				metadataString = metadataString.split("=", Qt::KeepEmptyParts).last();
			else
				metadataString = QString();
		}
		metadata.append(metadataString);
	}
	//QString title		= metadata.at(0);
	QString prefix		= metadata.at(1);
	QString postfix		= metadata.at(2);
	//QString description	= metadata.at(3);

	ts >> numcolors;
	ts.skipRawData(4);
	ts >> colType;
	for (quint16 cc = 0; cc < numcolors; cc++)
	{
		QString name = readAdobeUniCodeString(ts);
		if (name.startsWith("$$$/"))
		{
			if (name.contains("="))
			{
				QString name2 = name.split("=", Qt::KeepEmptyParts).last();
				if (!name2.isEmpty())
					name = name2;
			}
		}

		if (!name.isEmpty() && !prefix.isEmpty())
			name = prefix + " " + name;
		else if (!name.isEmpty() && !vendorStr.isEmpty())
			name = vendorStr + " " + name;
		if (!name.isEmpty() && !postfix.isEmpty())
			name = name + " " + postfix;
		name = name.simplified();

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
			else if (colType == 7)		// Lab
			{
				double L = (componentR / 255.0) * 100.0;
				double a = -128.0 + componentG;
				double b = -128.0 + componentB;
				lf.setLabColor(L, a, b);
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
