/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <qdir.h>
#include <qdom.h>
#include <qhttp.h>
#include <qnetwork.h>
#include <iostream>
#include <cstdlib>

#include "upgradechecker.h"
#include "upgradechecker.moc"
#include "prefsmanager.h"
#include "scribuscore.h"


UpgradeChecker::UpgradeChecker()
 : QObject()
{
	getter=0;
	updates.clear();
	version=(VERSION);
	stability="unstablecvs";
	QString versionStripped=version.lower();
	isCVS=versionStripped.contains("cvs");
	if (isCVS)
		versionStripped.remove("cvs");
	major=versionStripped.section('.',0,0).toInt();
	minor=versionStripped.section('.',1,1).toInt();
	revision1=versionStripped.section('.',2,2).toInt();
	revision2=versionStripped.section('.',3,4).toInt();
	#if defined(Q_OS_MAC)
	platform="MacOSX";
	#elif defined(Q_OS_WIN32)
	platform="Win32";
	#else
	platform="X11";
	#endif
}

UpgradeChecker::~UpgradeChecker()
{
}

bool UpgradeChecker::fetch()
{
	QString filename("scribusversions.xml");
	//TODO fix for win32 and mac
	tempFile=QDir::homeDirPath()+"/.scribus/"+filename;

	fin=false;
	
	QFile file(tempFile);
	if (getter)
		delete getter;
	getter=new QHttp();
	connect (getter, SIGNAL(done(bool)), this, SLOT(fileFinished(bool)));
	connect (getter, SIGNAL(requestStarted(int)), this, SLOT(reqStarted(int)));
	connect (getter, SIGNAL(requestFinished(int, bool)), this, SLOT(reqFinished(int, bool)));
	retrieveError=false;
	getter->setHost("www.scribus.net");
	if (retrieveError)
		return true;
	qDebug("%s", tr("Attempting to get the Scribus version update file").local8Bit().data());
	qDebug("%s", tr("(No data on your computer will be sent to an external location)").local8Bit().data());
	if(!file.open(IO_ReadWrite))
		return true;
	getterID=getter->get("/downloads/"+filename, &file);
	
	int waitCount=0;
	while (!fin && waitCount<10 && !retrieveError)
	{
		qApp->processEvents();
		sleep(1);
		++waitCount;
		std::cout << ". " << std::flush;
	}
	std::cout << std::endl;
	getter->closeConnection();
	bool errorOccurred=false;
	if (waitCount>=10)
	{
		qDebug("%s", tr("Timed out when attempting to get update file.").local8Bit().data());
		errorOccurred=true;
	}
	if (retrieveError || getter->error()!=QHttp::NoError)
	{
		qDebug("%s", tr("Error when attempting to get update file: %1").arg(getter->errorString()).local8Bit().data());
		errorOccurred=true;
	}
	if (errorOccurred)
	{
		file.close();
		file.remove();
		return true;
	}

	file.reset();
	process(file);
	file.close();
 	file.remove();
	return false;
}

bool UpgradeChecker::process( QFile& dataFile )
{
	
	QTextStream ts(&dataFile);
	ts.setEncoding(QTextStream::UnicodeUTF8);
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( "scribusversions" );
	QString data(ts.read());
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol )) 
	{
		if (data.lower().contains("404 not found"))
			qDebug("%s", tr("File not found on server").local8Bit().data());
		else
			qDebug("%s", tr("Could not open version file: %1\nError:%2 at line: %3, row: %4").arg(dataFile.name()).arg(errorMsg).arg(eline).arg(ecol).local8Bit().data());
		return false;
	}
	
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while( !n.isNull() ) {
		QDomElement e = n.toElement();
		if( !e.isNull() ) {
			if (e.tagName()=="release")
			{
				if (e.hasAttribute("stability") && e.hasAttribute("platform") && e.hasAttribute("version"))
				{
					if (e.attribute("platform")==platform)
					{
						QString verA(e.attribute("version"));
						QString verAStripped=verA.lower();
						bool verIsCVS=verAStripped.contains("cvs");
						if (verIsCVS)
							verAStripped.remove("cvs");
						uint verMajor=verAStripped.section('.',0,0).toInt();
						uint verMinor=verAStripped.section('.',1,1).toInt();
						uint verRevsion1=verAStripped.section('.',2,2).toInt();
						uint verRevsion2=verAStripped.section('.',3,3).toInt();
						if (!(verMajor==major && verMinor==minor && verRevsion1==revision1 && verRevsion2==revision2))
						{
							if (
								(verMajor>major) ||
								(verMajor==major && verMinor>minor) ||
								(verMajor==major && verMinor==minor && verRevsion1>revision1) ||
								(verMajor==major && verMinor==minor && verRevsion1==revision1 && verRevsion2>revision2)
								)
								updates.append(verA);
						}
					}
				}
			}
		}
		n = n.nextSibling();
	}
	return true;
}

QStringList UpgradeChecker::upgradeData( )
{
	return updates;
}

void UpgradeChecker::show(bool error)
{
	if (error)
	{
		qDebug("%s", tr("An error occurred while looking for updates for Scribus, please check your internet connection.").local8Bit().data());
		return;
	}
	if (updates.isEmpty())
	{
		qDebug("%s", tr("No updates are available for your version of Scribus %1").arg(version).local8Bit().data());
		return;
	}
	qDebug("%s", tr("One or more updates for your version of Scribus (%1) are available:").arg(version).local8Bit().data());
	
	for ( QStringList::Iterator it = updates.begin(); it != updates.end(); ++it )
		qDebug("%s", QString("%1").arg(*it).local8Bit().data());
}

void UpgradeChecker::fileFinished(bool /*error*/)
{
	fin=true;
}

void UpgradeChecker::fileStarted(bool /*error*/)
{
}

void UpgradeChecker::reqStarted(int /*id*/)
{
}

void UpgradeChecker::reqFinished(int /*id*/, bool error)
{
	retrieveError=error;
}
