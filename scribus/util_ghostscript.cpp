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

#include "prefsfile.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribus.h"
#include "util.h"

using namespace std;


int callGS(const QStringList& args_in, const QString device)
{
	QString cmd;
 	QStringList args;
	PrefsManager* prefsManager = PrefsManager::instance();
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
	if (prefsManager->appPrefs.gs_AntiAliasText)
		args.append( "-dTextAlphaBits=4" );
	if (prefsManager->appPrefs.gs_AntiAliasGraphics)
		args.append( "-dGraphicsAlphaBits=4" );

	// Add any extra font paths being used by Scribus to gs's font search path
	PrefsContext *pc = PrefsManager::instance()->prefsFile->getContext("Fonts");
	PrefsTable *extraFonts = pc->getTable("ExtraFontDirs");
	const char sep = ScPaths::envPathSeparator;
	if (extraFonts->getRowCount() >= 1)
		cmd = QString("-sFONTPATH=%1").arg(QDir::convertSeparators(extraFonts->get(0,0)));
	for (int i = 1; i < extraFonts->getRowCount(); ++i)
		cmd += QString("%1%2").arg(sep).arg(QDir::convertSeparators(extraFonts->get(i,0)));
	if( !cmd.isEmpty() )
		args.append( cmd );

	args += args_in;
	args.append("-c");
	args.append("showpage");
//	qDebug(args.join(" ").toAscii());
	return System( getShortPathName(prefsManager->ghostscriptExecutable()), args );
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
	else if (!ScCore->havePNGAlpha())
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
	return system(cmd1.toLocal8Bit().constData());
}

int convertPS2PS(QString in, QString out, const QStringList& opts, int level)
{
	PrefsManager* prefsManager=PrefsManager::instance();
	QStringList args;
	args.append( "-q" );
	args.append( "-dQUIET" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	if( level == 2 )
	{
		int major = 0, minor = 0;
		// ps2write cannot be detected with testGSAvailability()
		// so determine availability according to gs version.
		getNumericGSVersion(major, minor);
		if ((major >=8 && minor >= 53) || major > 8)
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
		if(level <= 3)
			args.append( QString("-dLanguageLevel=%1").arg(level) );
	}
	args += opts;
	args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(out)) );
	args.append( QDir::convertSeparators(in) );
	int ret = System( getShortPathName(prefsManager->ghostscriptExecutable()), args );
	return ret;
}

