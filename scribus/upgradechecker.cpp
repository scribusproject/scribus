/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QNetworkRequest>
#include <QTextCodec>
#include <QTextStream>
#include <QWidget>

#include <iostream>
#include <cstdlib>

#include "prefsmanager.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "ui/sctextbrowser.h"
#include "upgradechecker.h"


#ifdef _WIN32
#include <windows.h>
#define sleep(t) Sleep(t*1000)
#endif

UpgradeChecker::UpgradeChecker() : QObject()
{
	init();
	writeToConsole=true;
}

UpgradeChecker::~UpgradeChecker()
{
}

void UpgradeChecker::init()
{
	errorReported=false;
	message="";
	updates.clear();
	version=(VERSION);
	stability="unstablesvn";
	QString versionStripped=version.toLower();
	isCVS=versionStripped.contains("svn");
	if (isCVS)
		versionStripped.remove("svn");
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


void UpgradeChecker::fetch()
{
	QString filename("scribusversions.xml");
	tempFile=ScPaths::getTempFileDir()+filename;

	fin=false;
	
	rcvdFile=new QFile(tempFile);
	networkManager=new QNetworkAccessManager(this);
	if (networkManager!=0 && rcvdFile!=0)
	{
		outputText( tr("No data on your computer will be sent to an external location"));
		qApp->processEvents();
		if(rcvdFile->open(QIODevice::ReadWrite))
		{
			QString hostname("services.scribus.net");
			QString filepath("/"+filename);
			QUrl fileURL(QString("http://%1%2").arg(hostname).arg(filepath));
			outputText("<b>"+ tr("Attempting to get the Scribus version update file:")+"</b>");
			outputText(fileURL.toString());

			QNetworkRequest networkRequest(fileURL);
			networkReply = networkManager->get(networkRequest);
			connect(networkReply, SIGNAL(finished()), SLOT(downloadFinished()));
			connect(networkReply, SIGNAL(readyRead()), SLOT(downloadReadyRead()));

			int waitCount=0;
			while (!fin && waitCount<20)
			{
					sleep(1);
					++waitCount;
					if (writeToConsole)
							std::cout << ". " << std::flush;
					outputText( ".", true );
					qApp->processEvents();
			}
			if (writeToConsole)
					std::cout << std::endl;
			if (waitCount>=20)
			{
					outputText("<b>"+ tr("Timed out when attempting to get update file.")+"</b>");
			}
			rcvdFile->close();
		}
		rcvdFile->remove();
	}
	delete rcvdFile;
	rcvdFile=0;
	outputText( tr("Finished") );
	networkReply->deleteLater();
	networkManager->deleteLater();
}

void UpgradeChecker::downloadFinished()
{
	if (networkReply->error())
		outputText(QString("Failed: %1").arg(qPrintable(networkReply->errorString())));
	else
	{
		rcvdFile->reset();
		process();
		fin=true;
		show(networkReply->error()!=QNetworkReply::NoError);
	}
}


void UpgradeChecker::downloadReadyRead()
{
	rcvdFile->write(networkReply->readAll());
}

bool UpgradeChecker::process()
{
	if (!rcvdFile)
		return false;
	QTextStream ts(rcvdFile);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( "scribusversions" );
	QString data(ts.readAll());
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol )) 
	{
		if (data.toLower().contains("404 not found"))
			outputText("<b>"+ tr("File not found on server")+"</b>");
		else
			outputText("<b>"+ tr("Could not open version file: %1\nError:%2 at line: %3, row: %4").arg(rcvdFile->fileName()).arg(errorMsg).arg(eline).arg(ecol)+"</b>");
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
						bool newVersion = false;
						QString verA(e.attribute("version"));
						QString verAStripped=verA.toLower();
						bool verIsCVS=verAStripped.contains("cvs");
						if (verIsCVS)
							verAStripped.remove("cvs");
						uint verMajor=verAStripped.section('.',0,0).toInt();
						uint verMinor=verAStripped.section('.',1,1).toInt();
						uint verRevsion1=verAStripped.section('.',2,2).toInt();
						uint verRevsion2=verAStripped.section('.',3,3).toInt();
						//If we found a release whe a user is running an old CVS version
						if (verMajor==major && verMinor==minor && verRevsion1==revision1 && verRevsion2==revision2 && isCVS && !verIsCVS && !updates.contains(verA))
							newVersion = true;
						else
						//If we found a version that is not the same as what we are running
						if (!(verMajor==major && verMinor==minor && verRevsion1==revision1 && verRevsion2==revision2))
						{
							if (
								((verMajor>major) ||
								(verMajor==major && verMinor>minor) ||
								(verMajor==major && verMinor==minor && verRevsion1>revision1) ||
								(verMajor==major && verMinor==minor && verRevsion1==revision1 && verRevsion2>revision2))
								&& !updates.contains(verA)
								)
								newVersion = true;
						}
						if (newVersion)
						{
							QString ver = verA;
							QString link = e.attribute("link", "");
							if (!link.isEmpty())
							{
								QString linkStr = QString("<a href=\"%1\">%2</a>").arg(link).arg(link);
								ver = QString("%1 : %2").arg(verA).arg(linkStr);
							}
							updates.append(ver);
						}
					}
				}
			}
			else
			if (e.tagName()=="message")
			{
				message+=e.text();
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
	outputText("<br/>");
	if (error)
	{
		outputText("<b>"+ tr("An error occurred while looking for updates for Scribus, please check your internet connection.")+"</b>");
		return;
	}
	if (updates.isEmpty())
		outputText("<b>"+ tr("No updates are available for your version of Scribus %1").arg(version)+"</b>");
	else
	{
		outputText("<b>"+ tr("One or more updates for your version of Scribus (%1) are available:").arg(version)+"</b>");
		outputText( tr("This list may contain development/unstable versions."));
		for ( QStringList::Iterator it = updates.begin(); it != updates.end(); ++it )
			outputText(*it);
		outputText("<b>"+ tr("Please visit www.scribus.net for details.")+"</b>");
	}
	outputText(message);
}

void UpgradeChecker::outputText(QString text, bool /*noLineFeed*/)
{
	QString outText(text);
	outText.remove("<b>");
	outText.remove("</b>");
	outText.remove("<i>");
	outText.remove("</i>");
	outText.replace("<br>","\n");
	outText.replace("<br/>","\n");
	qDebug() << outText.toLocal8Bit().data();
}


void UpgradeChecker::reportError(const QString& s)
{
	if (!errorReported)
	{
		outputText("<br/><b>"+ tr("Error: %1").arg(s)+"</b>");
		errorReported=true;
	}
}

UpgradeCheckerGUI::UpgradeCheckerGUI(ScTextBrowser *tb) : UpgradeChecker()
{
	outputWidget=tb;
	writeToConsole=false;
}

UpgradeCheckerGUI::~UpgradeCheckerGUI()
{
}

void UpgradeCheckerGUI::outputText(QString text, bool noLineFeed)
{
	ScTextBrowser* w=outputWidget;
	if (w)
	{
		QString wText(w->toPlainText());
		wText.replace("\n","<br>");
		wText.remove("<qt>");
		wText.remove("</qt>");
		if (noLineFeed)
			w->setSimpleText("<qt>"+wText+text+"</qt>");
		else
			w->setSimpleText("<qt>"+wText+text+"<br>"+"</qt>");
	}	
}


