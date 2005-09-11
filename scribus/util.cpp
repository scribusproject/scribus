/***************************************************************************
                          util.cpp  -  description
                             -------------------
    begin                : Fri Sep 14 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "util.h"
#include <qbitmap.h>
#include <qpainter.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qdatastream.h>
#include <qregexp.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <algorithm>
#include <cstdlib>
#include <cmath>

#include "scconfig.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if defined(_WIN32)
#if defined(_MSC_VER)
#define __STDC__ 1 // hack to get md5_buffer correctly identified
#endif
#include <windows.h>
#endif

#include "md5.h"

#include <setjmp.h>
#include "qprocess.h"
#include "scpaths.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "prefsmanager.h"
#include "commonstrings.h"

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

#include "scribus.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
#ifdef HAVE_LIBZ
	#include <zlib.h>
#endif
#ifdef HAVE_TIFF
	#include <tiffio.h>
#endif
#ifdef HAVE_CMS
	#include CMS_INC
extern cmsHPROFILE CMSoutputProf;
extern cmsHPROFILE CMSprinterProf;
extern cmsHTRANSFORM stdTransG;
extern cmsHTRANSFORM stdProofG;
extern cmsHTRANSFORM stdTransImgG;
extern cmsHTRANSFORM stdProofImgG;
extern bool BlackPoint;
extern bool SoftProofing;
extern bool Gamut;
extern bool CMSuse;
extern int IntentMonitor;
extern int IntentPrinter;
#endif


using namespace std;

void sDebug(QString message)
{
	qDebug("%s", message.ascii());
}

QImage ProofImage(QImage *Image)
{
#ifdef HAVE_CMS
	QImage out = Image->copy();
	if ((CMSuse) && (SoftProofing))
	{
		int outheight=out.height();
		for (int i=0; i < outheight; ++i)
		{
			LPBYTE ptr = out.scanLine(i);
			cmsDoTransform(stdProofImgG, ptr, ptr, out.width());
		}
	}
	else
	{
		if (CMSuse)
		{
			int outheight=out.height();
			for (int i=0; i < outheight; ++i)
			{
				LPBYTE ptr = out.scanLine(i);
				cmsDoTransform(stdTransImgG, ptr, ptr, out.width());
			}
		}
	}
	return out;
#else
	return Image->copy();
#endif
}

/******************************************************************
 * Function System()
 *
 * Create a new process via QProcess and wait until finished.
 * return the process exit code.
 *
 ******************************************************************/

int System(const QStringList & args)
{
	QProcess *proc = new QProcess(NULL);
	proc->setArguments(args);
	if ( !proc->start() )
	{
		delete proc;
		return 1;
	}
	/* start was OK */
	/* wait a little bit */
	while( proc->isRunning() )
#ifndef _WIN32
		usleep(5000);
#else
		Sleep(5);
#endif

	int ex = proc->exitStatus();
	delete proc;
	return ex;
}

/**
 * @brief Return common gs args used across Scribus
 */

/******************************************************************
 * Function callGS()
 *   build the complete list of arguments for the call of our
 *   System() function.
 *
 *   The gs commands are all similar and consist of a few constant
 *   arguments, the variablke arguments and the end arguments which
 *   are also invariant. It will always use -q -dNOPAUSE and
 *   will always end with -c showpage -c quit. It also does automatic
 *   device selection unless overridden, and uses the user's antialiasing
 *   preferences and font search path.
 ******************************************************************/

int callGS(const QStringList& args_in, const QString device)
{
	return callGS(args_in.join(" "), device);
}

int callGS(const QString& args_in, const QString device)
{
	PrefsManager* prefsManager=PrefsManager::instance();
	QString cmd1 = getShortPathName(prefsManager->ghostscriptExecutable());
	cmd1 += " -q -dNOPAUSE";
	// Choose rendering device
	if (!device.isEmpty())
		// user specified device
		cmd1 += " -sDEVICE="+device;
	else if (ScApp->HavePngAlpha != 0)
		cmd1 += " -sDEVICE=png16m";
	else
		cmd1 += " -sDEVICE=pngalpha";
	// and antialiasing
	if (prefsManager->appPrefs.gs_AntiAliasText)
		cmd1 += " -dTextAlphaBits=4";
	if (prefsManager->appPrefs.gs_AntiAliasGraphics)
		cmd1 += " -dGraphicsAlphaBits=4";

	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
#ifndef _WIN32
	if (extraFonts->getRowCount() >= 1)
		cmd1 += QString(" -sFONTPATH='%1'").arg(extraFonts->get(0,0));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd1 += QString(":'%1'").arg(extraFonts->get(i,0));
#else
	if (extraFonts->getRowCount() >= 1)
		cmd1 += QString(" -sFONTPATH=\"%1\"").arg(extraFonts->get(0,0));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd1 += QString(";\"%1\"").arg(extraFonts->get(i,0));
#endif

	// then add any user specified args and run gs
	cmd1 += " " + args_in + " -c showpage -c quit";
//	qDebug("Calling gs as: %s", cmd1.ascii());
	return system(cmd1.local8Bit());
}
 
int  convertPS2PS(QString in, QString out, const QString& opts, int level)
{
	PrefsManager* prefsManager=PrefsManager::instance();
	QString cmd1 = getShortPathName(prefsManager->ghostscriptExecutable());
	cmd1 += " -q -dNOPAUSE -dSAFER -dBATCH";
	cmd1 += " -sDEVICE=pswrite";
	if(level <= 3)
		cmd1 += QString(" -dLanguageLevel=%1").arg(level);
	cmd1 += " " + opts + " ";
	cmd1 += " -sOutputFile=\"" + QDir::convertSeparators(out) + "\"";
	cmd1 += " \"" + QDir::convertSeparators(in) + "\"";
	int ret = system(cmd1.local8Bit());
	return ret;
}

