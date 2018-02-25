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

#include "cmsettings.h"
#include "colormgmt/sccolormgmtengine.h"
#include "scclocale.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scimgdataloader_ps.h"
#include "sctextstream.h"
#include "prefsmanager.h"
#include "util.h"
#include "util_formats.h"
#include "util_ghostscript.h"
#include "util_math.h"
#include "scimage.h"

extern "C"
{
#define XMD_H           // shut JPEGlib up
#if defined(Q_OS_UNIXWARE)
#  define HAVE_BOOLEAN  // libjpeg under Unixware seems to need this
#endif
#include <jpeglib.h>
#include <jerror.h>
#include <csetjmp>
#undef HAVE_STDLIB_H
#ifdef const
#  undef const          // remove crazy C hackery in jconfig.h
#endif
}

ScImgDataLoader_PS::ScImgDataLoader_PS(void) : ScImgDataLoader(),
	 m_isDCS1(false),
	 m_isDCS2(false),
	 m_isDCS2multi(false),
	 m_isPhotoshop(false),
	 m_hasPhotoshopImageData(false),
	 m_doThumbnail(false),
	 m_hasThumbnail(false),
	 m_inTrailer(false),
	 m_BBoxInTrailer(false),
	 m_isRotated(false),
	 m_psXSize(0),
	 m_psYSize(0),
	 m_psDepth(0),
	 m_psMode(0),
	 m_psChannel(0),
	 m_psBlock(0),
	 m_psDataType(0)
{
	initSupportedFormatList();
}

void ScImgDataLoader_PS::initialize(void)
{
	m_doThumbnail = false;
	m_hasThumbnail = false;

	ScImgDataLoader::initialize();
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
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
	if ( !QFile::exists(fn) )
		return;
	QFile f(fn);
	if (!f.open(QIODevice::ReadOnly))
		return;

	QString tmp;
	QDataStream ts(&f);
	while (!ts.atEnd())
	{
		tmp = readLineFromDataStream(ts);
		if (tmp.startsWith("%%BeginICCProfile:"))
		{
			QByteArray psdata;
			while (!ts.atEnd())
			{
				tmp = readLineFromDataStream(ts);
				for (int a = 2; a < tmp.length(); a += 2)
				{
					bool ok;
					ushort data = tmp.midRef(a, 2).toUShort(&ok, 16);
					psdata.resize(psdata.size()+1);
					psdata[psdata.size()-1] = data;
				}
				if (tmp.startsWith("%%EndICCProfile"))
				{
					ScColorMgmtEngine engine(ScCore->defaultEngine);
					ScColorProfile prof = engine.openProfileFromMem(psdata);
					if (prof)
					{
						if (prof.colorSpace() == ColorSpace_Rgb)
							m_profileComponents = 3;
						if (prof.colorSpace() == ColorSpace_Cmyk)
							m_profileComponents = 4;
						m_imageInfoRecord.profileName = prof.productDescription();
						m_imageInfoRecord.isEmbedded = true;
						m_embeddedProfile = psdata;
					}
					break;
				}
			}
		}
	}
}

