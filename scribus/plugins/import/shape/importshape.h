/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTSHAPE_H
#define IMPORTSHAPE_H

#include <QDomElement>
#include <QList>
#include <QtGlobal>
#include <QObject>
#include <QMultiMap>
#include <QString>
#include <QTransform>

#include "pluginapi.h"
#include "fpointarray.h"
#include "pageitem.h"
#include "sccolor.h"

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

//! \brief Sml importer plugin
class ShapePlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Sml importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	ShapePlug( ScribusDoc* doc, int flags );
	~ShapePlug();

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
	void parseHeader(const QString& fName, double &b, double &h);
	bool convert(const QString& fn);
	void finishItem(PageItem* ite);
	void parseGroup(QDomNode &DOC);
	void parseGroupProperties(QDomNode &DOC, double &minXCoor, double &minYCoor, double &maxXCoor, double &maxYCoor, bool &firstCheck);
	double parseUnit(const QString &unit);
	const char * getCoord( const char *ptr, double &number );
	bool parseSVG( const QString &s, FPointArray *ite );
	void svgClosePath(FPointArray *i);
	void svgMoveTo(double x1, double y1);
	void svgLineTo(FPointArray *i, double x1, double y1);
	void svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3);
	
	QList<PageItem*> Elements;
	QStack<QList<PageItem*> > groupStack;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };
	QStringList importedColors;
	bool FirstM { false };
	bool WasM { false };
	bool PathClosed { false };
	double CurrX { 0.0 }; 
	double CurrY { 0.0 };
	double StartX { 0.0 };
	double StartY { 0.0 };
	double Conversion { 1.0 };
	int PathLen { 0 };

	bool interactive { false };
	MultiProgressDialog * progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };
	QString baseFile;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
