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
	DrwPlug( ScribusDoc* doc, int m_flags );
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
	bool import(QString fn, const TransactionSettings& trSettings, int m_flags, bool showProgress = true);
	QImage readThumbnail(QString fn);

private:
	bool convert(QString fn);
	void decodeCmdData(QDataStream &ts, uint dataLen, quint8 cmd);
	void decodeCmd(quint8 cmd, int pos);
	void decodeSymbol(QDataStream &ds, bool last = false);
	void handleLineStyle(PageItem* m_currentItem, quint8 m_flags, QString m_lineColor);
	void handleGradient(PageItem* m_currentItem, quint8 m_patternIndex, QString m_fillColor, QString m_backColor, QRectF m_bBox);
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
	QStack<DRWGroup> m_groupStack;
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
	QStack<DRWObjectList> m_listStack;
	struct DRWGradient
	{
		int type;
		double xOffset;
		double yOffset;
		double angle;
	};
	QMap<int, DRWGradient> m_gradientMap;
	QMap<int, QByteArray> m_patternDataMap;
	QMap<QString, QString> m_patternMap;
	double m_baseX, m_baseY;
	double m_docWidth;
	double m_docHeight;
	QStringList m_importedColors;
	QStringList m_importedPatterns;
	QString m_lineColor;
	QString m_fillColor;
	QString m_backColor;
	double m_lineWidth;
	int m_createObjCode;
	int m_nrOfPoints;
	PageItem *m_currentItem;
	quint8 m_flags;
	quint8 m_patternIndex;
	QRectF m_bBox;

	QImage m_tmpImage;
	QImage m_tmpImage2;
	quint16 m_bitsPerPixel;
	quint16 m_bytesScanline;
	quint16 m_planes;
	quint16 m_imageHeight;
	quint16 m_imageWidth;
	quint16 m_scanLinesRead;
	quint8 m_rTrans;
	quint8 m_gTrans;
	quint8 m_bTrans;
	bool m_imageValid;

	quint8 m_fontID;
	quint8 m_fontStyle;
	quint16 m_nrOfChars;
	quint16 m_fontSize;
	quint16 m_fontWidth;
	quint16 m_nrOfParagraphs;
	quint16 m_paragraphCounter;
	QString m_fontName;
	QString m_fontColor;
	struct DRWParagraph
	{
		quint8 paragraphAlignment;
		quint16 paragraphLen;
	};
	QList<DRWParagraph> m_paragraphList;
	QMap<quint8, QString> m_fontMap;

	int m_symbolCount;
	int m_recordCount;

	FPointArray m_Coords;
	QByteArray m_cmdData;
	double m_scaleFactor;
	double m_scaleX;
	double m_scaleY;
	double m_rotationAngle;
	QPointF m_posPivot;
	bool m_interactive;
	MultiProgressDialog * m_progressDialog;
	bool m_cancel;
	ScribusDoc* m_Doc;
	Selection* m_tmpSel;
	int m_importerFlags;
	QString m_baseFile;
	QImage m_thumbnailImage;
	bool m_thumbRead;

public slots:
	void cancelRequested() { m_cancel = true; }
};

#endif
