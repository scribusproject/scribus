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
	
	QList<PageItem*> Elements;
	int currentItemNr;
	QStack<QList<PageItem*> > groupStack;
	ColorList CustColors;
	double baseX, baseY;
	double docWidth;
	double docHeight;

	double LineW;
	QString CurrColorFill;
	QColor backColor;
	QString CurrColorStroke;
	QColor foreColor;
	double CurrStrokeShade;
	double CurrFillShade;
	bool patternMode;
	QByteArray patternData;
	QMap<QString, QString> patternMap;
	QRect currRect;
	int currRectItemNr;
	int currRectType;
	QRect lastImageRect;
	QStringList importedColors;
	QStringList importedPatterns;
	QPoint ovalSize;
	QMap<int, QString> fontMap;
	int currentTextSize;
	int currentFontID;
	int currentFontStyle;
	FPointArray lastCoords;
	QByteArray imageData;

	FPointArray Coords;
	QPoint currentPoint;
	QPoint currentPointT;
	bool lineMode;
	bool postscriptMode;
	bool textIsPostScript;
	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	int oldDocItemCount;
	QString baseFile;
	int pctVersion;
	bool skipOpcode;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
