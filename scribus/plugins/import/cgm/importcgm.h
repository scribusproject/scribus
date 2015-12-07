/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          importcgm.h  -  description
                             -------------------
    begin                : Wed Dez 23 2009
    copyright            : (C) 2009 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#ifndef IMPORTCGM_H
#define IMPORTCGM_H

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

class ScBitReader
{
	public:
		ScBitReader(QByteArray &data);
		~ScBitReader();
		quint32 getUInt(uint size);
		void alignToWord();
	private:
		int m_actByte;
		int m_actBit;
		QByteArray m_buffer;
};

//! \brief Cgm importer plugin
class CgmPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Cgm importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	CgmPlug( ScribusDoc* doc, int flags );
	~CgmPlug();

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
	void parseHeader(QString fName, double &b, double &h);
	bool convert(QString fn);
	void decodeText(QFile &f);

/* binary Decoder */
	void    decodeBinary(QDataStream &ts, quint16 elemClass, quint16 elemID, quint16 paramLen);
	void    decodeClass0(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    decodeClass1(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    decodeClass2(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    decodeClass3(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    decodeClass4(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    decodeClass5(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    decodeClass6(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    decodeClass7(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    decodeClass8(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    decodeClass9(QDataStream &ts, quint16 elemID, quint16 paramLen);
	void    getBinaryBezierPath(QDataStream &ts, quint16 paramLen);
	void    getBinaryPath(QDataStream &ts, quint16 paramLen, bool disjoint = false);
	void    getBinaryColorTable(QDataStream &ts, quint16 paramLen);
	ScColor getBinaryDirectColor(ScBitReader *breader);
	ScColor getBinaryDirectColor(QDataStream &ts);
	QString getBinaryIndexedColor(ScBitReader *breader);
	QString getBinaryIndexedColor(QDataStream &ts);
	QString getBinaryColor(QDataStream &ts);
	double  getBinaryDistance(QDataStream &ts);
	QPointF getBinaryCoords(QDataStream &ts, bool raw = false);
	uint    getBinaryUInt(QDataStream &ts, int intP);
	int     getBinaryInt(QDataStream &ts, int intP);
	double  getBinaryReal(QDataStream &ts, int realP, int realM);
	QString getBinaryText(QDataStream &ts);
	void    alignStreamToWord(QDataStream &ts, uint len);

/* core functions */
	void    handleStartMetaFile(QString value);
	void    handleStartPicture(QString value);
	void    handleStartPictureBody(double width, double height);
	void    handleMetaFileDescription(QString value);
	QString handleColor(ScColor &color, QString proposedName);
	double  convertCoords(double input);
	QPointF convertCoords(QPointF input);
	void appendPath(QPainterPath &path1, QPainterPath &path2);
	PageItem* itemAdd(PageItem::ItemType itemType, PageItem::ItemFrameType frameType, double x, double y, double b, double h, double w, QString fill, QString stroke);
	void    finishItem(PageItem* ite, bool line = true);

/* common variables */
	int m_metaFileVersion;
	int m_vdcType;
	int m_vdcInt;
	int m_vdcReal;
	int m_vdcMantissa;
	bool m_vcdFlippedH;
	bool m_vcdFlippedV;
	double m_vdcWidth;
	double m_vdcHeight;
	double m_metaFileScale;
	double m_metaScale;
	int m_metaFileScaleMode;
	int m_intPrecision;
	int m_realPrecision;
	int m_realMantissa;
	int m_realFraction;
	bool m_realPrecisionSet;
	int m_indexPrecision;
	int m_colorPrecision;
	int m_colorIndexPrecision;
	uint m_maxColorIndex;
	int m_colorModel;
	int m_colorMode;
	int m_namePrecision;
	int m_lineWidthMode;
	int m_edgeWidthMode;
	int m_markerSizeMode;
	double m_viewPortScale;
	int m_viewPortScaleMode;

	int m_lineBundleIndex;
	Qt::PenStyle m_lineType;
	Qt::PenCapStyle m_lineCap;
	Qt::PenJoinStyle m_lineJoin;
	double m_lineWidth;
	Qt::PenStyle m_edgeType;
	Qt::PenCapStyle m_edgeCap;
	Qt::PenJoinStyle m_edgeJoin;
	double m_edgeWidth;
	uint m_minColor, m_maxColor;
	QString m_lineColor;
	bool m_lineVisible;
	QString m_edgeColor;
	QString m_fillColor;
	int m_fillType;
	int m_patternIndex;
	QMap<int, QString> m_patternTable;
	double m_patternScaleX;
	double m_patternScaleY;
	QString m_backgroundColor;
	bool m_backgroundSet;
	QMap<uint, QString> m_ColorTableMap;
	QRectF m_clipRect;
	bool m_useClipRect;
	bool m_clipSet;
	QPainterPath m_regionPath;
	bool m_recordRegion;
	int m_currentRegion;
	QMap<int, QPainterPath> m_regionMap;
	QPointF m_fillRefPoint;

	QList<PageItem*> m_Elements;
	int m_currentItemNr;
	QStack<QList<PageItem*> > m_groupStack;
	ColorList m_CustColors;
	double m_baseX, m_baseY;
	double m_docWidth;
	double m_docHeight;
	QStringList m_importedColors;

	FPointArray m_Coords;
	bool m_interactive;
	MultiProgressDialog * m_progressDialog;
	bool m_cancel;
	ScribusDoc* m_Doc;
	Selection* m_tmpSel;
	int m_importerFlags;
	int m_oldDocItemCount;
	QString m_baseFile;
	bool m_importRunning;
	bool m_firstPage;
	bool m_vcdSet;
	bool m_wasEndPic;
	bool m_recordFigure;
	QPainterPath m_figurePath;
	bool m_figClose;
	int m_figDocIndex;
	int m_figElemIndex;
	int m_figGstIndex;
	QString m_figFillColor;
	QMap<int, QString> m_fontID_Map;
	int m_fontIndex;
	QString m_textColor;
	int m_textSize;
	int m_textAlignH;
	int m_textScaleMode;
	QString m_pictName;

public slots:
	void cancelRequested() { m_cancel = true; }
};

#endif
