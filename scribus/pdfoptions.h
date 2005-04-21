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

	/**
	 * @author Craig Ringer
	 * @brief Save the PDF settings to a file or other stream
	 *
	 * @warning unimplemented, always fails.
	 *
	 * @param outStream QTextStream to write output to.
	 * @return True for success.
	 */
	bool saveTo(QTextStream& outStream) const;
	bool saveTo(QString outFileName) const;
	bool saveTo(FILE* outFilePtr) const;

	/**
	 * @author Craig Ringer
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

#endif
