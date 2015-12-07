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
	void parseHeader(QString fName, double &x, double &y, double &w, double &h);
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

	int m_baseX, m_baseY;
	int m_docWidth;
	int m_docHeight;
	double m_resX, m_resY;

	double m_LineW;
	QColor m_backColor;
	QColor m_foreColor;
	bool m_patternMode;
	QByteArray m_patternData;
	QRect m_currRect;
	QBrush m_currPatternBrush;
	QRect m_lastImageRect;
	QPoint m_ovalSize;
	QMap<int, QString> m_fontMap;
	int m_currentTextSize;
	int m_currentFontID;
	int m_currentFontStyle;
	QByteArray m_imageData;

	QPainterPath m_Coords;
	QPoint m_currentPoint;
	QPoint m_currentPointT;
	QPainter m_imagePainter;
	bool m_postscriptMode;
	bool m_textIsPostScript;
	int m_pctVersion;
	bool m_skipOpcode;
};

#endif
