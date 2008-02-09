/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTXFIG_H
#define IMPORTXFIG_H

#include "qglobal.h"
#include "qobject.h"
#include "qstring.h"

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include <QList>
#include <QMatrix>
#include <QMultiMap>

class MultiProgressDialog;
class ScribusDoc;
class Selection;

//! \brief Xfig importer plugin
class XfigPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the AI importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	XfigPlug( ScribusDoc* doc, int flags );
	~XfigPlug();

	/*!
	\author Franz Schmid
	\date
	\brief Perform import.
	\param fn QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval bool true if import was ok
	 */
	bool import(QString fn, int flags, bool showProgress = true);

private:
	
	/*!
	\author Franz Schmid
	\date
	\brief Does the conversion.
	\param fn QString
	\param x X position
	\param y Y position
	\param b double
	\param h double
	\retval bool true if conversion was ok
	 */
	bool parseHeader(QString fName, double &x, double &y, double &b, double &h);
	void parseColor(QString data);
	void useColor(int colorNum, int area_fill, bool forFill);
	QList<double> getDashValues(double linewidth, int code);
	void processPolyline(QDataStream &ts, QString data);
	void processSpline(QDataStream &ts, QString data);
	void processArc(QDataStream &ts, QString data);
	void processEllipse(QString data);
	QString cleanText(QString text);
	void processText(QString data);
	void processData(QDataStream &ts, QString data);
	double fig2Pts(double in);
	void resortItems();
	bool convert(QString fn);
	
	QList<PageItem*> Elements;
	QList<PageItem*> PatternElements;
	QMultiMap<int, int> depthMap;
	int currentItemNr;
	QStack<QList<PageItem*> > groupStack;
	ColorList CustColors;
	double baseX, baseY;
	double docX;
	double docY;
	double docWidth;
	double docHeight;

	double LineW;
	QString CurrColorFill;
	QString CurrColorStroke;
	double CurrStrokeShade;
	double CurrFillShade;

	FPointArray Coords;
	FPointArray clipCoords;
	bool interactive;
	MultiProgressDialog * progressDialog;
	bool cancel;
	ScribusDoc* m_Doc;
	Selection* tmpSel;
	QMap<int, QString> importedColors;
	int importerFlags;
	bool patternMode;
	QString currentPatternDefName;
	QString currentPatternName;
	double patternX1;
	double patternY1;
	double patternX2;
	double patternY2;
	double currentPatternX;
	double currentPatternY;
	double currentPatternXScale;
	double currentPatternYScale;
	double currentPatternRotation;
	QString docCreator;
	QString docDate;
	QString docTime;
	QString docOrganisation;
	QString docTitle;
	int oldDocItemCount;
	QString baseFile;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
