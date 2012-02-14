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

#include <QDebug>
#include <QList>
#include <QMatrix>
#include <QPalette>
#include <QPoint>
#include <QPolygon>
#include <QRegion>
#include <cassert>


#include "canvas.h"
#include "commonstrings.h"
#include "guidemanager.h"
#include "hyphenator.h"
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
#include "scribusXml.h"
#include "serializer.h"
#include "selection.h"
#include "text/nlsconfig.h"
#include "undomanager.h"
#include "undostate.h"
#include "util.h"
#include "util_math.h"
#ifdef HAVE_CAIRO
	#include <cairo.h>
#endif

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
	lastUndoAction = NOACTION;
	
	connect(&itemText,SIGNAL(changed()), this, SLOT(slotInvalidateLayout()));
}

PageItem_TextFrame::PageItem_TextFrame(const PageItem & p) : PageItem(p)
{
	invalid = true;
	cursorBiasBackward = false;
	unicodeTextEditMode = false;
	unicodeInputCount = 0;
	unicodeInputString = "";
	lastUndoAction = NOACTION;

	connect(&itemText,SIGNAL(changed()), this, SLOT(slotInvalidateLayout()));
}

static QRegion itemShape(PageItem* docItem, double xOffset, double yOffset)
{
	QRegion res;
	QMatrix pp;
	pp.translate(docItem->xPos() - xOffset, docItem->yPos() - yOffset);
	pp.rotate(docItem->rotation());
	if (docItem->textFlowUsesBoundingBox())
	{
		QPolygon tcli(4);
		tcli.setPoint(0, QPoint(0,0));
		tcli.setPoint(1, QPoint(qRound(docItem->width()), 0));
		tcli.setPoint(2, QPoint(qRound(docItem->width()), qRound(docItem->height())));
		tcli.setPoint(3, QPoint(0, qRound(docItem->height())));
		res = QRegion(pp.map(tcli));
	}
	else if ((docItem->textFlowUsesImageClipping()) && (docItem->imageClip.size() != 0))
	{
		QList<uint> Segs;
		QPolygon Clip2 = FlattenPath(docItem->imageClip, Segs);
		res = QRegion(pp.map(Clip2)).intersect(QRegion(pp.map(docItem->Clip)));
	}
	else if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
	{
		QList<uint> Segs;
		QPolygon Clip2 = FlattenPath(docItem->ContourLine, Segs);
		res = QRegion(pp.map(Clip2));
	}
	else
		res = QRegion(pp.map(docItem->Clip));
	return  res;
}

QRegion PageItem_TextFrame::availableRegion()
{
	QRegion result(this->Clip);
	if (!isEmbedded)
	{
		bool invertible(false);
		QMatrix canvasToLocalMat;
		canvasToLocalMat.translate(Xpos, Ypos);
		canvasToLocalMat.rotate(Rot);
		canvasToLocalMat = canvasToLocalMat.inverted(&invertible);

		if (!invertible) return QRegion();

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
			int currentGroup = -1;
			for (int a = 0; a < m_Doc->MasterItems.count(); ++a)
			{
				docItem = m_Doc->MasterItems.at(a);
				LayerLevItem = m_Doc->layerLevelFromNumber(docItem->LayerNr);
				if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev && m_Doc->layerFlow(docItem->LayerNr)))
				{
					if (docItem->Groups.count() == 0)
						currentGroup = -1;
					if ((currentGroup != -1) && (docItem->Groups.count() != 0) && (currentGroup == docItem->Groups.top()))
						continue;
					if (docItem->textFlowAroundObject())
					{
						QRegion itemRgn = itemShape(docItem, Mp->xOffset() - Dp->xOffset(), Mp->yOffset() - Dp->yOffset());
						result = result.subtracted( canvasToLocalMat.map(itemRgn) );
					}
					if (docItem->isGroupControl)
						currentGroup = docItem->Groups.top();
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
			int currentGroup = -1;
			for (uint a = 0; a < docItemsCount; ++a)
			{
				docItem = m_Doc->Items->at(a);
				LayerLevItem = m_Doc->layerLevelFromNumber(docItem->LayerNr);
				if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev && m_Doc->layerFlow(docItem->LayerNr)))
				{
					if (docItem->Groups.count() == 0)
						currentGroup = -1;
					if ((currentGroup != -1) && (docItem->Groups.count() != 0) && (currentGroup == docItem->Groups.top()))
						continue;
					if (docItem->textFlowAroundObject())
					{
						QRegion itemRgn = itemShape(docItem, 0, 0);
						result = result.subtracted( canvasToLocalMat.map(itemRgn) );
					}
					if (docItem->isGroupControl)
						currentGroup = docItem->Groups.top();
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
//				qDebug() << QString("Pageitem_Textframe: style of master: %1 align=%2").arg(pstyle.parent()).arg(pstyle.alignment());
//				qDebug() << QString("Pageitem_Textframe: shadow itemText->%1").arg(OnMasterPage);
			}
			if (newShadow != OnMasterPage) {
				shadows[newShadow] = shadows[OnMasterPage].copy();
//				const ParagraphStyle& pstyle(shadows[newShadow].paragraphStyle(0));
//				qDebug() << QString("Pageitem_Textframe: style of shadow copy: %1 align=%2").arg(pstyle.parent()).arg(pstyle.alignment());
			}
//			qDebug() << QString("Pageitem_Textframe: shadow %1<-%2").arg(newShadow).arg(OnMasterPage);
		}
		itemText = shadows[newShadow];
