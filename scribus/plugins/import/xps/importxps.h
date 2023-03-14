/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Oct 26 2013
	copyright            : (C) 2013 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#ifndef IMPORTXPS_H
#define IMPORTXPS_H

#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

#include "commonstrings.h"
#include "fpointarray.h"
#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;
class ScZipHandler;

//! \brief Xps importer plugin
class XpsPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Xps importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	XpsPlug( ScribusDoc* doc, int flags );
	~XpsPlug();

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
	struct ObjState
	{
		QPainterPath currentPath;
		bool currentPathClosed { false };
		QPainterPath clipPath;
		QTransform transform;
		QString CurrColorFill { CommonStrings::None };
		double fillOpacity { 0.0 };
		QString CurrColorStroke { CommonStrings::None };
		double strokeOpacity { 0.0 };
		double LineW { 1.0 };
		VGradient currentGradient;
		QPointF gradientStart;
		QPointF gradientEnd;
		QPointF gradientFocus;
		double gradientScale { 1.0 };
		int fillGradientTyp { 0 };
		QString imagePath;
		QString patternName;
		VGradient gradientMask;
		QPointF maskStart;
		QPointF maskEnd;
		QPointF maskFocus;
		double maskScale { 1.0 };
		int maskTyp { 0 };
		QString patternMask;
		VGradient gradientStroke;
		QPointF strokeStart;
		QPointF strokeEnd;
		QPointF strokeFocus;
		double strokeScale { 1.0 };
		int strokeTyp { 0 };
		QString patternStroke;
		int itemType { 0 };
		Qt::PenCapStyle CapStyle { Qt::FlatCap };
		Qt::PenJoinStyle JoinStyle { Qt::MiterJoin };
		QVector<double> DashPattern;
		double DashOffset { 0.0 };
	};

	bool convert(const QString& fn);
	bool parseDocSequence(const QString& designMap);
	bool parseDocReference(const QString& designMap);
	void parsePageReference(const QString& designMap);
	PageItem* parseObjectXML(QDomElement& dpg, const QString& path);
	void parseOpacityXML(QDomElement &spe, const QString& path, ObjState &obState);
	void parseStrokeXML(QDomElement &spe, const QString& path, ObjState &obState);
	void parseFillXML(QDomElement &spe, const QString& path, ObjState &obState);
	void parsePathDataXML(QDomElement &spe, ObjState &obState, bool forClip = false);
	QString parsePathGeometryXML(QDomElement &spe);
	void parseResourceFile(const QString& resFile);
	void resolveLinks();
	PageItem* addClip(PageItem* retObj, ObjState &obState);
	PageItem* createItem(QDomElement &dpg, ObjState &obState);
	void finishItem(PageItem* item, ObjState &obState);
	QString handleColor(QString rgbColor, double &opacity);
	int hex2int(char hex);
	bool parseGUID( const QString &guidString, unsigned short guid[16]);
	ScFace loadFontByName(const QString &fileName);

	QList<PageItem*> Elements;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };

	FPointArray Coords;
	bool interactive { false };
	MultiProgressDialog * progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };
	QString baseFile;
	QString m_FileName;
	QStringList importedColors;
	QStringList importedPatterns;
	bool firstPage { true };
	int pagecount { 0 };
	double topMargin { 0.0 };
	double leftMargin { 0.0 };
	double rightMargin { 0.0 };
	double bottomMargin { 0.0 };
	double conversionFactor { 1.0 };
	QHash<QString, QPainterPath> pathResources;
	QHash<QString, PageItem*> linkTargets;
	QHash<PageItem*, QString> linkSources;
	QHash<QString, ScFace> loadedFonts;
	ScZipHandler *uz { nullptr };
	QStringList tempFontFiles;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
