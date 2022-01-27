/*
 * Copyright (C) 2011 Jain Basil Aliyas <jainbasil@gmail.com>
 *
 * For general Scribus (>=1.3.2) copyright and licensing information please refer
 * to the COPYING file provided with the program. Following this notice may exist
 * a copyright and/or license notice that predates the release of Scribus 1.3.2
 * for which a new license (GPL+exception) is in place.
*/

#ifndef API_COLOR_H_
#define API_COLOR_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"
#include "sccolor.h"

class ColorAPI : public QObject
{
    Q_OBJECT
	
    Q_PROPERTY(bool spotColor READ isSpotColor WRITE setSpotColor)
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    ColorAPI(ScColor *l, QString n);
    virtual ~ColorAPI();

public slots:
	bool isSpotColor();
	void setSpotColor(bool value);
	
	QString name();
	void setName(QString name);
	
	void replace(QString replace);
	void remove(QString replace);
	
	void changeCMYK(int c, int m, int y, int k);
	void changeRGB(int r, int g, int b);
	
private:
    ScColor *color;
	QString colorName;

};

#endif /*API_COLOR_H_*/