// Return the GhostScript version string, or QString::null if it couldn't be retrived.
QString getGSVersion()
{
	QString gsVer;
	QStringList args;
	QString gsExe = getShortPathName(PrefsManager::instance()->ghostscriptExecutable());
	args.append(gsExe.local8Bit());
	args.append(QString("--version").local8Bit());
	QProcess* proc = new QProcess(args);
	proc->setCommunication(QProcess::Stdout);
	proc->start();
	while(proc->isRunning())
	{
#ifndef _WIN32
		usleep(5000);
#else
		Sleep(5);
#endif
		qApp->processEvents();
	}
	if(!proc->exitStatus())
		gsVer = proc->readLineStdout();
	delete proc;
	return gsVer;
}

// Return the GhostScript major and minor version numbers.
bool getNumericGSVersion(int & major, int & minor)
{
	QString gs_ver_string(getGSVersion());
	// gs's version string is of the form MAJOR.MINOR, so look for the .
	// then convert to numbers. 7.07 will become (7,7) for example.
	bool success = false;
	major = gs_ver_string.section('.', 0, 0).toInt(&success);
	if (!success)
		return false;
	minor = gs_ver_string.section('.', 1, 1).toInt(&success);
	if (!success)
		return false;
	return true;
}

QString getGSDefaultExeName(void)
{
	QString gsName;
#if defined _WIN32
	// Try to locate GhostScript thanks to the registry
	DWORD size;
	HKEY hKey;
	LONG retValue;
	DWORD regType = REG_SZ;
	char regVersion[MAX_PATH];
	char regPath[MAX_PATH];
	char gsPath[MAX_PATH];

	// Set gsName to its default value
	gsName = "gswin32c.exe";

	// Search AFPL Ghostscript first as it has more chance to be up to date
	if( RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\AFPL Ghostscript", &hKey) == ERROR_SUCCESS )
		strcpy(regPath, "SOFTWARE\\AFPL Ghostscript");
	else if( RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\GPL Ghostscript", &hKey) == ERROR_SUCCESS )
		strcpy(regPath, "SOFTWARE\\GPL Ghostscript");
	else
		return gsName;

	// Search the first SubKey corresponding to the version key
	size = sizeof(regVersion) - 1;
	retValue = RegEnumKeyEx(hKey, 0, regVersion, &size, NULL, NULL, NULL, NULL);
	RegCloseKey(hKey);
	if( retValue != ERROR_SUCCESS )
		return gsName;

	strcat(regPath, "\\");
	strcat(regPath, regVersion);

	// Get the GS_DLL Value
	if (RegOpenKey(HKEY_LOCAL_MACHINE, regPath, &hKey) != ERROR_SUCCESS)
          return gsName;
	size = sizeof(gsPath) - 1;
	retValue = RegQueryValueEx(hKey, "GS_DLL", 0, &regType, (LPBYTE) gsPath, &size);
	RegCloseKey(hKey);
	if( retValue != ERROR_SUCCESS )
		return gsName;

	// We now have GhostScript dll path, but we want gswin32c.exe
	// Normally gswin32c.exe and gsdll.dll are in the same directory
	gsName = gsPath;
	size = gsName.findRev("\\");
	if(size <= 0)
		return QString("gswin32c.exe");
	gsName = gsName.left(size + 1);
	gsName += "gswin32c.exe";

	// Check GhostScript executable existence.
	QFileInfo fInfo(gsName);
	if( fInfo.exists() )
		gsName.replace("\\", "/"); // Return a qt-styled path
	else
		gsName = "gswin32c.exe";

#else
	gsName = "gs";
#endif
	return gsName;
}

// On Windows, return short path name, else return longPath;
QString getShortPathName(QString longPath)
{
	QString shortPath = longPath;
#if defined _WIN32
	QFileInfo fInfo(longPath);
	if(fInfo.exists())
	{
		char shortName[MAX_PATH + 1];
		// An error should not be blocking as ERROR_INVALID_PARAMETER can simply mean
		// that volume does not support 8.3 filenames, so return longPath in this case
		int ret = GetShortPathName(QDir::convertSeparators(longPath).local8Bit(), shortName, sizeof(shortName));
		if( ret != ERROR_INVALID_PARAMETER && ret < sizeof(shortName))
			shortPath = shortName;
	}
#endif
	return shortPath;
}

int copyFile(QString source, QString target)
{
	if ((source.isNull()) || (target.isNull()))
		return -1;
	if (source == target)
		return -1;
	QFile s(source);
	QFile t(target);
	if (!s.exists())
		return -1;
	QByteArray bb(s.size());
	if (s.open(IO_ReadOnly))
	{
		s.readBlock(bb.data(), s.size());
		s.close();
		if (t.open(IO_WriteOnly))
		{
			t.writeBlock(bb.data(), bb.size());
			t.close();
		}
	}
	return 0;
}

int moveFile(QString source, QString target)
{
	if ((source.isNull()) || (target.isNull()))
		return -1;
	if (source == target)
		return -1;
	copyFile(source, target);
	QFile::remove(source);
	return 0;
}

QPixmap LoadPDF(QString fn, int Page, int Size, int *w, int *h)
{
	QString tmp, cmd1, cmd2;
	QString pdfFile = QDir::convertSeparators(fn);
	QString tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/sc.png");
	QPixmap pm;
	int ret = -1;
	tmp.setNum(Page);
	QStringList args;
	args.append("-r72");
	args.append("-sOutputFile=\""+tmpFile+"\"");
	args.append("-dFirstPage="+tmp);
	args.append("-dLastPage="+tmp);
	args.append("\""+pdfFile+"\"");
	ret = callGS(args);
	if (ret == 0)
	{
		QImage image;
		image.load(tmpFile);
		unlink(tmpFile);
		QImage im2;
		*h = image.height();
		*w = image.width();
		double sx = image.width() / static_cast<double>(Size);
		double sy = image.height() / static_cast<double>(Size);
		double t = (sy < sx ? sx : sy);
		im2 = image.smoothScale(static_cast<int>(image.width() / t), static_cast<int>(image.height() / t));
		pm.convertFromImage(im2);
		QPainter p;
		p.begin(&pm);
		p.setBrush(Qt::NoBrush);
		p.setPen(Qt::black);
		p.drawRect(0, 0, pm.width(), pm.height());
		p.end();
		im2.detach();
	}
	return pm;
}

QString GetAttr(QDomElement *el, QString at, QString def)
{
	return el->attribute(at, def);
}

int QStoInt(QString in)
{
	/* Dont need this, toInt returns 0 on failure, dont even need this function really.
	bool ok = false;
	int c = in.toInt(&ok);
	return ok ? c : 0;
	*/
	return in.toInt();
}

double QStodouble(QString in)
{
	bool ok = false;
	double c = in.toDouble(&ok);
	return ok ? c : 0.0;
}

QPixmap loadIcon(QString nam)
{
	QString iconFilePath = QString("%1/%2").arg(ScPaths::instance().iconDir()).arg(nam);
	QPixmap pm;
	if (!QFile::exists(iconFilePath))
		qWarning("Unable to load icon %s: File not found", iconFilePath.ascii());
	else
	{
		pm.load(iconFilePath);
		if (pm.isNull())
			qWarning("Unable to load icon %s: Got null pixmap", iconFilePath.ascii());
	}
	return pm;
}

uint getDouble(QString in, bool raw)
{
	QByteArray bb(4);
	if (raw)
	{
		bb[3] = static_cast<uchar>(QChar(in.at(0)));
		bb[2] = static_cast<uchar>(QChar(in.at(1)));
		bb[1] = static_cast<uchar>(QChar(in.at(2)));
		bb[0] = static_cast<uchar>(QChar(in.at(3)));
	}
	else
	{
		bb[0] = static_cast<uchar>(QChar(in.at(0)));
		bb[1] = static_cast<uchar>(QChar(in.at(1)));
		bb[2] = static_cast<uchar>(QChar(in.at(2)));
		bb[3] = static_cast<uchar>(QChar(in.at(3)));
	}
	uint ret;
	ret = bb[0] & 0xff;
	ret |= (bb[1] << 8) & 0xff00;
	ret |= (bb[2] << 16) & 0xff0000;
	ret |= (bb[3] << 24) & 0xff000000;
	return ret;
}

// Legacy implementation of LoadText with incorrect
// handling of unicode data. This should be retired.
// Use loadRawText instead.
// FIXME XXX
//
bool loadText(QString filename, QString *Buffer)
{
	QFile f(filename);
	QFileInfo fi(f);
	if (!fi.exists())
		return false;
	bool ret;
	QByteArray bb(f.size());
	if (f.open(IO_ReadOnly))
	{
		f.readBlock(bb.data(), f.size());
		f.close();
		for (uint posi = 0; posi < bb.size(); ++posi)
			*Buffer += QChar(bb[posi]);
		ret = true;
	}
	else
		ret = false;
	return ret;
}

// Replacement version of loadText that returns a QByteArray as an out
// parameter. The QByteArray, usually actually a QCString, is filled with the
// contents of the specified file. The return byte string is of unknown
// encoding; the caller must handle encoding issues. There is no need to
// preallocate the buffer, and the new data replaces any old contents.
bool loadRawText(QString filename, QByteArray & buf)
{
	bool ret = false;
	QFile f(filename);
	QFileInfo fi(f);
	if (fi.exists())
	{
		bool ret;
		QCString tempBuf(f.size());
		if (f.open(IO_ReadOnly))
		{
			Q_ULONG bytesRead = f.readBlock(tempBuf.data(), f.size());
			ret = bytesRead == f.size();
			if (ret)
				buf = tempBuf; // sharing makes this efficient
		}
	}
	if (f.isOpen())
		f.close();
	return ret;
}

QPointArray RegularPolygon(double w, double h, uint c, bool star, double factor, double rota)
{
	uint cx = star ? c * 2 : c;
	double seg = 360.0 / cx;
	double sc = rota + 180.0;
	double di = factor;
	int mx = 0;
	int my = 0;
	QPointArray pts = QPointArray();
	for (uint x = 0; x < cx; ++x)
	{
		sc = seg * x + 180.0 + rota;
		if (star)
		{
			double wf = x % 2 == 0 ? w / 2 : w / 2 * di;
			double hf = x % 2 == 0 ? h / 2 : h / 2 * di;
			mx = qRound(sin(sc / 180 * M_PI) * (wf) + (w/2));
			my = qRound(cos(sc / 180 * M_PI) * (hf) + (h/2));
		}
		else
		{
			mx = qRound(sin(sc / 180 * M_PI) * (w/2) + (w/2));
			my = qRound(cos(sc / 180 * M_PI) * (h/2) + (h/2));
		}
		pts.resize(x+1);
		pts.setPoint(x, mx, my);
	}
	return pts;
}

FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota)
{
	uint cx = star ? c * 2 : c;
	double seg = 360.0 / cx;
	double sc = rota + 180.0;
	double di = factor;
	double mx = 0;
	double my = 0;
	FPointArray pts;
	for (uint x = 0; x < cx; ++x)
	{
		sc = seg * x + 180.0 + rota;
		if (star)
		{
			double wf = x % 2 == 0 ? w / 2 : w / 2 * di;
			double hf = x % 2 == 0 ? h / 2 : h / 2 * di;
			mx = qRound(sin(sc / 180 * M_PI) * (wf) + (w/2));
			my = qRound(cos(sc / 180 * M_PI) * (hf) + (h/2));
		}
		else
		{
			mx = sin(sc / 180 * M_PI) * (w/2) + (w/2);
			my = cos(sc / 180 * M_PI) * (h/2) + (h/2);
		}
		pts.resize(x+1);
		pts.setPoint(x, mx, my);
	}
	return pts;
}

QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs)
{
	QPointArray Bez(4);
	QPointArray outa, cli;
	Segs.clear();
	if (ina.size() > 3)
	{
		for (uint poi=0; poi<ina.size()-3; poi += 4)
		{
			if (ina.point(poi).x() > 900000)
			{
				outa.resize(outa.size()+1);
				outa.setPoint(outa.size()-1, cli.point(cli.size()-1));
				Segs.append(outa.size());
				continue;
			}
			BezierPoints(&Bez, ina.pointQ(poi), ina.pointQ(poi+1), ina.pointQ(poi+3), ina.pointQ(poi+2));
			cli = Bez.cubicBezier();
			outa.putPoints(outa.size(), cli.size()-1, cli);
		}
		outa.resize(outa.size()+1);
		outa.setPoint(outa.size()-1, cli.point(cli.size()-1));
	}
	return outa;
}

