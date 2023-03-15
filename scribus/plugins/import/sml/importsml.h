/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTSML_H
#define IMPORTSML_H

#include <QDomElement>
#include <QtGlobal>
#include <QList>
#include <QMultiMap>
#include <QObject>
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
	bool import(const QString& fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(const QString& fn);

private:
	void parseHeader(const QString& fName, double &b, double &h);
	bool convert(const QString& fn);
	void finishItem(QDomElement &e, PageItem* ite);
	void processShapeNode(QDomElement &elem);
	QString processColor(QDomElement &elem);
	void processStrokeNode(QDomElement &elem);
	void processFillNode(QDomElement &elem);
	void processLineNode(QDomElement &elem);
	void processPointNode(QDomElement &elem);
	
	QList<PageItem*> Elements;
	int currentItemNr { 0 };
	QStack<QList<PageItem*> > groupStack;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };

	double LineW { 1.0 };
	QString CurrColorFill { "White" };
	QString CurrColorStroke { "Black" };
	double CurrStrokeShade { 100.0 };
	double CurrFillShade { 100.0 };
	Qt::PenStyle Dash { Qt::SolidLine };
	Qt::PenCapStyle LineEnd { Qt::FlatCap };
	Qt::PenJoinStyle LineJoin { Qt::MiterJoin };
	int fillStyle { 1 };
	QStringList importedColors;
	double currx { 0.0 };
	double curry { 0.0 };
	double startx { 0.0 };
	double starty { 0.0 };
	bool first { true };
	int count { 0 };

	FPointArray Coords;
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
