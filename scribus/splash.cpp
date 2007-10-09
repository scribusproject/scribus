/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QRegExp>

#include "splash.h"

#include "util.h"
#include "util_icon.h"


SplashScreen::SplashScreen() : QWidget( 0, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::SplashScreen)
	 //WX11BypassWM )
{
	pix = loadIcon("Splash.png", true);
	Q_ASSERT(!pix.isNull());
	if (pix.isNull()) {
		pix = QPixmap(360, 200);
		pix.fill(Qt::darkGray);
	}
	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(pix));
	setPalette(palette);
	resize( pix.size() );
	QRect scr = QApplication::desktop()->screenGeometry();
	move( scr.center() - rect().center() );
	show();
	repaint();
}

void SplashScreen::mousePressEvent( QMouseEvent * )
{
	hide();
}

void SplashScreen::repaint()
{
	QWidget::repaint();
	QApplication::flush();
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
	QPixmap textPix(pix);
	QPainter painter( &textPix);// Qt4, this );
#if defined _WIN32
	QFont font("Lucida Sans Unicode", 9);
#elif defined(__INNOTEK_LIBC__)
	QFont font("WarpSans", 8);
#elif defined(Q_OS_MAC)
	QFont font("Helvetica Regular", 11);
#else
	QFont font("Bitstream Vera Sans", 8);
#endif
	painter.setFont(font);
//	painter.setPen(QColor(236,233,216));
	painter.setPen(Qt::white);
	//painter.setRasterOp(NotROP);
	painter.drawText( 315, textPix.height()-8, tmp );
	painter.end();
	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(textPix));
	setPalette(palette);
	repaint();
}