double xy2Deg(double x, double y)
{
	return (atan2(y,x)*(180.0/M_PI));
}

void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4)
{
	ar->setPoint(0, n1);
	ar->setPoint(1, n2);
	ar->setPoint(2, n3);
	ar->setPoint(3, n4);
	return;
}

void Level2Layer(ScribusDoc *currentDoc, struct Layer *ll, int Level)
{
	uint layerCount=currentDoc->layerCount();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		if (currentDoc->Layers[la2].Level == Level)
		{
			ll->isViewable = currentDoc->Layers[la2].isViewable;
			ll->isPrintable = currentDoc->Layers[la2].isPrintable;
			ll->LNr = currentDoc->Layers[la2].LNr;
			ll->Name = currentDoc->Layers[la2].Name;
			break;
		}
	}
}

/* CB Replaced by ScribusDoc::layerLevelFromNumber
int Layer2Level(ScribusDoc *currentDoc, int LayerNr)
{
	int retVal=currentDoc->layerLevelFromNumber(LayerNr);
	int layerCount=currentDoc->layerCount();
	for (uint la2 = 0; la2 < layerCount; ++la2)
	{
		if (currentDoc->Layers[la2].LNr == LayerNr)
			return currentDoc->Layers[la2].Level;
	}
	return 0;
}
*/
QString CompressStr(QString *in)
{
	QString out = "";
#ifdef HAVE_LIBZ
	QByteArray bb(in->length());
	for (uint ax = 0; ax < in->length(); ++ax)
		bb[ax] = uchar(QChar(in->at(ax)));
	uLong exlen = uint(bb.size() * 0.001 + 16) + bb.size();
	QByteArray bc(exlen);
	int errcode = compress2((Byte *)bc.data(), &exlen, (Byte *)bb.data(), uLong(bb.size()), 9);
	if (errcode != Z_OK)
	{
		qDebug("compress2 failed with code %i", errcode);
		out = *in;
	}
	else {
		for (uint cl = 0; cl < exlen; ++cl)
			out += QChar(bc[cl]);
	}
#else
	out = *in;
#endif
	return out;
}

