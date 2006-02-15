/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qapplication.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qregexp.h>
#include "splash.h"
extern QPixmap loadIcon(QString nam);


SplashScreen::SplashScreen() 
	: QWidget( 0, 0, WStyle_Customize | WStyle_NoBorder | WStyle_StaysOnTop | WStyle_Tool | WX11BypassWM )
{
	pix = loadIcon("Splash.png");
	Q_ASSERT(!pix.isNull());
	if (pix.isNull())
		return;
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
	QString tmp = message;
	int f = 0;
	while (f != -1)
	{
		f = tmp.find(QRegExp( "&\\S*" ));
		if (f != -1)
		{
			tmp.remove(f, 1);
			f = 0;
		}
	}
	QPixmap textPix = pix;
	QPainter painter( &textPix, this );
	painter.setFont(QFont("Helvetica", 10));
	painter.setPen( white );
	//painter.setRasterOp(NotROP);
	painter.drawText( 10, textPix.height()-8, tmp );
	setErasePixmap( textPix );
	repaint();
}
