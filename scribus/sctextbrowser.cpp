/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QApplication>
#include <QCursor>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QToolTip>
#include <QUrl>

#if defined(_WIN32)
#include <windows.h>
#include <shellapi.h>
#endif

#include "prefsmanager.h"
#include "sctextbrowser.h"
#include "urllauncher.h"

ScTextBrowser::ScTextBrowser( QWidget * parent )
	 : QTextBrowser(parent)
{
	setOpenLinks(false);
	//textbrowser connections
	connect(this, SIGNAL(highlighted(const QString &)), this, SLOT(hoverMouse(const QString &)));
	connect(this, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(navigateOverride(const QUrl &)));
}

void ScTextBrowser::hoverMouse(const QString &link)
{
	if (link.isEmpty())
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	else
	{
		qApp->changeOverrideCursor(QCursor(Qt::PointingHandCursor));
//		Add this back in if we want links to show in tooltips
// 		QToolTip::showText(QCursor::pos(), link);
	}
	emit overLink(link);
}

void ScTextBrowser::navigateOverride(const QUrl & link)
{
	if (link.scheme()=="http")
		UrlLauncher::instance()->launchUrlExt(link, parentWidget());
	else
		setSource(link);
	/*
#if defined(_WIN32)
	if (link.scheme()=="http")
	{
		QString url(link.authority());
		ShellExecuteW( winId(), 0, (LPCWSTR)url.utf16(), 0, 0, SW_SHOWNORMAL );
		return;
	}
#endif
#if !defined(QT_OS_MAC) && !defined(_WIN32)
	if (link.scheme()=="http")
	{
		QString extBrowser(PrefsManager::instance()->extBrowserExecutable());
		QFileInfo fi(extBrowser);
		if (extBrowser.isEmpty() || !fi.exists())
		{
			extBrowser = QFileDialog::getOpenFileName(QString::null, QString::null, this, "changeExtBrowser", tr("Locate your web browser"));
			if (!QFileInfo(extBrowser).exists())
				extBrowser="";
			PrefsManager::instance()->setExtBrowserExecutable(extBrowser);
		}		
		if (!extBrowser.isEmpty())
		{
			QStringList args;
			args << link;
			QProcess webProc;
			if (!webProc.startDetached(extBrowser, args))
				QMessageBox::critical(this, tr("External Web Browser Failed to Start"), tr("Scribus was not able to start the external web browser application %1. Please check the setting in Preferences").arg(PrefsManager::instance()->extBrowserExecutable()), QMessageBox::Ok, QMessageBox::NoButton);
		}
	}
	else
		setSource(link);
#else
	setSource(link);
#endif
	*/
}
