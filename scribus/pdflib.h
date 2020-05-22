/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PDFLIB_H
#define PDFLIB_H

#include <QObject>
#include <QImage>
#include <QMap>
#include <vector>

#include "scconfig.h"
#include "scribusapi.h"

class PDFOptions;
class ScribusDoc;

/**
 * PDFLib provides an interface to the core PDF export functionality of scribus.
 * This class does not directly implement export; it's purpose is to isolate
 * implementation details from the rest of the codebase.
 *
 * The real implementation is in pdflib_core.cpp .
 */
class SCRIBUS_API PDFlib : public QObject
{
	Q_OBJECT

public:
	/**
	 * Instantiate a new PDFLib that will operate on `docu'.
	 *
	 * \warning current PDFLib implementations may not function correctly if re-used for
	 *          multiple exports. Create a new object for each job.
	 *
	 * \param docu Document to use in export process
	 */
	explicit PDFlib(ScribusDoc & docu);
	explicit PDFlib(ScribusDoc & docu, const PDFOptions& options);

	~PDFlib();

	/**
	 * Perform an export.
	 *
	 * \param fn Output file name
	 * \param pageNs List of pages from document to be exported as sequential PDF pages
	 * \param thumbs A mapping of input (document) page numbers to pre-rendered thumbnails.
	 */
	bool doExport(const QString& fn, const std::vector<int> & pageNs, const QMap<int, QImage>& thumbs);

	/**
	 * Return an error message in case export has failed.
	 */
	const QString& errorMessage();
	/**
	 * Return if export has been aborted
	 */
	bool  exportAborted();

private:
    /// A pointer to the real implementation of pdflib .
	void* m_impl;
};

#endif
