/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QApplication>
#include <QDesktopWidget>
#include <qpainter.h>
#include <qpixmap.h>
#include <qregexp.h>
//Added by qt3to4:
#include <QMouseEvent>
#include "splash.h"
extern QPixmap loadIcon(QString nam);


SplashScreen::SplashScreen() 
	: QWidget( 0, 0, Qt::WStyle_Customize | Qt::WStyle_NoBorder | Qt::WStyle_StaysOnTop | Qt::WStyle_Tool |
	Qt::WStyle_Splash)
	 //WX11BypassWM )
{
	pix = loadIcon("Splash.png");
	Q_ASSERT(!pix.isNull());
	if (pix.isNull()) {
		pix = QPixmap(360, 200);
		pix.fill(Qt::darkGray);
	}
	setErasePixmap( pix );
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
		f = tmp.find(rx);
		if (f != -1)
		{
			tmp.remove(f, 1);
			f = 0;
		}
	}
	QPixmap textPix(pix);
	QPainter painter( &textPix, this );
#if defined _WIN32
	QFont font("Lucida Sans", 10);
#elif defined(__INNOTEK_LIBC__)
	QFont font("WarpSans", 9);
#else
	QFont font("Bitstream Vera Sans", 10);
#endif
	painter.setFont(font);
	painter.setPen(QColor(236,233,216));
	//painter.setRasterOp(NotROP);
	painter.drawText( 81, textPix.height()-8, tmp );
	painter.end();
	setErasePixmap( textPix );
	repaint();
}
