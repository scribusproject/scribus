/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qfile.h>
#include <qfileinfo.h>
#include "gsutil.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scimgdataloader_ps.h"

ScImgDataLoader_PS::ScImgDataLoader_PS(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_PS::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "ps" );
	m_supportedFormats.append( "eps" );
}

void ScImgDataLoader_PS::loadEmbeddedProfile(const QString& fn)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
}

bool ScImgDataLoader_PS::loadPicture(const QString& fn, int gsRes, bool thumbnail)
{
	QStringList args;
	double x, y, b, h;
	bool found = false;
	QString tmp, dummy, cmd1, cmd2, BBox, tmp2;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return false;
	QChar tc;
	QString ext = fi.extension(false).lower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.png");
	QString picFile = QDir::convertSeparators(fn);
	float xres = gsRes;
	float yres = gsRes;

	initialize();
	m_imageInfoRecord.type = 3;
	m_imageInfoRecord.exifDataValid = false;
	QFile f(fn);
	if (f.open(IO_ReadOnly))
	{
		bool psFound = false;
		QTextStream ts(&f);
		while (!ts.atEnd())
		{
			tc = ' ';
			tmp = "";
			while ((tc != '\n') && (tc != '\r'))
			{
				ts >> tc;
				if ((tc != '\n') && (tc != '\r'))
					tmp += tc;
			}
			if (tmp.startsWith("%%BoundingBox:"))
			{
				found = true;
				BBox = tmp.remove("%%BoundingBox:");
			}
			if (!found)
			{
				if (tmp.startsWith("%%BoundingBox"))
				{
					found = true;
					BBox = tmp.remove("%%BoundingBox");
				}
			}
			if (tmp.startsWith("%%EndComments"))
			{
				while (!ts.atEnd())
				{
					tc = ' ';
					tmp = "";
					while ((tc != '\n') && (tc != '\r'))
					{
						ts >> tc;
						if ((tc != '\n') && (tc != '\r'))
							tmp += tc;
					}
					if ((!tmp.isEmpty()) && (!tmp.startsWith("%")))
					{
						psFound = true;
						break;
					}
					if (tmp.startsWith("%BeginPhotoshop"))
					{
						QByteArray psdata;
						while (!ts.atEnd())
						{
							tc = ' ';
							tmp = "";
							while ((tc != '\n') && (tc != '\r'))
							{
								ts >> tc;
								if ((tc != '\n') && (tc != '\r'))
									tmp += tc;
							}
							if (tmp.startsWith("%EndPhotoshop"))
							{
								QDataStream strPhot(psdata,IO_ReadOnly);
								strPhot.setByteOrder( QDataStream::BigEndian );
								PSDHeader fakeHeader;
								QTextStream ts2(&BBox, IO_ReadOnly);
								ts2 >> x >> y >> b >> h;
								fakeHeader.width = b;
								fakeHeader.height = h;
								parseRessourceData(strPhot, fakeHeader, psdata.size());
								m_imageInfoRecord.valid = (m_imageInfoRecord.PDSpathData.size()) > 0 ? true : false;
								psFound = true;
								break;
							}
							for (uint a = 2; a < tmp.length(); a += 2)
							{
								bool ok;
								ushort data = tmp.mid(a, 2).toUShort(&ok, 16);
								psdata.resize(psdata.size()+1);
								psdata[psdata.size()-1] = data;
							}
						}
						if (psFound)
							break;
					}
					if (psFound)
						break;
				}
			}
			if (psFound)
				break;
		}
	}
	f.close();
	if ((thumbnail) && (m_imageInfoRecord.exifDataValid))
	{
		QTextStream ts2(&BBox, IO_ReadOnly);
		ts2 >> x >> y >> b >> h;
		m_imageInfoRecord.exifInfo.width = b;
		m_imageInfoRecord.exifInfo.height = h;
		m_image = m_imageInfoRecord.exifInfo.thumbnail;
		return true;
	}
	if (found)
	{
		QTextStream ts2(&BBox, IO_ReadOnly);
		ts2 >> x >> y >> b >> h;
		h = h * gsRes / 72.0;
		QStringList args;
		xres = gsRes;
		yres = gsRes;
		if (ext == "eps")
			args.append("-dEPSCrop");
		args.append("-r"+QString::number(gsRes));
		args.append("-sOutputFile="+tmpFile);
		args.append(picFile);
		int retg = callGS(args);
		if (retg == 0)
		{
			m_image.load(tmpFile);
			m_image.setAlphaBuffer(true);
			if (ScCore->havePNGAlpha() != 0)
			{
				int wi = m_image.width();
				int hi = m_image.height();
				for( int yi=0; yi < hi; ++yi )
				{
					QRgb *s = (QRgb*)(m_image.scanLine( yi ));
					QRgb alphaFF = qRgba(255,255,255,255);
					QRgb alpha00 = qRgba(255,255,255,  0);
					for(int xi=0; xi < wi; ++xi )
					{
						if((*s) == alphaFF)
							(*s) &= alpha00;
						s++;
					}
				}
			}
			unlink(tmpFile);
			if (ext == "eps")
			{
				m_imageInfoRecord.BBoxX = static_cast<int>(x);
				m_imageInfoRecord.BBoxH = static_cast<int>(h);
			}
			else
			{
				m_imageInfoRecord.BBoxX = 0;
				m_imageInfoRecord.BBoxH = m_image.height();
			}
			m_imageInfoRecord.xres = qRound(gsRes);
			m_imageInfoRecord.yres = qRound(gsRes);
			m_imageInfoRecord.colorspace = 0;
			m_image.setDotsPerMeterX ((int) (xres / 0.0254));
			m_image.setDotsPerMeterY ((int) (yres / 0.0254));
			return true;
		}
	}
	return false;
}

