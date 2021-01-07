/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PDFTEXTRECOGNITION_H
#define PDFTEXTRECOGNITION_H

#include <QSizeF>
#include <QStack>
#include <QString>

#include "pageitem.h"
#include "importpdfconfig.h"
#include "slaoutput.h"

#include <poppler/GfxState.h>
#include <poppler/CharCodeToUnicode.h>

/* PDF TextBox Framework */
/*
* Holds all the details for each glyph in the text imported from the pdf file.
*
*/
struct PdfGlyph
{
	double dx;  // X advance value
	double dy;  // Y advance value
	double rise;    // Text rise parameter
	QChar code;   // UTF-16 coded character
};


class PdfTextRegionLine
{
public:
	qreal maxHeight = {};
	//we can probably use maxHeight for this.	
	qreal width = {};
	int glyphIndex = {};
	QPointF baseOrigin = QPointF({}, {});
	std::vector<PdfTextRegionLine> segments = std::vector<PdfTextRegionLine>();

};

class PdfTextRegion
{
public:
	enum class LineType
	{
		FIRSTPOINT,
		SAMELINE,
		STYLESUPERSCRIPT,
		STYLENORMALRETURN,
		STYLEBELOWBASELINE,
		NEWLINE,
		ENDOFLINE, //TODO: Implement an end of line test
		FAIL
	};
#
	/*
* the bounding box shape splines in percentage of width and height. In this case 100% as we want to clip shape to be the full TextBox width and height. */
	static constexpr double boundingBoxShape[32] = {
		0.0, 0.0,
		0.0, 0.0,
		100.0, 0.0,
		100.0, 0.0,
		100.0, 0.0,
		100.0, 0.0,
		100.0, 100.0,
		100.0, 100.0,
		100.0, 100.0,
		100.0, 100.0,
		0.0, 100.0,
		0.0, 100.0,
		0.0, 100.0,
		0.0, 100.0,
		0.0, 0.0,
		0.0, 0.0
	};

	QPointF pdfTextRegionBasenOrigin = QPointF({}, {});
	qreal maxHeight = {};
	qreal lineSpacing = { 1 };
	std::vector<PdfTextRegionLine> pdfTextRegionLines = std::vector<PdfTextRegionLine>();
	qreal maxWidth = {};
	QPointF lineBaseXY = QPointF({ }, { }); //updated with the best match left value from all the textRegionLines and the best bottom value from the textRegionLines.segments;
	QPointF lastXY = QPointF({}, {});
	std::vector<PdfGlyph> glyphs;

	static bool collinear(qreal a, qreal b);
	bool isCloseToX(qreal x1, qreal x2);
	bool isCloseToY(qreal y1, qreal y2);
	bool adjunctLesser(qreal testY, qreal lastY, qreal baseY);
	bool adjunctGreater(qreal testY, qreal lastY, qreal baseY);
	PdfTextRegion::LineType linearTest(QPointF point, bool xInLimits, bool yInLimits);
	PdfTextRegion::LineType isRegionConcurrent(QPointF newPoint);
	PdfTextRegion::LineType moveToPoint(QPointF newPoint);
	PdfTextRegion::LineType addGlyphAtPoint(QPointF newGlyphPoint, PdfGlyph new_glyph);
	void renderToTextFrame(PageItem* textNode);
	bool isNew();
};

class PdfTextRecognition
{
public:
	PdfTextRecognition();
	~PdfTextRecognition();

	enum class AddCharMode
	{
		ADDFIRSTCHAR,
		ADDBASICCHAR,
		ADDCHARWITHNEWSTYLE,
		ADDCHARWITHPREVIOUSSTYLE,
		ADDCHARWITHBASESTLYE
	};

	void setCharMode(AddCharMode mode)
	{
		m_addCharMode = mode;
	}

	PdfTextRegion* activePdfTextRegion = nullptr; //faster and cleaner than calling back on the vector all the time.

	void addPdfTextRegion();
	void addChar(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, POPPLER_CONST_082 Unicode* u, int uLen);
	bool isNewLineOrRegion(QPointF newPosition);

private:
	std::vector<PdfTextRegion> m_pdfTextRegions = std::vector<PdfTextRegion>();
	AddCharMode m_addCharMode = AddCharMode::ADDFIRSTCHAR;
	PdfGlyph AddCharCommon(GfxState* state, double x, double y, double dx, double dy, Unicode const* u, int uLen);
	PdfGlyph AddFirstChar(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen);
	PdfGlyph AddBasicChar(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen);
	PdfGlyph AddCharWithNewStyle(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen);
	PdfGlyph AddCharWithPreviousStyle(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen);
};


class PdfTextOutputDev : public SlaOutputDev
{
public:
	PdfTextOutputDev(ScribusDoc* doc, QList<PageItem*>* Elements, QStringList* importedColors, int flags);
	virtual ~PdfTextOutputDev();

	void updateFont(GfxState* state) override;

	//----- text drawing
	void  beginTextObject(GfxState* state) override;
	void  endTextObject(GfxState* state) override;
	void  drawChar(GfxState* state, double /*x*/, double /*y*/, double /*dx*/, double /*dy*/, double /*originX*/, double /*originY*/, CharCode /*code*/, int /*nBytes*/, POPPLER_CONST_082 Unicode* /*u*/, int /*uLen*/) override;
	GBool beginType3Char(GfxState* /*state*/, double /*x*/, double /*y*/, double /*dx*/, double /*dy*/, CharCode /*code*/, POPPLER_CONST_082 Unicode* /*u*/, int /*uLen*/) override;
	void  endType3Char(GfxState* /*state*/) override;
	void  type3D0(GfxState* /*state*/, double /*wx*/, double /*wy*/) override;
	void  type3D1(GfxState* /*state*/, double /*wx*/, double /*wy*/, double /*llx*/, double /*lly*/, double /*urx*/, double /*ury*/) override;

private:
	void setFillAndStrokeForPDF(GfxState* state, PageItem* text_node);
	void updateTextPos(GfxState* state) override;
	void renderTextFrame();
	void finishItem(PageItem* item);
	PdfTextRecognition m_pdfTextRecognition = {};
};
#endif
