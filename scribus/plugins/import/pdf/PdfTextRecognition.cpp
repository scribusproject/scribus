/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "PdfTextRecognition.h"


PdfTextRecognition::PdfTextRecognition()
{
	m_textRegions.push_back(activeTextRegion);
	setCharMode(AddCharMode::ADDFIRSTCHAR);
}

PdfTextRecognition::~PdfTextRecognition()
{
}

void PdfTextRecognition::addTextRegion()
{
	activeTextRegion = TextRegion();
	m_textRegions.push_back(activeTextRegion);
	setCharMode(PdfTextRecognition::AddCharMode::ADDFIRSTCHAR);
}

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


bool PdfTextRecognition::isNewLineOrRegion(QPointF newPosition)
{
	return (activeTextRegion.collinear(activeTextRegion.lastXY.y(), activeTextRegion.textRegionLines.back().baseOrigin.y()) &&
		!activeTextRegion.collinear(newPosition.y(), activeTextRegion.lastXY.y()))
		|| (activeTextRegion.collinear(newPosition.y(), activeTextRegion.lastXY.y())
			&& !activeTextRegion.isCloseToX(newPosition.x(), activeTextRegion.lastXY.x()));
}

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
PdfGlyph PdfTextRecognition::AddCharCommon(GfxState* state, double x, double y, double dx, double dy, Unicode const* u, int uLen)
{
	//qDebug() << "AddBasicChar() '" << u << " : " << uLen;
	PdfGlyph newGlyph;
	newGlyph.dx = dx;
	newGlyph.dy = dy;

	// Convert the character to UTF-16 since that's our SVG document's encoding
	if (uLen > 1)
		qDebug() << "AddBasicChar() '" << u << " : " << uLen;
	newGlyph.code = static_cast<char16_t>(u[uLen - 1]);


	newGlyph.rise = state->getRise();
	return newGlyph;
}

PdfGlyph PdfTextRecognition::AddBasicChar(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen)
{
	PdfGlyph newGlyph = AddCharCommon(state, x, y, dx, dy, u, uLen);
	activeTextRegion.lastXY = QPointF(x, y);
	activeTextRegion.glyphs.push_back(newGlyph);
	return newGlyph;
}

/*TODO: Currently not implemented, just stub code*/
PdfGlyph PdfTextRecognition::AddCharWithNewStyle(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen)
{
	//qDebug() << "AddCharWithNewStyle() '" << u << " : " << uLen;
	auto newGlyph = AddCharCommon(state, x, y, dx, dy, u, uLen);
	activeTextRegion.glyphs.push_back(newGlyph);
	return newGlyph;
}

/*TODO: Currently not implemented, just stub code*/
PdfGlyph PdfTextRecognition::AddCharWithPreviousStyle(GfxState* state, double x, double y, double dx, double dy, double originX, double originY, CharCode code, int nBytes, Unicode const* u, int uLen)
{
	//qDebug() << "AddCharWithPreviousStyle() '" << u << " : " << uLen;
	auto newGlyph = AddCharCommon(state, x, y, dx, dy, u, uLen);
	activeTextRegion.glyphs.push_back(newGlyph);
	return newGlyph;
}


/*
In geometry, collinearity of a set of points is the property of their lying on a single line. A set of points with this property is said to be collinear.
In greater generality, the term has been used for aligned objects, that is, things being "in a line" or "in a row".
 PDF never deviates from the line when it comes to collinear, but allow for 1pixel of divergence
*/
bool TextRegion::collinear(qreal a, qreal b)
{
	return abs(a - b) < 1 ? true : false;
}

// like collinear but we allow a deviation of up to +-2 rejoin font text widths
bool TextRegion::isCloseToX(qreal x1, qreal x2)
{
	//FIXME: This should use the char width not linespacing which is y
	return (abs(x2 - x1) <= lineSpacing * 6) || (abs(x1 - this->textRegioBasenOrigin.x()) <= lineSpacing);
}

// like collinear but we allow a deviation of up to 3 rejoin font linespaces, but in one direction and half a line space in the other direction
bool TextRegion::isCloseToY(qreal y1, qreal y2)
{
	//FIXME: Actually test the correct magnitudes not the abs value. There should be a parameter in the UI to set the matching tolerance but hard code to allow 3 linespaces difference before we say that the y is out of scope.
	return (y2 - y1) >= 0 && y2 - y1 <= lineSpacing * 3;
}

// less than the last y value but bot more than the line spacing less, could also use the base line of the last line to be more accurate
bool TextRegion::adjunctLesser(qreal testY, qreal lastY, qreal baseY)
{
	return (testY > lastY
		&& testY <= baseY + lineSpacing
		&& lastY <= baseY + lineSpacing);
}

