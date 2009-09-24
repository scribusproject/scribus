/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_PICT_H
#define SCIMGDATALOADER_PICT_H

#include "scimgdataloader.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QRect>
#include <QPainter>
#include <QPainterPath>

class ScImgDataLoader_PICT : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();

public:
	ScImgDataLoader_PICT(void);

	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0);
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail);

private:
	void parsePict(QDataStream &ts);
	void alignStreamToWord(QDataStream &ts, uint len);
	void handleColor(QDataStream &ts, bool back);
	void handleColorRGB(QDataStream &ts, bool back);
	void handlePenPattern(QDataStream &ts);
	void handlePolygon(QDataStream &ts, quint16 opCode);
	void handleShape(QDataStream &ts, quint16 opCode);
	void handleSameShape(QDataStream &ts, quint16 opCode);
	void handleFontName(QDataStream &ts);
	void handleTextSize(QDataStream &ts);
	void handleTextFont(QDataStream &ts);
	void handleTextStyle(QDataStream &ts);
	void handleTextMode(QDataStream &ts);
	void handleLongText(QDataStream &ts);
	void handleDHText(QDataStream &ts);
	void handleDVText(QDataStream &ts);
	void handleDHVText(QDataStream &ts);
	void createTextPath(QByteArray textString);
	void handlePenMode(QDataStream &ts);
	void handlePenSize(QDataStream &ts);
	void handleOvalSize(QDataStream &ts);
	void handleShortLine(QDataStream &ts);
	void handleShortLineFrom(QDataStream &ts);
	void handleLine(QDataStream &ts);
	void handleLineFrom(QDataStream &ts);
	void handlePixmap(QDataStream &ts, quint16 opCode);
	void handleQuickTime(QDataStream &ts, quint16 opCode);
	void handleComment(QDataStream &ts, bool longComment);
	QRect readRect(QDataStream &ts);
	QByteArray decodeRLE(QByteArray &in, quint16 bytesPerLine, int twoByte);
	QBrush setFillPattern();

	int baseX, baseY;
	int docWidth;
	int docHeight;

	double LineW;
	QColor backColor;
	QColor foreColor;
	bool patternMode;
	QByteArray patternData;
	QRect currRect;
	QBrush currPatternBrush;
	int currRectItemNr;
	int currRectType;
	QRect lastImageRect;
	QPoint ovalSize;
	QMap<int, QString> fontMap;
	int currentTextSize;
	int currentFontID;
	int currentFontStyle;
	QByteArray imageData;

	QPainterPath Coords;
	QPoint currentPoint;
	QPoint currentPointT;
	QPainter imagePainter;
	bool postscriptMode;
	bool textIsPostScript;
	int pctVersion;
	bool skipOpcode;
};

#endif
