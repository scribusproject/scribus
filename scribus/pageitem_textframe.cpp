/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem.cpp  -  description
                             -------------------
    begin                : Sat Apr 7 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QPolygon>
#include <QList>
#include <QMatrix>
#include <QPoint>
#include <QRegion>
#include <QPalette>
#include <cmath>
#include <cassert>

#include "canvas.h"
#include "commonstrings.h"
#include "guidemanager.h"
#include "hyphenator.h"
#include "mpalette.h"
#include "page.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "selection.h"
#include "text/nlsconfig.h"
#include "undomanager.h"
#include "undostate.h"
#include "util.h"
#include "util_math.h"


using namespace std;

PageItem_TextFrame::PageItem_TextFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::TextFrame, x, y, w, h, w2, fill, outline)
{
	invalid = true;
	firstChar = 0;
	cursorBiasBackward = false;
	unicodeTextEditMode = false;
	unicodeInputCount = 0;
	unicodeInputString = "";
}


static QRegion itemShape(PageItem* docItem, double xOffset, double yOffset)
{
	QRegion res;
	QMatrix pp;
//	pp.begin(view->viewport());
	pp.translate(docItem->xPos() - xOffset, docItem->yPos() - yOffset);
	pp.rotate(docItem->rotation());
	if (docItem->textFlowUsesBoundingBox())
	{
		QPolygon tcli(4);
		tcli.setPoint(0, QPoint(0,0));
		tcli.setPoint(1, QPoint(qRound(docItem->width()), 0));
		tcli.setPoint(2, QPoint(qRound(docItem->width()), qRound(docItem->height())));
		tcli.setPoint(3, QPoint(0, qRound(docItem->height())));
//		res = QRegion(pp.xForm(tcli));
		res = QRegion(pp.map(tcli));
	}
	else if ((docItem->textFlowUsesImageClipping()) && (docItem->imageClip.size() != 0))
	{
		QList<uint> Segs;
		QPolygon Clip2 = FlattenPath(docItem->imageClip, Segs);
//		res = QRegion(pp.xForm(Clip2));
		res = QRegion(pp.map(Clip2)).intersect(QRegion(pp.map(docItem->Clip)));
	}
	else if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
	{
		QList<uint> Segs;
		QPolygon Clip2 = FlattenPath(docItem->ContourLine, Segs);
//		res = QRegion(pp.xForm(Clip2));
		res = QRegion(pp.map(Clip2));
	}
	else
//		res = QRegion(pp.xForm(docItem->Clip));
		res = QRegion(pp.map(docItem->Clip));
//	pp.end();
	return  res;
}

QRegion PageItem_TextFrame::availableRegion(QRegion clip)
{
	QRegion result(clip);
	if (!isEmbedded)
	{
		int LayerLev = m_Doc->layerLevelFromNumber(LayerNr);
		uint docItemsCount=m_Doc->Items->count();
		Page* Mp=0;
		Page* Dp=0;
		PageItem* docItem=0;
		int LayerLevItem;
		if (!OnMasterPage.isEmpty())
		{
			if ((savedOwnPage == -1) || (savedOwnPage >= signed(m_Doc->Pages->count())))
				return result;
			Mp = m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]);
			Dp = m_Doc->Pages->at(savedOwnPage);
			for (int a = 0; a < m_Doc->MasterItems.count(); ++a)
			{
				docItem = m_Doc->MasterItems.at(a);
				LayerLevItem = m_Doc->layerLevelFromNumber(docItem->LayerNr);
				if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev && m_Doc->layerFlow(docItem->LayerNr)))
				{
					if (docItem->textFlowAroundObject())
					{
						result = result.subtract(itemShape(docItem, Mp->xOffset() - Dp->xOffset(), Mp->yOffset() - Dp->yOffset()));
					}
				}
			} // for all masterItems
			// (JG) #6009 : disable possible interaction between master text frames and normal frames
			// which have the text flow option set
			/*if (!m_Doc->masterPageMode())
			{
				for (uint a = 0; a < docItemsCount; ++a)
				{
					docItem = m_Doc->Items->at(a);
					Mp = m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]);
					Dp = m_Doc->Pages->at(OwnPage);
					if ((docItem->textFlowAroundObject()) && (docItem->OwnPage == OwnPage))
					{
						result = result.subtract(itemShape(docItem, m_Doc->view(), Mp->xOffset() - Dp->xOffset(), Mp->yOffset() - Dp->yOffset()));
					}
				} // for all docItems
			} // if (! masterPageMode) */
		} // if (!OnMasterPage.isEmpty())
		else
		{
			for (uint a = 0; a < docItemsCount; ++a)
			{
				docItem = m_Doc->Items->at(a);
				LayerLevItem = m_Doc->layerLevelFromNumber(docItem->LayerNr);
				if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev && m_Doc->layerFlow(docItem->LayerNr)))
				{
					 if (docItem->textFlowAroundObject())
						result = result.subtract(itemShape(docItem, 0, 0));
				}
			} // for all docItems
			} // if(OnMasterPage.isEmpty()		
	} // if(!Embedded)
	return result;
}


void PageItem_TextFrame::setShadow()
{
	if (OnMasterPage.isEmpty())
		return;
	
	QString newShadow = m_Doc->masterPageMode() ? OnMasterPage : QString::number(OwnPage);
	if (newShadow != currentShadow) {
		if (currentShadow == OnMasterPage) {
			// masterpage was edited, clear all shadows
			shadows.clear();
		}
		if (!shadows.contains(newShadow)) {
			if (!shadows.contains(OnMasterPage)) {
				shadows[OnMasterPage] = itemText;
//				const ParagraphStyle& pstyle(shadows[OnMasterPage].paragraphStyle(0));
//				qDebug(QString("Pageitem_Textframe: style of master: %1 align=%2").arg(pstyle.parent()).arg(pstyle.alignment()));
//				qDebug(QString("Pageitem_Textframe: shadow itemText->%1").arg(OnMasterPage));
			}
			if (newShadow != OnMasterPage) {
				shadows[newShadow] = shadows[OnMasterPage].copy();
//				const ParagraphStyle& pstyle(shadows[newShadow].paragraphStyle(0));
//				qDebug(QString("Pageitem_Textframe: style of shadow copy: %1 align=%2").arg(pstyle.parent()).arg(pstyle.alignment()));
			}
//			qDebug(QString("Pageitem_Textframe: shadow %1<-%2").arg(newShadow).arg(OnMasterPage));
		}
		itemText = shadows[newShadow];
//		const ParagraphStyle& pstyle(itemText.paragraphStyle(0));
//		qDebug(QString("Pageitem_Textframe: style of shadow: %1 align=%2").arg(pstyle.parent()).arg(pstyle.alignment()));
		invalid = true;
		currentShadow = newShadow;
	}
}
		
#ifdef NLS_PROTO
#include "text/pageitem_textframe.cpp"
#else

static void dumpIt(const ParagraphStyle& pstyle, QString indent = QString("->"))
{
	QString db = QString("%6%1/%2 @ %3: %4--%5 linespa%6: %7 align%8")
		   .arg(pstyle.name())
		   .arg(pstyle.parent())
		   .arg( (unsigned long int) &pstyle)
		   .arg(pstyle.leftMargin())
		   .arg(pstyle.rightMargin())
		   .arg(indent)
		   .arg(pstyle.lineSpacingMode())
		   .arg(pstyle.lineSpacing())
		   .arg(pstyle.alignment());
	qDebug(db.toLatin1().constData());
	static QString more("  ");
	if (pstyle.hasParent())
		dumpIt(*dynamic_cast<const ParagraphStyle*>(pstyle.parentStyle()), more + indent);
}


static const bool legacy = true;

/*
static void layoutDropCap(GlyphLayout layout, double curX, double curY, double offsetX, double offsetY, double dropCapDrop) 
{	
}
*/

/**
 Clones the tab fill char as often as necssary after all distances are known
 */
static void fillInTabLeaders(StoryText & itemText, LineSpec & curLine)
{
	// fill in tab leaders
	double xPos = curLine.x;
	for (int ti= curLine.firstItem; ti < curLine.lastItem; ++ti)
	{
		ScText * hl = itemText.item(ti);
		if (hl->ch == SpecialChars::TAB) 
		{
			GlyphLayout * tglyph = hl->glyph.more;
			
			if (!tglyph)
				continue;
			
			const CharStyle & charStyle(itemText.charStyle(ti));
			double wt   = charStyle.font().glyphWidth(tglyph->glyph, charStyle.fontSize() * tglyph->scaleV / 10.0);
			double len  = hl->glyph.xadvance;
			int count   = static_cast<int>(len / wt);
			double sPos = -len;
			tglyph->xoffset = sPos;
//			qDebug(QString("tab leaders: %1 %2 width=%3 count=%4").arg(sPos).arg(curLine.y).arg(wt).arg(count));
			for (int cx = 1; cx < count; ++cx)
			{
				// clone fillchar
				tglyph->grow();
				*(tglyph->more) = *tglyph;
				tglyph->more->more = NULL;
				tglyph = tglyph->more;
				tglyph->xoffset =  sPos + wt * cx;
			}
		}
		xPos += hl->glyph.wide();
	}
}



enum TabStatus {
	TabNONE    = 0,
	TabLEFT    = TabNONE,
	TabRIGHT   = 1,
	TabPOINT   = 2,
	TabCOMMA   = 3,
	TabCENTER  = 4
};


/**
fields which describe what type of tab is currently active
 */
struct TabControl {
	bool   active;
	int    status;
	int    charIndex;
	double xPos;
	QChar  fillChar;
};

/**
fields which describe how the current line is placed into the frame
 */
struct LineControl {
	LineSpec line;
	int      itemsInLine;
	int      hyphenCount;
	double   colWidth;
	double   colGap;
	double   colLeft;
	double   colRight;
	int      column;
	bool     startOfCol;
	double   ascend;
	double   descend;
	double   width;
	double   xPos;
	double   yPos;
	int      breakIndex;
	double   breakXPos;

	double   maxShrink;
	double   maxStretch;
	
	
	/// remember frame dimensions and offsets
	void init(double w, double h, const MarginStruct& extra, double lCorr)
	{
		insets = extra;
		lineCorr = lCorr;
		frameWidth = w;
		frameHeight = h;
	}
	
	/// start at column 0
	void initColumns(double width, double gap)
	{
		column = 0;
		colWidth = width;
		colGap = gap;
	}
	
	/// move position to next column
	void nextColumn(double asce)
	{
		startOfCol = true;
		colLeft = (colWidth + colGap) * column + insets.Left + lineCorr;
		colRight = colLeft + colWidth;
		if (legacy)
			colRight += insets.Right + lineCorr; //???
		xPos = colLeft;
		yPos = asce + insets.Top + lineCorr + 1;
	}
	
	bool isEndOfCol(double morespace = 0)
	{
		return yPos + morespace + insets.Bottom + lineCorr > frameHeight;
	}
	
	/**
		init fields for a new line at current position
	 */
	void startLine(int first)
	{		
		itemsInLine = 0;
		line.x = xPos;
		line.y = yPos;
		line.firstItem = first;
		line.ascent = 10;
		line.descent = 0;
		line.width = 0;
		line.naturalWidth = 0;
		breakIndex = -1;
		breakXPos = 0;
		maxShrink = 0;
		maxStretch = 0;
	}
	

	/// called when glyphs are placed on the line
	void rememberShrinkStretch(QChar ch, double wide, const ParagraphStyle& style)
	{
		if (SpecialChars::isExpandingSpace(ch))
			maxShrink += (1 - style.minWordTracking()) * wide;
		else
		{
			maxShrink += (1 - style.minGlyphExtension()) * wide;
		}
		maxStretch += (style.maxGlyphExtension() - 1) * wide;
        }

	/// called when a possible break is passed
	void rememberBreak(int index, double pos)
	{
		if (pos > colRight)
		{
			// only look for the first break behind the right edge
			maxShrink = 0;
			
			// check if we already have a better break
			if (breakIndex >= 0)
			{
				double oldLooseness = qAbs(colRight - breakXPos);

				double newLooseness = pos - colRight;
				if (newLooseness >= oldLooseness)
					return;
			}
			breakXPos = colRight;
		}
		else
		{
			breakXPos = pos;
		}
		breakIndex = index;
	}
	
	/// called when a mandatory break is found
	void breakLine(const StoryText& itemText, int last)
	{
		breakIndex = last;
		breakXPos  = line.x;
		for (int j = line.firstItem; j <= last; ++j)
			if ( (itemText.item(j)->effects() & ScStyle_SuppressSpace) == 0)
				breakXPos += itemText.item(j)->glyph.wide();
	}
	
	/// use the last remembered break to set line width and itemrange
	void finishLine(double endX)
	{
		line.lastItem = breakIndex;
		line.naturalWidth = breakXPos - line.x;
		line.width = endX - line.x;
		maxShrink = maxStretch = 0;
	}
	
	
	bool isEndOfLine(double moreSpace = 0)
	{
		if (legacy)
			return xPos + insets.Right + lineCorr + moreSpace - maxShrink >= colRight;
		else
			return xPos + moreSpace - maxShrink >= colRight;
	}
	
	/// find x position to start current line
	double startOfLine(const QRegion& shape, const QMatrix& pf2, double ascent, double descent, double morespace)
	{
		QPoint pt1, pt2;
		double tmpX = xPos;
		pt1 = QPoint(static_cast<int>(ceil(tmpX)), static_cast<int>(yPos + descent));
		pt2 = QPoint(static_cast<int>(ceil(tmpX)), static_cast<int>(ceil(yPos - ascent)));
		// increase pt1/pt2 until i-beam reaches end of line
		while ((!shape.contains(pf2.map(pt1))) || (!shape.contains(pf2.map(pt2))))
		{
			tmpX++;
			if (xPos + (legacy? lineCorr + insets.Right : 0) + lineCorr + morespace >= colRight)
			{
				return tmpX;
			}
			pt1 = QPoint(static_cast<int>(ceil(tmpX)), static_cast<int>(yPos + descent));
			pt2 = QPoint(static_cast<int>(ceil(tmpX)), static_cast<int>(ceil(yPos - ascent)));
		}
		return tmpX;
	}
	
	/// find x position where this line must end
	double endOfLine(const QRegion& shape, const QMatrix& pf2, double ascent, double descent, double morespace = 0)
	{
		double EndX = floor(qMax(line.x, qMin(colRight,breakXPos) - 1));
		QPoint pt1, pt2;
		//	qDebug(QString("endx start=%1, hl is '%2'").arg(EndX).arg(hl->ch));
		do {
			EndX += 0.125;
			pt1 = QPoint(qRound(ceil(EndX + insets.Right)), static_cast<int>(yPos+descent));
			pt2 = QPoint(qRound(ceil(EndX + insets.Right)), static_cast<int>(ceil(yPos-ascent)));
		} while 
			(   (EndX + (legacy? lineCorr + insets.Right : 0) < colRight - morespace)
			  && shape.contains(pf2.map(pt1))
			  && shape.contains(pf2.map(pt2)) 
			  );
		
		return EndX;
	}
	
	
	double getLineAscent(const StoryText& itemText)
	{
		double result = 0;
		if ((itemText.text(line.firstItem) == SpecialChars::PARSEP) || (itemText.text(line.firstItem) == SpecialChars::LINEBREAK))
			result = itemText.charStyle(line.firstItem).font().ascent(itemText.charStyle(line.firstItem).fontSize() / 10.0);
		else if (itemText.object(line.firstItem) != 0)
			result = qMax(result, (itemText.object(line.firstItem)->gHeight + itemText.object(line.firstItem)->lineWidth()) * (itemText.charStyle(line.firstItem).scaleV() / 1000.0));
		else //if (itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap == 0)
			result = itemText.charStyle(line.firstItem).font().realCharAscent(itemText.text(line.firstItem), itemText.charStyle(line.firstItem).fontSize() / 10.0);
		for (int zc = 0; zc < itemsInLine; ++zc)
		{
			QChar ch = itemText.text(line.firstItem + zc);
			if (ch == SpecialChars::PAGENUMBER)
				ch = '8'; // should have highest ascender even in oldstyle
			const CharStyle& cStyle(itemText.charStyle(line.firstItem + zc));
			if ((ch == SpecialChars::TAB) || (ch == QChar(10))
				|| (ch == SpecialChars::PARSEP) || (ch == SpecialChars::NBHYPHEN)
				|| (ch == SpecialChars::COLBREAK) || (ch == SpecialChars::LINEBREAK)
				|| (ch == SpecialChars::FRAMEBREAK) || (ch.isSpace()))
				continue;
			double asce;
			if (itemText.object(line.firstItem + zc) != 0)
				asce = itemText.object(line.firstItem + zc)->gHeight + itemText.object(line.firstItem + zc)->lineWidth() * (cStyle.scaleV() / 1000.0);
			else //if (itemText.charStyle(current.line.firstItem + zc).effects() & ScStyle_DropCap == 0)
				asce = cStyle.font().realCharAscent(ch, cStyle.fontSize() / 10.0);
			//							qDebug(QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(result));
			result = qMax(result, asce);
		}
		return result;
	}
	
