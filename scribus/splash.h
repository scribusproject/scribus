#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <qpixmap.h>
#include <qwidget.h>

class SplashScreen : public QWidget
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

