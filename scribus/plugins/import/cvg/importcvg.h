/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTCVG_H
#define IMPORTCVG_H

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

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

//! \brief Cvg importer plugin
class CvgPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Cvg importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	CvgPlug( ScribusDoc* doc, int flags );
	~CvgPlug();

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
	bool importFile(const QString& fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(const QString& fn);

private:
	void parseHeader(const QString& fName, double &b, double &h);
	bool convert(const QString& fn);
	void getObjects(QDataStream &ts, bool colorFlag, quint32 lenData);
	void parseColor(quint32 dataF, quint32 dataS, bool color, quint16 flag);
	
	QList<PageItem*> Elements;
	int currentItemNr { 0 };
	QStack<QList<PageItem*> > groupStack;
	ColorList CustColors;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 0.0 };
	double docHeight { 0.0 };
	qreal scPg { 0 };

	double LineW { 0.0 };
	QString CurrColorFill;
	QString CurrColorStroke;
	double CurrStrokeShade { 0.0 };
	double CurrFillShade { 0.0 };
	QStringList importedColors;

	FPointArray Coords;
	bool interactive { false };
	MultiProgressDialog* progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };
	QString baseFile;

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
