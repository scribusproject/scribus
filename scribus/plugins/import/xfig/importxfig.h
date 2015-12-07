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
	bool import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);

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
	QVector<double> getDashValues(double linewidth, int code);
	void processArrows(int forward_arrow, QString fArrowData, int backward_arrow, QString bArrowData, int depth, PageItem *ite);
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
	
	QList<PageItem*> m_Elements;
	QList<PageItem*> m_PatternElements;
	QMultiMap<int, int> m_depthMap;
	int m_currentItemNr;
	QStack<QList<PageItem*> > m_groupStack;
	ColorList m_CustColors;
	double m_baseX, baseY;
	double m_docX;
	double m_docY;
	double m_docWidth;
	double m_docHeight;

	double m_LineW;
	QString m_CurrColorFill;
	QString m_CurrColorStroke;
	double m_CurrStrokeShade;
	double m_CurrFillShade;

	FPointArray m_Coords;
	FPointArray m_clipCoords;
	bool m_interactive;
	MultiProgressDialog * m_progressDialog;
	bool m_cancel;
	ScribusDoc* m_Doc;
	Selection* m_tmpSel;
	QMap<int, QString> m_importedColors;
	int m_importerFlags;
	bool m_patternMode;
	QString m_currentPatternDefName;
	QString m_currentPatternName;
	double m_patternX1;
	double m_patternY1;
	double m_patternX2;
	double m_patternY2;
	double m_currentPatternX;
	double m_currentPatternY;
	double m_currentPatternXScale;
	double m_currentPatternYScale;
	double m_currentPatternRotation;
	QString m_docCreator;
	QString m_docDate;
	QString m_docTime;
	QString m_docOrganisation;
	QString m_docTitle;
	int m_oldDocItemCount;
	QString m_baseFile;

public slots:
	void cancelRequested() { m_cancel = true; }
};

#endif
