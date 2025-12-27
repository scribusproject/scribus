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
		ScBitReader(const QByteArray &data);

		quint32 getUInt(uint size);
		void alignToWord();

	private:
		int actByte { 0 };
		int actBit { 7 };
		QByteArray buffer;
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
	~CgmPlug() override;

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
	void parseHeader(const QString& fName, double &b, double &h);
	bool convert(const QString& fn);
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
	ScColor getBinaryDirectColor(ScBitReader *breader) const;
	ScColor getBinaryDirectColor(QDataStream &ts) const;
	QString getBinaryIndexedColor(ScBitReader *breader);
	QString getBinaryIndexedColor(QDataStream &ts);
	QString getBinaryColor(QDataStream &ts);
	double  getBinaryDistance(QDataStream &ts) const;
	QPointF getBinaryCoords(QDataStream &ts, bool raw = false) const;
	uint    getBinaryUInt(QDataStream &ts, int intP) const;
	int     getBinaryInt(QDataStream &ts, int intP) const;
	double  getBinaryReal(QDataStream &ts, int realP, int realM) const;
	QString getBinaryText(QDataStream &ts) const;
	void    alignStreamToWord(QDataStream &ts, uint len) const;

/* core functions */
	void    handleStartMetaFile(const QString& value);
	void    handleStartPicture(const QString& value);
	void    handleStartPictureBody(double width, double height);
	void    handleMetaFileDescription(const QString& value);
	QString handleColor(const ScColor &color, const QString& proposedName);
	double  convertCoords(double input) const;
	QPointF convertCoords(const QPointF& input) const;
	void appendPath(QPainterPath &path1, const QPainterPath &path2) const;
	PageItem* itemAdd(PageItem::ItemType itemType, PageItem::ItemFrameType frameType, double x, double y, double b, double h, double w, const QString& fill, const QString& stroke);
	void    finishItem(PageItem* ite, bool line = true);

/* common variables */
	int metaFileVersion { 0 };
	int vdcType { 0 };
	int vdcInt { 0 };
	int vdcReal { 0 };
	int vdcMantissa { 0 };
	bool vcdFlippedH { false };
	bool vcdFlippedV { false };
	double vdcWidth { 0.0 };
	double vdcHeight { 0.0 };
	double metaFileScale { 1.0 };
	double metaScale { 1.0 };
	int metaFileScaleMode { 0 };
	int intPrecision { 0 };
	int realPrecision { 0 };
	int realMantissa { 0 };
	int realFraction { 0 };
	bool realPrecisionSet { false };
	int indexPrecision { 0 };
	int colorPrecision { 0 };
	int colorIndexPrecision { 0 };
	uint maxColorIndex { 0 };
	int m_colorModel { 0 };
	int colorMode { 0 };
	int namePrecision { 0 };
	int lineWidthMode { 0 };
	int edgeWidthMode { 0 };
	int markerSizeMode { 0 };
	double viewPortScale { 1.0 };
	int viewPortScaleMode { 0 };

	int lineBundleIndex { 0 };
	Qt::PenStyle lineType { Qt::NoPen };
	Qt::PenCapStyle lineCap { Qt::FlatCap };
	Qt::PenJoinStyle lineJoin { Qt::MiterJoin };
	double lineWidth { 0.0 };
	Qt::PenStyle edgeType { Qt::NoPen };
	Qt::PenCapStyle edgeCap { Qt::FlatCap };
	Qt::PenJoinStyle edgeJoin { Qt::MiterJoin };
	double edgeWidth { 0.0 };
	uint minColor { 0 };
	uint maxColor { 0 };
	QString lineColor;
	bool lineVisible { false };
	QString edgeColor;
	QString fillColor;
	int fillType { 0 };
	int patternIndex { 0 };
	QMap<int, QString> patternTable;
	double patternScaleX { 1.0 };
	double patternScaleY { 1.0 };
	QString backgroundColor;
	bool backgroundSet { false };
	QMap<uint, QString> ColorTableMap;
	QRectF clipRect;
	bool useClipRect { false };
	bool clipSet { false };
	QPainterPath regionPath;
	bool recordRegion { false };
	int currentRegion { 0 };
	QMap<int, QPainterPath> regionMap;
	QPointF fillRefPoint;

	QList<PageItem*> Elements;
	int currentItemNr { 0 };
	QStack<QList<PageItem*> > groupStack;
	ColorList CustColors;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 0.0 };
	double docHeight { 0.0 };
	QStringList importedColors;

	FPointArray Coords;
	bool interactive { false };
	MultiProgressDialog* progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };
	int oldDocItemCount { 0 };
	QString baseFile;
	bool importRunning { false };
	bool firstPage { false };
	bool vcdSet { false };
	bool wasEndPic { false };
	bool recordFigure { false };
	QPainterPath figurePath;
	bool figClose { false };
	int figDocIndex { 0 };
	int figElemIndex { 0 };
	int figGstIndex { 0 };
	QString figFillColor;
	QMap<int, QString> fontID_Map;
	int m_fontIndex { 0 };
	QString textColor;
	int textSize { 10 };
	int textAlignH { 0 };
	int textScaleMode { 0 };
	QString pictName;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
