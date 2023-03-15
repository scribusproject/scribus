/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTPCT_H
#define IMPORTPCT_H

#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QRect>

#include "pluginapi.h"
#include "fpointarray.h"
#include "pageitem.h"
#include "sccolor.h"

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
	bool import(const QString& fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(const QString& fn);

private:
	void parseHeader(const QString& fName, double &x, double &y, double &b, double &h);
	bool convert(const QString& fn);
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
	void createTextPath(const QByteArray& textString);
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
	int currentItemNr { 0 };
	QStack<QList<PageItem*> > groupStack;
	ColorList CustColors;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double offsetX { 0.0 };
	double offsetY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };
	double resX { 1.0 };
	double resY { 1.0 };

	double LineW { 1.0 };
	QString CurrColorFill { "White" };
	QColor backColor;
	QString CurrColorStroke { "Black" };
	QColor foreColor;
	double CurrStrokeShade { 100.0 };
	double CurrFillShade { 100.0 };
	bool patternMode { false };
	QByteArray patternData;
	QMap<QString, QString> patternMap;
	QRect currRect;
	int currRectItemNr { 0 };
	int currRectType { 0 };
	QRect lastImageRect;
	QStringList importedColors;
	QStringList importedPatterns;
	QPoint ovalSize;
	QMap<int, QString> fontMap;
	int currentTextSize { 12 };
	int currentFontID { 0 };
	int currentFontStyle { 0 };
	FPointArray lastCoords;
	QByteArray imageData;

	FPointArray Coords;
	QPoint currentPoint;
	QPoint currentPointT;
	bool lineMode { false };
	bool postscriptMode { false };
	bool textIsPostScript { false };
	bool interactive { false };
	MultiProgressDialog * progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };
	QString baseFile;
	int pctVersion { 0 };
	bool skipOpcode { false };

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
