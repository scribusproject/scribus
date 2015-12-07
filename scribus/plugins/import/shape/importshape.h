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
	
	QList<PageItem*> m_Elements;
	QStack<QList<PageItem*> > m_groupStack;
	double m_baseX, m_baseY;
	double m_docWidth;
	double m_docHeight;
	QStringList m_importedColors;
	bool m_first;
	int m_count;
	bool m_FirstM, m_WasM, m_PathClosed;
	double m_CurrX, m_CurrY, m_StartX, m_StartY, m_Conversion;
	int m_PathLen;

	bool m_interactive;
	MultiProgressDialog * m_progressDialog;
	bool m_cancel;
	ScribusDoc* m_Doc;
	Selection* m_tmpSel;
	int m_importerFlags;
	int m_oldDocItemCount;
	QString m_baseFile;

public slots:
	void cancelRequested() { m_cancel = true; }
};

#endif
