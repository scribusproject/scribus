/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QFile>
#include <QFileInfo>

#include "util_ghostscript.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scimgdataloader_pdf.h"

ScImgDataLoader_PDF::ScImgDataLoader_PDF(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_PDF::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "pdf" );
}

void ScImgDataLoader_PDF::loadEmbeddedProfile(const QString& fn)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
}

bool ScImgDataLoader_PDF::loadPicture(const QString& fn, int gsRes, bool /*thumbnail*/)
{
	QStringList args;
	if (!QFile::exists(fn))
		return false;
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.png");
	QString picFile = QDir::convertSeparators(fn);
	float xres = gsRes;
	float yres = gsRes;

	initialize();
	m_imageInfoRecord.type = ImageTypePDF;
	m_imageInfoRecord.exifDataValid = false;
	args.append("-r"+QString::number(gsRes));
	args.append("-sOutputFile="+tmpFile);
	args.append("-dFirstPage=1");
	args.append("-dLastPage=1");
	args.append(picFile);
	int retg = callGS(args);
	if (retg == 0)
	{
		m_image.load(tmpFile);
		QFile::remove(tmpFile);
		if (!ScCore->havePNGAlpha())
		{
			for( int yi=0; yi < m_image.height(); ++yi )
			{
				QRgb *s = (QRgb*)(m_image.scanLine( yi ));
				for(int xi=0; xi < m_image.width(); ++xi )
				{
					if((*s) == 0xffffffff)
						(*s) &= 0x00ffffff;
					s++;
				}
			}
		}
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = m_image.height();
		m_imageInfoRecord.xres = qRound(gsRes);
		m_imageInfoRecord.yres = qRound(gsRes);
		m_imageInfoRecord.colorspace = ColorSpaceRGB;
		m_image.setDotsPerMeterX ((int) (xres / 0.0254));
		m_image.setDotsPerMeterY ((int) (yres / 0.0254));
		return true;
	}
	return false;
}

void ScImgDataLoader_PDF::preloadAlphaChannel(const QString& fn, int gsRes)
{
	float xres, yres;
//	short resolutionunit = 0;
	initialize();
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return;
	QString tmp, BBox;
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.png");
	QString picFile = QDir::convertSeparators(fn);
	QStringList args;
	xres = gsRes;
	yres = gsRes;
	args.append("-r"+QString::number(gsRes));
//	args.append("-sOutputFile=\""+tmpFile + "\"");
	args.append("-sOutputFile="+tmpFile);
	args.append("-dFirstPage=1");
	args.append("-dLastPage=1");
//	args.append("\""+picFile+"\"");
	args.append(picFile);
	int retg = callGS(args);
	if (retg == 0)
	{
		m_image.load(tmpFile);
		QFile::remove(tmpFile);
		if (!ScCore->havePNGAlpha())
		{
			QRgb *s;
			for( int yi=0; yi < m_image.height(); ++yi )
			{
				s = (QRgb*)(m_image.scanLine( yi ));
				for(int xi=0; xi < m_image.width(); ++xi )
				{
					if((*s) == 0xffffffff)
						(*s) &= 0x00ffffff;
					s++;
				}
			}
		}
	}
}
