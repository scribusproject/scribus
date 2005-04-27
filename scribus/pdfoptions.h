#ifndef PDFOPTIONS_H
#define PDFOPTIONS_H

/**
 * @file pdfoptions.h
 * @author Franz Schmidt
 * @author Craig Ringer
 * @brief Defines class PDFOptions, used for loading/saving/passing around PDF options
 */

#include "qstring.h"
#include "qmap.h"
#include "qvaluelist.h"
#include "qdom.h"

struct LPIData;
struct PDFPresentationData;

/**
 * @brief PDF Options struture. Capable of saving and loading to/from file and verifying its self.
 */
class PDFOptions
{
public:
	// Note: A default constructor, copy constructor, and
	// assignment operator must be defined so that this class
	// can be used as a value.
	// The automatically generated ones are quite sufficient.
	//PDFOptions();
	//PDFOptions(const PDFOptions& other);
	//PDFOptions& operator=(const PDFOptions& other);
	//~PDFOptions() {};

	enum VerifyResults
	{
		Verify_NoError = 0,
		Verify_OptionConflict,
		Verify_OptionOutOfRange,
		Verify_OtherError
	};

	enum PDFVersion
	{
		PDFVersion_13 = 13,
		PDFVersion_14 = 14,
		PDFVersion_15 = 15,
		PDFVersion_X3 = 12,
	};

	/**
	 * @author Craig Ringer
	 * @brief Sanity check the options defined.
	 *
	 * Unimplemented, always returns Verify_OtherError
	 *
	 * Checks the PDF option structure for conflicts between mututally
	 * exclusive options, ensures all options are within sane ranges,
	 * and that there are no nonsensical options values set. If nothing
	 * is wrong, returns Verify_NoError, otherwise returns error code from
	 * PDFOptions::VerifyResults. If problemDescription is not NULL,
	 * it will contain a human-readable description of the error on return.
	 *
	 * @warning DO NOT *EVER* TEST THE VALUE OF problemDescription. Rely on the
	 *			return code instead. problemDescription is subject to
	 *			translation and its contents may change without notice.
	 *
	 * @param problemDescription Error description
	 * @return Verify_NoError for sane options, otherwise error code.
	 */
	PDFOptions::VerifyResults verify(QString* problemDescription);
	PDFOptions::VerifyResults verify();

	bool Thumbnails;
	bool Articles;
	bool useLayers;
	bool Compress;
	int CompressMethod;
	int Quality;
	bool RecalcPic;
	bool Bookmarks;
	int PicRes;
	PDFVersion Version;
	int Resolution;
	int Binding;
	QValueList<QString> EmbedList;
	QValueList<QString> SubsetList;
	bool MirrorH;
	bool MirrorV;
	int RotateDeg;
	bool PresentMode;
	QValueList<PDFPresentationData> PresentVals;
	QString Datei;
	bool isGrayscale;
	bool UseRGB;
	bool UseProfiles;
	bool UseProfiles2;
	bool UseLPI;
	QMap<QString,LPIData> LPISettings;
	QString SolidProf;
	int SComp;
	QString ImageProf;
	bool EmbeddedI;
	int Intent2;
	QString PrintProf;
	QString Info;
	int Intent;
	double BleedTop;
	double BleedLeft;
	double BleedRight;
	double BleedBottom;
	bool Encrypt;
	QString PassOwner;
	QString PassUser;
	int Permissions;
};

/*
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
 *       // handle error. Not like this:
 *       qDebug("Darn, it didn't work.");
 *
 *    // Reading:
 *    PDFOptions opts;
 *    PDFOptionsIO io(opts);
 *    if (!io.readFrom("/path/to/file"))
 *       // handle error. Not like this:
 *       qDebug("incomprehensible gibberish");
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
	bool writeTo(FILE* outFilePtr);

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
	bool readFrom(FILE* inFilePtr);

	/**
	 * @brief Return human-readable explanation of last error.
	 * @warning Do not depend on particular values of this in your code.
	 */
	const QString& lastError() const;

protected:
	// Populate the current DOM tree with the settings from the
	// current PDFOptions instance.
	void buildSettings();

	// Helper functions. Add elements under the root element
	// with a single attribute "value=" set to the passed value.
	void addElem(QString name, bool value);
	void addElem(QString name, QString value);
	void addElem(QString name, int value);
	void addElem(QString name, double value);
	void addList(QString name, QValueList<QString>& value);

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
