/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTWPG_H
#define IMPORTWPG_H

#include <QObject>
#include <QString>

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include "vgradient.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QVector>

#include <libmspub/libmspub.h>
#include <libwpd-stream/libwpd-stream.h>
#include <libwpd/libwpd.h>
#include <libwpg/libwpg.h>

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

class RawPainter : public libwpg::WPGPaintInterface
{
public:
	RawPainter();
	void startGraphics(const ::WPXPropertyList &propList);
	void endGraphics();
	void startLayer(const ::WPXPropertyList &propList);
	void endLayer();
	void startEmbeddedGraphics(const ::WPXPropertyList &propList);
	void endEmbeddedGraphics();
	void setStyle(const ::WPXPropertyList &propList, const ::WPXPropertyListVector &gradient);
	void drawRectangle(const ::WPXPropertyList &propList);
	void drawEllipse(const ::WPXPropertyList &propList);
	void drawPolyline(const ::WPXPropertyListVector &vertices);
	void drawPolygon(const ::WPXPropertyListVector &vertices);
	void drawPath(const ::WPXPropertyListVector &path);
	void drawGraphicObject(const ::WPXPropertyList &propList, const ::WPXBinaryData &binaryData);
	void startTextObject(const ::WPXPropertyList &propList, const ::WPXPropertyListVector &path);
	void endTextObject();
	void startTextLine(const ::WPXPropertyList &propList);
	void endTextLine();
	void startTextSpan(const ::WPXPropertyList &propList);
	void endTextSpan();
	void insertText(const ::WPXString &str);
	QString constructFontName(QString fontBaseName, QString fontStyle);
	double fromPercentage(const QString &s );
	QColor  parseColorN( const QString &rgbColor );
	QString parseColor( const QString &s );
	void applyFill(PageItem* ite);
	void finishItem(PageItem* ite);

	ScribusDoc* m_Doc;
	Selection* tmpSel;
	QList<PageItem*> *Elements;
	struct groupEntry
	{
		QList<PageItem*> Items;
		FPointArray clip;
	};
	QStack<groupEntry> groupStack;
	QStringList *importedColors;
	QStringList *importedPatterns;
	double LineW;
	QString CurrColorFill;
	QString CurrColorStroke;
	double CurrStrokeShade;
	double CurrFillShade;
	double CurrStrokeTrans;
	double CurrFillTrans;
	FPointArray Coords;
	bool fillrule;
	double gradientAngle;
	bool isGradient;
	VGradient currentGradient;
	QString gradColor1Str;
	QColor gradColor1;
	double gradColor1Trans;
	QString gradColor2Str;
	QColor gradColor2;
	double gradColor2Trans;
	QVector<double> dashArray;
	Qt::PenJoinStyle lineJoin;
	Qt::PenCapStyle lineEnd;
	double baseX, baseY;
	double docWidth;
	double docHeight;
	int importerFlags;
	bool firstPage;
	QString baseLayer;
	int actPage;
	WPXPropertyList m_style;
	PageItem *actTextItem;
	ParagraphStyle textStyle;
	CharStyle textCharStyle;
	double m_linespace;
	double m_maxFontSize;
	bool lineSpSet;
	bool lineSpIsPT;
};
/*
class ScrPainter : public libwpg::WPGPaintInterface
{
public:
	ScrPainter();

	void startGraphics(double imageWidth, double imageHeight);
	void endGraphics();
	void startLayer(unsigned int id);
	void endLayer(unsigned int id);
	void setPen(const libwpg::WPGPen& pen);
	void setBrush(const libwpg::WPGBrush& brush);
	void setFillRule(FillRule rule);
	void drawRectangle(const libwpg::WPGRect& rect, double rx, double ry);
	void drawEllipse(const libwpg::WPGPoint& center, double rx, double ry);
	void drawPolygon(const libwpg::WPGPointArray& vertices, bool closed);
	void drawPath(const libwpg::WPGPath& path);
	void drawBitmap(const libwpg::WPGBitmap& bitmap, double hres, double vres);
	void drawImageObject(const libwpg::WPGBinaryData& binaryData);

	void finishItem(PageItem* ite);
	QList<PageItem*> Elements;
	QStringList importedColors;
	ScribusDoc* m_Doc;
	double LineW;
	QString CurrColorFill;
	QString CurrColorStroke;
	double CurrStrokeShade;
	double CurrFillShade;
	double CurrStrokeTrans;
	double CurrFillTrans;
	FPointArray Coords;
	double baseX, baseY;
	bool fillrule;
	double gradientAngle;
	bool isGradient;
	bool fillSet;
	bool strokeSet;
	VGradient currentGradient;
	QVector<double> dashArray;
	Qt::PenJoinStyle lineJoin;
	Qt::PenCapStyle lineEnd;
	int flags;
	bool firstLayer;
};
*/
//! \brief PUB importer plugin
class PubPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Pub importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	PubPlug( ScribusDoc* doc, int flags );
	~PubPlug();

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
	
	QList<PageItem*> Elements;
	double baseX, baseY;
	double docWidth;
	double docHeight;

	QStringList importedColors;
	QStringList importedPatterns;

	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