// less than the last y value but bot more than the line spacing less, could also use the base line of the last line to be more accurate
bool TextRegion::adjunctGreater(qreal testY, qreal lastY, qreal baseY)
{
	return (testY <= lastY
		&& testY >= baseY - lineSpacing * 0.75
		&& lastY != baseY);
}
/*
Test to see if the point is part of the current block of text or is part of a new block of text(FrameworkLineTests::FAIL).
checks to see if it's the first point, on the same line, super and sub script, returning to baseline from super script or if we are on a new line.
matching is fuzzy allowing for multiple linespaces and text indentation. right hand justifications still needs to be dealt with as well as identifying if we are on a new paragraph
tests are weaker if we are on the first and moving to the second lines of text because we don't have enough information about how the text in the region
is formatted and in those cases the linespace is taken to be twice the glyph width.
FIXME: This needs fixing when font support is added and the ascending and descending values for the font should be used instead of the glyphs width.
*/
TextRegion::LineType TextRegion::linearTest(QPointF point, bool xInLimits, bool yInLimits)
{
	if (collinear(point.y(), lastXY.y()))
		if (collinear(point.x(), lastXY.x()))
			return LineType::FIRSTPOINT;
		else if (xInLimits)
			return LineType::SAMELINE;
	else if (adjunctLesser(point.y(), lastXY.y(), lineBaseXY.y()))
		return LineType::STYLESUPERSCRIPT;	
	else if (adjunctGreater(point.y(), lastXY.y(), lineBaseXY.y()))
		if (collinear(point.y(), lineBaseXY.y()))
			return LineType::STYLENORMALRETURN;
		else
			return LineType::STYLESUPERSCRIPT;
	else if(isCloseToX(point.x(), textRegioBasenOrigin.x()) && isCloseToY(point.y(), lastXY.y()))				
				if ((textRegionLines.size() >= 2))
					return LineType::NEWLINE;
				else if (textRegionLines.size() == 1)
					return LineType::NEWLINE;
		qDebug() << "NEWLINE2 oops:" << point << ":" << textRegioBasenOrigin << ":" << lineSpacing;
	return LineType::FAIL;
}

// Just perform some basic checks to see if newPoint can reasonably be ascribed to the current textframe.
TextRegion::LineType TextRegion::isRegionConcurrent(QPointF newPoint)
{
	if (glyphs.empty())
	{
		lineBaseXY = newPoint;
		lastXY = newPoint;
	}

	//TODO: I need to write down which ones we want so I can work it all out, for now just some basic fuzzy matching support.
	bool xInLimits = isCloseToX(newPoint.x(), lastXY.x());
	bool yInLimits = isCloseToY(newPoint.y(), lastXY.y());
	LineType pass = linearTest(newPoint, xInLimits, yInLimits);
	return pass;
}


TextRegion::LineType TextRegion::moveToPoint(QPointF newPoint)
{
	//qDebug() << "moveToPoint: " << newPoint;

	// Do some initialization if we are in a new text region
	if (glyphs.empty())
	{
		lineBaseXY = newPoint;
		lastXY = newPoint;
	}

	//TODO: I need to write down which ones we want so I can work it all out, for now just some basic fuzzy matching support.
	//TODO: x limiting should be different for moving as opposed to adding a new glyph because moving is due to a discontinuity in glyphs
	LineType mode = isRegionConcurrent(newPoint);
	//TODO: need to check to see if we are creating a new paragraph or not. basically if the cursor is returned to x origin before it reached x width. this could be returned as part of a matrix by linearTest that specifies exactly how the test ws passed. maybew return an enum with either the mode that passed or a failure value
	if (mode == LineType::FAIL)
		return mode;

	//create new lines and line segments depending upon the mode of the movement.
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

//TODO:, extract some font heights instead of using dx all the time
TextRegion::LineType TextRegion::addGlyphAtPoint(QPointF newGlyphPoint, PdfGlyph newGlyph)
{
	QPointF movedGlyphPoint = QPointF(newGlyphPoint.x() + newGlyph.dx, newGlyphPoint.y() + newGlyph.dy);
	//FIXME: There should be no need for testing the scope when adding a new glyph because move to should have been called first but leave it in for now to catch any errors in the logic
	if (glyphs.size() == 1)
	{
		// FIXME: do a propper lookup of the height
		lineSpacing = newGlyph.dx * 2;
		lastXY = newGlyphPoint;
		lineBaseXY = newGlyphPoint;
	}
	// TODO: add and move may want different versions of isCloseToX, but for now use the same generic function for both
	LineType mode = isRegionConcurrent(newGlyphPoint);
	if (mode == LineType::FAIL)
		return mode;

	maxHeight = abs(textRegioBasenOrigin.y() - movedGlyphPoint.y()) + lineSpacing > maxHeight ? abs(textRegioBasenOrigin.y() - movedGlyphPoint.y()) + lineSpacing : maxHeight;
	//move to deals with setting newlines and segments, all we have to do is populate them with the parameters the glyph gives us such as it's width and height and set the glyph index for the newlines and segments

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

void TextRegion::renderToTextFrame(PageItem* textNode)
{
	// nothing clever, just get all the body text in one lump and update the text frame
	textNode->setWidthHeight(this->maxWidth, this->maxHeight);
	QString bodyText = "";
	for (int glyphIndex = this->textRegionLines.begin()->glyphIndex; glyphIndex <= this->textRegionLines.back().segments.back().glyphIndex; glyphIndex++)
		bodyText += glyphs[glyphIndex].code;

	textNode->itemText.insertChars(bodyText);
	textNode->frameTextEnd();
}

bool TextRegion::isNew()
{
	return textRegionLines.empty() ||
		glyphs.empty();
}
