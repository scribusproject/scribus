/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          gsutil.cpp  -  description
                             -------------------
    begin                : Fri Sep 14 2001
    copyright            : (C) 2001 by Franz Schmid
    copyright            : (C) 2006- Scribus Team (code moved from util.cpp)
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

#include "gsutil.h"

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpainter.h>
#include <qprocess.h>

/* CB old includes from util.cpp.
#include <algorithm>
#include <cstdlib>
#include <cmath>
*/
#include "scconfig.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
/*
#include <setjmp.h>
#include "pagestructs.h"
*/
#include "prefsfile.h"
/*
#include "prefscontext.h"
#include "prefstable.h"
*/
#include "prefsmanager.h"
#include "scpaths.h"

/*
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
*/
#include "scribus.h"
/*
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
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
*/

#include "util.h"

using namespace std;


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
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH=%1").arg(extraFonts->get(0,0));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString("%1%2").arg(sep).arg(extraFonts->get(i,0));
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
		args.append( QString("-dLanguageLevel=%1").arg(level) );
	args += opts;
	args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(out)) );
	args.append( QDir::convertSeparators(in) );
	int ret = System( args );
	return ret;
}

int  testGSAvailability( void )
{
	QStringList args;
	PrefsManager* prefsManager = PrefsManager::instance();
	args.append( getShortPathName(prefsManager->ghostscriptExecutable()) );
	args.append( "-h" );
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
	QProcess proc(args);
	proc.setCommunication(QProcess::Stdout);
	proc.start();
	while(proc.isRunning())
	{
#ifndef _WIN32
		usleep(5000);
#else
		Sleep(5);
#endif
		qApp->processEvents();
	}
	if(!proc.exitStatus())
		gsVer = proc.readLineStdout();
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

