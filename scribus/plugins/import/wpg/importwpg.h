/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTWPG_H
#define IMPORTWPG_H

#include "qglobal.h"
#include "qobject.h"
#include "qstring.h"

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include "vgradient.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QVector>
#include "third_party/wpg/libwpg.h"

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

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
	QList<PageItem*> m_Elements;
	QStringList m_importedColors;
	ScribusDoc* m_Doc;
	double m_LineW;
	QString m_CurrColorFill;
	QString m_CurrColorStroke;
	double m_CurrStrokeShade;
	double m_CurrFillShade;
	double m_CurrStrokeTrans;
	double m_CurrFillTrans;
	FPointArray m_Coords;
	double m_baseX, m_baseY;
	bool m_fillrule;
	double m_gradientAngle;
	bool m_isGradient;
	bool m_fillSet;
	bool m_strokeSet;
	VGradient m_currentGradient;
	QVector<double> m_dashArray;
	Qt::PenJoinStyle m_lineJoin;
	Qt::PenCapStyle m_lineEnd;
	int m_flags;
	bool m_firstLayer;
};

//! \brief WPG importer plugin
class WpgPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Wpg importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	WpgPlug( ScribusDoc* doc, int flags );
	~WpgPlug();

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
