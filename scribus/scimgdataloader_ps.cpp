/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qfile.h>
#include <qfileinfo.h>
#include <qregexp.h>
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
	QChar tc;
	QString tmp;
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
	if ( !QFile::exists(fn) )
		return;
	QFile f(fn);
	if (f.open(IO_ReadOnly))
	{
		QTextStream ts(&f);
		while (!ts.atEnd())
		{
			tmp = ts.readLine();
			if (tmp.startsWith("%%BeginICCProfile:"))
			{
				QByteArray psdata;
				while (!ts.atEnd())
				{
					tmp = ts.readLine();
					for (uint a = 2; a < tmp.length(); a += 2)
					{
						bool ok;
						ushort data = tmp.mid(a, 2).toUShort(&ok, 16);
						psdata.resize(psdata.size()+1);
						psdata[psdata.size()-1] = data;
					}
					if (tmp.startsWith("%%EndICCProfile"))
					{
						cmsHPROFILE prof = cmsOpenProfileFromMem(psdata.data(), psdata.size());
						if (prof)
						{
							if (static_cast<int>(cmsGetColorSpace(prof)) == icSigRgbData)
								m_profileComponents = 3;
							if (static_cast<int>(cmsGetColorSpace(prof)) == icSigCmykData)
								m_profileComponents = 4;
							const char *Descriptor;
							Descriptor = cmsTakeProductDesc(prof);
							m_imageInfoRecord.profileName = QString(Descriptor);
							m_imageInfoRecord.isEmbedded = true;
							m_embeddedProfile.duplicate((const char*)psdata.data(), psdata.size());
						}
						cmsCloseProfile(prof);
						break;
					}
				}
			}
		}
	}
}

