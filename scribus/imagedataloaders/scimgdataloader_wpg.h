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

	void startGraphics(double imageWidth, double imageHeight) override;
	void endGraphics() override;
	void startLayer(unsigned int id) override;
	void endLayer(unsigned int id) override;
	void setPen(const libwpg::WPGPen& pen) override;
	void setBrush(const libwpg::WPGBrush& brush) override;
	void setFillRule(FillRule rule) override;
	void drawRectangle(const libwpg::WPGRect& rect, double rx, double ry) override;
	void drawEllipse(const libwpg::WPGPoint& center, double rx, double ry) override;
	void drawPolygon(const libwpg::WPGPointArray& vertices, bool closed) override;
	void drawPath(const libwpg::WPGPath& path) override;
	void drawBitmap(const libwpg::WPGBitmap& bitmap, double hres, double vres) override;
	void drawImageObject(const libwpg::WPGBinaryData& binaryData) override;

	QPainterPath Coords;
	QPainter imagePainter;
	QPen currentPen;
	QBrush currentBrush;
	bool fillrule { false };
	double gradientAngle { 0.0 };
	bool isGradient { false };
	bool fillSet { false };
	bool strokeSet { false };
	QLinearGradient currentGradient;
	QImage* image { nullptr };
};

class ScImgDataLoader_WPG : public ScImgDataLoader
{
public:
	ScImgDataLoader_WPG();

	bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) override;
	void loadEmbeddedProfile(const QString& fn, int page = 0) override;
	bool loadPicture(const QString& fn, int page, int res, bool thumbnail) override;

protected:
	void initSupportedFormatList();
};

#endif
