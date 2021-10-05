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
public:
	ScImgDataLoader_PICT();

	bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) override;
	void loadEmbeddedProfile(const QString& fn, int page = 0) override;
	bool loadPicture(const QString& fn, int page, int res, bool thumbnail) override;

protected:
	void initSupportedFormatList();

private:
	void parseHeader(const QString& fName, double &x, double &y, double &w, double &h);
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
	void createTextPath(const QByteArray& textString);
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
	QByteArray decodeRLE(QByteArray &in, quint16 bytesPerLine, int multByte);
	QBrush setFillPattern();

	int m_baseX { 0 };
	int m_baseY { 0 };
	int m_docWidth { 0 };
	int m_docHeight { 0 };
	double m_resX { 0.0 };
	double m_resY { 0.0 };

	double m_lineW { 0.0 };
	QColor m_backColor;
	QColor m_foreColor;
	bool m_patternMode { false };
	QByteArray m_patternData;
	QRect m_currRect;
	QBrush m_currPatternBrush;
	QRect m_lastImageRect;
	QPoint m_ovalSize;
	QMap<int, QString> m_fontMap;
	int m_currentTextSize { 0 };
	int m_currentFontID { 0 };
	int m_currentFontStyle { 0 };
	QByteArray m_imageData;

	QPainterPath m_Coords;
	QPoint m_currentPoint;
	QPoint m_currentPointT;
	QPainter m_imagePainter;
	bool m_postscriptMode { false };
	bool m_textIsPostScript { false };
	int m_pctVersion { 0 };
	bool m_skipOpcode { false };
};

#endif
