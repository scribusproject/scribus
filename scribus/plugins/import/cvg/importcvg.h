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
	bool import(QString fn, const TransactionSettings& trSettings, int flags, bool showProgress = true);
	QImage readThumbnail(QString fn);

private:
	void parseHeader(QString fName, double &b, double &h);
	bool convert(QString fn);
	void getObjects(QDataStream &ts, bool colorFlag, quint32 lenData);
	void parseColor(quint32 dataF, quint32 dataS, bool color, quint16 flag);
	
	QList<PageItem*> m_Elements;
	int m_currentItemNr;
	QStack<QList<PageItem*> > m_groupStack;
	ColorList m_CustColors;
	double m_baseX, m_baseY;
	double m_docWidth;
	double m_docHeight;
	qreal m_scPg;

	double m_LineW;
	QString m_CurrColorFill;
	QString m_CurrColorStroke;
	double m_CurrStrokeShade;
	double m_CurrFillShade;
	QStringList m_importedColors;

	FPointArray m_Coords;
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
