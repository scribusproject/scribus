/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMPORTPDF_H
#define IMPORTPDF_H

#include <QBrush>
#include <QBuffer>
#include <QColor>
#include <QImage>
#include <QList>
#include <QMultiMap>
#include <QObject>
#include <QPen>
#include <QtGlobal>
#include <QSizeF>
#include <QString>
#include <QTextStream>

#include <memory>

#include "fpointarray.h"
#include "importpdfconfig.h"
#include "pluginapi.h"
#include "pageitem.h"
#include "sccolor.h"

class QColor;

class MultiProgressDialog;
class ScribusDoc;
class Selection;
class TransactionSettings;

class GooString;
class PDFDoc;

//! \brief PDF importer plugin
class PdfPlug : public QObject
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\date
	\brief Create the PDF importer window.
	\param fName QString
	\param flags combination of loadFlags
	\param showProgress if progress must be displayed
	\retval EPSPlug plugin
	*/
	PdfPlug( ScribusDoc* doc, int flags );
	~PdfPlug();

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
	QImage readPreview(int pgNum, int width, int height, int box);
	enum PDF_Box_Type
	{
		Media_Box	= 0,
		Bleed_Box	= 1,
		Trim_Box	= 2,
		Crop_Box	= 3,
		Art_Box		= 4
	};

private:
	bool convert(const QString& fn);
	QRectF getCBox(int box, int pgNum);
	QString UnicodeParsedString(POPPLER_CONST GooString *s1);
	QString UnicodeParsedString(const std::string& s1);
	
	QList<PageItem*> m_elements;

	QStringList m_importedColors;
	
	bool m_cancel {false};
	bool m_interactive;
	bool m_noDialogs;
	MultiProgressDialog *m_progressDialog {nullptr};
	ScribusDoc* m_Doc {nullptr};
	Selection* m_tmpSele {nullptr};
	int m_importerFlags;
	QString m_baseFile;
	PDFDoc *m_pdfDoc {nullptr};

public slots:
	void cancelRequested() { m_cancel = true; }
};

#endif
