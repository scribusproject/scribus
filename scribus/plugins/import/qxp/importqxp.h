/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTQXP_H
#define IMPORTQXP_H

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

//! \brief QXP importer plugin
class QxpPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the Fh importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	QxpPlug(ScribusDoc* doc, int flags);
	~QxpPlug() override;

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
	bool importFile(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);

private:
	bool convert(QString fn);

	QList<PageItem*> Elements;
	double baseX { 0.0 };
	double baseY { 0.0 };
	double docWidth { 1.0 };
	double docHeight { 1.0 };

	QStringList importedColors;
	QStringList importedPatterns;

	bool interactive { false };
	MultiProgressDialog* progressDialog { nullptr };
	bool cancel { false };
	ScribusDoc* m_Doc { nullptr };
	Selection* tmpSel { nullptr };
	int importerFlags { 0 };

public slots:
	void cancelRequested() { cancel = true; }
};

#endif
