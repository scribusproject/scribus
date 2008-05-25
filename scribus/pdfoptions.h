/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PDFOPTIONS_H
#define PDFOPTIONS_H

/**
 * @file pdfoptions.h
 * @author Franz Schmid
 * @author Craig Ringer
 * @brief Defines class PDFOptions, used for loading/saving/passing around PDF options
 */

#include "qstring.h"
#include "qmap.h"
#include "QList"
#include "scribusapi.h"
#include "scribusstructs.h"

struct PDFPresentationData;
struct LPIData;
class  MarginStruct;

/**
 * @brief PDF Options struture. Capable of verifying its self, but otherwise largely
 *        a dumb struct.
 *
 * If you change this class, please ensure that PDFOptionsIO is
 * updated to match and scribus/dtd/scribuspdfoptions.dtd is tweaked
 * if required.
 *
 * @sa PDFOptionsIO
 */
class SCRIBUS_API PDFOptions
{
public:

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

	enum PDFPageLayout
	{
		SinglePage = 0,
		OneColumn,
		TwoColumnLeft,
		TwoColumnRight
	};

	enum PDFCompression
	{
		Compression_Auto = 0,
		Compression_JPEG = 1,
		Compression_ZIP  = 2,
		Compression_None = 3
	};

	/**
	 * @author Craig Ringer
	 * @brief Sanity check the options defined.
	 *
	 * Unimplemented, always returns Verify_NoError
	 *
	 * Checks the PDF option structure for conflicts between mututally
	 * exclusive options, ensures all options are within sane ranges,
	 * and that there are no nonsensical options values set. If nothing
	 * is wrong, returns Verify_NoError, otherwise returns error code from
	 * PDFOptions::VerifyResults. If problemDescription is not NULL,
	 * it will contain a human-readable description of the error on return.
	 *
	 * @warning DO NOT *EVER* TEST THE VALUE OF problemDescription. Rely on the
	 *          return code instead. problemDescription is subject to
	 *          translation and its contents may change without notice.
	 *
	 * @param problemDescription Error description
	 * @return Verify_NoError for sane options, otherwise error code.
	 */
	PDFOptions::VerifyResults verify(QString* problemDescription);
	PDFOptions::VerifyResults verify();

	bool firstUse;
	bool Thumbnails;
	bool Articles;
	bool useLayers;
	bool Compress;
	PDFCompression CompressMethod;
	int  Quality;
	bool RecalcPic;
	bool Bookmarks;
	int  PicRes;
	bool embedPDF;
	PDFVersion Version;
	int  Resolution;
	int  Binding;
	QList<QString> EmbedList;
	QList<QString> SubsetList;
	bool MirrorH;
	bool MirrorV;
	bool doClip;
	int  RotateDeg;
	bool PresentMode;
	QList<PDFPresentationData> PresentVals;
	QString fileName;
	bool isGrayscale;
	bool UseRGB;
	bool UseProfiles;
	bool UseProfiles2;
	bool UseLPI;
	bool UseSpotColors;
	bool doOverprint;
	bool doMultiFile;
	QMap<QString,LPIData> LPISettings;
	QString SolidProf;
	int  SComp;
	QString ImageProf;
	bool EmbeddedI;
	int  Intent2;
	QString PrintProf;
	QString Info;
	int  Intent;
	MarginStruct bleeds;
	bool Encrypt;
	QString PassOwner;
	QString PassUser;
	int  Permissions;
	int  PageLayout;
	bool displayBookmarks;
	bool displayThumbs;
	bool displayLayers;
	bool displayFullscreen;
	bool hideToolBar;
	bool hideMenuBar;
	bool fitWindow;
	bool cropMarks;
	bool bleedMarks;
	bool registrationMarks;
	bool colorMarks;
	bool docInfoMarks;
	bool useDocBleeds;
	double markOffset;
	QString openAction;
};

#endif
