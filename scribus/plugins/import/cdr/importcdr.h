/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTCDR_H
#define IMPORTCDR_H

#include <QObject>
#include <QString>

#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"
#include "fpointarray.h"
#include "vgradient.h"
#include <QList>
#include <QTransform>
#include <QMultiMap>
#include <QVector>

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

//! \brief CDR importer plugin
class CdrPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Cdr importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	CdrPlug( ScribusDoc* doc, int flags );
	~CdrPlug();

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
	bool convert(const QString& fn);

	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };

	QList<PageItem*> Elements;
	QStringList importedColors;
	QStringList importedPatterns;

	bool interactive { false };
	bool cancel { false };
	int  importerFlags { 0 };

	MultiProgressDialog* progressDialog { nullptr };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