void ScImgDataLoader_PS::scanForFonts(QString fn)
{
	QFile f(fn);
	if (!f.open(QIODevice::ReadOnly))
		return;
	QDataStream ts(&f);
	QString tmp;
	while (!ts.atEnd())
	{
		tmp = readLineFromDataStream(ts);
		if (tmp.startsWith("%%BeginFont:"))
		{
			tmp = tmp.remove("%%BeginFont:");
			ScTextStream ts2(&tmp, QIODevice::ReadOnly);
			QString tmp2;
			ts2 >> tmp2;
			m_FontListe.removeAll(tmp2);
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
	m_isDCS1 = false;
	m_isDCS2 = false;
	m_isDCS2multi = false;
	m_isPhotoshop = false;
	m_hasPhotoshopImageData = false;
	m_hasThumbnail = false;
	m_inTrailer = false;
	m_BBoxInTrailer = false;
	m_isRotated = false;
	int plateCount = 0;
	uint startPos = 0;
	m_FontListe.clear();
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		QByteArray tempBuf(9, ' ');
		f.read(tempBuf.data(), 8);
		if (getDouble(tempBuf.mid(0, 4), true) == 0xC5D0D3C6)
		{
			startPos = getDouble(tempBuf.mid(4, 4), false);
			if (m_doThumbnail)
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
					f.read(imgc.data(), thumbLen);
					QString tmpFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "preview.tiff");
					QFile f2(tmpFile);
					if (f2.open(QIODevice::WriteOnly))
						f2.write(imgc.data(), thumbLen);
					f2.close();
					imgc.resize(0);
					ScImage thum;
					CMSettings cms(0, "", Intent_Perceptual);
					cms.allowColorManagement(false);
					bool mode = true;
					if (thum.loadPicture(tmpFile, 1, cms, ScImage::RGBData, 72, &mode))
					{
						m_imageInfoRecord.exifDataValid = true;
						m_imageInfoRecord.exifInfo.thumbnail = thum.qImage().copy();
					}
					QFile::remove(tmpFile);
					m_hasThumbnail = true;
				}
			}
		}
		bool psFound = false;
		bool isAtend = false;
		QDataStream ts(&f);
		ts.device()->seek(startPos);
		while (!ts.atEnd())
		{
			tmp = readLineFromDataStream(ts);
			if (tmp.startsWith("%%Creator: "))
				m_Creator = tmp.remove("%%Creator: ");
			if (tmp.startsWith("%%Pages: "))
			{
				tmp = tmp.remove("%%Pages: ");
				bool ok;
				int pages = tmp.toInt( &ok );
				if (ok)
					m_imageInfoRecord.numberOfPages = pages; 
			}
			if (tmp.startsWith("%%Trailer"))
				m_inTrailer = true;
			if (tmp.startsWith("%%BoundingBox:"))
			{
				found = true;
				if (m_inTrailer)
					m_BBoxInTrailer = true;
				m_BBox = tmp.remove("%%BoundingBox:");
			}
			if (!found)
			{
				if (tmp.startsWith("%%BoundingBox"))
				{
					found = true;
					if (m_inTrailer)
						m_BBoxInTrailer = true;
					m_BBox = tmp.remove("%%BoundingBox");
				}
			}
			if (tmp.startsWith("%%Orientation:"))
			{
				if (tmp.contains("Landscape"))
					m_isRotated = true;
			}
			if (tmp.startsWith("%%CyanPlate:"))
			{
				m_colorPlates.insert("Cyan", tmp.remove("%%CyanPlate: "));
				m_isDCS1 = true;
			}
			if (tmp.startsWith("%%MagentaPlate:"))
			{
				m_colorPlates.insert("Magenta", tmp.remove("%%MagentaPlate: "));
				m_isDCS1 = true;
			}
			if (tmp.startsWith("%%YellowPlate:"))
			{
				m_colorPlates.insert("Yellow", tmp.remove("%%YellowPlate: "));
				m_isDCS1 = true;
			}
			if (tmp.startsWith("%%BlackPlate:"))
			{
				m_colorPlates.insert("Black", tmp.remove("%%BlackPlate: "));
				m_isDCS1 = true;
			}
			if (tmp.startsWith("%%PlateFile: ("))
			{
				tmp = tmp.remove("%%PlateFile: (");
				int endNam = tmp.indexOf(")");
				QString plateNam = tmp.left(endNam);
				tmp = tmp.remove(plateNam+")");
				ScTextStream ts2(&tmp, QIODevice::ReadOnly);
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
						if (m_Creator.contains("Photoshop Version 9"))	// This is very strange, it seems that there is a bug in PS 9 which writes weired entries
						{
							pos -= (191 + plateCount * 83);
							len -= 83;
						}
						offs.pos = pos;
						offs.len = len;
						m_colorPlates2.insert(plateNam, offs);
						m_isDCS2 = true;
						plateCount++;
					}
					else
					{
						m_colorPlates.insert(plateNam, lenStr);
						m_isDCS2 = true;
						m_isDCS2multi = true;
					}
				}
			}
			if (tmp.startsWith("%%DocumentFonts:"))
			{
				tmp = tmp.remove("%%DocumentFonts:");
				ScTextStream ts2(&tmp, QIODevice::ReadOnly);
				QString tmp2;
				ts2 >> tmp2;
				if (!tmp2.contains("(atend)"))
				{
					if (!tmp2.isEmpty())
						m_FontListe.append(tmp2);
					while (!ts.atEnd())
					{
						uint oldPos = ts.device()->pos();
						tmp = readLineFromDataStream(ts);
						if (!tmp.startsWith("%%+"))
						{
							ts.device()->seek(oldPos);
							break;
						}
						tmp = tmp.remove(0,3);
						ScTextStream ts2(&tmp, QIODevice::ReadOnly);
						QString tmp2;
						ts2 >> tmp2;
						if (!tmp2.isEmpty())
							m_FontListe.append(tmp2);
					}
				}
				else
					isAtend = true;
			}
			if (tmp.startsWith("%%CMYKCustomColor"))
			{
				tmp = tmp.remove(0,18);
				ScTextStream ts2(&tmp, QIODevice::ReadOnly);
				ts2 >> c >> m >> y >> k;
				FarNam = ts2.readAll();
				FarNam = FarNam.trimmed();
				FarNam = FarNam.remove(0,1);
				FarNam = FarNam.remove(FarNam.length()-1,1);
				cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
				cc.setSpotColor(true);
				m_CustColors.insert(FarNam, cc);
				while (!ts.atEnd())
				{
					uint oldPos = ts.device()->pos();
					tmp = readLineFromDataStream(ts);
					if (!tmp.startsWith("%%+"))
					{
						ts.device()->seek(oldPos);
						break;
					}
					tmp = tmp.remove(0,3);
					ScTextStream ts2(&tmp, QIODevice::ReadOnly);
					ts2 >> c >> m >> y >> k;
					FarNam = ts2.readAll();
					FarNam = FarNam.trimmed();
					FarNam = FarNam.remove(0,1);
					FarNam = FarNam.remove(FarNam.length()-1,1);
					cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
					cc.setSpotColor(true);
					m_CustColors.insert(FarNam, cc);
				}
			}
			if (tmp.startsWith("%%EndComments"))
			{
				while (!ts.atEnd())
				{
					tmp = readLineFromDataStream(ts);
					if ((!tmp.isEmpty()) && (!tmp.startsWith("%")))
					{
						psFound = true;
						break;
					}
					if (tmp.startsWith("%ImageData: "))
					{
						m_hasPhotoshopImageData = true;
						tmp.remove("%ImageData: ");
						ScTextStream ts2(&tmp, QIODevice::ReadOnly);
						ts2 >> m_psXSize >> m_psYSize >> m_psDepth >> m_psMode >> m_psChannel >> m_psBlock >> m_psDataType >> m_psCommand;
						m_psCommand = m_psCommand.remove(0,1);
						m_psCommand = m_psCommand.remove(m_psCommand.length()-1,1);
					}
					if (tmp.startsWith("%BeginPhotoshop"))
					{
						QByteArray psdata;
						while (!ts.atEnd())
						{
							tmp = readLineFromDataStream(ts);
							if (tmp.startsWith("%EndPhotoshop"))
							{
								QDataStream strPhot( &psdata, QIODevice::ReadOnly);
								strPhot.setByteOrder( QDataStream::BigEndian );
								PSDHeader fakeHeader;
								ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
								ts2 >> x >> y >> b >> h;
								fakeHeader.width = qRound(b);
								fakeHeader.height = qRound(h);
								parseRessourceData(strPhot, fakeHeader, psdata.size());
								m_imageInfoRecord.valid = (m_imageInfoRecord.PDSpathData.size()) > 0 ? true : false;
								if (m_imageInfoRecord.PDSpathData.size() > 0)
								{
									QTransform mm;
									mm.scale(m_imageInfoRecord.xres / 72.0, m_imageInfoRecord.yres / 72.0);
									QMap<QString, FPointArray>::Iterator it;
									for (it = m_imageInfoRecord.PDSpathData.begin(); it != m_imageInfoRecord.PDSpathData.end(); ++it)
									{
										it.value().map(mm);
									}
								}
								m_isPhotoshop = true;
								break;
							}
							for (int a = 2; a < tmp.length(); a += 2)
							{
								bool ok;
								ushort data = tmp.midRef(a, 2).toUShort(&ok, 16);
								psdata.resize(psdata.size()+1);
								psdata[psdata.size()-1] = data;
							}
						}
						if ((m_doThumbnail) && ((m_hasThumbnail) || (!m_imageInfoRecord.exifInfo.thumbnail.isNull())))
							return true;
					}
					if (tmp.startsWith("%%BeginICCProfile:"))
					{
						QByteArray psdata;
						while (!ts.atEnd())
						{
							tmp = readLineFromDataStream(ts);
							for (int a = 2; a < tmp.length(); a += 2)
							{
								bool ok;
								ushort data = tmp.midRef(a, 2).toUShort(&ok, 16);
								psdata.resize(psdata.size()+1);
								psdata[psdata.size()-1] = data;
							}
							if (tmp.startsWith("%%EndICCProfile"))
							{
								ScColorMgmtEngine engine(ScCore->defaultEngine);
								ScColorProfile prof = engine.openProfileFromMem(psdata);
								if (prof)
								{
									if (prof.colorSpace() == ColorSpace_Rgb)
										m_profileComponents = 3;
									if (prof.colorSpace() == ColorSpace_Cmyk)
										m_profileComponents = 4;
									m_imageInfoRecord.profileName = prof.productDescription();
									m_imageInfoRecord.isEmbedded = true;
									m_embeddedProfile = psdata;
								}
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
	return true;
}

bool ScImgDataLoader_PS::loadPicture(const QString& fn, int page, int gsRes, bool thumbnail)
{
	double x = 0;
	double y = 0;
	double b = 0;
	double h = 0;
	bool found = false;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return false;
	QString ext = fi.suffix().toLower();
	if (ext.isEmpty())
		ext = getImageType(fn);
	QString tmpFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + QString("sc%1.png").arg(qMax(1, page)));
	QString tmpFiles = QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc%d.png");
	QString picFile = QDir::toNativeSeparators(fn);
	float xres = gsRes;
	float yres = gsRes;

	initialize();

	m_imageInfoRecord.type = ImageTypeEPS;
	m_imageInfoRecord.exifDataValid = false;
	m_imageInfoRecord.numberOfPages = 1; // will be overwritten by parse()
	m_doThumbnail = thumbnail;
	m_colorPlates2.clear();
	m_colorPlates.clear();
	m_CustColors.clear();
	m_CustColors.insert("Cyan", ScColor(255, 0, 0, 0));
	m_CustColors.insert("Magenta", ScColor(0, 255, 0, 0));
	m_CustColors.insert("Yellow", ScColor(0, 0, 255, 0));
	m_CustColors.insert("Black", ScColor(0, 0, 0, 255));
	found = parseData(fn);
	if (m_FontListe.count() != 0)
	{
		scanForFonts(fn);
		if (m_FontListe.count() != 0)
		{
			bool missing = false;
			QString missingF = "";
			for (int fo = 0; fo < m_FontListe.count(); fo++)
			{
				if (!PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.contains(m_FontListe[fo]))
				{
					missing = true;
					missingF += m_FontListe[fo]+"\n";
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
		ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
		ts2 >> x >> y >> b >> h;
		m_imageInfoRecord.exifInfo.width = qRound(b);
		m_imageInfoRecord.exifInfo.height = qRound(h);
		m_image = m_imageInfoRecord.exifInfo.thumbnail;
		if ((m_isPhotoshop) && (m_hasPhotoshopImageData))
		{
			m_imageInfoRecord.exifInfo.width = m_psXSize;
			m_imageInfoRecord.exifInfo.height = m_psYSize;
			m_imageInfoRecord.type = ImageType7;
			if (m_psMode == 4)
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
		m_pixelFormat = Format_BGRA_8;
		return true;
	}
	if (found)
	{
		if (m_isDCS1)
			loadDCS1(fn, gsRes);
		else if (m_isDCS2)
			loadDCS2(fn, gsRes);
		else if ((m_isPhotoshop) && (m_hasPhotoshopImageData))
			loadPhotoshop(fn, gsRes);
		else if ((!m_imageInfoRecord.isEmbedded) || ((m_imageInfoRecord.isEmbedded) && (m_profileComponents == 3)))
		{
			ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
			ts2 >> x >> y >> b >> h;
			QStringList args;
			xres = gsRes;
			yres = gsRes;
			if (extensionIndicatesEPS(ext))
			{
				if (!m_BBoxInTrailer)
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
				if ((extensionIndicatesEPS(ext) && m_BBoxInTrailer) || (m_isRotated))
				{
					int ex = qRound(x * gsRes / 72.0);
					int ey = qRound(m_image.height() - h);
					int ew = qRound((b - x) * gsRes / 72.0);
					int eh = qRound(h - y * gsRes / 72.0);
					m_image = m_image.copy(ex, ey, ew, eh);
				}
				if ((!ScCore->havePNGAlpha()) || (m_isRotated))
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
				files = QDir(ScPaths::tempFileDir()).entryList(files);
				for (int i=0; i < files.count(); ++i)
					QFile::remove(QDir::toNativeSeparators(ScPaths::tempFileDir() + files[i]));
				
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
				m_imageInfoRecord.xres = gsRes;
				m_imageInfoRecord.yres = gsRes;
				if ((m_imageInfoRecord.isEmbedded) && (m_profileComponents == 3))
					m_imageInfoRecord.type = ImageType7;
				m_imageInfoRecord.colorspace = ColorSpaceRGB;
				m_image.setDotsPerMeterX ((int) (xres / 0.0254));
				m_image.setDotsPerMeterY ((int) (yres / 0.0254));
				m_pixelFormat = Format_BGRA_8;
			}
		}
		else
		{
			ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
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
			int retg = callGS(args);
			if (retg == 0)
			{
				m_image.load(tmpFile);
				x = 0;
				y = 0;
				b = m_image.width() / gsRes * 72.0;
				h = m_image.height() / gsRes * 72.0;
			}
			retg = callGS(args, "bitcmyk");
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
				files = QDir(ScPaths::tempFileDir()).entryList(files);
				for (int i=0; i < files.count(); ++i)
					QFile::remove(QDir::toNativeSeparators(ScPaths::tempFileDir() + files[i]));
				
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
				m_imageInfoRecord.xres = gsRes;
				m_imageInfoRecord.yres = gsRes;
				m_imageInfoRecord.colorspace = ColorSpaceCMYK;
				m_imageInfoRecord.type = ImageType7;
				m_image.setDotsPerMeterX ((int) (xres / 0.0254));
				m_image.setDotsPerMeterY ((int) (yres / 0.0254));
				m_pixelFormat = Format_YMCK_8;
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
	if ((m_psDataType >= 1) && (m_psDataType <= 6) && ((m_psMode == 3) || (m_psMode == 4)))
	{
		loadPhotoshopBinary(fn);
		return;
	}
	QStringList args;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	QString tmpFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc1.png");
	int retg;
	int GsMajor;
	int GsMinor;
	getNumericGSVersion(GsMajor, GsMinor);
	ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
	double x, y, b, h;
	ts2 >> x >> y >> b >> h;
	h = h * gsRes / 72.0;
	if (extensionIndicatesEPS(ext))
		args.append("-dEPSCrop");
	if (m_psMode == 4)
		args.append("-dGrayValues=256");
	if ((GsMajor >= 8) && (GsMinor >= 53))
		args.append("-dNOPSICC");		// prevent GS from applying an embedded ICC profile as it will be applied later on in ScImage.
	args.append("-r"+QString::number(gsRes));
	args.append("-sOutputFile=" + tmpFile);
	args.append(QDir::toNativeSeparators(fn));
	if (m_psMode == 4)
		retg = callGS(args, "bitcmyk");
	else
		retg = callGS(args);
	if (retg == 0)
	{
		if (m_psMode == 4)
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
			m_pixelFormat = Format_YMCK_8;
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
			m_pixelFormat = Format_BGRA_8;
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
#if defined(Q_OS_WIN32)
	if ((infile = _wfopen((const wchar_t*) fn.utf16(), L"rb")) == NULL)
		return false;
#else
	if ((infile = fopen (fn.toLocal8Bit(), "rb")) == NULL)
		return false;
#endif
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, true);
	jpeg_start_decompress(&cinfo);
	if ( cinfo.output_components == 3 || cinfo.output_components == 4)
		m_image = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_ARGB32 );
	else if ( cinfo.output_components == 1 )
	{
		m_image = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_Indexed8 );
		m_image.setColorCount(256);
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
			m_pixelFormat = Format_BGRA_8;
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
			m_pixelFormat = Format_YMCK_8;
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
			m_pixelFormat = Format_BGRA_8;
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
#if defined(Q_OS_WIN32)
	if ((infile = _wfopen((const wchar_t*) fn.utf16(), L"rb")) == NULL)
		return false;
#else
	if ((infile = fopen (fn.toLocal8Bit(), "rb")) == NULL)
		return false;
#endif
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, true);
	jpeg_start_decompress(&cinfo);
	if ( cinfo.output_components == 3 || cinfo.output_components == 4)
		tmpImg = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_ARGB32 );
	else if ( cinfo.output_components == 1 )
	{
		tmpImg = QImage( cinfo.output_width, cinfo.output_height, QImage::Format_Indexed8 );
		m_image.setColorCount(256);
		for (int i=0; i<256; i++)
			tmpImg.setColor(i, qRgb(i,i,i));
		m_pixelFormat = Format_GRAY_8;
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
			m_pixelFormat = Format_BGRA_8;
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
			m_pixelFormat = Format_YMCK_8;
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
	ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
	ts2 >> x >> y >> b >> h;
	QString tmpFile(QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc1.jpg"));
	QFile f2(tmpFile);
	QString tmp;
	m_image = QImage(m_psXSize, m_psYSize, QImage::Format_ARGB32);
	m_image.fill(qRgba(0, 0, 0, 0));
	m_imageInfoRecord.xres = qRound(m_psXSize / b * 72.0);
	m_imageInfoRecord.yres = qRound(m_psYSize / h * 72.0);
	QByteArray psdata;
	QFile f(fn);
	int yCount = 0;
	if (!f.open(QIODevice::ReadOnly))
	{
		qDebug()<<"Failed to open QFile f in ScImgDataLoader_PS::loadPhotoshopBinary";
		return;
	}
	if (m_psDataType > 2)
	{
		if (!f2.open(QIODevice::WriteOnly))
		{
			qDebug()<<"Failed to open QFile f2 in ScImgDataLoader_PS::loadPhotoshopBinary";
			return;
		}
	}
	QDataStream ts(&f);
	while (!ts.atEnd())
	{
		tmp = readLineFromDataStream(ts);
		if (tmp == m_psCommand)
		{
			if (m_psDataType == 1)
			{
				QRgb *p;
				uchar cc, cm, cy, ck;
				for (int yh = 0; yh < m_image.height(); ++yh )
				{
					if (m_psMode == 4)
						psdata.resize(m_psXSize * (4 + m_psChannel));
					else
						psdata.resize(m_psXSize * (3 + m_psChannel));
					f.read(psdata.data(), psdata.size());
					p = (QRgb *)m_image.scanLine( yh );
					for (int xh = 0; xh < m_image.width(); ++xh )
					{
						cc = psdata[xh];
						cm = psdata[m_psXSize+xh];
						cy = psdata[m_psXSize*2+xh];
						ck = psdata[m_psXSize*3+xh];
						if (m_psMode == 4)
							*p = qRgba(cc, cm, cy, ck);
						else
							*p = qRgba(cc, cm, cy, 255);
						p++;
					}
				}
			}
			else if (m_psDataType > 1)
			{
				while (!ts.atEnd())
				{
					tmp = readLineFromDataStream(ts);
					if ((tmp.isEmpty()) || (tmp.startsWith("%%EndBinary")))
						break;
					if (m_psDataType == 2)
					{
						for (int a = 0; a < tmp.length(); a += 2)
						{
							bool ok;
							ushort data = tmp.midRef(a, 2).toUShort(&ok, 16);
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
				if (m_psDataType > 2)
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
							cm = psdata[yCount+m_psXSize+xh];
							cy = psdata[yCount+m_psXSize*2+xh];
							if (m_psMode == 4)
							{
								ck = psdata[yCount+m_psXSize*3+xh];
								*p = qRgba(cc, cm, cy, ck);
							}
							else
								*p = qRgba(cc, cm, cy, 255);
							p++;
						}
						if (m_psMode == 4)
							yCount += m_psXSize * (4 + m_psChannel);
						else
							yCount += m_psXSize * (3 + m_psChannel);
					}
				}
			}
			if (m_psMode == 4)
			{
				m_imageInfoRecord.colorspace = ColorSpaceCMYK;
				m_pixelFormat = Format_YMCK_8;
			}
			else
			{
				m_imageInfoRecord.colorspace = ColorSpaceRGB;
				m_pixelFormat = Format_BGRA_8;
			}
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

void ScImgDataLoader_PS::loadPhotoshopBinary(QString fn, QImage &tmpImg)
{
	double x, y, b, h;
	ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
	ts2 >> x >> y >> b >> h;
	QString tmpFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc1.jpg");
	QFile f2(tmpFile);
	QString tmp;
	tmpImg = QImage(m_psXSize, m_psYSize, QImage::Format_ARGB32);
	tmpImg.fill(qRgba(0, 0, 0, 0));
	QByteArray psdata;
	QFile f(fn);
	int yCount = 0;
	if (f.open(QIODevice::ReadOnly))
	{
		if (m_psDataType > 2)
		{
			f2.open(QIODevice::WriteOnly);
		}
		QDataStream ts(&f);
		while (!ts.atEnd())
		{
			tmp = readLineFromDataStream(ts);
			if (tmp == m_psCommand)
			{
				if (m_psDataType == 1)
				{
					QRgb *p;
					uchar cc, cm, cy, ck;
					for (int yh = 0; yh < tmpImg.height(); ++yh )
					{
						if (m_psMode == 4)
							psdata.resize(m_psXSize * (4 + m_psChannel));
						else
							psdata.resize(m_psXSize * (3 + m_psChannel));
						f.read(psdata.data(), psdata.size());
						p = (QRgb *)tmpImg.scanLine( yh );
						for (int xh = 0; xh < tmpImg.width(); ++xh )
						{
							cc = psdata[xh];
							cm = psdata[m_psXSize+xh];
							cy = psdata[m_psXSize*2+xh];
							if (m_psMode == 4)
							{
								ck = psdata[m_psXSize*3+xh];
								*p = qRgba(cc, cm, cy, ck);
							}
							else
								*p = qRgba(cc, cm, cy, 255);
							p++;
						}
					}
				}
				else if (m_psDataType > 1)
				{
					while (!ts.atEnd())
					{
						tmp = readLineFromDataStream(ts);
						if ((tmp.isEmpty()) || (tmp.startsWith("%%EndBinary")))
							break;
						if (m_psDataType == 2)
						{
							for (int a = 0; a < tmp.length(); a += 2)
							{
								bool ok;
								ushort data = tmp.midRef(a, 2).toUShort(&ok, 16);
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
					if (m_psDataType > 2)
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
								cm = psdata[yCount+m_psXSize+xh];
								cy = psdata[yCount+m_psXSize*2+xh];
								if (m_psMode == 4)
								{
									ck = psdata[yCount+m_psXSize*3+xh];
									*p = qRgba(cc, cm, cy, ck);
								}
								else
									*p = qRgba(cc, cm, cy, 255);
								p++;
							}
							if (m_psMode == 4)
								yCount += m_psXSize * (4 + m_psChannel);
							else
								yCount += m_psXSize * (3 + m_psChannel);
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
	QString tmpFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc1.png");
	QString tmpFile2 = QDir::toNativeSeparators(ScPaths::tempFileDir() + "tmp.eps");
	QString baseFile = fi.absolutePath();
	QString picFile = QDir::toNativeSeparators(fn);
	float xres = gsRes;
	float yres = gsRes;
	ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
	ts2 >> x >> y >> b >> h;
	xres = gsRes;
	yres = gsRes;
	if ((m_isPhotoshop) && (m_hasPhotoshopImageData))
	{
		m_image = QImage(m_psXSize, m_psYSize, QImage::Format_ARGB32);
		xres = m_psXSize / b * 72.0;
		yres = m_psYSize / h * 72.0;
	}
	else
		m_image = QImage( qRound(b * gsRes / 72.0), qRound(h * gsRes / 72.0), QImage::Format_ARGB32 );
	m_image.fill(qRgba(0, 0, 0, 0));
	if (!m_isDCS2multi)
	{
		for (QMap<QString, plateOffsets>::Iterator it = m_colorPlates2.begin(); it != m_colorPlates2.end(); ++it)
		{
			QByteArray imgc(it.value().len, ' ');
			QFile f(picFile);
			if (f.open(QIODevice::ReadOnly))
			{
				f.seek(it.value().pos);
				f.read(imgc.data(), it.value().len);
			}
			f.close();
			QFile f2(tmpFile2);
			if (f2.open(QIODevice::WriteOnly))
				f2.write(imgc.data(), it.value().len);
			f2.close();
			imgc.resize(0);
			if ((m_isPhotoshop) && (m_hasPhotoshopImageData))
			{
				QImage tmpImg;
				loadPhotoshopBinary(tmpFile2, tmpImg);
				blendImages(tmpImg, m_CustColors[it.key()]);
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
					blendImages(tmpImg, m_CustColors[it.key()]);
					QFile::remove(tmpFile);
				}
			}
			QFile::remove(tmpFile2);
		}
	}
	else
	{
		for (QMap<QString, QString>::Iterator it = m_colorPlates.begin(); it != m_colorPlates.end(); ++it)
		{
			tmpFile2 = QDir::toNativeSeparators(baseFile+"/"+it.value());
			if ((m_isPhotoshop) && (m_hasPhotoshopImageData))
			{
				QImage tmpImg;
				loadPhotoshopBinary(tmpFile2, tmpImg);
				blendImages(tmpImg, m_CustColors[it.key()]);
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
					blendImages(tmpImg, m_CustColors[it.key()]);
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
	m_pixelFormat = Format_YMCK_8;
}

void ScImgDataLoader_PS::loadDCS1(QString fn, int gsRes)
{
	QStringList args;
	double x, y, b, h;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.suffix().toLower();
	QString tmpFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc1.png");
	QString baseFile = fi.absolutePath();
	QString picFile;
	float xres = gsRes;
	float yres = gsRes;
	ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
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
	picFile = QDir::toNativeSeparators(baseFile+"/"+m_colorPlates["Cyan"]);
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
	picFile = QDir::toNativeSeparators(baseFile+"/"+m_colorPlates["Magenta"]);
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
	picFile = QDir::toNativeSeparators(baseFile+"/"+m_colorPlates["Yellow"]);
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
	picFile = QDir::toNativeSeparators(baseFile+"/"+m_colorPlates["Black"]);
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
	m_imageInfoRecord.xres = gsRes;
	m_imageInfoRecord.yres = gsRes;
	m_imageInfoRecord.colorspace = ColorSpaceCMYK;
	m_imageInfoRecord.type = ImageType7;
	m_image.setDotsPerMeterX ((int) (xres / 0.0254));
	m_image.setDotsPerMeterY ((int) (yres / 0.0254));
	m_pixelFormat = Format_YMCK_8;
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
	QString tmpFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + QString("sc%1.png").arg(qMax(1, page)));
	QString tmpFiles = QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc%d.png");
	QString picFile = QDir::toNativeSeparators(fn);
	double x, y, b, h;
	bool found = false;
	found = parseData(fn);
	if (found)
	{
		ScTextStream ts2(&m_BBox, QIODevice::ReadOnly);
		ts2 >> x >> y >> b >> h;
		h = h * gsRes / 72.0;
		QStringList args;
		xres = gsRes;
		yres = gsRes;
		if (extensionIndicatesEPS(ext))
		{
			if (!m_BBoxInTrailer)
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
			if ((extensionIndicatesEPS(ext) && m_BBoxInTrailer) || (m_isRotated))
			{
				int ex = qRound(x * gsRes / 72.0);
				int ey = qRound(m_image.height() - h);
				int ew = qRound((b - x) * gsRes / 72.0);
				int eh = qRound(h - y * gsRes / 72.0);
				m_image = m_image.copy(ex, ey, ew, eh);
			}
			if ((!ScCore->havePNGAlpha()) || (m_isRotated))
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
			files = QDir(ScPaths::tempFileDir()).entryList(files);
			for (int i=0; i < files.count(); ++i)
				QFile::remove(QDir::toNativeSeparators(ScPaths::tempFileDir() + files[i]));

			hasAlpha = true;
			m_imageInfoRecord.actualPageNumber = page;
			m_imageInfoRecord.type = ImageTypeEPS;
			m_imageInfoRecord.colorspace = ColorSpaceRGB;
			m_image.setDotsPerMeterX ((int) (xres / 0.0254));
			m_image.setDotsPerMeterY ((int) (yres / 0.0254));
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
