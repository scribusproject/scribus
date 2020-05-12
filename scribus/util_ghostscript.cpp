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

#include "util_ghostscript.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>
#include <QProcess>

#include "scconfig.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if defined(_WIN32)
#include <windows.h>
#ifndef KEY_WOW64_32KEY
	#define KEY_WOW64_32KEY (0x0200)
#endif
#ifndef KEY_WOW64_64KEY
	#define KEY_WOW64_64KEY (0x0100)
#endif
#endif

#include "prefsfile.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribuscore.h"

#include "util.h"

using namespace std;


int callGS(const QStringList& args_in, const QString& device, const QString& fileStdErr, const QString& fileStdOut)
{
	QString cmd;
 	QStringList args;
	PrefsManager& prefsManager = PrefsManager::instance();
	args.append( "-q" );
	args.append( "-dNOPAUSE" );
	args.append( "-dQUIET" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	// Choose rendering device
	if (!device.isEmpty())
		args.append( QString("-sDEVICE=%1").arg(device) ); // user specified device
	else if (!ScCore->havePNGAlpha())
		args.append( "-sDEVICE=png16m" );
	else
		args.append( "-sDEVICE=pngalpha" );
	// and antialiasing
	if (prefsManager.appPrefs.extToolPrefs.gs_AntiAliasText)
		args.append( "-dTextAlphaBits=4" );
	if (prefsManager.appPrefs.extToolPrefs.gs_AntiAliasGraphics)
		args.append( "-dGraphicsAlphaBits=4" );

	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = PrefsManager::instance().prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH=%1").arg(QDir::toNativeSeparators(extraFonts->get(0,0)));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString("%1%2").arg(sep).arg(QDir::toNativeSeparators(extraFonts->get(i,0)));
	if (!cmd.isEmpty())
		args.append( cmd );

	args += args_in;
	args.append("-c");
	args.append("showpage");
//	qDebug(args.join(" ").toLatin1());
	return System( getShortPathName(prefsManager.ghostscriptExecutable()), args, fileStdErr, fileStdOut );
}

int callGS(const QString& args_in, const QString& device)
{
	PrefsManager& prefsManager=PrefsManager::instance();
	QString cmd1 = getShortPathName(prefsManager.ghostscriptExecutable());
	cmd1 += " -q -dNOPAUSE -dQUIET -dPARANOIDSAFER -dBATCH";
	// Choose rendering device
	if (!device.isEmpty())
		// user specified device
		cmd1 += " -sDEVICE="+device;
	else if (!ScCore->havePNGAlpha())
		cmd1 += " -sDEVICE=png16m";
	else
		cmd1 += " -sDEVICE=pngalpha";
	// and antialiasing
	if (prefsManager.appPrefs.extToolPrefs.gs_AntiAliasText)
		cmd1 += " -dTextAlphaBits=4";
	if (prefsManager.appPrefs.extToolPrefs.gs_AntiAliasGraphics)
		cmd1 += " -dGraphicsAlphaBits=4";

	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = PrefsManager::instance().prefsFile->getContext("Fonts");
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
	return system(cmd1.toLocal8Bit().constData());
}

int convertPS2PS(const QString& in, const QString& out, const QStringList& opts, int level)
{
	PrefsManager& prefsManager=PrefsManager::instance();
	QStringList args;
	args.append( "-q" );
	args.append( "-dQUIET" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	if (level == 2)
	{
		int gsVersion = 0;
		// ps2write cannot be detected with testGSAvailability()
		// so determine availability according to gs version.
		getNumericGSVersion(gsVersion);
		if (gsVersion >= 853)
			args.append( "-sDEVICE=ps2write" );
		else
		{
			args.append( "-sDEVICE=pswrite" );
			args.append( QString("-dLanguageLevel=%1").arg(level) );
		}
			
	}
	else
	{
		args.append( "-sDEVICE=pswrite" );
		if (level <= 3)
			args.append( QString("-dLanguageLevel=%1").arg(level) );
	}
	args += opts;
	args.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(out)) );
	args.append( QDir::toNativeSeparators(in) );
	int ret = System( getShortPathName(prefsManager.ghostscriptExecutable()), args );
	return ret;
}

int convertPS2PDF(const QString& in, const QString& out, const QStringList& opts)
{
	PrefsManager& prefsManager=PrefsManager::instance();
	QStringList args;
	args.append( "-q" );
	args.append( "-dQUIET" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	args.append( "-sDEVICE=pdfwrite" );
	args += opts;
	args.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(out)) );
	args.append( QDir::toNativeSeparators(in) );
	int ret = System( getShortPathName(prefsManager.ghostscriptExecutable()), args );
	return ret;
}

