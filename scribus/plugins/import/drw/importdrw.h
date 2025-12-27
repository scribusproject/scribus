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
	~DrwPlug() override;

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
	bool importFile(const QString& fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(const QString& fn);

private:
	bool convert(const QString& fn);
	void decodeCmdData(QDataStream &ts, uint dataLen, quint8 cmd);
	void decodeCmd(quint8 cmd, int pos);
	void decodeSymbol(QDataStream &ds, bool last = false);
	void handleLineStyle(PageItem* currentItem, quint8 flags, const QString& lineColor) const;
	void handleGradient(PageItem* currentItem, quint8 patternIndex, const QString& fillColor, const QString& backColor, const QRectF& bBox);
	void handlePreviewBitmap(QDataStream &ds);
	QString handleColor(const ScColor &color, const QString& proposedName);
	void getCommonData(QDataStream &ds);
	QString getColor(QDataStream &ds);
	void finishItem(PageItem* ite, bool scale = true);
	double getValue(QDataStream &ds) const;
	double getRawValue(QDataStream &ds) const;
	QPointF getCoordinate(QDataStream &ds) const;
	
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
		int type { 0 };
		double xOffset { 0.0 };
		double yOffset { 0.0 };
		double angle { 0.0 };
	};
	QMap<int, DRWGradient> gradientMap;
	QMap<int, QByteArray> patternDataMap;
	QMap<QString, QString> patternMap;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 0.0 };
	double docHeight { 0.0 };
	QStringList importedColors;
	QStringList importedPatterns;
	QString lineColor;
	QString fillColor;
	QString backColor;
	double lineWidth { 0.0 };
	int createObjCode { 0 };
	int nrOfPoints { 0 };
	PageItem *currentItem { nullptr };
	quint8 flags { 0 };
	quint8 patternIndex { 0 };
	QRectF bBox;

	QImage tmpImage;
	QImage tmpImage2;
	quint16 bitsPerPixel { 8 };
	quint16 bytesScanline { 0 };
	quint16 planes { 0 };
	quint16 imageHeight { 0 };
	quint16 imageWidth { 0 };
	quint16 scanLinesRead { 0 };
	quint8 rTrans { 0 };
	quint8 gTrans { 0 };
	quint8 bTrans { 0 };
	bool imageValid { false };

	quint8 fontID { 0 };
	quint8 fontStyle { 0 };
	quint16 nrOfChars { 0 };
	quint16 fontSize { 10 };
	quint16 fontWidth { 0 };
	quint16 nrOfParagraphs { 0 };
	quint16 paragraphCounter { 0 };
	QString fontName;
	QString fontColor;
	struct DRWParagraph
	{
		quint8 paragraphAlignment { 0 };
		quint16 paragraphLen { 0 };
	};
	QList<DRWParagraph> paragraphList;
	QMap<quint8, QString> fontMap;

	int symbolCount { 0 };
	int recordCount { 0 };

	FPointArray Coords;
	QByteArray cmdData;
	double scaleFactor { 1.0 };
	double scaleX { 1.0 };
	double scaleY { 1.0 };
	double rotationAngle { 0 };
	QPointF posPivot;
	bool interactive { false };
	MultiProgressDialog* progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };
	QString baseFile;
	QImage thumbnailImage;
	bool thumbRead { false };

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
