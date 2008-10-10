/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <QTextStream>

#include "scpaths.h"
#include "scribuscore.h"
#include "scimgdataloader_ps.h"
#include "prefsmanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_ghostscript.h"
#include "util_math.h"
#include "cmsettings.h"
#include "scimage.h"

extern "C"
{
#define XMD_H           // shut JPEGlib up
#if defined(Q_OS_UNIXWARE)
#  define HAVE_BOOLEAN  // libjpeg under Unixware seems to need this
#endif
#include <jpeglib.h>
#include <jerror.h>
#undef HAVE_STDLIB_H
#ifdef const
#  undef const          // remove crazy C hackery in jconfig.h
#endif
}

ScImgDataLoader_PS::ScImgDataLoader_PS(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_PS::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "ps" );
	m_supportedFormats.append( "eps" );
	m_supportedFormats.append( "epsi" );
}

void ScImgDataLoader_PS::loadEmbeddedProfile(const QString& fn, int /* page */)
{
	QChar tc;
	QString tmp;
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
	if ( !QFile::exists(fn) )
		return;
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream ts(&f);
		while (!ts.atEnd())
		{
			tmp = readLinefromDataStream(ts);
			if (tmp.startsWith("%%BeginICCProfile:"))
			{
				QByteArray psdata;
				while (!ts.atEnd())
				{
					tmp = readLinefromDataStream(ts);
					for (int a = 2; a < tmp.length(); a += 2)
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
							m_embeddedProfile = QByteArray((const char*)psdata.data(), psdata.size());
						}
						cmsCloseProfile(prof);
						break;
					}
				}
			}
		}
	}
}

