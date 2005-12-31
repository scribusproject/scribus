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
#include <valarray>
#include <windows.h>
#endif

#include "md5.h"

#include <setjmp.h>
#include "commonstrings.h"
#include "pagestructs.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "prefsmanager.h"
#include "qprocess.h"
#include "scmessagebox.h"
#include "scpaths.h"

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

/**
 * @brief Synchronously execute a new process, optionally saving its output
 *
 * Create a new process via QProcess and wait until finished.  Return the
 * process exit code. Exit code 1 is returned if the process could not be
 * started or terminated abnormally.
 *
 * Note that the argument list is handled exactly as documented by QProcess.
 * In particular, no shell metacharacter expansion is performed (so you can't
 * use $HOME for example, and no quoting is required or appropriate), and each
 * list entry is one argument.
 *
 * If output file paths are provided, any existing file will be truncated and
 * overwritten.
 *
 * @param args Arguments, as per QProcess documentation.
 * @param fileStdErr Path to save error output to, or "" to discard.
 * @param fileStdOut Path to save normal output to, or "" to discard.
 * @return Program exit code, or 1 on failure.
 *
 */
int System(const QStringList & args, const QString fileStdErr, const QString fileStdOut)
{
	QByteArray stdErrData;
	QByteArray stdOutData;
	QProcess proc(0);
	proc.setArguments(args);
	if ( !proc.start() )
	{
		// TODO: Should we really be returning 1 on failure? Many apps store
		// their error code in the upper bits, ie return 1 << 8 , to avoid
		// messing with the exit code of the process. We could also just have
		// an out parameter for our own exit code.
		return 1;
	}
	/* start was OK */
	/* wait a little bit */
	while( proc.isRunning() )
	{
#ifndef _WIN32
		usleep(5000);
#else
		Sleep(5);
#endif
	}
	// exitStatus() returns 0 on non-normal exit
	int ex = 1;
	if (proc.normalExit())
		ex = proc.exitStatus();

	qDebug("Result code is: %i", ex);

	if ( !fileStdErr.isEmpty() )
	{
		stdErrData = proc.readStderr();
		QFile ferr(fileStdErr);
		if ( ferr.open(IO_WriteOnly) )
		{
			ferr.writeBlock( stdErrData );
			ferr.close();
		}
	}

	if ( !fileStdOut.isEmpty() )
	{
		stdOutData = proc.readStdout();
		QFile fout(fileStdOut);
		if ( fout.open(IO_WriteOnly) )
		{
			fout.writeBlock( stdOutData );
			fout.close();
		}
	}

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
	QString cmd;
	QStringList args;
	PrefsManager* prefsManager = PrefsManager::instance();
	args.append( getShortPathName(prefsManager->ghostscriptExecutable()) );
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-dQUIET" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	// Choose rendering device
	if (!device.isEmpty())
		args.append( QString("-sDEVICE=%1").arg(device) ); // user specified device
	else if (ScMW->HavePngAlpha != 0)
		args.append( "-sDEVICE=png16m" );
	else
		args.append( "-sDEVICE=pngalpha" );
	// and antialiasing
	if (prefsManager->appPrefs.gs_AntiAliasText)
		args.append( "-dTextAlphaBits=4" );
	if (prefsManager->appPrefs.gs_AntiAliasGraphics)
		args.append( "-dGraphicsAlphaBits=4" );

	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
#ifndef _WIN32
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH='%1'").arg(extraFonts->get(0,0));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString(":'%1'").arg(extraFonts->get(i,0));
#else
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH=\"%1\"").arg(extraFonts->get(0,0));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString(";\"%1\"").arg(extraFonts->get(i,0));
#endif
	if( !cmd.isEmpty() )
		args.append( cmd );

	args += args_in;
	args.append("-c");
	args.append("showpage");
	return System( args );
}

int callGS(const QString& args_in, const QString device)
{
	PrefsManager* prefsManager=PrefsManager::instance();
	QString cmd1 = getShortPathName(prefsManager->ghostscriptExecutable());
	cmd1 += " -q -dNOPAUSE -dQUIET -dPARANOIDSAFER -dBATCH";
	// Choose rendering device
	if (!device.isEmpty())
		// user specified device
		cmd1 += " -sDEVICE="+device;
	else if (ScMW->HavePngAlpha != 0)
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
	cmd1 += " " + args_in + " -c showpage";
//	qDebug("Calling gs as: %s", cmd1.ascii());
	return system(cmd1.local8Bit());
}
 
