/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QLabel>
#include <QSplashScreen>
#include <QPixmap>
#include <QString>
#include "scribusapi.h"


//! \brief Scribus splash screen
class SCRIBUS_API ScSplashScreen : public QSplashScreen
{
	Q_OBJECT

	public:
		/*!
		\author Franz Schmid
		\brief Constructor for SplashScreen
		 */
		ScSplashScreen ( const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = Qt::WindowFlags() );
		~ScSplashScreen() {}
		/*!
		\author Franz Schmid
		\brief Sets new status on SplashScreen and calls for a SplashScreen::repaint afterwards.
		\param message const QString& message to display as actions are performed on startup when SplashScreen is displayed.
		*/
		void setStatus( const QString &message );

	protected:
		void drawContents(QPainter *painter);
};

#endif

