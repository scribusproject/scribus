/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTSHAPE_H
#define IMPORTSHAPE_H

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
#include <QDomElement>

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
	bool import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);

private:
	void parseHeader(QString fName, double &b, double &h);
	bool convert(QString fn);
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
	double baseX, baseY;
	double docWidth;
	double docHeight;
	QStringList importedColors;
	bool first;
	int count;
	bool FirstM, WasM, PathClosed;
	double CurrX, CurrY, StartX, StartY, Conversion;
	int PathLen;

	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	int importerFlags;
	int oldDocItemCount;
	QString baseFile;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