	double getLineHeight(const StoryText& itemText)
	{
		double result = 0;
		if (itemText.object(line.firstItem) != 0)
			result = qMax(result, (itemText.object(line.firstItem)->gHeight + itemText.object(line.firstItem)->lineWidth()) * (itemText.charStyle(line.firstItem).scaleV() / 1000.0));
		else //if (itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap == 0)
			result = itemText.charStyle(line.firstItem).font().height(itemText.charStyle(line.firstItem).fontSize() / 10.0);
		for (int zc = 0; zc < itemsInLine; ++zc)
		{
			QChar ch = itemText.text(line.firstItem+zc);
			if ((ch == SpecialChars::TAB) || (ch == QChar(10))
				|| (ch == SpecialChars::PARSEP) || (ch == SpecialChars::NBHYPHEN)
				|| (ch == SpecialChars::COLBREAK) || (ch == SpecialChars::FRAMEBREAK)
				|| (ch == SpecialChars::LINEBREAK) || (ch.isSpace()))
				continue;
			double asce;
			if (itemText.object(line.firstItem+zc) != 0)
				asce = (itemText.object(line.firstItem+zc)->gHeight + itemText.object(line.firstItem+zc)->lineWidth()) * (itemText.charStyle(line.firstItem+zc).scaleV() / 1000.0);
			else //if (itemText.charStyle(current.line.firstItem+zc).effects() & ScStyle_DropCap == 0)
				asce = itemText.charStyle(line.firstItem+zc).font().height(itemText.charStyle(line.firstItem+zc).fontSize() / 10.0);
			//					qDebug(QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(result));
			result = qMax(result, asce);
		}
		return result;
	}
	
	
private:
	double frameWidth;
	double frameHeight;
	MarginStruct insets;
	double lineCorr;
};



/// called when line length is known and line is to be justified
static void justifyLine(StoryText& itemText, LineSpec& line)
{
	
	double glyphNatural = 0;
	double spaceNatural = 0;
	double glyphExtension;
	double spaceExtension;

	const ParagraphStyle& style(itemText.paragraphStyle(line.firstItem));

	// measure natural widths for glyphs and spaces
	for (int sof = line.firstItem; sof <= line.lastItem; ++sof)
	{
		if (!SpecialChars::isExpandingSpace(itemText.text(sof)))
		{
			glyphNatural += itemText.item(sof)->glyph.wide();
		}
		else if ( (itemText.item(sof)->effects() & ScStyle_SuppressSpace) == 0)
		{
			spaceNatural += itemText.item(sof)->glyph.wide();
		}
	}

	// decision: prio 1: stretch glyph;  prio 2: stretch spaces
	
	if (line.width < spaceNatural + glyphNatural * style.minGlyphExtension() && spaceNatural > 0)
	{
		glyphExtension = style.minGlyphExtension() - 1;
		spaceExtension = (line.width - glyphNatural * (1+glyphExtension) ) / spaceNatural  - 1;
	}
	else if (line.width < spaceNatural + glyphNatural * style.maxGlyphExtension() && glyphNatural > 0)
	{
		spaceExtension = 0;
		glyphExtension = (line.width - spaceNatural) / glyphNatural  - 1;
	}
	else
	{
		glyphExtension = style.maxGlyphExtension() - 1;
		if (spaceNatural > 0)
			spaceExtension = (line.width - glyphNatural * (1+glyphExtension) ) / spaceNatural  - 1;
		else
			spaceExtension = 0;
	}
	
	double glyphScale = 1 + glyphExtension;
	
/*
	qDebug(QString("justify: line = %7 natural = %1 + %2 = %3 (%4); spaces + %5%%; min=%8; glyphs + %6%%; min=%9")
		   .arg(spaceNatural).arg(glyphNatural).arg(spaceNatural+glyphNatural).arg(line.naturalWidth)
		   .arg(spaceExtension).arg(glyphExtension).arg(line.width)
		   .arg(style.minWordTracking()).arg(style.minGlyphExtension()));
	*/
	
	// distribute whitespace on spaces and glyphs
	for (int yof = line.firstItem; yof <= line.lastItem; ++yof)
	{
		double wide = itemText.item(yof)->glyph.wide();
		if (!SpecialChars::isExpandingSpace(itemText.text(yof)))
		{
			itemText.item(yof)->glyph.last()->xadvance += wide * glyphExtension;
			GlyphLayout* glyph = &(itemText.item(yof)->glyph);
			while (glyph)
			{
				glyph->xoffset *= glyphScale;
				glyph->scaleH *= glyphScale;
				glyph = glyph->more;
			}
		}
		else if ((itemText.item(yof)->effects() & ScStyle_SuppressSpace) == 0)
		{
			itemText.item(yof)->glyph.last()->xadvance += wide * spaceExtension;
		}
	}
}


/// called when linelength is known and line is not justified
static void indentLine(StoryText& itemText, LineSpec& line, double leftIndent)
{
	if (line.naturalWidth > line.width)
	{
		justifyLine(itemText, line);
	}
	if (leftIndent > 0)
	{
		line.x += leftIndent;
		line.width -= leftIndent;
	}
}

/// calculate how much the first char should stick out to the left
static double opticalLeftMargin(const StoryText& itemText, const LineSpec& line)
{
	int b = line.firstItem;
	while (b < line.lastItem && (itemText.item(b)->effects() & ScStyle_SuppressSpace))
		   ++b;
	
	double chs = itemText.charStyle(b).fontSize() * (itemText.charStyle(b).scaleH() / 1000.0);
	QChar chr = itemText.text(b);
	double leftCorr = itemText.charStyle(b).font().realCharWidth(chr, chs / 10.0);
	if (QString("'´`").indexOf(chr) >= 0
		|| chr == QChar(0x2018) // quote 6
		|| chr == QChar(0x2019) // quote 9
		|| chr == QChar(0x201a) // lower quote 9
		|| chr == QChar(0x201b) // upper reversed 9 6
		|| chr == QChar(0x2039) // single guillemet <
		|| chr == QChar(0x203a) // single guillemet >
		)
		leftCorr *= -0.7;
	else if (QString("\"").indexOf(chr) >= 0
			 || chr == QChar(0x00ab) // guillemet <<
			 || chr == QChar(0x00bb) // guillemet >>
			 || chr == QChar(0x201c) // quote 66
			 || chr == QChar(0x201d) // quote 99
			 || chr == QChar(0x201e) // lower quote 99
			 || chr == QChar(0x201f) // upper reversed 99
			 ) 
		leftCorr *= -0.5;
	else {
		leftCorr = itemText.charStyle(b).font().charWidth(QChar(' '), chs / 10.0, chr);
		leftCorr -= itemText.charStyle(b).font().charWidth(QChar(' '), chs / 10.0);
//				double leftCorr2 = itemText.charStyle(a).font().charWidth(QChar('K'), chs / 10.0, chr);
//				leftCorr2 -= itemText.charStyle(a).font().charWidth(QChar('K'), chs / 10.0);
//				leftCorr = qMin(leftCorr, leftCorr2);
	}
	return leftCorr;
}



/// calculate how much the last char should stick out to the right
static double opticalRightMargin(const StoryText& itemText, const LineSpec& line)
{
	int b = line.lastItem;
	while (b > line.firstItem && 
		   (SpecialChars::isBreakingSpace(itemText.text(b)) || SpecialChars::isBreak(itemText.text(b)))
		   )
		--b;
	if (b >= line.firstItem) 
	{
		double chs = itemText.charStyle(b).fontSize() * (itemText.charStyle(b).scaleH() / 1000.0);
		QChar chr = (itemText.item(b)->effects() & ScStyle_SmartHyphenVisible) ? 
			QChar('-') : itemText.text(b);
		double rightCorr = itemText.charStyle(b).font().realCharWidth(chr, chs / 10.0);
		if (QString("-,.`´'~").indexOf(chr) >= 0
			|| chr == QChar(0x2018)
			|| chr == QChar(0x2019)
			|| chr == QChar(0x201a)
			|| chr == QChar(0x201b)
			|| chr == QChar(0x2039)
			|| chr == QChar(0x203a)
			|| chr == QChar(0x2032) // PRIME
			)
			rightCorr *= 0.7;
		else if (QString(";:\"").indexOf(chr) >= 0
				 || chr == QChar(0x00ab)
				 || chr == QChar(0x00bb)
				 || chr == QChar(0x201c)
				 || chr == QChar(0x201d)
				 || chr == QChar(0x201e)
				 || chr == QChar(0x201f)
				 || chr == QChar(0x2013) // EN DASH
				 || chr == QChar(0x2033) // double prime
				 )
			rightCorr *= 0.5;
		else {
			rightCorr = itemText.charStyle(b).font().charWidth(chr, chs / 10.0);
			rightCorr -= itemText.charStyle(b).font().charWidth(chr, chs / 10.0, QChar('.'));
		}
		return rightCorr;
	}
	return 0.0;
}