void ScImgDataLoader_PS::preloadAlphaChannel(const QString& fn, int gsRes)
{
	float xres, yres;
	short resolutionunit = 0;
	initialize();
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return;
	QString tmp, BBox, tmp2;
	QString ext = fi.extension(false).lower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.png");
	QString picFile = QDir::convertSeparators(fn);
	double x, y, b, h;
	bool found = false;
	int retg = -1;
	QChar tc;
	
	QFile f(fn);
	if (f.open(IO_ReadOnly))
	{
		QTextStream ts(&f);
		while (!ts.atEnd())
		{
			tc = ' ';
			tmp = "";
			while ((tc != '\n') && (tc != '\r'))
			{
				ts >> tc;
				if ((tc != '\n') && (tc != '\r'))
					tmp += tc;
			}
			if (tmp.startsWith("%%BoundingBox:"))
			{
				found = true;
				BBox = tmp.remove("%%BoundingBox:");
			}
			if (!found)
			{
				if (tmp.startsWith("%%BoundingBox"))
				{
					found = true;
					BBox = tmp.remove("%%BoundingBox");
				}
			}
			if (tmp.startsWith("%%EndComments"))
				break;
		}
	}
	f.close();
	if (found)
	{
		QTextStream ts2(&BBox, IO_ReadOnly);
		ts2 >> x >> y >> b >> h;
		h = h * gsRes / 72.0;
		QStringList args;
		xres = gsRes;
		yres = gsRes;
		if (ext == "eps")
			args.append("-dEPSCrop");
		args.append("-r"+QString::number(gsRes));
		args.append("-sOutputFile="+tmpFile);
		args.append(picFile);
		int retg = callGS(args);
		if (retg == 0)
		{
			m_image.load(tmpFile);
			m_image.setAlphaBuffer(true);
			if (ScCore->havePNGAlpha() != 0)
			{
				int wi = m_image.width();
				int hi = m_image.height();
				for( int yi=0; yi < hi; ++yi )
				{
					QRgb *s = (QRgb*)(m_image.scanLine( yi ));
					QRgb alphaFF = qRgba(255,255,255,255);
					QRgb alpha00 = qRgba(255,255,255,  0);
					for(int xi=0; xi < wi; ++xi )
					{
						if((*s) == alphaFF)
							(*s) &= alpha00;
						s++;
					}
				}
			}
			m_image.setAlphaBuffer(true);
			unlink(tmpFile);
		}
	}
}
