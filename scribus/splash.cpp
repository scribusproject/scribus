#include <qapplication.h>
#include <qpainter.h>
#include <qpixmap.h>
#include "splash.h"
extern QPixmap loadIcon(QString nam);

/*!
 \fn SplashScreen::SplashScreen() 
 \author Franz Schmid
 \date
 \brief Constructor for SplashScreen
 \param None
 \retval None
 */
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

/*!
 \fn void SplashScreen::mousePressEvent( QMouseEvent * )
 \author Franz Schmid
 \date
 \brief When mouse is clicked the splashscreen is hidden
 \param QMouseEvent* QMouseEvent pointer
 \retval None
 */
void SplashScreen::mousePressEvent( QMouseEvent * )
{
	hide();
}

/*!
 \fn void SplashScreen::repaint()
 \author Franz Schmid
 \date
 \brief Repaints the splashscreen when status is changed in SplashScreen::setStatus
 \param None
 \retval None
 */
void SplashScreen::repaint()
{
	QWidget::repaint();
	QApplication::flush();
}

/*!
 \fn void SplashScreen::setStatus( const QString &message )
 \author Franz Schmid
 \date
 \brief Sets new status on SplashScreen and calls for a SplashScreen::repaint afterwards.
 \param message const QString& message to display as actions are performed on startup when SplashScreen is displayed.
 \retval None
 */
void SplashScreen::setStatus( const QString &message )
{
	QPixmap textPix = pix;
	QPainter painter( &textPix, this );
	painter.setPen( black );
	painter.drawText( 10, 90, message );
	setErasePixmap( textPix );
	repaint();
}