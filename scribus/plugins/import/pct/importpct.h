/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTPCT_H
#define IMPORTPCT_H


#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QRect>

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

//! \brief Pct (Mac Pict) importer plugin
class PctPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Pct importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	PctPlug( ScribusDoc* doc, int flags );
	~PctPlug();

	/*!
	\author Franz Schmid
	\date
	\brief Perform import.
	\param fn QString
	\param trSettings undo transaction settings
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval bool true if import was ok
	 */
	bool import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);

private:
	void parseHeader(QString fName, double &x, double &y, double &b, double &h);
	bool convert(QString fn);
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
	void handleLongText(QDataStream &ts);
	void handleDHText(QDataStream &ts);
	void handleDVText(QDataStream &ts);
	void handleDHVText(QDataStream &ts);
	void createTextPath(QByteArray textString);
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
	void setFillPattern(PageItem* ite);
	void handleLineModeEnd();
	void finishItem(PageItem* ite);
	
	QList<PageItem*> m_Elements;
	int m_currentItemNr;
	QStack<QList<PageItem*> > m_groupStack;
	ColorList m_CustColors;
	double m_baseX, m_baseY;
	double m_offsetX, m_offsetY;
	double m_docWidth;
	double m_docHeight;
	double m_resX, m_resY;

	double m_LineW;
	QString m_CurrColorFill;
	QColor m_backColor;
	QString m_CurrColorStroke;
	QColor m_foreColor;
	double m_CurrStrokeShade;
	double m_CurrFillShade;
	bool m_patternMode;
	QByteArray m_patternData;
	QMap<QString, QString> m_patternMap;
	QRect m_currRect;
	int m_currRectItemNr;
	int m_currRectType;
	QRect m_lastImageRect;
	QStringList m_importedColors;
	QStringList m_importedPatterns;
	QPoint m_ovalSize;
	QMap<int, QString> m_fontMap;
	int m_currentTextSize;
	int m_currentFontID;
	int m_currentFontStyle;
	FPointArray m_lastCoords;
	QByteArray m_imageData;

	FPointArray m_Coords;
	QPoint m_currentPoint;
	QPoint m_currentPointT;
	bool m_lineMode;
	bool m_postscriptMode;
	bool m_textIsPostScript;
	bool m_interactive;
	MultiProgressDialog * m_progressDialog;
	bool m_cancel;
	ScribusDoc* m_Doc;
	Selection* m_tmpSel;
	int m_importerFlags;
	int m_oldDocItemCount;
	QString m_baseFile;
	int m_pctVersion;
	bool m_skipOpcode;

public slots:
	void cancelRequested() { m_cancel = true; }
};

#endif
