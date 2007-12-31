/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QFile>
#include <QFileInfo>
#include "scimgdataloader_gimp.h"

ScImgDataLoader_GIMP::ScImgDataLoader_GIMP(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_GIMP::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "pat" );
}

void ScImgDataLoader_GIMP::loadEmbeddedProfile(const QString& fn)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
}

bool ScImgDataLoader_GIMP::loadPicture(const QString& fn, int /*res*/, bool /*thumbnail*/)
{
	if (!QFile::exists(fn))
		return false;
	initialize();
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream s( &f );
		s.setByteOrder( QDataStream::BigEndian );
		uint headersize, versionInfo, patternWidth, patternHeight, patternType;
		uchar chData, alpha, r, g, b;
		uchar magicKey[4];
		s >> headersize;
		s >> versionInfo;
		s >> patternWidth;
		s >> patternHeight;
		s >> patternType;
		QString magic = "";
		for( int i = 0; i < 4; i++ )
		{
			s >> magicKey[i];
			magic += QChar(magicKey[i]);
		}
		if (magic != "GPAT")
			return false;
		s.device()->seek( headersize );
		m_image = QImage(patternWidth, patternHeight, QImage::Format_ARGB32);
		if (m_image.isNull())
			return false;
		QRgb *d;
		for (uint y = 0; y < patternHeight; y++)
		{
			d = (QRgb*)(m_image.scanLine( y ));
			if (patternType == 1)
			{
				for (uint x = 0; x < patternWidth; x++)
				{
					s >> chData;
					*d = qRgba(chData, chData, chData, 255);
					d++;
				}
			}
			else if (patternType == 2)
			{
				for (uint x = 0; x < patternWidth; x++)
				{
					s >> chData;
					s >> alpha;
					*d = qRgba(chData, chData, chData, alpha);
					d++;
				}
			}
			else if (patternType == 3)
			{
				for (uint x = 0; x < patternWidth; x++)
				{
					s >> r;
					s >> g;
					s >> b;
					*d = qRgba(r, g, b, 255);
					d++;
				}
			}
			else if (patternType == 4)
			{
				for (uint x = 0; x < patternWidth; x++)
				{
					s >> r;
					s >> g;
					s >> b;
					s >> alpha;
					*d = qRgba(r, g, b, alpha);
					d++;
				}
			}
		}
		m_imageInfoRecord.type = ImageTypeOther;
		m_imageInfoRecord.exifDataValid = false;
		m_imageInfoRecord.xres = 72;
		m_imageInfoRecord.yres = 72;
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.colorspace = ColorSpaceRGB;
		m_imageInfoRecord.BBoxH = m_image.height();
		return true;
	}
	return true;
}

void ScImgDataLoader_GIMP::preloadAlphaChannel(const QString& fn, int res)
{
	initialize();
	return;
}
