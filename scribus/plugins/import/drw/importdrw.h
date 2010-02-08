/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          importdrw.h  -  description
                             -------------------
    begin                : Mon Jan 11 2010
    copyright            : (C) 2010 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef IMPORTDRW_H
#define IMPORTDRW_H

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

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

//! \brief Drw importer plugin
class DrwPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Cvg importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	DrwPlug( ScribusDoc* doc, int flags );
	~DrwPlug();

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
	bool convert(QString fn);
	void decodeCmdData(QDataStream &ts, uint dataLen, quint8 cmd);
	void decodeCmd(quint8 cmd, int pos);
	void decodeSymbol(QDataStream &ds, bool last = false);
	void handleLineStyle(PageItem* currentItem, quint8 flags, QString lineColor);
	void handleGradient(PageItem* currentItem, quint8 patternIndex, QString fillColor, QString backColor, QRectF bBox);
	void handlePreviewBitmap(QDataStream &ds);
	QString handleColor(ScColor &color, QString proposedName);
	void getCommonData(QDataStream &ds);
	QString getColor(QDataStream &ds);
	void finishItem(PageItem* ite, bool scale = true);
	double getValue(QDataStream &ds);
	double getRawValue(QDataStream &ds);
	QPointF getCoordinate(QDataStream &ds);
	
	QList<PageItem*> Elements;
	struct DRWGroup
	{
		double xoffset;
		double yoffset;
		double width;
		double height;
		double lineWidth;
		double scaleX;
		double scaleY;
		double rotationAngle;
		int nrOfItems;
		int counter;
		bool filled;
		quint8 patternIndex;
		quint8 flags;
		QString fillColor;
		QString lineColor;
		QString backColor;
		QPointF posPivot;
		QList<PageItem*> GElements;
	};
	QStack<DRWGroup> groupStack;
	struct DRWObjectList
	{
		double groupX;
		double groupY;
		double width;
		double height;
		double scaleX;
		double scaleY;
		double rotationAngle;
		QPointF posPivot;
		quint16 nrOfItems;
		quint16 counter;
		QString itemGroupName;
		PageItem* groupItem;
		QList<PageItem*> GElements;
	};
	QStack<DRWObjectList> listStack;
	struct DRWGradient
	{
		int type;
		double xOffset;
		double yOffset;
		double angle;
	};
	QMap<int, DRWGradient> gradientMap;
	QMap<int, QByteArray> patternDataMap;
	QMap<QString, QString> patternMap;
	double baseX, baseY;
	double docWidth;
	double docHeight;
	QStringList importedColors;
	QStringList importedPatterns;
	QString lineColor;
	QString fillColor;
	QString backColor;
	double lineWidth;
	int createObjCode;
	int nrOfPoints;
	PageItem *currentItem;
	quint8 flags;
	quint8 patternIndex;
	QRectF bBox;

	QImage tmpImage;
	QImage tmpImage2;
	quint16 bitsPerPixel;
	quint16 bytesScanline;
	quint16 planes;
	quint16 imageHeight;
	quint16 imageWidth;
	quint16 scanLinesRead;
	quint8 rTrans;
	quint8 gTrans;
	quint8 bTrans;
	bool imageValid;

	quint8 fontID;
	quint8 fontStyle;
	quint16 nrOfChars;
	quint16 fontSize;
	quint16 fontWidth;
	quint16 nrOfParagraphs;
	quint16 paragraphCounter;
	QString fontName;
	QString fontColor;
	struct DRWParagraph
	{
		quint8 paragraphAlignment;
		quint16 paragraphLen;
	};
	QList<DRWParagraph> paragraphList;
	QMap<quint8, QString> fontMap;

	int symbolCount;
	int recordCount;

	FPointArray Coords;
	QByteArray cmdData;
	double scaleFactor;
	double scaleX;
	double scaleY;
	double rotationAngle;
	QPointF posPivot;
	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	QString baseFile;
	QImage thumbnailImage;
	bool thumbRead;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
