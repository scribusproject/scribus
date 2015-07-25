/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QPixmap>

#include "iconmanager.h"
#include "sctextbrowser.h"
#include "urllauncher.h"
#include "util.h"

ScTextBrowser::ScTextBrowser( QWidget * parent )
	 : QTextBrowser(parent)
{
	setOpenExternalLinks(true);
	connect(this, SIGNAL(sourceChanged(QUrl)), this, SLOT(catchHome(QUrl)));
	connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(externalLinkClick(QUrl)));
}

void ScTextBrowser::clear()
{
	setHtml(QString::fromLatin1("<html></html>"));
}

void ScTextBrowser::find(const QString& txt, const int& options)
{
	QTextBrowser::find(txt, QTextDocument::FindFlag(options));
}

void ScTextBrowser::home()
{
	setSource(homeUrl);
}


void ScTextBrowser::catchHome(QUrl url)
{
	if (homeUrl.isEmpty())
	{
		homeUrl = url;
		disconnect(this, SIGNAL(sourceChanged(QUrl)), this, SLOT(catchHome(QUrl)));
	}
}

void ScTextBrowser::externalLinkClick(QUrl url)
{
	if (url.scheme()=="http")
		UrlLauncher::instance()->launchUrlExt(url, parentWidget());
}