int  convertPS2PS(QString in, QString out, const QStringList& opts, int level)
{
	PrefsManager* prefsManager=PrefsManager::instance();
	QStringList args;
	args.append( getShortPathName(prefsManager->ghostscriptExecutable()) );
	args.append( "-q" );
	args.append( "-dQUIET" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	args.append( "-sDEVICE=pswrite" );
	if(level <= 3)
		args.append( QString(" -dLanguageLevel=%1").arg(level) );
	args += opts;
	args.append( QString("-sOutputFile=\"%1\"").arg(QDir::convertSeparators(out)) );
	args.append( QString("\"%1\"").arg(QDir::convertSeparators(in)) );
	int ret = System( args );
	return ret;
}

int  testGSAvailability( void )
{
	QStringList args;
	PrefsManager* prefsManager = PrefsManager::instance();
	args.append( getShortPathName(prefsManager->ghostscriptExecutable()) );
	args.append( "-h" );
	args.append( ">" );
#ifndef _WIN32
	args.append( "/dev/null" );
#else
	args.append( "NUL" );
#endif
	args.append( "2>&1" );
	int ret = System( args );
	return ret;
}

int  testGSDeviceAvailability( QString device )
{
	QStringList args;
	PrefsManager* prefsManager = PrefsManager::instance();
	args.append( getShortPathName(prefsManager->ghostscriptExecutable()) );
	args.append( QString("-sDEVICE=%1").arg( device ) );
	args.append( "-c" );
	args.append( "quit" );
	args.append( ">" );
#ifndef _WIN32
	args.append( "/dev/null" );
#else
	args.append( "NUL" );
#endif
	args.append( "2>&1" );
	int ret = System( args );
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
	int bytesread;
	if ((source.isNull()) || (target.isNull()))
		return -1;
	if (source == target)
		return -1;
	QFile s(source);
	QFile t(target);
	if (!s.exists())
		return -1;
	QByteArray bb( 65536 );
	if (s.open(IO_ReadOnly))
	{
		if (t.open(IO_WriteOnly))
		{
			bytesread = s.readBlock( bb.data(), bb.size() );
			while( bytesread > 0 )
			{
				t.writeBlock( bb.data(), bytesread );
				bytesread = s.readBlock( bb.data(), bb.size() );
			}
			t.close();
		}
		s.close();
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
		/*
		int len = bb.size();
		int oldLen = Buffer->length();
		Buffer->setLength( oldLen + len + 1);
		// digged into Qt 3.3 sources to find that. Might break in Qt 4 -- AV
		unsigned short * ucsString = const_cast<unsigned short *>(Buffer->ucs2()) + oldLen;
		char * data = bb.data();
		for (uint posi = 0; posi < len; ++posi)
			*ucsString++ = *data++;
		*ucsString = 0;
		 */
		ret = true;
	}
	else
		ret = false;
	return ret;
}

// Replacement version of loadText that returns a QCString as an out parameter.
// The QCString is filled with the contents of the specified file. The return
// byte string is of unknown encoding; the caller must handle encoding issues.
// There is no need to preallocate the buffer, and the new data replaces any
// old contents.
bool loadRawText(const QString & filename, QCString & buf)
{
	bool ret = false;
	QFile f(filename);
	QFileInfo fi(f);
	if (fi.exists())
	{
		bool ret;
		QCString tempBuf(f.size() + 1);
		if (f.open(IO_ReadOnly))
		{
			unsigned int bytesRead = f.readBlock(tempBuf.data(), f.size());
			tempBuf[bytesRead] = '\0';
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
		int t = ScMessageBox::warning(parent, QObject::tr("File exists"), 
									"<qt>"+ QObject::tr("A file named '%1' already exists.<br/>Do you want to replace it with the file you are saving?").arg(filename) +"</qt>",
									QObject::tr("&Replace"), CommonStrings::tr_Cancel, "", 1, 1);
		if (t == 1)
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
	for (uint azz=0; azz<currentDoc->Items->count(); ++azz)
	{
		PageItem *currItem = currentDoc->Items->at(azz);
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
	OB->PType = static_cast<PageItem::ItemType>(obj->attribute("PTYPE").toInt());
	OB->Width=obj->attribute("WIDTH").toDouble();
	OB->Height=obj->attribute("HEIGHT").toDouble();
	OB->RadRect = obj->attribute("RADRECT", "0").toDouble();
	OB->ClipEdited = obj->attribute("CLIPEDIT", "0").toInt();
	OB->FrameType = obj->attribute("FRTYPE", "0").toInt();
	OB->Pwidth=obj->attribute("PWIDTH").toDouble();
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
	OB->Shade = obj->attribute("SHADE").toInt();
	OB->Shade2 = obj->attribute("SHADE2").toInt();
	OB->TxtStroke=obj->attribute("TXTSTROKE", "None");
	OB->ShTxtFill=obj->attribute("TXTFILLSH", "100").toInt();
	OB->ShTxtStroke=obj->attribute("TXTSTRSH", "100").toInt();
	OB->TxtScale=qRound(obj->attribute("TXTSCALE", "100").toDouble() * 10);
	OB->TxtScaleV=qRound(obj->attribute("TXTSCALEV", "100").toDouble() * 10);
	OB->TxTBase=qRound(obj->attribute("TXTBASE", "0").toDouble() * 10);
	OB->TxTStyle=obj->attribute("TXTSTYLE", "0").toInt();
	OB->TxtShadowX=qRound(obj->attribute("TXTSHX", "5").toDouble() * 10);
	OB->TxtShadowY=qRound(obj->attribute("TXTSHY", "-5").toDouble() * 10);
	OB->TxtOutline=qRound(obj->attribute("TXTOUT", "1").toDouble() * 10);
	OB->TxtUnderPos=qRound(obj->attribute("TXTULP", "-0.1").toDouble() * 10);
	OB->TxtUnderWidth=qRound(obj->attribute("TXTULW", "-0.1").toDouble() * 10);
	OB->TxtStrikePos=qRound(obj->attribute("TXTSTP", "-0.1").toDouble() * 10);
	OB->TxtStrikeWidth=qRound(obj->attribute("TXTSTW", "-0.1").toDouble() * 10);
	OB->Cols = obj->attribute("COLUMNS", "1").toInt();
	OB->ColGap = obj->attribute("COLGAP", "0.0").toDouble();
	OB->GrType = obj->attribute("GRTYP", "0").toInt();
	OB->fill_gradient.clearStops();
	if (OB->GrType != 0)
	{
		OB->GrStartX = obj->attribute("GRSTARTX", "0.0").toDouble();
		OB->GrStartY = obj->attribute("GRSTARTY", "0.0").toDouble();
		OB->GrEndX = obj->attribute("GRENDX", "0.0").toDouble();
		OB->GrEndY = obj->attribute("GRENDY", "0.0").toDouble();
		OB->GrColor = obj->attribute("GRCOLOR","");
		if (OB->GrColor.isEmpty())
			OB->GrColor = "Black";
		OB->GrColor2 = obj->attribute("GRCOLOR2","Black");
		if (OB->GrColor2.isEmpty())
			OB->GrColor2 = "Black";
		OB->GrShade = obj->attribute("GRSHADE", "100").toInt();
		OB->GrShade2 = obj->attribute("GRSHADE2", "100").toInt();
	}
	OB->Rot=obj->attribute("ROT").toDouble();
	OB->PLineArt=Qt::PenStyle(obj->attribute("PLINEART").toInt());
	OB->PLineEnd=Qt::PenCapStyle(obj->attribute("PLINEEND", "0").toInt());
	OB->PLineJoin=Qt::PenJoinStyle(obj->attribute("PLINEJOIN", "0").toInt());
	OB->LineSp=obj->attribute("LINESP").toDouble();
	OB->LineSpMode = obj->attribute("LINESPMode", "0").toInt();
	OB->LocalScX=obj->attribute("LOCALSCX").toDouble();
	OB->LocalScY=obj->attribute("LOCALSCY").toDouble();
	OB->LocalX=obj->attribute("LOCALX").toDouble();
	OB->LocalY=obj->attribute("LOCALY").toDouble();
	OB->PicArt=obj->attribute("PICART").toInt();
	OB->flippedH = obj->attribute("FLIPPEDH").toInt() % 2;
	OB->flippedV = obj->attribute("FLIPPEDV").toInt() % 2;
	OB->BBoxX=obj->attribute("BBOXX").toDouble();
	OB->BBoxH=obj->attribute("BBOXH").toDouble();
	OB->ScaleType = obj->attribute("SCALETYPE", "1").toInt();
	OB->AspectRatio = obj->attribute("RATIO", "0").toInt();
	OB->isPrintable=obj->attribute("PRINTABLE").toInt();
	OB->m_isAnnotation=obj->attribute("ANNOTATION", "0").toInt();
	OB->m_annotation.setType(obj->attribute("ANTYPE", "0").toInt());
	OB->m_annotation.setAction(obj->attribute("ANACTION",""));
	OB->m_annotation.setE_act(obj->attribute("ANEACT",""));
	OB->m_annotation.setX_act(obj->attribute("ANXACT",""));
	OB->m_annotation.setD_act(obj->attribute("ANDACT",""));
	OB->m_annotation.setFo_act(obj->attribute("ANFOACT",""));
	OB->m_annotation.setBl_act(obj->attribute("ANBLACT",""));
	OB->m_annotation.setK_act(obj->attribute("ANKACT",""));
	OB->m_annotation.setF_act(obj->attribute("ANFACT",""));
	OB->m_annotation.setV_act(obj->attribute("ANVACT",""));
	OB->m_annotation.setC_act(obj->attribute("ANCACT",""));
	OB->m_annotation.setActionType(obj->attribute("ANACTYP", "0").toInt());
	OB->m_annotation.setExtern(obj->attribute("ANEXTERN",""));
	if ((!OB->m_annotation.Extern().isEmpty()) && (OB->m_annotation.ActionType() != 8))
	{
		QFileInfo efp(OB->m_annotation.Extern());
		OB->m_annotation.setExtern(efp.absFilePath());
	}
	OB->m_annotation.setZiel(obj->attribute("ANZIEL", "0").toInt());
	OB->AnName=obj->attribute("ANNAME","");
	OB->m_annotation.setToolTip(obj->attribute("ANTOOLTIP",""));
	OB->m_annotation.setRollOver(obj->attribute("ANROLL",""));
	OB->m_annotation.setDown(obj->attribute("ANDOWN",""));
	OB->m_annotation.setBwid(obj->attribute("ANBWID", "1").toInt());
	OB->m_annotation.setBsty(obj->attribute("ANBSTY", "0").toInt());
	OB->m_annotation.setFeed(obj->attribute("ANFEED", "1").toInt());
	OB->m_annotation.setFlag(obj->attribute("ANFLAG", "0").toInt());
	OB->m_annotation.setFont(obj->attribute("ANFONT", "4").toInt());
	OB->m_annotation.setFormat(obj->attribute("ANFORMAT", "0").toInt());
	OB->m_annotation.setVis(obj->attribute("ANVIS", "0").toInt());
	OB->m_annotation.setIsChk(static_cast<bool>(obj->attribute("ANCHK", "0").toInt()));
	OB->m_annotation.setAAact(static_cast<bool>(obj->attribute("ANAA", "0").toInt()));
	OB->m_annotation.setHTML(static_cast<bool>(obj->attribute("ANHTML", "0").toInt()));
	OB->m_annotation.setUseIcons(static_cast<bool>(obj->attribute("ANICON", "0").toInt()));
	OB->m_annotation.setChkStil(obj->attribute("ANCHKS", "0").toInt());
	OB->m_annotation.setMaxChar(obj->attribute("ANMC", "-1").toInt());
	OB->m_annotation.setBorderColor(obj->attribute("ANBCOL","None"));
	OB->m_annotation.setIPlace(obj->attribute("ANPLACE", "1").toInt());
	OB->m_annotation.setScaleW(obj->attribute("ANSCALE", "0").toInt());
	if (obj->attribute("TRANSPARENT", "0").toInt() == 1)
		OB->Pcolor = "None";
	OB->Textflow=obj->attribute("TEXTFLOW").toInt();
	OB->Textflow2 =obj->attribute("TEXTFLOW2", "0").toInt();
	OB->UseContour = obj->attribute("TEXTFLOW3", "0").toInt();
	OB->Extra=obj->attribute("EXTRA").toDouble();
	OB->TExtra=obj->attribute("TEXTRA", "1").toDouble();
	OB->BExtra=obj->attribute("BEXTRA", "1").toDouble();
	OB->RExtra=obj->attribute("REXTRA", "1").toDouble();
	OB->PoShow = obj->attribute("PLTSHOW", "0").toInt();
	OB->BaseOffs = obj->attribute("BASEOF", "0").toDouble();
	OB->ISize = qRound(obj->attribute("ISIZE", "12").toDouble() * 10);
	if (obj->hasAttribute("EXTRAV"))
		OB->ExtraV = qRound(obj->attribute("EXTRAV", "0").toDouble() / obj->attribute("ISIZE", "12").toDouble() * 1000.0);
	else
		OB->ExtraV = obj->attribute("TXTKERN").toInt();
	OB->Pfile=obj->attribute("PFILE");
	OB->Pfile2=obj->attribute("PFILE2","");
	OB->Pfile3=obj->attribute("PFILE3","");
	OB->IProfile=obj->attribute("PRFILE","");
	OB->EmProfile=obj->attribute("EPROF","");
	OB->IRender = obj->attribute("IRENDER", "1").toInt();
	OB->UseEmbedded = obj->attribute("EMBEDDED", "1").toInt();
	OB->Locked = static_cast<bool>(obj->attribute("LOCK", "0").toInt());
	OB->LockRes = static_cast<bool>(obj->attribute("LOCKR", "0").toInt());
	OB->Reverse = static_cast<bool>(obj->attribute("REVERS", "0").toInt());
	OB->isTableItem = static_cast<bool>(obj->attribute("isTableItem", "0").toInt());
	OB->TopLine = static_cast<bool>(obj->attribute("TopLine", "0").toInt());
	OB->LeftLine = static_cast<bool>(obj->attribute("LeftLine", "0").toInt());
	OB->RightLine = static_cast<bool>(obj->attribute("RightLine", "0").toInt());
	OB->BottomLine = static_cast<bool>(obj->attribute("BottomLine", "0").toInt());
	OB->TopLinkID =  obj->attribute("TopLINK", "-1").toInt();
	OB->LeftLinkID =  obj->attribute("LeftLINK", "-1").toInt();
	OB->RightLinkID =  obj->attribute("RightLINK", "-1").toInt();
	OB->BottomLinkID =  obj->attribute("BottomLINK", "-1").toInt();
	OB->Transparency = obj->attribute("TransValue", "0.0").toDouble();
	if (obj->hasAttribute("TransValueS"))
		OB->TranspStroke = obj->attribute("TransValueS", "0.0").toDouble();
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
	if ((obj->hasAttribute("NUMTAB")) && (obj->attribute("NUMTAB", "0").toInt() != 0))
	{
		struct PageItem::TabRecord tb;
		tmp = obj->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		OB->TabValues.clear();
		for (int cxv = 0; cxv < obj->attribute("NUMTAB", "0").toInt(); cxv += 2)
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
	if ((obj->hasAttribute("NUMDASH")) && (obj->attribute("NUMDASH", "0").toInt() != 0))
	{
		tmp = obj->attribute("DASHS");
		QTextStream dgv(&tmp, IO_ReadOnly);
		OB->DashValues.clear();
		for (int cxv = 0; cxv < obj->attribute("NUMDASH", "0").toInt(); ++cxv)
		{
			dgv >> xf;
			OB->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		OB->DashValues.clear();
	OB->DashOffset = obj->attribute("DASHOFF", "0.0").toDouble();
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

	QRgb index=rgb.rgb();
	if (pxCache.contains(index))
		return pxCache[index];

	QPixmap *pm = new QPixmap(15, 15);
	pm->fill(rgb);
	QPainter p;
	p.begin(pm);
	p.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	p.setPen(b);
	p.drawRect(0, 0, 15, 15);
	p.end();
	pxCache.insert(index, pm);
	return pm;
}

QPixmap * getWidePixmap(QColor rgb)
{
	static QMap<QRgb, QPixmap*> pxCache;

	QRgb index=rgb.rgb();
	if (pxCache.contains(index))
		return pxCache[index];

	QPixmap *pm = new QPixmap(30, 15);
	pm->fill(rgb);
	pxCache.insert(index, pm);
	return pm;
}

static Q_UINT64 code64(ScColor & col) {
	int C, M, Y, K, R, G, B;
	Q_UINT64 result;
	col.getRGB( &R, &G, &B );
	col.getCMYK( &C, &M, &Y, &K );
	result |= col.getColorModel() == colorModelRGB ? 1 : 0;
	result |= col.isOutOfGamut() ? 64 : 0;
	result |= col.isSpotColor() ? 32 : 0;
	result |= col.isRegistrationColor() ? 16 : 0;
	result <<= 8;
	result |= C;
	result <<= 8;
	result |= M;
	result <<= 8;
	result |= Y;
	result <<= 8;
	result |= K;
	result <<= 8;
	result |= R;
	result <<= 8;
	result |= G;
	result <<= 8;
	result |= B;
	return result;
}

QPixmap * getFancyPixmap(ScColor col) {
	static QMap<Q_UINT64, QPixmap*> pxCache;
	
	static QPixmap alertIcon;
	static QPixmap cmykIcon;
	static QPixmap rgbIcon;
	static QPixmap spotIcon;
	static QPixmap regIcon;
	static bool iconsInitialized = false;
	
	if ( !iconsInitialized ) {
		alertIcon = loadIcon("alert.png");
		cmykIcon = loadIcon("cmyk.png");
		rgbIcon = loadIcon("rgb.png");
		spotIcon = loadIcon("spot.png");
		regIcon = loadIcon("register.png");
		iconsInitialized = true;
	}
	
	Q_UINT64 res=code64(col);
	if (pxCache.contains(res))
		return pxCache[res];
	
	QPixmap *pa=new QPixmap(60, 15);
	QPixmap *pm=getSmallPixmap(col.getRawRGBColor());
	pa->fill(Qt::white);
	paintAlert(*pm, *pa, 0, 0);
	col.checkGamut();
	if (col.isOutOfGamut())
		paintAlert(alertIcon, *pa, 15, 0);
	if ((col.getColorModel() == colorModelCMYK) || (col.isSpotColor()))
		paintAlert(cmykIcon, *pa, 30, 0);
	else
		paintAlert(rgbIcon, *pa, 30, 0);
	if (col.isSpotColor())
		paintAlert(spotIcon, *pa, 46, 2);
	if (col.isRegistrationColor())
		paintAlert(regIcon, *pa, 45, 0);
	pxCache.insert(res, pa);
	return pa;
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
	uint clipSize=Clip->size();
	for (uint c = 0; c < clipSize; ++c)
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
	uint clipSize=Clip->size();
	for (uint c = 0; c < clipSize; ++c)
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
	number = number.setNum(pageNo + ScMW->doc->FirstPnum);
	QString defaultName = ScMW->doc->DocName;
	if (defaultName.isNull())
		defaultName = "export";
	else
	{
		QFileInfo fi(defaultName);
		defaultName = fi.baseName(true);
	}
	return QString("%1-%2%3.%4").arg(defaultName).arg(QObject::tr("page", "page export")).arg(number).arg(extension);
}

inline double square(double x)
{
	return x*x;
}

inline double distance(double x, double y)
{
	return sqrt(x*x+y*y);
}

const QString getStringFromSequence(DocumentSectionType type, uint position)
{
	QString retVal("");
	switch( type )
	{
		case Type_1_2_3:
			retVal=QString::number(position);
			break;
		case Type_A_B_C:
			retVal=numberToLetterSequence(position).upper();
			break;
		case Type_a_b_c:
			retVal=numberToLetterSequence(position);
			break;
		case Type_I_II_III:
			retVal=arabicToRoman(position);
			break;
		case Type_i_ii_iii:
			//well, for lower case people will want that, even if its "wrong"
			//ie, X=10, x=10000
			retVal=arabicToRoman(position).lower();
			break;
		default:
			break;
	}
	return retVal;
}

const QString numberToLetterSequence(uint i)
{
	QString retVal("");
	unsigned digits = 1;
	unsigned offset = 0;
	uint column=i;
	--column;

	if( column > 4058115285U ) return  QString("@");

	for( unsigned limit = 26; column >= limit+offset; limit *= 26, digits++ )
		offset += limit;

	for( unsigned c = column - offset; digits; --digits, c/=26 )
		retVal.prepend( QChar( 'a' + (c%26) ) );
	return retVal;
}

const QString arabicToRoman(uint i)
{
	QString roman("");
	int arabic = i;
	while (arabic - 1000000 >= 0){
	roman += "m";
	arabic -= 1000000;
	}
	while (arabic - 900000 >= 0){
	roman += "cm";
	arabic -= 900000;
	}
	while (arabic - 500000 >= 0){
	roman += "d";
	arabic -= 500000;
	}
	while (arabic - 400000 >= 0){
	roman += "cd";
	arabic -= 400000;
	}
	while (arabic - 100000 >= 0){
	roman += "c";
	arabic -= 100000;
	}
	while (arabic - 90000 >= 0){
	roman += "xc";
	arabic -= 90000;
	}
	while (arabic - 50000 >= 0){
	roman += "l";
	arabic -= 50000;
	}
	while (arabic - 40000 >= 0){
	roman += "xl";
	arabic -= 40000;
	}
	while (arabic - 10000 >= 0){
	roman += "x";
	arabic -= 10000;
	}
	while (arabic - 9000 >= 0){
	roman += "Mx";
	arabic -= 9000;
	}
	while (arabic - 5000 >= 0){
	roman += "v";
	arabic -= 5000;
	}
	while (arabic - 4000 >= 0){
	roman += "Mv";
	arabic -= 4000;
	}
	while (arabic - 1000 >= 0){
	roman += "M";
	arabic -= 1000;
	}
	while (arabic - 900 >= 0){
	roman += "CM";
	arabic -= 900;
	}
	while (arabic - 500 >= 0){
	roman += "D";
	arabic -= 500;
	}
	while (arabic - 400 >= 0){
	roman += "CD";
	arabic -= 400;
	}
	while (arabic - 100 >= 0){
	roman += "C";
	arabic -= 100;
	}
	while (arabic - 90 >= 0){
	roman += "XC";
	arabic -= 90;
	}
	while (arabic - 50 >= 0){
	roman += "L";
	arabic -= 50;
	}
	while (arabic - 40 >= 0){
	roman += "XL";
	arabic -= 40;
	}
	while (arabic - 10 >= 0){
	roman += "X";
	arabic -= 10;
	}
	while (arabic - 9 >= 0){
	roman += "IX";
	arabic -= 9;
	}
	while (arabic - 5 >= 0){
	roman += "V";
	arabic -= 5;
	}
	while (arabic - 4 >= 0){
	roman += "IV";
	arabic -= 4;
	}
	while (arabic - 1 >= 0){
	roman += "I";
	arabic -= 1;
	}
	return roman;
}

//CB Moved from scribus.cpp
void parsePagesString(QString pages, std::vector<int>* pageNs, int sourcePageCount)
{
	QString tmp(pages);
	QString token;
	int from, to, pageNr;
	do
	{
		if (tmp.find(",") == -1)
		{
			token = tmp;
			tmp = "";
		}
		else
		{
			token = tmp.left(tmp.find(","));
			tmp = tmp.right(tmp.length() - tmp.find(",") - 1);
		}

		token = token.stripWhiteSpace();
		if (token == "*") // Import all source doc pages
		{
			for (int i = 1; i <= sourcePageCount; ++i)
				pageNs->push_back(i);
		}
		else if (token.find("-") != -1) // import a range of source doc pages
		{
			from = QString(token.left(token.find("-"))).toInt();
			to = QString(token.right(token.length() - token.find("-") - 1)).toInt();
			if ((from != 0) && (to != 0))
			{
				if (from > sourcePageCount)
					from = sourcePageCount;
				if (to > sourcePageCount)
					to = sourcePageCount;
				if (from == to)
					pageNs->push_back(to);
				else if (from < to)
				{
					for (int i = from; i <= to; ++i)
						pageNs->push_back(i);
				}
				else
				{
					for (int i = from; i >= to; --i)
						pageNs->push_back(i);
				}
			}
		}
		else // import single source doc page
		{
			pageNr = token.toInt();
			if ((pageNr > 0) && (pageNr <= sourcePageCount))
				pageNs->push_back(pageNr);
		}
	} while (!tmp.isEmpty());
}
