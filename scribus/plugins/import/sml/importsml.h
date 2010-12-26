/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTSML_H
#define IMPORTSML_H

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
class SmlPlug : public QObject
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
	SmlPlug( ScribusDoc* doc, int flags );
	~SmlPlug();

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
	void finishItem(QDomElement &e, PageItem* ite);
	void processShapeNode(QDomElement &elem);
	QString processColor(QDomElement &elem);
	void processStrokeNode(QDomElement &elem);
	void processFillNode(QDomElement &elem);
	void processLineNode(QDomElement &elem);
	void processPointNode(QDomElement &elem);
	
	QList<PageItem*> Elements;
	int currentItemNr;
	QStack<QList<PageItem*> > groupStack;
	double baseX, baseY;
	double docWidth;
	double docHeight;

	double LineW;
	QString CurrColorFill;
	QString CurrColorStroke;
	double CurrStrokeShade;
	double CurrFillShade;
	Qt::PenStyle Dash;
	Qt::PenCapStyle LineEnd;
	Qt::PenJoinStyle LineJoin;
	int fillStyle;
	QStringList importedColors;
	double currx, curry, startx, starty;
	bool first;
	int count;

	FPointArray Coords;
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
