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
#include "upgradechecker.h"


#ifdef _WIN32
#include <windows.h>
#define sleep(t) Sleep(t*1000)
#endif

UpgradeChecker::UpgradeChecker() : QObject()
{
	init();

	m_fin = false;
	m_file = 0;
	m_networkManager = 0;
	m_networkReply = 0;
	m_writeToConsole = true;
}

UpgradeChecker::~UpgradeChecker()
{
}

void UpgradeChecker::init()
{
	m_errorReported=false;
	m_message="";
	m_updates.clear();
	m_version=(VERSION);
	m_stability="unstablesvn";
	QString versionStripped=m_version.toLower();
	m_isCVS=versionStripped.contains("svn");
	if (m_isCVS)
		versionStripped.remove("svn");
	major=versionStripped.section('.',0,0).toInt();
	minor=versionStripped.section('.',1,1).toInt();
	m_revision1=versionStripped.section('.',2,2).toInt();
	m_revision2=versionStripped.section('.',3,4).toInt();
	#if defined(Q_OS_MAC)
	m_platform="MacOSX";
	#elif defined(Q_OS_WIN32)
	m_platform="Win32";
	#else
	m_platform="X11";
	#endif
}


void UpgradeChecker::fetch()
{
	QString filename("scribusversions.xml");
	m_tempFile=ScPaths::tempFileDir()+filename;

	m_fin=false;

	m_file=new QFile(m_tempFile);
	m_networkManager=new QNetworkAccessManager(this);
	if (m_networkManager!=0 && m_file!=0)
	{
		outputText( tr("No data on your computer will be sent to an external location"));
		qApp->processEvents();
		if(m_file->open(QIODevice::ReadWrite))
		{
			QString hostname("services.scribus.net");
			QString filepath("/"+filename);
			QUrl fileURL(QString("https://%1%2").arg(hostname, filepath));
			outputText("<b>"+ tr("Attempting to get the Scribus version update file:")+"</b>");
			outputText(fileURL.toString());

			QNetworkRequest networkRequest(fileURL);
			m_networkReply = m_networkManager->get(networkRequest);
			connect(m_networkReply, SIGNAL(finished()), SLOT(downloadFinished()));
			connect(m_networkReply, SIGNAL(readyRead()), SLOT(downloadReadyRead()));

			int waitCount=0;
			while (!m_fin && waitCount<20)
			{
					sleep(1);
					++waitCount;
					if (m_writeToConsole)
							std::cout << ". " << std::flush;
					outputText( ".", true );
					qApp->processEvents();
			}
			if (m_writeToConsole)
					std::cout << std::endl;
			if (waitCount>=20)
			{
					outputText("<b>"+ tr("Timed out when attempting to get update file.")+"</b>");
			}
			m_file->close();
		}
		m_file->remove();
	}
	delete m_file;
	m_file=0;
	outputText( tr("Finished") );
	m_networkReply->deleteLater();
	m_networkManager->deleteLater();
}

void UpgradeChecker::downloadFinished()
{
	if (m_networkReply->error())
		outputText(QString("Failed: %1").arg(qPrintable(m_networkReply->errorString())));
	else
	{
		m_file->reset();
		process();
		m_fin=true;
		show(m_networkReply->error()!=QNetworkReply::NoError);
	}
}


void UpgradeChecker::downloadReadyRead()
{
	m_file->write(m_networkReply->readAll());
}

bool UpgradeChecker::process()
{
	if (!m_file)
		return false;
	QTextStream ts(m_file);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	QString errorMsg;
	int eline;
	int ecol;
	QDomDocument doc( "scribusversions" );
	QString data(ts.readAll());
	if ( !doc.setContent( data, &errorMsg, &eline, &ecol )) 
	{
		if (data.contains("404 not found", Qt::CaseInsensitive))
			outputText("<b>"+ tr("File not found on server")+"</b>");
		else
			outputText("<b>"+ tr("Could not open version file: %1\nError:%2 at line: %3, row: %4").arg(m_file->fileName()).arg(errorMsg).arg(eline).arg(ecol)+"</b>");
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
					if (e.attribute("platform")==m_platform)
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
						if (verMajor==major && verMinor==minor && verRevsion1==m_revision1 && verRevsion2==m_revision2 && m_isCVS && !verIsCVS && !m_updates.contains(verA))
							newVersion = true;
						else
						//If we found a version that is not the same as what we are running
						if (!(verMajor==major && verMinor==minor && verRevsion1==m_revision1 && verRevsion2==m_revision2))
						{
							if (
								((verMajor>major) ||
								(verMajor==major && verMinor>minor) ||
								(verMajor==major && verMinor==minor && verRevsion1>m_revision1) ||
								(verMajor==major && verMinor==minor && verRevsion1==m_revision1 && verRevsion2>m_revision2))
								&& !m_updates.contains(verA)
								)
								newVersion = true;
						}
						if (newVersion)
						{
							QString ver(verA);
							QString link(e.attribute("link", ""));
							if (!link.isEmpty())
							{
								QString linkStr = QString("<a href=\"%1\">%2</a>").arg(link, link);
								ver = QString("%1 : %2").arg(verA, linkStr);
							}
							m_updates.append(ver);
						}
					}
				}
			}
			else
			if (e.tagName()=="message")
			{
				m_message+=e.text();
			}
		}
		n = n.nextSibling();
	}
	return true;
}

QStringList UpgradeChecker::upgradeData( )
{
	return m_updates;
}

void UpgradeChecker::show(bool error)
{
	outputText("<br/>");
	if (error)
	{
		outputText("<b>"+ tr("An error occurred while looking for updates for Scribus, please check your internet connection.")+"</b>");
		return;
	}
	if (m_updates.isEmpty())
		outputText("<b>"+ tr("No updates are available for your version of Scribus %1").arg(m_version)+"</b>");
	else
	{
		outputText("<b>"+ tr("One or more updates for your version of Scribus (%1) are available:").arg(m_version)+"</b>");
		outputText( tr("This list may contain development/unstable versions."));
		for ( QStringList::Iterator it = m_updates.begin(); it != m_updates.end(); ++it )
			outputText(*it);
		outputText("<b>"+ tr("Please visit www.scribus.net for details.")+"</b>");
	}
	outputText(m_message);
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
	if (!m_errorReported)
	{
		outputText("<br/><b>"+ tr("Error: %1").arg(s)+"</b>");
		m_errorReported=true;
	}
}

UpgradeCheckerGUI::UpgradeCheckerGUI(QTextBrowser *tb) : UpgradeChecker()
{
	m_outputWidget=tb;
	m_writeToConsole=false;
}

UpgradeCheckerGUI::~UpgradeCheckerGUI()
{
}

void UpgradeCheckerGUI::outputText(QString text, bool noLineFeed)
{
	QTextBrowser* w=m_outputWidget;
	if (w)
	{
		QString wText(w->toPlainText());
		wText.replace("\n","<br>");
		wText.remove("<qt>");
		wText.remove("</qt>");
		if (noLineFeed)
			w->setHtml("<qt>"+wText+text+"</qt>");
		else
			w->setHtml("<qt>"+wText+text+"<br>"+"</qt>");
	}	
}