void ScImgDataLoader_PS::scanForFonts(QString fn)
{
	QString tmp;
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		QDataStream ts(&f);
		while (!ts.atEnd())
		{
			tmp = readLinefromDataStream(ts);
			if (tmp.startsWith("%%BeginFont:"))
			{
				tmp = tmp.remove("%%BeginFont:");
				QTextStream ts2(&tmp, QIODevice::ReadOnly);
				QString tmp2;
				ts2 >> tmp2;
				FontListe.removeAll(tmp2);
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
	isPhotoshop = false;
	hasPhotoshopImageData = false;
	hasThumbnail = false;
	inTrailer = false;
	BBoxInTrailer = false;
	int plateCount = 0;
	uint startPos = 0;
	FontListe.clear();
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		QByteArray tempBuf(9, ' ');
		f.read(tempBuf.data(), 8);
		if (getDouble(QString(tempBuf.mid(0, 4)), true) == 0xC5D0D3C6)
		{
			startPos = getDouble(tempBuf.mid(4, 4), false);
			if (doThumbnail)
			{
				f.seek(0);
				QByteArray tmp2buf(29, ' ');
				f.read(tmp2buf.data(), 28);
				uint thumbStart = 0;
				thumbStart = tmp2buf[20] & 0xff;
				thumbStart |= (tmp2buf[21] << 8) & 0xff00;
				thumbStart |= (tmp2buf[22] << 16) & 0xff0000;
				thumbStart |= (tmp2buf[23] << 24) & 0xff000000;
				uint thumbLen = 0;
				thumbLen = tmp2buf[24] & 0xff;
				thumbLen |= (tmp2buf[25] << 8) & 0xff00;
				thumbLen |= (tmp2buf[26] << 16) & 0xff0000;
				thumbLen |= (tmp2buf[27] << 24) & 0xff000000;
				if (thumbLen != 0)
				{
					QByteArray imgc(thumbLen, ' ');
					f.seek(thumbStart);
					uint readB = f.read(imgc.data(), thumbLen);
					readB = 0;
					QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "preview.tiff");
					QFile f2(tmpFile);
					if (f2.open(QIODevice::WriteOnly))
						f2.write(imgc.data(), thumbLen);
					f2.close();
					imgc.resize(0);
					ScImage thum;
					CMSettings cms(0, "", 0);
					bool mode = true;
					if (thum.LoadPicture(tmpFile, 1, cms, false, false, ScImage::RGBData, 72, &mode))
					{
						m_imageInfoRecord.exifDataValid = true;
						m_imageInfoRecord.exifInfo.thumbnail = thum.qImage().copy();
					}
					QFile::remove(tmpFile);
					hasThumbnail = true;
				}
			}
		}
		bool psFound = false;
		bool isAtend = false;
		QDataStream ts(&f);
		ts.device()->seek(startPos);
		while (!ts.atEnd())
		{
			tmp = readLinefromDataStream(ts);
			if (tmp.startsWith("%%Creator: "))
				Creator = tmp.remove("%%Creator: ");
			if (tmp.startsWith("%%Pages: "))
			{
				tmp = tmp.remove("%%Pages: ");
				bool ok;
				int pages = tmp.toInt( &ok );
				if (ok)
					m_imageInfoRecord.numberOfPages = pages; 
			}
			if (tmp.startsWith("%%Trailer"))
				inTrailer = true;
			if (tmp.startsWith("%%BoundingBox:"))
			{
				found = true;
				if (inTrailer)
					BBoxInTrailer = true;
				BBox = tmp.remove("%%BoundingBox:");
			}
			if (!found)
			{
				if (tmp.startsWith("%%BoundingBox"))
				{
					found = true;
					if (inTrailer)
						BBoxInTrailer = true;
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
				int endNam = tmp.indexOf(")");
				QString plateNam = tmp.left(endNam);
				tmp = tmp.remove(plateNam+")");
				QTextStream ts2(&tmp, QIODevice::ReadOnly);
				QString posStr, dummy, lenStr;
				ts2 >> dummy >> posStr >> lenStr;
				if (dummy == "EPS")
				{
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
			}
			if (tmp.startsWith("%%DocumentFonts:"))
			{
				tmp = tmp.remove("%%DocumentFonts:");
				QTextStream ts2(&tmp, QIODevice::ReadOnly);
				QString tmp2;
				ts2 >> tmp2;
				if (!tmp2.contains("(atend)"))
				{
					if (!tmp2.isEmpty())
						FontListe.append(tmp2);
					while (!ts.atEnd())
					{
						uint oldPos = ts.device()->pos();
						tmp = readLinefromDataStream(ts);
						if (!tmp.startsWith("%%+"))
						{
							ts.device()->seek(oldPos);
							break;
						}
						tmp = tmp.remove(0,3);
						QTextStream ts2(&tmp, QIODevice::ReadOnly);
						QString tmp2;
						ts2 >> tmp2;
						if (!tmp2.isEmpty())
							FontListe.append(tmp2);
					}
				}
				else
					isAtend = true;
			}
			if (tmp.startsWith("%%CMYKCustomColor"))
			{
				tmp = tmp.remove(0,18);
				QTextStream ts2(&tmp, QIODevice::ReadOnly);
				ts2 >> c >> m >> y >> k;
				FarNam = ts2.readAll();
				FarNam = FarNam.trimmed();
				FarNam = FarNam.remove(0,1);
				FarNam = FarNam.remove(FarNam.length()-1,1);
				cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
				cc.setSpotColor(true);
				CustColors.insert(FarNam, cc);
				while (!ts.atEnd())
				{
					uint oldPos = ts.device()->pos();
					tmp = readLinefromDataStream(ts);
					if (!tmp.startsWith("%%+"))
					{
						ts.device()->seek(oldPos);
						break;
					}
					tmp = tmp.remove(0,3);
					QTextStream ts2(&tmp, QIODevice::ReadOnly);
					ts2 >> c >> m >> y >> k;
					FarNam = ts2.readAll();
					FarNam = FarNam.trimmed();
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
					tmp = readLinefromDataStream(ts);
					if ((!tmp.isEmpty()) && (!tmp.startsWith("%")))
					{
						psFound = true;
						break;
					}
					if (tmp.startsWith("%ImageData: "))
					{
						hasPhotoshopImageData = true;
						tmp.remove("%ImageData: ");
						QTextStream ts2(&tmp, QIODevice::ReadOnly);
						ts2 >> psXSize >> psYSize >> psDepth >> psMode >> psChannel >> psBlock >> psDataType >> psCommand;
						psCommand = psCommand.remove(0,1);
						psCommand = psCommand.remove(psCommand.length()-1,1);
					}
					if (tmp.startsWith("%BeginPhotoshop"))
					{
						QByteArray psdata;
						while (!ts.atEnd())
						{
							tmp = readLinefromDataStream(ts);
							if (tmp.startsWith("%EndPhotoshop"))
							{
								QDataStream strPhot( &psdata, QIODevice::ReadOnly);
								strPhot.setByteOrder( QDataStream::BigEndian );
								PSDHeader fakeHeader;
								QTextStream ts2(&BBox, QIODevice::ReadOnly);
								ts2 >> x >> y >> b >> h;
								fakeHeader.width = qRound(b);
								fakeHeader.height = qRound(h);
								parseRessourceData(strPhot, fakeHeader, psdata.size());
								m_imageInfoRecord.valid = (m_imageInfoRecord.PDSpathData.size()) > 0 ? true : false;
								if (m_imageInfoRecord.PDSpathData.size() > 0)
								{
									QMatrix mm;
									mm.scale(m_imageInfoRecord.xres / 72.0, m_imageInfoRecord.yres / 72.0);
									QMap<QString, FPointArray>::Iterator it;
									for (it = m_imageInfoRecord.PDSpathData.begin(); it != m_imageInfoRecord.PDSpathData.end(); ++it)
									{
										it.value().map(mm);
									}
								}
								isPhotoshop = true;
								break;
							}
							for (int a = 2; a < tmp.length(); a += 2)
							{
								bool ok;
								ushort data = tmp.mid(a, 2).toUShort(&ok, 16);
								psdata.resize(psdata.size()+1);
								psdata[psdata.size()-1] = data;
							}
						}
						if ((doThumbnail) && ((hasThumbnail) || (!m_imageInfoRecord.exifInfo.thumbnail.isNull())))
							return true;
					}
					if (tmp.startsWith("%%BeginICCProfile:"))
					{
						QByteArray psdata;
						while (!ts.atEnd())
						{
							tmp = readLinefromDataStream(ts);
							for (int a = 2; a < tmp.length(); a += 2)
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
									m_embeddedProfile = QByteArray((const char*)psdata.data(), psdata.size());
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
			if ((psFound) && (!isAtend))
				break;
		}
	}
	f.close();
	return found;
}

bool ScImgDataLoader_PS::loadPicture(const QString& fn, int page, int gsRes, bool thumbnail)
{
	QStringList args;
	double x, y, b, h;
	bool found = false;
	QString tmp, dummy, cmd1, cmd2, tmp2;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return false;
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		ext = getImageType(fn);
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + QString("sc%1.png").arg(qMax(1, page)));
	QString tmpFiles = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc%d.png");
	QString picFile = QDir::convertSeparators(fn);
	float xres = gsRes;
	float yres = gsRes;

	initialize();

	m_imageInfoRecord.type = ImageTypeEPS;
	m_imageInfoRecord.exifDataValid = false;
	m_imageInfoRecord.numberOfPages = 1; // will be overwritten by parse()
	doThumbnail = thumbnail;
	colorPlates2.clear();
	colorPlates.clear();
	CustColors.clear();
	CustColors.insert("Cyan", ScColor(255, 0, 0, 0));
	CustColors.insert("Magenta", ScColor(0, 255, 0, 0));
	CustColors.insert("Yellow", ScColor(0, 0, 255, 0));
	CustColors.insert("Black", ScColor(0, 0, 0, 255));
	found = parseData(fn);
	if (FontListe.count() != 0)
	{
		scanForFonts(fn);
		if (FontListe.count() != 0)
		{
			bool missing = false;
			QString missingF = "";
			for (int fo = 0; fo < FontListe.count(); fo++)
			{
				if (!PrefsManager::instance()->appPrefs.AvailFonts.contains(FontListe[fo]))
				{
					missing = true;
					missingF += FontListe[fo]+"\n";
				}
			}
			if (missing)
			{
				m_message = QObject::tr("The Font(s):\n%1 are not embedded or available for Scribus.\nThey might be replaced by \"Courier\", depending how your Ghostscript is configured.\nTherefore the image may be not correct").arg(missingF);
				m_msgType = warningMsg;
			}
		}
	}
	if ((thumbnail) && (m_imageInfoRecord.exifDataValid) && (!m_imageInfoRecord.exifInfo.thumbnail.isNull()))
	{
		QTextStream ts2(&BBox, QIODevice::ReadOnly);
		ts2 >> x >> y >> b >> h;
		m_imageInfoRecord.exifInfo.width = qRound(b);
		m_imageInfoRecord.exifInfo.height = qRound(h);
		m_image = m_imageInfoRecord.exifInfo.thumbnail;
		if ((isPhotoshop) && (hasPhotoshopImageData))
		{
			m_imageInfoRecord.exifInfo.width = qRound(psXSize);
			m_imageInfoRecord.exifInfo.height = qRound(psYSize);
			m_imageInfoRecord.type = ImageType7;
			if (psMode == 4)
			{
				m_imageInfoRecord.colorspace = ColorSpaceCMYK;
				QRgb *s;
				unsigned char cc, cm, cy, ck;
				for( int yit=0; yit < m_image.height(); ++yit )
				{
					s = (QRgb*)(m_image.scanLine( yit ));
					for(int xit=0; xit < m_image.width(); ++xit )
					{
						cc = 255 - qRed(*s);
						cm = 255 - qGreen(*s);
						cy = 255 - qBlue(*s);
						ck = qMin(qMin(cc, cm), cy);
						*s = qRgba(cc-ck,cm-ck,cy-ck,ck);
						s++;
					}
				}
			}
			else
				m_imageInfoRecord.colorspace = ColorSpaceRGB;
		}
		else
			m_imageInfoRecord.colorspace = ColorSpaceRGB;
		m_imageInfoRecord.actualPageNumber = page;
		return true;
	}
	if (found)
	{
		if (isDCS1)
			loadDCS1(fn, gsRes);
		else if (isDCS2)
			loadDCS2(fn, gsRes);
		else if ((isPhotoshop) && (hasPhotoshopImageData))
			loadPhotoshop(fn, gsRes);
		else if ((!m_imageInfoRecord.isEmbedded) || ((m_imageInfoRecord.isEmbedded) && (m_profileComponents == 3)))
		{
			QTextStream ts2(&BBox, QIODevice::ReadOnly);
			ts2 >> x >> y >> b >> h;
			QStringList args;
			xres = gsRes;
			yres = gsRes;
			if (extensionIndicatesEPS(ext))
			{
				if (!BBoxInTrailer)
					args.append("-dEPSCrop");
			}
			args.append("-r"+QString::number(gsRes));
			args.append("-sOutputFile="+tmpFiles);
			args.append(picFile);
			h = h * gsRes / 72.0;
			int retg = callGS(args);
			if (retg == 0)
			{
				m_image.load(tmpFile);
				if (extensionIndicatesEPS(ext) && BBoxInTrailer)
				{
					int ex = qRound(x * gsRes / 72.0);
					int ey = qRound(m_image.height() - h);
					int ew = qRound((b - x) * gsRes / 72.0);
					int eh = qRound(h - y * gsRes / 72.0);
					m_image = m_image.copy(ex, ey, ew, eh);
				}
				if (!ScCore->havePNGAlpha())
				{
					int wi = m_image.width();
					int hi = m_image.height();
					QRgb alphaFF = qRgba(255,255,255,255);
					QRgb alpha00 = qRgba(255,255,255,  0);
					QRgb *s;
					for( int yi=0; yi < hi; ++yi )
					{
						s = (QRgb*)(m_image.scanLine( yi ));
						for(int xi=0; xi < wi; ++xi )
						{
							if((*s) == alphaFF)
								(*s) &= alpha00;
							s++;
						}
					}
				}

				QStringList files = QStringList("sc*.png");
				files = QDir(ScPaths::getTempFileDir()).entryList(files);
				for (int i=0; i < files.count(); ++i)
					QFile::remove(QDir::convertSeparators(ScPaths::getTempFileDir() + files[i]));
				
				if (extensionIndicatesEPS(ext))
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
					m_imageInfoRecord.type = ImageType7;
				m_imageInfoRecord.colorspace = ColorSpaceRGB;
				m_image.setDotsPerMeterX ((int) (xres / 0.0254));
				m_image.setDotsPerMeterY ((int) (yres / 0.0254));
			}
		}
		else
		{
			QTextStream ts2(&BBox, QIODevice::ReadOnly);
			ts2 >> x >> y >> b >> h;
			h = h * gsRes / 72.0;
			QStringList args;
			xres = gsRes;
			yres = gsRes;
			if (extensionIndicatesEPS(ext))
				args.append("-dEPSCrop");
			args.append("-dGrayValues=256");
			args.append("-r"+QString::number(gsRes));
			args.append("-sOutputFile="+tmpFiles);
			args.append(picFile);
//			qDebug() << "scimgdataloader_ps:" << args;
			int retg = callGS(args, "bitcmyk");
			if (retg == 0)
			{
				m_image = QImage( qRound(b * gsRes / 72.0), qRound(h * gsRes / 72.0), QImage::Format_ARGB32 );
				m_image.fill(qRgba(0, 0, 0, 0));
				int w = qRound(b * gsRes / 72.0);
				int w2 = 4*w;
				int h2 = qRound(h * gsRes / 72.0);
				uint *p;
				int cyan, magenta, yellow, black;
				QByteArray imgc(w2, ' ');
				QFile f(tmpFile);
				if (f.open(QIODevice::ReadOnly))
				{
					for (int y=0; y < h2; ++y )
					{
						p = (uint *)m_image.scanLine( y );
						f.read(imgc.data(), w2);
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
				
				QStringList files = QStringList("sc*.png");
				files = QDir(ScPaths::getTempFileDir()).entryList(files);
				for (int i=0; i < files.count(); ++i)
					QFile::remove(QDir::convertSeparators(ScPaths::getTempFileDir() + files[i]));
				
				if (extensionIndicatesEPS(ext))
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
				m_imageInfoRecord.colorspace = ColorSpaceCMYK;
				m_imageInfoRecord.type = ImageType7;
				m_image.setDotsPerMeterX ((int) (xres / 0.0254));
				m_image.setDotsPerMeterY ((int) (yres / 0.0254));
			}
			else
			{
				qDebug() << "Ghostscript returned result" << retg;
			}
		}
		m_imageInfoRecord.actualPageNumber = page;
		return true;
	}
	return false;
}

void ScImgDataLoader_PS::loadPhotoshop(QString fn, int gsRes)
{
	if ((psDataType >= 1) && (psDataType <= 6) && ((psMode == 3) || (psMode == 4)))
	{
		loadPhotoshopBinary(fn);
		return;
	}
	QStringList args;
	double x, y, b, h;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc1.png");
	int retg;
	int GsMajor;
	int GsMinor;
	getNumericGSVersion(GsMajor, GsMinor);
	QTextStream ts2(&BBox, QIODevice::ReadOnly);
	ts2 >> x >> y >> b >> h;
	h = h * gsRes / 72.0;
	if (extensionIndicatesEPS(ext))
		args.append("-dEPSCrop");
	if (psMode == 4)
		args.append("-dGrayValues=256");
	if ((GsMajor >= 8) && (GsMinor >= 53))
		args.append("-dNOPSICC");		// prevent GS from applying an embedded ICC profile as it will be applied later on in ScImage.
	args.append("-r"+QString::number(gsRes));
	args.append("-sOutputFile=" + tmpFile);
	args.append(QDir::convertSeparators(fn));
	if (psMode == 4)
		retg = callGS(args, "bitcmyk");
	else
		retg = callGS(args);
	if (retg == 0)
	{
		if (psMode == 4)
		{
			m_image = QImage( qRound(b * gsRes / 72.0), qRound(h * gsRes / 72.0), QImage::Format_ARGB32 );
			m_image.fill(qRgba(0, 0, 0, 0));
			int w = qRound(b * gsRes / 72.0);
			int w2 = 4*w;
			int h2 = qRound(h * gsRes / 72.0);
			uint *p;
			int cyan, magenta, yellow, black;
			QByteArray imgc(w2, ' ');
			QFile f(tmpFile);
			if (f.open(QIODevice::ReadOnly))
			{
				for (int y=0; y < h2; ++y )
				{
					p = (uint *)m_image.scanLine( y );
					f.read(imgc.data(), w2);
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
			m_imageInfoRecord.colorspace = ColorSpaceCMYK;
			m_imageInfoRecord.type = ImageType7;
		}
		else
		{
			m_image.load(tmpFile);
			if (!ScCore->havePNGAlpha())
			{
				int wi = m_image.width();
				int hi = m_image.height();
				QRgb alphaFF = qRgba(255,255,255,255);
				QRgb alpha00 = qRgba(255,255,255,  0);
				QRgb *s;
				for( int yi=0; yi < hi; ++yi )
				{
					s = (QRgb*)(m_image.scanLine( yi ));
					for(int xi=0; xi < wi; ++xi )
					{
						if((*s) == alphaFF)
							(*s) &= alpha00;
						s++;
					}
				}
			}
			m_imageInfoRecord.type = ImageType7;
			m_imageInfoRecord.colorspace = ColorSpaceRGB;
		}
		
		QFile::remove(tmpFile);

		if (extensionIndicatesEPS(ext))
		{
			m_imageInfoRecord.BBoxX = static_cast<int>(x);
			m_imageInfoRecord.BBoxH = static_cast<int>(h);
		}
		else
		{
			m_imageInfoRecord.BBoxX = 0;
			m_imageInfoRecord.BBoxH = m_image.height();
		}
		m_image.setDotsPerMeterX ((int) (m_imageInfoRecord.xres / 0.0254));
		m_image.setDotsPerMeterY ((int) (m_imageInfoRecord.yres / 0.0254));
	}
	else
	{
		qDebug() << "Ghostscript returned result" << retg;
	}
}

void ScImgDataLoader_PS::decodeA85(QByteArray &psdata, QString tmp)
{
	uchar byte;
	ushort data;
	unsigned long sum = 0;
	int quintet = 0;
	for (int c = 0; c < tmp.length(); c++)
	{
		byte = QChar(tmp.at(c)).cell();
		if (byte >= '!' && byte <= 'u')
		{
			sum = sum * 85 + ((unsigned long)byte - '!');
			quintet++;
			if (quintet == 5)
			{
				psdata.resize(psdata.size()+4);
				data = sum >> 24;
				psdata[psdata.size()-4] = data;
				data = (sum >> 16) & 0xFF;
				psdata[psdata.size()-3] = data;
				data = (sum >> 8) & 0xFF;
				psdata[psdata.size()-2] = data;
				data = sum & 0xFF;
				psdata[psdata.size()-1] = data;
				quintet = 0;
				sum = 0;
			}
		}
		else if (byte == 'z')
		{
			psdata.resize(psdata.size()+4);
			psdata[psdata.size()-4] = 0;
			psdata[psdata.size()-3] = 0;
			psdata[psdata.size()-2] = 0;
			psdata[psdata.size()-1] = 0;
		}
		else if (byte == '~')
		{
			if (quintet)
			{
				int i;
				for (i = 0; i < 5 - quintet; i++)
					sum *= 85;
				if (quintet > 1)
					sum += (0xFFFFFF >> ((quintet - 2) * 8));
				for (i = 0; i < quintet - 1; i++)
				{
					data = (sum >> (24 - 8 * i)) & 0xFF;
					psdata.resize(psdata.size()+1);
					psdata[psdata.size()-1] = data;
				}
				quintet = 0;
			}
			break;
		}
	}
}

typedef struct my_error_mgr
{
	struct jpeg_error_mgr pub;            /* "public" fields */
	jmp_buf setjmp_buffer;  /* for return to caller */
} *my_error_ptr;

static void my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp (myerr->setjmp_buffer, 1);
}

bool ScImgDataLoader_PS::loadPSjpeg(QString fn)
{
	if (!QFile::exists(fn))
		return false;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr         jerr;
	FILE     *infile;
	cinfo.err = jpeg_std_error (&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	infile = NULL;
	if (setjmp (jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress (&cinfo);
		if (infile)
			fclose (infile);
		return false;
	}
	jpeg_create_decompress (&cinfo);
	if ((infile = fopen (fn.toLocal8Bit(), "rb")) == NULL)
		return false;
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, true);
	jpeg_start_decompress(&cinfo);
	if ( cinfo.output_components == 3 || cinfo.output_components == 4)
		m_image = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_ARGB32 );
	else if ( cinfo.output_components == 1 )
	{
		m_image = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_Indexed8 );
		m_image.setNumColors(256);
		for (int i=0; i<256; i++)
			m_image.setColor(i, qRgb(i,i,i));
	}
	if (!m_image.isNull())
	{
		uchar* data = m_image.bits();
		int bpl = m_image.bytesPerLine();
		while (cinfo.output_scanline < cinfo.output_height)
		{
			uchar *d = data + cinfo.output_scanline * bpl;
			(void) jpeg_read_scanlines(&cinfo, &d, 1);
		}
		if ( cinfo.output_components == 3 )
		{
			uchar *in;
			QRgb *out;
			for (uint j=0; j<cinfo.output_height; j++)
			{
				in = m_image.scanLine(j) + cinfo.output_width * 3;
				out = (QRgb*) m_image.scanLine(j);
				for (uint i=cinfo.output_width; i--; )
				{
					in -= 3;
					out[i] = qRgb(in[0], in[1], in[2]);
				}
			}
		}
		if ( cinfo.output_components == 4 )
		{
			QRgb *ptr;
			unsigned char c, m, y ,k;
			unsigned char *p;
			for (int i = 0; i < m_image.height(); i++)
			{
				ptr = (QRgb*)  m_image.scanLine(i);
				if ((cinfo.jpeg_color_space == JCS_YCCK) || ((cinfo.jpeg_color_space == JCS_CMYK) && (cinfo.saw_Adobe_marker)))
				{
					for (int j = 0; j <  m_image.width(); j++)
					{
						p = (unsigned char *) ptr;
						c = p[0];
						m = p[1];
						y =  p[2];
						k =  p[3];
						*ptr = qRgba(c, m, y, k);
						ptr++;
					}
				}
				else
				{
					for (int j = 0; j <  m_image.width(); j++)
					{
						p = (unsigned char *) ptr;
						c = p[0];
						m = p[1];
						y =  p[2];
						k =  p[3];
						*ptr = qRgba(y, m, c, k);
						ptr++;
					}
				}
			}
		}
		if ( cinfo.output_components == 1 )
		{
			QImage tmpImg = m_image.convertToFormat(QImage::Format_ARGB32);
			m_image = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_ARGB32 );
			QRgb *s;
			QRgb *d;
			for( int yi=0; yi < tmpImg.height(); ++yi )
			{
				s = (QRgb*)(tmpImg.scanLine( yi ));
				d = (QRgb*)(m_image.scanLine( yi ));
				for(int xi=0; xi < tmpImg.width(); ++xi )
				{
					(*d) = (*s);
					s++;
					d++;
				}
			}
		}
	}
	(void) jpeg_finish_decompress(&cinfo);
	fclose (infile);
	jpeg_destroy_decompress (&cinfo);
	return (!m_image.isNull());
}

bool ScImgDataLoader_PS::loadPSjpeg(QString fn, QImage &tmpImg)
{
	if (!QFile::exists(fn))
		return false;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr         jerr;
	FILE     *infile;
	cinfo.err = jpeg_std_error (&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	infile = NULL;
	if (setjmp (jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress (&cinfo);
		if (infile)
			fclose (infile);
		return false;
	}
	jpeg_create_decompress (&cinfo);
	if ((infile = fopen (fn.toLocal8Bit(), "rb")) == NULL)
		return false;
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, true);
	jpeg_start_decompress(&cinfo);
	if ( cinfo.output_components == 3 || cinfo.output_components == 4)
		tmpImg = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_ARGB32 );
	else if ( cinfo.output_components == 1 )
	{
		tmpImg = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_Indexed8 );
		tmpImg.setNumColors(256);
		for (int i=0; i<256; i++)
			tmpImg.setColor(i, qRgb(i,i,i));
	}
	if (!tmpImg.isNull())
	{
		uchar* data = tmpImg.bits();
		int bpl = tmpImg.bytesPerLine();
		while (cinfo.output_scanline < cinfo.output_height)
		{
			uchar *d = data + cinfo.output_scanline * bpl;
			(void) jpeg_read_scanlines(&cinfo, &d, 1);
		}
		if ( cinfo.output_components == 3 )
		{
			uchar *in;
			QRgb *out;
			for (uint j=0; j<cinfo.output_height; j++)
			{
				in = tmpImg.scanLine(j) + cinfo.output_width * 3;
				out = (QRgb*) tmpImg.scanLine(j);
				for (uint i=cinfo.output_width; i--; )
				{
					in -= 3;
					out[i] = qRgb(in[0], in[1], in[2]);
				}
			}
		}
		if ( cinfo.output_components == 4 )
		{
			QRgb *ptr;
			unsigned char c, m, y ,k;
			unsigned char *p;
			for (int i = 0; i < tmpImg.height(); i++)
			{
				ptr = (QRgb*)  tmpImg.scanLine(i);
				if ((cinfo.jpeg_color_space == JCS_YCCK) || ((cinfo.jpeg_color_space == JCS_CMYK) && (cinfo.saw_Adobe_marker)))
				{
					for (int j = 0; j <  tmpImg.width(); j++)
					{
						p = (unsigned char *) ptr;
						c = p[0];
						m = p[1];
						y =  p[2];
						k =  p[3];
						*ptr = qRgba(c, m, y, k);
						ptr++;
					}
				}
				else
				{
					for (int j = 0; j <  tmpImg.width(); j++)
					{
						p = (unsigned char *) ptr;
						c = p[0];
						m = p[1];
						y =  p[2];
						k =  p[3];
						*ptr = qRgba(y, m, c, k);
						ptr++;
					}
				}
			}
		}
		if ( cinfo.output_components == 1 )
		{
			QImage tmpImg2 = tmpImg.convertToFormat(QImage::Format_ARGB32);
			tmpImg = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_ARGB32 );
			QRgb *s;
			QRgb *d;
			for( int yi=0; yi < tmpImg2.height(); ++yi )
			{
				s = (QRgb*)(tmpImg2.scanLine( yi ));
				d = (QRgb*)(tmpImg.scanLine( yi ));
				for(int xi=0; xi < tmpImg2.width(); ++xi )
				{
					(*d) = (*s);
					s++;
					d++;
				}
			}
		}
	}
	(void) jpeg_finish_decompress(&cinfo);
	fclose (infile);
	jpeg_destroy_decompress (&cinfo);
	return (!tmpImg.isNull());
}

void ScImgDataLoader_PS::loadPhotoshopBinary(QString fn)
{
	double x, y, b, h;
	QTextStream ts2(&BBox, QIODevice::ReadOnly);
	ts2 >> x >> y >> b >> h;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc1.jpg");
	QFile f2(tmpFile);
	QString tmp;
	m_image = QImage(psXSize, psYSize, QImage::Format_ARGB32);
	m_image.fill(qRgba(0, 0, 0, 0));
	m_imageInfoRecord.xres = qRound(psXSize / b * 72.0);
	m_imageInfoRecord.yres = qRound(psYSize / h * 72.0);
	QByteArray psdata;
	QFile f(fn);
	int yCount = 0;
	if (f.open(QIODevice::ReadOnly))
	{
		if (psDataType > 2)
		{
			f2.open(QIODevice::WriteOnly);
		}
		QDataStream ts(&f);
		while (!ts.atEnd())
		{
			tmp = readLinefromDataStream(ts);
			if (tmp == psCommand)
			{
				if (psDataType == 1)
				{
					QRgb *p;
					uchar cc, cm, cy, ck;
					for (int yh = 0; yh < m_image.height(); ++yh )
					{
						if (psMode == 4)
							psdata.resize(psXSize * (4 + psChannel));
						else
							psdata.resize(psXSize * (3 + psChannel));
						f.read(psdata.data(), psdata.size());
						p = (QRgb *)m_image.scanLine( yh );
						for (int xh = 0; xh < m_image.width(); ++xh )
						{
							cc = psdata[xh];
							cm = psdata[psXSize+xh];
							cy = psdata[psXSize*2+xh];
							ck = psdata[psXSize*3+xh];
							if (psMode == 4)
								*p = qRgba(cc, cm, cy, ck);
							else
								*p = qRgba(cc, cm, cy, 255);
							p++;
						}
					}
				}
				else if (psDataType > 1)
				{
					while (!ts.atEnd())
					{
						tmp = readLinefromDataStream(ts);
						if ((tmp.isEmpty()) || (tmp.startsWith("%%EndBinary")))
							break;
						if (psDataType == 2)
						{
							for (int a = 0; a < tmp.length(); a += 2)
							{
								bool ok;
								ushort data = tmp.mid(a, 2).toUShort(&ok, 16);
								psdata.resize(psdata.size()+1);
								psdata[psdata.size()-1] = data;
							}
						}
						else
						{
							decodeA85(psdata, tmp);
							f2.write(psdata.data(), psdata.size());
							psdata.resize(0);
						}
					}
					if (psDataType > 2)
					{
						f2.close();
						loadPSjpeg(tmpFile);
						QFile::remove(tmpFile);
					}
					else
					{
						QRgb *p;
						uchar cc, cm, cy, ck;
						for (int yh = 0; yh < m_image.height(); ++yh )
						{
							p = (QRgb *)m_image.scanLine( yh );
							for (int xh = 0; xh < m_image.width(); ++xh )
							{
								cc = psdata[yCount+xh];
								cm = psdata[yCount+psXSize+xh];
								cy = psdata[yCount+psXSize*2+xh];
								if (psMode == 4)
								{
									ck = psdata[yCount+psXSize*3+xh];
									*p = qRgba(cc, cm, cy, ck);
								}
								else
									*p = qRgba(cc, cm, cy, 255);
								p++;
							}
							if (psMode == 4)
								yCount += psXSize * (4 + psChannel);
							else
								yCount += psXSize * (3 + psChannel);
						}
					}
				}
				if (psMode == 4)
					m_imageInfoRecord.colorspace = ColorSpaceCMYK;
				else
					m_imageInfoRecord.colorspace = ColorSpaceRGB;
				m_imageInfoRecord.type  = ImageType7;
				m_imageInfoRecord.BBoxX = 0;
				m_imageInfoRecord.BBoxH = m_image.height();
				m_image.setDotsPerMeterX ((int) (m_imageInfoRecord.xres / 0.0254));
				m_image.setDotsPerMeterY ((int) (m_imageInfoRecord.yres / 0.0254));
				f.close();
				return;
			}
		}
		f.close();
	}
}

void ScImgDataLoader_PS::loadPhotoshopBinary(QString fn, QImage &tmpImg)
{
	double x, y, b, h;
	QTextStream ts2(&BBox, QIODevice::ReadOnly);
	ts2 >> x >> y >> b >> h;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc1.jpg");
	QFile f2(tmpFile);
	QString tmp;
	tmpImg = QImage(psXSize, psYSize, QImage::Format_ARGB32);
	tmpImg.fill(qRgba(0, 0, 0, 0));
	QByteArray psdata;
	QFile f(fn);
	int yCount = 0;
	if (f.open(QIODevice::ReadOnly))
	{
		if (psDataType > 2)
		{
			f2.open(QIODevice::WriteOnly);
		}
		QDataStream ts(&f);
		while (!ts.atEnd())
		{
			tmp = readLinefromDataStream(ts);
			if (tmp == psCommand)
			{
				if (psDataType == 1)
				{
					QRgb *p;
					uchar cc, cm, cy, ck;
					for (int yh = 0; yh < tmpImg.height(); ++yh )
					{
						if (psMode == 4)
							psdata.resize(psXSize * (4 + psChannel));
						else
							psdata.resize(psXSize * (3 + psChannel));
						f.read(psdata.data(), psdata.size());
						p = (QRgb *)tmpImg.scanLine( yh );
						for (int xh = 0; xh < tmpImg.width(); ++xh )
						{
							cc = psdata[xh];
							cm = psdata[psXSize+xh];
							cy = psdata[psXSize*2+xh];
							if (psMode == 4)
							{
								ck = psdata[psXSize*3+xh];
								*p = qRgba(cc, cm, cy, ck);
							}
							else
								*p = qRgba(cc, cm, cy, 255);
							p++;
						}
					}
				}
				else if (psDataType > 1)
				{
					while (!ts.atEnd())
					{
						tmp = readLinefromDataStream(ts);
						if ((tmp.isEmpty()) || (tmp.startsWith("%%EndBinary")))
							break;
						if (psDataType == 2)
						{
							for (int a = 0; a < tmp.length(); a += 2)
							{
								bool ok;
								ushort data = tmp.mid(a, 2).toUShort(&ok, 16);
								psdata.resize(psdata.size()+1);
								psdata[psdata.size()-1] = data;
							}
						}
						else
						{
							decodeA85(psdata, tmp);
							f2.write(psdata.data(), psdata.size());
							psdata.resize(0);
						}
					}
					if (psDataType > 2)
					{
						f2.close();
						loadPSjpeg(tmpFile, tmpImg);
						QFile::remove(tmpFile);
					}
					else
					{
						QRgb *p;
						uchar cc, cm, cy, ck;
						for (int yh = 0; yh < tmpImg.height(); ++yh )
						{
							p = (QRgb *)tmpImg.scanLine( yh );
							for (int xh = 0; xh < tmpImg.width(); ++xh )
							{
								cc = psdata[yCount+xh];
								cm = psdata[yCount+psXSize+xh];
								cy = psdata[yCount+psXSize*2+xh];
								if (psMode == 4)
								{
									ck = psdata[yCount+psXSize*3+xh];
									*p = qRgba(cc, cm, cy, ck);
								}
								else
									*p = qRgba(cc, cm, cy, 255);
								p++;
							}
							if (psMode == 4)
								yCount += psXSize * (4 + psChannel);
							else
								yCount += psXSize * (3 + psChannel);
						}
					}
				}
				f.close();
				return;
			}
		}
		f.close();
	}
}

void ScImgDataLoader_PS::loadDCS2(QString fn, int gsRes)
{
	QStringList args;
	double x, y, b, h;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc1.png");
	QString tmpFile2 = QDir::convertSeparators(ScPaths::getTempFileDir() + "tmp.eps");
	QString baseFile = fi.absolutePath();
	QString picFile = QDir::convertSeparators(fn);
	float xres = gsRes;
	float yres = gsRes;
	QTextStream ts2(&BBox, QIODevice::ReadOnly);
	ts2 >> x >> y >> b >> h;
	xres = gsRes;
	yres = gsRes;
	if ((isPhotoshop) && (hasPhotoshopImageData))
	{
		m_image = QImage(psXSize, psYSize, QImage::Format_ARGB32);
		xres = psXSize / b * 72.0;
		yres = psYSize / h * 72.0;
	}
	else
		m_image = QImage( qRound(b * gsRes / 72.0), qRound(h * gsRes / 72.0), QImage::Format_ARGB32 );
	m_image.fill(qRgba(0, 0, 0, 0));
	if (!isDCS2multi)
	{
		for (QMap<QString, plateOffsets>::Iterator it = colorPlates2.begin(); it != colorPlates2.end(); ++it)
		{
			QByteArray imgc(it.value().len, ' ');
			QFile f(picFile);
			if (f.open(QIODevice::ReadOnly))
			{
				f.seek(it.value().pos);
				uint readB = f.read(imgc.data(), it.value().len);
				readB = 0;
			}
			f.close();
			QFile f2(tmpFile2);
			if (f2.open(QIODevice::WriteOnly))
				f2.write(imgc.data(), it.value().len);
			f2.close();
			imgc.resize(0);
			if ((isPhotoshop) && (hasPhotoshopImageData))
			{
				QImage tmpImg;
				loadPhotoshopBinary(tmpFile2, tmpImg);
				blendImages(tmpImg, CustColors[it.key()]);
			}
			else
			{
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
					QFile::remove(tmpFile);
				}
			}
			QFile::remove(tmpFile2);
		}
	}
	else
	{
		for (QMap<QString, QString>::Iterator it = colorPlates.begin(); it != colorPlates.end(); ++it)
		{
			tmpFile2 = QDir::convertSeparators(baseFile+"/"+it.value());
			if ((isPhotoshop) && (hasPhotoshopImageData))
			{
				QImage tmpImg;
				loadPhotoshopBinary(tmpFile2, tmpImg);
				blendImages(tmpImg, CustColors[it.key()]);
			}
			else
			{
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
					QFile::remove(tmpFile);
				}
				args.clear();
			}
		}
	}
	if (extensionIndicatesEPS(ext))
	{
		m_imageInfoRecord.BBoxX = static_cast<int>(x);
		m_imageInfoRecord.BBoxH = static_cast<int>(h);
	}
	else
	{
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = m_image.height();
	}
	m_imageInfoRecord.xres = qRound(xres);
	m_imageInfoRecord.yres = qRound(yres);
	m_imageInfoRecord.colorspace = ColorSpaceCMYK;
	m_imageInfoRecord.type = ImageType7;
	m_image.setDotsPerMeterX ((int) (xres / 0.0254));
	m_image.setDotsPerMeterY ((int) (yres / 0.0254));
}

void ScImgDataLoader_PS::loadDCS1(QString fn, int gsRes)
{
	QStringList args;
	double x, y, b, h;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc1.png");
	QString baseFile = fi.absolutePath();
	QString picFile;
	float xres = gsRes;
	float yres = gsRes;
	QTextStream ts2(&BBox, QIODevice::ReadOnly);
	ts2 >> x >> y >> b >> h;
	xres = gsRes;
	yres = gsRes;
	m_image = QImage( qRound(b * gsRes / 72.0), qRound(h * gsRes / 72.0), QImage::Format_ARGB32 );
	m_image.fill(qRgba(0, 0, 0, 0));
	bool isEncapPS=extensionIndicatesEPS(ext);
	if (isEncapPS)
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
		QFile::remove(tmpFile);
	}
	args.clear();

	if (isEncapPS)
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
		QFile::remove(tmpFile);
	}
	args.clear();

	if (isEncapPS)
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
		QFile::remove(tmpFile);
	}
	args.clear();

	if (isEncapPS)
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
		QFile::remove(tmpFile);
	}
	args.clear();

	if (isEncapPS)
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
	m_imageInfoRecord.colorspace = ColorSpaceCMYK;
	m_imageInfoRecord.type = ImageType7;
	m_image.setDotsPerMeterX ((int) (xres / 0.0254));
	m_image.setDotsPerMeterY ((int) (yres / 0.0254));
}