int convertPS2PDF(QString in, QString out, const QStringList& opts)
{
	PrefsManager* prefsManager=PrefsManager::instance();
	QStringList args;
	args.append( "-q" );
	args.append( "-dQUIET" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	args.append( "-sDEVICE=pdfwrite" );
	args += opts;
	args.append( QString("-sOutputFile=%1").arg(QDir::convertSeparators(out)) );
	args.append( QDir::convertSeparators(in) );
	int ret = System( getShortPathName(prefsManager->ghostscriptExecutable()), args );
	return ret;
}

bool testGSAvailability( void )
{
	QStringList args;
	PrefsManager* prefsManager = PrefsManager::instance();
	return testGSAvailability(prefsManager->ghostscriptExecutable());
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
	return (proc.exitCode()==0);
}

bool testGSDeviceAvailability( const QString& device )
{
	QStringList args;
	PrefsManager* prefsManager = PrefsManager::instance();
	args.append( QString("-sDEVICE=%1").arg( device ) );
	args.append( "-c" );
	args.append( "quit" );
	QProcess proc;
	proc.start(getShortPathName(prefsManager->ghostscriptExecutable()), args);
	if (!proc.waitForStarted(5000))
		return false;
	proc.waitForFinished(5000);
	return (proc.exitCode()==0);
}

// Return the GhostScript version string, or QString::null if it couldn't be retrived.
QString getGSVersion()
{
	QStringList args;
	args.append(QString("--version").toLocal8Bit());
	QString gsExe = getShortPathName(PrefsManager::instance()->ghostscriptExecutable());
	QProcess proc;
	proc.start(gsExe.toLocal8Bit(), args);
	if (proc.waitForStarted(5000))
		while (!proc.waitForFinished(5000))
			qApp->processEvents();
	QString gsVer;
	if (proc.exitStatus()==QProcess::NormalExit)
		gsVer = proc.readAllStandardOutput();
	return gsVer;
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
	if (!success)
		return false;
	return true;
}

QString getGSDefaultExeName(void)
{
	QString gsName("gs");
#if defined _WIN32
	// Set gsName to its default value
	gsName = "gswin32c.exe";

	QMap<int, QString> gplGS  = getGSExePaths("SOFTWARE\\GPL Ghostscript");
	QMap<int, QString> afplGS = getGSExePaths("SOFTWARE\\AFPL Ghostscript");
	QMap<int, QString> gsVersions = gplGS.unite(afplGS);

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
	gsPaths << "/usr/bin/gs" << "/usr/local/bin/gs" << "/opt/local/bin/gs";
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

QMap<int, QString> SCRIBUS_API getGSExePaths(const QString& regKey)
{
	QMap<int, QString> gsVersions;
#if defined _WIN32
	// Try to locate GhostScript thanks to the registry
	DWORD size;
	HKEY hKey1, hKey2;
	DWORD regType = REG_SZ;
	WCHAR regVersion[MAX_PATH];
	WCHAR regPath[MAX_PATH];
	WCHAR gsPath[MAX_PATH];
	QString gsVersion, gsName;

	if( RegOpenKeyW(HKEY_LOCAL_MACHINE, (LPCWSTR) regKey.utf16(), &hKey1) == ERROR_SUCCESS )
	{
		size = sizeof(regVersion)/sizeof(WCHAR) - 1;
		DWORD keyIndex = 0;
		while ( RegEnumKeyExW(hKey1, keyIndex, regVersion, &size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS )
		{
			int gsNumericVer, gsMajor, gsMinor;
			wcscpy(regPath, (const wchar_t*) regKey.utf16());
			wcscat(regPath, L"\\");
			wcscat(regPath, regVersion);
			if (RegOpenKeyW(HKEY_LOCAL_MACHINE, regPath, &hKey2) == ERROR_SUCCESS)
			{
				size = sizeof(gsPath) - 1;
				if (RegQueryValueExW(hKey2, L"GS_DLL", 0, &regType, (LPBYTE) gsPath, &size) == ERROR_SUCCESS)
				{
					// We now have GhostScript dll path, but we want gswin32c.exe
					// Normally gswin32c.exe and gsdll.dll are in the same directory
					if ( getNumericGSVersion(QString::fromUtf16((const ushort*) regVersion), gsMajor, gsMinor) )
					{
						gsNumericVer = gsMajor * 1000 + gsMinor;
						gsName = QString::fromUtf16((const ushort*) gsPath);
						size   = gsName.lastIndexOf("\\");
						if (size > 0)
						{
							gsName  = gsName.left(size + 1);
							gsName += "gswin32c.exe";
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
	PrefsManager* prefsManager=PrefsManager::instance();
	if (getNumericGSVersion(gsMajor, gsMinor))
	{
		gsNumericVer = gsMajor * 1000 + gsMinor;
		gsVersions.insert(gsNumericVer, prefsManager->ghostscriptExecutable());
	}
#endif
	return gsVersions;
}

QPixmap LoadPDF(QString fn, int Page, int Size, int *w, int *h)
{
	QString tmp, cmd1, cmd2;
	QString pdfFile = QDir::convertSeparators(fn);
	QString tmpFile = QDir::convertSeparators(ScPaths::getTempFileDir() + "sc.png");
	QPixmap pm;
	int ret = -1;
	tmp.setNum(Page);
	QStringList args;
	args.append("-r72");
//	args.append("-sOutputFile=\""+tmpFile+"\"");
	args.append("-sOutputFile="+tmpFile);
	args.append("-dFirstPage="+tmp);
	args.append("-dLastPage="+tmp);
//	args.append("\""+pdfFile+"\"");
	args.append(pdfFile);
	ret = callGS(args);
	if (ret == 0)
	{
		QImage image;
		image.load(tmpFile);
		QFile::remove(tmpFile);
		QImage im2;
		*h = image.height();
		*w = image.width();
		double sx = image.width() / static_cast<double>(Size);
		double sy = image.height() / static_cast<double>(Size);
		double t = (sy < sx ? sx : sy);
		im2 = image.scaled(static_cast<int>(image.width() / t), static_cast<int>(image.height() / t), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		pm=QPixmap::fromImage(im2);
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


