/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pdftextrecognition.h"

constexpr double PdfTextRegion::boundingBoxShape[32];

/*
*	constructor, initialize the textRegions vector and set the addChar mode
*/
PdfTextRecognition::PdfTextRecognition()
{
	m_pdfTextRegions.push_back(PdfTextRegion());
	activePdfTextRegion = &(m_pdfTextRegions.back());
	setCharMode(AddCharMode::ADDFIRSTCHAR);
}

/*
*	nothing to do in the destructor yet
*/
PdfTextRecognition::~PdfTextRecognition()
{
}

/*
*	add a new text region and make it the active region
*/
void PdfTextRecognition::addPdfTextRegion()
{
	m_pdfTextRegions.push_back(PdfTextRegion());
	activePdfTextRegion = &(m_pdfTextRegions.back());
	setCharMode(PdfTextRecognition::AddCharMode::ADDFIRSTCHAR);
}

/*
*	function called via integration with poppler's addChar callback. It decides how to add the charter based on the mode that is set
*/
void PdfTextRecognition::addChar(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, POPPLER_CONST_082 Unicode* u, int uLen)
{

	switch (this->m_addCharMode)
	{
	case AddCharMode::ADDFIRSTCHAR:
		AddFirstChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
		break;
	case AddCharMode::ADDBASICCHAR:
		AddBasicChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
		break;
	case AddCharMode::ADDCHARWITHNEWSTYLE:
		AddCharWithNewStyle(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
		break;
	case AddCharMode::ADDCHARWITHPREVIOUSSTYLE:
		AddCharWithPreviousStyle(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
		break;
	}
}

/*
*	basic test to see if the point lies in a new line or region
*/
bool PdfTextRecognition::isNewLineOrRegion(QPointF newPosition)
{
	return (activePdfTextRegion->collinear(activePdfTextRegion->lastXY.y(), activePdfTextRegion->pdfTextRegionLines.back().baseOrigin.y()) &&
		!activePdfTextRegion->collinear(newPosition.y(), activePdfTextRegion->lastXY.y()))
		|| (activePdfTextRegion->collinear(newPosition.y(), activePdfTextRegion->lastXY.y())
			&& !activePdfTextRegion->isCloseToX(newPosition.x(), activePdfTextRegion->lastXY.x()));
}


/*
*	basic functionality to be performed when addChar is called
*	FIXME: what to do when uLen != 1
*/
PdfGlyph PdfTextRecognition::AddCharCommon(GfxState* state, double x, double y, double dx, double dy, Unicode const* u, int uLen)
{
	const double * ctm = state->getCTM();
	QTransform trans(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	QPointF charDelta1 = trans.map(QPointF(0.0, 0.0));
	QPointF charDelta2 = trans.map(QPointF(dx, dy));
	QPointF charDelta = charDelta2 - charDelta1;

	//qDebug() << "AddBasicChar() '" << u << " : " << uLen;
	PdfGlyph newGlyph;
	newGlyph.dx = charDelta.x();
	newGlyph.dy = charDelta.y();

	// Convert the character to UTF-16 since that's our SVG document's encoding

	if (uLen > 1)
		qDebug() << "FIXME: AddBasicChar() '" << u << " : " << uLen;
	newGlyph.code = static_cast<char16_t>(u[uLen - 1]);
	newGlyph.rise = state->getRise();
	return newGlyph;
}

/*
*	Tell the text region to add a glyph so that line segments and regions be created
*	If the character being added is the first character in a textregion or after a change in positioning or styles or the end of a line
*	The success == TextRegion::LineType::FAIL test is an invariant test that should never pass. if a rogue glyph is detected then it means there is a bug in the logic probably in TextRegion::addGlyphAtPoint or TextRegion::linearTest or TextRegion::moveToPoint
*/
PdfGlyph PdfTextRecognition::AddFirstChar(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen)
{
	//qDebug() << "AddFirstChar() '" << u << " : " << uLen;
	PdfGlyph newGlyph = PdfTextRecognition::AddCharCommon(state, x, y, dx, dy, u, uLen);
	activePdfTextRegion->glyphs.push_back(newGlyph);
	setCharMode(AddCharMode::ADDBASICCHAR);

	//only need to be called for the very first point
	const double * ctm = state->getCTM();
	QTransform trans(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);
	QPointF glyphPos = trans.map(QPointF(x, y));

	auto success = activePdfTextRegion->addGlyphAtPoint(glyphPos, newGlyph);
	if (success == PdfTextRegion::LineType::FAIL)
		qDebug("FIXME: Rogue glyph detected, this should never happen because the cursor should move before glyphs in new regions are added.");
	return newGlyph;
}

/*
*	just add a character to the textregion without doing anything special
*/
PdfGlyph PdfTextRecognition::AddBasicChar(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen)
{
	const double * ctm = state->getCTM();
	QTransform trans(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);

	PdfGlyph newGlyph = AddCharCommon(state, x, y, dx, dy, u, uLen);
	activePdfTextRegion->lastXY = trans.map(QPointF(x, y));
	activePdfTextRegion->glyphs.push_back(newGlyph);
	return newGlyph;
}

/*
*	Apply a new style to this glyph ands glyphs that follow and add it to the style stack
*	TODO: Currently not implemented, just stub code
*/
PdfGlyph PdfTextRecognition::AddCharWithNewStyle(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen)
{
	//qDebug() << "AddCharWithNewStyle() '" << u << " : " << uLen;
	auto newGlyph = AddCharCommon(state, x, y, dx, dy, u, uLen);
	activePdfTextRegion->glyphs.push_back(newGlyph);
	return newGlyph;
}

/*
*	return to the previous style on the style stack
*	TODO: Currently not implemented, just stub code
*/
PdfGlyph PdfTextRecognition::AddCharWithPreviousStyle(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen)
{
	//qDebug() << "AddCharWithPreviousStyle() '" << u << " : " << uLen;
	auto newGlyph = AddCharCommon(state, x, y, dx, dy, u, uLen);
	activePdfTextRegion->glyphs.push_back(newGlyph);
	return newGlyph;
}

/*
*	functions to do fuzzy testing on the proximity of points to one another and in relation to the textregion
*	FIXME: There should be a parameter in the UI to set the matching tolerance but hard code for now
*/

/*
*	In geometry, collinearity of a set of points is the property of their lying on a single line. A set of points with this property is said to be collinear.
*	In greater generality, the term has been used for aligned objects, that is, things being "in a line" or "in a row".
*	PDF never deviates from the line when it comes to collinear, but allow for 1pixel of divergence
*/
bool PdfTextRegion::collinear(qreal a, qreal b)
{
	return abs(a - b) < 1 ? true : false;
}

/*
*	like collinear but we allow a deviation of 6 text widths from between positions or 1 text width from the textregion's x origin
*   FIXME: This should use the char width not linespacing which is y
*/
bool PdfTextRegion::isCloseToX(qreal x1, qreal x2)
{
	return (abs(x2 - x1) <= lineSpacing * 6) || (abs(x1 - this->pdfTextRegionBasenOrigin.x()) <= lineSpacing);
}

/*
*	like collinear but we allow a deviation of 3 text heights downwards but none upwards
*/
bool PdfTextRegion::isCloseToY(qreal y1, qreal y2)
{
	return (y2 - y1) >= 0 && y2 - y1 <= lineSpacing * 3;
}

/*
*	less than, page upwards, the last y value but bot more than the line spacing less, could also use the base line of the last line to be more accurate
*/
bool PdfTextRegion::adjunctLesser(qreal testY, qreal lastY, qreal baseY)
{
	return (testY > lastY
		&& testY <= baseY + lineSpacing
		&& lastY <= baseY + lineSpacing);
}

/*
*	greater, page downwards, than the last y value but not more than 3/4 of a line space below baseline
*/
bool PdfTextRegion::adjunctGreater(qreal testY, qreal lastY, qreal baseY)
{
	return (testY <= lastY
		&& testY >= baseY - lineSpacing * 0.75
		&& lastY != baseY);
}

/*
*	Test to see if the point is part of the current block of text or is part of a new block of text(FrameworkLineTests::FAIL).
*	checks to see if it's the first point, on the same line, super and sub script, returning to baseline from super script or if we are on a new line.
*	matching is fuzzy allowing for multiple linespaces and text indentation. right hand justifications still needs to be dealt with as well as identifying if we are on a new paragraph
*	tests are weaker if we are on the first and moving to the second lines of text because we don't have enough information about how the text in the region
*	is formatted and in those cases the linespace is taken to be twice the glyph width.
*	FIXME: This needs fixing when font support is added and the ascending and descending values for the font should be used instead of the glyphs width.
*	TODO: support LineType::STYLESUBSCRIPT
*	TODO: support NEWLINE new paragraphs with multiple linespaces and indented x insteads of just ignoring the relative x position
*	TODO: I don't know if the invariant qDebug cases should always report an error or only do so when DEBUG_TEXT_IMPORT is defined. My feeling is they should always report because it meanms something has happened that shouldn't have and it's useful feedback.
*/
PdfTextRegion::LineType PdfTextRegion::linearTest(QPointF point, bool xInLimits, bool yInLimits)
{
	if (collinear(point.y(), lastXY.y()))
	{
		if (collinear(point.x(), lastXY.x()))
			return LineType::FIRSTPOINT;
		else if (xInLimits)
			return LineType::SAMELINE;
#ifdef DEBUG_TEXT_IMPORT
		else
			qDebug() << "FIRSTPOINT/SAMELINE oops:" << "point:" << point << " pdfTextRegioBasenOrigin:" << pdfTextRegionBasenOrigin << " baseline:" << this->lineBaseXY << " lastXY:" << lastXY << " linespacing:" << lineSpacing << " pdfTextRegionLines.size:" << pdfTextRegionLines.size();
#endif
	}
	else if (adjunctLesser(point.y(), lastXY.y(), lineBaseXY.y()))
		return LineType::STYLESUPERSCRIPT;
	else if (adjunctGreater(point.y(), lastXY.y(), lineBaseXY.y()))
	{
		if (collinear(point.y(), lineBaseXY.y()))
			return LineType::STYLENORMALRETURN;
		else
			return LineType::STYLESUPERSCRIPT;
	}
	else if (isCloseToX(point.x(), pdfTextRegionBasenOrigin.x()))
	{
		if (isCloseToY(lastXY.y(), point.y()) && !collinear(point.y(), lastXY.y()))
		{
			if (pdfTextRegionLines.size() >= 2)
				return LineType::NEWLINE;
			else if (pdfTextRegionLines.size() == 1)
				return LineType::NEWLINE;
#ifdef DEBUG_TEXT_IMPORT
			else
				qDebug() << "NEWLINE oops2:" << "point:" << point << " pdfTextRegionBasenOrigin:" << pdfTextRegionBasenOrigin << " baseline:" << this->lineBaseXY << " lastXY:" << lastXY << " linespacing:" << lineSpacing << " pdfTextRegionLines.size:" << pdfTextRegionLines.size();
#endif
		}
#ifdef DEBUG_TEXT_IMPORT
		else
			qDebug() << "NEWLINE oops:" << "point:" << point << " pdfTextRegioBasenOrigin:" << pdfTextRegionBasenOrigin << " baseline:" << this->lineBaseXY << " lastXY:" << lastXY << " linespacing:" << lineSpacing << " textPdfRegionLines.size:" << pdfTextRegionLines.size();
#endif
	}
#ifdef DEBUG_TEXT_IMPORT //This isn't an invariant case like the others, we actually expect this to happen some of the time
	qDebug() << "FAILED with oops:" << "point:" << point << " pdfTextRegioBasenOrigin:" << pdfTextRegionBasenOrigin << " baseline:" << this->lineBaseXY << " lastXY:" << lastXY << " linespacing:" << lineSpacing << " textPdfRegionLines.size:" << pdfTextRegionLines.size();
#endif
	return LineType::FAIL;
}

/*
*	Perform some fuzzy checks to see if newPoint can reasonably be ascribed to the current textframe.
*	FIXME: It may be that move and addGlyph need different versions of isCloseToX and isCloseToY but keep them the same just for now
*/
PdfTextRegion::LineType PdfTextRegion::isRegionConcurrent(QPointF newPoint)
{
	if (glyphs.empty())
	{
		lineBaseXY = newPoint;
		lastXY = newPoint;
	}

	bool xInLimits = isCloseToX(newPoint.x(), lastXY.x());
	bool yInLimits = isCloseToY(lastXY.y(), newPoint.y());
	LineType pass = linearTest(newPoint, xInLimits, yInLimits);
	return pass;
}

/*
*	Move the position of the cursor to a new point,
*	test if that point is within the current textframe or within a new textframe.
*	initialize the textregion and setup lines and segments
*	TODO: iscloseto x and y may need to be different from addGlyph but use the common isRegionbConcurrent for now
*		need to check to see if we are creating a new paragraph or not.
*		basically if the cursor is returned to x origin before it reached x width.
*		Also needs to have support for rotated text, but I expect I'll add this by removing the text rotation
*		from calls to movepoint and addGlyph and instead rotating the whole text region as a block
*/
PdfTextRegion::LineType PdfTextRegion::moveToPoint(QPointF newPoint)
{
	//qDebug() << "moveToPoint: " << newPoint;

	if (glyphs.empty())
	{
		lineBaseXY = newPoint;
		lastXY = newPoint;
	}
	LineType mode = isRegionConcurrent(newPoint);
	if (mode == LineType::FAIL)
		return mode;

	PdfTextRegionLine* pdfTextRegionLine = nullptr;
	if (mode == LineType::NEWLINE || mode == LineType::FIRSTPOINT)
	{
		if (mode != LineType::FIRSTPOINT || pdfTextRegionLines.empty())
			pdfTextRegionLines.push_back(PdfTextRegionLine());

		pdfTextRegionLine = &pdfTextRegionLines.back();
		pdfTextRegionLine->baseOrigin = newPoint;
		if (mode == LineType::NEWLINE)
		{
			pdfTextRegionLine->maxHeight = abs(newPoint.y() - lastXY.y());
			if (pdfTextRegionLines.size() == 2)
				lineSpacing = abs(newPoint.y() - lastXY.y()) + 1;
		}
	}

	pdfTextRegionLine = &pdfTextRegionLines.back();
	if ((mode == LineType::FIRSTPOINT && pdfTextRegionLine->segments.empty()) || mode == LineType::NEWLINE
		|| mode != LineType::FIRSTPOINT && pdfTextRegionLine->segments[0].glyphIndex != pdfTextRegionLine->glyphIndex)
	{
		PdfTextRegionLine newSegment = PdfTextRegionLine();
		pdfTextRegionLine->segments.push_back(newSegment);
	}
	PdfTextRegionLine* segment = &pdfTextRegionLine->segments.back();
	segment->baseOrigin = newPoint;
	segment->maxHeight = (mode == LineType::STYLESUPERSCRIPT) ?
		abs(lineSpacing - (newPoint.y() - lastXY.y())) :
		pdfTextRegionLines.back().maxHeight;

	if (mode != LineType::NEWLINE && mode != LineType::FIRSTPOINT)
	{
		pdfTextRegionLines.back().segments.back().width = abs(pdfTextRegionLines.back().segments.back().baseOrigin.x() - newPoint.x());
		pdfTextRegionLine = &pdfTextRegionLines.back();
		pdfTextRegionLine->width = abs(pdfTextRegionLine->baseOrigin.x() - newPoint.x());
	}

	maxHeight = abs(pdfTextRegionBasenOrigin.y() - newPoint.y()) > maxHeight ? abs(pdfTextRegionBasenOrigin.y() - newPoint.y()) : maxHeight;
	lastXY = newPoint;

	return mode;
}

/*
*	Add a new glyph to the current line segment, lines and segments should already have been setup by the
*	moveto function which should generally be called prior to addGlyph to setup the lines and segments correctly.
*	does some basic calculations to determine and save widths and heights and linespacings of texts etc...
*	FIXME: these need to be changed to use the mode average of all glyps added to the text frame instead of just picking the first ones we come across
*		the mode average can also be used to determine the base font style when fonts are added
*		left and right hand margins however need to use the maximum and minimum, support for right hand justification
*		and centered text needs to be added as we only support left and fully justified at the moment.
*		Approximated heights and widths and linespaces need to use the correct font data when font support has been added,
*		but for now just use the x advance value. using font data should also allow for the support of rotated text that may use a mixture of x and y advance
*/
PdfTextRegion::LineType PdfTextRegion::addGlyphAtPoint(QPointF newGlyphPoint, PdfGlyph newGlyph)
{
	QPointF movedGlyphPoint = QPointF(newGlyphPoint.x() + newGlyph.dx, newGlyphPoint.y() + newGlyph.dy);
	if (glyphs.size() == 1)
	{
		lineSpacing = newGlyph.dx * 3;
		lastXY = newGlyphPoint;
		lineBaseXY = newGlyphPoint;
	}

	LineType mode = isRegionConcurrent(newGlyphPoint);
	if (mode == LineType::FAIL)
		return mode;

	maxHeight = abs(pdfTextRegionBasenOrigin.y() - movedGlyphPoint.y()) + lineSpacing > maxHeight ? abs(pdfTextRegionBasenOrigin.y() - movedGlyphPoint.y()) + lineSpacing : maxHeight;

	PdfTextRegionLine* pdfTextRegionLine = &pdfTextRegionLines.back();
	if (mode == LineType::NEWLINE || mode == LineType::FIRSTPOINT)
	{
		pdfTextRegionLine->glyphIndex = glyphs.size() - 1;
		pdfTextRegionLine->baseOrigin = QPointF(pdfTextRegionBasenOrigin.x(), newGlyphPoint.y());
	}

	PdfTextRegionLine* segment = &pdfTextRegionLine->segments.back();
	segment->width = abs(movedGlyphPoint.x() - segment->baseOrigin.x());
	segment->glyphIndex = glyphs.size() - 1;
	qreal thisHeight = pdfTextRegionLines.size() > 1 ?
		abs(newGlyphPoint.y() - pdfTextRegionLines[pdfTextRegionLines.size() - 2].baseOrigin.y()) :
		newGlyph.dx;

	segment->maxHeight = thisHeight > segment->maxHeight ? thisHeight : segment->maxHeight;
	pdfTextRegionLine->maxHeight = pdfTextRegionLine->maxHeight > thisHeight ? pdfTextRegionLine->maxHeight : thisHeight;
	pdfTextRegionLine->width = abs(movedGlyphPoint.x() - pdfTextRegionLine->baseOrigin.x());

	maxWidth = pdfTextRegionLine->width > maxWidth ? pdfTextRegionLine->width : maxWidth;
	if (pdfTextRegionLine->segments.size() == 1)
		lineBaseXY = pdfTextRegionLine->baseOrigin;

	lastXY = movedGlyphPoint;

	return mode;
}

/*
*	Render the text region to the frame,
*	nothing clever for now, just apply the whole block of text to the textNode
*	TODO: Add support for fonts and styles based on line segments
*	add support for rotated text
*/
void PdfTextRegion::renderToTextFrame(PageItem* textNode)
{
	textNode->setWidthHeight(this->maxWidth, this->maxHeight);
	QString bodyText = "";
	for (int glyphIndex = this->pdfTextRegionLines.begin()->glyphIndex; glyphIndex <= this->pdfTextRegionLines.back().segments.back().glyphIndex; glyphIndex++)
		bodyText += glyphs[glyphIndex].code;

	textNode->itemText.insertChars(bodyText);
	textNode->frameTextEnd();
}

/*
*	Quick test to see if this is a virgin textregion
*/
bool PdfTextRegion::isNew()
{
	return pdfTextRegionLines.empty() ||
		glyphs.empty();
}


PdfTextOutputDev::PdfTextOutputDev(ScribusDoc* doc, QList<PageItem*>* Elements, QStringList* importedColors, int flags) : SlaOutputDev(doc, Elements, importedColors, flags)
{
	// Nothing to do at the moment
}

PdfTextOutputDev::~PdfTextOutputDev()
{
	// Nothing to do at the moment
}

/*
 *	Updates current text position and move to a position and or add a new glyph at the previous position.
 *	NOTE: The success == TextRegion::LineType::FAIL test is an invariant test that should never pass. if a rogue glyph is detected then it means there is a bug in the logic probably in TextRegion::addGlyphAtPoint or TextRegion::linearTest or TextRegion::moveToPoint
 *	FIXME: render the textframe, this should be done after the document has finished loading the current page so all the layout fix-ups can be put in-place first
 *	FIXME: textRegion needs to support moveBackOneGlyph instead of my manual implementation in this function.
 */
void PdfTextOutputDev::updateTextPos(GfxState* state)
{
	const double * ctm = state->getCTM();
	QTransform trans(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]);

	QPointF newPosition = trans.map(QPointF(state->getCurX(), state->getCurY()));
	PdfTextRegion *activePdfTextRegion = m_pdfTextRecognition.activePdfTextRegion;

	if (activePdfTextRegion->isNew())
	{
		activePdfTextRegion->pdfTextRegionBasenOrigin = newPosition;
		m_pdfTextRecognition.setCharMode(PdfTextRecognition::AddCharMode::ADDFIRSTCHAR);
	}
	else
	{
		// if we've will move to a new line or new text region then update the current text region with the last glyph, this ensures all textlines and textregions have terminating glyphs.
		if (m_pdfTextRecognition.isNewLineOrRegion(newPosition))
		{
			QPointF glyphPosition = activePdfTextRegion->lastXY;
			activePdfTextRegion->lastXY.setX(activePdfTextRegion->lastXY.x() - activePdfTextRegion->glyphs.back().dx);
			if (activePdfTextRegion->addGlyphAtPoint(glyphPosition, activePdfTextRegion->glyphs.back()) == PdfTextRegion::LineType::FAIL)
				qDebug("FIXME: Rogue glyph detected, this should never happen because the cursor should move before glyphs in new regions are added.");
#ifdef DEBUG_TEXT_IMPORT
			else
				qDebug() << "Newline should be next";
#endif
		}
	}
	PdfTextRegion::LineType linePdfTestResult = activePdfTextRegion->moveToPoint(newPosition);
	if (linePdfTestResult == PdfTextRegion::LineType::FAIL)
	{
#ifdef DEBUG_TEXT_IMPORT
		qDebug("updateTextPos: renderPdfTextFrame() + m_pdfTextRecognition.addPdfTextRegion()");
#endif
		renderTextFrame();
		m_pdfTextRecognition.addPdfTextRegion();
		updateTextPos(state);
	}
}
/*
*	render the textregion to a new PageItem::TextFrame, currently some hackjish defaults have been implemented there are a number of FIXMEs and TODOs
*	FIXME: Paragraphs need to be implemented properly  this needs to be applied to the charstyle of the default pstyle
*	FIXME xcord and ycord need to be set properly based on GfxState and the page transformation matrix
*	TODO: Implement paragraph styles
*	TODO: Implement character styles and fonts.
*	TODO Decide if we should be setting the clipshape of the POoLine values as is the case with other import implementations
*/
void PdfTextOutputDev::renderTextFrame()
{
	//qDebug() << "_flushText()    m_doc->currentPage()->xOffset():" << m_doc->currentPage()->xOffset();
	auto activePdfTextRegion = m_pdfTextRecognition.activePdfTextRegion;
	if (activePdfTextRegion->glyphs.empty())
		return;

	qreal xCoor = m_doc->currentPage()->xOffset() + activePdfTextRegion->pdfTextRegionBasenOrigin.x();
	qreal yCoor = m_doc->currentPage()->yOffset() + activePdfTextRegion->pdfTextRegionBasenOrigin.y() - activePdfTextRegion->lineSpacing; // don't know if y is top down or bottom up
#ifdef DEBUG_TEXT_IMPORT
	qDebug() << "rendering new frame at:" << xCoor << "," << yCoor << " With lineheight of: " << activePdfTextRegion->lineSpacing << "Height:" << activePdfTextRegion->maxHeight << " Width:" << activePdfTextRegion->maxWidth;
#endif
	int z = m_doc->itemAdd(PageItem::TextFrame, PageItem::Rectangle, xCoor, yCoor, 40, 40, 0, CommonStrings::None, CommonStrings::None);
	PageItem* textNode = m_doc->Items->at(z);

	ParagraphStyle pStyle;
	pStyle.setLineSpacingMode(pStyle.AutomaticLineSpacing);
	pStyle.setHyphenationMode(pStyle.AutomaticHyphenation);

	CharStyle& cStyle = pStyle.charStyle();
	cStyle.setScaleH(1000.0);
	cStyle.setScaleV(1000.0);
	cStyle.setHyphenChar(SpecialChars::BLANK.unicode());

	// #16567: Override user prefs
	textNode->setColumns(1);

	Selection tempSelection(nullptr, false);
	tempSelection.addItem(textNode);
	m_doc->itemSelection_ApplyParagraphStyle(pStyle, &tempSelection);
	finishItem(textNode);

	//_setFillAndStrokeForPdf(state, text_node);
	textNode->ClipEdited = true;
	textNode->FrameType = 3;
	textNode->setLineEnd(m_lineEnd);
	textNode->setLineJoin(m_lineJoin);
	textNode->setTextFlowMode(PageItem::TextFlowDisabled);
	textNode->setLineTransparency(1.0);
	textNode->setFillColor(CommonStrings::None);
	textNode->setLineColor(CommonStrings::None);
	textNode->setLineWidth(0.0);
	textNode->setFillShade(m_graphicStack.top().fillShade);


	/* Oliver Stieber 2020-06-11 Set text matrix... This need to be done so that the global world view that we rite out glyphs to is transformed correctly by the context matrix for each glyph, possibly anyhow.
	needs the way in which we are handling transformations for the page to be more concrete before this code can be implemented either here or somewhere else
	FIXME: Setting the text matrix isn't supported at the moment
	QTransform text_transform(_text_matrix);
	text_transform.setMatrix(text_transform.m11(), text_transform.m12(), 0,
		text_transform.m21(), text_transform.m22(), 0,
		first_glyph.position.x(), first_glyph.position.y(), 1);
	gchar *transform = sp_svg_transform_write(text_transform);
	text_node->setAttribute("transform", transform);
	g_free(transform);
	*/

	/*
	* This code sets the font and style in a very simplistic way, it's been commented out as it needs to be updated to be used within PdfTextRecognition &co.
	int shade = 100;
	QString CurrColorText = getColor(state->getFillColorSpace(), state->getFillColor(), &shade);
	applyTextStyleToCharStyle(pStyle.charStyle(), _glyphs[0].style->getFont().family(), CurrColorText, _glyphs[0].style->getFont().pointSizeF());// *_font_scaling);
	*/

	textNode->itemText.setDefaultStyle(pStyle);
	textNode->invalid = true;
	activePdfTextRegion->renderToTextFrame(textNode);
	textNode->itemText.insertChars(SpecialChars::PARSEP, true);

	/*
	*	This code can be used to set PoLine instead of setting the FrameShape if setting the PoLine is the more correct way of doing things.
	*	I have no idea of what the PoLine is at this time except for it changes when the shape is set and appears to be unit scales as opposed to percentage scaled
	FPointArray boundingBoxShape;
	boundingBoxShape.resize(0);
	boundingBoxShape.svgInit();
	//doubles to create a shape, it's 100% textframe width by 100% textframe height

	boundingBoxShape.svgMoveTo(TextRegion::boundingBoxShape[0], TextRegion::boundingBoxShape[1]);
	for (int a = 0; a < 16; a += 2)
	{
		boundingBoxShape.append(FPoint(TextRegion::boundingBoxShape[a * 2], TextRegion::boundingBoxShape[a * 2 + 1]));
	}
	boundingBoxShape.scale(textNode->width() / 100.0, textNode->height() / 100.0);
	*/
	textNode->SetFrameShape(32, PdfTextRegion::boundingBoxShape);
	textNode->ContourLine = textNode->PoLine.copy();

	m_Elements->append(textNode);
	if (m_groupStack.count() != 0)
	{
		m_groupStack.top().Items.append(textNode);
		applyMask(textNode);
	}
}

/*
*	code mostly taken from importodg.cpp which also supports some line styles and more fill options etc...
*/
void PdfTextOutputDev::finishItem(PageItem* item)
{
	item->ClipEdited = true;
	item->FrameType = 3;
	/*code can be enabled when PoLine is set or when the shape is set as that sets PoLine
	FPoint wh = getMaxClipF(&item->PoLine);
	item->setWidthHeight(wh.x(), wh.y());
	item->Clip = flattenPath(item->PoLine, item->Segments);
	*/
	item->OldB2 = item->width();
	item->OldH2 = item->height();
	item->updateClip();
	item->OwnPage = m_doc->OnPage(item);
}

void PdfTextOutputDev::drawChar(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, POPPLER_CONST_082 Unicode* u, int uLen)
{
	// TODO Implement the clipping operations. At least the characters are shown.
	int textRenderingMode = state->getRender();
	// Invisible or only used for clipping
	if (textRenderingMode == 3)
		return;
	if (textRenderingMode < 8)
	{
		m_pdfTextRecognition.addChar(state, x, y, dx, dy, originX, originY, code, nBytes, u, uLen);
	}
}

void PdfTextOutputDev::beginTextObject(GfxState* state)
{
	pushGroup();
	if (!m_pdfTextRecognition.activePdfTextRegion->pdfTextRegionLines.empty())
	{
#ifdef DEBUG_TEXT_IMPORT
		qDebug("beginTextObject: m_textRecognition.addTextRegion()");
#endif
		m_pdfTextRecognition.addPdfTextRegion();
	}
}

void PdfTextOutputDev::endTextObject(GfxState* state)
{
	auto* activePdfTextRegion = m_pdfTextRecognition.activePdfTextRegion;
	if (!activePdfTextRegion->pdfTextRegionLines.empty() && activePdfTextRegion->glyphs.size() > 0)
	{
		// Add the last glyph to the textregion
		QPointF glyphXY = activePdfTextRegion->lastXY;
		activePdfTextRegion->lastXY.setX(activePdfTextRegion->lastXY.x() - activePdfTextRegion->glyphs.back().dx);
		if (activePdfTextRegion->addGlyphAtPoint(glyphXY, activePdfTextRegion->glyphs.back()) == PdfTextRegion::LineType::FAIL)
		{
			qDebug("FIXME: Rogue glyph detected, this should never happen because the cursor should move before glyphs in new regions are added.");
		}
#ifdef DEBUG_TEXT_IMPORT
		qDebug("endTextObject: renderTextFrame");
#endif
		renderTextFrame();
	}
	else if (!activePdfTextRegion->pdfTextRegionLines.empty())
		qDebug("FIXME:Rogue textblock");

	m_pdfTextRecognition.setCharMode(PdfTextRecognition::AddCharMode::ADDFIRSTCHAR);

	SlaOutputDev::endTextObject(state);
}

/*
* update the font for the next block of glyphs.
* just a stub for now
*/
void PdfTextOutputDev::updateFont(GfxState* state)
{

}
/*
* NOTE: Override these for now and do nothing so they don't get picked up and rendered as vectors by the base class,
	though in the long run we may actually want that unless they can be implemented in a similar way to the text import getChar in which case overloading the makes perfect sense.
*/
GBool PdfTextOutputDev::beginType3Char(GfxState* state, double x, double y, double dx, double dy, CharCode code, POPPLER_CONST_082 Unicode* u, int uLen)
{
	//stub
	return gTrue;
}
void  PdfTextOutputDev::endType3Char(GfxState* state)
{
	//stub
}
void  PdfTextOutputDev::type3D0(GfxState* state, double wx, double wy)
{
	//stub
}
void  PdfTextOutputDev::type3D1(GfxState* state, double wx, double wy, double ll, double lly, double urx, double ury)
{
	//stub
}
