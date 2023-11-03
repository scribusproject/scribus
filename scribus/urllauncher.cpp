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
#include <QString>
#include <QStringList>

#include "urllauncher.h"
#include "prefsmanager.h"

#include "scribuscore.h"
#include "util.h"

UrlLauncher& UrlLauncher::instance()
{
	static UrlLauncher _instance;
	return _instance;
}

void UrlLauncher::launchUrlExt(const QString& link, QWidget *parent)
{
	launchUrlExt(QUrl(link), parent);
}

void UrlLauncher::launchUrlExt(const QUrl& link, QWidget *parent)
{
	if (link.scheme()=="http" || link.scheme()=="https")
	{
		QWidget* p = parent;
		if (p == nullptr)
			p = ScCore->primaryMainWindow();
		QString extBrowser(PrefsManager::instance().extBrowserExecutable());
		if (extBrowser.isEmpty())
		{
			if (!QDesktopServices::openUrl(link))
			{
				extBrowser = QFileDialog::getOpenFileName(p, tr("Locate your web browser"), QString(), QString());
				if (!QFileInfo::exists(extBrowser))
					extBrowser.clear();
				PrefsManager::instance().setExtBrowserExecutable(extBrowser);
			}
		}
		if (!extBrowser.isEmpty())
		{
			QStringList args;
			args << link.toString();
			bool ok = QProcess::startDetached(extBrowser, args);
			if (!ok)
			{
				QMessageBox::StandardButton sb = ScMessageBox::critical(p, tr("External Web Browser Failed to Start"), tr("Scribus was not able to start the external web browser application %1. Please check the setting in Preferences.\nWould you like to start the system's default browser instead?").arg(PrefsManager::instance().extBrowserExecutable()), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
				if (sb == QMessageBox::Ok)
					QDesktopServices::openUrl(link);
			}
		}
	}
	else
		QDesktopServices::openUrl(link);
}