char *toHex( uchar u )
{
	static char hexVal[3];
	int i = 1;
	while ( i >= 0 )
	{
		ushort hex = (u & 0x000f);
		if ( hex < 0x0a )
			hexVal[i] = '0'+hex;
		else
			hexVal[i] = 'A'+(hex-0x0a);
		u = u >> 4;
		i--;
	}
	hexVal[2] = '\0';
	return hexVal;
}

QString String2Hex(QString *in, bool lang)
{
	int i = 0;
	QString out = "";
	for( uint xi = 0; xi < in->length(); ++xi )
	{
		out += toHex(uchar(QChar(in->at(xi))));
		++i;
		if ((i>40) && (lang))
		{
			out += '\n';
			i=0;
		}
	}
	return out;
}

QByteArray ComputeMD5Sum(QByteArray *in)
{
	QByteArray MDsum(16);
	md5_buffer (in->data(), in->size(), reinterpret_cast<void*>(MDsum.data()));
	return MDsum;
}

QString Path2Relative(QString Path)
{
	QString	Ndir = "";
	QStringList Pdir;
	QFileInfo Bfi = QFileInfo(Path);
	QStringList Bdir;
	bool end = true;
	uint dcoun = 0;
	uint dcoun2 = 0;

#ifndef _WIN32
	Pdir = QStringList::split("/", QDir::currentDirPath());
	Bdir = QStringList::split("/", Bfi.dirPath(true));
#else
	// On win32, file systems are case insensitive
	Pdir = QStringList::split("/", QDir::currentDirPath().lower());
	Bdir = QStringList::split("/", Bfi.dirPath(true).lower());
	// We must check that both path are located on same drive
	if( Pdir.size() > 0 && Bdir.size() > 0 )
	{
		QString drive = Bdir.front();
		QString currentDrive = Pdir.front();
		if( drive != currentDrive ) 
			return Path;
	}
#endif

	while (end)
	{
		if (Pdir[dcoun] == Bdir[dcoun])
			dcoun++;
		else
			break;
		if (dcoun > Pdir.count())
			break;
	}
	dcoun2 = dcoun;

#ifdef _WIN32
	Bdir = QStringList::split("/", Bfi.dirPath(true));
#endif

	for (uint ddx2 = dcoun; ddx2 < Pdir.count(); ddx2++)
		Ndir += "../";
	for (uint ddx = dcoun2; ddx < Bdir.count(); ddx++)
		Ndir += Bdir[ddx]+"/";
	Ndir += Bfi.fileName();
	return Ndir;
}

