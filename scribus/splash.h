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

class SCRIBUS_API SplashScreen : public QWidget
{
public:
	SplashScreen();
	~SplashScreen() {};
	void setStatus( const QString &message );
	void repaint();

protected:
	void mousePressEvent( QMouseEvent * );

private:
	QPixmap pix;
};

#endif

