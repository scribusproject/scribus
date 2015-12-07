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
#include <QDomDocument>
#include <QDomElement>

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
	bool import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);

private:
	struct ObjState
	{
		QPainterPath currentPath;
		bool currentPathClosed;
		QPainterPath clipPath;
		QTransform transform;
		QString CurrColorFill;
		double fillOpacity;
		QString CurrColorStroke;
		double strokeOpacity;
		double LineW;
		VGradient currentGradient;
		QPointF gradientStart;
		QPointF gradientEnd;
		QPointF gradientFocus;
		double gradientScale;
		int fillGradientTyp;
		QString imagePath;
		QString patternName;
		VGradient gradientMask;
		QPointF maskStart;
		QPointF maskEnd;
		QPointF maskFocus;
		double maskScale;
		int maskTyp;
		QString patternMask;
		VGradient gradientStroke;
		QPointF strokeStart;
		QPointF strokeEnd;
		QPointF strokeFocus;
		double strokeScale;
		int strokeTyp;
		QString patternStroke;
		int itemType;
		Qt::PenCapStyle CapStyle;
		Qt::PenJoinStyle JoinStyle;
		QVector<double> DashPattern;
		double DashOffset;
	};
	bool convert(QString fn);
	bool parseDocSequence(QString designMap);
	bool parseDocReference(QString designMap);
	void parsePageReference(QString designMap);
	PageItem* parseObjectXML(QDomElement& dpg, QString path);
	void parseOpacityXML(QDomElement &spe, QString path, ObjState &obState);
	void parseStrokeXML(QDomElement &spe, QString path, ObjState &obState);
	void parseFillXML(QDomElement &spe, QString path, ObjState &obState);
	void parsePathDataXML(QDomElement &spe, ObjState &obState, bool forClip = false);
	QString parsePathGeometryXML(QDomElement &spe);
	void parseResourceFile(QString resFile);
	void resolveLinks();
	PageItem* addClip(PageItem* retObj, ObjState &obState);
	PageItem* createItem(QDomElement &dpg, ObjState &obState);
	void finishItem(PageItem* item, ObjState &obState);
	QString handleColor(QString rgbColor, double &opacity);
	int hex2int(char hex);
	bool parseGUID( const QString &guidString, unsigned short guid[16]);
	ScFace loadFontByName(const QString &fileName);
	QList<PageItem*> m_Elements;
	double m_baseX, m_baseY;
	double m_docWidth;
	double m_docHeight;

	FPointArray m_Coords;
	bool m_interactive;
	MultiProgressDialog * m_progressDialog;
	bool m_cancel;
	ScribusDoc* m_Doc;
	Selection* m_tmpSel;
	int m_importerFlags;
	QString m_baseFile;
	QString m_FileName;
	QStringList m_importedColors;
	QStringList m_importedPatterns;
	bool m_firstPage;
	int m_pagecount;
	double m_topMargin;
	double m_leftMargin;
	double m_rightMargin;
	double m_bottomMargin;
	double m_conversionFactor;
	QHash<QString, QPainterPath> m_pathResources;
	QHash<QString, PageItem*> m_linkTargets;
	QHash<PageItem*, QString> m_linkSources;
	QHash<QString, ScFace> m_loadedFonts;
	ScZipHandler *m_uz;
	QStringList m_tempFontFiles;

public slots:
	void cancelRequested() { m_cancel = true; }
};

#endif