void PageItem_TextFrame::layout() 
{
	
	if (BackBox != NULL && BackBox->invalid) {
//		qDebug("textframe: len=%d, going back", itemText.length());
		invalid = false;
		PageItem_TextFrame* prevInChain = dynamic_cast<PageItem_TextFrame*>(BackBox);
		if (!prevInChain)
			qDebug(QString("layout(): backBox=%1 is no textframe!!").arg((ulong)BackBox).toLatin1().constData());
		else 
			BackBox->layout();
		return;
	}
	else if (!invalid && OnMasterPage.isEmpty()) {
//		qDebug(QString("textframe: len=%1, invalid=%2 OnMasterPage=%3: no relayout").arg(itemText.length()).arg(invalid).arg(OnMasterPage));
		return;
	}
	if (invalid && BackBox == NULL)
		firstChar = 0;
	
//	qDebug(QString("textframe(%1,%2): len=%3, start relayout at %4").arg(Xpos).arg(Ypos).arg(itemText.length()).arg(firstInFrame()));
//	ScribusView* view = m_Doc->view();
	QMatrix pf2;
	QPoint pt1, pt2;
	QRegion cm;
	double chs, chsd = 0;
	double oldCurY, EndX, OFs, wide, kernVal;
	QString chstr;
	ScText *hl;
	ParagraphStyle style;
	int /*ParagraphStyle::OpticalMarginType*/ opticalMargins = ParagraphStyle::OM_None;
	
	bool outs = false;
	bool fBorder = false;
	bool goNoRoom = false;
	bool goNextColumn = false;
	
	TabControl tabs;
	tabs.active    = false;     // RTab
	tabs.status    = TabNONE;   // TabCode
	tabs.charIndex = -1;        // StartRT
	tabs.xPos      = 0;         // RTabX

	QList<ParagraphStyle::TabRecord> tTabValues;
	tTabValues.clear();
	
	bool DropCmode = false;
	bool AbsHasDrop = false;
	double desc=0, asce=0, maxDY=0, desc2=0, maxDX=0;
	int DropLines = 0;
	double DropCapDrop = 0;
	
	itemText.clearLines();

	double lineCorr = 0;
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth / 2.0;
	
	// TODO: refactor this into PageItem
	MarginStruct extra;
	extra.Top = TExtra;
	extra.Left = Extra;
	extra.Right = RExtra;
	extra.Bottom = BExtra;

	LineControl current;
	current.init(Width, Height, extra, lineCorr);
	current.initColumns(columnWidth(), ColGap);
	current.hyphenCount = 0;
				 
	// dump styles
/*	
	for (int i=0; i < itemText.nrOfParagraphs(); ++i) {
		const ParagraphStyle& pstyle(itemText.paragraphStyle(itemText.endOfParagraph(i)));
		qDebug(QString("par %1:").arg(i));
		dumpIt(pstyle);
	}
	qDebug(QString("default:"));
	dumpIt(itemText.defaultStyle());
*/	
	
//	pf2.begin(view->viewport());
	pf2.translate(Xpos, Ypos);
	pf2.rotate(Rot);
	
	setShadow();
	if ((itemText.length() != 0)) // || (NextBox != 0))
	{
		// determine layout area
		QRegion cl = availableRegion(QRegion(pf2.map(Clip)));
		
		if (imageFlippedH())
		{
			pf2.translate(Width, 0);
			pf2.scale(-1, 1);
		}
		if (imageFlippedV())
		{
			pf2.translate(0, Height);
			pf2.scale(1, -1);
		}
		
		current.nextColumn(0);

		// find start of first line
		if (firstInFrame() < itemText.length())
		{
			hl = itemText.item(firstInFrame());
			style = itemText.paragraphStyle(firstInFrame());
			if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
			{
				style.setLineSpacing(style.charStyle().font().height(style.charStyle().fontSize() / 10.0));
//				qDebug(QString("auto linespacing: %1").arg(style.lineSpacing()));
			}

//			qDebug(QString("style @0: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//				   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():""));
			if (style.hasDropCap())
			{
				if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
					chs = qRound(m_Doc->typographicSettings.valueBaseGrid  * style.dropCapLines() * 10);
				else
					chs = qRound(style.lineSpacing() * style.dropCapLines() * 10);
			}
			else 
				chs = hl->fontSize();
			desc2 = -hl->font().descent(chs / 10.0);
			current.yPos = extra.Top + lineCorr;
//			qDebug(QString("first line at y=%1").arg(current.yPos));
		}
		else // empty itemText:
		{
			desc2 = -itemText.defaultStyle().charStyle().font().descent(itemText.defaultStyle().charStyle().fontSize() / 10.0);
			current.yPos = itemText.defaultStyle().lineSpacing() + extra.Top+lineCorr-desc2;
		}
		current.startLine(firstInFrame());
		outs = false;
		OFs = 0;
		MaxChars = 0;
		for (int a = firstInFrame(); a < itemText.length(); ++a)
		{
			hl = itemText.item(a);
			if (a > 0 && itemText.text(a-1) == SpecialChars::PARSEP)
				style = itemText.paragraphStyle(a);
			if (current.itemsInLine == 0)
				opticalMargins = style.opticalMargins();
			
//			qDebug(QString("style pos %1: %2 (%3)").arg(a).arg(style.alignment()).arg(style.parent()));
			const CharStyle& charStyle =  (chstr[0] != SpecialChars::PARSEP? itemText.charStyle(a) : itemText.paragraphStyle(a).charStyle());
			if (!charStyle.parent().isEmpty())
			{
//				qDebug(QString("charstyle pos %1: %2 (%3 %4 %5 %6 %7 %8 %9)").arg(a).arg(charStyle.parent())
//					   .arg((uint)charStyle.parentStyle()).arg(charStyle.font().scName()).arg(charStyle.fontSize())
//					   .arg(charStyle.fillColor()).arg(charStyle.strokeColor()).arg(charStyle.parentStyle()->name()).arg((uint)charStyle.parentStyle()->parentStyle()));
			}				
//			qDebug(QString("charstyle pos %1: %2 (%3)").arg(a).arg(charStyle.asString()).arg(charStyle.fontSize()));
//			qDebug(QString("style @%6: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//				   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent())
//				   .arg(a));

			chstr = ExpandToken(a);
//			qDebug(QString("expanded token: '%1'").arg(chstr));
			if (chstr.isEmpty())
				chstr = SpecialChars::ZWNBSPACE;
			if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
			{
				style.setLineSpacing(style.charStyle().font().height(style.charStyle().fontSize() / 10.0));
			}
			// find out about par gap and dropcap
			if (a == firstInFrame())
			{
				if (a == 0 || itemText.text(a-1) == SpecialChars::PARSEP)
				{
					current.yPos += style.gapBefore();
					if (chstr[0] != SpecialChars::PARSEP)
					{
						DropCmode = style.hasDropCap();
						if (DropCmode)
							DropLines = style.dropCapLines();
					}
					else
						DropCmode = false;
				}
			}

			{  // local block for 'fl'
				StyleFlag fl = hl->effects();
				fl &= ~ScStyle_DropCap;
				fl &= ~ScStyle_SmartHyphenVisible;
				hl->setEffects(fl);
			}

			// No space at begin of line, 
			if (legacy)
			{
				// unless at begin of par (eeks)
				if ( (current.itemsInLine == 0) && (SpecialChars::isBreakingSpace(hl->ch))
					 && (a > 0 && ! SpecialChars::isBreak(itemText.text(a-1)))
					 && ! (a > 0 && SpecialChars::isBreakingSpace(itemText.text(a-1)) 
						   && (itemText.charStyle(a-1).effects() & ScStyle_SuppressSpace) != ScStyle_SuppressSpace))
				{
					hl->setEffects(hl->effects() | ScStyle_SuppressSpace);
					hl->glyph.xadvance = 0;
					continue;
				}
				else
					hl->setEffects(hl->effects() & ~ScStyle_SuppressSpace);
			}
			else // from 134 on use NBSPACE for this effect
			{
				if ( current.itemsInLine == 0 && SpecialChars::isBreakingSpace(hl->ch))
				{
					hl->setEffects(hl->effects() | ScStyle_SuppressSpace);
					hl->glyph.xadvance = 0;
					continue;
				}
				else
					hl->setEffects(hl->effects() & ~ScStyle_SuppressSpace);
			}
			//			qDebug(QString("textframe(%1,%2): len=%3, fontsize=%4, ascent=%5").arg(Xpos).arg(Ypos).arg(itemText.length())
//					   .arg(charStyle.fontSize()).arg(charStyle.font().ascent()));				
			if (current.itemsInLine == 0)
			{
 				// more about par gap and dropcaps
				if ((a > firstInFrame() && itemText.text(a-1) == SpecialChars::PARSEP) || (a == 0 && BackBox == 0 && !current.startOfCol)) // after || always evaluates to false? FIXME
				{
//					qDebug(QString("gap before2: y=%1+%2").arg(current.yPos).arg(style.gapBefore()));
					current.yPos += style.gapBefore();
					DropCapDrop = 0;
					if (chstr[0] != SpecialChars::PARSEP)
						DropCmode = style.hasDropCap();
					else
						DropCmode = false;
					if (DropCmode)
					{
						DropLines = style.dropCapLines();
						if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
							DropCapDrop = m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
						else
						{
							if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
								DropCapDrop = style.lineSpacing() * (DropLines-1);
							else
								DropCapDrop = charStyle.font().height(style.charStyle().fontSize() / 10.0) * (DropLines-1);
						}
//						qDebug(QString("dropcapdrop: y=%1+%2").arg(current.yPos).arg(DropCapDrop));
						current.yPos += DropCapDrop;
					}
				}
			}
			// find charsize & y pos
			if (DropCmode)
			{
				// dropcap active?
				if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
					DropCapDrop = m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
				else
				{
					if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
						DropCapDrop = style.lineSpacing() * (DropLines-1);
					else
						DropCapDrop = charStyle.font().height(style.charStyle().fontSize() / 10.0) * (DropLines-1);
				}

				if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
				{
					chsd = (10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / (charStyle.font().realCharHeight(chstr[0], 1))));
					chs = (10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / charStyle.font().realCharAscent(chstr[0], 1)));
				}
				else
				{
					if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
					{
						chsd = (10 * ((style.lineSpacing() * (DropLines-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / (charStyle.font().realCharHeight(chstr[0], 1))));
						chs = (10 * ((style.lineSpacing() * (DropLines-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / charStyle.font().realCharAscent(chstr[0], 1)));
					}
					else
					{
						double currasce = charStyle.font().height(style.charStyle().fontSize() / 10.0);
						chsd = (10 * ((currasce * (DropLines-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / (charStyle.font().realCharHeight(chstr[0], 1))));
						chs = (10 * ((currasce * (DropLines-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / charStyle.font().realCharAscent(chstr[0], 1)));
					}
				}
				hl->setEffects(hl->effects() | ScStyle_DropCap);
				hl->glyph.yoffset -= DropCapDrop;
			}
			else // ! dropCapMode
			{
				if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
					chs = qRound((hl->embedded.getItem()->gHeight + hl->embedded.getItem()->lineWidth()) * 10);
				else
					chs = charStyle.fontSize();
			}
			// find tracking
			if (current.itemsInLine == 0)
			{
				itemText.item(a)->setEffects(itemText.item(a)->effects() | ScStyle_StartOfLine);
				kernVal = 0;
			}
			else
			{
				kernVal = 0; // chs * charStyle.tracking() / 10000.0;
				itemText.item(a)->setEffects(itemText.item(a)->effects() & ~ScStyle_StartOfLine);
			}
			hl->glyph.yadvance = 0;
			oldCurY = layoutGlyphs(*hl, chstr, hl->glyph);
			// find out width of char
			if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
				wide = hl->embedded.getItem()->gWidth + hl->embedded.getItem()->lineWidth();
			else
			{
				wide = hl->glyph.wide();
				// apply kerning
				if (a+1 < itemText.length())
				{
					uint glyph2 = charStyle.font().char2CMap(itemText.text(a+1));
					double kern= charStyle.font().glyphKerning(hl->glyph.glyph, glyph2, chs / 10.0) * hl->glyph.scaleH;
					wide += kern;
					hl->glyph.xadvance += kern;
				}
			}
			if (DropCmode)
			{
				// drop caps are wider...
				if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
				{
					wide = hl->embedded.getItem()->gWidth + hl->embedded.getItem()->lineWidth();
					if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						asce = m_Doc->typographicSettings.valueBaseGrid * DropLines;
					else
					{
						if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
							asce = style.lineSpacing() * DropLines;
						else
							asce = charStyle.font().height(style.charStyle().fontSize() / 10.0) * DropLines;
					}
					hl->glyph.scaleH /= hl->glyph.scaleV;
					hl->glyph.scaleV = (asce / (hl->embedded.getItem()->gHeight + hl->embedded.getItem()->lineWidth()));
					hl->glyph.scaleH *= hl->glyph.scaleV;
				}
				else
				{
					wide = charStyle.font().realCharWidth(chstr[0], chsd / 10.0);
					asce = charStyle.font().realCharHeight(chstr[0], chsd / 10.0);
//					qDebug(QString("dropcaps pre: chsd=%1 realCharHeight = %2 chstr=%3").arg(chsd).arg(asce).arg(chstr2[0]));
					hl->glyph.scaleH /= hl->glyph.scaleV;
					hl->glyph.scaleV = (asce / charStyle.font().realCharHeight(chstr[0], charStyle.fontSize() / 10.0));
					hl->glyph.scaleH *= hl->glyph.scaleV;
				}
				hl->glyph.xadvance = wide;
				desc2 = 0;
				desc = 0;
			}
			else // !DropCMode
			{
//				qDebug(QString("textframe ascent/descent: fontsize=%1, ascent=%2, descent=%3")
//					   .arg(charStyle.fontSize()).arg(charStyle.font().ascent()).arg(charStyle.font().descent()));				

				if (SpecialChars::isExpandingSpace(hl->ch))
				{
					double wordtracking = charStyle.wordTracking();
					hl->glyph.xadvance *= wordtracking;
					wide *= wordtracking;
				}
				
				// find ascent / descent
				double hlcsize10=charStyle.fontSize() / 10.0;
				if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
				{
					desc = desc2 = 0;
				}
				else
				{
					desc = desc2 = -charStyle.font().descent(hlcsize10);
					current.rememberShrinkStretch(hl->ch, wide, style);
				}
				asce = charStyle.font().ascent(hlcsize10);
//				wide = wide * hl->glyph.scaleH;
			}
			fBorder = false;
			// end of col reached?
//			qDebug(QString("eocol? %1 + %2 + %3 > %4 -- asce=%5").arg(current.yPos).arg(extra.Bottom).arg(lineCorr).arg(Height).arg(asce));
			if (current.isEndOfCol())
			{
				current.column++;
				if (current.column < Cols)
				{
					// start next col
					current.nextColumn(asce);
					if (((a > firstInFrame()) && (itemText.text(a-1) == SpecialChars::PARSEP)) 
						|| ((a == firstInFrame()) && (BackBox == 0)))
					{
						if (chstr[0] != SpecialChars::PARSEP)
							DropCmode = style.hasDropCap();
						else
							DropCmode = false;
						if (DropCmode)
						{
							if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
								desc2 = -charStyle.font().descent() * m_Doc->typographicSettings.valueBaseGrid * style.dropCapLines();
							else
								desc2 = -charStyle.font().descent() * style.lineSpacing() * style.dropCapLines();
						}
						if (DropCmode)
							DropLines = style.dropCapLines();
					}
//					qDebug(QString("layout: nextcol grid=%1 x %2").arg(style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing).arg(m_Doc->typographicSettings.valueBaseGrid));
					if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
					{
						double by = Ypos;
						if (OwnPage != -1)
							by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
						int ol1 = qRound((by + current.yPos - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
						int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
//						qDebug(QString("baseline adjust: y=%1->%2").arg(current.yPos).arg(ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by));

						current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
					}
				}
				else
				{
					MaxChars = a;
//					qDebug(QString("no room A: %1").arg(a));
					goto NoRoom;
				}
			}
			// ok, there's more room in col
			if (current.itemsInLine == 0)
			{
//				qDebug(QString("newline: '%1' %2 %3").arg(a).arg(current.yPos).arg(asce));
				// start a new line
				double TopOffset = asce;
				double BotOffset = desc2;
				goNoRoom = false;
				if (current.startOfCol)
				{
//					qDebug(QString("current.startOfCol: %1 + %2 + %3 + 1 =%4").arg(asce).arg(extra.Top).arg(lineCorr).arg(asce + extra.Top + lineCorr + 1));
					current.yPos = asce + extra.Top + lineCorr + 1;
//							if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
//								current.yPos += m_Doc->docParagraphStyles[hl->cab].gapBefore;
				}
//				qDebug(QString("layout: nextline grid=%1 x %2").arg(style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing).arg(m_Doc->typographicSettings.valueBaseGrid));
				if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
				{
					double by = Ypos;
					if (OwnPage != -1)
						by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
					int ol1 = qRound((by + current.yPos - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
					int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
//					qDebug(QString("useBaselIneGrid: %1 * %2 + %3 - %4").arg(ol2 / 10000.0).arg(m_Doc->typographicSettings.valueBaseGrid).arg(m_Doc->typographicSettings.offsetBaseGrid).arg(by));
//					qDebug(QString("baseline adjust: y=%1->%2").arg(current.yPos).arg(ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by));
					current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
				}
				/* this causes different spacing for first line:
				if (current.yPos-TopOffset < 0.0)
				{
					qDebug(QString("current.yPos - Ascender < 0 (%1 - %2 < 0)").arg(current.yPos).arg(TopOffset));
 					current.yPos = TopOffset+1;
				}
				 */
				// find linelength:
				pt1 = QPoint(static_cast<int>(ceil(current.xPos)), static_cast<int>(current.yPos+BotOffset));
				pt2 = QPoint(static_cast<int>(ceil(current.xPos)), static_cast<int>(ceil(current.yPos-TopOffset)));
				// increase pt1/pt2 until i-beam reaches end of line
//				qDebug(QString("linestart: %1 + %2 + %3 < %4").arg(current.xPos).arg(wide).arg(style.rightMargin()).arg(current.colRight));
				double leftIndent = 0;
				double xStep = legacy? 1 : 0.125;
				while (!cl.contains(pf2.map(pt1)) || !cl.contains(pf2.map(pt2)) || current.isEndOfLine(wide + leftIndent + style.rightMargin()))
				{
					fBorder = true;
					current.xPos += xStep;
					if (current.isEndOfLine(wide + leftIndent + style.rightMargin()))
					{
//						qDebug(QString("eocol %5? %1 + %2 + %3 + %4").arg(current.yPos).arg(current.startOfCol).arg(style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing).arg(style.lineSpacing()).arg(current.column));
						// new line
						fBorder = false;
						current.xPos = current.colLeft;

						leftIndent = style.leftMargin();
						if (a > 0 && itemText.text(a-1) == SpecialChars::PARSEP)
							leftIndent += style.firstIndent();

						if (current.startOfCol)
						{
//							qDebug(QString("startofcol adjust: y=%1->%2").arg(current.yPos).arg(current.yPos+1));

							current.yPos++;
						}
						else
						{
							current.yPos += style.lineSpacing();
//							qDebug(QString("next line: y=%1").arg(current.yPos));
						}
//						qDebug(QString("layout: next lower line grid=%1 x %2").arg(style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing).arg(m_Doc->typographicSettings.valueBaseGrid));
						if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						{
							double by = Ypos;
							if (OwnPage != -1)
								by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
							int ol1 = qRound((by + current.yPos - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
//							qDebug(QString("baseline adjust: y=%1->%2").arg(current.yPos).arg(ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by));
							current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
						}
						if (current.isEndOfCol())
						{
							fBorder = false;
							current.column++;
							if (current.column < Cols)
							{
								current.nextColumn(asce);
								if (((a > firstInFrame()) && (itemText.text(a-1) == SpecialChars::PARSEP)) || ((a == firstInFrame()) && (BackBox == 0)))
								{
									if (chstr[0] != SpecialChars::PARSEP)
										DropCmode = style.hasDropCap();
									else
										DropCmode = false;
									if (DropCmode)
									{
										if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
											desc2 = -charStyle.font().descent() * m_Doc->typographicSettings.valueBaseGrid * style.dropCapLines();
										else
											desc2 = -charStyle.font().descent() * style.lineSpacing() * style.dropCapLines();
									}
									if (DropCmode)
										DropLines = style.dropCapLines();
								}
//								qDebug(QString("layout: nextcol2 grid=%1 x %2").arg(style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing).arg(m_Doc->typographicSettings.valueBaseGrid));
								if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
								{
									double by = Ypos;
									if (OwnPage != -1)
										by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
									int ol1 = qRound((by + current.yPos - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
									int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
//									qDebug(QString("baseline adjust: y=%1->%2").arg(current.yPos).arg(ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by));
									current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
								}
							}
							else
							{
								MaxChars = a;
//								qDebug(QString("no room B: %1").arg(a));
								goto NoRoom;
							}
						}
					}
					pt1 = QPoint(static_cast<int>(ceil(current.xPos)), static_cast<int>(current.yPos+BotOffset));
					pt2 = QPoint(static_cast<int>(ceil(current.xPos)), static_cast<int>(ceil(current.yPos-TopOffset)));
				}

				if (((fBorder)) && (!AbsHasDrop))
					current.xPos += extra.Left;
				// indent first line of par
				if (a > 0)
				{
					if (itemText.text(a-1) == SpecialChars::PARSEP)
						current.xPos += style.firstIndent();
				}
				else
				{
					if (legacy)
						current.xPos += style.firstIndent();
				}
				// add left margin
				if (!AbsHasDrop)
					current.xPos += style.leftMargin();
				if (opticalMargins & ParagraphStyle::OM_LeftHangingPunct) {
					current.xPos += opticalLeftMargin(itemText, current.line);
				}
//				qDebug(QString("store line: (%1,%2)").arg(current.xPos).arg(current.yPos));
				current.line.x = current.xPos;
				current.line.y = current.yPos;
				fBorder = false;
			}
			else {
//				qDebug(QString("cont.' line: '%1'").arg(a));
			}
			// right tab stuff
			if (tabs.active)
			{
				if (((hl->ch == '.') && (tabs.status == TabPOINT)) || ((hl->ch == ',') && (tabs.status == TabCOMMA)) || (hl->ch == SpecialChars::TAB))
				{
					tabs.active = false;
					tabs.status = TabNONE;
				}
			}
			// tab positioning
			if (hl->ch == SpecialChars::TAB)
			{
				wide = 1;
				if (tabs.active)
					tabs.active = false;
				else // ???
				{
					tabs.xPos = current.xPos;
					tTabValues = style.tabValues();
					if (tTabValues.isEmpty())
					{
						if ((current.xPos - current.colLeft) != 0)
						{
							if (current.xPos == current.colLeft + ceil((current.xPos-current.colLeft) / m_Doc->toolSettings.dTabWidth) * m_Doc->toolSettings.dTabWidth)
								current.xPos += m_Doc->toolSettings.dTabWidth;
							else
								current.xPos = current.colLeft + ceil((current.xPos-current.colLeft) / m_Doc->toolSettings.dTabWidth) * m_Doc->toolSettings.dTabWidth;
						}
						else
							current.xPos = current.colLeft + m_Doc->toolSettings.dTabWidth;
						tabs.status = TabNONE;
						tabs.active = false;
					}
					else
					{
						double tCurX = current.xPos - current.colLeft;
						double oCurX = tCurX + wide;
						for (int yg = static_cast<int>(tTabValues.count()-1); yg > -1; yg--)
						{
							if (oCurX < tTabValues[yg].tabPosition)
							{
								tabs.status = static_cast<int>(tTabValues[yg].tabType);
								tCurX = tTabValues[yg].tabPosition;
								tabs.fillChar    = tTabValues[yg].tabFillChar;
							}
						}
						tabs.active = (tabs.status != TabLEFT);
						if (tCurX == oCurX-wide)
							current.xPos = current.colLeft + ceil((current.xPos-current.colLeft) / m_Doc->toolSettings.dTabWidth) * m_Doc->toolSettings.dTabWidth;
						else
							current.xPos = current.colLeft + tCurX;
						
						// remember fill char
//						qDebug(QString("tab: %1 '%2'").arg(tCurX).arg(tabs.fillChar));
						if (!tabs.fillChar.isNull()) {
							hl->glyph.grow();
							GlyphLayout * tglyph = hl->glyph.more;
							tglyph->glyph = charStyle.font().char2CMap(tabs.fillChar);
							tglyph->yoffset = hl->glyph.yoffset;
							tglyph->scaleV = tglyph->scaleH = chs / charStyle.fontSize();
							tglyph->xadvance = 0;
						}
					}
					hl->glyph.xadvance = current.xPos - tabs.xPos;
//					wide = current.xPos - RTabX;
					tabs.charIndex = a;
				}
			}
			
			// remember y pos
			if (DropCmode)
				hl->glyph.yoffset -= charStyle.font().realCharHeight(chstr[0], chsd / 10.0) - charStyle.font().realCharAscent(chstr[0], chsd / 10.0);

			
			// remember x pos
			double breakPos = current.xPos;
			
			if (!tabs.active) // normal case
			{
				current.xPos += wide+kernVal;
			}
			else if (tabs.active && tabs.status == TabCENTER) 	// center tab
			{
				current.xPos += (wide+kernVal) / 2;
				current.xPos = qMax(current.xPos, current.colLeft);
			}
			else // other tabs.active			
			{
				current.xPos = qMax(current.xPos, current.colLeft);
			}
			
			
			//FIXME: asce / desc set correctly?
			if (legacy && 
				(((hl->ch == '-' || (hl->effects() & ScStyle_HyphenationPossible)) && (current.hyphenCount < m_Doc->HyCount || m_Doc->HyCount == 0))  
				|| hl->ch == SpecialChars::SHYPHEN))
			{
				if (hl->effects() & ScStyle_HyphenationPossible || hl->ch == SpecialChars::SHYPHEN)
				{
					pt1 = QPoint(qRound(ceil(current.xPos+extra.Right - current.maxShrink + charStyle.font().charWidth('-', charStyle.fontSize() / 10.0) * (charStyle.scaleH() / 1000.0))), qRound(current.yPos+desc));
					pt2 = QPoint(qRound(ceil(current.xPos+extra.Right - current.maxShrink + charStyle.font().charWidth('-', charStyle.fontSize() / 10.0) * (charStyle.scaleH() / 1000.0))), qRound(ceil(current.yPos-asce)));
				}
				else
				{
					pt1 = QPoint(qRound(ceil(current.xPos+extra.Right - current.maxShrink )), qRound(current.yPos+desc));
					pt2 = QPoint(qRound(ceil(current.xPos+extra.Right - current.maxShrink )), qRound(ceil(current.yPos-asce)));
				}
			}
			else if (!legacy && SpecialChars::isBreakingSpace(hl->ch))
			{
				pt1 = QPoint(qRound(ceil(breakPos + extra.Right - current.maxShrink )), qRound(current.yPos+desc));
				pt2 = QPoint(qRound(ceil(breakPos + extra.Right - current.maxShrink )), qRound(ceil(current.yPos-asce)));
			}
			else
			{
				pt1 = QPoint(qRound(ceil(current.xPos+extra.Right - current.maxShrink)), qRound(current.yPos+desc));
				pt2 = QPoint(qRound(ceil(current.xPos+extra.Right - current.maxShrink)), qRound(ceil(current.yPos-asce)));
			}
			
			// test if end of line reached
			if ((!cl.contains(pf2.map(pt1))) || (!cl.contains(pf2.map(pt2))) || (legacy && current.isEndOfLine(style.rightMargin())))
				outs = true;
			if (current.isEndOfCol())
				outs = true;
			if ((hl->ch == SpecialChars::FRAMEBREAK) && (a < itemText.length()-1))
				goNoRoom = true;
			if ((hl->ch == SpecialChars::COLBREAK) && (Cols > 1))
				goNextColumn = true;


			// remember possible break
			if ( (SpecialChars::isBreakingSpace(hl->ch) || hl->ch == SpecialChars::TAB) && !outs)
			{
				if ( a == firstInFrame() || !SpecialChars::isBreakingSpace(itemText.text(a-1)) )
				{
					current.rememberBreak(a, breakPos);
				}
			}
			

			// hyphenation
			if (((hl->effects() & ScStyle_HyphenationPossible) || (hl->ch == '-') || hl->ch == SpecialChars::SHYPHEN) && (!outs) && !itemText.text(a-1).isSpace() )
			{
				breakPos = current.xPos;
				if (hl->ch != '-')
				{
					breakPos += charStyle.font().charWidth('-', charStyle.fontSize() / 10.0) * (charStyle.scaleH() / 1000.0);
				}
				
				double rightHang = 0;
				if (opticalMargins & ParagraphStyle::OM_RightHangingPunct) {
					rightHang = 0.7 * charStyle.font().realCharWidth('-', (charStyle.fontSize() / 10.0) * (charStyle.scaleH() / 1000.0));
				}
				
				if (legacy || (breakPos - rightHang < current.colRight - style.rightMargin()))
				{
					if ((current.hyphenCount < m_Doc->HyCount) || (m_Doc->HyCount == 0) || hl->ch == SpecialChars::SHYPHEN)
					{
						current.rememberBreak(a, breakPos);
					}
				}
			}
			
						
			++current.itemsInLine;
			
			if (tabs.active)
			{
				double cen = 1;
				if (tabs.status == TabCENTER)
					cen = 2;
				
				double newTabAdvance = itemText.item(tabs.charIndex)->glyph.xadvance - (wide+kernVal) / cen;
				
				if (newTabAdvance >= 0) {
					itemText.item(tabs.charIndex)->glyph.xadvance = newTabAdvance;
				}
				else {
					tabs.active = false;
					tabs.status = TabNONE;
				}
			}
			
			if (DropCmode)
			{
				DropCmode = false;
				AbsHasDrop = true;
				maxDY = current.yPos;
				current.xPos += style.dropCapOffset();
				hl->glyph.xadvance += style.dropCapOffset();
//				qDebug(QString("dropcapoffset: %1 -> %2").arg(current.xPos-style.dropCapOffset()).arg(current.xPos));
				current.xPos = qMax(current.xPos, current.colLeft);
				maxDX = current.xPos;
				QPolygon tcli(4);
				if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
				{
					current.yPos -= m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
					double by = Ypos;
					if (OwnPage != -1)
						by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
					int ol1 = qRound((by + current.yPos - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
					int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
//					qDebug(QString("baseline adjust after dropcaps: y=%1->%2").arg(current.yPos).arg(ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by));
					current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
					//FIXME: use current.colLeft instead of xOffset?
					tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY-DropLines*m_Doc->typographicSettings.valueBaseGrid)));
					tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*m_Doc->typographicSettings.valueBaseGrid)));
				}
				else
				{
					if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
					{
						current.yPos -= style.lineSpacing() * (DropLines-1);
//						qDebug(QString("after dropcaps: y=%1").arg(current.yPos));
						tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY - DropLines * style.lineSpacing())));
						tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*style.lineSpacing())));
					}
					else
					{
						double currasce = charStyle.font().height(style.charStyle().fontSize() / 10.0);
						current.yPos -= currasce * (DropLines-1);
//						qDebug(QString("after dropcaps: y=%1").arg(current.yPos));
						tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY-DropLines*currasce)));
						tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*currasce)));
					}
				}
				tcli.setPoint(2, QPoint(qRound(maxDX), qRound(maxDY)));
				tcli.setPoint(3, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY)));
				cm = QRegion(pf2.map(tcli));
				cl = cl.subtract(cm);
