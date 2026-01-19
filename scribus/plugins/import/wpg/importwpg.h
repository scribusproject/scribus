/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTWPG_H
#define IMPORTWPG_H

#include <QList>
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QTransform>
#include <QVector>

#include "fpointarray.h"
#include "pageitem.h"
#include "pluginapi.h"
#include "sccolor.h"
#include "third_party/wpg/libwpg.h"
#include "vgradient.h"

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

class ScrPainter : public libwpg::WPGPaintInterface
{
public:
	ScrPainter();

	void startGraphics(double imageWidth, double imageHeight) override;
	void endGraphics() override;
	void startLayer(unsigned int id) override;
	void endLayer(unsigned int id) override;
	void setPen(const libwpg::WPGPen& pen) override;
	void setBrush(const libwpg::WPGBrush& brush) override;
	void setFillRule(FillRule rule) override;
	void drawRectangle(const libwpg::WPGRect& rect, double rx, double ry) override;
	void drawEllipse(const libwpg::WPGPoint& center, double rx, double ry) override;
	void drawPolygon(const libwpg::WPGPointArray& vertices, bool closed) override;
	void drawPath(const libwpg::WPGPath& path) override;
	void drawBitmap(const libwpg::WPGBitmap& bitmap, double hres, double vres) override;
	void drawImageObject(const libwpg::WPGBinaryData& binaryData) override;

	void finishItem(PageItem* ite);

	QList<PageItem*> Elements;
	QStringList importedColors;
	ScribusDoc* m_Doc { nullptr };
	double LineW { 1.0 };
	QString CurrColorFill { "Black" };
	QString CurrColorStroke { "Black" };
	double CurrStrokeShade { 100.0 };
	double CurrFillShade { 100.0 };
	double CurrStrokeTrans { 0.0 };
	double CurrFillTrans { 0.0 };
	FPointArray Coords;
	double baseX { 0.0 };
	double baseY { 0.0 };
	bool fillrule { true };
	double gradientAngle { 0.0 };
	bool isGradient { false };
	bool fillSet { false };
	bool strokeSet { false };
	VGradient currentGradient;
	QVector<double> dashArray;
	Qt::PenJoinStyle lineJoin { Qt::MiterJoin };
	Qt::PenCapStyle lineEnd { Qt::FlatCap };
	int flags { 0 };
	bool firstLayer { true };
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
	WpgPlug(ScribusDoc* doc, int flags);
	~WpgPlug() override;

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
	
	QList<PageItem*> Elements;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };

	QStringList importedColors;

	bool interactive { false };
	MultiProgressDialog* progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