bool ScImgDataLoader_PS::parseData(QString fn)
{
	QChar tc;
	QString tmp, FarNam;
	ScColor cc;
	double x, y, b, h, c, m, k;
	bool found = false;
	isDCS1 = false;
	isDCS2 = false;
	isDCS2multi = false;
	int plateCount = 0;
	QFile f(fn);
	if (f.open(IO_ReadOnly))
	{
		bool psFound = false;
		QTextStream ts(&f);
		while (!ts.atEnd())
		{
			tmp = ts.readLine();
			if (tmp.startsWith("%%Creator: "))
				Creator = tmp.remove("%%Creator: ");
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
			if (tmp.startsWith("%%CyanPlate:"))
			{
				colorPlates.insert("Cyan", tmp.remove("%%CyanPlate: "));
				isDCS1 = true;
			}
			if (tmp.startsWith("%%MagentaPlate:"))
			{
				colorPlates.insert("Magenta", tmp.remove("%%MagentaPlate: "));
				isDCS1 = true;
			}
			if (tmp.startsWith("%%YellowPlate:"))
			{
				colorPlates.insert("Yellow", tmp.remove("%%YellowPlate: "));
				isDCS1 = true;
			}
			if (tmp.startsWith("%%BlackPlate:"))
			{
				colorPlates.insert("Black", tmp.remove("%%BlackPlate: "));
				isDCS1 = true;
			}
			if (tmp.startsWith("%%PlateFile: ("))
			{
				tmp = tmp.remove("%%PlateFile: (");
				int endNam = tmp.find(")");
				QString plateNam = tmp.left(endNam);
				tmp = tmp.remove(plateNam+")");
				QTextStream ts2(&tmp, IO_ReadOnly);
				QString posStr, dummy, lenStr;
				ts2 >> dummy >> posStr >> lenStr;
				if (posStr.startsWith("#"))
				{
					posStr = posStr.remove(0, 1);
					uint pos = posStr.toUInt();
					uint len = lenStr.toUInt();
					struct plateOffsets offs;
					if (Creator.contains("Photoshop Version 9"))	// This is very strange, it seems that there is a bug in PS 9 which writes weired entries
					{
						pos -= (191 + plateCount * 83);
						len -= 83;
					}
					offs.pos = pos;
					offs.len = len;
					colorPlates2.insert(plateNam, offs);
					isDCS2 = true;
					plateCount++;
				}
				else
				{
					colorPlates.insert(plateNam, lenStr);
					isDCS2 = true;
					isDCS2multi = true;
				}
			}
			if (tmp.startsWith("%%CMYKCustomColor"))
			{
				tmp = tmp.remove(0,18);
				QTextStream ts2(&tmp, IO_ReadOnly);
				ts2 >> c >> m >> y >> k;
				FarNam = ts2.read();
				FarNam = FarNam.stripWhiteSpace();
				FarNam = FarNam.remove(0,1);
				FarNam = FarNam.remove(FarNam.length()-1,1);
				cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
				cc.setSpotColor(true);
				CustColors.insert(FarNam, cc);
				while (!ts.atEnd())
				{
					uint oldPos = ts.device()->at();
					tmp = ts.readLine();
					if (!tmp.startsWith("%%+"))
					{
						ts.device()->at(oldPos);
						break;
					}
					tmp = tmp.remove(0,3);
					QTextStream ts2(&tmp, IO_ReadOnly);
					ts2 >> c >> m >> y >> k;
					FarNam = ts2.read();
					FarNam = FarNam.stripWhiteSpace();
					FarNam = FarNam.remove(0,1);
					FarNam = FarNam.remove(FarNam.length()-1,1);
					cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
					cc.setSpotColor(true);
					CustColors.insert(FarNam, cc);
				}
			}
			if (tmp.startsWith("%%EndComments"))
			{
				while (!ts.atEnd())
				{
					tmp = ts.readLine();
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
							tmp = ts.readLine();
							if (tmp.startsWith("%EndPhotoshop"))
							{
								QDataStream strPhot(psdata,IO_ReadOnly);
								strPhot.setByteOrder( QDataStream::BigEndian );
								PSDHeader fakeHeader;
								QTextStream ts2(&BBox, IO_ReadOnly);
								ts2 >> x >> y >> b >> h;
								fakeHeader.width = qRound(b);
								fakeHeader.height = qRound(h);
								parseRessourceData(strPhot, fakeHeader, psdata.size());
								m_imageInfoRecord.valid = (m_imageInfoRecord.PDSpathData.size()) > 0 ? true : false;
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
					}
					if (tmp.startsWith("%%BeginICCProfile:"))
					{
						QByteArray psdata;
						while (!ts.atEnd())
						{
							tmp = ts.readLine();
							for (uint a = 2; a < tmp.length(); a += 2)
							{
								bool ok;
								ushort data = tmp.mid(a, 2).toUShort(&ok, 16);
								psdata.resize(psdata.size()+1);
								psdata[psdata.size()-1] = data;
							}
							if (tmp.startsWith("%%EndICCProfile"))
							{
								cmsHPROFILE prof = cmsOpenProfileFromMem(psdata.data(), psdata.size());
								if (prof)
								{
									if (static_cast<int>(cmsGetColorSpace(prof)) == icSigRgbData)
										m_profileComponents = 3;
									if (static_cast<int>(cmsGetColorSpace(prof)) == icSigCmykData)
										m_profileComponents = 4;
									const char *Descriptor;
									Descriptor = cmsTakeProductDesc(prof);
									m_imageInfoRecord.profileName = QString(Descriptor);
									m_imageInfoRecord.isEmbedded = true;
									m_embeddedProfile.duplicate((const char*)psdata.data(), psdata.size());
								}
								cmsCloseProfile(prof);
								break;
							}
						}
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
	return found;
}

bool ScImgDataLoader_PS::loadPicture(const QString& fn, int gsRes, bool thumbnail)
{
	QStringList args;
	double x, y, b, h;
	bool found = false;
	QString tmp, dummy, cmd1, cmd2, tmp2;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return false;
	QString ext = fi.extension(false).lower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.png");
	QString picFile = QDir::convertSeparators(fn);
	float xres = gsRes;
	float yres = gsRes;

	initialize();
	m_imageInfoRecord.type = 3;
	m_imageInfoRecord.exifDataValid = false;
	colorPlates2.clear();
	colorPlates.clear();
	CustColors.clear();
	CustColors.insert("Cyan", ScColor(255, 0, 0, 0));
	CustColors.insert("Magenta", ScColor(0, 255, 0, 0));
	CustColors.insert("Yellow", ScColor(0, 0, 255, 0));
	CustColors.insert("Black", ScColor(0, 0, 0, 255));
	found = parseData(fn);
	if ((thumbnail) && (m_imageInfoRecord.exifDataValid))
	{
		QTextStream ts2(&BBox, IO_ReadOnly);
		ts2 >> x >> y >> b >> h;
		m_imageInfoRecord.exifInfo.width = qRound(b);
		m_imageInfoRecord.exifInfo.height = qRound(h);
		m_image = m_imageInfoRecord.exifInfo.thumbnail;
		return true;
	}
	if (found)
	{
		if (isDCS1)
			loadDCS1(fn, gsRes);
		else if (isDCS2)
			loadDCS2(fn, gsRes);
		else if ((!m_imageInfoRecord.isEmbedded) || ((m_imageInfoRecord.isEmbedded) && (m_profileComponents == 3)))
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
				if ((m_imageInfoRecord.isEmbedded) && (m_profileComponents == 3))
					m_imageInfoRecord.type = 7;
				m_imageInfoRecord.colorspace = 0;
				m_image.setDotsPerMeterX ((int) (xres / 0.0254));
				m_image.setDotsPerMeterY ((int) (yres / 0.0254));
			}
		}
		else
		{
			QTextStream ts2(&BBox, IO_ReadOnly);
			ts2 >> x >> y >> b >> h;
			h = h * gsRes / 72.0;
			QStringList args;
			xres = gsRes;
			yres = gsRes;
			if (ext == "eps")
				args.append("-dEPSCrop");
			args.append("-dGrayValues=256");
			args.append("-r"+QString::number(gsRes));
			args.append("-sOutputFile="+tmpFile);
			args.append(picFile);
			int retg = callGS(args, "bitcmyk");
			if (retg == 0)
			{
				m_image.create( qRound(b * gsRes / 72.0), qRound(h * gsRes / 72.0), 32 );
				m_image.fill(qRgba(0, 0, 0, 0));
				int w = qRound(b * gsRes / 72.0);
				int w2 = 4*w;
				int h2 = qRound(h * gsRes / 72.0);
				uint *p;
				int cyan, magenta, yellow, black;
				QByteArray imgc(w2);
				QFile f(tmpFile);
				if (f.open(IO_ReadOnly))
				{
					for (int y=0; y < h2; ++y )
					{
						p = (uint *)m_image.scanLine( y );
						f.readBlock(imgc.data(), w2);
						for (int x=0; x < w2; x += 4 )
						{
							cyan = uchar(imgc[x]);
							magenta = uchar(imgc[x + 1]);
							yellow = uchar(imgc[x + 2]);
							black = uchar(imgc[x + 3]);
							*p = qRgba(cyan, magenta, yellow, black);
							p++;
						}
					}
					f.close();
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
				m_imageInfoRecord.colorspace = 1;
				m_imageInfoRecord.type = 7;
				m_image.setDotsPerMeterX ((int) (xres / 0.0254));
				m_image.setDotsPerMeterY ((int) (yres / 0.0254));
			}
		}
		return true;
	}
	return false;
}

void ScImgDataLoader_PS::loadDCS2(QString fn, int gsRes)
{
	QStringList args;
	double x, y, b, h;
	QString tmp, dummy, cmd1, cmd2, tmp2;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.png");
	QString tmpFile2 = QDir::convertSeparators(ScPaths::getTempFileDir() + "tmp.eps");
	QString baseFile = fi.dirPath(true);
	QString picFile = QDir::convertSeparators(fn);
	float xres = gsRes;
	float yres = gsRes;
	QTextStream ts2(&BBox, IO_ReadOnly);
	ts2 >> x >> y >> b >> h;
	xres = gsRes;
	yres = gsRes;
	m_image.create( qRound(b * gsRes / 72.0), qRound(h * gsRes / 72.0), 32 );
	m_image.fill(qRgba(0, 0, 0, 0));
	if (!isDCS2multi)
	{
		for (QMap<QString, plateOffsets>::Iterator it = colorPlates2.begin(); it != colorPlates2.end(); ++it)
		{
			QByteArray imgc(it.data().len);
			QFile f(picFile);
			if (f.open(IO_ReadOnly))
			{
				f.at(it.data().pos);
				uint readB = f.readBlock(imgc.data(), it.data().len);
			}
			f.close();
			QFile f2(tmpFile2);
			if (f2.open(IO_WriteOnly))
				f2.writeBlock(imgc.data(), it.data().len);
			f2.close();
			imgc.resize(0);
			args.append("-dEPSCrop");
			args.append("-r"+QString::number(gsRes));
			args.append("-sOutputFile="+tmpFile);
			args.append(tmpFile2);
			int retg = callGS(args);
			if (retg == 0)
			{
				QImage tmpImg;
				tmpImg.load(tmpFile);
				blendImages(tmpImg, CustColors[it.key()]);
				unlink(tmpFile);
			}
			unlink(tmpFile2);
		}
	}
	else
	{
		for (QMap<QString, QString>::Iterator it = colorPlates.begin(); it != colorPlates.end(); ++it)
		{
			tmpFile2 = QDir::convertSeparators(baseFile+"/"+it.data());
			args.append("-dEPSCrop");
			args.append("-r"+QString::number(gsRes));
			args.append("-sOutputFile="+tmpFile);
			args.append(tmpFile2);
			int retg = callGS(args);
			if (retg == 0)
			{
				QImage tmpImg;
				tmpImg.load(tmpFile);
				blendImages(tmpImg, CustColors[it.key()]);
				unlink(tmpFile);
			}
			args.clear();
		}
	}
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
	m_imageInfoRecord.colorspace = 1;
	m_imageInfoRecord.type = 7;
	m_image.setDotsPerMeterX ((int) (xres / 0.0254));
	m_image.setDotsPerMeterY ((int) (yres / 0.0254));
}

void ScImgDataLoader_PS::loadDCS1(QString fn, int gsRes)
{
	QStringList args;
	double x, y, b, h;
	QString tmp, dummy, cmd1, cmd2, tmp2;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.png");
	QString baseFile = fi.dirPath(true);
	QString picFile;
	float xres = gsRes;
	float yres = gsRes;
	QTextStream ts2(&BBox, IO_ReadOnly);
	ts2 >> x >> y >> b >> h;
	xres = gsRes;
	yres = gsRes;
	m_image.create( qRound(b * gsRes / 72.0), qRound(h * gsRes / 72.0), 32 );
	m_image.fill(qRgba(0, 0, 0, 0));
	if (ext == "eps")
		args.append("-dEPSCrop");
	args.append("-r"+QString::number(gsRes));
	args.append("-sOutputFile="+tmpFile);
	picFile = QDir::convertSeparators(baseFile+"/"+colorPlates["Cyan"]);
	args.append(picFile);
	int retg = callGS(args);
	if (retg == 0)
	{
		QImage tmpImg;
		tmpImg.load(tmpFile);
		blendImages(tmpImg, ScColor(255, 0, 0, 0));
		unlink(tmpFile);
	}
	args.clear();

	if (ext == "eps")
		args.append("-dEPSCrop");
	args.append("-r"+QString::number(gsRes));
	args.append("-sOutputFile="+tmpFile);
	picFile = QDir::convertSeparators(baseFile+"/"+colorPlates["Magenta"]);
	args.append(picFile);
	retg = callGS(args);
	if (retg == 0)
	{
		QImage tmpImg;
		tmpImg.load(tmpFile);
		blendImages(tmpImg, ScColor(0, 255, 0, 0));
		unlink(tmpFile);
	}
	args.clear();

	if (ext == "eps")
		args.append("-dEPSCrop");
	args.append("-r"+QString::number(gsRes));
	args.append("-sOutputFile="+tmpFile);
	picFile = QDir::convertSeparators(baseFile+"/"+colorPlates["Yellow"]);
	args.append(picFile);
	retg = callGS(args);
	if (retg == 0)
	{
		QImage tmpImg;
		tmpImg.load(tmpFile);
		blendImages(tmpImg, ScColor(0, 0, 255, 0));
		unlink(tmpFile);
	}
	args.clear();

	if (ext == "eps")
		args.append("-dEPSCrop");
	args.append("-r"+QString::number(gsRes));
	args.append("-sOutputFile="+tmpFile);
	picFile = QDir::convertSeparators(baseFile+"/"+colorPlates["Black"]);
	args.append(picFile);
	retg = callGS(args);
	if (retg == 0)
	{
		QImage tmpImg;
		tmpImg.load(tmpFile);
		blendImages(tmpImg, ScColor(0, 0, 0, 255));
		unlink(tmpFile);
	}
	args.clear();

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
	m_imageInfoRecord.colorspace = 1;
	m_imageInfoRecord.type = 7;
	m_image.setDotsPerMeterX ((int) (xres / 0.0254));
	m_image.setDotsPerMeterY ((int) (yres / 0.0254));
}

void ScImgDataLoader_PS::blendImages(QImage &source, ScColor col)
{
	int h = source.height();
	int w = source.width();
	int cyan, c, m, yc, k, cc, mm, yy, kk;
	col.getCMYK(&c, &m, &yc, &k);
	for (int y=0; y < h; ++y )
	{
		QRgb *p = (QRgb *)m_image.scanLine( y );
		QRgb *pq = (QRgb *)source.scanLine( y );
		for (int x=0; x < w; ++x )
		{
			cyan = 255 - qRed(*pq);
			if (cyan != 0)
			{
				(c == 0) ? cc = qRed(*p) : cc = QMIN(c * cyan / 255 + qRed(*p), 255);
				(m == 0) ? mm = qGreen(*p) : mm = QMIN(m * cyan / 255 + qGreen(*p), 255);
				(yc == 0) ? yy = qBlue(*p) : yy = QMIN(yc * cyan / 255 + qBlue(*p), 255);
				(k == 0) ? kk = qAlpha(*p) : kk = QMIN(k * cyan / 255 + qAlpha(*p), 255);
				*p = qRgba(cc, mm, yy, kk);
			}
			p++;
			pq++;
		}
	}
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
