/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_WPG_H
#define SCIMGDATALOADER_WPG_H

#include "scimgdataloader.h"

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QLinearGradient>

#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QPainter>
#include <QPainterPath>
#include "third_party/wpg/libwpg.h"


class ScrPainterIm : public libwpg::WPGPaintInterface
{
public:
	ScrPainterIm();

	void startGraphics(double imageWidth, double imageHeight);
	void endGraphics();
	void startLayer(unsigned int id);
	void endLayer(unsigned int id);
	void setPen(const libwpg::WPGPen& pen);
	void setBrush(const libwpg::WPGBrush& brush);
	void setFillRule(FillRule rule);
	void drawRectangle(const libwpg::WPGRect& rect, double rx, double ry);
	void drawEllipse(const libwpg::WPGPoint& center, double rx, double ry);
	void drawPolygon(const libwpg::WPGPointArray& vertices, bool closed);
	void drawPath(const libwpg::WPGPath& path);
	void drawBitmap(const libwpg::WPGBitmap& bitmap, double hres, double vres);
	void drawImageObject(const libwpg::WPGBinaryData& binaryData);

	QPainterPath Coords;
	QPainter imagePainter;
	QPen currentPen;
	QBrush currentBrush;
	bool fillrule;
	double gradientAngle;
	bool isGradient;
	bool fillSet;
	bool strokeSet;
	QLinearGradient currentGradient;
	QImage *image;
};

class ScImgDataLoader_WPG : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();

public:
	ScImgDataLoader_WPG(void);

	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0);
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail);
};
#endif
