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

#include "paletteloader_adobe_ase.h"
#include "sccolor.h"
#include "scribusdoc.h"
#include "sctextstream.h"
#include "util.h"
#include "util_formats.h"

bool PaletteLoader_Adobe_ase::isFileSupported(const QString & fileName) const
{
	QFileInfo fi(fileName);
	QString ext = fi.suffix().toLower();
	if (ext != "ase")
		return false;
	
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	
	QDataStream ts(&file);
	ts.setByteOrder(QDataStream::BigEndian);
	ts.setFloatingPointPrecision(QDataStream::SinglePrecision);
	quint16 vers1 = 0;
	quint16 vers2 = 0;
	quint32 signature;
	ts >> signature;
	ts >> vers1 >> vers2;
	file.close();
	
	return (signature == 0x41534546) && (vers1 == 1) && (vers2 == 0);
}

bool PaletteLoader_Adobe_ase::importFile(const QString& fileName, bool /*merge*/)
{
	QFileInfo fi(fileName);
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	int oldCount = m_colors->count();

	ScColor lf;
	QDataStream ts(&file);
	ts.setByteOrder(QDataStream::BigEndian);
	ts.setFloatingPointPrecision(QDataStream::SinglePrecision);
	quint16 vers1 = 0;
	quint16 vers2 = 0;
	quint32 signature;
	ts >> signature;
	ts >> vers1 >> vers2;
	if ((signature != 0x41534546) || (vers1 != 1) || (vers2 != 0))
	{
		file.close();
		return false;
	}

	QString blockName;
	quint32 numBlocks;
	ts >> numBlocks;
	for (quint32 n = 0; n < numBlocks; n++)
	{
		quint16 blockType;
		quint32 blockLen;
		ts >> blockType;
		ts >> blockLen;
		if (blockType == 0xC001)
			blockName = readAdobeUniCodeString16(ts);
		else if (blockType == 0x0001)
		{
			if (blockName.isEmpty())
				blockName = fi.baseName();
			QString Cname = readAdobeUniCodeString16(ts);
			quint32 clrType;
			quint16 spotMode;
			ts >> clrType;
			if (clrType == 0x52474220)		// RGB
			{
				float r, g, b;
				ts >> r >> g >> b;
				ts >> spotMode;
				lf.setRgbColorF(r, g, b);
				lf.setSpotColor(spotMode == 1);
				lf.setRegistrationColor(false);
				if (Cname.isEmpty())
				{
					Cname = blockName + QString("#%1%2%3").arg(qRound(255 * r),2,16,QChar('0')).arg(qRound(255 * g),2,16,QChar('0')).arg(qRound(255 * b),2,16,QChar('0')).toUpper();
					Cname.replace(" ","_");
				}
				m_colors->tryAddColor(Cname, lf);
			}
			else if (clrType == 0x434D594B)	// CMYK
			{
				float c, m, y, k;
				ts >> c >> m >> y >> k;
				ts >> spotMode;
				lf.setColorF(c, m, y, k);
				lf.setSpotColor(spotMode == 1);
				lf.setRegistrationColor(false);
				if (Cname.isEmpty())
				{
					Cname = blockName + QString("#%1%2%3%4").arg(qRound(255 * c),2,16,QChar('0')).arg(qRound(255 * m),2,16,QChar('0')).arg(qRound(255 * y),2,16,QChar('0')).arg(qRound(255 * k),2,16,QChar('0')).toUpper();
					Cname.replace(" ","_");
				}
				m_colors->tryAddColor(Cname, lf);
			}
			else if (clrType == 0x47726179)	// Gray
			{
				float g;
				ts >> g;
				ts >> spotMode;
				lf.setColorF(0, 0, 0, g);
				lf.setSpotColor(spotMode == 1);
				lf.setRegistrationColor(false);
				if (Cname.isEmpty())
				{
					Cname = blockName + QString("#000000%1").arg(qRound(255 * g),2,16,QChar('0')).toUpper();
					Cname.replace(" ","_");
				}
				m_colors->tryAddColor(Cname, lf);
			}
			else if (clrType == 0x4C414220) // Lab
			{
				float L, a, b;
				ts >> L >> a >> b;
				ts >> spotMode;
				lf.setLabColor(L * 100.0, a, b);
				lf.setSpotColor(spotMode == 1);
				lf.setRegistrationColor(false);
				if (Cname.isEmpty())
				{
					Cname = blockName + QString("_%1_%2_%3").arg(qRound(L * 100)).arg(qRound(a)).arg(qRound(b));
					Cname.replace(" ","_");
				}
				m_colors->tryAddColor(Cname, lf);
			}
		}
	}
	file.close();
	
	return (m_colors->count() != oldCount);
}
