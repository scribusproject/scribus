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

#include <QList>
#include <QMap>
#include <QString>

#include "pdfversion.h"
#include "scribusapi.h"
#include "scribusstructs.h"

struct PDFPresentationData;
struct LPIData;
class  MarginStruct;

/**
 * @brief PDF Options structure. Capable of verifying its self, but otherwise largely
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

	enum PDFFontEmbedding
	{
		EmbedFonts = 0,
		OutlineFonts = 1,
		DontEmbed  = 2
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

	bool exportsLayers() const;
	bool supportsEmbeddedOpenTypeFonts() const;
	bool supportsOCGs() const;
	bool supportsTransparency() const;

	bool firstUse { true };
	bool Thumbnails { false };
	bool Articles { false };
	bool useLayers { false };
	bool Compress { true };
	PDFCompression CompressMethod { Compression_Auto };
	int  Quality { 0 };
	bool RecalcPic { false };
	bool Bookmarks { false };
	int  PicRes { 300 };
	bool embedPDF { false };
	PDFVersion Version { PDFVersion::PDF_14 };
	int  Resolution { 300 };
	int  Binding { 0 };
	PDFFontEmbedding FontEmbedding { EmbedFonts };
	QList<QString> EmbedList;
	QList<QString> SubsetList;
	QList<QString> OutlineList;
	bool MirrorH { false };
	bool MirrorV { false };
	bool doClip { false };
	int  RotateDeg { 0 };
	bool PresentMode { false };
	QString fileName;
	bool isGrayscale { false };
	bool UseRGB { true };
	bool UseProfiles { false };
	bool UseProfiles2 { false };
	bool UseLPI { false };
	bool UseSpotColors { true };
	bool doMultiFile { false };
	bool openAfterExport { false };
	QMap<QString,LPIData> LPISettings;
	QString SolidProf;
	int  SComp { 3 };
	QString ImageProf;
	bool EmbeddedI { false };
	int  Intent2 { 0 };
	QString PrintProf;
	QString Info;
	int  Intent { 0 };
	MarginStruct bleeds;
	bool Encrypt { false };
	QString PassOwner;
	QString PassUser;
	int  Permissions { -4 };
	int  PageLayout { SinglePage };
	bool displayBookmarks { false };
	bool displayThumbs { false };
	bool displayLayers { false };
	bool displayFullscreen { false };
	bool hideToolBar { false };
	bool hideMenuBar { false };
	bool fitWindow { false };
	bool cropMarks { false };
	bool bleedMarks { false };
	bool registrationMarks { false };
	bool colorMarks { false };
	bool docInfoMarks { false };
	bool useDocBleeds { true };
	double markLength { 20.0 };
	double markOffset { 0.0 };
	QString openAction;
	int pageRangeSelection { 0 }; // All pages
	QString pageRangeString;
};

#endif