/***************************************************************************
    begin                : Wed Oct 29 2003
    copyright            : (C) 2003 The Scribus Team
    email                : paul@all-the-johnsons.co.uk
 ***************************************************************************/
// check if the file exists, if it does, ask if they're sure
// return true if they're sure, else return false;

bool overwrite(QWidget *parent, QString filename)
{
	bool retval = true;
	QFileInfo fi(filename);
	if (fi.exists())
	{
		int t = QMessageBox::warning(parent, QObject::tr("File exists"),
		                             QObject::tr("A file named '%1' already exists.\nDo you want to replace it with the file you are saving?").arg(filename),
		                             CommonStrings::tr_Cancel, QObject::tr("&Replace"), "", 1, 0);
		if (t == 0)
			retval = false;
	}
	return retval;
}

void WordAndPara(PageItem* currItem, int *w, int *p, int *c, int *wN, int *pN, int *cN)
{
	QChar Dat = QChar(32);
	int para = 0;
	int ww = 0;
	int cc = 0;
	int paraN = 0;
	int wwN = 0;
	int ccN = 0;
	bool first = true;
	PageItem *nextItem = currItem;
	PageItem *nbl = currItem;
	while (nextItem != 0)
	{
		if (nextItem->BackBox != 0)
			nextItem = nextItem->BackBox;
		else
			break;
	}
	while (nextItem != 0)
	{
		for (uint a = 0; a < nextItem->itemText.count(); ++a)
		{
			QChar b = nextItem->itemText.at(a)->ch[0];
			if (b == QChar(13))
			{
				if (a >= nextItem->MaxChars)
					paraN++;
				else
					para++;
			}
			if ((!b.isLetterOrNumber()) && (Dat.isLetterOrNumber()) && (!first))
			{
				if (a >= nextItem->MaxChars)
					wwN++;
				else
					ww++;
			}
			if (a >= nextItem->MaxChars)
				ccN++;
			else
				cc++;
			Dat = b;
			first = false;
		}
		nbl = nextItem;
		nextItem = nextItem->NextBox;
	}
	if (nbl->MaxChars < nbl->itemText.count())
		paraN++;
	else
		para++;
	if (Dat.isLetterOrNumber())
	{
		if (nbl->MaxChars < nbl->itemText.count())
			wwN++;
		else
			ww++;
	}
	*w = ww;
	*p = para;
	*c = cc;
	*wN = wwN;
	*pN = paraN;
	*cN = ccN;
}

void ReOrderText(ScribusDoc *currentDoc, ScribusView *view)
{
	double savScale = view->getScale();
	view->setScale(1.0);
	currentDoc->RePos = true;
	QPixmap pgPix(10, 10);
	QRect rd = QRect(0,0,9,9);
	ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
	for (uint azz=0; azz<currentDoc->MasterItems.count(); ++azz)
	{
		PageItem *currItem = currentDoc->MasterItems.at(azz);
		if (currItem->itemType() == PageItem::PathText)
			currItem->DrawObj(painter, rd);
	}
	for (uint azz=0; azz<currentDoc->Items.count(); ++azz)
	{
		PageItem *currItem = currentDoc->Items.at(azz);
		if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
			currItem->DrawObj(painter, rd);
	}
	currentDoc->RePos = false;
	view->setScale(savScale);
	delete painter;
}

/*! 10/06/2004 - pv
\param QString s1 first string
\param QString s2 second string
\retval bool t/f related s1>s2
 */
bool compareQStrings(QString s1, QString s2)
{
	if (QString::localeAwareCompare(s1, s2) >= 0)
		return false;
	return true;
}

/*! 10/06/2004 - pv
Returns a sorted list of QStrings - sorted by locale specific
rules! Uses compareQStrings() as rule. There is STL used!
TODO: Maybe we can implement one cass for various sorting...
\param QStringList aList unsorted string list
\retval QStringList sorted string list
*/
QStringList sortQStringList(QStringList aList)
{
	std::vector<QString> sortList;
	QStringList retList;
	QStringList::Iterator it;
	for (it = aList.begin(); it != aList.end(); ++it)
		sortList.push_back(*it);
	std::sort(sortList.begin(), sortList.end(), compareQStrings);
	for(uint i = 0; i < sortList.size(); i++)
		retList.append(sortList[i]);
	return retList;
}

