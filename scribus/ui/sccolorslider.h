/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCCOLORSLIDER_H
#define SCCOLORSLIDER_H

#include <QSlider>
#include "scribusapi.h"

class QPaintEvent;
class QWidget;

class SCRIBUS_API ScColorSlider : public QSlider
{
public: 
	ScColorSlider(QWidget* parent = 0);
	
	virtual void paintEvent(QPaintEvent * p);
};

#endif