bool testGSAvailability()
{
	QStringList args;
	PrefsManager& prefsManager = PrefsManager::instance();
	return testGSAvailability(prefsManager.ghostscriptExecutable());
}

bool testGSAvailability( const QString& gsPath )
{
	QStringList args;
	args.append( "-h" );
	QProcess proc;
	proc.start(getShortPathName(gsPath), args);
	if (!proc.waitForStarted(5000))
		return false;
	proc.waitForFinished(5000);
	return (proc.exitCode() == 0);
}

bool testGSDeviceAvailability( const QString& device )
{
	QStringList args;
	PrefsManager& prefsManager = PrefsManager::instance();
	args.append( QString("-sDEVICE=%1").arg( device ) );
	args.append( "-c" );
	args.append( "quit" );
	QProcess proc;
	proc.start(getShortPathName(prefsManager.ghostscriptExecutable()), args);
	if (!proc.waitForStarted(5000))
		return false;
	proc.waitForFinished(5000);
	return (proc.exitCode() == 0);
}

// Return the GhostScript version string, or QString() if it couldn't be retrived.
QString getGSVersion()
{
	QStringList args;
	args.append(QString("--version").toLocal8Bit());
	QString gsExe = getShortPathName(PrefsManager::instance().ghostscriptExecutable());
	QProcess proc;
	proc.start(gsExe.toLocal8Bit(), args);
	if (proc.waitForStarted(5000))
		while (!proc.waitForFinished(5000))
			qApp->processEvents();
	QString gsVer;
	if (proc.exitStatus() == QProcess::NormalExit)
		gsVer = proc.readAllStandardOutput();
	return gsVer;
}

bool getNumericGSVersion(int &version)
{
	int gsMajor(0), gsMinor(0);
	version = 0;
	if (getNumericGSVersion(gsMajor, gsMinor))
	{
		version = 100 * gsMajor + gsMinor;
		return true;
	}
	return false;
}

// Return the GhostScript major and minor version numbers.
bool getNumericGSVersion(int & major, int & minor)
{
	QString gs_ver_string(getGSVersion());
	return getNumericGSVersion(gs_ver_string, major, minor);
}

bool getNumericGSVersion(const QString& ver, int& major, int& minor)
{
	// gs's version string is of the form MAJOR.MINOR, so look for the .
	// then convert to numbers. 7.07 will become (7,7) for example.
	bool success = false;
	major = ver.section('.', 0, 0).toInt(&success);
	if (!success)
		return false;
	minor = ver.section('.', 1, 1).toInt(&success);
	return success;
}

QString getGSDefaultExeName()
{
	QString gsName("gs");
#if defined _WIN32
	// Set gsName to its default value
	gsName = "gswin32c.exe";

	// Test is we are running a 64bit version of WINDOWS
	bool isWindows64 = false;
	wchar_t* procArch = _wgetenv(L"PROCESSOR_ARCHITECTURE");
	if (procArch)
	{
		isWindows64 |= (wcscmp(procArch, L"AMD64") == 0);
		isWindows64 |= (wcscmp(procArch, L"IA64") == 0);
	}
	wchar_t* procArchWow64 = _wgetenv(L"PROCESSOR_ARCHITEW6432");
	if (procArchWow64) isWindows64 = true;

	// Search for Ghostsscript executable in native registry
	QMap<int, QString> gsVersions;
	gsVersions.unite( getGSExePaths("SOFTWARE\\GPL Ghostscript") );
	gsVersions.unite( getGSExePaths("SOFTWARE\\AFPL Ghostscript") );

	// If running on Windows 64bit, search alternate registry view,
	// ie 32bit registry if process is 64bit, 64bit registry if process is 32bit
	if (isWindows64)
	{
		gsVersions.unite( getGSExePaths("SOFTWARE\\GPL Ghostscript", true) );
		gsVersions.unite( getGSExePaths("SOFTWARE\\AFPL Ghostscript", true) );
	}

	if (gsVersions.isEmpty())
		return gsName;

	int currentVer = 0;
	QString gsPath;
	QMap<int, QString>::ConstIterator it, itEnd = gsVersions.constEnd();
	for (it = gsVersions.constBegin(); it != itEnd; ++it)
	{
		int version = it.key();
		if (version > currentVer)
		{
			gsPath = it.value();
			QFileInfo fInfo(gsPath);
			if (fInfo.exists())
			{
				gsName = gsPath;
				currentVer = version;
			}
		}
	}
#endif
#if defined Q_OS_MAC
	QStringList gsPaths;
	gsPaths << "/usr/bin/gs" << "/usr/local/bin/gs" << "/opt/local/bin/gs" << "/sw/bin/gs";
	for (int i = 0; i < gsPaths.size(); ++i)
	{
		QFileInfo fInfo(gsPaths.at(i));
		if (fInfo.exists())
		{
			gsName = gsPaths.at(i);
			break;
		}
	}
#endif
	return gsName;
}