void GetItemProps(bool newVersion, QDomElement *obj, struct CopyPasteBuffer *OB)
{
	QString tmp;
	int x, y;
	double xf, yf, xf2;
	OB->PType = static_cast<PageItem::ItemType>(QStoInt(obj->attribute("PTYPE")));
	OB->Width=QStodouble(obj->attribute("WIDTH"));
	OB->Height=QStodouble(obj->attribute("HEIGHT"));
	OB->RadRect = QStodouble(obj->attribute("RADRECT","0"));
	OB->ClipEdited = QStoInt(obj->attribute("CLIPEDIT", "0"));
	OB->FrameType = QStoInt(obj->attribute("FRTYPE", "0"));
	OB->Pwidth=QStodouble(obj->attribute("PWIDTH"));
	OB->Pcolor = obj->attribute("PCOLOR");
	if ((!newVersion) && (OB->PType == 4))
	{
		OB->TxtFill = obj->attribute("PCOLOR2");
		OB->Pcolor2 = "None";
	}
	else
	{
		OB->Pcolor2 = obj->attribute("PCOLOR2");
		OB->TxtFill = obj->attribute("TXTFILL", "Black");
	}
	OB->Shade = QStoInt(obj->attribute("SHADE"));
	OB->Shade2 = QStoInt(obj->attribute("SHADE2"));
	OB->TxtStroke=obj->attribute("TXTSTROKE", "None");
	OB->ShTxtFill=QStoInt(obj->attribute("TXTFILLSH", "100"));
	OB->ShTxtStroke=QStoInt(obj->attribute("TXTSTRSH", "100"));
	OB->TxtScale=qRound(QStodouble(obj->attribute("TXTSCALE", "100")) * 10);
	OB->TxtScaleV=qRound(QStodouble(obj->attribute("TXTSCALEV", "100")) * 10);
	OB->TxTBase=qRound(QStodouble(obj->attribute("TXTBASE", "0")) * 10);
	OB->TxTStyle=QStoInt(obj->attribute("TXTSTYLE", "0"));
	OB->TxtShadowX=qRound(QStodouble(obj->attribute("TXTSHX", "5")) * 10);
	OB->TxtShadowY=qRound(QStodouble(obj->attribute("TXTSHY", "-5")) * 10);
	OB->TxtOutline=qRound(QStodouble(obj->attribute("TXTOUT", "1")) * 10);
	OB->TxtUnderPos=qRound(QStodouble(obj->attribute("TXTULP", "-0.1")) * 10);
	OB->TxtUnderWidth=qRound(QStodouble(obj->attribute("TXTULW", "-0.1")) * 10);
	OB->TxtStrikePos=qRound(QStodouble(obj->attribute("TXTSTP", "-0.1")) * 10);
	OB->TxtStrikeWidth=qRound(QStodouble(obj->attribute("TXTSTW", "-0.1")) * 10);
	OB->Cols = QStoInt(obj->attribute("COLUMNS","1"));
	OB->ColGap = QStodouble(obj->attribute("COLGAP","0.0"));
	OB->GrType = QStoInt(obj->attribute("GRTYP","0"));
	OB->fill_gradient.clearStops();
	if (OB->GrType != 0)
	{
		OB->GrStartX = QStodouble(obj->attribute("GRSTARTX","0.0"));
		OB->GrStartY = QStodouble(obj->attribute("GRSTARTY","0.0"));
		OB->GrEndX = QStodouble(obj->attribute("GRENDX","0.0"));
		OB->GrEndY = QStodouble(obj->attribute("GRENDY","0.0"));
		OB->GrColor = obj->attribute("GRCOLOR","");
		if (OB->GrColor.isEmpty())
			OB->GrColor = "Black";
		OB->GrColor2 = obj->attribute("GRCOLOR2","Black");
		if (OB->GrColor2.isEmpty())
			OB->GrColor2 = "Black";
		OB->GrShade = QStoInt(obj->attribute("GRSHADE","100"));
		OB->GrShade2 = QStoInt(obj->attribute("GRSHADE2","100"));
	}
	OB->Rot=QStodouble(obj->attribute("ROT"));
	OB->PLineArt=Qt::PenStyle(QStoInt(obj->attribute("PLINEART")));
	OB->PLineEnd=Qt::PenCapStyle(QStoInt(obj->attribute("PLINEEND","0")));
	OB->PLineJoin=Qt::PenJoinStyle(QStoInt(obj->attribute("PLINEJOIN","0")));
	OB->LineSp=QStodouble(obj->attribute("LINESP"));
	OB->LineSpMode = QStoInt(obj->attribute("LINESPMode","0"));
	OB->LocalScX=QStodouble(obj->attribute("LOCALSCX"));
	OB->LocalScY=QStodouble(obj->attribute("LOCALSCY"));
	OB->LocalX=QStodouble(obj->attribute("LOCALX"));
	OB->LocalY=QStodouble(obj->attribute("LOCALY"));
	OB->PicArt=QStoInt(obj->attribute("PICART"));
	OB->flippedH = QStoInt(obj->attribute("FLIPPEDH")) % 2;
	OB->flippedV = QStoInt(obj->attribute("FLIPPEDV")) % 2;
	OB->BBoxX=QStodouble(obj->attribute("BBOXX"));
	OB->BBoxH=QStodouble(obj->attribute("BBOXH"));
	OB->ScaleType = QStoInt(obj->attribute("SCALETYPE","1"));
	OB->AspectRatio = QStoInt(obj->attribute("RATIO","0"));
	OB->isPrintable=QStoInt(obj->attribute("PRINTABLE"));
	OB->isAnnotation=QStoInt(obj->attribute("ANNOTATION","0"));
	OB->AnType = QStoInt(obj->attribute("ANTYPE","0"));
	OB->AnAction = obj->attribute("ANACTION","");
	OB->An_E_act = obj->attribute("ANEACT","");
	OB->An_X_act = obj->attribute("ANXACT","");
	OB->An_D_act = obj->attribute("ANDACT","");
	OB->An_Fo_act = obj->attribute("ANFOACT","");
	OB->An_Bl_act = obj->attribute("ANBLACT","");
	OB->An_K_act = obj->attribute("ANKACT","");
	OB->An_F_act = obj->attribute("ANFACT","");
	OB->An_V_act = obj->attribute("ANVACT","");
	OB->An_C_act = obj->attribute("ANCACT","");
	OB->AnActType = QStoInt(obj->attribute("ANACTYP","0"));
	OB->An_Extern = obj->attribute("ANEXTERN","");
	if ((!OB->An_Extern.isEmpty()) && (OB->AnActType != 8))
	{
		QFileInfo efp(OB->An_Extern);
		OB->An_Extern = efp.absFilePath();
	}
	OB->AnZiel = QStoInt(obj->attribute("ANZIEL","0"));
	OB->AnName = obj->attribute("ANNAME","");
	OB->AnToolTip = obj->attribute("ANTOOLTIP","");
	OB->AnRollOver = obj->attribute("ANROLL","");
	OB->AnDown = obj->attribute("ANDOWN","");
	OB->AnBwid = QStoInt(obj->attribute("ANBWID","1"));
	OB->AnBsty = QStoInt(obj->attribute("ANBSTY","0"));
	OB->AnFeed = QStoInt(obj->attribute("ANFEED","1"));
	OB->AnFlag = QStoInt(obj->attribute("ANFLAG","0"));
	OB->AnFont = QStoInt(obj->attribute("ANFONT","4"));
	OB->AnFormat = QStoInt(obj->attribute("ANFORMAT","0"));
	OB->AnVis = QStoInt(obj->attribute("ANVIS","0"));
	OB->AnIsChk = static_cast<bool>(QStoInt(obj->attribute("ANCHK","0")));
	OB->AnAAact = static_cast<bool>(QStoInt(obj->attribute("ANAA","0")));
	OB->AnHTML = static_cast<bool>(QStoInt(obj->attribute("ANHTML","0")));
	OB->AnUseIcons = static_cast<bool>(QStoInt(obj->attribute("ANICON","0")));
	OB->AnChkStil = QStoInt(obj->attribute("ANCHKS","0"));
	OB->AnMaxChar = QStoInt(obj->attribute("ANMC","-1"));
	OB->AnBColor = obj->attribute("ANBCOL","None");
	OB->AnIPlace = QStoInt(obj->attribute("ANPLACE","1"));
	OB->AnScaleW = QStoInt(obj->attribute("ANSCALE","0"));
	if (QStoInt(obj->attribute("TRANSPARENT","0")) == 1)
		OB->Pcolor = "None";
	OB->Textflow=QStoInt(obj->attribute("TEXTFLOW"));
	OB->Textflow2 =QStoInt(obj->attribute("TEXTFLOW2","0"));
	OB->UseContour = QStoInt(obj->attribute("TEXTFLOW3","0"));
	OB->Extra=QStodouble(obj->attribute("EXTRA"));
	OB->TExtra=QStodouble(obj->attribute("TEXTRA", "1"));
	OB->BExtra=QStodouble(obj->attribute("BEXTRA", "1"));
	OB->RExtra=QStodouble(obj->attribute("REXTRA", "1"));
	OB->PoShow = QStoInt(obj->attribute("PLTSHOW","0"));
	OB->BaseOffs = QStodouble(obj->attribute("BASEOF","0"));
	OB->ISize = qRound(QStodouble(obj->attribute("ISIZE","12")) * 10);
	if (obj->hasAttribute("EXTRAV"))
		OB->ExtraV = qRound(QStodouble(obj->attribute("EXTRAV","0")) / QStodouble(obj->attribute("ISIZE","12")) * 1000.0);
	else
		OB->ExtraV = QStoInt(obj->attribute("TXTKERN"));
	OB->Pfile=obj->attribute("PFILE");
	OB->Pfile2=obj->attribute("PFILE2","");
	OB->Pfile3=obj->attribute("PFILE3","");
	OB->IProfile=obj->attribute("PRFILE","");
	OB->EmProfile=obj->attribute("EPROF","");
	OB->IRender = QStoInt(obj->attribute("IRENDER","1"));
	OB->UseEmbedded = QStoInt(obj->attribute("EMBEDDED","1"));
	OB->Locked = static_cast<bool>(QStoInt(obj->attribute("LOCK","0")));
	OB->LockRes = static_cast<bool>(QStoInt(obj->attribute("LOCKR","0")));
	OB->Reverse = static_cast<bool>(QStoInt(obj->attribute("REVERS","0")));
	OB->isTableItem = static_cast<bool>(QStoInt(obj->attribute("isTableItem","0")));
	OB->TopLine = static_cast<bool>(QStoInt(obj->attribute("TopLine","0")));
	OB->LeftLine = static_cast<bool>(QStoInt(obj->attribute("LeftLine","0")));
	OB->RightLine = static_cast<bool>(QStoInt(obj->attribute("RightLine","0")));
	OB->BottomLine = static_cast<bool>(QStoInt(obj->attribute("BottomLine","0")));
	OB->TopLinkID =  QStoInt(obj->attribute("TopLINK","-1"));
	OB->LeftLinkID =  QStoInt(obj->attribute("LeftLINK","-1"));
	OB->RightLinkID =  QStoInt(obj->attribute("RightLINK","-1"));
	OB->BottomLinkID =  QStoInt(obj->attribute("BottomLINK","-1"));
	OB->Transparency = QStodouble(obj->attribute("TransValue","0.0"));
	if (obj->hasAttribute("TransValueS"))
		OB->TranspStroke = QStodouble(obj->attribute("TransValueS","0.0"));
	else
		OB->TranspStroke = OB->Transparency;
	tmp = "";
	if (obj->hasAttribute("NUMCLIP"))
	{
		OB->Clip.resize(obj->attribute("NUMCLIP").toUInt());
		tmp = obj->attribute("CLIPCOOR");
		QTextStream fc(&tmp, IO_ReadOnly);
		for (uint c=0; c<obj->attribute("NUMCLIP").toUInt(); ++c)
		{
			fc >> x;
			fc >> y;
			OB->Clip.setPoint(c, x, y);
		}
	}
	else
		OB->Clip.resize(0);
	tmp = "";
	if (obj->hasAttribute("NUMPO"))
	{
		OB->PoLine.resize(obj->attribute("NUMPO").toUInt());
		tmp = obj->attribute("POCOOR");
		QTextStream fp(&tmp, IO_ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMPO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			OB->PoLine.setPoint(cx, xf, yf);
		}
	}
	else
		OB->PoLine.resize(0);
	tmp = "";
	if (obj->hasAttribute("NUMCO"))
	{
		OB->ContourLine.resize(obj->attribute("NUMCO").toUInt());
		tmp = obj->attribute("COCOOR");
		QTextStream fp(&tmp, IO_ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMCO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			OB->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else
		OB->ContourLine.resize(0);
	tmp = "";
	if ((obj->hasAttribute("NUMTAB")) && (QStoInt(obj->attribute("NUMTAB","0")) != 0))
	{
		struct PageItem::TabRecord tb;
		tmp = obj->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		OB->TabValues.clear();
		for (int cxv = 0; cxv < QStoInt(obj->attribute("NUMTAB","0")); cxv += 2)
		{
			tgv >> xf;
			tgv >> xf2;
			tb.tabPosition = xf2;
			tb.tabType = static_cast<int>(xf);
			tb.tabFillChar = QChar();
			OB->TabValues.append(tb);
		}
		tmp = "";
	}
	else
		OB->TabValues.clear();
	if ((obj->hasAttribute("NUMDASH")) && (QStoInt(obj->attribute("NUMDASH","0")) != 0))
	{
		tmp = obj->attribute("DASHS");
		QTextStream dgv(&tmp, IO_ReadOnly);
		OB->DashValues.clear();
		for (int cxv = 0; cxv < QStoInt(obj->attribute("NUMDASH","0")); ++cxv)
		{
			dgv >> xf;
			OB->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		OB->DashValues.clear();
	OB->DashOffset = QStodouble(obj->attribute("DASHOFF","0.0"));
}

QColor SetColor(ScribusDoc *currentDoc, QString color, int shad)
{
	return currentDoc->PageColors[color].getShadeColorProof(shad);
}


/**
 * QPixmaps are really slow with Qt/Mac 3.3.4. Really, *really*, slow.
 * So we better cache them.
 */
QPixmap * getSmallPixmap(QColor rgb)
{
	static QMap<QRgb, QPixmap*> pxCache;

	QPixmap * pm = pxCache[rgb.rgb()];
	if (!pm)
	{
		pm = new QPixmap(15, 15);
		pm->fill(rgb);
		QPainter p;
		p.begin(pm);
		p.setBrush(Qt::NoBrush);
		QPen b(Qt::black, 1);
		p.setPen(b);
		p.drawRect(0, 0, 15, 15);
		p.end();
		pxCache[rgb.rgb()] = pm;
	}
	return pm;
}

QPixmap * getWidePixmap(QColor rgb)
{
	static QMap<QRgb, QPixmap*> pxCache;

	QPixmap * pm = pxCache[rgb.rgb()];
	if (!pm) {
		pm = new QPixmap(30, 15);
		pm->fill(rgb);
		pxCache[rgb.rgb()] = pm;
	}
	return pm;
}

void paintAlert(QPixmap &toPaint, QPixmap &target, int x, int y)
{
	QPainter p;
	p.begin(&target);
	p.drawPixmap(x, y, toPaint);
	p.end();
}

FPoint getMaxClipF(FPointArray* Clip)
{
	FPoint np, rp;
	double mx = 0;
	double my = 0;
	for (uint c = 0; c < Clip->size(); ++c)
	{
		np = Clip->point(c);
		if (np.x() > 900000)
			continue;
		if (np.x() > mx)
			mx = np.x();
		if (np.y() > my)
			my = np.y();
	}
	rp.setXY(mx, my);
	return rp;
}

FPoint getMinClipF(FPointArray* Clip)
{
	FPoint np, rp;
	double mx = 99999;
	double my = 99999;
	for (uint c = 0; c < Clip->size(); ++c)
	{
		np = Clip->point(c);
		if (np.x() > 900000)
			continue;
		if (np.x() < mx)
			mx = np.x();
		if (np.y() < my)
			my = np.y();
	}
	rp.setXY(mx, my);
	return rp;
}

/*!
 \fn QString checkFileExtension(const QString &currName, const QString &extension)
 \author Craig Bradney
 \brief A quick function to make sure a filename has the correct extension and add it if not
 \param currName Current filename
 \param extension File extension to ensure exists
 */
QString checkFileExtension(const QString &currName, const QString &extension)
{
	QString newName(currName);
	//If filename ends with a period, just add the extension
	if (newName.right(1)==".")
	{
		newName+=extension.lower();
		return newName;
	}
	//If filename doesnt end with the period+extension, add it on
	QString dotExt("." + extension.lower());
	if (!newName.endsWith(dotExt,false))
		newName+=dotExt;
	return newName;
}

/*! Creates a common name for page exports (SVG, bitmap, EPS).
Output format is: documentname-page01.extension
\param pageNo number of the exported page (begins from 1)
\param extension "svg" or e.g. "png" etc.
\retval QString standardized filename
\author Petr Vanek
*/
QString getFileNameByPage(uint pageNo, QString extension)
{
	QString number;
	number = number.setNum(pageNo + ScApp->doc->FirstPnum);
	QString defaultName = ScApp->doc->DocName;
	if (defaultName.isNull())
		defaultName = "export";
	else
	{
		QFileInfo fi(defaultName);
		defaultName = fi.baseName(true);
	}
	return QString("%1-%2%3.%4").arg(defaultName).arg(QObject::tr("page", "page export")).arg(number).arg(extension);
}
