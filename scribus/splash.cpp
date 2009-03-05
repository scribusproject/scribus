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

#include "splash.h"

#include "util.h"
#include "util_icon.h"


SplashScreen::SplashScreen( const QPixmap & pixmap, Qt::WindowFlags f ) : QSplashScreen( pixmap, f)
{

}

void SplashScreen::setStatus( const QString &message )
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
	showMessage ( tmp, Qt::AlignRight | Qt::AlignBottom, Qt::white );
}