QMap<int, QString> SCRIBUS_API getGSExePaths(const QString& regKey, bool alternateView)
{
	QMap<int, QString> gsVersions;
#if defined _WIN32
	// Try to locate GhostScript thanks to the registry
	DWORD size, regVersionSize;
	HKEY hKey1, hKey2;
	DWORD regType = REG_SZ;
	REGSAM flags  = KEY_READ;
	WCHAR regVersion[MAX_PATH] = {};
	WCHAR regPath[MAX_PATH] = {};
	WCHAR gsPath[MAX_PATH] = {};
	QString gsVersion, gsExeName, gsName;

	bool isWin64Api = false;
#if defined(_WIN64)
	isWin64Api = true;
#endif

	gsExeName = isWin64Api ? "gswin64c.exe" : "gswin32c.exe";
	if (alternateView)
	{
		gsExeName = isWin64Api ? "gswin32c.exe" : "gswin64c.exe";
		flags |= (isWin64Api ? KEY_WOW64_32KEY : KEY_WOW64_64KEY);
	}

	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, (LPCWSTR) regKey.utf16(), 0, flags, &hKey1) == ERROR_SUCCESS)
	{
		regVersionSize = sizeof(regVersion) / sizeof(WCHAR) - 1;
		DWORD keyIndex = 0;
		while (RegEnumKeyExW(hKey1, keyIndex, regVersion, &regVersionSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
		{
			int gsNumericVer, gsMajor, gsMinor;
			wcscpy(regPath, (const wchar_t*) regKey.utf16());
			wcscat(regPath, L"\\");
			wcscat(regPath, regVersion);
			if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, regPath, 0, flags, &hKey2) == ERROR_SUCCESS)
			{
				size = sizeof(gsPath) - 2;
				if (RegQueryValueExW(hKey2, L"GS_DLL", 0, &regType, (LPBYTE) gsPath, &size) == ERROR_SUCCESS)
				{
					// We now have GhostScript dll path, but we want gswin32c.exe
					// Normally gswin32c.exe and gsdll.dll are in the same directory
					if (getNumericGSVersion(QString::fromUtf16((const ushort*) regVersion), gsMajor, gsMinor))
					{
						gsNumericVer = gsMajor * 100 + gsMinor;
						gsName = QString::fromUtf16((const ushort*) gsPath);
						size   = gsName.lastIndexOf("\\");
						if (size > 0)
						{
							gsName  = gsName.left(size + 1);
							gsName += gsExeName;
							gsName.replace("\\", "/");
							gsVersions.insert(gsNumericVer, gsName);
						}	
					}	
				}
				RegCloseKey(hKey2);
			}
			keyIndex++;
		}
		RegCloseKey(hKey1);
	}
#else
	int gsNumericVer, gsMajor, gsMinor;
	PrefsManager& prefsManager = PrefsManager::instance();
	if (getNumericGSVersion(gsMajor, gsMinor))
	{
		gsNumericVer = gsMajor * 100 + gsMinor;
		gsVersions.insert(gsNumericVer, prefsManager.ghostscriptExecutable());
	}
#endif
	return gsVersions;
}

QPixmap loadPDF(const QString& fn, int page, int size, int *w, int *h)
{
	QString tmp;
	QString pdfFile = QDir::toNativeSeparators(fn);
	QString tmpFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "sc.png");

	tmp.setNum(page);
	QStringList args;
	args.append("-r72");
//	args.append("-sOutputFile=\"" + tmpFile + "\"");
	args.append("-sOutputFile=" + tmpFile);
	args.append("-dFirstPage=" + tmp);
	args.append("-dLastPage=" + tmp);
//	args.append("\"" + pdfFile + "\"");
	args.append(pdfFile);

	int ret = callGS(args);
	if (ret != 0)
		return QPixmap();

	QImage image;
	image.load(tmpFile);
	QFile::remove(tmpFile);
	*w = image.width();
	*h = image.height();
	if ((*w <= 0) || (*h <= 0))
		return QPixmap();

	double sx = image.width() / static_cast<double>(size);
	double sy = image.height() / static_cast<double>(size);
	double t = (sy < sx ? sx : sy);
	QImage im2 = image.scaled(static_cast<int>(image.width() / t), static_cast<int>(image.height() / t), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap pm = QPixmap::fromImage(im2);
	QPainter p;
	p.begin(&pm);
	p.setBrush(Qt::NoBrush);
	p.setPen(Qt::black);
	p.drawRect(0, 0, pm.width(), pm.height());
	p.end();
	im2.detach();

	return pm;
}