//				current.yPos = maxDY;
			}
			// end of line
			if ( SpecialChars::isBreak(hl->ch, Cols > 1) || (outs))
			{
				tabs.active = false;
				tabs.status = TabNONE;
				if (SpecialChars::isBreak(hl->ch, Cols > 1))
				{
					// find end of line
					current.breakLine(itemText, a);
					EndX = current.endOfLine(cl, pf2, asce, desc, style.rightMargin());
					current.finishLine(EndX);
					
//					if (style.alignment() != 0)
					{
						if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
							current.line.width += opticalRightMargin(itemText, current.line);

						OFs = 0;
						if (style.alignment() == 2)
							OFs = current.line.width - current.line.naturalWidth;
						if (style.alignment() == 1)
							OFs = (current.line.width - current.line.naturalWidth) / 2;
						if (style.alignment() == 3)
							OFs = 0;
						
						if (style.alignment() == 4
							|| (style.alignment() == 3 
								&&  (hl->ch == SpecialChars::LINEBREAK ||
									 hl->ch == SpecialChars::FRAMEBREAK ||
									 hl->ch == SpecialChars::COLBREAK)
								&&  !itemText.text(current.line.lastItem - 1).isSpace()))
						{
							justifyLine(itemText, current.line); 
						}
						else
						{
							if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
								current.line.naturalWidth += opticalRightMargin(itemText, current.line);
							double optiWidth = current.colRight - style.lineSpacing()/2.0 - current.line.x;
							if (current.line.naturalWidth > optiWidth)
								current.line.width = qMax(current.line.width - current.maxShrink, optiWidth);
							// simple offset
							indentLine(itemText, current.line, OFs);
						}
						current.xPos = current.colRight;
					}
				}
				else // outs -- last char went outside the columns (or into flow-around shape)
				{
					if (current.breakIndex >= 0)            // Hier koenen auch andere Trennungen eingebaut werden
					{
						// go back to last break position
//						qDebug(QString("new break pos a=%1 BuPos=%2 current.breakIndex=%3").arg(a).arg(BuPos).arg(current.breakIndex));
						a = current.breakIndex;
						assert( a >= 0 );
						assert( a < itemText.length() );
						hl = itemText.item(a);
						style = itemText.paragraphStyle(a);
						if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
						{
							style.setLineSpacing(style.charStyle().font().height(style.charStyle().fontSize() / 10.0));
//							qDebug(QString("auto linespacing: %1").arg(style.lineSpacing()));
						}
						current.itemsInLine = a - current.line.firstItem + 1;
//						qDebug(QString("style outs pos %1: %2 (%3)").arg(a).arg(style.alignment()).arg(style.parent()));
//						qDebug(QString("style <@%6: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//							   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():"")
//							   .arg(a));

						if (hl->ch == ' ') {
							hl->setEffects(hl->effects() | ScStyle_SuppressSpace);
							hl->glyph.xadvance = 0;
						}
						
						EndX = current.endOfLine(cl, pf2, asce, desc, style.rightMargin());
						current.finishLine(EndX);

//???						current.breakXPos = current.line.x;
//???						for (int j=current.line.firstItem; j <= a; ++j)
//???							current.breakXPos += itemText.item(j)->glyph.wide();
						
						if ((hl->effects() & ScStyle_HyphenationPossible) || hl->ch == SpecialChars::SHYPHEN)
						{
							// insert hyphen
							current.hyphenCount++;
							hl->setEffects(hl->effects() | ScStyle_SmartHyphenVisible);
							hl->glyph.grow();
							hl->glyph.more->glyph = charStyle.font().char2CMap(QChar('-'));
							hl->glyph.more->xadvance = charStyle.font().charWidth('-', itemText.charStyle(a).fontSize() / 10.0) * (itemText.charStyle(a).scaleH() / 1000.0);
						}
						else 
						{
							if (hl->ch != '-')
								current.hyphenCount = 0;
							hl->setEffects(hl->effects() & ~ScStyle_SmartHyphenVisible);
							hl->glyph.shrink();
						}
						
						// Justification
//						if (style.alignment() != 0)
						{
							if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
								current.line.width += opticalRightMargin(itemText, current.line);

							OFs = 0;
							if (style.alignment() == 2)
								OFs = current.line.width - current.line.naturalWidth;
							if (style.alignment() == 1)
								OFs = (current.line.width - current.line.naturalWidth) / 2;
							
							if ((style.alignment() == 3) 
								|| (style.alignment() == 4))
							{
								justifyLine(itemText, current.line);
							}
							else
							{
								if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
									current.line.naturalWidth += opticalRightMargin(itemText, current.line);
								indentLine(itemText, current.line, OFs);
							}
//							qDebug(QString("line: endx=%1 next pos=(%2,%3").arg(EndX).arg(current.line.x + current.line.width).arg(current.yPos));
							current.xPos = current.line.x + current.line.width;
						}
					}
					else  // no break position
					{
						if (a >= current.line.firstItem)
						{
							--a;
							--current.itemsInLine;
						}
						if (a >= 0)
						{
							hl = itemText.item(a);
							style = itemText.paragraphStyle(a);
							if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
							{
								style.setLineSpacing(style.charStyle().font().height(style.charStyle().fontSize() / 10.0));
//								qDebug(QString("auto linespacing: %1").arg(style.lineSpacing()));
							}
						}
						current.breakLine(itemText, a);
//						qDebug(QString("style no break pos %1: %2 (%3)").arg(a).arg(style.alignment()).arg(style.parent()));
//						qDebug(QString("style nb @%6: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//							   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent())
//							   .arg(a));
						EndX = current.endOfLine(cl, pf2, asce, desc, style.rightMargin());
						current.finishLine(EndX);
//						qDebug(QString("no break pos: %1-%2 @ %3 wid %4 nat %5 endX %6")
//							   .arg(current.line.firstItem).arg(current.line.firstItem)
//							   .arg(current.line.x).arg(current.line.width).arg(current.line.naturalWidth).arg(EndX));
						indentLine(itemText, current.line, 0);
					}
				}
				if ( outs || SpecialChars::isBreak(hl->ch, (Cols > 1)) )
				{
					if (outs && !current.isEndOfLine(hl->glyph.wide() + style.rightMargin()))
					{
						if (( hl->ch == SpecialChars::PARSEP || hl->ch == SpecialChars::LINEBREAK) 
							&& AbsHasDrop)
						{
							AbsHasDrop = false;
							if (current.yPos < maxDY)
								current.yPos = maxDY;
//							qDebug(QString("after dropcaps: y=%1 maxDY=%2").arg(current.yPos).arg(maxDY));
						}
						bool fromOut = true;
						double BotOffset = desc+extra.Bottom+lineCorr;
//						pt1 = QPoint(qRound(current.xPos+extra.Right), static_cast<int>(current.yPos+BotOffset));
//						pt2 = QPoint(qRound(current.xPos+extra.Right), static_cast<int>(ceil(current.yPos-asce)));
						while (!current.isEndOfLine(style.rightMargin()))
						{
							current.xPos++;
							if (current.isEndOfLine(style.rightMargin()))
							{
								fromOut = false;
//								qDebug(QString("layout: next lower2 grid=%1 x %2").arg(style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing).arg(m_Doc->typographicSettings.valueBaseGrid));
//								qDebug(QString("nextline: y=%1+%2").arg(current.yPos).arg(style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing? m_Doc->typographicSettings.valueBaseGrid : style.lineSpacing()));

								if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
									current.yPos += m_Doc->typographicSettings.valueBaseGrid;
								else
									current.yPos += style.lineSpacing();
								if (current.isEndOfCol(desc) && (current.column+1 == Cols))
								{
									goNoRoom = true;
									MaxChars = a + 1;
//									qDebug(QString("go no room 1: %1").arg(a));
									break;
								}
								if (AbsHasDrop)
								{
									if ((current.yPos > maxDY) && (current.yPos - asce > maxDY))
									{
										AbsHasDrop = false;
										current.xPos = current.colLeft;
									}
									else
										current.xPos = maxDX;
								}
								else
									current.xPos = current.colLeft;
								if (SpecialChars::isBreak(hl->ch))
								{
									if (hl->ch == SpecialChars::PARSEP)
									{
//										qDebug(QString("gap after: y=%1+%2").arg(current.yPos).arg(style.gapAfter()));
										current.yPos += style.gapAfter();
									}
									current.hyphenCount = 0;
								}
								break;
							}
							pt1 = QPoint(qRound(current.xPos+extra.Right), static_cast<int>(current.yPos+BotOffset));
							pt2 = QPoint(qRound(current.xPos+extra.Right), static_cast<int>(ceil(current.yPos-asce)));
							if (cl.contains(pf2.map(pt1)) && cl.contains(pf2.map(pt2)))
								break;
//							else
//								qDebug(QString("looking for start of line at %1,%2").arg(current.xPos).arg(current.yPos));
						}
						if (fromOut)
						{
							if (current.isEndOfCol(desc) && (current.column+1 == Cols))
							{
								goNoRoom = true;
								MaxChars = a + 1;
//								qDebug(QString("go no room 2: %1").arg(a));
								break;
							}
							current.xPos--;
							current.xPos = qMax(current.xPos, current.colLeft);
						}
					}
					else
					{
						if (( hl->ch == SpecialChars::PARSEP || hl->ch == SpecialChars::LINEBREAK ) 
							&& AbsHasDrop)
						{
							AbsHasDrop = false;
							if (current.yPos < maxDY)
								current.yPos = maxDY;
//							qDebug(QString("after dropcap2: y=%1+%2").arg(current.yPos).arg(maxDY));
						}
//						qDebug(QString("layout: next lower3 grid=%1 x %2").arg(style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing).arg(m_Doc->typographicSettings.valueBaseGrid));


						if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						{
//							qDebug(QString("next line (grid): y=%1+%2").arg(current.yPos).arg(m_Doc->typographicSettings.valueBaseGrid));

							current.yPos += m_Doc->typographicSettings.valueBaseGrid;
						}
						else if (a < lastInFrame() && style.lineSpacingMode() != ParagraphStyle::AutomaticLineSpacing)
						{
//							qDebug(QString("next line (fixed): y=%1+%2").arg(current.yPos).arg(itemText.paragraphStyle(a+1).lineSpacing()));
							current.yPos += itemText.paragraphStyle(a+1).lineSpacing();
						}
						else
						{
//							qDebug(QString("next line (auto): y=%1+%2").arg(current.yPos).arg(style.lineSpacing()));
							current.yPos += style.lineSpacing();
						}
						if (AbsHasDrop)
						{
							if ((current.yPos > maxDY) && (current.yPos - asce > maxDY))
							{
								AbsHasDrop = false;
								current.xPos = current.colLeft;
							}
							else
								current.xPos = maxDX;
						}
						else
							current.xPos = current.colLeft;
						if ( SpecialChars::isBreak(hl->ch) )
						{
							if (hl->ch == SpecialChars::PARSEP)
							{
//								qDebug(QString("gap after: y=%1+%2").arg(current.yPos).arg(style.gapAfter()));
								current.yPos += style.gapAfter();
							}
							current.hyphenCount = 0;
						}
					}
				}
				

				// calc. needed vertical space
				if (current.itemsInLine != 0)
				{
					if ((!AbsHasDrop) && (current.startOfCol) && (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing))
					{
						//FIXME: use glyphs, not chars
						double firstasce = itemText.charStyle(current.line.firstItem).font().ascent(itemText.charStyle(current.line.firstItem).fontSize() / 10.0);
						double currasce = current.getLineAscent(itemText);
							/*0;
						if ((itemText.text(current.line.firstItem) == SpecialChars::PARSEP) || (itemText.text(current.line.firstItem) == SpecialChars::LINEBREAK))
							currasce = itemText.charStyle(current.line.firstItem).font().ascent(itemText.charStyle(current.line.firstItem).fontSize() / 10.0);
						else if (itemText.object(current.line.firstItem) != 0)
							currasce = qMax(currasce, (itemText.object(current.line.firstItem)->gHeight + itemText.object(current.line.firstItem)->lineWidth()) * (itemText.charStyle(current.line.firstItem).scaleV() / 1000.0));
						else //if (itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap == 0)
							currasce = itemText.charStyle(current.line.firstItem).font().realCharAscent(itemText.text(current.line.firstItem), itemText.charStyle(current.line.firstItem).fontSize() / 10.0);
						for (int zc = 0; zc < current.itemsInLine; ++zc)
						{
							QChar ch = itemText.text(current.line.firstItem + zc);
							if (ch == SpecialChars::PAGENUMBER)
								ch = '8'; //should have highest ascender even in oldstyle
							const CharStyle& cStyle(itemText.charStyle(current.line.firstItem + zc));
							if ((ch == SpecialChars::TAB) || (ch == QChar(10))
								|| (ch == SpecialChars::PARSEP) || (ch == SpecialChars::NBHYPHEN)
								|| (ch == SpecialChars::COLBREAK) || (ch == SpecialChars::LINEBREAK)
								|| (ch == SpecialChars::FRAMEBREAK) || (ch.isSpace()))
								continue;
							double asce;
							if (itemText.object(current.line.firstItem + zc) != 0)
								asce = (itemText.object(current.line.firstItem + zc)->gHeight + itemText.object(current.line.firstItem + zc)->lineWidth()) * (cStyle.scaleV() / 1000.0);
							else //if (itemText.charStyle(current.line.firstItem + zc).effects() & ScStyle_DropCap == 0)
								asce = cStyle.font().realCharAscent(ch, cStyle.fontSize() / 10.0);
//							qDebug(QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(currasce));
						currasce = qMax(currasce, asce);
						}*/
						double adj = firstasce - currasce;
//						qDebug(QString("move1 line %1.. down by %2").arg(current.line.firstItem).arg(-adj));
						current.line.ascent = currasce;
						current.line.y -= adj;
						current.yPos -= adj;
					}
					else if ((!current.startOfCol) && (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing) && (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
					{
						QChar ch = itemText.text(current.line.firstItem);
						double firstasce = style.lineSpacing();
						double currasce = current.getLineHeight(itemText);
						/*0;
						if (itemText.object(current.line.firstItem) != 0)
							currasce = qMax(currasce, (itemText.object(current.line.firstItem)->gHeight + itemText.object(current.line.firstItem)->lineWidth()) * (itemText.charStyle(current.line.firstItem).scaleV() / 1000.0));
						else //if (itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap == 0)
							currasce = itemText.charStyle(current.line.firstItem).font().height(itemText.charStyle(current.line.firstItem).fontSize() / 10.0);
						for (int zc = 0; zc < current.itemsInLine; ++zc)
						{
							QChar ch = itemText.text(current.line.firstItem + zc);
							if ((ch == SpecialChars::TAB) || (ch == QChar(10))
								|| (ch == SpecialChars::PARSEP) || (ch == SpecialChars::NBHYPHEN)
								|| (ch == SpecialChars::COLBREAK) || (ch == SpecialChars::FRAMEBREAK)
								|| (ch == SpecialChars::LINEBREAK) || (ch.isSpace()))
								continue;
							double asce;
							if (itemText.object(current.line.firstItem + zc) != 0)
								asce = (itemText.object(current.line.firstItem + zc)->gHeight + itemText.object(current.line.firstItem + zc)->lineWidth()) * (itemText.charStyle(current.line.firstItem + zc).scaleV() / 1000.0);
							else //if (itemText.charStyle(current.line.firstItem + zc).effects() & ScStyle_DropCap == 0)
								asce = itemText.charStyle(current.line.firstItem + zc).font().height(itemText.charStyle(current.line.firstItem + zc).fontSize() / 10.0);
//							qDebug(QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(currasce));
							currasce = qMax(currasce, asce);
						}
						*/							
						double adj = firstasce - currasce;
//						qDebug(QString("move2 line %1.. down by %2").arg(current.line.firstItem).arg(-adj));
						current.line.ascent = currasce;
						current.line.y -= adj;
						current.yPos -= adj;
					}
					if ( itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap)
					{
						// put line back to top
						current.line.y -= DropCapDrop;
						itemText.item(current.line.firstItem)->glyph.yoffset += DropCapDrop;
					}
				}
				
				fillInTabLeaders(itemText, current.line);

				current.startOfCol = false;
/* has no effect except setting wide, asce and desc
 				uint loopC = BuPos3;
				if (m_Doc->guidesSettings.showControls)
					loopC++;  // ??? AV
				for (int zc = 0; zc < loopC; ++zc)
				{
					if ( zc >= current.itemsInLine) {
						qDebug("layout: zc too large %d / %d", zc, current.itemsInLine);
						continue;
					}
					if ( startLin + zc >= itemText.length()) {
						qDebug("layout: startLin+zc too large %d+%d / %d", startLin, zc, itemText.length());
						continue;
					}
//					else
//						qDebug("layout: startLin, zc ok. %d, %d, %d", startLin, zc, LiList.count());
					double wide2 = 0;
					double xcoZli = itemText.item(current.line.firstItem + zc)->glyph.xoffset;
//					itemText.item(startLin+zc)->glyph.xoffset = Zli2->xco;
///					itemText.item(startLin+zc)->glyph.yoffset = Zli2->yco;
					tTabValues = itemText.paragraphStyle(startLin+zc).tabValues();
					desc = - itemText.charStyle(startLin+zc).font().descent(itemText.charStyle(startLin+zc).fontSize() / 10.0);
					asce = itemText.charStyle(startLin+zc).font().ascent(itemText.charStyle(startLin+zc).fontSize() / 10.0);
					bool Sele = itemText.selected(startLin+zc);
					if (((Sele && (Select)) || (((NextBox != 0) || (BackBox != 0)) && Sele)) && (m_Doc->appMode == modeEdit))
					{
						wide = itemText.item(startLin+zc)->glyph.xadvance;
						if ((zc > 0) && (itemText.text(startLin+zc) == SpecialChars::TAB))
						{
							wide2 = itemText.item(startLin+zc-1)->glyph.xadvance;
							xcoZli = itemText.item(startLin+zc-1)->glyph.xoffset + wide2;
							wide = itemText.item(startLin+zc)->glyph.xoffset - xcoZli + itemText.item(startLin+zc)->glyph.xadvance;
						}
					}
				}
				*/
//				qDebug(QString("layout: line %1 - %2 @ (%3,%4) wd %5/%6").arg(current.line.firstItem).arg(current.line.lastItem)
//					   .arg(current.line.x).arg(current.line.y).arg(current.line.naturalWidth).arg(current.line.width));
				if (current.line.firstItem > current.line.lastItem)
					; //qDebug(QString("layout: empty line %1 - %2").arg(current.line.firstItem).arg(current.line.lastItem));
				else if (current.itemsInLine > 0)
					itemText.appendLine(current.line);
				current.startLine(a+1);
				outs = false;
				if (goNoRoom)
				{
					goNoRoom = false;
					MaxChars = a+1;
//					qDebug(QString("no room C: %1").arg(a+1));
					goto NoRoom;
				}
				if (goNextColumn)
				{
					goNextColumn = false;
					current.column++;
					if (current.column < Cols)
					{
						current.nextColumn(asce);
					}
					else
					{
						MaxChars = a;
//						qDebug(QString("no room D: %1").arg(a));
						goto NoRoom;
					}
				}
			}
		}
		if (goNoRoom)
		{
			goNoRoom = false;
			goto NoRoom;
		}
// end of itemText
		int a = itemText.length()-1;
		hl = a >=0 ? itemText.item(a) : NULL;
		current.breakLine(itemText, a);
		EndX = current.endOfLine(cl, pf2, asce, desc, style.rightMargin());
		current.finishLine(EndX);

//		if (style.alignment() != 0)
		{
			if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
			{
				current.line.width += opticalRightMargin(itemText, current.line);
			}
			
			OFs = 0;
			if (style.alignment() == 2)
				OFs = current.line.width - current.line.naturalWidth;
			if (style.alignment() == 1)
				OFs = (current.line.width - current.line.naturalWidth) / 2;
			if (style.alignment() == 3)
				OFs = 0;
			if (style.alignment() == 4
				|| (style.alignment() == 3 
					&&  (hl->ch == SpecialChars::LINEBREAK ||
						 hl->ch == SpecialChars::FRAMEBREAK ||
						 hl->ch == SpecialChars::COLBREAK)
					&&  !itemText.text(current.line.firstItem + current.itemsInLine-1).isSpace()))
			{
				justifyLine(itemText, current.line);
			}
			else
			{
				if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
					current.line.naturalWidth += opticalRightMargin(itemText, current.line);
				indentLine(itemText, current.line, OFs);
			}
		}

		if (current.itemsInLine != 0)
		{
			if ((!AbsHasDrop) && (current.startOfCol) && (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing))
			{
				double firstasce = itemText.charStyle(current.line.firstItem).font().ascent(itemText.charStyle(current.line.firstItem).fontSize() / 10.0);
				double currasce = current.getLineAscent(itemText);
				/*0;
				double asce;
				if ((itemText.text(current.line.firstItem) == SpecialChars::PARSEP) || (itemText.text(current.line.firstItem) == SpecialChars::LINEBREAK))
					asce = itemText.charStyle(current.line.firstItem).font().ascent(itemText.charStyle(current.line.firstItem).fontSize() / 10.0);
				else if (itemText.object(current.line.firstItem) != 0)
					asce = (itemText.object(current.line.firstItem)->gHeight + itemText.object(current.line.firstItem)->lineWidth()) * (itemText.charStyle(current.line.firstItem).scaleV() / 1000.0));
				else //if (itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap == 0)
					asce = itemText.charStyle(current.line.firstItem).font().realCharAscent(itemText.text(current.line.firstItem), itemText.charStyle(current.line.firstItem).fontSize() / 10.0);
				qDebug(QString("starting with ascender %1 (firstasce=%2)").arg(asce).arg(firstasce));
				currasce = asce;
				for (int zc = 0; zc < current.itemsInLine; ++zc)
				{
					QChar ch = itemText.text(current.line.firstItem+zc);
					if (ch == SpecialChars::PAGENUMBER)
						ch = '8'; // should have highest ascender even in oldstyle
					if ((ch == SpecialChars::TAB) || (ch == QChar(10))
						|| (ch == SpecialChars::PARSEP) || (ch == SpecialChars::NBHYPHEN)
						|| (ch == SpecialChars::FRAMEBREAK) || (ch == SpecialChars::COLBREAK)
						|| (ch == SpecialChars::LINEBREAK) || (ch.isSpace()))
						continue;
					if (itemText.object(current.line.firstItem+zc) != 0)
						asce = (itemText.object(current.line.firstItem+zc)->gHeight + itemText.object(current.line.firstItem+zc)->lineWidth()) * itemText.charStyle(current.line.firstItem+zc).scaleV() / 1000.0);
					else //if (itemText.charStyle(current.line.firstItem+zc).effects() & ScStyle_DropCap == 0)
						asce = itemText.charStyle(current.line.firstItem+zc).font().realCharAscent(itemText.text(current.line.firstItem+zc), itemText.charStyle(current.line.firstItem+zc).fontSize() / 10.0));
//					qDebug(QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(currasce));
					currasce = qMax(currasce, asce);
				}
				*/
				double adj = firstasce - currasce;
//				qDebug(QString("move3 line %1.. down by %2 current ascender=%3").arg(current.line.firstItem).arg(-adj).arg(currasce));

				current.line.ascent = currasce;
				current.line.y -= adj;
				current.yPos -= adj;
			}
			else if ((!current.startOfCol) && (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing) && (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
			{
				double firstasce = style.lineSpacing();
				double currasce = current.getLineHeight(itemText);
				/*0;
				if (itemText.object(current.line.firstItem) != 0)
					currasce = qMax(currasce, (itemText.object(current.line.firstItem)->gHeight + itemText.object(current.line.firstItem)->lineWidth()) * (itemText.charStyle(current.line.firstItem).scaleV() / 1000.0));
				else //if (itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap == 0)
					currasce = itemText.charStyle(current.line.firstItem).font().height(itemText.charStyle(current.line.firstItem).fontSize() / 10.0);
				for (int zc = 0; zc < current.itemsInLine; ++zc)
				{
					QChar ch = itemText.text(current.line.firstItem+zc);
					if (ch == SpecialChars::PAGENUMBER)
						ch = '8'; // should have highest ascender even in oldstyle
					if ((ch == SpecialChars::TAB) || (ch == QChar(10))
						|| (ch == SpecialChars::PARSEP) || (ch == SpecialChars::NBHYPHEN)
						|| (ch == SpecialChars::COLBREAK) || (ch == SpecialChars::FRAMEBREAK)
						|| (ch == SpecialChars::LINEBREAK) || (ch.isSpace()))
						continue;
					double asce;
					if (itemText.object(current.line.firstItem+zc) != 0)
						asce = (itemText.object(current.line.firstItem+zc)->gHeight + itemText.object(current.line.firstItem+zc)->lineWidth()) * (itemText.charStyle(current.line.firstItem+zc).scaleV() / 1000.0);
					else //if (itemText.charStyle(current.line.firstItem+zc).effects() & ScStyle_DropCap == 0)
						asce = itemText.charStyle(current.line.firstItem+zc).font().height(itemText.charStyle(current.line.firstItem+zc).fontSize() / 10.0);
//					qDebug(QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(currasce));
					currasce = qMax(currasce, asce);
				}
				*/
				double adj = firstasce - currasce;
//				qDebug(QString("move4 line %1.. down by %2").arg(current.line.firstItem).arg(-adj));

				current.line.ascent = currasce;
				current.line.y -= adj;
				current.yPos -= adj;
			}
			if ( itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap )
			{
				// put line back to top
				current.line.y -= DropCapDrop;
				itemText.item(current.line.firstItem)->glyph.yoffset += DropCapDrop;
			}
		}
		fillInTabLeaders(itemText, current.line);
		current.startOfCol = false;
		goNextColumn = false;

//		qDebug(QString("layout: last line %1 - %2 @ (%3,%4) wd %5/%6").arg(current.line.firstItem).arg(current.line.lastItem)
//			   .arg(current.line.x).arg(current.line.y).arg(current.line.naturalWidth).arg(current.line.width));
		
		if (current.itemsInLine > 0) {
			itemText.appendLine(current.line);
		}
	}
	MaxChars = itemText.length();
	invalid = false;
//	pf2.end();
	if (NextBox != NULL) {
		NextBox->invalid = true;
		dynamic_cast<PageItem_TextFrame*>(NextBox)->firstChar = MaxChars;
	}
//	qDebug("textframe: len=%d, done relayout", itemText.length());
	return;
			
NoRoom:     
//	pf2.end();
	invalid = false;
	PageItem_TextFrame * next = dynamic_cast<PageItem_TextFrame*>(NextBox);
	if (next != 0)
	{
		next->invalid = true;
		next->firstChar = MaxChars;
		if (CPos > signed(MaxChars))
		{
			int nCP = CPos;
//			CPos = MaxChars;
			if (m_Doc->appMode == modeEdit)
			{
				//							OwnPage->Deselect(true);
				next->CPos = qMax(nCP, signed(MaxChars));
				//							Doc->currentPage = NextBox->OwnPage;
				//							NextBox->OwnPage->SelectItemNr(NextBox->ItemNr);
//				qDebug("textframe: len=%d, leaving relayout in editmode && Tinput", itemText.length());
				return;
			}
		}
		// relayout next frame
//		qDebug("textframe: len=%d, going to next", itemText.length());
		next->layout();
	}
//	qDebug("textframe: len=%d, done relayout (no room %d)", itemText.length(), MaxChars);
}

void PageItem_TextFrame::invalidateLayout(bool wholeChain)
{
	this->invalid = true;
	if (wholeChain)
	{
		PageItem *prevFrame = this->prevInChain();
		while (prevFrame != 0)
		{
			prevFrame->invalid = true;
			prevFrame = prevFrame->prevInChain();
		}
		PageItem *nextFrame = this->nextInChain();
		while (nextFrame != 0)
		{
			nextFrame->invalid = true;
			nextFrame = nextFrame->nextInChain();
		}
	}
}

void PageItem_TextFrame::DrawObj_Item(ScPainter *p, QRect e, double sc)
{
	layout();
	if (invalid)
		return;
	QMatrix pf2;
	QPoint pt1, pt2;
	QRegion cm;
	double wide, lineCorr;
	QChar chstr0;
	ScText *hl;
	QString cachedStroke = "";
	QString cachedFill = "";
	double cachedFillShade = -1;
	double cachedStrokeShade = -1;
	QString actStroke = "";
	QString actFill = "";
	double actFillShade = -1;
	double actStrokeShade = -1;
	QColor cachedFillQ;
	QColor cachedStrokeQ;
//	QValueList<ParagraphStyle::TabRecord> tTabValues;
	double desc, asce, tabDist;
//	tTabValues.clear();
	p->save();
	QRect e2;
	if (isEmbedded)
		e2 = e;
	else
	{
		e2 = QRect(qRound(e.x()  / sc + m_Doc->minCanvasCoordinate.x()), qRound(e.y()  / sc + m_Doc->minCanvasCoordinate.y()), qRound(e.width() / sc), qRound(e.height() / sc));
		pf2.translate(Xpos, Ypos);
	}
	pf2.rotate(Rot);
	if (!m_Doc->layerOutline(LayerNr))
	{
		if ((fillColor() != CommonStrings::None) || (GrType != 0))
		{
			p->setupPolygon(&PoLine);
			p->fillPath();
		}
	}
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth / 2.0;
	else
		lineCorr = 0;
	if ((isAnnotation()) && (annotation().Type() == 2) && (!Pfile.isEmpty()) && (PicAvail) && (PicArt) && (annotation().UseIcons()))
	{
		p->save();
		p->setupPolygon(&PoLine);
		p->setClipPath();
		p->scale(LocalScX, LocalScY);
		p->translate(LocalX*LocalScX, LocalY*LocalScY);
		if (pixm.width() > 0 && pixm.height() > 0)
			p->drawImage(pixm.qImagePtr());
		p->restore();
	}
/* Experimental Addition to display an Image as Background */
/*
	else if ((!Pfile.isEmpty()) && (PicAvail) && (PicArt))
	{
		p->save();
		if (imageClip.size() != 0)
		{
			p->setupPolygon(&imageClip);
			p->setClipPath();
		}
		p->setupPolygon(&PoLine);
		p->setClipPath();
		if (imageFlippedH())
		{
			p->translate(Width, 0);
			p->scale(-1, 1);
		}
		if (imageFlippedV())
		{
			p->translate(0, Height);
			p->scale(1, -1);
		}
		p->translate(LocalX*LocalScX, LocalY*LocalScY);
		p->scale(LocalScX, LocalScY);
		if (pixm.imgInfo.lowResType != 0)
			p->scale(pixm.imgInfo.lowResScale, pixm.imgInfo.lowResScale);
		p->drawImage(pixm.qImagePtr());
		p->restore();
	}
*/
	if (itemText.length() != 0)
	{
//		qDebug("drawing textframe: len=%d", itemText.length());
		if (imageFlippedH())
		{
			p->translate(Width, 0);
			p->scale(-1, 1);
			pf2.translate(Width, 0);
			pf2.scale(-1, 1);
		}
		if (imageFlippedV())
		{
			p->translate(0, Height);
			p->scale(1, -1);
			pf2.translate(0, Height);
			pf2.scale(1, -1);
		}
		uint tabCc = 0;
		assert( firstInFrame() >= 0 );
		assert( lastInFrame() < itemText.length() );
		LineSpec ls;
		for (uint ll=0; ll < itemText.lines(); ++ll) {
			ls = itemText.line(ll);
			tabDist = ls.x;
			double CurX = ls.x;
//			p->setLineWidth(0);
//			p->setBrush(Qt::yellow);
//			p->drawRect(ls.x, ls.y-ls.ascent, ls.width, ls.ascent);
//			p->setBrush(Qt::red);
//			p->drawRect(ls.x, ls.y, ls.width, ls.descent);
			
			QColor tmp;
			for (int a = ls.firstItem; a <= qMin(ls.lastItem, itemText.length()); ++a)
			{
				hl = itemText.item(a);
				const CharStyle& charStyle(itemText.charStyle(a));
				double chs = charStyle.fontSize() * hl->glyph.scaleV;
				bool selected = itemText.selected(a);
				if (charStyle.effects() & ScStyle_StartOfLine)
					tabCc = 0;
				chstr0 = hl->ch;
				actFill = charStyle.fillColor();
				actFillShade = charStyle.fillShade();
				if (actFill != CommonStrings::None)
				{
					if ((cachedFillShade != actFillShade) || (cachedFill != actFill))
					{
						SetFarbe(&tmp, actFill, actFillShade);
						p->setBrush(tmp);
						cachedFillQ = tmp;
						cachedFill = actFill;
						cachedFillShade = actFillShade;
					}
					else
						p->setBrush(cachedFillQ);
				}
				if (charStyle.effects() & ScStyle_DropCap)
				{
					const ParagraphStyle& style(itemText.paragraphStyle(a));
					if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						chs = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (style.dropCapLines()-1) + (charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / charStyle.font().realCharHeight(chstr0, 10)));
					else
					{
						if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
							chs = qRound(10 * ((style.lineSpacing() * (style.dropCapLines()-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / charStyle.font().realCharHeight(chstr0, 10)));
						else
						{
							double currasce = charStyle.font().height(style.charStyle().fontSize() / 10.0);
							chs = qRound(10 * ((currasce * (style.dropCapLines()-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / charStyle.font().realCharHeight(chstr0, 10)));
						}
					}
				}
				if (chstr0 == SpecialChars::TAB)
					tabCc++;
				// paint selection
				if (!m_Doc->RePos)
				{
					double xcoZli = CurX + hl->glyph.xoffset;
					desc = - charStyle.font().descent(charStyle.fontSize() / 10.0);
					asce = charStyle.font().ascent(charStyle.fontSize() / 10.0);
					if (((selected && Select) || ((NextBox != 0 || BackBox != 0) && selected)) && (m_Doc->appMode == modeEdit))
					{
						wide = hl->glyph.xadvance;
						p->setFillMode(1);
						p->setBrush(qApp->palette().color(QPalette::Active, QPalette::Highlight));
						p->setLineWidth(0);
//						if ((a > 0) && (QChar(hl->glyph.glyph) == SpecialChars::TAB))
//						{
//							xcoZli = CurX + itemText.item(a-1)->glyph.xoffset + itemText.charStyle(a-1).font().charWidth(itemText.text(a-1), itemText.charStyle(a-1).fontSize() / 10.0);
//							wide = CurX + hl->glyph.xoffset - xcoZli + hl->glyph.xadvance;
//						}
						if (!m_Doc->RePos)
							p->drawRect(xcoZli, ls.y + hl->glyph.yoffset - asce * hl->glyph.scaleV, wide+1, (asce+desc) * (hl->glyph.scaleV));
						p->setBrush(qApp->palette().color(QPalette::Active, QPalette::HighlightedText));
					}
					// FIXME temporary solution to have at least something like a cursor
					if ((a == CPos-1) && (m_Doc->appMode == modeEdit))
					{
						p->setPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
						p->drawLine(FPoint(xcoZli+hl->glyph.xadvance+1, ls.y + hl->glyph.yoffset - asce * hl->glyph.scaleV), FPoint(xcoZli+hl->glyph.xadvance+1, ls.y + hl->glyph.yoffset));
					}
					actStroke = charStyle.strokeColor();
					actStrokeShade = charStyle.strokeShade();
					if (actStroke != CommonStrings::None)
					{
						if ((cachedStrokeShade != actStrokeShade) || (cachedStroke != actStroke))
						{
							SetFarbe(&tmp, actStroke, actStrokeShade);
							p->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
							cachedStrokeQ = tmp;
							cachedStroke = actStroke;
							cachedStrokeShade = actStrokeShade;
						}
						else
							p->setPen(cachedStrokeQ, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					}
					// paint glyphs
					if (e2.intersects(pf2.mapRect(QRect(qRound(CurX + hl->glyph.xoffset),qRound(ls.y + hl->glyph.yoffset-asce), qRound(hl->glyph.xadvance+1), qRound(asce+desc)))))
					{
						p->save();
						p->translate(CurX, ls.y);
						if (hl->ch == SpecialChars::OBJECT)
							DrawObj_Embedded(p, e, charStyle, hl->embedded.getItem());
						else
							drawGlyphs(p, charStyle, hl->glyph);
						p->restore();
					}
					if (hl->ch == SpecialChars::OBJECT)
						CurX += (hl->embedded.getItem()->gWidth + hl->embedded.getItem()->lineWidth());
					else
						CurX += hl->glyph.wide();
				}
				tabDist = CurX;
			}
		}
	}
	else {
//		qDebug("skipping textframe: len=%d", itemText.count());
	}

//	pf2.end();
	p->restore();
}


void PageItem_TextFrame::DrawObj_Post(ScPainter *p)
{
	ScribusView* view = m_Doc->view();
	if (m_Doc->layerOutline(LayerNr))
	{
		p->setPen(m_Doc->layerMarker(LayerNr), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::None);
		p->setBrushOpacity(1.0);
		p->setPenOpacity(1.0);
		p->setupPolygon(&PoLine);
		p->strokePath();
	}
	else
	{
		if (fillBlendmode() != 0)
			p->endLayer();
		if (!m_Doc->RePos)
		{
			if (lineBlendmode() != 0)
				p->beginLayer(1.0 - lineTransparency(), lineBlendmode());
			if (lineColor() != CommonStrings::None)
			{
				p->setPen(strokeQColor, m_lineWidth, PLineArt, PLineEnd, PLineJoin);
				if (DashValues.count() != 0)
					p->setDash(DashValues, DashOffset);
			}
			else
				p->setLineWidth(0);
			if (!isTableItem)
			{
				p->setupPolygon(&PoLine);
				if (NamedLStyle.isEmpty())
					p->strokePath();
				else
				{
					multiLine ml = m_Doc->MLineStyles[NamedLStyle];
					QColor tmp;
					for (int it = ml.size()-1; it > -1; it--)
					{
						SetFarbe(&tmp, ml[it].Color, ml[it].Shade);
						p->setPen(tmp, ml[it].Width,
								static_cast<Qt::PenStyle>(ml[it].Dash),
								static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
								static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
						p->strokePath();
					}
				}
			}
			if (lineBlendmode() != 0)
				p->endLayer();
		}
	}
	if ((!isEmbedded) && (!m_Doc->RePos))
	{
		double scpInv = 1.0 / (qMax(view->scale(), 1.0));
		if ((Frame) && (m_Doc->guidesSettings.framesShown) && ((itemType() == ImageFrame) || (itemType() == TextFrame) || (itemType() == PathText)))
		{
			p->setPen(PrefsManager::instance()->appPrefs.DFrameNormColor, scpInv, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
			if ((isBookmark) || (m_isAnnotation))
				p->setPen(PrefsManager::instance()->appPrefs.DFrameAnnotationColor, scpInv, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
			if ((BackBox != 0) || (NextBox != 0))
				p->setPen(PrefsManager::instance()->appPrefs.DFrameLinkColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if (m_Locked)
				p->setPen(PrefsManager::instance()->appPrefs.DFrameLockColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

			p->setFillMode(0);
			if (itemType()==PathText)
			{
				if (Clip.count() != 0)
				{
					FPointArray tclip;
					FPoint np = FPoint(Clip.point(0));
					tclip.resize(2);
					tclip.setPoint(0, np);
					tclip.setPoint(1, np);
					for (int a = 1; a < Clip.size(); ++a)
					{
						np = FPoint(Clip.point(a));
						tclip.putPoints(tclip.size(), 4, np.x(), np.y(), np.x(), np.y(), np.x(), np.y(), np.x(), np.y());
					}
					np = FPoint(Clip.point(0));
					tclip.putPoints(tclip.size(), 2, np.x(), np.y(), np.x(), np.y());
					p->setupPolygon(&tclip);
				}
			}
			else
				p->setupPolygon(&PoLine);
			p->strokePath();
		}
		if ((m_Doc->guidesSettings.framesShown) && textFlowUsesContourLine() && (ContourLine.size() != 0))
		{
			p->setPen(Qt::lightGray, scpInv, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
			p->setupPolygon(&ContourLine);
			p->strokePath();
		}

		//Draw the overflow icon
		if (frameOverflows())
		{//CB && added here for jghali prior to commit access
			if ((!view->m_canvas->isPreviewMode()))
				drawOverflowMarker(p);
		}
		if ((m_Doc->guidesSettings.colBordersShown) && (!view->m_canvas->isPreviewMode()))
			drawColumnBorders(p);
		if ((m_Doc->guidesSettings.layerMarkersShown) && (m_Doc->layerCount() > 1) && (!m_Doc->layerOutline(LayerNr)))
		{
			p->setPen(Qt::black, 0.5/ m_Doc->view()->scale(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setPenOpacity(1.0);
			p->setBrush(m_Doc->layerMarker(LayerNr));
			p->setBrushOpacity(1.0);
			p->setFillMode(ScPainter::Solid);
			double ofwh = 10;
			double ofx = Width - ofwh/2;
			double ofy = Height - ofwh*3;
			p->drawRect(ofx, ofy, ofwh, ofwh);
		}
		//if (m_Doc->selection->findItem(this)!=-1)
		//	drawLockedMarker(p);
	}
//	Tinput = false;
	FrameOnly = false;
	p->restore();
}


void PageItem_TextFrame::clearContents()
{
	PageItem *nextItem = this;
	while (nextItem->prevInChain() != 0)
		nextItem = nextItem->prevInChain();

	ParagraphStyle defaultStyle = nextItem->itemText.defaultStyle();
	nextItem->itemText.clear();
	nextItem->itemText.setDefaultStyle(defaultStyle);

	while (nextItem != 0)
	{
		nextItem->CPos = 0;
		nextItem->invalid = true;
		nextItem = nextItem->nextInChain();
	}
}

void PageItem_TextFrame::handleModeEditKey(QKeyEvent *k, bool& keyRepeat)
{
	int oldPos = CPos; // 15-mar-2004 jjsa for cursor movement with Shift + Arrow key
	int kk = k->key();
	int as = k->text()[0].unicode();
	QString uc = k->text();
	QString cr, Tcha, Twort;
	uint Tcoun;
	int len, pos;
	int KeyMod=0;
	Qt::KeyboardModifiers buttonModifiers = k->modifiers();
	switch (buttonModifiers)
	{
		case Qt::ShiftModifier:
			KeyMod = Qt::SHIFT;
			break;
		case Qt::AltModifier:
			KeyMod = Qt::ALT;
			break;
		case Qt::ControlModifier:
			KeyMod = Qt::CTRL;
			break;
	}
	ScribusView* view = m_Doc->view();	
	view->slotDoCurs(false);
	switch (kk)
	{
	case Qt::Key_PageDown:
	case Qt::Key_PageUp:
	case Qt::Key_End:
	case Qt::Key_Home:
	case Qt::Key_Right:
	case Qt::Key_Left:
	case Qt::Key_Up:
	case Qt::Key_Down:
		if ( (buttonModifiers & Qt::ShiftModifier) == 0 )
			deselectAll();
	}
	//<< ISO 14755
	//if ((buttonState & Qt::ControlButton) && (buttonState & Qt::ShiftButton))
	//{
	//	if (!unicodeTextEditMode)
	//	{
	//		unicodeTextEditMode=true;
	//		unicodeInputCount = 0;
	//		unicodeInputString = "";
	//		keyrep = false;
	//	}
	//	qDebug(QString("%1 %2 %3 %4 %5").arg("uni").arg("c+s").arg(uc).arg(kk).arg(as));
	//}
	//>>
	if (unicodeTextEditMode)
	{
		int conv = 0;
		bool ok = false;
		unicodeInputString += uc;
		conv = unicodeInputString.toInt(&ok, 16);
		if (!ok)
		{
			unicodeTextEditMode = false;
			unicodeInputCount = 0;
			unicodeInputString = "";
			keyRepeat = false;
			return;
		}
		unicodeInputCount++;
		if (unicodeInputCount == 4)
		{
			unicodeTextEditMode = false;
			unicodeInputCount = 0;
			unicodeInputString = "";
			if (ok)
			{
				if (itemText.lengthOfSelection() > 0)
					deleteSelectedTextFromFrame();
				if (conv < 31)
					conv = 32;
				itemText.insertChars(CPos, QString(QChar(conv)), true);
				CPos += 1;
//				Tinput = true;
				m_Doc->scMW()->setTBvals(this);
				update();
				keyRepeat = false;
				return;
			}
		}
		else
		{
			keyRepeat = false;
			return;
		}
	}
	switch (kk)
	{
	case Qt::Key_F12:
		unicodeTextEditMode = true;
		unicodeInputCount = 0;
		unicodeInputString = "";
		keyRepeat = false;
		return;
		break;
	case Qt::Key_Home:
		// go to begin of line
		if ( (pos = CPos) == firstInFrame() )
			break; // at begin of frame
		len = lastInFrame();
		if ( pos >= len )
			pos = len-1;
		if ( (buttonModifiers & Qt::ControlModifier) == 0 )
		{
			pos = itemText.startOfLine(pos);
		}
		else
		{
			//Control Home for start of frame text
			pos = itemText.startOfFrame(pos);
		}
		CPos = pos;
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(-1, oldPos);
//		if ( this->itemText.lengthOfSelection() > 0 )
//			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_End:
		// go to end of line
		len = lastInFrame();
		if ( CPos >= len )
			break; // at end of frame
		if ( (buttonModifiers & Qt::ControlModifier) == 0 )
		{
			CPos = itemText.endOfLine(CPos);
		}
		else
		{
			//Control End for end of frame text
			CPos = itemText.endOfFrame(CPos);
		}
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(1, oldPos);
//		if ( this->itemText.lengthOfSelection() > 0 )
//			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_Down:
		if (buttonModifiers & Qt::ControlModifier)
		{
			// go to end of paragraph
			len = itemText.length();
			CPos = itemText.nextParagraph(CPos);
			if ( buttonModifiers & Qt::ShiftModifier )
				ExpandSel(1, oldPos);
		}
		else
		{
			if (CPos <= lastInFrame())
			{
				CPos = itemText.nextLine(CPos);
				if ( buttonModifiers & Qt::ShiftModifier )
				{
					if ( buttonModifiers & Qt::AltModifier )
						CPos = lastInFrame()+1;
					ExpandSel(1, oldPos);
				}
				else 
					if (CPos > lastInFrame() && NextBox != 0)
					{
						if (NextBox->frameDisplays(CPos))
						{
							view->Deselect(true);
							NextBox->CPos = CPos;
							view->SelectItemNr(NextBox->ItemNr);
						}
					}
			}
			else
			{
				if (NextBox != 0)
				{
					if (NextBox->frameDisplays(lastInFrame()+1))
					{
						view->Deselect(true);
						NextBox->CPos = lastInFrame()+1;
						view->SelectItemNr(NextBox->ItemNr);
					}
				}
			}
		}
//		if ( this->itemText.lengthOfSelection() > 0 )
//			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_Up:
		if (buttonModifiers & Qt::ControlModifier)
		{
			if ( (pos = CPos) == firstInFrame() )
				break; // at begin of frame
			len = itemText.length();
			CPos = itemText.prevParagraph(CPos);
			if ( buttonModifiers & Qt::ShiftModifier )
				ExpandSel(-1, oldPos);
		}
		else
		{
			if (CPos > firstInFrame())
			{
				if (CPos > lastInFrame() || CPos >= itemText.length())
					CPos = lastInFrame();
				CPos = itemText.prevLine(CPos);
				if ( buttonModifiers & Qt::ShiftModifier )
				{
					if ( buttonModifiers & Qt::AltModifier )
						CPos = firstInFrame();
					ExpandSel(-1, oldPos);
				}
				else
					if (CPos == firstInFrame() && BackBox != 0)
					{
						view->Deselect(true);
						BackBox->CPos = BackBox->lastInFrame();
						view->SelectItemNr(BackBox->ItemNr);
					}
			}
			else
			{
				CPos = firstInFrame();
				if (BackBox != 0)
				{
					view->Deselect(true);
					BackBox->CPos = BackBox->lastInFrame();
					view->SelectItemNr(BackBox->ItemNr);
				}
			}
		}
//		if ( this->itemText.lengthOfSelection() > 0 )
//			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_PageUp:
		CPos = itemText.startOfFrame(CPos);
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(-1, oldPos);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_PageDown:
		CPos = itemText.endOfFrame(CPos);
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(1, oldPos);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_Left:
		if ( buttonModifiers & Qt::ControlModifier )
		{
			setNewPos(oldPos, itemText.length(), -1);
			if ( buttonModifiers & Qt::ShiftModifier )
				ExpandSel(-1, oldPos);
		}
		else if ( buttonModifiers & Qt::ShiftModifier )
		{
			--CPos;
			if ( CPos < 0 )
				CPos = 0;
			else
				ExpandSel(-1, oldPos);
		}
		else
		{
			--CPos;
			if (CPos < firstInFrame())
			{
				CPos = firstInFrame();
				if (BackBox != 0)
				{
					view->Deselect(true);
					BackBox->CPos = BackBox->lastInFrame();
					view->SelectItemNr(BackBox->ItemNr);
					//currItem = currItem->BackBox;
				}
			}
		}
		if ((CPos > 0) && (CPos >= lastInFrame()))
		{
			CPos = lastInFrame();
			if (itemText.charStyle(CPos-1).effects() & ScStyle_SuppressSpace)
			{
				--CPos;
				while ((CPos > 0) && (itemText.charStyle(CPos).effects() & ScStyle_SuppressSpace))
				{
					--CPos;
					if (CPos == 0)
						break;
				}
			}
		}
		else
		{
			while ((CPos > 0) && (itemText.charStyle(CPos).effects() & ScStyle_SuppressSpace))
			{
				--CPos;
				if (CPos == 0)
					break;
			}
		}
//		if ( itemText.lengthOfSelection() > 0 )
//			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_Right:
		if ( buttonModifiers & Qt::ControlModifier )
		{
			setNewPos(oldPos, itemText.length(), 1);
			if ( buttonModifiers & Qt::ShiftModifier )
				ExpandSel(1, oldPos);
		}
		else if ( buttonModifiers & Qt::ShiftModifier )
		{
			++CPos;
			if ( CPos > itemText.length() )
				--CPos;
			else
				ExpandSel(1, oldPos);
		}
		else
		{
			++CPos; // new position within text ?
			if (CPos > lastInFrame())
			{
//				--CPos;
				CPos = lastInFrame() + 1;
				if (NextBox != 0)
				{
					if (NextBox->frameDisplays(CPos))
					{
						view->Deselect(true);
						NextBox->CPos = CPos;
						view->SelectItemNr(NextBox->ItemNr);
						//currItem = currItem->NextBox;
					}
				}
			}
		}
		if ( itemText.lengthOfSelection() > 0 )
			update();
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_Delete:
		if (CPos == itemText.length())
		{
			if (itemText.lengthOfSelection() > 0)
			{
				deleteSelectedTextFromFrame();
				m_Doc->scMW()->setTBvals(this);
//				view->RefreshItem(this);
			}
			keyRepeat = false;
			return;
		}
		if (itemText.length() == 0)
		{
			keyRepeat = false;
			return;
		}
		cr = itemText.text(CPos,1);
		if (itemText.lengthOfSelection() == 0)
			itemText.select(CPos, 1, true);
		deleteSelectedTextFromFrame();
//		Tinput = false;
		if ((cr == QChar(13)) && (itemText.length() != 0))
		{
//			m_Doc->chAbStyle(this, findParagraphStyle(m_Doc, itemText.paragraphStyle(qMax(CPos-1,0))));
//			Tinput = false;
		}
		m_Doc->scMW()->setTBvals(this);
//		view->RefreshItem(this);
		break;
	case Qt::Key_Backspace:
		if (CPos == 0)
		{
			if (itemText.lengthOfSelection() > 0)
			{
				deleteSelectedTextFromFrame();
				m_Doc->scMW()->setTBvals(this);
				update();
			}
			break;
		}
		if (itemText.length() == 0)
			break;
		cr = itemText.text(qMax(CPos-1,0),1);
		if (itemText.lengthOfSelection() == 0)
		{
			--CPos;
			itemText.select(CPos, 1, true);
		}
		deleteSelectedTextFromFrame();
//		Tinput = false;
		if ((cr == QChar(13)) && (itemText.length() != 0))
		{
//			m_Doc->chAbStyle(this, findParagraphStyle(m_Doc, itemText.paragraphStyle(qMax(CPos-1,0))));
//			Tinput = false;
		}
		m_Doc->scMW()->setTBvals(this);
//		view->RefreshItem(this);
		break;
	default:
		if ((itemText.lengthOfSelection() > 0) && (kk < 0x1000))
			deleteSelectedTextFromFrame();
		//if ((kk == Qt::Key_Tab) || ((kk == Qt::Key_Return) && (buttonState & Qt::ShiftButton)))
		if (kk == Qt::Key_Tab)
		{
			itemText.insertChars(CPos, QString(SpecialChars::TAB), true);
			CPos += 1;
//			Tinput = true;
//			view->RefreshItem(this);
			break;
		}
		if ((uc[0] > QChar(31) && m_Doc->currentStyle.charStyle().font().canRender(uc[0])) || (as == 13) || (as == 30))
		{
			itemText.insertChars(CPos, uc, true);
			CPos += 1;
			if ((m_Doc->docHyphenator->AutoCheck) && (CPos > 1))
			{
				Twort = "";
				Tcoun = 0;
				for (int hych = CPos-1; hych > -1; hych--)
				{
					Tcha = itemText.text(hych,1);
					if (Tcha[0] == ' ')
					{
						Tcoun = hych+1;
						break;
					}
					Twort.prepend(Tcha);
				}
				if (!Twort.isEmpty())
				{
					m_Doc->docHyphenator->slotHyphenateWord(this, Twort, Tcoun);
				}
			}
			invalid = true;
//			Tinput = true;
//			view->RefreshItem(this);
		}
		break;
	}
	update();
//	view->slotDoCurs(true);
	if ((kk == Qt::Key_Left) || (kk == Qt::Key_Right) || (kk == Qt::Key_Up) || (kk == Qt::Key_Down))
	{
		keyRepeat = false;
		return;
	}
}

void PageItem_TextFrame::deleteSelectedTextFromFrame()
{
	if (itemText.lengthOfSelection() > 0) {
		CPos = itemText.startOfSelection();
		itemText.removeSelection();
		HasSel = false;
	}
//	m_Doc->updateFrameItems();
	m_Doc->scMW()->DisableTxEdit();
}


//CB Rewrote JJSA code, March 06
// jjsa added on 15-mar-2004
// calculate the end position while ctrl + arrow pressed

void PageItem_TextFrame::setNewPos(int oldPos, int len, int dir)
{
	
	bool isSpace, wasSpace;
	if ( dir > 0 && oldPos < len )
	{
		wasSpace = itemText.text(oldPos).isSpace();
		CPos=oldPos+1;
		while (CPos<len)
		{
			isSpace = itemText.text(CPos).isSpace();
			if (wasSpace && !isSpace)
				break;
			++CPos;
			wasSpace=isSpace;
			
		}
		/*
		isSpace = itemText.at(oldPos)->ch.at(0).isSpace();
		CPos = oldPos +1;
		for (int i=oldPos+1; i < len; i++)
		{
			if ( itemText.at(i)->ch.at(0).isSpace() != isSpace )
				break;
			CPos++;
		}
		*/
	}
	else if ( dir < 0 && oldPos > 0 )
	{
		CPos=oldPos-1;
		wasSpace = itemText.text(CPos).isSpace();
		while (CPos>0)
		{
			isSpace = itemText.text(CPos).isSpace();
			if (!wasSpace && isSpace)
			{
				++CPos;
				break;
			}
			--CPos;
			wasSpace=isSpace;
			
		}
		/*
		oldPos--;
		isSpace = itemText.at(oldPos)->ch.at(0).isSpace();
		for (int i=oldPos; i >= 0; i--)
		{
			if (  itemText.at(i)->ch.at(0).isSpace() != isSpace )
				break;
			CPos--;
		}
		*/
	}
	cursorBiasBackward = (dir < 0);
}



// jjsa added on 15-mar-2004 expand / decrease selection

// jjsa added on 14-mar-2004 text selection with pressed
// shift button and <-, -> cursor keys
// Parameters
//   PageItem *currItem text item to be processed
//   inc < 0 for left key > 0 for right key
//  if value is +/-1 work on slection
//  if value is +/-2 refresh if text under cursor is selected

void PageItem_TextFrame::ExpandSel(int dir, int oldPos)
{
	int len = itemText.length();
	bool rightSel = false; // assume left right and actual char not selected
	bool leftSel  = false;
	bool actSel   = false;
	bool selMode  = false;

	if ( dir == -1 || dir == 1 )
		selMode = true;
	else
	{
		if ( dir > 0 )
			dir = 1;
		else
			dir = -1;
	}
   // show for selection of previous, actual and next character
	if ( itemText.lengthOfSelection() > 0 ) /* selection already present */
	{
		if (dir > 0 && oldPos < len) // -> key
		{
			if ( oldPos == 0 )
				leftSel = false;
			else
				leftSel = itemText.selected(oldPos-1);
			actSel = itemText.selected(oldPos);
			rightSel = false; // not relevant
		}
		else if ( dir > 0 && oldPos == len) // -> key
		{
			return;
		}
		else if ( dir < 0 && oldPos > 0 ) // <- key
		{
			actSel = itemText.selected(oldPos-1);
			leftSel = false; // not relevant
			if ( oldPos < len  )
				rightSel = itemText.selected(oldPos);
			else
				rightSel = false;
		}
		else if ( dir < 0 && oldPos == 0 ) // <- key
		{
         return;
		}
		if ( selMode && !(leftSel||actSel||rightSel) )
		{
         // selected outside from concerned range
			itemText.deselectAll();
			HasSel = false;
			//CB Replace with direct call for now //emit HasNoTextSel();
			m_Doc->scMW()->DisableTxEdit();
		}
		else if ( !selMode )
		{
			if (leftSel||actSel||rightSel)
				update();
		}
	}
	if ( !selMode )
		return;
   // no selection
	if ( !HasSel )
	{
		HasSel = true;
		//CB Replace with direct call for now //emit HasTextSel();
		m_Doc->scMW()->EnableTxEdit();
		leftSel = true;
		rightSel = true;
	}
	int start;
	int end;
	int sel;
	if (dir == 1) // ->  key
	{
		start = oldPos;
		end   = CPos;
		sel = leftSel == true;
	}
	else
	{
		start = CPos;
		end   = oldPos;
		sel = rightSel == true;
	}
	itemText.select(start, end-start, sel);
	if ( ! sel )
		//CB Replace with direct call for now //emit  HasNoTextSel();
		m_Doc->scMW()->DisableTxEdit();

	cursorBiasBackward = (dir < 0);

	update();
}

void PageItem_TextFrame::deselectAll()
{
	PageItem *item = this;
	while( item->prevInChain() )
		item=item->prevInChain();

	while ( item )
	{
		if ( item->itemText.lengthOfSelection() > 0 )
		{
			item->itemText.deselectAll();
			update();
			item->HasSel = false;
		}
		item = item->nextInChain();
	}
	//CB Replace with direct call for now //emit HasNoTextSel();
	m_Doc->scMW()->DisableTxEdit();
}

double PageItem_TextFrame::columnWidth()
{
	double lineCorr;
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth;
	else
		lineCorr = 0;
	return (Width - (ColGap * (Cols - 1)) - Extra - RExtra - 2 * lineCorr) / Cols;
//	return (Width - (ColGap * (Cols - 1)) - Extra - RExtra - lineCorr) / Cols;
}

void PageItem_TextFrame::drawOverflowMarker(ScPainter *p)
{
	/*CB Old large corner indicator.
	double scp1 = 1.0/qMax(ScMW->view->getScale(), 1);
	double scp16 = 16.0*scp1;
	double scp14 = 14.0*scp1;
	double scp3 = 3.0*scp1;
	double scm_lineWidth16 = Width - scp16;
	double scpheight16 = Height - scp16;
	double scm_lineWidth3 = Width - scp3;
	double scpheight3 = Height - scp3;
	p->setPen(Qt::black, scp1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setBrush(white);
	p->drawRect(scm_lineWidth16, scpheight16, scp14, scp14);
	p->drawLine(FPoint(scm_lineWidth16, scpheight16), FPoint(scm_lineWidth3, scpheight3));
	p->drawLine(FPoint(scm_lineWidth16, scpheight3), FPoint(scm_lineWidth3, scpheight16));
	*/
	//TODO: CB clean
	ScribusView* view = m_Doc->view();
	double scp1 = 1 ;// / ScMW->view->scale();
	double ofwh = 8 * scp1;
	//CB moved down while locked marker disabled
	//double ofx = Width - ofwh/2;
	//double ofy = Height - ofwh*3.0;
	double ofx = Width - ofwh/2;
	double ofy = Height - ofwh*1.5;
	double lx1= ofx;
	double ly1= ofy;
	double lx2= ofx+ofwh;
	double ly2= ofy+ofwh;
	p->setPen(Qt::black, 0.5/ view->scale(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setPenOpacity(1.0);
	p->setBrush(Qt::white);
	p->setBrushOpacity(1.0);
	p->setFillMode(ScPainter::Solid);
	p->drawRect(ofx, ofy, ofwh, ofwh);
	p->drawLine(FPoint(lx1, ly1), FPoint(lx2, ly2));
	p->drawLine(FPoint(lx1, ly2), FPoint(lx2, ly1));
}

void PageItem_TextFrame::drawColumnBorders(ScPainter *p)
{
	ScribusView* view = m_Doc->view();
	p->setPen(Qt::black, 0.5/ view->scale(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setPenOpacity(1.0);
	p->setBrush(Qt::white);
	p->setBrushOpacity(1.0);
	p->setFillMode(ScPainter::Solid);
	p->setupPolygon(&PoLine);
	p->setClipPath();
	double colWidth = columnWidth();
	double colLeft=0;
	int curCol=0;
	double lineCorr=0;
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth / 2.0;
	if (TExtra + lineCorr!=0.0)
		p->drawLine(FPoint(0, TExtra + lineCorr), FPoint(Width, TExtra + lineCorr));
	if (BExtra + lineCorr!=0.0)
		p->drawLine(FPoint(0, Height - BExtra - lineCorr), FPoint(Width, Height - BExtra - lineCorr));
	double oldColRightX = Extra + lineCorr;
	while(curCol < Cols)
	{
		colLeft=(colWidth + ColGap) * curCol + Extra + lineCorr;
		if (colLeft != 0.0)
			p->drawLine(FPoint(colLeft, 0), FPoint(colLeft, 0+Height));
		if (colLeft + colWidth != Width)
			p->drawLine(FPoint(colLeft+colWidth, 0), FPoint(colLeft+colWidth, 0+Height));
		oldColRightX=colLeft+colWidth;
		++curCol;
	}
	
}

#endif  //NLS_PROTO

bool PageItem_TextFrame::createInfoGroup(QFrame *infoGroup, QGridLayout *infoGroupLayout)
{
	int Parag = 0;
	int Words = 0;
	int Chara = 0;
	int ParagN = 0;
	int WordsN = 0;
	int CharaN = 0;
	
	QLabel *infoCT = new QLabel(infoGroup);
	QLabel *linesCT = new QLabel(infoGroup);
	QLabel *linesT = new QLabel(infoGroup);
	QLabel *parCT = new QLabel(infoGroup);
	QLabel *parT = new QLabel(infoGroup);
	QLabel *wordCT = new QLabel(infoGroup);
	QLabel *wordT = new QLabel(infoGroup);
	QLabel *charCT = new QLabel(infoGroup);
	QLabel *charT = new QLabel(infoGroup);
	
	if ((nextInChain() != 0) || (prevInChain() != 0))
		infoCT->setText(tr("Linked Text"));
	else
		infoCT->setText(tr("Text Frame"));
	infoGroupLayout->addWidget( infoCT, 0, 0, 1, 2, Qt::AlignCenter );
	
	WordAndPara(this, &Words, &Parag, &Chara, &WordsN, &ParagN, &CharaN);
	parCT->setText(tr("Paragraphs: "));
	infoGroupLayout->addWidget( parCT, 1, 0, Qt::AlignRight );
	if (ParagN != 0)
		parT->setText(QString::number(Parag+ParagN)+" ("+QString::number(ParagN)+")");
	else
		parT->setText(QString::number(Parag));
	infoGroupLayout->addWidget( parT, 1, 1 );
	
	linesCT->setText(tr("Lines: "));
	infoGroupLayout->addWidget( linesCT, 2, 0, Qt::AlignRight );
	linesT->setText(QString::number(itemText.lines()));
	infoGroupLayout->addWidget( linesT, 2, 1 );
	
	
	wordCT->setText(tr("Words: "));
	infoGroupLayout->addWidget( wordCT, 3, 0, Qt::AlignRight );
	if (WordsN != 0)
		wordT->setText(QString::number(Words+WordsN)+" ("+QString::number(WordsN)+")");
	else
		wordT->setText(QString::number(Words));
	infoGroupLayout->addWidget( wordT, 3, 1 );
	
	charCT->setText(tr("Chars: "));
	infoGroupLayout->addWidget(charCT, 4, 0, Qt::AlignRight );
	if (CharaN != 0)
		charT->setText(QString::number(Chara+CharaN)+" ("+QString::number(CharaN)+")");
	else
		charT->setText(QString::number(Chara));
	infoGroupLayout->addWidget( charT, 4, 1 );
	return true;
}

bool PageItem_TextFrame::createContextMenu(QMenu *menu, int step)
{
	static QMenu *menuPDF = 0;
	QMap<QString, QPointer<ScrAction> > actions = doc()->scMW()->scrActions;
	QAction *act;
	
	if (menu == 0) {
		if (menuPDF) delete menuPDF;
		return true;
	}
	switch(step) {
		case 10:
			menu->addSeparator();
			menu->addAction(actions["fileImportText"]);
			menu->addAction(actions["fileImportAppendText"]);
			menu->addAction(actions["toolsEditWithStoryEditor"]);
			menu->addAction(actions["insertSampleText"]);
			break;
		case 20:
			if (doc()->currentPage()->pageName().isEmpty())
			{
				menuPDF = new QMenu();
				menuPDF->addAction(actions["itemPDFIsAnnotation"]);
				menuPDF->addAction(actions["itemPDFIsBookmark"]);
				if (isAnnotation())
				{
					if ((annotation().Type() == 0) || (annotation().Type() == 1) || (annotation().Type() > 9))
						menuPDF->addAction(actions["itemPDFAnnotationProps"]);
					else
						menuPDF->addAction(actions["itemPDFFieldProps"]);
				}
			}
			act = menu->addMenu(menuPDF);
			act->setText( ScribusView::tr("&PDF Options"));
			break;
		case 30:
			if (isTableItem)
			{
				actions["itemConvertToImageFrame"]->setEnabled(true);
				menu->addAction(actions["itemConvertToImageFrame"]);
				//TODO: ConvertToLatexFrame
			} else {
				if ((prevInChain() == 0) && (nextInChain() == 0))
					menu->addAction(actions["itemConvertToImageFrame"]);
				menu->addAction(actions["itemConvertToOutlines"]);
				if ((prevInChain() == 0) && (nextInChain() == 0))
					menu->addAction(actions["itemConvertToPolygon"]);
			}
			break;
		case 40:
			if (itemText.lines() != 0) {
				menu->addAction(actions["editClearContents"]);
			} else {
				return false;
			}
			break;
		default:
			return false;
	}
	return true;
}
