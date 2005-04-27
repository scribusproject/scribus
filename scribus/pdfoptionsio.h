#ifndef PDFOPTIONSIO_H
#define PDFOPTIONSIO_H

#include "pdfoptions.h"
#include "qdom.h"

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
 * @sa PDFOptions
 */
class PDFOptionsIO
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
	 * @param outStream QTextStream to write output to.
	 * @return True for success.
	 */
	bool writeTo(QTextStream& outStream);
	bool writeTo(QString outFileName);

	/**
	 * @brief Load the PDF settings from a file or other data stream, overwriting
	 *        any current settings
	 *
	 * @warning unimplemented, always fails
	 *
	 * @warning on failure, struct contents are undefined.
	 *
	 * @param inStream QTextStream to settings data from.
	 * @return True for success.
	 */
	bool readFrom(QTextStream& inStream);
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
	QString PDFOptionsIO::buildXMLString();

	// Populate the current DOM tree with the settings from the
	// current PDFOptions instance.
	void buildSettings();

	// Helper functions. Add elements under the root element
	// with a single attribute "value=" set to the passed value.
	void addElem(QDomElement& addTo, QString name, bool value);
	void addElem(QDomElement& addTo, QString name, QString value);
	void addElem(QDomElement& addTo, QString name, int value);
	void addElem(QDomElement& addTo, QString name, double value);
	void addList(QDomElement& addTo, QString name, QValueList<QString>& value);

	// Helper: add the PresentVals data to the document
	void addPresentationData();
	// Helper: Add the LPI settings to the document
	void addLPISettings();


	// The QDomDocument used by the class for all its XML work
	QDomDocument doc;
	// The root element
	QDomElement root;
	// The PDFOptions instance we're operating on
	PDFOptions* opts;

	// Version, of the form Mmpr: Major, minor, patch, revision
	// eg 1300 - 1.3.0r0
	static const int formatVersion;

	// Error explanation if a function fails
	// For user only, do not depend on particular values of this.
	QString error;
};

#endif
