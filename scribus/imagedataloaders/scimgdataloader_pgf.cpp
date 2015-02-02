/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scconfig.h"

#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QList>
#include "scimgdataloader_pgf.h"
#include "third_party/pgf/PGFimage.h"
#include "util.h"

// C Ansi includes
extern "C"
{
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
}

ScImgDataLoader_PGF::ScImgDataLoader_PGF(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_PGF::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "pgf" );
}

void ScImgDataLoader_PGF::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
}

bool ScImgDataLoader_PGF::loadPicture(const QString& fn, int /*page*/, int /*res*/, bool thumbnail)
{
	if (!QFile::exists(fn))
		return false;
	initialize();
#ifdef WIN32
	HANDLE fd = CreateFileW((LPCWSTR) fn.utf16(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (fd == INVALID_HANDLE_VALUE)
		return false;
#else
	int fd = open(QFile::encodeName(fn), O_RDONLY);
	if (fd == -1)
		return false;
#endif
	try
	{
		CPGFFileStream stream(fd);
		CPGFImage      pgfImg;
		pgfImg.Open(&stream);
		int level = 0;
/*
        const PGFHeader* header = pgfImg.GetHeader();
        qDebug() << "PGF width    = " << header->width;
        qDebug() << "PGF height   = " << header->height;
        qDebug() << "PGF bbp      = " << header->bpp;
        qDebug() << "PGF channels = " << header->channels;
        qDebug() << "PGF quality  = " << header->quality;
        qDebug() << "PGF mode     = " << header->mode;
        qDebug() << "PGF levels   = " << header->nLevels;
*/
		pgfImg.Read(level);
		if (pgfImg.BPP() > 32)
			return false;
		if (pgfImg.Channels() > 4)
			return false;
		if (pgfImg.Mode() == 2)
			return false;
		if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
		{
			if (pgfImg.Channels() == 1)
			{
				if (pgfImg.BPP() == 1)
				{
					m_image = QImage(pgfImg.Width(level), pgfImg.Height(level), QImage::Format_Mono);
					int map[] = {0};
					pgfImg.GetBitmap(m_image.bytesPerLine(), (UINT8*)m_image.bits(), m_image.depth(), map);
					m_image.invertPixels();
				}
				else if (pgfImg.BPP() == 8)
				{
					QByteArray data;
					data.resize(pgfImg.Width(level) * pgfImg.Height(level));
					int map[] = {0};
					pgfImg.GetBitmap(pgfImg.Width(level), (UINT8*)data.data(), 8, map);
					m_image = QImage(pgfImg.Width(level), pgfImg.Height(level), QImage::Format_ARGB32);
					int imgDcount = 0;
					for (uint y = 0; y < pgfImg.Height(level); y++)
					{
						QRgb *q = (QRgb*)(m_image.scanLine(y));
						for (uint x = 0; x < pgfImg.Width(level); x++)
						{
							uchar r = data[imgDcount++];
							*q++ = qRgba(r, r, r, 255);
						}
					}
				}
			}
			else if (pgfImg.Channels() == 3)
			{
				QByteArray data;
				data.resize(pgfImg.Width(level) * pgfImg.Height(level) * 3);
				int map[] = {0, 1, 2};
				pgfImg.GetBitmap(pgfImg.Width(level) * 3, (UINT8*)data.data(), 24, map);
				m_image = QImage(pgfImg.Width(level), pgfImg.Height(level), QImage::Format_ARGB32);
				int imgDcount = 0;
				for (uint y = 0; y < pgfImg.Height(); y++)
				{
					QRgb *q = (QRgb*)(m_image.scanLine(y));
					for (uint x = 0; x < pgfImg.Width(); x++)
					{
						uchar r = data[imgDcount++];
						uchar g = data[imgDcount++];
						uchar b = data[imgDcount++];
						*q++ = qRgba(r, g, b, 255);
					}
				}
			}
			else if (pgfImg.Channels() == 4)
			{
				m_image = QImage(pgfImg.Width(level), pgfImg.Height(level), QImage::Format_ARGB32);
				int map[] = {3, 2, 1, 0};
				pgfImg.GetBitmap(m_image.bytesPerLine(), (UINT8*)m_image.bits(), m_image.depth(), map);
			}
		}
		else
		{
			if (pgfImg.Channels() == 1)
			{
				if (pgfImg.BPP() == 1)
				{
					m_image = QImage(pgfImg.Width(level), pgfImg.Height(level), QImage::Format_Mono);
					int map[] = {0};
					pgfImg.GetBitmap(m_image.bytesPerLine(), (UINT8*)m_image.bits(), 1, map);
					m_image.invertPixels();
				}
				else if (pgfImg.BPP() == 8)
				{
					QByteArray data;
					data.resize(pgfImg.Width(level) * pgfImg.Height(level));
					int map[] = {0};
					pgfImg.GetBitmap(pgfImg.Width(level), (UINT8*)data.data(), 8, map);
					m_image = QImage(pgfImg.Width(level), pgfImg.Height(level), QImage::Format_ARGB32);
					int imgDcount = 0;
					for (uint y = 0; y < pgfImg.Height(level); y++)
					{
						QRgb *q = (QRgb*)(m_image.scanLine(y));
						for (uint x = 0; x < pgfImg.Width(level); x++)
						{
							uchar r = data[imgDcount++];
							*q++ = qRgba(r, r, r, 255);
						}
					}
				}
			}
			else if (pgfImg.Channels() == 3)
			{
				QByteArray data;
				data.resize(pgfImg.Width(level) * pgfImg.Height(level) * 3);
				int map[] = {2, 1, 0};
				pgfImg.GetBitmap(pgfImg.Width(level) * 3, (UINT8*)data.data(), 24, map);
				m_image = QImage(pgfImg.Width(level), pgfImg.Height(level), QImage::Format_ARGB32);
				int imgDcount = 0;
				for (uint y = 0; y < pgfImg.Height(level); y++)
				{
					QRgb *q = (QRgb*)(m_image.scanLine(y));
					for (uint x = 0; x < pgfImg.Width(level); x++)
					{
						uchar r = data[imgDcount++];
						uchar g = data[imgDcount++];
						uchar b = data[imgDcount++];
						*q++ = qRgba(r, g, b, 255);
					}
				}
			}
			else if (pgfImg.Channels() == 4)
			{
				m_image = QImage(pgfImg.Width(level), pgfImg.Height(level), QImage::Format_ARGB32);
				int map[] = {0, 1, 2, 3};
				pgfImg.GetBitmap(m_image.bytesPerLine(), (UINT8*)m_image.bits(), m_image.depth(), map);
			}
		}
		pgfImg.Close();
#ifdef WIN32
		CloseHandle(fd);
#else
		close(fd);
#endif
		m_imageInfoRecord.type = ImageTypeOther;
		m_imageInfoRecord.exifDataValid = false;
		float xres = m_image.dotsPerMeterX() * 0.0254;
		float yres = m_image.dotsPerMeterY() * 0.0254;
		if( xres <= 1.0 || xres > 3000.0 )
			xres = 72.0;
		if( yres <= 1.0 || yres > 3000.0 )
			yres = 72.0;
		int resInf = m_imageInfoRecord.lowResType;
		m_image = m_image.convertToFormat(QImage::Format_ARGB32);
		m_image.setDotsPerMeterX ((int) (xres / 0.0254));
		m_image.setDotsPerMeterY ((int) (yres / 0.0254));
		m_imageInfoRecord.colorspace = ColorSpaceRGB;
		m_imageInfoRecord.xres = qRound(xres);
		m_imageInfoRecord.yres = qRound(yres);
		m_imageInfoRecord.lowResType = resInf;
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = m_image.height();
		m_pixelFormat = Format_BGRA_8;
		return true;
	}
	catch(IOException& e)
	{
		return false;
	}
	return false;
}

bool ScImgDataLoader_PGF::preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha)
{
	if (loadPicture(fn, page, res, false))
	{
		hasAlpha = m_image.hasAlphaChannel();
		if (m_image.hasAlphaChannel())
			m_image = m_image.convertToFormat(QImage::Format_ARGB32);
		else
			m_image = QImage(); // Discard data immediately
		return true;
	}
	return false;
}
