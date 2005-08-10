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

