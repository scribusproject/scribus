/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "pdftextrecognition.h"

#ifndef DEBUG_TEXT_IMPORT
	#define DEBUG_TEXT_IMPORT
#endif

/*
*	constructor, initialize the textRegions vector and set the addChar mode
*/
PdfTextRecognition::PdfTextRecognition()
{
	m_textRegions.push_back(activeTextRegion);
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
void PdfTextRecognition::addTextRegion()
{
	activeTextRegion = TextRegion();
	m_textRegions.push_back(activeTextRegion);
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
	return (activeTextRegion.collinear(activeTextRegion.lastXY.y(), activeTextRegion.textRegionLines.back().baseOrigin.y()) &&
		!activeTextRegion.collinear(newPosition.y(), activeTextRegion.lastXY.y()))
		|| (activeTextRegion.collinear(newPosition.y(), activeTextRegion.lastXY.y())
			&& !activeTextRegion.isCloseToX(newPosition.x(), activeTextRegion.lastXY.x()));
}


/*
*	basic functionality to be performed when addChar is called
*	FIXME: what to do when uLen != 1
*/
PdfGlyph PdfTextRecognition::AddCharCommon(GfxState* state, double x, double y, double dx, double dy, Unicode const* u, int uLen)
{
	//qDebug() << "AddBasicChar() '" << u << " : " << uLen;
	PdfGlyph newGlyph;
	newGlyph.dx = dx;
	newGlyph.dy = dy;

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
	activeTextRegion.glyphs.push_back(newGlyph);
	setCharMode(AddCharMode::ADDBASICCHAR);

	//only need to be called for the very first point
	auto success = activeTextRegion.addGlyphAtPoint(QPointF(x, y), newGlyph);
	if (success == TextRegion::LineType::FAIL)
		qDebug("FIXME: Rogue glyph detected, this should never happen because the cursor should move before glyphs in new regions are added.");
	return newGlyph;
}

/*
*	just add a character to the textregion without doing anything special
*/
PdfGlyph PdfTextRecognition::AddBasicChar(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen)
{
	PdfGlyph newGlyph = AddCharCommon(state, x, y, dx, dy, u, uLen);
	activeTextRegion.lastXY = QPointF(x, y);
	activeTextRegion.glyphs.push_back(newGlyph);
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
	activeTextRegion.glyphs.push_back(newGlyph);
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
	activeTextRegion.glyphs.push_back(newGlyph);
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
bool TextRegion::collinear(qreal a, qreal b)
{
	return abs(a - b) < 1 ? true : false;
}

/*
*	like collinear but we allow a deviation of 6 text widths from between positions or 1 text width from the textregion's x origin
*   FIXME: This should use the char width not linespacing which is y
*/
bool TextRegion::isCloseToX(qreal x1, qreal x2)
{
	
	return (abs(x2 - x1) <= lineSpacing * 6) || (abs(x1 - this->textRegioBasenOrigin.x()) <= lineSpacing);
}

/*
*	like collinear but we allow a deviation of 3 text heights downwards but none upwards
*/
bool TextRegion::isCloseToY(qreal y1, qreal y2)
{	
	return (y2 - y1) >= 0 && y2 - y1 <= lineSpacing * 3;	
}

/*
*	less than, page upwards, the last y value but bot more than the line spacing less, could also use the base line of the last line to be more accurate
*/
bool TextRegion::adjunctLesser(qreal testY, qreal lastY, qreal baseY)
{
	return (testY > lastY
		&& testY <= baseY + lineSpacing
		&& lastY <= baseY + lineSpacing);
}

/*
*	greater, page downwards, than the last y value but not more than 3/4 of a line space below baseline
*/
bool TextRegion::adjunctGreater(qreal testY, qreal lastY, qreal baseY)
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
TextRegion::LineType TextRegion::linearTest(QPointF point, bool xInLimits, bool yInLimits)
{
	if (collinear(point.y(), lastXY.y()))
		if (collinear(point.x(), lastXY.x()))
			return LineType::FIRSTPOINT;
		else if (xInLimits)
			return LineType::SAMELINE;
		#ifdef DEBUG_TEXT_IMPORT
		else
			qDebug() << "FIRSTPOINT/SAMELINE oops:" << "point:" << point << " textRegioBasenOrigin:" << textRegioBasenOrigin << " baseline:" << this->lineBaseXY << " lastXY:" << lastXY << " linespacing:" << lineSpacing << " textRegionLines.size:" << textRegionLines.size();
		#endif
	else if (adjunctLesser(point.y(), lastXY.y(), lineBaseXY.y()))
		return LineType::STYLESUPERSCRIPT;
	else if (adjunctGreater(point.y(), lastXY.y(), lineBaseXY.y()))
		if (collinear(point.y(), lineBaseXY.y()))
			return LineType::STYLENORMALRETURN;
		else
			return LineType::STYLESUPERSCRIPT;
	else if (isCloseToX(point.x(), textRegioBasenOrigin.x()))
		if (isCloseToY(point.y(), lastXY.y()) && !collinear(point.y(), lastXY.y()))
			if (textRegionLines.size() >= 2)
				return LineType::NEWLINE;
			else if (textRegionLines.size() == 1)
				return LineType::NEWLINE;
			#ifdef DEBUG_TEXT_IMPORT
			else				
			qDebug() << "NEWLINE oops2:" << "point:" << point << " textRegioBasenOrigin:" << textRegioBasenOrigin << " baseline:" << this->lineBaseXY << " lastXY:" << lastXY << "linespacing:" << lineSpacing << "textRegionLines.size:" << textRegionLines.size() << " textRegionLines[textRegionLines.size() - 2].width:" << textRegionLines[textRegionLines.size() - 2].width << " maxWidth:" << maxWidth;
			#endif
		#ifdef DEBUG_TEXT_IMPORT
		else
			qDebug() << "NEWLINE oops:" << "point:" << point << " textRegioBasenOrigin:" << textRegioBasenOrigin << " baseline:" << this->lineBaseXY << " lastXY:" << lastXY << "linespacing:" << lineSpacing << "textRegionLines.size:" << textRegionLines.size();
		#endif
	#ifdef DEBUG_TEXT_IMPORT //This isn't an invariant case like the others, we actually expect this to happen some of the time
	qDebug() << "FAILED with oops:" << "point:" << point << " textRegioBasenOrigin:" << textRegioBasenOrigin << " baseline:" << this->lineBaseXY <<" lastXY:"<< lastXY << " linespacing:" << lineSpacing << " textRegionLines.size:" << textRegionLines.size();
	#endif
	return LineType::FAIL;
}

/*
*	Perform some fuzzy checks to see if newPoint can reasonably be ascribed to the current textframe.
*	FIXME: It may be that move and addGlyph need different versions of isCloseToX and isCloseToY but keep them the same just for now
*/
TextRegion::LineType TextRegion::isRegionConcurrent(QPointF newPoint)
{
	if (glyphs.empty())
	{
		lineBaseXY = newPoint;
		lastXY = newPoint;
	}

	bool xInLimits = isCloseToX(newPoint.x(), lastXY.x());
	bool yInLimits = isCloseToY(newPoint.y(), lastXY.y());
	LineType pass = linearTest(newPoint, xInLimits, yInLimits);
	return pass;
}

/*
*	Move the position of the cursor to a new point,
*	test if that point is within the current textframe or within a new textframe.
*	initialize the textregion and setup lines and segments
*	TODO: iscloseto x and y may need to be different from addGlyph but use thge common isRegionbConcurrent for now
*		need to check to see if we are creating a new paragraph or not.
*		basically if the cursor is returned to x origin before it reached x width.
*		Also needs to have support for rotated text, but I expect I'll add this by removing the text rotation
*		from calls to movepoint and addGlyph and instead rotating the whole text region as a block
*/
TextRegion::LineType TextRegion::moveToPoint(QPointF newPoint)
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

	TextRegionLine* textRegionLine = nullptr;
	if (mode == LineType::NEWLINE || mode == LineType::FIRSTPOINT)
	{
		if (mode != LineType::FIRSTPOINT || textRegionLines.empty())
			textRegionLines.push_back(TextRegionLine());

		textRegionLine = &textRegionLines.back();
		textRegionLine->baseOrigin = newPoint;
		if (mode == LineType::NEWLINE)
		{
			textRegionLine->maxHeight = abs(newPoint.y() - lastXY.y());
			if (textRegionLines.size() == 2)
				lineSpacing = abs(newPoint.y() - lastXY.y()) + 1;
		}
	}

	textRegionLine = &textRegionLines.back();
	if ((mode == LineType::FIRSTPOINT && textRegionLine->segments.empty()) || mode == LineType::NEWLINE
		|| mode != LineType::FIRSTPOINT && textRegionLine->segments[0].glyphIndex != textRegionLine->glyphIndex)
	{
		TextRegionLine newSegment = TextRegionLine();
		textRegionLine->segments.push_back(newSegment);
	}
	TextRegionLine* segment = &textRegionLine->segments.back();
	segment->baseOrigin = newPoint;
	segment->maxHeight = (mode == LineType::STYLESUPERSCRIPT) ?
		abs(lineSpacing - (newPoint.y() - lastXY.y())) :
		textRegionLines.back().maxHeight;

	if (mode != LineType::NEWLINE && mode != LineType::FIRSTPOINT)
	{
		textRegionLines.back().segments.back().width = abs(textRegionLines.back().segments.back().baseOrigin.x() - newPoint.x());
		textRegionLine = &textRegionLines.back();
		textRegionLine->width = abs(textRegionLine->baseOrigin.x() - newPoint.x());
	}

	maxHeight = abs(textRegioBasenOrigin.y() - newPoint.y()) > maxHeight ? abs(textRegioBasenOrigin.y() - newPoint.y()) : maxHeight;
	lastXY = newPoint;

	return mode;
}

/*
*	Add a new glyph to the current line segment, lines and segments should already have been setup by the
*	moveto function which should generally be called prior to addGlyph to setup the lines and segments correctly.
*	does some basic calculations to determine and save withs and heights and linespacings of texts etc...
*	FIXME: these need to be changed to use the mode average of all glyps added to the text frame instead of just picking the first ones we come accross
*		the mode average can also be used to determine the base font style when fonts are added
*		left and right hand margins however need to use the maximum and minimum, support for right hand justification
*		and centered text needs to be added as we only support left and fully justified at the moment.
*		Approximated heights and widths and linespaces need to use the correct font data when font support has been added,
*		but for now just use the x advance value. using font data should also allow for the support of rotated text that may use a mixture of x and y advance
*/
TextRegion::LineType TextRegion::addGlyphAtPoint(QPointF newGlyphPoint, PdfGlyph newGlyph)
{
	QPointF movedGlyphPoint = QPointF(newGlyphPoint.x() + newGlyph.dx, newGlyphPoint.y() + newGlyph.dy);
	if (glyphs.size() == 1)
	{
		lineSpacing = newGlyph.dx * 3;
		lastXY = newGlyphPoint;
		lineBaseXY = newGlyphPoint;
	} else if (textRegionLines.size() == 1)
		lineSpacing = maxWidth * 3;

	LineType mode = isRegionConcurrent(newGlyphPoint);
	if (mode == LineType::FAIL)
		return mode;

	maxHeight = abs(textRegioBasenOrigin.y() - movedGlyphPoint.y()) + lineSpacing > maxHeight ? abs(textRegioBasenOrigin.y() - movedGlyphPoint.y()) + lineSpacing : maxHeight;

	TextRegionLine* textRegionLine = &textRegionLines.back();
	if (mode == LineType::NEWLINE || mode == LineType::FIRSTPOINT)
	{
		textRegionLine->glyphIndex = glyphs.size() - 1;
		textRegionLine->baseOrigin = QPointF(textRegioBasenOrigin.x(), newGlyphPoint.y());
	}

	TextRegionLine* segment = &textRegionLine->segments.back();
	segment->width = abs(movedGlyphPoint.x() - segment->baseOrigin.x());
	segment->glyphIndex = glyphs.size() - 1;
	qreal thisHeight = textRegionLines.size() > 1 ?
		abs(newGlyphPoint.y() - textRegionLines[textRegionLines.size() - 2].baseOrigin.y()) :
		newGlyph.dx;

	segment->maxHeight = thisHeight > segment->maxHeight ? thisHeight : segment->maxHeight;
	textRegionLine->maxHeight = textRegionLine->maxHeight > thisHeight ? textRegionLine->maxHeight : thisHeight;
	textRegionLine->width = abs(movedGlyphPoint.x() - textRegionLine->baseOrigin.x());

	maxWidth = textRegionLine->width > maxWidth ? textRegionLine->width : maxWidth;
	if (textRegionLine->segments.size() == 1)
		lineBaseXY = textRegionLine->baseOrigin;

	lastXY = movedGlyphPoint;

	return mode;
}

/*
*	Render the text region to the frame,
*	nothing clever for now, just apply the whole block of text to the textNode
*	TODO: Add support for fonts and styles based on line segments
*	add support for rotated text
*/
void TextRegion::renderToTextFrame(PageItem* textNode)
{
	textNode->setWidthHeight(this->maxWidth, this->maxHeight);
	QString bodyText = "";
	for (int glyphIndex = this->textRegionLines.begin()->glyphIndex; glyphIndex <= this->textRegionLines.back().segments.back().glyphIndex; glyphIndex++)
		bodyText += glyphs[glyphIndex].code;

	textNode->itemText.insertChars(bodyText);
	textNode->frameTextEnd();
}

/*
*	Quick test to see if this is a virgin textregion
*/
bool TextRegion::isNew()
{
	return textRegionLines.empty() ||
		glyphs.empty();
}