void ScImgDataLoader_PS::blendImages(QImage &source, ScColor col)
{
	int h = source.height();
	int w = source.width();
	int cyan, c, m, yc, k, cc, mm, yy, kk;
	col.getCMYK(&c, &m, &yc, &k);
	QRgb *p;
	QRgb *pq;
	for (int y=0; y < h; ++y )
	{
		p = (QRgb *)m_image.scanLine( y );
		pq = (QRgb *)source.scanLine( y );
		for (int x=0; x < w; ++x )
		{
			cyan = 255 - qRed(*pq);
			if (cyan != 0)
			{
				(c == 0) ? cc = qRed(*p) : cc = qMin(c * cyan / 255 + qRed(*p), 255);
				(m == 0) ? mm = qGreen(*p) : mm = qMin(m * cyan / 255 + qGreen(*p), 255);
				(yc == 0) ? yy = qBlue(*p) : yy = qMin(yc * cyan / 255 + qBlue(*p), 255);
				(k == 0) ? kk = qAlpha(*p) : kk = qMin(k * cyan / 255 + qAlpha(*p), 255);
				*p = qRgba(cc, mm, yy, kk);
			}
			p++;
			pq++;
		}
	}
}

bool ScImgDataLoader_PS::preloadAlphaChannel(const QString& fn, int page, int gsRes, bool& hasAlpha)
{
	float xres, yres;

	initialize();

	hasAlpha = false;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return false;
	QString ext = fi.suffix().toLower();
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + QString("sc%1.png").arg(qMax(1, page)));
	QString tmpFiles = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc%d.png");
	QString picFile = QDir::convertSeparators(fn);
	double x, y, b, h;
	bool found = false;
	found = parseData(fn);
	if (found)
	{
		QTextStream ts2(&BBox, QIODevice::ReadOnly);
		ts2 >> x >> y >> b >> h;
		h = h * gsRes / 72.0;
		QStringList args;
		xres = gsRes;
		yres = gsRes;
		if (extensionIndicatesEPS(ext))
		{
			if (!BBoxInTrailer)
				args.append("-dEPSCrop");
		}
		args.append("-r"+QString::number(gsRes));
		args.append("-sOutputFile="+tmpFiles);
		args.append(picFile);
//		qDebug() << "scimgdataloader_ps(alpha):" << args;
		int retg = callGS(args);
		if (retg == 0)
		{
			m_image.load(tmpFile);
			if (extensionIndicatesEPS(ext) && BBoxInTrailer)
			{
				int ex = qRound(x * gsRes / 72.0);
				int ey = qRound(m_image.height() - h);
				int ew = qRound((b - x) * gsRes / 72.0);
				int eh = qRound(h - y * gsRes / 72.0);
				m_image = m_image.copy(ex, ey, ew, eh);
			}
			if (!ScCore->havePNGAlpha())
			{
				int wi = m_image.width();
				int hi = m_image.height();
				QRgb alphaFF = qRgba(255,255,255,255);
				QRgb alpha00 = qRgba(255,255,255,  0);
				QRgb *s;
				for( int yi=0; yi < hi; ++yi )
				{
					s = (QRgb*)(m_image.scanLine( yi ));
					for(int xi=0; xi < wi; ++xi )
					{
						if((*s) == alphaFF)
							(*s) &= alpha00;
						s++;
					}
				}
			}
			
			QStringList files = QStringList("sc*.png");
			files = QDir(ScPaths::getTempFileDir()).entryList(files);
			for (int i=0; i < files.count(); ++i)
				QFile::remove(QDir::convertSeparators(ScPaths::getTempFileDir() + files[i]));

			hasAlpha = true;
			m_imageInfoRecord.actualPageNumber = page;
			return true;
		}
		else
		{
			qDebug() << "Ghostscript returned result" << retg;
		}
		return false;
	}
	m_imageInfoRecord.actualPageNumber = page;
	return true;
}