//		const ParagraphStyle& pstyle(itemText.paragraphStyle(0));
//		qDebug() << QString("Pageitem_Textframe: style of shadow: %1 align=%2").arg(pstyle.parent()).arg(pstyle.alignment());
		invalid = true;
		currentShadow = newShadow;
	}
}

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
	qDebug() << db;
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
	for (int ti= curLine.firstItem; ti <= curLine.lastItem; ++ti)
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
//			qDebug() << QString("tab leaders: %1 %2 width=%3 count=%4").arg(sPos).arg(curLine.y).arg(wt).arg(count);
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
	bool     hasDropCap;
	bool     afterOverflow;
	bool     addLine;
	bool     recalculateY;
	bool     lastInRowLine;
	bool     addLeftIndent;
	bool     wasFirstInRow;
	double   leftIndent;
	double   rightMargin;
	double   mustLineEnd;
	int      restartIndex;  //index of char where line computing should be restarted
	int      restartRowIndex;  //index of char where row of text is started
	double   restartX; //starting X position of line if must be restarted
	double   rowDesc;
	
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
		hasDropCap = false;
	}
	
	/// start at column 0
	void initColumns(double width, double gap)
	{
		column = 0;
		colWidth = width;
		colGap = gap;
	}
	
	/// move position to next column
	void nextColumn(double asce = 0.0)
	{
		startOfCol = true;
		colLeft = (colWidth + colGap) * column + insets.Left + lineCorr;
		//now colRight is REAL column right edge
		colRight = colLeft + colWidth;
		if (legacy)
			colRight += lineCorr;
		xPos = colLeft;
		yPos = asce + insets.Top + lineCorr;
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
		line.lastItem = 0;
		line.ascent = 0.0;
		line.descent = 0.0;
		line.width = 0.0;
		line.naturalWidth = 0.0;
		line.colLeft = colLeft;
		breakIndex = -1;
		breakXPos = 0.0;
		maxShrink = 0.0;
		maxStretch = 0.0;
		width = 0.0;
		leftIndent = 0.0;
		rightMargin = 0.0;
		rowDesc = 0.0;
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
	void rememberBreak(int index, double pos, double morespace = 0)
	{
		if (pos > colRight - morespace)
		{
			// only look for the first break behind the right edge
			//maxShrink = 0;
			
			// check if we already have a better break
			if (breakIndex >= 0)
			{
				double oldLooseness = qAbs(colRight - breakXPos);

				double newLooseness = pos - colRight;
				if (newLooseness >= oldLooseness)
					return;
			}
		}
		breakXPos = pos;
		breakIndex = index;
	}
	
	/// called when a mandatory break is found
	void breakLine(const StoryText& itemText, const ParagraphStyle& style, FirstLineOffsetPolicy offsetPolicy, int last)
	{
		breakIndex = last;
		breakXPos  = line.x;
		for (int j = line.firstItem; j <= last; ++j)
			if ( (itemText.item(j)->effects() & ScStyle_SuppressSpace) == 0)
				breakXPos += itemText.item(j)->glyph.wide();
		// #8194, #8717 : update line ascent and descent with sensible values
		// so that endOfLine() returns correct result
		updateHeightMetrics(itemText);
		// #9060 : update line offset too
		//updateLineOffset(itemText, style, offsetPolicy);
	}

	/// use the last remembered break to set line width and itemrange
	void finishLine(double endX)
	{
		line.lastItem = breakIndex;
		line.naturalWidth = breakXPos - line.x;
		line.width = endX - line.x;
		maxShrink = maxStretch = 0;
	}

	int restartRow(bool recalcY)
	{
		if (recalcY)
			yPos++;
		recalculateY = recalcY;
		xPos = restartX = colLeft;
		startLine(restartRowIndex);
		addLeftIndent = true;
		afterOverflow = false;
		return restartRowIndex -1;
	}

	int restartLine(bool recalcY, bool add)
	{
		recalculateY = recalcY;
		addLine = add;
		xPos = restartX;
		startLine(restartIndex);
		afterOverflow = false;
		return restartIndex -1;
	}

	bool isEndOfLine(double moreSpace = 0)
	{
		bool res;
		if (legacy)
			res = ceil(xPos + lineCorr - maxShrink) + ceil(moreSpace) >= floor(colRight);
		else
			res = ceil(xPos - maxShrink)  + ceil(moreSpace) >= floor(colRight);
		return res;
	}

	/// Keep old endOfLine code for reference
	/*double endOfLine_old(const QRegion& shape, const QMatrix& pf2, double morespace, int Yasc, int Ydesc)
	{
		// if we aren't restricted further, we'll end at this maxX:
		double maxX = colRight - morespace;
		if (legacy) maxX -= lineCorr;

		double StartX = floor(qMax((double)line.x, qMin(maxX,breakXPos-maxShrink-1))-1);
		int xPos  = static_cast<int>(ceil(maxX));

		QPoint  pt12 (xPos, Yasc);
		QPoint  pt22 (xPos, Ydesc);
		QRect   pt(pt12,pt22);
		QRegion region; 

		double EndX2 = StartX;
		double Interval = 0.25;
		do {
			int xP = static_cast<int>(ceil(EndX2 + morespace));
			pt.moveTopLeft(QPoint(xP, Yasc));
			region = QRegion(pf2.mapToPolygon(pt)).subtracted(shape);
			if (!region.isEmpty())
				break;
			EndX2 += Interval;
		} while ((EndX2 < maxX) && region.isEmpty());

		return qMin(EndX2, maxX);
	}*/

	/// find x position where this line must end
	double endOfLine(const QRegion& shape, double morespace, int yAsc, int yDesc)
	{
		// if we aren't restricted further, we'll end at this maxX:
		double maxX = colRight - morespace;
		if (legacy) maxX -= lineCorr;

		double StartX = floor(qMax((double)line.x, qMin(maxX,breakXPos-maxShrink-1))-1);
		int xPos  = static_cast<int>(ceil(maxX));

		QPoint  pt12 (xPos, yAsc);
		QPoint  pt22 (xPos, yDesc);

		QPolygon p;
		p.append (QPoint (StartX, yAsc));
		p.append (QPoint (StartX, yDesc));
		p.append (pt12);
		p.append (pt22);
		// check if something gets in the way
		QRegion lineI = shape.intersected (p.boundingRect());
		// if the intersection only has 1 rectangle, then nothing gets in the way
		if (lineI.numRects() == 1)
		{
			int   cPos = static_cast<int>(ceil(StartX + morespace));
			QRect cRect (QPoint(cPos, yAsc), QPoint(cPos, yDesc));
			if (QRegion(cRect).subtracted(shape).isEmpty())
			{
				QRect rect = lineI.rects().at(0);
				double  mx = qMax(rect.left(), rect.right()) /*- pf2.dx()*/; 
				int steps  = static_cast<int>((mx - StartX - morespace - 2) / 0.25);
				if (steps > 0)
				{
					StartX += steps * 0.25;
				}
			}
		}

		QRect   pt(pt12, pt22);

		double EndX2 = StartX;
		double Interval = 0.25;
		do {
			int xP = static_cast<int>(ceil(EndX2 + morespace));
			pt.moveTopLeft(QPoint(xP, yAsc));
			if (!regionContainsRect(shape, pt))
				break;
			EndX2 += Interval;
		} while ((EndX2 < maxX) && regionContainsRect(shape, pt));

		/*double oldEndX2 = endOfLine_old(shape, pf2, morespace, yAsc, yDesc);
		if (oldEndX2 != qMin(EndX2, maxX))
		{
			qDebug() << "Different EndX : " << oldEndX2 << " (old) " << EndX2 << " (new) ";
		}*/

		return qMin(EndX2, maxX);
	}

	double getLineAscent(const StoryText& itemText)
	{
		double result = 0;
		QChar  firstChar = itemText.text(line.firstItem);
		const CharStyle& fcStyle(itemText.charStyle(line.firstItem));
		if ((firstChar == SpecialChars::PARSEP) || (firstChar == SpecialChars::LINEBREAK))
			result = fcStyle.font().ascent(fcStyle.fontSize() / 10.0);
		else if (itemText.object(line.firstItem) != 0)
			result = qMax(result, (itemText.object(line.firstItem)->gHeight + itemText.object(line.firstItem)->lineWidth()) * (fcStyle.scaleV() / 1000.0));
		else if ((firstChar == SpecialChars::PAGENUMBER) || (firstChar == SpecialChars::PAGECOUNT))
			result = fcStyle.font().realCharAscent('8', fcStyle.fontSize() / 10.0);
		else //if (itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap == 0)
			result = fcStyle.font().realCharAscent(firstChar, fcStyle.fontSize() / 10.0);
		for (int zc = 0; zc < itemsInLine; ++zc)
		{
			QChar ch = itemText.text(line.firstItem + zc);
			if ((ch == SpecialChars::PAGENUMBER) || (ch == SpecialChars::PAGECOUNT))
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
			//	qDebug() << QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(result);
			result = qMax(result, asce);
		}
		return result;
	}

	double getLineDescent(const StoryText& itemText)
	{
		double result = 0;
		QChar  firstChar = itemText.text(line.firstItem);
		const CharStyle& fcStyle(itemText.charStyle(line.firstItem));
		if ((firstChar == SpecialChars::PARSEP) || (firstChar == SpecialChars::LINEBREAK))
			result = fcStyle.font().descent(fcStyle.fontSize() / 10.0);
		else if (itemText.object(line.firstItem) != 0)
			result = 0.0;
		else if ((firstChar == SpecialChars::PAGENUMBER) || (firstChar == SpecialChars::PAGECOUNT))
			result = fcStyle.font().realCharDescent('8', fcStyle.fontSize() / 10.0);
		else //if (itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap == 0)
			result = fcStyle.font().realCharDescent(firstChar, fcStyle.fontSize() / 10.0);
		for (int zc = 0; zc < itemsInLine; ++zc)
		{
			QChar ch = itemText.text(line.firstItem + zc);
			if ((ch == SpecialChars::PAGENUMBER) || (ch == SpecialChars::PAGECOUNT))
				ch = '8'; // should have highest ascender even in oldstyle
			const CharStyle& cStyle(itemText.charStyle(line.firstItem + zc));
			if ((ch == SpecialChars::TAB) || (ch == QChar(10))
				|| (ch == SpecialChars::PARSEP) || (ch == SpecialChars::NBHYPHEN)
				|| (ch == SpecialChars::COLBREAK) || (ch == SpecialChars::LINEBREAK)
				|| (ch == SpecialChars::FRAMEBREAK) || (ch.isSpace()))
				continue;
			double desc;
			if (itemText.object(line.firstItem + zc) != 0)
				desc = 0.0;
			else //if (itemText.charStyle(current.line.firstItem + zc).effects() & ScStyle_DropCap == 0)
				desc = cStyle.font().realCharDescent(ch, cStyle.fontSize() / 10.0);
			//	qDebug() << QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(result);
			result = qMax(result, desc);
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
			//	qDebug() << QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(result);
			result = qMax(result, asce);
		}
		return result;
	}

	void updateHeightMetrics(const StoryText& itemText)
	{
		double asce, desc;
		line.ascent  = 0;
		line.descent = 0;
		for (int zc = 0; zc < itemsInLine; ++zc)
		{
			QChar ch = itemText.text(line.firstItem+zc);
			if ((ch == SpecialChars::TAB) || (ch == QChar(10))
				|| (ch == SpecialChars::PARSEP) || (ch == SpecialChars::NBHYPHEN)
				|| (ch == SpecialChars::COLBREAK) || (ch == SpecialChars::FRAMEBREAK)
				|| (ch == SpecialChars::LINEBREAK) || (ch.isSpace()))
				continue;
			const CharStyle& cStyle(itemText.charStyle(line.firstItem + zc));
			double scaleV = cStyle.scaleV() / 1000.0;
			double offset = (cStyle.fontSize() / 10) * (cStyle.baselineOffset() / 1000.0);

			if (itemText.object(line.firstItem+zc) != 0)
			{
				asce = (itemText.object(line.firstItem+zc)->gHeight + itemText.object(line.firstItem+zc)->lineWidth()) * scaleV + offset;
				desc = 0.0;
			}
			else //if (itemText.charStyle(current.line.firstItem+zc).effects() & ScStyle_DropCap == 0)
			{
				asce = cStyle.font().realCharAscent(ch, cStyle.fontSize() / 10.0) * scaleV + offset;
				desc = cStyle.font().realCharDescent(ch, cStyle.fontSize() / 10.0) * scaleV - offset;
			}
			//	qDebug() << QString("checking char 'x%2' with ascender %1 > %3").arg(asce).arg(ch.unicode()).arg(result);
			line.ascent  = qMax((double)line.ascent, asce);
			line.descent = qMax((double)line.descent, desc);
		}
	}
	
// yPos should not be changed when all line is already calculated - at new y position there can be overflow!!!
//
//	void updateLineOffset(const StoryText& itemText, const ParagraphStyle& style, FirstLineOffsetPolicy offsetPolicy)
//	{
//		if (itemsInLine <= 0)
//			return;
//		if ((!hasDropCap) && (startOfCol) && (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing))
//		{
//			//FIXME: use glyphs, not chars
//			double firstasce = itemText.charStyle(line.firstItem).font().ascent(itemText.charStyle(line.firstItem).fontSize() / 10.0);
//			double adj (0.0);
//			double currasce (this->getLineAscent(itemText));
//			if( offsetPolicy == FLOPRealGlyphHeight )
//			{
//				adj = firstasce - currasce;
//			}
//			else if( offsetPolicy == FLOPFontAscent )
//			{
//				adj = 0.0;
//			}
//			else if( offsetPolicy == FLOPLineSpacing )
//			{
//				adj = firstasce - style.lineSpacing();
//			}
//			line.ascent = currasce;
//			line.y -= adj;
//			yPos -= adj;
//		}
//		else if ((!startOfCol) && (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
//		{
//			QChar ch = itemText.text(line.firstItem);
//			double firstasce = style.lineSpacing();
//			double currasce  = getLineHeight(itemText);						
//			double adj = firstasce - currasce;
//			qDebug() << QString("move2 line %1.. down by %2").arg(current.line.firstItem).arg(-adj);
//			line.ascent = currasce;
//			line.y -= adj;
//			yPos -= adj;
//		}
//	}

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
	qDebug() << QString("justify: line = %7 natural = %1 + %2 = %3 (%4); spaces + %5%%; min=%8; glyphs + %6%%; min=%9")
		   .arg(spaceNatural).arg(glyphNatural).arg(spaceNatural+glyphNatural).arg(line.naturalWidth)
		   .arg(spaceExtension).arg(glyphExtension).arg(line.width)
		   .arg(style.minWordTracking()).arg(style.minGlyphExtension());
	*/
	
	int startItem = line.firstItem;
	if (itemText.item(startItem)->effects() & ScStyle_DropCap)
		startItem++;
	// distribute whitespace on spaces and glyphs
	for (int yof = startItem; yof <= line.lastItem; ++yof)
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

//defined but not used
///// calculate how much the first char should stick out to the left
//static double opticalLeftMargin(const StoryText& itemText, const LineSpec& line)
//{
//	int b = line.firstItem;
//	while (b < line.lastItem && (itemText.item(b)->effects() & ScStyle_SuppressSpace))
//		   ++b;
	
//	double chs = itemText.charStyle(b).fontSize() * (itemText.charStyle(b).scaleH() / 1000.0);
//	QChar chr = itemText.text(b);
//	double leftCorr = itemText.charStyle(b).font().realCharWidth(chr, chs / 10.0);
//	if (QString("'´`").indexOf(chr) >= 0
//		|| chr == QChar(0x2018) // quote 6
//		|| chr == QChar(0x2019) // quote 9
//		|| chr == QChar(0x201a) // lower quote 9
//		|| chr == QChar(0x201b) // upper reversed 9 6
//		|| chr == QChar(0x2039) // single guillemet <
//		|| chr == QChar(0x203a) // single guillemet >
//		)
//		leftCorr *= -0.7;
//	else if (QString("\"").indexOf(chr) >= 0
//			 || chr == QChar(0x00ab) // guillemet <<
//			 || chr == QChar(0x00bb) // guillemet >>
//			 || chr == QChar(0x201c) // quote 66
//			 || chr == QChar(0x201d) // quote 99
//			 || chr == QChar(0x201e) // lower quote 99
//			 || chr == QChar(0x201f) // upper reversed 99
//			 ) 
//		leftCorr *= -0.5;
//	else {
//		leftCorr = itemText.charStyle(b).font().charWidth(QChar(' '), chs / 10.0, chr);
//		leftCorr -= itemText.charStyle(b).font().charWidth(QChar(' '), chs / 10.0);
//	}
//	return leftCorr;
//}

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

static double findRealOverflowEnd(const QRegion& shape, QRect pt, double maxX)
{
	while (!regionContainsRect(shape, pt) && pt.right() < maxX)
		pt.translate(1, 0);
	if (pt.right() >= maxX)
		return maxX;
	return pt.left() + 0.5;
}

void PageItem_TextFrame::layout() 
{
// 	qDebug()<<"==Layout==" << itemName() ;
// 	printBacktrace(24);
	if (BackBox != NULL && BackBox->invalid) {
//		qDebug("textframe: len=%d, going back", itemText.length());
		// Why that invalid = false here? Calling prevInChain->layout() does
		// not ensure that this box will be layouted
		// invalid = false;
		PageItem_TextFrame* prevInChain = dynamic_cast<PageItem_TextFrame*>(BackBox);
		while (prevInChain && prevInChain->invalid)
		{
			if (!prevInChain->BackBox || !prevInChain->BackBox->invalid)
				break;
			prevInChain = dynamic_cast<PageItem_TextFrame*>(prevInChain->BackBox);
		}
		if (prevInChain && prevInChain->invalid)
			prevInChain->layout();
		// #9592 : warning, BackBox->layout() may not layout BackBox next box
		if (!invalid)
			return;
	}
	else if (!invalid && OnMasterPage.isEmpty()) {
//		qDebug() << QString("textframe: len=%1, invalid=%2 OnMasterPage=%3: no relayout").arg(itemText.length()).arg(invalid).arg(OnMasterPage);
		return;
	}
	if (invalid && BackBox == NULL)
		firstChar = 0;
	
//	qDebug() << QString("textframe(%1,%2): len=%3, start relayout at %4").arg(Xpos).arg(Ypos).arg(itemText.length()).arg(firstInFrame());
	QPoint pt1, pt2;
	QRect pt;
	double chs, chsd = 0;
	double oldCurY, EndX, OFs, wide, kernVal;
	QString chstr;
	ScText *hl;
	ParagraphStyle style;
	int /*ParagraphStyle::OpticalMarginType*/ opticalMargins = ParagraphStyle::OM_None;
	
	bool outs = false;
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
	double desc=0, asce=0, realAsce=0, realDesc = 0;
	double maxDY=0, maxDX=0;
	double DropCapDrop = 0;
	int    DropLines = 0;
	int    DropLinesCount = 0;
	
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

	//hold Y position of last computed line of text (with glyphs descent)
	//for moving next line if glyphs are higher than that
	double lastLineY = 0;
	
	// dump styles
/*	
	for (int i=0; i < itemText.nrOfParagraphs(); ++i) {
		const ParagraphStyle& pstyle(itemText.paragraphStyle(itemText.endOfParagraph(i)));
		qDebug("par %d:", i);
		dumpIt(pstyle);
	}
	qDebug() << "default:";
	dumpIt(itemText.defaultStyle());
*/
	
	setShadow();
	if ((itemText.length() != 0)) // || (NextBox != 0))
	{
		// determine layout area
		QRegion cl = availableRegion();
		if (cl.isEmpty())
		{
			MaxChars = firstInFrame();
			goto NoRoom;
		}
		
		if (imageFlippedH() || imageFlippedV())
		{
			QMatrix matrix;
			if (imageFlippedH())
			{
				matrix.translate(Width, 0);
				matrix.scale(-1, 1);
			}
			if (imageFlippedV())
			{
				matrix.translate(0, Height);
				matrix.scale(1, -1);
			}
			cl = matrix.map(cl);
		}
		
		current.nextColumn();
		lastLineY = extra.Top;

		// find start of first line
		if (firstInFrame() < itemText.length())
		{
			hl = itemText.item(firstInFrame());
			style = itemText.paragraphStyle(firstInFrame());
			if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
			{
				style.setLineSpacing(style.charStyle().font().height(style.charStyle().fontSize() / 10.0));
//				qDebug() << QString("auto linespacing: %1").arg(style.lineSpacing());
			}
			else if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
				style.setLineSpacing(m_Doc->typographicSettings.valueBaseGrid);

//			qDebug() << QString("style @0: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//				   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():"");
			if (style.hasDropCap())
			{
				if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
					chs = qRound(m_Doc->typographicSettings.valueBaseGrid  * style.dropCapLines() * 10);
				else
					chs = qRound(style.lineSpacing() * style.dropCapLines() * 10);
			}
			else 
				chs = hl->fontSize();
			desc = -hl->font().descent(chs / 10.0);
			current.yPos = extra.Top + lineCorr;
//			qDebug() << QString("first line at y=%1").arg(current.yPos);
		}
		else // empty itemText:
		{
			desc = -itemText.defaultStyle().charStyle().font().descent(itemText.defaultStyle().charStyle().fontSize() / 10.0);
			current.yPos = itemText.defaultStyle().lineSpacing() + extra.Top + lineCorr - desc;
		}
		current.startLine(firstInFrame());

		outs = false;
		OFs = 0;
		MaxChars = 0;
		double realEnd = 0;
		current.restartIndex = current.restartRowIndex = firstInFrame();
		current.afterOverflow = false;
		current.addLine = false;
		current.recalculateY = true;
		current.lastInRowLine = false;
		current.addLeftIndent = true;
		current.wasFirstInRow = false;
		current.leftIndent = 0;
		current.rightMargin = 0.0;
		current.mustLineEnd = current.colRight;
		current.restartX = 0;
		int lastStat = 0, curStat = 0;

		for (int a = firstInFrame(); a < itemText.length(); ++a)
		{
			hl = itemText.item(a);
			curStat = SpecialChars::getCJKAttr(hl->ch);
			if (a > 0 && itemText.text(a-1) == SpecialChars::PARSEP)
				style = itemText.paragraphStyle(a);
			if (current.itemsInLine == 0)
				opticalMargins = style.opticalMargins();
			
			const CharStyle& charStyle =  (hl->ch != SpecialChars::PARSEP? itemText.charStyle(a) : itemText.paragraphStyle(a).charStyle());
			chstr = ExpandToken(a);
			double hlcsize10 = charStyle.fontSize() / 10.0;
			double scaleV = charStyle.scaleV() / 1000.0;
			double scaleH = charStyle.scaleH() / 1000.0;
			double offset = hlcsize10 * (charStyle.baselineOffset() / 1000.0);

			if (chstr.isEmpty())
				chstr = SpecialChars::ZWNBSPACE;
			if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
				style.setLineSpacing(charStyle.font().height(hlcsize10));
			else if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
				style.setLineSpacing(m_Doc->typographicSettings.valueBaseGrid);
			// find out about par gap and dropcap
			if (a == firstInFrame())
			{
				if (a == 0 || itemText.text(a-1) == SpecialChars::PARSEP)
				{
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
				if ( current.itemsInLine == 0 && (SpecialChars::isBreakingSpace(hl->ch) || hl->ch.isSpace()))
				{
					hl->setEffects(hl->effects() | ScStyle_SuppressSpace);
					hl->glyph.xadvance = 0;
					continue;
				}
				else
					hl->setEffects(hl->effects() & ~ScStyle_SuppressSpace);
			}
			if (current.itemsInLine == 0)
			{
				if (style.rightMargin() == 0)
				{
					//addLine = true;
					current.rightMargin = 0.0;
				}
				else
				{
					if (current.lastInRowLine)
						current.rightMargin = style.rightMargin();
					else
						current.rightMargin = 0.0;
				}
				current.breakIndex = -1;
				if (current.startOfCol && !current.afterOverflow && current.recalculateY)
					current.yPos = qMax(current.yPos, extra.Top);
				// more about par gap and dropcaps
				if ((a > firstInFrame() && itemText.text(a-1) == SpecialChars::PARSEP) || (a == 0 && BackBox == 0 && current.startOfCol))
				{
					if (!current.afterOverflow && current.recalculateY && !current.startOfCol)
						current.yPos += style.gapBefore();
					DropCapDrop = 0;
					if (chstr[0] != SpecialChars::PARSEP)
						DropCmode = style.hasDropCap();
					else
						DropCmode = false;
					if (DropCmode && !current.afterOverflow)
					{
						DropLines = style.dropCapLines();
						if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
							DropCapDrop = m_Doc->typographicSettings.valueBaseGrid * (DropLines - 1);
						else if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
							DropCapDrop = style.lineSpacing() * (DropLines - 1);
						else
							DropCapDrop = charStyle.font().height(hlcsize10) * (DropLines - 1);
					}
				}
			}
			// find charsize factors
			if (DropCmode)
			{
				if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
					DropCapDrop = m_Doc->typographicSettings.valueBaseGrid * (DropLines - 1);
				else if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
					DropCapDrop = style.lineSpacing() * (DropLines - 1);
				else
					DropCapDrop = charStyle.font().height(hlcsize10) * (DropLines - 1);

				// FIXME : we should ensure that fonts are loaded before calls to layout()
				// ScFace::realCharHeight()/Ascent() ensure font is loaded thanks to an indirect call to char2CMap()
				// ScFace::ascent() can be called safely afterwards
				double realCharHeight = charStyle.font().realCharHeight(chstr[0], 1);
				double realCharAscent = charStyle.font().realCharAscent(chstr[0], 1);
				double fontAscent     = charStyle.font().ascent(hlcsize10);
				if (realCharHeight == 0.0)
					realCharHeight = charStyle.font().height(hlcsize10);
				if (realCharAscent == 0.0)
					realCharAscent = fontAscent;
				if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
				{
					chsd = (10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1) + fontAscent) / realCharHeight));
					chs  = (10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1) + fontAscent) / realCharAscent));
				}
				else if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
				{
					chsd = (10 * ((style.lineSpacing() * (DropLines-1) + fontAscent) / realCharHeight));
					chs  = (10 * ((style.lineSpacing() * (DropLines-1) + fontAscent) / realCharAscent));
				}
				else
				{
					double currasce = charStyle.font().height(hlcsize10);
					chsd = (10 * ((currasce * (DropLines-1) + fontAscent) / realCharHeight));
					chs  = (10 * ((currasce * (DropLines-1) + fontAscent) / realCharAscent));
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
			// set StartOfLine (and find tracking?)
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
			// find out width, ascent and descent of char
			if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
			{
				wide = hl->embedded.getItem()->gWidth + hl->embedded.getItem()->lineWidth();
				hl->glyph.xadvance = wide * hl->glyph.scaleH;
			}
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
		// change xadvance, xoffset according to JIS X4051
					ScText *hl2 = itemText.item(a+1);
					int nextStat = SpecialChars::getCJKAttr(hl2->ch);
					int prevStat;
					if(curStat != 0){	// current char is CJK
						if(nextStat == 0 && !SpecialChars::isBreakingSpace(hl2->ch)){
							switch(curStat & SpecialChars::CJK_CHAR_MASK){
							case SpecialChars::CJK_KANJI:
							case SpecialChars::CJK_KANA:
							case SpecialChars::CJK_NOTOP:
								kern = wide / 4;
								wide += kern;
								hl->glyph.xadvance += kern;
							}
						} else {	// next char is CJK, too
							switch(curStat & SpecialChars::CJK_CHAR_MASK){
							case SpecialChars::CJK_FENCE_END:
								switch(nextStat & SpecialChars::CJK_CHAR_MASK){
								case SpecialChars::CJK_FENCE_BEGIN:
								case SpecialChars::CJK_FENCE_END:
								case SpecialChars::CJK_COMMA:
								case SpecialChars::CJK_PERIOD:
								case SpecialChars::CJK_MIDPOINT:
									kern = -wide / 2;
									wide += kern;
									hl->glyph.xadvance += kern;
								}
								break;
							case SpecialChars::CJK_COMMA:
							case SpecialChars::CJK_PERIOD:
								switch(nextStat & SpecialChars::CJK_CHAR_MASK){
								case SpecialChars::CJK_FENCE_BEGIN:
								case SpecialChars::CJK_FENCE_END:
									kern = -wide / 2;
									wide += kern;
									hl->glyph.xadvance += kern;
								}
								break;
							case SpecialChars::CJK_MIDPOINT:
								switch(nextStat & SpecialChars::CJK_CHAR_MASK){
								case SpecialChars::CJK_FENCE_BEGIN:
									kern = -wide / 2;
									wide += kern;
									hl->glyph.xadvance += kern;
								}
								break;
							case SpecialChars::CJK_FENCE_BEGIN:
								if(a == current.line.firstItem){ // first char of the line
									prevStat = SpecialChars::CJK_FENCE_BEGIN;
								} else {
									hl2 = itemText.item(a-1);
									prevStat = SpecialChars::getCJKAttr(hl2->ch) & SpecialChars::CJK_CHAR_MASK;
								}
								if(prevStat == SpecialChars::CJK_FENCE_BEGIN){
									kern = -wide / 2;
									wide += kern;
									hl->glyph.xadvance += kern;
									hl->glyph.xoffset += kern;
								}
								break;
							}

						}
					} else {	// current char is not CJK
						if(nextStat != 0 && !SpecialChars::isBreakingSpace(hl->ch)){
							switch(nextStat & SpecialChars::CJK_CHAR_MASK){
							case SpecialChars::CJK_KANJI:
							case SpecialChars::CJK_KANA:
							case SpecialChars::CJK_NOTOP:
								kern = hl2->glyph.wide() / 4;
								wide += kern;
								hl->glyph.xadvance += kern;
							}
						}
					}
				}
			}
			if (DropCmode)
			{
				// drop caps are wider...
				if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
				{
					double itemHeight = hl->embedded.getItem()->gHeight + hl->embedded.getItem()->lineWidth();
					if (itemHeight == 0)
						itemHeight = charStyle.font().height(style.charStyle().fontSize() / 10.0);
					wide = hl->embedded.getItem()->gWidth + hl->embedded.getItem()->lineWidth();
					if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						realAsce = m_Doc->typographicSettings.valueBaseGrid * DropLines;
					else
					{
						if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
							realAsce = style.lineSpacing() * DropLines;
						else
							realAsce = charStyle.font().height(style.charStyle().fontSize() / 10.0) * DropLines;
					}
					hl->glyph.scaleH /= hl->glyph.scaleV;
					hl->glyph.scaleV = (realAsce / itemHeight);
					hl->glyph.scaleH *= hl->glyph.scaleV;
				}
				else
				{
					double realCharHeight = charStyle.font().realCharHeight(chstr[0], hlcsize10);
					if (realCharHeight == 0)
						realCharHeight = charStyle.font().height(style.charStyle().fontSize() / 10.0);
					wide = (charStyle.font().realCharWidth(chstr[0], chsd / 10.0) * scaleH) + (1 - scaleH); //dropcaps are to close to next glyphs if glyphs are hard scaled
					realAsce = charStyle.font().realCharHeight(chstr[0], chsd / 10.0) * scaleV + offset;
					asce = charStyle.font().ascent(hlcsize10);
					// qDebug() QString("dropcaps pre: chsd=%1 realCharHeight = %2 chstr=%3").arg(chsd).arg(asce).arg(chstr2[0]);
					hl->glyph.scaleH /= hl->glyph.scaleV;
					hl->glyph.scaleV = (realAsce / realCharHeight);
					hl->glyph.scaleH *= hl->glyph.scaleV;
					hl->glyph.xoffset -= 0.5; //drop caps are always to far from column left edge
				}
				hl->glyph.xadvance = wide;
				desc = realDesc =0;
			}
			else // !DropCMode
			{
				if (SpecialChars::isExpandingSpace(hl->ch))
				{
					double wordtracking = charStyle.wordTracking();
					hl->glyph.xadvance *= wordtracking;
					wide *= wordtracking;
				}
				// find ascent / descent
				if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
					desc = realDesc = 0;
				else
				{
					desc = -charStyle.font().descent(hlcsize10);
					realDesc = charStyle.font().realCharDescent(chstr[0], hlcsize10) * scaleV - offset;
					current.rememberShrinkStretch(hl->ch, wide, style);
				}
				asce = charStyle.font().ascent(hlcsize10);
				realAsce = charStyle.font().realCharAscent(chstr[0], hlcsize10) * scaleV + offset;
			}

			//check for Y position at beginning of line
			if (current.itemsInLine == 0 && !current.afterOverflow)
			{
				if (current.recalculateY)
				{
					//if top of column Y position depends on first line offset
					if (current.startOfCol)
					{
						lastLineY = qMax(lastLineY, extra.Top + lineCorr);
						if (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing)
						{
							if (firstLineOffset() == FLOPRealGlyphHeight)
							{
								if (DropCmode)
									current.yPos += asce;
								else
									current.yPos += realAsce;
							}
							else if (firstLineOffset() == FLOPLineSpacing)
								current.yPos += style.lineSpacing();
							else
								current.yPos += asce;
						}
						else if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						{
							if (current.yPos <= lastLineY)
								current.yPos = lastLineY +1;
							double by = Ypos;
							if (OwnPage != -1)
								by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
							int ol1 = qRound((by + current.yPos - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
							current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
						}
					}
					else
					{
						if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						{
							current.yPos += m_Doc->typographicSettings.valueBaseGrid;
							double by = Ypos;
							if (OwnPage != -1)
								by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
							int ol1 = qRound((by + current.yPos - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
							current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
						}
						else
							current.yPos += style.lineSpacing();
					}
					if (DropCmode)
						current.yPos += DropCapDrop;
				}
				//set left indentation
				current.leftIndent = 0.0;
				if (current.addLeftIndent && (maxDX == 0 || DropCmode))
				{
					current.leftIndent = style.leftMargin();
					if (current.hasDropCap)
						current.leftIndent = 0;
					if (a==0 || (a > 0 && (itemText.text(a-1) == SpecialChars::PARSEP)))
						current.leftIndent += style.firstIndent();
					current.addLeftIndent = false;
				}
			}
			current.recalculateY = true;
			int maxYAsc = 0, maxYDesc = 0;
			if (current.startOfCol)
			{
				//qMax is used for glyphs shifted or vertically scalled above ascent or below descent
				double addAsce = qMax(realAsce, asce);
				if (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing)
				{
					if (firstLineOffset() == FLOPRealGlyphHeight)
						addAsce = realAsce;
					else if (firstLineOffset() == FLOPLineSpacing)
						addAsce = style.lineSpacing();
				}
				maxYAsc = static_cast<int>(floor(current.yPos - addAsce));
			}
			else
				maxYAsc = static_cast<int>(floor(current.yPos - qMax(realAsce, asce)));
			//fix for glyphs with negative realAsce value
			maxYAsc = qMax(maxYAsc, 0);
			maxYDesc = static_cast<int>(ceil(current.yPos + qMax(realDesc, desc)));

			if (current.itemsInLine == 0 && !current.afterOverflow)
			{
				//start a new line
				goNoRoom = false;

				// find line`s start
				pt1 = QPoint(static_cast<int>(floor(current.xPos)), maxYAsc);
				pt2 = QPoint(static_cast<int>(ceil(current.xPos + (style.minGlyphExtension() * wide))),maxYDesc);
				pt = QRect(pt1, pt2);
				realEnd = 0;
				//check if there is overflow at start of line, if so jump behind it and check again
				double Xpos, Xend;
				bool done = false;
				bool newColumn = false;
				while (!done)
				{
					Xpos = current.xPos;
					Xend = current.xPos + current.leftIndent;
					//check if in indent any overflow occurs
					while (Xpos <= Xend && Xpos < current.colRight)
					{
						if (!regionContainsRect(cl, pt))
						{
							Xpos = current.xPos = realEnd = findRealOverflowEnd(cl, pt, current.colRight);
							Xend = current.xPos + current.leftIndent;
						}
						else
							Xpos++;
						pt.moveTopLeft(QPoint(static_cast<int>(floor(Xpos)),maxYAsc));
					}
					current.xPos = Xend;
					done = true;
					if (current.isEndOfLine((style.minGlyphExtension() * wide) + current.rightMargin + current.leftIndent))
					{
						// new line
						current.xPos = qMax(current.colLeft, maxDX);
						if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						{
							current.yPos++;
							double by = Ypos;
							if (OwnPage != -1)
								by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
							int ol1 = qRound((by + current.yPos - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
							current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
						}
						else if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
							current.yPos += (current.startOfCol ? 1 : style.lineSpacing());
						else
							current.yPos++;
						lastLineY = maxYAsc +1;
						if (current.startOfCol)
						{
							//qMax is used for glyphs shifted or vertically scalled above ascent or below descent
							double addAsce = qMax(realAsce, asce);
							if (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing)
							{
								if (firstLineOffset() == FLOPRealGlyphHeight)
									addAsce = realAsce;
								else if (firstLineOffset() == FLOPLineSpacing)
									addAsce = style.lineSpacing();
							}
							maxYAsc = static_cast<int>(floor(current.yPos - addAsce));
						}
						else
							maxYAsc = static_cast<int>(floor(current.yPos - qMax(realAsce, asce)));
						maxYDesc = static_cast<int>(ceil(current.yPos + qMax(realDesc, desc)));

						pt.moveTopLeft(QPoint(static_cast<int>(floor(current.xPos)),maxYAsc));
						done = false;
					}
					if (current.isEndOfCol(realDesc))
					{
						current.column++;
						if (current.column < Cols)
						{
							newColumn = true;
							break;
						}
						else
						{
							MaxChars = a;
							goto NoRoom;
						}
					}
				}
				if (newColumn)
				{
					current.nextColumn();
					current.mustLineEnd = current.colRight;
					current.restartX = current.xPos;
					lastLineY = current.yPos;
					current.rowDesc = 0;
					a--;
					current.recalculateY = true;
					current.addLeftIndent = true;
					continue;
				}
				current.line.x = current.restartX = current.xPos;
				current.line.y = current.yPos;
			}

			//check if line must start at new Y position due to current glyph height or previous line descent
			if (!SpecialChars::isBreak(hl->ch, true)
				&& !SpecialChars::isBreakingSpace(hl->ch)
				&& !SpecialChars::isExpandingSpace(hl->ch)
				&& hl->ch != SpecialChars::TAB)
			{
				double diff = 0;
				if (current.startOfCol || DropCmode)
					diff = realAsce - (current.yPos - lastLineY);
				else if (style.lineSpacingMode() != ParagraphStyle::FixedLineSpacing)
					diff = charStyle.font().realCharAscent(QChar('l'), hlcsize10) * scaleV + offset - (current.yPos - lastLineY);
				if (diff >= 1 || (!DropCmode && diff > 0))
				{
					if (current.hasDropCap && DropLinesCount == 0)
					{
						current.hasDropCap = false;
						current.yPos = maxDY;
						maxDX = 0;
					}
					int linesDrop = 0;
					if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
					{
						linesDrop = ceil(diff / m_Doc->typographicSettings.valueBaseGrid);
						current.yPos += m_Doc->typographicSettings.valueBaseGrid * linesDrop;
					}
					else /*if (current.startOfCol)*/
					{
						//FIX ME - that is ugly hack I must made, because simply expression
						//current.yPos += diff; stop working, dont know why (compiler bug?)
						float YPOS = (float) current.yPos + (float) diff + 0.01;
						current.yPos = (double) YPOS;
						if (current.hasDropCap && diff > DropCapDrop)
						{
							current.hasDropCap = false;
						}
					}
					if (current.hasDropCap && linesDrop > 0 && DropLinesCount > 1)
					{
						DropLinesCount += linesDrop;
						if (DropLinesCount >= DropLines)
						{
							current.hasDropCap = false;
							maxDX = 0;
							maxDY = 0;
						}
					}
					a = current.restartRow(false);
					continue;
				}
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
							if (oCurX < tTabValues.at(yg).tabPosition)
							{
								tabs.status = static_cast<int>(tTabValues.at(yg).tabType);
								tCurX = tTabValues.at(yg).tabPosition;
								tabs.fillChar = tTabValues.at(yg).tabFillChar;
							}
						}
						tabs.active = (tabs.status != TabLEFT);
						if (tCurX == oCurX-wide)
							current.xPos = current.colLeft + ceil((current.xPos-current.colLeft) / m_Doc->toolSettings.dTabWidth) * m_Doc->toolSettings.dTabWidth;
						else
							current.xPos = current.colLeft + tCurX;
						
						// remember fill char
						if (!tabs.fillChar.isNull()) {
							hl->glyph.growWithTabLayout();
							TabLayout * tglyph = dynamic_cast<TabLayout*>(hl->glyph.more);
							if (tglyph)
							{
								tglyph->fillChar = tabs.fillChar;
								tglyph->glyph    = charStyle.font().char2CMap(tabs.fillChar);
								tglyph->yoffset  = hl->glyph.yoffset;
								tglyph->scaleV   = tglyph->scaleH = chs / charStyle.fontSize();
								tglyph->xadvance = 0;
							}
						}
					}
					current.xPos -= (legacy ? 1.0 : 0.0);
					hl->glyph.xadvance = current.xPos + wide + kernVal - tabs.xPos;
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
			// remember possible break
			if ( (SpecialChars::isBreakingSpace(hl->ch) || hl->ch == SpecialChars::TAB))
			{
				if ( a == firstInFrame() || !SpecialChars::isBreakingSpace(itemText.text(a-1)) )
				{
					current.rememberBreak(a, breakPos, style.rightMargin());
				}
			}
			// CJK break
			if(a > current.line.firstItem){ // not the first char
				if( (lastStat == 0) && (curStat == 0)){	// both non-CJK
					// do nothing
				} else {
					// non-CJK char does not have CJK_NOBREAK_AFTER/CJK_NOBREAK_BEFORE
					if((lastStat & SpecialChars::CJK_NOBREAK_AFTER) == 0 &&
					   (curStat & SpecialChars::CJK_NOBREAK_BEFORE) == 0){
						current.rememberBreak(a-1, breakPos, style.rightMargin());
					}
				}
				
			}
			lastStat = curStat;
			//check against space before PARSEP
			/*if (SpecialChars::isBreakingSpace(hl->ch) && (a + 1 < itemText.length()) && (itemText.item(a+1)->ch == SpecialChars::PARSEP))
			{
				a++;
				hl = itemText.item(a);
			}*/

			// test if end of line reached
			double overflowWidth = 0.0;
			double hyphWidth = 0.0;
			bool inOverflow = false;
			if (hl->effects() & ScStyle_HyphenationPossible || hl->ch == SpecialChars::SHYPHEN)
				hyphWidth = charStyle.font().charWidth('-', hlcsize10) * (charStyle.scaleH() / 1000.0);
			if ((current.isEndOfLine(style.rightMargin() + hyphWidth)) || current.isEndOfCol(realDesc) || SpecialChars::isBreak(hl->ch, Cols > 1) || (current.xPos - current.maxShrink + hyphWidth) >= current.mustLineEnd)
			{
				//end of row reached - right column, end of column, break char or line must end
				if (current.itemsInLine == 0 && !current.afterOverflow && !SpecialChars::isBreak(hl->ch, Cols > 1))
				{
					//no glyphs in line, so start new row
					if (SpecialChars::isBreak(hl->ch, Cols > 1))
						current.restartRowIndex = a +1;
					a = current.restartRow(true);
					inOverflow = false;
					outs = false;
					continue;
				}
				//there we have some glyphs
				if (current.breakIndex < 0)
 				{
 					//force break
					if (!SpecialChars::isBreak(hl->ch, Cols > 1))
					{
						//force line end at previouse glyph
						a--;
						current.mustLineEnd = current.line.x;
					}
 					current.breakLine(itemText, style, firstLineOffset(),a);
 				}
				if (!current.addLine && !current.lastInRowLine && current.afterOverflow)
				{
					//we reach right edge of column
					//if we are after overflow area we have to go back and try to insert text before it
					//if we have some text here - insert text WITHOUT right margin
					//if there is no place for text - insert text WITH right margin and end line
					current.lastInRowLine = false;
					if (current.line.firstItem == current.restartIndex)
						current.lastInRowLine = true;
					if (current.hasDropCap && DropLinesCount == 0 && current.restartIndex == current.restartRowIndex)
					{
						current.hasDropCap = false;
						maxDX = 0;
						current.yPos = maxDY;
					}
					a = current.restartLine(false,true);
					outs = false;
					inOverflow = false;
					continue;
				}
				outs = true;
				current.addLine = true;
				current.lastInRowLine = true;
				current.rightMargin = style.rightMargin();
				if (current.isEndOfCol(realDesc))
					goNextColumn = true;
			}
			else
			{
				int charStart, charEnd;
				if (current.itemsInLine == 0)
				{
					charStart = static_cast<int>(floor(current.line.x));
					charEnd = static_cast<int>(ceil(current.xPos));
				}
				else
				{
					charStart = static_cast<int>(qMax(floor(current.xPos - current.maxShrink - (style.minGlyphExtension() * wide)),0.0));
					charEnd = static_cast<int>(ceil(current.xPos - current.maxShrink) + hyphWidth);
				}
				if (legacy &&
						(((hl->ch == '-' || (hl->effects() & ScStyle_HyphenationPossible)) && (current.hyphenCount < m_Doc->HyCount || m_Doc->HyCount == 0))
						 || hl->ch == SpecialChars::SHYPHEN))
				{
					if (hl->effects() & ScStyle_HyphenationPossible || hl->ch == SpecialChars::SHYPHEN)
					{
						pt1 = QPoint(charStart, maxYAsc);
						pt2 = QPoint(static_cast<int>(charEnd + hyphWidth), maxYDesc);
					}
					else
					{
						pt1 = QPoint(charStart, maxYAsc);
						pt2 = QPoint(charEnd, maxYDesc);
					}
				}
				else if (!legacy && SpecialChars::isBreakingSpace(hl->ch))
				{
					pt1 = QPoint(static_cast<int>(qMax(floor(breakPos - current.maxShrink - (style.minGlyphExtension() * wide)),0.0)), maxYAsc);
					pt2 = QPoint(charEnd, maxYDesc);
				}
				else
				{
					pt1 = QPoint(charStart, maxYAsc);
					pt2 = QPoint(charEnd, maxYDesc);
				}
				pt = QRect(pt1, pt2);
				if (!regionContainsRect(cl, pt))
				{
					realEnd = findRealOverflowEnd(cl, pt, current.colRight);
					outs = true;
				}
				else if (style.rightMargin() > 0.0)
				{
					if (current.lastInRowLine || current.xPos - current.maxShrink + style.rightMargin() > current.colRight - style.rightMargin())
						//condition after || is for find overflows in right margin area
					{
						pt.translate(static_cast<int>(ceil(style.rightMargin())), 0);
						if (!regionContainsRect(cl, pt))
						{
							realEnd = findRealOverflowEnd(cl, pt, current.colRight);
							outs = true;
						}
					}
				}
				if (outs)
				{
					if (current.itemsInLine == 0)
					{
						current.line.x = current.xPos = realEnd;
						a--;
						current.startLine(a+1);
						if (!current.wasFirstInRow)
							current.addLeftIndent = true;
						if (current.hasDropCap && DropLinesCount == 0 && !current.afterOverflow)
						{
							current.hasDropCap = false;
							current.yPos = maxDY;
						}
						current.recalculateY = false;
						outs = false;
						continue;
					}
					if (current.breakIndex < 0)
					{
						a--;
						current.breakLine(itemText, style, firstLineOffset(), a);
					}
					//check if after overflow text can be placed
					overflowWidth = realEnd - (current.xPos - current.maxShrink);
					double newXAdd = overflowWidth - style.rightMargin() + style.minGlyphExtension() * wide + hyphWidth;
					if (current.isEndOfLine(newXAdd) || current.xPos + newXAdd >= current.colRight || realEnd >= current.mustLineEnd)
					{
						if (!current.afterOverflow)
						{
							current.addLine = true;
							current.lastInRowLine = true;
						}
						//line must end before overflov
						if (!current.addLine && !current.lastInRowLine)
						{
							if (current.afterOverflow)
							{
								if (current.breakIndex < 0)
								{
									current.lastInRowLine = true;
									current.mustLineEnd = current.line.x;
								}
								else if (current.line.firstItem == current.restartIndex)
									current.lastInRowLine = true;
							}
							else
								current.lastInRowLine = true;
							if (current.hasDropCap && DropLinesCount == 0)
							{
								current.hasDropCap = false;
								maxDX = 0;
							}
							a = current.restartLine(false, true);
							inOverflow = false;
							outs = false;
							continue;
						}
						current.addLine = true;
						current.rightMargin = style.rightMargin();
						current.lastInRowLine = true;
					}
					else
						inOverflow = true;
				}
			}

			// hyphenation
			if (((hl->effects() & ScStyle_HyphenationPossible) || (hl->ch == '-') || hl->ch == SpecialChars::SHYPHEN) && (!outs) && !itemText.text(a-1).isSpace() )
			{
				breakPos = current.xPos;
				if (hl->ch != '-')
				{
					breakPos += hyphWidth;
				}
				
				double rightHang = 0;
				if (opticalMargins & ParagraphStyle::OM_RightHangingPunct) {
					rightHang = 0.7 * hyphWidth;
				}
				
				if (legacy || (breakPos - rightHang < current.colRight - style.rightMargin()))
				{
					if ((current.hyphenCount < m_Doc->HyCount) || (m_Doc->HyCount == 0) || hl->ch == SpecialChars::SHYPHEN)
					{
						current.rememberBreak(a, breakPos, style.rightMargin() + hyphWidth);
					}
				}
			}

			if ((hl->ch == SpecialChars::FRAMEBREAK) && (a < itemText.length()-1))
				goNoRoom = true;
			if ((hl->ch == SpecialChars::COLBREAK) && (Cols > 1))
				goNextColumn = true;

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
			
			if (DropCmode && !outs)
			{
				DropCmode = false;
				DropLinesCount = 0;
				maxDY = current.yPos;
				current.hasDropCap = true;
				current.xPos += style.dropCapOffset();
				hl->glyph.xadvance += style.dropCapOffset();
				maxDX = current.xPos;
				if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
				{
					current.yPos -= m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
					double by = Ypos;
					if (OwnPage != -1)
						by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
					int ol1 = qRound((by + current.yPos - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
					int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
					current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
				}
				else if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
				{
					current.yPos -= style.lineSpacing() * (DropLines-1);
				}
				else
				{
					double currasce = charStyle.font().height(charStyle.fontSize() / 10.0);
					current.yPos -= currasce * (DropLines-1);
				}
				current.recalculateY = false;
			}
			// end of line
			if (outs)
			{
				tabs.active = false;
				tabs.status = TabNONE;
				if (SpecialChars::isBreak(hl->ch, Cols > 1))
				{
					// find end of line
					current.breakLine(itemText, style, firstLineOffset(), a);
					EndX = current.endOfLine(cl, style.rightMargin(), maxYAsc, maxYDesc);
					current.finishLine(EndX);
					//addLine = true;
					assert(current.addLine);
					//current.startOfCol = false;
					//addLeftIndent = true;
					if (hl->ch == SpecialChars::PARSEP)
					{
						maxDX = 0;
						if (current.hasDropCap)
						{
							current.yPos = maxDY;
							maxDY = 0;
							current.hasDropCap = false;
						}
					}

//					if (style.alignment() != 0)
					{
						if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
							current.line.width += opticalRightMargin(itemText, current.line);

						OFs = 0;
						if (style.alignment() == ParagraphStyle::Rightaligned)
							OFs = current.line.width - current.line.naturalWidth;
						if (style.alignment() == ParagraphStyle::Centered)
							OFs = (current.line.width - current.line.naturalWidth) / 2;
						if (style.alignment() == ParagraphStyle::Justified)
							OFs = 0;
						
						if (style.alignment() == ParagraphStyle::Extended
							|| (style.alignment() == ParagraphStyle::Justified 
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
							double optiWidth = current.colRight - style.rightMargin() - style.lineSpacing()/2.0 - current.line.x;
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
					if (current.breakIndex >= 0)
						a = current.breakIndex;
					assert( a >= 0 );
					assert( a < itemText.length() );
					hl = itemText.item(a);
					current.itemsInLine = a - current.line.firstItem + 1;
					if (current.addLine)
					{
						// go back to last break position
						style = itemText.paragraphStyle(a);
						if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
							style.setLineSpacing(charStyle.font().height(hlcsize10));
						else if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
							style.setLineSpacing(m_Doc->typographicSettings.valueBaseGrid);
						
						if (hl->ch == ' ') {
							hl->setEffects(hl->effects() | ScStyle_SuppressSpace);
							hl->glyph.xadvance = 0;
						}
						
						current.updateHeightMetrics(itemText);
						//current.updateLineOffset(itemText, style, firstLineOffset());
						//current.xPos = current.breakXPos;
						EndX = current.endOfLine(cl, current.rightMargin, maxYAsc, maxYDesc);
						current.finishLine(EndX);
						
						hyphWidth = 0.0;
						if ((hl->effects() & ScStyle_HyphenationPossible) || hl->ch == SpecialChars::SHYPHEN)
						{
							// insert hyphen
							if (current.lastInRowLine)
								//increase hyphen count only for hyphens a the end of text row, omit hyphens before overflow
								current.hyphenCount++;
							hl->setEffects(hl->effects() | ScStyle_SmartHyphenVisible);
							hl->glyph.grow();
							hl->glyph.more->glyph = charStyle.font().char2CMap(QChar('-'));
							hl->glyph.more->xadvance = charStyle.font().charWidth('-', itemText.charStyle(a).fontSize() / 10.0) * scaleH; //FIX ME - hyphen is not rendered with proper width - check yhis with large glyphs horizontal scaling eg. 20%
							hyphWidth = hl->glyph.more->xadvance;
						}
						else
						{
							if (hl->ch != '-')
								current.hyphenCount = 0;
							hl->setEffects(hl->effects() & ~ScStyle_SmartHyphenVisible);
							hl->glyph.shrink();
						}
						
						// Justification
						if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
							current.line.width += opticalRightMargin(itemText, current.line);
						else
							current.line.naturalWidth += hyphWidth;
						
						OFs = 0;
						if (style.alignment() == ParagraphStyle::Rightaligned)
							OFs = current.line.width - current.line.naturalWidth;
						if (style.alignment() == ParagraphStyle::Centered)
							OFs = (current.line.width - current.line.naturalWidth) / 2;
						
						if ((style.alignment() == ParagraphStyle::Justified) || (style.alignment() == ParagraphStyle::Extended))
							justifyLine(itemText, current.line);
						else
						{
							if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
								current.line.naturalWidth += opticalRightMargin(itemText, current.line);
							indentLine(itemText, current.line, OFs);
						}
						current.xPos = current.line.x + current.line.width;
					}
				}
				if (inOverflow)
					current.xPos = realEnd;

				//line break or end of column
				if (( hl->ch == SpecialChars::PARSEP || hl->ch == SpecialChars::LINEBREAK)
				    && current.hasDropCap)
				{
					current.hasDropCap = false;
					if (current.yPos < maxDY)
						current.yPos = maxDY;
					maxDX = 0;
				}
				if (current.isEndOfCol(desc))
				{
					//check if realy line extends bottom margin
					current.updateHeightMetrics(itemText);
					if (current.isEndOfCol(current.line.descent))
					{
						if (current.itemsInLine == 0 || current.column+1 == Cols)
						{
							goNoRoom = true;
							MaxChars = a + 1;
							break;
						}
						goNextColumn = true;
					}
				}
				if (current.line.firstItem <= current.line.lastItem && current.itemsInLine > 0)
				{
					if (current.addLine && current.breakIndex >= 0)
					{
						if (itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap)
						{
							// put line back to top
							current.line.y -= DropCapDrop;
							itemText.item(current.line.firstItem)->glyph.yoffset += DropCapDrop;
						}
						fillInTabLeaders(itemText, current.line);
						//if right margin is set we temporally save line, not append it
						itemText.appendLine(current.line);
						current.restartIndex = current.line.lastItem +1;
						a = current.restartIndex -1;
						current.rowDesc = qMax(current.rowDesc,current.yPos + current.line.descent);
						if (!current.lastInRowLine)
						{
							current.restartX = current.xPos;
							if (current.addLeftIndent)
							{
								current.addLeftIndent = false;
								current.wasFirstInRow = true;
							}
						}
					}
					else if (!inOverflow && !current.afterOverflow && current.lastInRowLine)
					{
						current.yPos++;
						a = current.restartLine(true, true);
						inOverflow = false;
						outs = false;
						current.mustLineEnd = current.colRight;
						continue;
					}
				}
				if (current.addLine && current.lastInRowLine)
				{
					current.recalculateY = true;
					current.wasFirstInRow = false;
					current.addLeftIndent = true;
					inOverflow = false;
					outs = false;
					current.startOfCol = false;
					current.restartX = current.xPos = qMax(maxDX, current.colLeft);
					lastLineY = current.rowDesc;
					if (current.hasDropCap)
					{
						++DropLinesCount;
						if (DropLinesCount >= DropLines)
						{
							current.hasDropCap = false;
							current.restartX = current.xPos = current.colLeft;
							maxDX = 0;
						}
					}
					lastLineY = current.rowDesc;
					current.mustLineEnd = current.colRight;
					current.restartRowIndex = current.restartIndex;
				}
				if ( SpecialChars::isBreak(hl->ch) )
				{
					if (hl->ch == SpecialChars::PARSEP)
						current.yPos += style.gapAfter();
					current.hyphenCount = 0;
				}
				if (inOverflow)
				{
					current.afterOverflow = true;
					inOverflow = false;
					current.addLeftIndent = false;
					current.recalculateY = false;
				}
				else
				{
					current.afterOverflow = false;
				}
				outs = false;
				current.addLine = false;
				current.lastInRowLine = false;
				current.startLine(a+1);
				if (goNoRoom)
				{
					goNoRoom = false;
					MaxChars = a+1;
					goto NoRoom;
				}
				if (goNextColumn)
				{
					goNextColumn = false;
					current.column++;
					if (current.column < Cols)
					{
						if (firstLineOffset() == FLOPRealGlyphHeight)
							asce = 0;
						current.nextColumn();
						current.mustLineEnd = current.colRight;
						current.addLeftIndent = true;
						lastLineY = extra.Top;
						current.rowDesc = 0;
						current.recalculateY = true;
					}
					else
					{
						MaxChars = a;
						goto NoRoom;
					}
				}
			}
			if (a == itemText.length() -1)
			{
				if (!current.afterOverflow || current.addLine)
				{
					current.addLine = true;
					current.lastInRowLine = true;
					current.breakLine(itemText, style, firstLineOffset(),a);
				}
				if (current.afterOverflow && !current.addLine)
				{
					if (current.restartIndex < current.line.firstItem)
					{
						a = current.restartLine(false,true);
						continue;
					}
					else
						current.breakLine(itemText, style, firstLineOffset(),a);
				}
			}
		}
		if (goNoRoom)
		{
			goNoRoom = false;
			goto NoRoom;
		}
// end of itemText
		if (current.itemsInLine > 0)
		{
			int a = itemText.length()-1;
			hl = (a >= 0) ? itemText.item(a) : NULL;
			current.breakLine(itemText, style, firstLineOffset(), a);

			int maxYAsc = 0, maxYDesc = 0;
			if (current.startOfCol)
			{
				//qMax is used for glyphs shifted or vertically scalled above ascent or below descent
				double addAsce = qMax(realAsce, asce);
				if (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing)
				{
					if (firstLineOffset() == FLOPRealGlyphHeight)
						addAsce = realAsce;
					else if (firstLineOffset() == FLOPLineSpacing)
						addAsce = style.lineSpacing();
				}
				maxYAsc = static_cast<int>(floor(current.yPos - addAsce));
			}
			else
				maxYAsc = static_cast<int>(floor(current.yPos - qMax(realAsce, asce)));
			maxYAsc = qMax(maxYAsc, 0);
			maxYDesc = static_cast<int>(ceil(current.yPos + qMax(realDesc, desc)));

			EndX = current.endOfLine(cl, style.rightMargin(), maxYAsc, maxYDesc);
			current.finishLine(EndX);

			if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
				current.line.width += opticalRightMargin(itemText, current.line);
			
			OFs = 0;
			if (style.alignment() == ParagraphStyle::Rightaligned)
				OFs = current.line.width - current.line.naturalWidth;
			if (style.alignment() == ParagraphStyle::Centered)
				OFs = (current.line.width - current.line.naturalWidth) / 2;
			if (style.alignment() == ParagraphStyle::Justified)
				OFs = 0;
			if (style.alignment() == ParagraphStyle::Extended
				|| (style.alignment() == ParagraphStyle::Justified 
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
			if ( itemText.charStyle(current.line.firstItem).effects() & ScStyle_DropCap )
			{
				// put line back to top
				current.line.y -= DropCapDrop;
				itemText.item(current.line.firstItem)->glyph.yoffset += DropCapDrop;
			}
			fillInTabLeaders(itemText, current.line);
			current.startOfCol = false;
			goNextColumn = false;

			itemText.appendLine(current.line);
			current.startOfCol = false;
		}
	}
	MaxChars = itemText.length();
	invalid = false;
	if (NextBox != NULL) 
	{
		PageItem_TextFrame* nextFrame = dynamic_cast<PageItem_TextFrame*>(NextBox);
		if (nextFrame != NULL)
		{
			nextFrame->invalid = true;
			nextFrame->firstChar = MaxChars;
		}
	}
//	qDebug("textframe: len=%d, done relayout", itemText.length());
	return;
			
NoRoom:     
	invalid = false;
	PageItem_TextFrame * next = dynamic_cast<PageItem_TextFrame*>(NextBox);
	if (next != NULL)
	{
		next->invalid = true;
		next->firstChar = MaxChars;
		if (itemText.cursorPosition() > signed(MaxChars))
		{
			int nCP = itemText.cursorPosition();
//			CPos = MaxChars;
			if (m_Doc->appMode == modeEdit)
			{
				//							OwnPage->Deselect(true);
				next->itemText.setCursorPosition( qMax(nCP, signed(MaxChars)) );
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

void PageItem_TextFrame::invalidateLayout()
{
	const bool wholeChain = true;
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

void PageItem_TextFrame::DrawObj_Item(ScPainter *p, QRectF cullingArea, double sc)
{
	if(invalid)
		layout();
	if (invalid)
		return;
	QMatrix pf2;
	QPoint pt1, pt2;
	double wide, lineCorr;
	QChar chstr0;
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
	p->save(); //SA1
	//	QRect e2;
	if (isEmbedded)
	{
	//	e2 = cullingArea;
	}
	else
	{
	//	e2 = QRect(qRound(cullingArea.x()  / sc + m_Doc->minCanvasCoordinate.x()), qRound(cullingArea.y()  / sc + m_Doc->minCanvasCoordinate.y()),
	//			   qRound(cullingArea.width() / sc), qRound(cullingArea.height() / sc));
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
	if ((isAnnotation()) && (annotation().Type() == 2) && (!Pfile.isEmpty()) && (PictureIsAvailable) && (PicArt) && (annotation().UseIcons()))
	{
		p->save();//SA2
		p->setupPolygon(&PoLine);
		p->setClipPath();
		p->scale(LocalScX, LocalScY);
		p->translate(LocalX*LocalScX, LocalY*LocalScY);
		if (pixm.width() > 0 && pixm.height() > 0)
			p->drawImage(pixm.qImagePtr());
		p->restore();//RE2
	}
	/* Experimental Addition to display an Image as Background */
	/*
	else if ((!Pfile.isEmpty()) && (PictureIsAvailable) && (PicArt))
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
		for (uint ll=0; ll < itemText.lines(); ++ll)
		{
			ls = itemText.line(ll);
			tabDist = ls.x;
			double CurX = ls.x;

			// Draw text selection rectangles
			QRectF selectedFrame;
			QList<QRectF> sFList;
			bool previousWasObject(false);
			double selX = ls.x;
			ScText *hls = 0;
			for (int as = ls.firstItem; as <= qMin(ls.lastItem, itemText.length() - 1); ++as)
			{
				bool selecteds = itemText.selected(as);
				hls = itemText.item(as);
				if(selecteds)
				{
					const CharStyle& charStyleS(itemText.charStyle(as));
					if(((as > ls.firstItem) && (charStyleS != itemText.charStyle(as-1)))
						|| ((!selectedFrame.isNull()) && (hls->ch == SpecialChars::OBJECT))
					    || previousWasObject)
					{
						sFList << selectedFrame;
						selectedFrame = QRectF();
						previousWasObject = false;
					}
					if (!m_Doc->RePos)
					{
						if (((selecteds && Select) || ((NextBox != 0 || BackBox != 0) && selecteds))
							&& (m_Doc->appMode == modeEdit))
						{
							double xcoZli = selX + hls->glyph.xoffset;
							// ugly hack to make selection correct, as xoffset is used to
							// remove left-half of CJK lparen , which is blank.
							if(hls->glyph.xoffset){
								int attr = SpecialChars::getCJKAttr(hls->ch) & SpecialChars::CJK_CHAR_MASK;
								if( attr == SpecialChars::CJK_FENCE_BEGIN){
									xcoZli -= hls->glyph.xoffset;
								}
							}
							desc = - charStyleS.font().descent(charStyleS.fontSize() / 10.0);
							asce = charStyleS.font().ascent(charStyleS.fontSize() / 10.0);
							wide = hls->glyph.wide();
							QRectF scr;
							if ((hls->ch == SpecialChars::OBJECT) && (hls->embedded.hasItem()))
							{
								double ww = (hls->embedded.getItem()->gWidth + hls->embedded.getItem()->lineWidth()) * hls->glyph.scaleH;
								double hh = (hls->embedded.getItem()->gHeight + hls->embedded.getItem()->lineWidth()) * hls->glyph.scaleV;
								scr = QRectF(xcoZli, ls.y - hh, ww , hh);
								previousWasObject = true;
							}
							else
								scr = QRectF(xcoZli, ls.y + hls->glyph.yoffset - asce * hls->glyph.scaleV, wide , (asce+desc) * (hls->glyph.scaleV));
							selectedFrame |=  scr;
						}
					}
				}
				if ((hls->ch == SpecialChars::OBJECT) && (hls->embedded.hasItem()))
					selX += (hls->embedded.getItem()->gWidth + hls->embedded.getItem()->lineWidth()) * hls->glyph.scaleH;
				else
					selX += hls->glyph.wide();
			}
			if(!selectedFrame.isNull())
				sFList << selectedFrame;
			p->save();//SA3
			p->setFillMode(1);
			p->setBrush(qApp->palette().color(QPalette::Active, QPalette::Highlight));
			p->setLineWidth(0);
			// TODO - I dunno why but scpainter does not accept
			// to actually set the pen to 0? As a wa,
			// we set its color same as brush: "à malin, malin et demi!".
			p->setPen(qApp->palette().color(QPalette::Active, QPalette::Highlight));
			for(int sfc(0);sfc < sFList.count();++sfc)
				p->drawRect(sFList[sfc].x(), sFList[sfc].y(), sFList[sfc].width(), sFList[sfc].height());
			p->restore();//RE3
			//	End of selection

			QColor tmp;
			ScText *hl = 0;
			for (int a = ls.firstItem; a <= qMin(ls.lastItem, itemText.length() - 1); ++a)
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
					p->setFillMode(ScPainter::Solid);
					if ((cachedFillShade != actFillShade) || (cachedFill != actFill))
					{
						SetQColor(&tmp, actFill, actFillShade);
						p->setBrush(tmp);
						cachedFillQ = tmp;
						cachedFill = actFill;
						cachedFillShade = actFillShade;
					}
					else
						p->setBrush(cachedFillQ);
				}
				else
					p->setFillMode(ScPainter::None);
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

				if (!m_Doc->RePos)
				{
					desc = - charStyle.font().descent(charStyle.fontSize() / 10.0);
					asce = charStyle.font().ascent(charStyle.fontSize() / 10.0);
					if (((selected && Select) || ((NextBox != 0 || BackBox != 0) && selected)) && (m_Doc->appMode == modeEdit))
					{
						// set text color to highlight if its selected
						p->setBrush(qApp->palette().color(QPalette::Active, QPalette::HighlightedText));
					}

					actStroke = charStyle.strokeColor();
					actStrokeShade = charStyle.strokeShade();
					if (actStroke != CommonStrings::None)
					{
						if ((cachedStrokeShade != actStrokeShade) || (cachedStroke != actStroke))
						{
							SetQColor(&tmp, actStroke, actStrokeShade);
							p->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
							cachedStrokeQ = tmp;
							cachedStroke = actStroke;
							cachedStrokeShade = actStrokeShade;
						}
						else
							p->setPen(cachedStrokeQ, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					}
					// paint glyphs
					if (isEmbedded || cullingArea.intersects(pf2.mapRect(QRect(qRound(CurX + hl->glyph.xoffset),qRound(ls.y + hl->glyph.yoffset-asce), qRound(hl->glyph.xadvance+1), qRound(asce+desc)))))
					{
						p->save();//SA4
						p->translate(CurX, ls.y);
						if (hl->ch == SpecialChars::OBJECT)
							DrawObj_Embedded(p, cullingArea, charStyle, hl->embedded.getItem());
						else
							drawGlyphs(p, charStyle, hl->glyph);
						p->restore();//RE4
					}
					if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
						CurX += (hl->embedded.getItem()->gWidth + hl->embedded.getItem()->lineWidth()) * hl->glyph.scaleH;
					else
						CurX += hl->glyph.wide();
				}
				tabDist = CurX;
			}
		}
	//	else {
	//		//		qDebug("skipping textframe: len=%d", itemText.count());
	//	}
		//	pf2.end();
	}
	p->restore();//RE1
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
			if (!isTableItem)
			{
				p->setupPolygon(&PoLine);
				if (NamedLStyle.isEmpty())
				{
					if (lineColor() != CommonStrings::None)
					{
						p->setPen(strokeQColor, m_lineWidth, PLineArt, PLineEnd, PLineJoin);
						if (DashValues.count() != 0)
							p->setDash(DashValues, DashOffset);
						p->strokePath();
					}
				}
				else
				{
					multiLine ml = m_Doc->MLineStyles[NamedLStyle];
					QColor tmp;
					for (int it = ml.size()-1; it > -1; it--)
					{
						SetQColor(&tmp, ml[it].Color, ml[it].Shade);
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
		// added to prevent fat frame outline due to antialiasing and considering you can’t pass a cosmetic pen to scpainter - pm
		double aestheticFactor(5.0);
		double scpInv = 1.0 / (qMax(view->scale(), 1.0) * aestheticFactor);
		if ((Frame) && (m_Doc->guidesSettings.framesShown))
		{
			p->setPen(PrefsManager::instance()->appPrefs.DFrameNormColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if ((isBookmark) || (m_isAnnotation))
				p->setPen(PrefsManager::instance()->appPrefs.DFrameAnnotationColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if ((BackBox != 0) || (NextBox != 0))
				p->setPen(PrefsManager::instance()->appPrefs.DFrameLinkColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if (m_Locked)
				p->setPen(PrefsManager::instance()->appPrefs.DFrameLockColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

			p->setFillMode(0);
// Ugly Hack to fix rendering problems with cairo >=1.5.10 && <1.8.0 follows
#ifdef HAVE_CAIRO
	#if ((CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 5, 10)) && (CAIRO_VERSION < CAIRO_VERSION_ENCODE(1, 8, 0)))
			p->setupPolygon(&PoLine, false);
	#else
			p->setupPolygon(&PoLine);
	#endif
#else
			p->setupPolygon(&PoLine);
#endif
			p->strokePath();
		}
		if ((m_Doc->guidesSettings.framesShown) && textFlowUsesContourLine() && (ContourLine.size() != 0))
		{
			p->setPen(Qt::lightGray, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
// Ugly Hack to fix rendering problems with cairo >=1.5.10 && <1.8.0 follows
#ifdef HAVE_CAIRO
	#if ((CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 5, 10)) && (CAIRO_VERSION < CAIRO_VERSION_ENCODE(1, 8, 0)))
			p->setupPolygon(&ContourLine, false);
	#else
			p->setupPolygon(&ContourLine);
	#endif
#else
			p->setupPolygon(&ContourLine);
#endif
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
		if ((m_Doc->guidesSettings.layerMarkersShown) && (m_Doc->layerCount() > 1) && (!m_Doc->layerOutline(LayerNr)) && (!view->m_canvas->isPreviewMode()))
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
	nextItem->itemTextSaxed = nextItem->getItemTextSaxed(PageItem::FRAME); //for undo
	ParagraphStyle defaultStyle = nextItem->itemText.defaultStyle();
	nextItem->itemText.clear();
	nextItem->itemText.setDefaultStyle(defaultStyle);
	nextItem->asTextFrame()->updateUndo(); //for undo
	while (nextItem != 0)
	{
		nextItem->invalid = true;
		nextItem = nextItem->nextInChain();
	}
}

void PageItem_TextFrame::handleModeEditKey(QKeyEvent *k, bool& keyRepeat)
{
	if (frameUnderflows())
	{
		itemText.setCursorPosition( itemText.length() );
		PageItem * jumpFrame = frameTextEnd();
		if (jumpFrame)
		{
			m_Doc->view()->Deselect(true);
			m_Doc->scMW()->selectItemsFromOutlines(jumpFrame);
			m_Doc->scMW()->setTBvals(jumpFrame);
			jumpFrame->update();
			update();
			switch (k->key())
			{
			case Qt::Key_PageDown:
			case Qt::Key_PageUp:
			case Qt::Key_End:
			case Qt::Key_Home:
			case Qt::Key_Right:
			case Qt::Key_Left:
			case Qt::Key_Up:
			case Qt::Key_Down:
			case Qt::Key_Delete:
			case Qt::Key_Backspace:
				break;
			default:
				jumpFrame->handleModeEditKey(k, keyRepeat);
				jumpFrame->layout();
				break;
			}
			return;
		}
	}
	int oldPos = itemText.cursorPosition(); // 15-mar-2004 jjsa for cursor movement with Shift + Arrow key
	int kk = k->key();
	int as = k->text()[0].unicode();
	QString uc = k->text();
	QString cr, Tcha, Twort;
	uint Tcoun;
	int len, pos;
	int keyModifiers=0;
	Qt::KeyboardModifiers buttonModifiers = k->modifiers();
	if (k->modifiers() & Qt::ShiftModifier)
		keyModifiers |= Qt::SHIFT;
	if (k->modifiers() & Qt::ControlModifier)
		keyModifiers |= Qt::CTRL;
	if (k->modifiers() & Qt::AltModifier)
		keyModifiers |= Qt::ALT;

	//<< ISO 14755
	//Check if we are trying to enter Unicode sequence mode first
	QKeySequence currKeySeq = QKeySequence(k->key() | keyModifiers);
	if(currKeySeq.matches(doc()->scMW()->scrActions["specialUnicodeSequenceBegin"]->shortcut())==QKeySequence::ExactMatch)
	{
		unicodeTextEditMode = true;
		unicodeInputCount = 0;
		unicodeInputString = "";
		keyRepeat = false;
		return;
	}
	//>>
	
	ScribusView* view = m_Doc->view();	
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
		lastUndoAction = PageItem::NOACTION;
	}

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
				{
					itemTextSaxed = getItemTextSaxed(PageItem::SELECTION);
					deleteSelectedTextFromFrame();
					lastUndoAction = PageItem::NOACTION;
				}
				if (conv < 31)
					conv = 32;
				if (conv == 32)
				{
					qDebug() << "SPACE conv?";
					lastUndoAction = PageItem::NOACTION;
				}
				oldCPos = itemText.cursorPosition();
				itemText.insertChars(QString(QChar(conv)), true);
				if (itemTextSaxed.isEmpty())
					updateUndo(INS, QString(QChar(conv)));
				else
					updateUndo(REPSAX, getTextSaxed(QString(QChar(conv))));
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

	// #7430 hack : inverted selection with RTL writing enabled
	if (reversed())
	{  //inverting left and rigt keys
		if (kk == Qt::Key_Left) 
			kk = Qt::Key_Right;
		else if (kk == Qt::Key_Right) 
			kk = Qt::Key_Left;
	}

	int oldLast = lastInFrame();
	switch (kk)
	{
	case Qt::Key_Home:
		// go to begin of line
		if ( (pos = itemText.cursorPosition()) == firstInFrame() )
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
		itemText.setCursorPosition(pos);
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(-1, oldPos);
//		if ( this->itemText.lengthOfSelection() > 0 )
//			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_End:
		// go to end of line
		len = lastInFrame();
		if ( itemText.cursorPosition() >= len )
			break; // at end of frame
		if ( (buttonModifiers & Qt::ControlModifier) == 0 )
		{
			itemText.setCursorPosition( itemText.endOfLine(itemText.cursorPosition()) );
		}
		else
		{
			//Control End for end of frame text
			itemText.setCursorPosition( itemText.endOfFrame(itemText.cursorPosition()) );
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
			itemText.setCursorPosition(itemText.nextParagraph(itemText.cursorPosition()));
			if ( buttonModifiers & Qt::ShiftModifier )
				ExpandSel(1, oldPos);
		}
		else
		{
			if (itemText.cursorPosition() <= lastInFrame())
			{
				itemText.setCursorPosition(itemText.nextLine(itemText.cursorPosition()));
				if ( buttonModifiers & Qt::ShiftModifier )
				{
					if ( buttonModifiers & Qt::AltModifier )
						itemText.setCursorPosition(lastInFrame() + 1);
					ExpandSel(1, oldPos);
				}
				else 
					if ((itemText.lines() > 0) && (oldPos >= itemText.line(itemText.lines()-1).firstItem) && (itemText.cursorPosition() >= lastInFrame()) && (NextBox != 0))
					{
						if (NextBox->frameDisplays(itemText.cursorPosition()))
						{
							view->Deselect(true);
							// we position the cursor at the beginning of the next frame
							// TODO position at the right place in next frame
							m_Doc->scMW()->selectItemsFromOutlines(NextBox);
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
						m_Doc->scMW()->selectItemsFromOutlines(NextBox);
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
			if ( (pos = itemText.cursorPosition()) == firstInFrame() )
				break; // at begin of frame
			len = itemText.length();
			itemText.setCursorPosition( itemText.prevParagraph(itemText.cursorPosition()) );
			if ( buttonModifiers & Qt::ShiftModifier )
				ExpandSel(-1, oldPos);
		}
		else
		{
			if (itemText.cursorPosition() > firstInFrame())
			{
				if (itemText.cursorPosition() > lastInFrame() || itemText.cursorPosition() >= itemText.length())
					itemText.setCursorPosition( lastInFrame() );
				itemText.setCursorPosition( itemText.prevLine(itemText.cursorPosition()) );
				if ( buttonModifiers & Qt::ShiftModifier )
				{
					if ( buttonModifiers & Qt::AltModifier )
						itemText.setCursorPosition( firstInFrame() );
					ExpandSel(-1, oldPos);
				}
				else
					if ((itemText.lines() > 0) && (oldPos <= itemText.line(0).lastItem) && (itemText.cursorPosition() == firstInFrame()) && (BackBox != 0))
					{
						view->Deselect(true);
						// TODO position at the right place in previous frame
						BackBox->itemText.setCursorPosition( BackBox->lastInFrame() );
						m_Doc->scMW()->selectItemsFromOutlines(BackBox);
					}
			}
			else
			{
				itemText.setCursorPosition( firstInFrame() );
				if (BackBox != 0)
				{
					view->Deselect(true);
					BackBox->itemText.setCursorPosition( BackBox->lastInFrame() );
					m_Doc->scMW()->selectItemsFromOutlines(BackBox);
				}
			}
		}
//		if ( this->itemText.lengthOfSelection() > 0 )
//			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_PageUp:
		if (itemText.cursorPosition() == firstInFrame() && BackBox != 0)
		{
			view->Deselect(true);
			BackBox->itemText.setCursorPosition( BackBox->firstInFrame() );
			m_Doc->scMW()->selectItemsFromOutlines(BackBox);
			//currItem = currItem->BackBox;
		}
		else
			itemText.setCursorPosition( itemText.startOfFrame(itemText.cursorPosition()) );
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(-1, oldPos);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_PageDown:
		if (!frameDisplays(itemText.length()-1) && itemText.cursorPosition() >= lastInFrame() && NextBox != 0)
		{
			view->Deselect(true);
			NextBox->itemText.setCursorPosition( NextBox->lastInFrame() );
			m_Doc->scMW()->selectItemsFromOutlines(NextBox);
			//currItem = currItem->BackBox;
		}
		else
			itemText.setCursorPosition( itemText.endOfFrame(itemText.cursorPosition()) );
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
			int pos = itemText.cursorPosition();
			itemText.setCursorPosition(-1, true);
			if ( pos > 0 )
				ExpandSel(-1, oldPos);
		}
		else
		{
			itemText.setCursorPosition(-1, true);
			if (itemText.cursorPosition() < firstInFrame())
			{
				itemText.setCursorPosition( firstInFrame() );
				if (BackBox != 0)
				{
					view->Deselect(true);
					BackBox->itemText.setCursorPosition( BackBox->lastInFrame() );
					m_Doc->scMW()->selectItemsFromOutlines(BackBox);
					//currItem = currItem->BackBox;
				}
			}
		}
		if ((itemText.cursorPosition() > 0) && (itemText.cursorPosition() >= lastInFrame())) // I do not see how its possible, may be dead code - pm
		{
			itemText.setCursorPosition( lastInFrame() );
//			if (itemText.charStyle(CPos-1).effects() & ScStyle_SuppressSpace)
//			{
//				--CPos;
				while ((itemText.cursorPosition() > 1) && (itemText.charStyle().effects() & ScStyle_SuppressSpace) && (itemText.charStyle(itemText.cursorPosition() - 1).effects() & ScStyle_SuppressSpace))
				{
					itemText.setCursorPosition(-1, true);
					if (itemText.cursorPosition() == 0)
						break;
				}
//			}
		}
		else
		{
			while ((itemText.cursorPosition() > 1) && (itemText.charStyle().effects() & ScStyle_SuppressSpace) && (itemText.charStyle(itemText.cursorPosition() - 1).effects() & ScStyle_SuppressSpace))
			{
				itemText.setCursorPosition(-1, true);
				if (itemText.cursorPosition() == 0)
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
			int pos = itemText.cursorPosition();
			itemText.setCursorPosition(1, true);
			if ( pos < itemText.length() )
				ExpandSel(1, oldPos);
		}
		else
		{
			itemText.setCursorPosition(1, true); // new position within text ?
			if (itemText.cursorPosition() > lastInFrame())
			{
//				--CPos;
				itemText.setCursorPosition(lastInFrame() + 1);
				if (NextBox != 0)
				{
					if (NextBox->frameDisplays(itemText.cursorPosition()))
					{
						view->Deselect(true);
						m_Doc->scMW()->selectItemsFromOutlines(NextBox);
						//currItem = currItem->NextBox;
					}
				}
			}
		}
// 		if ( itemText.lengthOfSelection() > 0 )
// 			update();
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_Delete:
		if (itemText.cursorPosition() == itemText.length())
		{
			if (itemText.lengthOfSelection() > 0)
			{
				oldCPos = itemText.startOfSelection();
				itemTextSaxed = getItemTextSaxed(PageItem::SELECTION);
				deleteSelectedTextFromFrame();
				updateUndo(DELSAX);
				m_Doc->scMW()->setTBvals(this);
				update();
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
		cr = itemText.text();
		if (itemText.lengthOfSelection() == 0)
		{
			itemText.select(itemText.cursorPosition(), 1, true);
			HasSel = true;
		}
		oldCPos = itemText.startOfSelection();
		itemTextSaxed = getItemTextSaxed(PageItem::SELECTION);
		deleteSelectedTextFromFrame();
		updateUndo(DELSAX);
		update();
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
		if (itemText.cursorPosition()  == 0)
		{
			if (itemText.lengthOfSelection() > 0)
			{
				oldCPos = itemText.startOfSelection();
				itemTextSaxed = getItemTextSaxed(PageItem::SELECTION);
				deleteSelectedTextFromFrame();
				updateUndo(DELSAX);
				m_Doc->scMW()->setTBvals(this);
				update();
			}
			break;
		}
		if (itemText.length() == 0)
			break;
		cr = itemText.text(qMax(itemText.cursorPosition() - 1,0),1);
		if (itemText.lengthOfSelection() == 0)
		{
			itemText.setCursorPosition(-1, true);
			itemText.select(itemText.cursorPosition(), 1, true);
			HasSel = true;
		}
		oldCPos = itemText.startOfSelection();
		itemTextSaxed = getItemTextSaxed(PageItem::SELECTION);
		deleteSelectedTextFromFrame();
		updateUndo(DELSAX);
//		Tinput = false;
		if ((cr == QChar(13)) && (itemText.length() != 0))
		{
//			m_Doc->chAbStyle(this, findParagraphStyle(m_Doc, itemText.paragraphStyle(qMax(itemText.cursorPosition()-1,0))));
//			Tinput = false;
		}
		updateLayout();
		if (oldLast != lastInFrame() && NextBox != 0 && NextBox->invalid)
			NextBox->updateLayout();
		if (itemText.cursorPosition() < firstInFrame())
		{
			itemText.setCursorPosition(firstInFrame());
			if (BackBox != 0)
			{
				lastUndoAction = PageItem::NOACTION;
				view->Deselect(true);
				if (BackBox->invalid)
					BackBox->updateLayout();
				BackBox->itemText.setCursorPosition( BackBox->lastInFrame() );
				m_Doc->scMW()->selectItemsFromOutlines(BackBox);
			}
		}
		m_Doc->scMW()->setTBvals(this);
		update();
//		view->RefreshItem(this);
		break;
	default:
		bool doUpdate = false;
		if (itemText.lengthOfSelection() > 0) // && (kk < 0x1000)
		{
			bool x11Hack=false;
			bool controlCharHack=false;
			if ((k->text().size() == 1))
			{
				ushort uni = k->text().at(0).unicode();
				controlCharHack = ((uni < 32) && (uni != SpecialChars::TAB.unicode()));
			}
#if defined (Q_WS_X11)
			if ((k->text().size()==1) && (k->modifiers() & Qt::ShiftModifier) && (k->modifiers() & Qt::ControlModifier) && (k->nativeVirtualKey() < 1000))
				x11Hack=true;
#endif
			if (!controlCharHack && !x11Hack && !k->text().isEmpty())
			{
				oldCPos = itemText.startOfSelection();
				itemTextSaxed = getItemTextSaxed(PageItem::SELECTION);
				deleteSelectedTextFromFrame();
				lastUndoAction = PageItem::NOACTION;
				doUpdate = true;
			}
			/*
			qDebug()<<"Key:"<<k->key();
			qDebug()<<"Text:"<<k->text();
			qDebug()<<"Size:"<<k->text().size();
			qDebug()<<"Modifiers:"<<k->modifiers();
			qDebug()<<"Native Modifiers:"<<k->nativeModifiers();
			qDebug()<<"Native Scan Code:"<<k->nativeScanCode();
			qDebug()<<"Native Virtual Key:"<<k->nativeVirtualKey();
			*/
		}
		//if ((kk == Qt::Key_Tab) || ((kk == Qt::Key_Return) && (buttonState & Qt::ShiftButton)))
		if (kk == Qt::Key_Tab)
		{
			oldCPos = itemText.cursorPosition();
			itemText.insertChars(QString(SpecialChars::TAB), true);
			lastUndoAction = PageItem::NOACTION;
			if (itemTextSaxed.isEmpty())
				updateUndo(INS, QString(SpecialChars::TAB));
			else
				updateUndo(REPSAX, getTextSaxed(QString(SpecialChars::TAB)));
//			Tinput = true;
//			view->RefreshItem(this);
			doUpdate = true;
		}
		else if ((uc[0] > QChar(31) && m_Doc->currentStyle.charStyle().font().canRender(uc[0])) || (as == 13) || (as == 30))
		{
			if (uc[0] == QChar(32) || uc[0] == QChar(13))
				lastUndoAction = PageItem::NOACTION;
			if (lastUndoAction != PageItem::INS)
				oldCPos = itemText.cursorPosition();
			itemText.insertChars(uc, true);
			if (itemTextSaxed.isEmpty())
				updateUndo(INS, uc);
			else
				updateUndo(REPSAX, getTextSaxed(uc));
			if ((m_Doc->docHyphenator->AutoCheck) && (itemText.cursorPosition() > 1))
			{
				Twort = "";
				Tcoun = 0;
				for (int hych = itemText.cursorPosition()-1; hych > -1; hych--)
				{
					Tcha = itemText.text(hych, 1);
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
			doUpdate = true;
		}
		if (doUpdate)
		{
			// update layout immediately, we need MaxChars to be correct to detect 
			// if we need to move to next frame or not
			updateLayout();
			if (oldLast != lastInFrame() && NextBox != 0 && NextBox->invalid)
				NextBox->updateLayout();
		}
		//check if cursor need to jump to next linked frame
		if ((itemText.cursorPosition() > lastInFrame() + 1) && (lastInFrame() < (itemText.length() - 2)) && NextBox != 0)
		{
			lastUndoAction = PageItem::NOACTION;
			view->Deselect(true);
			NextBox->update();
			m_Doc->scMW()->selectItemsFromOutlines(NextBox);
		}
		break;
	}
// 	update();
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
		itemText.setCursorPosition( itemText.startOfSelection() );
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
		itemText.setCursorPosition(oldPos + 1);
		while (itemText.cursorPosition() < len)
		{
			isSpace = itemText.text().isSpace();
			if (wasSpace && !isSpace)
				break;
			itemText.setCursorPosition(1, true);
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
		itemText.setCursorPosition(oldPos - 1);
		wasSpace = itemText.text().isSpace();
		while (itemText.cursorPosition() > 0)
		{
			isSpace = itemText.text().isSpace();
			if (!wasSpace && isSpace)
			{
				itemText.setCursorPosition(1, true);
				break;
			}
			itemText.setCursorPosition(-1, true);
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
//  if value is +/-2 refresh if text under cursor is selected -- not used

void PageItem_TextFrame::ExpandSel(int dir, int oldPos)
{
	int start = itemText.startOfSelection();
	// itemText.endOfSelection can return -1 in some case, which results in a crash
	// when calling itemText.select(end, CPos - end).
	// as I do not know where it is checked for negative value, just work around by
	// preventing it to be less than 0 here.
	int end = qMax(0,itemText.endOfSelection());
	
	if (oldPos >= end && itemText.cursorPosition() < start)
	{
		itemText.deselectAll();
		itemText.select(itemText.cursorPosition(), start - itemText.cursorPosition());
	}
	else if (oldPos <= start && itemText.cursorPosition() > end)
	{
		itemText.deselectAll();
		itemText.select(end, itemText.cursorPosition() - end);
	}
	else
	{
		itemText.extendSelection(oldPos, itemText.cursorPosition());
	}
	HasSel = (itemText.lengthOfSelection() > 0);
	if (HasSel)
	{
		//CB Replace with direct call for now //emit HasTextSel();
		m_Doc->scMW()->EnableTxEdit();
		
	}
	else
	{
		//CB Replace with direct call for now //emit  HasNoTextSel();
		m_Doc->scMW()->DisableTxEdit();
	}
	cursorBiasBackward = (oldPos > itemText.cursorPosition());

// 	layoutWeakLock = true;
// 	update();
	m_Doc->regionsChanged()->update(getBoundingRect());
	lastUndoAction = PageItem::NOACTION;
}

void PageItem_TextFrame::ExpandParSel() //expand selection to whole paragrpah(s)
{
	if (m_Doc->appMode != modeEdit)
		return;
	int StartSel = 0, LenSel = 0;
	if (HasSel)
	{
		//extend selection to whole paragraphs
		StartSel = itemText.startOfParagraph(itemText.nrOfParagraph(itemText.startOfSelection()));
		LenSel = itemText.endOfParagraph(itemText.nrOfParagraph(itemText.endOfSelection())) - StartSel;
	}
	else
	{
		//extend selection to whole paragraph
		StartSel = itemText.startOfParagraph();
		LenSel = itemText.endOfParagraph() - StartSel;
	}
	itemText.select(StartSel, LenSel);
	HasSel = true;
}

void PageItem_TextFrame::expandParaSelection(bool includeEOL)
{
	int selStart, selLength;
	if (HasSel)
	{
		//extend selection to whole paragraphs
		selStart  = itemText.startOfParagraph(itemText.nrOfParagraph(itemText.startOfSelection()));
		selLength = itemText.endOfParagraph(itemText.nrOfParagraph(itemText.endOfSelection())) - selStart;
	}
	else
	{
		//extend selection to whole paragraph
		selStart  = itemText.startOfParagraph();
		selLength = itemText.endOfParagraph() - selStart;
	}
	if (includeEOL)
		selLength += 1;
	selLength = qMin(selLength, itemText.length() - selStart);

	itemText.select(selStart, selLength);
	HasSel = true;
}

void PageItem_TextFrame::deselectAll()
{
	if ( itemText.lengthOfSelection() > 0 )
	{
		itemText.deselectAll();
		PageItem *item = this;
		while( item->prevInChain() )
			item=item->prevInChain();

		while ( item )
		{
			item->HasSel = false;
			item = item->nextInChain();
		}
// 		update();
		m_Doc->regionsChanged()->update(getBoundingRect());
	}
	//CB Replace with direct call for now //emit HasNoTextSel();
	m_Doc->scMW()->DisableTxEdit();
	lastUndoAction = PageItem::NOACTION;
}

double PageItem_TextFrame::columnWidth()
{
	double lineCorr;
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth / 2.0;
	else
		lineCorr = 0;
	return (Width - (ColGap * (Cols - 1)) - Extra - RExtra - 2 * lineCorr) / Cols;
//	return (Width - (ColGap * (Cols - 1)) - Extra - RExtra - lineCorr) / Cols;
}

void PageItem_TextFrame::drawOverflowMarker(ScPainter *p)
{
	//TODO: CB clean
	ScribusView* view = m_Doc->view();
	
	double sideLength = 10 / qMax(p->zoomFactor(), 1.0);
	double left   = Width  - sideLength / 2;
	double right  = left   + sideLength;
	double top    = Height - sideLength * 1.5;
	double bottom = top    + sideLength;

	QColor color(PrefsManager::instance()->appPrefs.DFrameNormColor);
	if ((isBookmark) || (m_isAnnotation))
		color = PrefsManager::instance()->appPrefs.DFrameAnnotationColor;
	if ((BackBox != 0) || (NextBox != 0))
		color = PrefsManager::instance()->appPrefs.DFrameLinkColor;
	if (m_Locked)
		color = PrefsManager::instance()->appPrefs.DFrameLockColor;
	if (m_Doc->m_Selection->containsItem(this))
		color = Qt::red;

	p->setPen(color, 0.5 / view->scale(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setPenOpacity(1.0);
	p->setBrush(Qt::white);
	p->setBrushOpacity(1.0);
	p->setFillMode(ScPainter::Solid);
	p->drawRect(left, top, sideLength, sideLength);
	p->drawLine(FPoint(left, top), FPoint(right, bottom));
	p->drawLine(FPoint(left, bottom), FPoint(right, top));
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

/*
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
*/

void PageItem_TextFrame::applicableActions(QStringList & actionList)
{
	actionList << "fileImportText";
	actionList << "fileImportAppendText";
	actionList << "toolsEditWithStoryEditor";
	actionList << "insertSampleText";
	actionList << "itemPDFIsAnnotation";
	if (doc()->currentPage()->pageName().isEmpty())
		actionList << "itemPDFIsBookmark";
	if (isAnnotation())
	{
		if ((annotation().Type() == 0) || (annotation().Type() == 1) || (annotation().Type() > 9))
			actionList << "itemPDFAnnotationProps";
		else
			actionList << "itemPDFFieldProps";
	}
	if (isTableItem)
		actionList << "itemConvertToImageFrame";
	else 
	{
		if ((prevInChain() == 0) && (nextInChain() == 0))
		{
			actionList << "itemConvertToImageFrame";
			actionList << "itemConvertToPolygon";
		}
		actionList << "itemConvertToOutlines";
	}
	if (itemText.lines() != 0)
		actionList << "editClearContents";
}

QString PageItem_TextFrame::infoDescription()
{
	return QString();
}

void PageItem_TextFrame::slotInvalidateLayout()
{
	invalidateLayout();
}

void PageItem_TextFrame::updateUndo(EditAct action, QString str)
{
	if (UndoManager::undoEnabled() && undoManager->undoEnabled())
	{
		int oldSelStart = -1, oldSelLen = -1;
		oldSelLen = itemText.lengthOfSelection();
		if (oldSelLen > 0)
			oldSelStart = itemText.startOfSelection();
		if (action == PARAMFULL && m_Doc->appMode == modeEdit)
		{
			//action is for paragraph where cursor is
			expandParaSelection(true);
			action = PARAMSEL;
		}
		if (itemText.cursorPosition() >= itemText.length() && itemTextSaxed.isEmpty() && action == PARAMFULL && m_Doc->appMode == modeEdit)
		{ 
			//case when cursor is after last character without selection and nothing was and will be done
			//changes will be ignored
			lastUndoAction = PageItem::NOACTION;
			restoreTextSelection(oldSelStart, oldSelLen);
			return;
		}
		SimpleState* ss;
		bool newState = true;  // indicate when new undoState should be created
		if (lastUndoAction == action && action != REPSAX && action != DELSAX)
		{
			ss = (SimpleState*) undoManager->getLastUndoState();
			if (ss && ss->undoObject() == this)
			{
				if (action == PARAMFULL || action == PARAMSEL)
				{
					newState = false;
					if (itemTextSaxed.length() > 0)
					{
						/*QString tmpStr = ss->get("STEXT_NEW");
						newState = (tmpStr.length() > 0 && tmpStr != itemTextSaxed);*/
						newState = true;
					}
					if (newState == false)
					{
						itemTextSaxed = ss->get("STEXT_OLD");
					}
				}
				else if (action == INSSAX || action == INS)
				{
					QString tmpstr(ss->get("STEXT_STR"));
					tmpstr.append(str);
					str = tmpstr;
					newState = false;
				}
			}
		}
		if (newState)
		{
			ss = new SimpleState(UndoManager::EditText);
			ss->set("STEXT_CPOS", oldCPos);
		}
		if (action == INSSAX || action == INS)
			ss->set("STEXT_STR",str);
		else if (action == DELSAX)
			ss->set("STEXT_STR",itemTextSaxed);
		else if (action == REPSAX)
		{
			ss->set("STEXT_NEW",str);
			ss->set("STEXT_OLD",itemTextSaxed);
		}
		else
		{
			if (action == PARAMFULL && m_Doc->appMode == modeEdit)
			{
				//action is for paragraph where cursor is
				expandParaSelection(true);
				action = PARAMSEL;
			}
			if (action == PARAMSEL)
			{
				ss->set("STEXT_CPOS", itemText.cursorPosition());
				ss->set("STEXT_SELSTART", itemText.startOfSelection());
				ss->set("STEXT_SELLEN", itemText.lengthOfSelection());
			}
			ss->set("STEXT_OLD", itemTextSaxed);
			itemTextSaxed = getItemTextSaxed((action == PARAMSEL) ? SELECTION : FRAME);
			ss->set("STEXT_NEW", itemTextSaxed);
			if (QString::compare(ss->get("STEXT_OLD"),ss->get("STEXT_NEW")) == 0)
			{
				//nothing change - quit without set new Undo step
				itemTextSaxed.clear();
				restoreTextSelection(oldSelStart, oldSelLen);
				if (newState)
					delete ss;
				return;
			}
		}
		itemTextSaxed.clear();
		lastUndoAction = action;
		ss->set("STEXT",(int) action);
		if (newState)
			undoManager->action(this, ss);
		// Restore user selection
		restoreTextSelection(oldSelStart, oldSelLen);
	}
}

void PageItem_TextFrame::restoreTextSelection(int oldSelStart, int oldSelLength)
{
	itemText.deselectAll();
	if (oldSelLength > 0)
		itemText.select(oldSelStart, oldSelLength);
	else if (oldSelLength == 0)
		itemText.deselectAll();
	HasSel = (oldSelLength > 0);
}
