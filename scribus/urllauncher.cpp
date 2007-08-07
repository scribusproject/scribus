/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QString>
#include <QStringList>

#include "urllauncher.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "scribuscore.h"

UrlLauncher* UrlLauncher::_instance = 0;

UrlLauncher::UrlLauncher() : QObject()
{
}

UrlLauncher::~UrlLauncher()
{
}

UrlLauncher* UrlLauncher::instance()
{
	if (_instance == 0)
		_instance = new UrlLauncher();

	return _instance;
}

void UrlLauncher::deleteInstance()
{
	if (_instance)
		delete _instance;
	_instance = 0;
}

void UrlLauncher::launchUrlExt(const QString& link, QWidget *parent)
{
	launchUrlExt(QUrl(link), parent);
}

void UrlLauncher::launchUrlExt(const QUrl& link, QWidget *parent)
{
	if (link.scheme()=="http")
	{
		QWidget *p=parent;
		if (p==0)
			p=ScCore->primaryMainWindow();
		QString extBrowser(PrefsManager::instance()->extBrowserExecutable());
		QFileInfo fi(extBrowser);
		if (extBrowser.isEmpty() || !fi.exists())
		{
			if (!QDesktopServices::openUrl(link))
			{
				extBrowser = QFileDialog::getOpenFileName(QString::null, QString::null, p, "changeExtBrowser", tr("Locate your web browser"));
				if (!QFileInfo(extBrowser).exists())
					extBrowser="";
				PrefsManager::instance()->setExtBrowserExecutable(extBrowser);
			}
		}
		if (!extBrowser.isEmpty())
		{
			QStringList args;
			args << link;
			QProcess webProc;
			if (!webProc.startDetached(extBrowser, args))
				QMessageBox::critical(p, tr("External Web Browser Failed to Start"), tr("Scribus was not able to start the external web browser application %1. Please check the setting in Preferences. Scribus will attempt to launch the system's default browser.").arg(PrefsManager::instance()->extBrowserExecutable()), QMessageBox::Ok, QMessageBox::NoButton);
		}
	}
	else
		QDesktopServices::openUrl(link);
}
