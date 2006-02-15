/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <qpixmap.h>
#include <qwidget.h>
#include "scribusapi.h"


//! \brief Scribus splash screen
class SCRIBUS_API SplashScreen : public QWidget
{
public:
	/*!
	\author Franz Schmid
	\brief Constructor for SplashScreen
	 */
	SplashScreen();
	~SplashScreen() {};
	/*!
	\author Franz Schmid
	\brief Sets new status on SplashScreen and calls for a SplashScreen::repaint afterwards.
	\param message const QString& message to display as actions are performed on startup when SplashScreen is displayed.
	*/
	void setStatus( const QString &message );
	/*!
	\author Franz Schmid
	\brief Repaints the splashscreen when status is changed in SplashScreen::setStatus
	*/
	void repaint();

protected:
	/*!
	\author Franz Schmid
	\brief When mouse is clicked the splashscreen is hidden
	Hide splash on click
	\param e QMouseEvent pointer
	 */
	void mousePressEvent( QMouseEvent * e);

private:
	QPixmap pix;
};

#endif

