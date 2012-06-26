/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_IMAGEITEM_H_
#define API_IMAGEITEM_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"
#include "api_item.h"
#include "pageitem_imageframe.h"

class ImageAPI : public ItemAPI
{
	Q_OBJECT
    Q_PROPERTY(double xScale READ xScale WRITE setXScale)
    Q_PROPERTY(double yScale READ yScale WRITE setYScale)
    Q_PROPERTY(double xOffset READ xOffset WRITE setXOffset)
    Q_PROPERTY(double yOffset READ yOffset WRITE setYOffset)

public:
    ImageAPI(PageItem_ImageFrame* im);
    virtual ~ImageAPI();

public slots:
    void load(QString filename);
    void scale(double x, double y);
    void offset(double x, double y);
    void setBrightness(double n);
    void setGrayscale();
    void scaleToFrame(bool scaletoframe, bool Proportional);

private:
	PageItem_ImageFrame* item;
	
    double xScale();
    void setXScale(double value);
	
    double yScale();
    void setYScale(double value);
	
	
    double xOffset();
    void setXOffset(double value);
	
	
    double yOffset();
    void setYOffset(double value);
};

#endif /*API_IMAGEITEM_H_*/
