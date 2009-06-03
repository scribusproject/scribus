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
}
