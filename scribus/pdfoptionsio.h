/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PDFOPTIONSIO_H
#define PDFOPTIONSIO_H

#include <QDomElement>
#include <QList>

#include "pdfoptions.h"
#include "scribusapi.h"

/**
 * @file pdfoptionsio.h
 * @author Craig Ringer
 */

/**
 * @brief Helper class for reading/writing PDFOptions
 * @author Craig Ringer
 *
 * PDFOptionsIO reads and writes PDFOptions to various
 * formats. Currently only a custom XML format is
 * supported, but support for reading/writing Adobe's
 * .joboptions (for example) might be added in future.
 *
 * Usage:
 *    // Writing:
 *    // where `opts' is an existing PDFOptions instance
 *    PDFOptionsIO io(opts);
 *    if (!io.writeTo("/path/to/file"))
 *       qDebug("Failed to save settings: %s", io.lastError.utf8());
 *
 *    // Reading:
 *    PDFOptions opts;
 *    PDFOptionsIO io(opts);
 *    if (!io.readFrom("/path/to/file"))
 *       qDebug("Failed to load settings: %s", io.lastError.utf8());
 *
 *  You should generally not keep PDFOptionsIO objects around;
 *  just create an instance when you need it.
 *
 * @sa PDFOptions
 */
class SCRIBUS_API PDFOptionsIO
{
public:
	/**
	 * @brief simple ctor
	 */
	PDFOptionsIO(PDFOptions& opts);

	/**
	 * @brief Save the PDF settings to a file or other stream
	 *
	 * @warning unimplemented, always fails.
	 *
	 * @param outDevice QIODevice to write output to.
	 * @param includePasswords true if with passwords
	 * @return True for success.
	 */
	bool writeTo(QIODevice& outDevice, bool includePasswords = false);
	bool writeTo(QString outFileName, bool includePasswords = false);

	/**
	 * @brief Load the PDF settings from a file or other data stream, overwriting
	 *        any current settings
	 *
	 * @warning unimplemented, always fails
	 *
	 * @warning on failure, struct contents are undefined.
	 *
	 * @param inStream QIODevice to settings data from.
	 * @return True for success.
	 */
	bool readFrom(QIODevice& inStream);
	bool readFrom(QString inFileName);

	/**
	 * @brief Return human-readable explanation of last error.
	 * @warning Do not depend on particular values of this in your code.
	 */
	const QString& lastError() const;

protected:
	// Build and return an XML representation of the settings.
	// QString::null is returned on failure, in which case
	// the error string is set.
	QString buildXMLString();

	// Populate the current DOM tree with the settings from the
	// current PDFOptions instance.
	void buildSettings();

	// Helper functions. Add elements under the root element
	// with a single attribute "value=" set to the passed value.
	void addElem(QDomElement& addTo, QString name, bool value);
	void addElem(QDomElement& addTo, QString name, QString value);
	void addElem(QDomElement& addTo, QString name, int value);
	void addElem(QDomElement& addTo, QString name, double value);
	void addList(QDomElement& addTo, QString name, QList<QString>& value);

	// Helper: add the PresentVals data to the document
	void addPresentationData();
	// Helper: Add the LPI settings to the document
	void addLPISettings();

	// Helper: set the `opts' members from the current DOM tree
	bool readSettings();

	// Helper functions. Read various single elements into variables
	// All of these return true for success. On failure, the passed
	// value pointer is undefined.
	bool readElem(QDomElement& parent, QString name, bool* value);
	bool readElem(QDomElement& parent, QString name, int* value);
	bool readElem(QDomElement& parent, QString name, double* value);
	bool readElem(QDomElement& parent, QString name, QString* value);
	bool readList(QDomElement& parent, QString name, QList<QString>* value);

	bool readPDFVersion();
	bool readPresentationData();
	bool readLPISettings();

	// Returns the named node under `parent' iff it's unique
	QDomNode getUniqueNode(QDomElement& parent, QString name);

	// Return a QDomElement corresponding to the passed node, ensuring it's a
	// valid element, the only one of its name under `parent', it has name
	// `name', and (by default) has a `value' attribute.
	QDomElement getValueElement(QDomNode& node, QString name, bool isValue = true);

	// The QDomDocument used by the class for all its XML work
	QDomDocument m_doc;
	// The root element
	QDomElement m_root;
	// The PDFOptions instance we're operating on
	PDFOptions* m_opts;
	// Whether to save passwords when writing out settings
	bool m_includePasswords;

	// Version, of the form Mmpr: Major, minor, patch, revision
	// eg 1300 - 1.3.0r0
	static const int formatVersion;

	// Error explanation if a function fails
	// For user only, do not depend on particular values of this.
	QString m_error;
};

#endif
