#include <qapplication.h>
#include <qpainter.h>
#include <qpixmap.h>
#include "splash.h"
extern QPixmap loadIcon(QString nam);

SplashScreen::SplashScreen() 
			: QWidget( 0, 0, WStyle_Customize | WStyle_NoBorder | WStyle_StaysOnTop | WStyle_Tool | WX11BypassWM )
{
	pix = loadIcon("Splash.png");
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
	QPixmap textPix = pix;
	QPainter painter( &textPix, this );
	painter.setPen( black );
	painter.drawText( 10, 90, message );
	setErasePixmap( textPix );
	repaint();
}

