/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PDFANALYZER_H
#define PDFANALYZER_H

#include <QList>
#include <QPair>
#include <QString>
#include <QMatrix>
#include "scconfig.h"

#ifdef HAVE_PODOFO
#include <podofo/podofo.h>
#endif

enum PDFContentStreamKeyword
{
	KW_k,
	KW_K,
	KW_rg,
	KW_RG,
	KW_g,
	KW_G,
	KW_CS,
	KW_cs,
	KW_SC,
	KW_SCN,
	KW_sc,
	KW_scn,
	KW_Do,
	KW_BI,
	KW_ID,
	KW_EI,
	KW_gs,
	KW_Tf,
	KW_cm,
	KW_q,
	KW_Q,
	KW_w,
	KW_J,
	KW_j,
	KW_M,
	KW_d,
	KW_Undefined
};
enum PDFColorSpace
{
	CS_DeviceGray,
	CS_DeviceRGB,
	CS_DeviceCMYK,
	CS_CalGray,
	CS_CalRGB,
	CS_Lab,
	CS_ICCBased,
	CS_Pattern,
	CS_Indexed,
	CS_Separation,
	CS_DeviceN,
	CS_Unknown
};
enum PDFFontType
{
	F_Type1,
	F_MMType1,
	F_TrueType,
	F_Type3,
	F_CIDFontType0,
	F_CIDFontType2,
	F_Unknown
};
struct PDFFont
{
	PDFFontType fontType;
	bool isEmbedded;
	bool isOpenType;
	PDFFont()
	{
		fontType = F_Unknown;
		isEmbedded = false;
		isOpenType = false;
	}
};
struct PDFGraphicState
{
	QMatrix ctm;
	PDFColorSpace strokeCS;
	PDFColorSpace fillCS;
	QList<double> strokeColor;
	QList<double> fillColor;
	double lineWidth;
	int lineCap;
	int lineJoin;
	double miterLimit;
	QPair<QList<int>, int> dashPattern;
	QPair<PDFFont, double> font;
	QList<QString> blendModes;
	double fillAlphaConstant;
	double strokeAlphaConstant;
	PDFGraphicState()
	{
		strokeCS = CS_DeviceGray;
		fillCS = CS_DeviceGray;
		strokeColor.append(0);
		fillColor.append(0);
		lineWidth = 1;
		lineCap = 0;
		lineJoin = 0;
		miterLimit = 10;
		QList<int> dashArray;
		int dashPhase = 0;
		dashPattern.first = dashArray;
		dashPattern.second = dashPhase;
		blendModes.append("Normal");
		fillAlphaConstant = 1;
		strokeAlphaConstant = 1;
	}
};
struct PDFImage
{
	QString imgName;
	int dpiX;
	int dpiY;
};

/**
 * PDFAnalyzer provides the facility to report various properties of a PDF.
 * At the moment, it can parse/analyze and record used color spaces, the use of transparency,
 * used fonts, and existing images in a page of a PDF.
 *
 * This class will be used by DocumentChecker's class to preflight and report any incompatible
 * properties according to a checker's profile.
 */
class PDFAnalyzer : public QObject
{
	Q_OBJECT

public:
	/**
	 * Instantiate a new PDFAnalyzer that will operate on the PDF specified by `filename'.
	 *
	 * \param filename Path to the PDF being analyzed.
	 */
	PDFAnalyzer(QString& filename);
	~PDFAnalyzer();

	/**
	 * Perform the actual inspection on one page of the PDF.
	 *
	 * \return A boolean is return indicating whether the process is successful.
	 * \param pageNum Specifying the page's number (zero-based) in the PDF where the analyzing process is opearted on.
	 * \param usedColorSpaces List of used color spaces in the page which will be filled while processing.
	 * \param hasTransparency A boolean which will be set to true after analyzing if the page contains transparency.
	 * \param usedFonts List of used fonts in the page which will be filled while processing.
	 * \param imgs List of images that this page contains.
	 */
	bool inspectPDF(int pageNum, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, QList<PDFImage> & imgs);
#ifdef HAVE_PODOFO
private:
	// pointer to the PoDoFo Pdf's object
	PoDoFo::PdfMemDocument* m_doc;

	// Call to this method to inspect a PdfCanvas (either a PdfPage or PdfXObject of subtype Form). This method will be called by inspectPDF
	// to start inspecting a PDF's page; it could well be called recursively to continue analyzing further in case form XObjects are painted
	// onto the page.
	bool inspectCanvas(PoDoFo::PdfCanvas* canvas, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, QList<PDFImage> & imgs);

	// Helper method to analyze a ColorSpace's array. They all have the form [/csType ...] (section 4.5 in PDF Spec 1.6).
	// csObject is a pointer to a PoDoFo's PdfObject which is in fact a PdfArray underneath.
	// A color space's type is returned.
	PDFColorSpace getCSType(PoDoFo::PdfObject* csObject);

	// Helper method to inspect a graphic state parameter dictionary (ExtGState subdictionary in the resource dictionary; Section 4.3.4 in PDF Spec 1.6).
	// Triggered by hitting a gs operator in the content stream.
	// extGStateObj is a pointer to a PoDoFo's PdfObject which is in fact a dictionary underneath.
	void inspectExtGStateObj(PoDoFo::PdfObject* extGStateObj, QList<PDFColorSpace> & usedColorSpaces, bool & hasTransparency, QList<PDFFont> & usedFonts, PDFGraphicState & currGS);

	// Helper method to analyze a font dictionary (section 5.4-5.8 in PDF Spec 1.6)
	// A PDFFont struct is returned containing some basic info regarding the specified font.
	PDFFont getFontInfo(PoDoFo::PdfObject* fontObj);
#endif
};
#endif
