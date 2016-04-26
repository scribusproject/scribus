/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QRegExp>

#include "scconfig.h"
#include "splash.h"

#include "util.h"

#ifdef HAVE_SVNVERSION
	#include "svnversion.h"
#endif

ScSplashScreen::ScSplashScreen( const QPixmap & pixmap, Qt::WindowFlags f ) : QSplashScreen( pixmap, f)
{
#if defined _WIN32
	QFont font("Lucida Sans Unicode", 9);
#elif defined(__INNOTEK_LIBC__)
	QFont font("WarpSans", 8);
#elif defined(Q_OS_MAC)
	QFont font("Helvetica Regular", 11);
#else
	QFont font("DejaVu Sans", 8);
	if (!font.exactMatch())
		font.setFamily("Bitstream Vera Sans");
#endif
	setFont(font);
}

void ScSplashScreen::setStatus( const QString &message )
{
	static QRegExp rx("&\\S*");	
	QString tmp(message);
	int f = 0;
	while (f != -1)
	{
		f = tmp.indexOf(rx);
		if (f != -1)
		{
			tmp.remove(f, 1);
			f = 0;
		}
	}

	showMessage ( tmp, Qt::AlignRight | Qt::AlignAbsolute | Qt::AlignBottom, Qt::white );
}

void ScSplashScreen::drawContents(QPainter* painter)
{
	QFont f(font());
	QSplashScreen::drawContents(painter);
	QRect r = rect().adjusted(0, 0, -15, -60);
	QFont lgf(font());
#if defined _WIN32
	lgf.setPointSize(30);
#elif defined(__INNOTEK_LIBC__)
	lgf.setPointSize(29);
#elif defined(Q_OS_MAC)
	lgf.setPointSize(32);
#else
	lgf.setPointSize(29);
#endif
	QString versionText(VERSION);
	painter->setFont(lgf);
	painter->drawText(r, Qt::AlignRight | Qt::AlignAbsolute | Qt::AlignBottom, versionText );

	if (versionText.contains("svn"))
	{
#if defined(HAVE_SVNVERSION) && defined(SVNVERSION)
		QString revText;
		revText=QString("SVN Revision: %1").arg(SVNVERSION);
		QRect r2 = rect().adjusted(0, 0, -15, -50);
		painter->setFont(f);
		painter->drawText(r2, Qt::AlignRight | Qt::AlignAbsolute | Qt::AlignBottom, revText );
#endif
		QFont wf(font());
#if defined _WIN32
		wf.setPointSize(10);
#elif defined(__INNOTEK_LIBC__)
		wf.setPointSize(9);
#elif defined(Q_OS_MAC)
		wf.setPointSize(12);
#else
		wf.setPointSize(9);
#endif
		painter->setFont(wf);
		painter->setPen(QPen(Qt::red));
		QString warningText("UNSTABLE. For testing purposes only!");
		QRect r3 = rect().adjusted(0, 0, -15, -25);
		painter->drawText(r3, Qt::AlignRight | Qt::AlignAbsolute | Qt::AlignBottom, warningText );
	}
}

