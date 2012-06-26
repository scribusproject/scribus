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
	Q_PROPERTY(double imageXScale READ imageXScale WRITE setImageXScale)
	Q_PROPERTY(double imageYScale READ imageYScale WRITE setImageYScale)
	Q_PROPERTY(double imageXOffset READ imageXOffset WRITE setImageXOffset)
	Q_PROPERTY(double imageYOffset READ imageYOffset WRITE setImageYOffset)
public:
    ImageAPI(PageItem_ImageFrame* im);
    virtual ~ImageAPI();

public slots:
	void loadImage(QString filename);
	void scaleImage(double x, double y);
	void setImageOffset(double x, double y);
	void setImageBrightness(double n);
	void setImageGrayscale();
	void setScaleImageToFrame(bool scaletoframe, bool Proportional);

private:
	PageItem_ImageFrame* item;
	
	double imageXScale();
	void setImageXScale(double value);
	
	double imageYScale();
	void setImageYScale(double value);
	
	
	double imageXOffset();
	void setImageXOffset(double value);
	
	
	double imageYOffset();
	void setImageYOffset(double value);
};

#endif /*API_IMAGEITEM_H_*/
