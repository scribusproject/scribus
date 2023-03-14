/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTXFIG_H
#define IMPORTXFIG_H

#include "pluginapi.h"
#include "sccolor.h"
#include "fpointarray.h"
#include <QList>
#include <QStack>
#include <QTransform>
#include <QMultiMap>
#include <QtGlobal>
#include <QObject>
#include <QString>

class MultiProgressDialog;
class PageItem;
class ScribusDoc;
class Selection;
class TransactionSettings;

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
	\param trSettings undo transaction settings
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval bool true if import was ok
	 */
	bool import(const QString& fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(const QString& fn);

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
	bool parseHeader(const QString& fName, double &x, double &y, double &b, double &h);
	void parseColor(QString data);
	void useColor(int colorNum, int area_fill, bool forFill);
	QVector<double> getDashValues(double linewidth, int code);
	void processArrows(int forward_arrow, QString fArrowData, int backward_arrow, QString bArrowData, int depth, PageItem *ite);
	void processPolyline(QDataStream &ts, const QString& data);
	void processSpline(QDataStream &ts, const QString& data);
	void processArc(QDataStream &ts, const QString& data);
	void processEllipse(const QString& data);
	QString cleanText(const QString& text);
	void processText(const QString& data);
	void processData(QDataStream &ts, const QString& data);
	double fig2Pts(double in);
	void resortItems();
	bool convert(const QString& fn);
	
	QList<PageItem*> Elements;
	QList<PageItem*> PatternElements;
	QMultiMap<int, int> depthMap;
	int currentItemNr;
	QStack<QList<PageItem*> > groupStack;
	ColorList CustColors;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docX { 0.0 };
	double docY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };

	double LineW { 1.0 };
	QString CurrColorFill { "White" };
	QString CurrColorStroke { "Black" };
	double CurrStrokeShade { 100.0 };
	double CurrFillShade { 100.0 };

	FPointArray Coords;
	FPointArray clipCoords;
	bool interactive { false };
	MultiProgressDialog * progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	QMap<int, QString> importedColors;
	int importerFlags { 0 };
	bool patternMode { false };
	QString currentPatternDefName;
	QString currentPatternName;
	double patternX1 { 0.0 };
	double patternY1 { 0.0 };
	double patternX2 { 0.0 };
	double patternY2 { 0.0 };
	double currentPatternX { 0.0 };
	double currentPatternY { 0.0 };
	double currentPatternXScale { 1.0 };
	double currentPatternYScale { 1.0 };
	double currentPatternRotation { 0.0 };
	QString docCreator;
	QString docDate;
	QString docTime;
	QString docOrganisation;
	QString docTitle;
	int oldDocItemCount { 0 };
	QString baseFile;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
