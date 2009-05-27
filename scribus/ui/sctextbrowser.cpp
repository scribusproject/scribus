/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sctextbrowser.h"

#include <QWebFrame>
#include <QEvent>
#include <QDebug>

#if defined(_WIN32)
#include <windows.h>
#include <shellapi.h>
#endif

#include "urllauncher.h"

ScTextBrowser::ScTextBrowser( QWidget * parent )
	 : QWebView(parent)
{
	// Questionable - e.g. I would like to browse only through help browser (a pref?)
	page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
	connect(this, SIGNAL(urlChanged(QUrl)), this, SLOT(catchHome(QUrl)));
	connect(this, SIGNAL(linkClicked(QUrl)), this, SLOT(externalLinkClick(QUrl)));
}

void ScTextBrowser::clear()
{
	setHtml(QString::fromAscii("<html></html>"));
}

void ScTextBrowser::setSimpleText(const QString& str)
{
	if(!str.isEmpty())
		setHtml(QString::fromLocal8Bit("<html><body>%1</body></html>").arg(str));
	else
		clear();
}

void ScTextBrowser::find(const QString& txt, const int& options)
{
	findText(txt, QWebPage::FindFlag(options));
}


QString ScTextBrowser::toPlainText()
{
	if(page())
	{
		if(page()->mainFrame())
		{
			return page()->mainFrame()->toPlainText();
		}
	}
	return QString();
}

void ScTextBrowser::home()
{
	setUrl(homeUrl);
}

void ScTextBrowser::catchHome(QUrl url)
{
	if(homeUrl.isEmpty())
	{
		homeUrl = url;
		disconnect(this, SIGNAL(urlChanged(QUrl)), this, SLOT(catchHome(QUrl)));
	}
}

void ScTextBrowser::externalLinkClick(QUrl url)
{
	UrlLauncher::instance()->launchUrlExt(url, parentWidget());
}

bool ScTextBrowser::event(QEvent * e)
{
//	bool ret(QWebView::event(e));
//	qDebug()<<"ScTextBrowser::event"<<e->type()<<ret;
//	return ret;
	return QWebView::event(e);
}
