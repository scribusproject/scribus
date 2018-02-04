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

#include "pageitem_textframe.h"

#include <QDebug>
#include <QList>
#include <QTransform>
#include <QPalette>
#include <QPoint>
#include <QPolygon>
#include <QRegion>
#include <cairo.h>
#include <cassert>

#include "actionmanager.h"
#include "appmodes.h"
#include "canvas.h"
#include "commonstrings.h"
#include "hyphenator.h"
#include "marks.h"
#include "notesstyles.h"
#include "numeration.h"
#include "pageitem.h"
#include "pageitem_group.h"
#include "pageitem_noteframe.h"
#include "prefsmanager.h"
#include "scconfig.h"
#include "scpage.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribusstructs.h"
#include "selection.h"
#include "text/boxes.h"
#include "text/screenpainter.h"
#include "text/textshaper.h"
#include "text/shapedtext.h"
#include "text/shapedtextfeed.h"
#include "ui/guidemanager.h"
#include "ui/marksmanager.h"
#include "undomanager.h"
#include "undostate.h"
#include "units.h"
#include "util.h"
#include "util_math.h"



using namespace std;

PageItem_TextFrame::PageItem_TextFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::TextFrame, x, y, w, h, w2, fill, outline)
{
	init();
	firstChar = 0;
}

PageItem_TextFrame::PageItem_TextFrame(const PageItem & p) : PageItem(p)
{
	init();
	m_notesFramesMap.clear();
}

void PageItem_TextFrame::init()
{
	invalid = true;
	cursorBiasBackward = false;
	unicodeTextEditMode = false;
	unicodeInputCount = 0;
	m_origAnnotPos = QRectF(xPos(), yPos(), width(), height());
	verticalAlign = 0;
	incompleteLines = 0;
	maxY = 0.0;
	connect(&itemText,SIGNAL(changed(int, int)), this, SLOT(slotInvalidateLayout(int, int)));
}

static QRegion itemShape(PageItem* docItem, double xOffset, double yOffset)
{
	QRegion res;
	QTransform pp;
	if (docItem->isGroupChild())
		pp.translate(docItem->gXpos, docItem->gYpos);
	else
		pp.translate(docItem->xPos() - xOffset, docItem->yPos() - yOffset);
	pp.rotate(docItem->rotation());
	if (docItem->textFlowUsesBoundingBox())
	{
		QRectF bb = docItem->getVisualBoundingRect();
		if (docItem->isGroupChild())
		{
			bb.translate(-docItem->xPos(), -docItem->yPos());
			bb.translate(docItem->gXpos, docItem->gYpos);
		}
		res = QRegion(bb.toRect());
	}
	else if ((docItem->textFlowUsesImageClipping()) && (docItem->imageClip.size() != 0))
	{
		QList<uint> Segs;
		QPolygon Clip2 = FlattenPath(docItem->imageClip, Segs);
		res = QRegion(pp.map(Clip2)).intersected(QRegion(pp.map(docItem->Clip)));
	}
	else if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
	{
		QList<uint> Segs;
		QPolygon Clip2 = FlattenPath(docItem->ContourLine, Segs);
		res = QRegion(pp.map(Clip2));
	}
	else
	{
		if (docItem->isSymbol() || docItem->isGroup())
		{
			if (docItem->imageFlippedH())
			{
				pp.translate(docItem->width(), 0);
				pp.scale(-1, 1);
			}
			if (docItem->imageFlippedV())
			{
				pp.translate(0, docItem->height());
				pp.scale(1, -1);
			}
		}
		if ((((docItem->lineColor() != CommonStrings::None) || (!docItem->patternStrokeVal.isEmpty()) || (docItem->GrTypeStroke > 0)) && (docItem->lineWidth() > 1)) || (!docItem->NamedLStyle.isEmpty()))
		{
//			QVector<double> m_array;
			QPainterPath ppa;
			QPainterPath result;
			if (docItem->itemType() == PageItem::PolyLine)
				ppa = docItem->PoLine.toQPainterPath(false);
			else
				ppa = docItem->PoLine.toQPainterPath(true);
			if (docItem->NamedLStyle.isEmpty())
			{
				QPainterPathStroker stroke;
				stroke.setCapStyle(docItem->lineEnd());
				stroke.setJoinStyle(docItem->lineJoin());
				stroke.setDashPattern(Qt::SolidLine);
				stroke.setWidth(docItem->lineWidth());
				result = stroke.createStroke(ppa);
			}
			else
			{
				multiLine ml = docItem->doc()->MLineStyles[docItem->NamedLStyle];
				int ind = ml.size()-1;
				if ((ml[ind].Color != CommonStrings::None) && (ml[ind].Width != 0))
				{
					QPainterPathStroker stroke;
					stroke.setCapStyle(static_cast<Qt::PenCapStyle>(ml[ind].LineEnd));
					stroke.setJoinStyle(static_cast<Qt::PenJoinStyle>(ml[ind].LineJoin));
					stroke.setDashPattern(Qt::SolidLine);
					stroke.setWidth(ml[ind].Width);
					result = stroke.createStroke(ppa);
				}
			}
			res = QRegion(pp.map(docItem->Clip));
			QList<QPolygonF> pl = result.toSubpathPolygons();
			for (int b = 0; b < pl.count(); b++)
			{
				res = res.united(QRegion(pp.map(pl[b].toPolygon())));
			}
		}
		else
			res = QRegion(pp.map(docItem->Clip));
	}
	return  res;
}

QRegion PageItem_TextFrame::calcAvailableRegion()
{
	QRegion result(this->Clip);
	if ((!isEmbedded) || (isGroupChild()))
	{
		bool invertible(false);
		QTransform canvasToLocalMat;
		if (isGroupChild())
			canvasToLocalMat.translate(gXpos, gYpos);
		else
			canvasToLocalMat.translate(m_xPos, m_yPos);
		canvasToLocalMat.rotate(m_rotation);
		canvasToLocalMat = canvasToLocalMat.inverted(&invertible);

		if (!invertible) return QRegion();

		int LayerLev = m_Doc->layerLevelFromID(LayerID);
		int docItemsCount = m_Doc->Items->count();
		ScPage* Mp=0;
		ScPage* Dp=0;
		PageItem* docItem=0;
		int LayerLevItem;
		QList<PageItem*> thisList;
		if (!OnMasterPage.isEmpty())
		{
			if ((savedOwnPage == -1) || (savedOwnPage >= signed(m_Doc->Pages->count())))
				return result;
			Mp = m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]);
			Dp = m_Doc->Pages->at(savedOwnPage);
			if (isGroupChild())
				thisList = Parent->asGroupFrame()->groupItemList;
			else
				thisList = m_Doc->MasterItems;
			int thisid = thisList.indexOf(this);
			for (int a = 0; a < m_Doc->MasterItems.count(); ++a)
			{
				docItem = m_Doc->MasterItems.at(a);
				// #10642 : masterpage items interact only with items placed on same masterpage
				if (docItem->OnMasterPage != OnMasterPage)
					continue;
				LayerLevItem = m_Doc->layerLevelFromID(docItem->LayerID);
				if (((a > thisid) && (docItem->LayerID == LayerID)) || (LayerLevItem > LayerLev && m_Doc->layerFlow(docItem->LayerID)))
				{
					if (docItem->textFlowAroundObject())
					{
						QRegion itemRgn = itemShape(docItem, Mp->xOffset() - Dp->xOffset(), Mp->yOffset() - Dp->yOffset());
						result = result.subtracted( canvasToLocalMat.map(itemRgn) );
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
			int thisid = 0;
			if (isGroupChild())
			{
				thisid = Parent->asGroupFrame()->groupItemList.indexOf(this);
				docItemsCount = Parent->asGroupFrame()->groupItemList.count();
				for (int a = thisid + 1; a < docItemsCount; ++a)
				{
					docItem = Parent->asGroupFrame()->groupItemList.at(a);
					if (docItem->textFlowAroundObject())
					{
						QRegion itemRgn = itemShape(docItem, 0, 0);
						result = result.subtracted( canvasToLocalMat.map(itemRgn) );
					}
				}
			}
			else
			{
				thisid = m_Doc->Items->indexOf(this);
				for (int a = 0; a < docItemsCount; ++a)
				{
					docItem = m_Doc->Items->at(a);
					LayerLevItem = m_Doc->layerLevelFromID(docItem->LayerID);
					if (((a > thisid) && (docItem->LayerID == LayerID)) || (LayerLevItem > LayerLev && m_Doc->layerFlow(docItem->LayerID)))
					{
						if (docItem->textFlowAroundObject())
						{
							QRegion itemRgn = itemShape(docItem, 0, 0);
							result = result.subtracted( canvasToLocalMat.map(itemRgn) );
						}
					}
				}
			} // for all docItems
		} // if(OnMasterPage.isEmpty()
	} // if(!Embedded)
	//else
	//	qDebug() << "QRegion empty";
	return result;
}

void PageItem_TextFrame::setShadow()
{
	if (OnMasterPage.isEmpty())
		return;

	QString newShadow = m_Doc->masterPageMode() ? OnMasterPage : QString::number(OwnPage);
	if (newShadow != m_currentShadow) {
		if (m_currentShadow == OnMasterPage) {
			// masterpage was edited, clear all shadows
			m_shadows.clear();
		}
		if (!m_shadows.contains(newShadow)) {
			if (!m_shadows.contains(OnMasterPage)) {
				m_shadows[OnMasterPage] = itemText;
//				const ParagraphStyle& pstyle(shadows[OnMasterPage].paragraphStyle(0));
//				qDebug() << QString("Pageitem_Textframe: style of master: %1 align=%2").arg(pstyle.parent()).arg(pstyle.alignment());
//				qDebug() << QString("Pageitem_Textframe: shadow itemText->%1").arg(OnMasterPage);
			}
			if (newShadow != OnMasterPage) {
				m_shadows[newShadow] = m_shadows[OnMasterPage].copy();
//				const ParagraphStyle& pstyle(shadows[newShadow].paragraphStyle(0));
//				qDebug() << QString("Pageitem_Textframe: style of shadow copy: %1 align=%2").arg(pstyle.parent()).arg(pstyle.alignment());
			}
//			qDebug() << QString("Pageitem_Textframe: shadow %1<-%2").arg(newShadow).arg(OnMasterPage);
		}
		itemText = m_shadows[newShadow];
//		const ParagraphStyle& pstyle(itemText.paragraphStyle(0));
//		qDebug() << QString("Pageitem_Textframe: style of shadow: %1 align=%2").arg(pstyle.parent()).arg(pstyle.alignment());
		invalid = true;
		m_currentShadow = newShadow;
	}
}
/*
static void debugLineLayout(const StoryText& itemText, const LineSpec& line)
{
	QFile debugFile(QDir::homePath() + "/Desktop/debug_line.csv");
	debugFile.open(QIODevice::WriteOnly);

	QTextStream stream(&debugFile);
	stream.setRealNumberNotation(QTextStream::FixedNotation);
	stream.setRealNumberPrecision(7);

	stream << "xoffset"  << "\t";
	stream << "yoffset"  << "\t";
	stream << "xadvance" << "\t";
	stream << "yadvance" << "\t";
	stream << "scaleH"   << "\t";
	stream << "scaleV"   << "\t";
	stream << "\n";

	for (int zc = line.firstChar; zc < line.lastChar; ++zc)
	{
		const ScText* item = itemText.item(zc);

		stream << item->glyph.xoffset  << "\t";
		stream << item->glyph.yoffset  << "\t";
		stream << item->glyph.xadvance << "\t";
		stream << item->glyph.yadvance << "\t";
		stream << item->glyph.scaleH   << "\t";
		stream << item->glyph.scaleV   << "\t";
		stream << "\n";
	}

	debugFile.close();
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
*/
static const bool legacy = true;

/*
static void layoutDropCap(GlyphLayout layout, double curX, double curY, double offsetX, double offsetY, double dropCapDrop)
{
}
*/


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
	bool         active;
	int          status;
	double       xPos;
	QChar        fillChar;
	GlyphLayout* tabGlyph;
};

class LineSpec
{
	public:
	LineSpec()
	{
		x = 0.0;
		y = 0.0;
		width  = 0.0;
		height = 0.0;
		ascent = 0.0;
		descent = 0.0;
		colLeft = 0.0;
		firstCluster = 0;
		lastCluster = 0;
		naturalWidth = 0.0;
		isFirstLine = false;
	}

	qreal x;
	qreal y;
	qreal width;
	qreal height;
	qreal ascent;
	qreal descent;
	qreal colLeft;

	int firstCluster;
	int lastCluster;
	qreal naturalWidth;
	bool isFirstLine;
};

/**
fields which describe how the current line is placed into the frame
*/
struct LineControl {
	LineSpec lineData;
	QList<GlyphCluster> glyphs;
	bool     isEmpty;
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
	int      restartIndex;  //index of glyph run where line computing should be restarted
	int      restartRowIndex;  //index of glyph run where row of text is started
	double   restartX; //starting X position of line if must be restarted
	double   rowDesc;

	double   xPos;
	double   yPos;
	int      breakIndex;
	double   breakXPos;

	double   maxShrink;
	double   maxStretch;
	ScribusDoc *doc;
	ITextContext* context;

	/// remember frame dimensions and offsets
	LineControl(double w, double h, const MarginStruct& extra, double lCorr, ScribusDoc* d, ITextContext* ctx, double colwidth, double colgap)
		: hasDropCap(false)
		, doc(d), context(ctx)
	{
		insets = extra;
		lineCorr = lCorr;
		frameWidth = w;
		frameHeight = h;
		column = 0;
		colWidth = colwidth;
		colGap = colgap;
		hyphenCount = 0;
		isEmpty = true;
		colLeft = insets.left() + lineCorr;
		colRight = colLeft + colWidth;
		startOfCol = true;
		afterOverflow = false;
		addLine = false;
		recalculateY = false;
		lastInRowLine = false;
		addLeftIndent = false;
		wasFirstInRow = false;
		leftIndent = 0.0;
		rightMargin = 0.0;
		mustLineEnd = false;
		restartIndex = 0;
		restartRowIndex = 0;
		restartX = 0.0;
		rowDesc = 0.0;
		xPos = 0.0;
		yPos = 0.0;
		breakIndex = -1;
		breakXPos = 0.0;
		maxShrink = 0.0;
		maxStretch = 0.0;
	}

	/// move position to next column
	void nextColumn(TextLayout &textLayout)
	{
		startOfCol = true;
		if (textLayout.story()->defaultStyle().direction() == ParagraphStyle::RTL)
			colLeft = textLayout.frame()->width() - insets.right() - ((colWidth * (column + 1)) + (colGap * column));
		else
			colLeft = (colWidth + colGap) * column + insets.left() + lineCorr;
		textLayout.addColumn(colLeft, colWidth);
		//now colRight is REAL column right edge
		colRight = colLeft + colWidth;
		if (legacy)
			colRight += lineCorr;
		xPos = colLeft;
		yPos = insets.top() + lineCorr;
		lineData.colLeft = colLeft;
	}

	bool isEndOfCol(double morespace = 0)
	{
		return yPos + morespace + insets.bottom() + lineCorr > frameHeight;
	}

	/**
		init fields for a new line at current position
	 */
	void startLine(int firstCluster)
	{
		glyphs.clear();
		isEmpty = true;
		lineData.x = xPos;
		lineData.y = yPos;
		lineData.firstCluster = firstCluster;
		lineData.lastCluster = 0;
		lineData.ascent = 0.0;
		lineData.descent = 0.0;
		lineData.width  = 0.0;
		lineData.height = 0.0;
		lineData.naturalWidth = 0.0;
		lineData.colLeft = colLeft;
		breakIndex = -1;
		breakXPos = 0.0;
		maxShrink = 0.0;
		maxStretch = 0.0;
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
	void breakLine(int last)
	{
		breakIndex = last;
		breakXPos  = lineData.x;

		for (int i = 0; i <= breakIndex - lineData.firstCluster; i++)
			breakXPos += glyphs.at(i).width();
		// #8194, #8717 : update line ascent and descent with sensible values
		// so that endOfLine() returns correct result
		updateHeightMetrics();
		// #9060 : update line offset too
	//	updateLineOffset(itemText, style, offsetPolicy);
	}

	/// use the last remembered break to set line width and itemrange
	void finishLine(double endX)
	{
		lineData.lastCluster = breakIndex;
		lineData.naturalWidth = breakXPos - lineData.x;
		lineData.width = endX - lineData.x;
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
		return restartRowIndex - 1;
	}

	int restartLine(bool recalcY, bool add)
	{
		recalculateY = recalcY;
		addLine = add;
		xPos = restartX;
		startLine(restartIndex);
		afterOverflow = false;
		return restartIndex - 1;
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
	/*double endOfLine_old(const QRegion& shape, const QTransform& pf2, double morespace, int Yasc, int Ydesc)
	{
		// if we aren't restricted further, we'll end at this maxX:
		double maxX = colRight - morespace;
		if (legacy) maxX -= lineCorr;

		double StartX = floor(qMax(line.x, qMin(maxX,breakXPos-maxShrink-1))-1);
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

		double StartX = floor(qMax(lineData.x, qMin(maxX, breakXPos-maxShrink-1))-1);
		StartX = qMax(0.0, StartX);

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
		if (lineI.rectCount() == 1)
		{
			int   cPos = static_cast<int>(ceil(StartX + morespace));
			QRect cRect (QPoint(cPos, yAsc), QPoint(cPos, yDesc));
			QRegion qr2 = QRegion(cRect).subtracted(shape);
			if (qr2.isEmpty()) // qr2 == 0 <=> cRect subset of shape
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

	void updateHeightMetrics()
	{
		lineData.ascent = lineData.descent = 0;
		if (glyphs.isEmpty())
			return;
		const CharStyle& cStyle(glyphs.at(0).style());
		double scaleV = cStyle.scaleV() / 1000.0;
		double offset = (cStyle.fontSize() / 10) * (cStyle.baselineOffset() / 1000.0);
		lineData.ascent = cStyle.font().ascent(cStyle.fontSize()/10.00) * scaleV + offset;
		lineData.descent = cStyle.font().descent(cStyle.fontSize()/10.00) * scaleV - offset;
	}

// yPos should not be changed when all line is already calculated - at new y position there can be overflow!!!
// edit: can't happen as it should only move upwards, and this is covered by the calculations done.
//void updateLineOffset(const StoryText& itemText, const ParagraphStyle& style, FirstLineOffsetPolicy offsetPolicy)
//{
//	if (charsInLine <= 0)
//		return;
//	if ((!hasDropCap) && (startOfCol) && (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing))
//	{
//		//FIXME: use glyphs, not chars
//		double firstasce = itemText.charStyle(line.firstChar).font().ascent(itemText.charStyle(line.firstChar).fontSize() / 10.0);
//		double adj (0.0);
//		double currasce (this->getLineAscent(itemText));
//		if (offsetPolicy == FLOPRealGlyphHeight)
//		{
//			adj = firstasce - currasce;
//		}
//		else if (offsetPolicy == FLOPFontAscent)
//		{
//			adj = 0.0;
//		}
//		else if (offsetPolicy == FLOPLineSpacing)
//		{
//			adj = firstasce - style.lineSpacing();
//		}
//		line.ascent = currasce;
//		line.y -= adj;
//		yPos -= adj;
//	}
//	else if ((!startOfCol) && (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
//	{
//		QChar ch = itemText.text(line.firstChar);
//		double firstasce = style.lineSpacing();
//		double currasce  = getLineHeight(itemText);
//		double adj = firstasce - currasce;
//		qDebug() << QString("move2 line %1.. down by %2").arg(current.line.firstChar).arg(-adj);
//		line.ascent = currasce;
//		line.y -= adj;
//		yPos -= adj;
//	}
//}

	/// called when line length is known and line is to be justified
	void justifyLine(const ParagraphStyle& style)
	{

		double glyphNatural = 0;
		double spaceNatural = 0;
		double glyphExtension;
		double spaceExtension;
		int spaceInsertion = 0;
		double imSpace = -1;
		int trackingInsertion = 0;
		double trackingAmount = 0;

		int glyphsCount = lineData.lastCluster - lineData.firstCluster + 1;

		for (int i = 0; i < glyphsCount; ++i)
		{
			const GlyphCluster& glyphCluster = glyphs.at(i);
			if (!glyphCluster.hasFlag(ScLayout_ExpandingSpace))
			{
				glyphNatural += glyphCluster.width();
			}
			else if (!glyphCluster.hasFlag(ScLayout_SuppressSpace))
			{
				spaceNatural += glyphCluster.width();
				if (imSpace < 0.0 || imSpace > glyphCluster.width())
					imSpace = glyphCluster.width();
			}
			if (i != 0 && glyphCluster.hasFlag(ScLayout_ImplicitSpace))
			{
				spaceInsertion += 1;
			}
			if (i != glyphsCount && glyphCluster.hasFlag(ScLayout_JustificationTracking))
			{
				trackingInsertion += 1;
			}
		}

		imSpace /= 2;

		// decision: prio 1: stretch glyph;  prio 2: insert spaces;  prio 3: stretch spaces

		if (lineData.width < spaceNatural + glyphNatural * style.minGlyphExtension() && spaceNatural > 0)
		{
			glyphExtension = style.minGlyphExtension() - 1;
			spaceExtension = (lineData.width - glyphNatural * (1+glyphExtension) ) / spaceNatural  - 1;
			imSpace = 0;
		}
		else if (lineData.width < spaceNatural + glyphNatural * style.maxGlyphExtension() && glyphNatural > 0)
		{
			spaceExtension = 0;
			glyphExtension = (lineData.width - spaceNatural) / glyphNatural  - 1;
			imSpace = 0;
		}
		else
		{
			glyphExtension = style.maxGlyphExtension() - 1;
			if (spaceInsertion) {
				double remaining = lineData.width - glyphNatural * (1 + glyphExtension) - spaceNatural;
				if (imSpace > 0) {
					if (remaining / spaceInsertion < imSpace) {
						imSpace = remaining / spaceInsertion;
						spaceExtension = 0;
					} else {
						spaceExtension = (remaining + spaceNatural) / (spaceNatural + spaceInsertion * imSpace) - 1;
						imSpace *= spaceExtension + 1;
					}
				} else {
					imSpace = remaining / spaceInsertion;
					spaceExtension = 0;
				}
			} else {
				if (spaceNatural > 0)
					spaceExtension = (lineData.width - glyphNatural * (1 + glyphExtension)) / spaceNatural - 1;
				else
					spaceExtension = 0;

				if (trackingInsertion && (spaceExtension == 0.0 || spaceExtension >= 20.0))
				{
					if (spaceExtension == 0.0)
					{
						double remaining = lineData.width - glyphNatural * (1 + glyphExtension) - spaceNatural;
						trackingAmount = remaining / trackingInsertion;
					}
					else
					{
						spaceExtension = qMin(20.0, spaceExtension);
						double remaining = lineData.width - glyphNatural * (1 + glyphExtension) - spaceNatural;
						remaining -= spaceExtension * spaceNatural;
						trackingAmount = remaining / trackingInsertion;
						if (trackingAmount > imSpace)
						{
							trackingAmount = imSpace;
							spaceExtension = (lineData.width - glyphNatural * (1 + glyphExtension) - trackingInsertion * trackingAmount) / spaceNatural - 1;
						}
					}
				}
			}
		}

		double glyphScale = 1 + glyphExtension;
		double naturalWidth = 0;

	/*
		qDebug() << QString("justify: line = %7 natural = %1 + %2 = %3 (%4); spaces + %5%%; min=%8; glyphs + %6%%; min=%9")
			   .arg(spaceNatural).arg(glyphNatural).arg(spaceNatural+glyphNatural).arg(line.naturalWidth)
			   .arg(spaceExtension).arg(glyphExtension).arg(line.width)
			   .arg(style.minWordTracking()).arg(style.minGlyphExtension());
		*/

		int startItem = 0;
		if (glyphs[startItem].hasFlag(ScLayout_DropCap))
		{
			startItem++;
			naturalWidth += glyphs[startItem].width();
		}
		// distribute whitespace on spaces and glyphs
		for (int i = startItem; i < glyphsCount; ++i)
		{
			GlyphCluster& glyphCluster = glyphs[i];
			double wide = glyphCluster.width();
			if (!glyphCluster.hasFlag(ScLayout_ExpandingSpace))
			{
				glyphCluster.setScaleH(glyphCluster.scaleH() * glyphScale);
				glyphCluster.xoffset *= glyphScale;
			}
			else if (!glyphCluster.hasFlag(ScLayout_SuppressSpace))
			{
				glyphCluster.extraWidth += (wide * spaceExtension);
			}
			if (i != 0 && glyphCluster.hasFlag(ScLayout_ImplicitSpace))
			{
				GlyphCluster& lastRun = glyphs[i - 1];
				lastRun.extraWidth += imSpace;
			}
			if (i != glyphsCount && trackingAmount != 0 && glyphCluster.hasFlag(ScLayout_JustificationTracking))
			{
				glyphCluster.extraWidth += trackingAmount;
			}
			naturalWidth += glyphCluster.width();
		}
		lineData.naturalWidth = naturalWidth;

		if ((style.alignment() == ParagraphStyle::Extended) &&
			(style.direction() == ParagraphStyle::RTL))
		{
			double offset = lineData.width - lineData.naturalWidth;
			if (offset > 1e-6)
				indentLine(style, offset);
		}
	}

	/// called when linelength is known and line is not justified
	void indentLine(const ParagraphStyle& style, double leftIndent)
	{
		if (lineData.naturalWidth > lineData.width)
		{
			justifyLine(style);
		}
		if (leftIndent > 0)
		{
			lineData.x += leftIndent;
			lineData.width -= leftIndent;
		}
	}

	/**
	 Clones the tab fill char as often as necssary after all distances are known
	 */
	void fillInTabLeaders()
	{
		// fill in tab leaders
		for (int i = 0; i < glyphs.count(); ++i)
		{
			GlyphCluster& glyphCluster = glyphs[i];
			if (glyphCluster.hasFlag(ScLayout_TabLeaders))
			{
				const CharStyle& charStyle(glyphCluster.style());
				GlyphLayout tglyph = glyphCluster.glyphs().last();
				double width = glyphCluster.width();
				double wt = charStyle.font().glyphWidth(tglyph.glyph, charStyle.fontSize() * tglyph.scaleV / 10.0);
				int count = static_cast<int>(width / wt);
				if (count > 0)
					glyphCluster.glyphs().clear();
				for(int cx = 0; cx < count; ++cx)
				{
					GlyphLayout more = tglyph;
					more.xadvance = 0;
					if (cx != 0)
						more.xoffset = (width / count) * cx;
					glyphCluster.append(more);
				}
				glyphCluster.glyphs().last().xadvance = width / glyphCluster.scaleH();
			}
		}
	}


	//defined but not used
	///// calculate how much the first char should stick out to the left
	//double opticalLeftMargin(const StoryText& itemText)
	//{
	//	int b = line.firstChar;
	//	while (b < line.lastChar && (itemText.flags(b) & ScLayout_SuppressSpace))
	//		   ++b;
	//
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
	////				double leftCorr2 = itemText.charStyle(a).font().charWidth(QChar('K'), chs / 10.0, chr);
	////				leftCorr2 -= itemText.charStyle(a).font().charWidth(QChar('K'), chs / 10.0);
	////				leftCorr = qMin(leftCorr, leftCorr2);
	//	}
	//	return leftCorr;
	//}

	/// calculate how much the last char should stick out to the right
	double opticalRightMargin(const StoryText& itemText)
	{
		int b = lineData.lastCluster - lineData.firstCluster;
		while (b > 0 &&
			   (SpecialChars::isBreakingSpace(itemText.text(glyphs[b].lastChar())) ||
				SpecialChars::isBreak(itemText.text(glyphs[b].lastChar())))
			   )
			--b;
		if (b >= 0)
		{
			const CharStyle& style = glyphs[b].style();
			const ScFace& font = style.font();
			double chs = style.fontSize() * (style.scaleH() / 1000.0);
			QChar chr = itemText.text(glyphs[b].lastChar());
			double rightCorr;
			if (glyphs[b].hasFlag(ScLayout_SoftHyphenVisible))
				rightCorr = font.hyphenWidth(style, chs / 10.0);
			else
				rightCorr = font.glyphBBox(font.char2CMap(chr.unicode()), chs / 10.0).width;
			if (glyphs[b].hasFlag(ScLayout_SoftHyphenVisible)
				|| QString("-,.`´'~").indexOf(chr) >= 0
				|| chr == QChar(0x2010)
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
#if 0
				// FIXME HOST: is the kerning with "." a realy reliable way to check this?
				rightCorr = chStyle.font().realCharWidth(chr, chs / 10.0);
				rightCorr -= chStyle.font().charWidth(chr, chs / 10.0, QChar('.'));
#else
				rightCorr = 0;
#endif
			}
			return rightCorr;
		}
		return 0.0;
	}

	LineBox* createLineBox()
	{
		LineBox* result = new LineBox();
		result->moveTo(lineData.x - colLeft, lineData.y - lineData.ascent);
		result->setWidth(lineData.width);
		result->setAscent(lineData.ascent);
		result->setDescent(lineData.descent);
		foreach (const GlyphCluster& run, ShapedTextFeed::putInVisualOrder(glyphs,   0, lineData.lastCluster - lineData.firstCluster + 1))
		{
			addBox(result, run);
//			qDebug() << "cluster" << run.firstChar() << ".." << run.lastChar() << "@" << run.visualIndex();
		}
		return result;
	}

	void addBox(LineBox *lineBox, const GlyphCluster& run)
	{
		Box* result;
		if (run.object().getPageItem(doc))
		{
			result = new ObjectBox(run, context);
			QRectF bBox = context->getVisualBoundingBox(run.object());
			if (run.hasFlag(ScLayout_DropCap))
				result->setAscent(bBox.height() * run.scaleV() - run.yoffset);
			else
				result->setAscent(bBox.height());
			result->setDescent(0);
		}
		else
		{
			result = new GlyphBox(run);
			result->setAscent(lineBox->ascent());
			result->setDescent(lineBox->descent());
		}
		lineBox->addBox(result);
	}

private:
	double frameWidth;
	double frameHeight;
	MarginStruct insets;
	double lineCorr;
};

static bool allowedCJKBreakAfter(QChar ch) {
	unsigned int code[] = {0x201C, 0x300C, 0xFF08, 0xFF3B, 0xFF5B, 0xFF5F, 0xFF62, 0xFF0D, 0};
	for (int i = 0; code[i]; ++i)
		if (code[i] == ch.unicode())
			return false;
	return true;
}

static int allowedCJKBreakBefore(QChar ch) {
	unsigned int code[] =
	 {0x201D, 0x3001, 0x3002, 0x300D, 0xFF01, 0xFF09, 0xFF0C, 0xFF0E, 0xFF1A,
	  0xFF1B, 0xFF1F, 0xFF3D, 0xFF5D, 0xFF60, 0xFF63, 0xFF64, 0};
	for (int i = 0; code[i]; ++i)
		if (code[i] == ch.unicode())
			return false;
	return true;
}

static bool implicitBreak(QChar f, QChar s) {
	if (SpecialChars::isCJK(f.unicode()) && SpecialChars::isCJK(s.unicode())) {
		return allowedCJKBreakAfter(f) && allowedCJKBreakBefore(s);
	} else
		return false;
}

static double findRealOverflowEnd(const QRegion& shape, QRect pt, double maxX)
{
	while (!regionContainsRect(shape, pt) && pt.right() < maxX)
		pt.translate(1, 0);
	if (pt.right() >= maxX)
		return maxX;
	return pt.left() + 0.5;
}

static double adjustToBaselineGrid (const LineControl &control, PageItem *item, int OwnPage)
{
	double by = item->yPos();
	if (OwnPage != -1)
		by = by - item->doc()->Pages->at(OwnPage)->yOffset();
	int ol1 = qRound((by + control.yPos - item->doc()->guidesPrefs().offsetBaselineGrid) * 10000.0);
	int ol2 = static_cast<int>(ol1 / item->doc()->guidesPrefs().valueBaselineGrid);
//						qDebug() << QString("baseline adjust: y=%1->%2").arg(current.yPos).arg(ceil(  ol2 / 10000.0 ) * item->doc()->typographicSettings.valueBaselineGrid + item->doc()->typographicSettings.offsetBaselineGrid - by);

	return ceil(  ol2 / 10000.0 ) * item->doc()->guidesPrefs().valueBaselineGrid + item->doc()->guidesPrefs().offsetBaselineGrid - by;
}

static double nextAutoTab (const LineControl &current, PageItem *item)
{
	double dtw = item->doc()->itemToolPrefs().textTabWidth;
	if (current.xPos == current.colLeft)
		return current.colLeft + dtw;
	double res = current.colLeft + ceil ((current.xPos - current.colLeft) / dtw) * dtw;
	if (res == current.xPos) res += dtw;
	return res;
}

//cezaryece: I remove static statement as this function is used also by PageItem_NoteFrame
double calculateLineSpacing (const ParagraphStyle &style, PageItem *item)
{
	if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
	{
		double autoLS = static_cast<double>(item->doc()->typographicPrefs().autoLineSpacing) / 100.0;
		return (style.charStyle().font().height(style.charStyle().fontSize() / 10.0) * autoLS);
	}
	if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
		return item->doc()->guidesPrefs().valueBaselineGrid;
	return style.lineSpacing();
}


// This assumes that layout() ran on the previous page and set the incomplete* vars
// It also clears the incomplete* vars, and changes the starting position for this frame
// The incomplete* vars are used to ensure that we don't run into an endless loop
// This setup won't cause any problems, as the only way for the starting position to change
// back is if the previous frame's layout() runs again, but if it does, it also sets the
// incomplete* vars for us
bool PageItem_TextFrame::moveLinesFromPreviousFrame ()
{
	PageItem_TextFrame* prev = dynamic_cast<PageItem_TextFrame*>(BackBox);
	if (!prev)
		return false;
	if (!prev->incompleteLines)
		return false;   // no incomplete lines - nothing to do
	int pos = textLayout.endOfFrame() - 1;
	QChar lastChar = itemText.text (pos);
	// qDebug()<<"pos is"<<pos<<", length is"<<itemText.length()<<", incomplete is "<<prev->incompleteLines;
	if ((pos != itemText.length()-1) && (!SpecialChars::isBreak (lastChar, true)))
		return false;  // the paragraph isn't ending yet
	int lines = textLayout.lines();  // lines added to the current frame

	ParagraphStyle style = itemText.paragraphStyle (pos);
	int need = style.keepLinesEnd () + 1;
	int prevneed = style.keepLinesStart () + 1;
	if (lines >= need)
	{
		prev->incompleteLines = 0;   // so that further paragraphs don't pull anything
		return false;   // we have enough lines
	}

	// too few lines - we need to pull some from the previous page
	int pull = need - lines;
	// if pulling the lines would lead to the original frame having too few, pull the whole paragraph
	if (prev->incompleteLines - pull < prevneed)
		pull = prev->incompleteLines;
	// qDebug()<<"pulling"<<pull<<"lines;
	// Okay, move the starting/ending character
	int startingPos = prev->incompletePositions[prev->incompleteLines - pull];
	for (int i = 0; i < pull; ++i)
		prev->textLayout.removeLastLine();
	firstChar = prev->MaxChars = startingPos;
	// keep the remaining incomplete lines flagged as such
	// this ensures that if pulling one line won't be enough, the subsequent call to layout() will pull more
	prev->incompleteLines -= pull;

	return true;
}

// called at the end of a frame or column
void PageItem_TextFrame::adjustParagraphEndings ()
{
	// More text to go - let's apply paragraph flowing options - orphans/widows, etc
	int pos = textLayout.endOfFrame() - 1;
	if (pos >= itemText.length() - 1)
		return;

	ParagraphStyle style = itemText.paragraphStyle (pos);
	int paragraphStart = itemText.prevParagraph (pos) + 1;
	QChar lastChar = itemText.text (pos);
	bool keepWithNext = style.keepWithNext() && (lastChar == SpecialChars::PARSEP);
	if (keepWithNext || (!SpecialChars::isBreak (lastChar, true)))
	{
		// paragraph continues in the next frame, or needs to be kept with the next one
		// check how many lines are in this frame
		int lineStart = textLayout.startOfLine (pos);
		incompleteLines = 1;
		incompletePositions.prepend (lineStart);
		while (lineStart > paragraphStart)
		{
			lineStart = textLayout.startOfLine (lineStart - 1);
			incompleteLines++;
			incompletePositions.prepend (lineStart);
		}
		int need = style.keepLinesStart () + 1;
		if (style.keepTogether())
			need = incompleteLines;
		int pull = 0;
		if (style.keepTogether() || (incompleteLines < need))
			pull = incompleteLines;
		// if we need to keep it with the next one, pull one line. Next frame layouting
		// will pull more from us if it proves necessary.
		if (keepWithNext && (!pull))
			pull = 1;

		if (pull)
		{
			qDebug() << "pulling" << pull << "lines";
			// push this paragraph to the next frame
			for (int i = 0; i < pull; ++i)
				textLayout.removeLastLine();
			MaxChars = incompletePositions[incompleteLines-pull];
			incompleteLines = 0;
			incompletePositions.clear();
		}
	}
}

void PageItem_TextFrame::layout()
{
//	qDebug()<<"==Layout==" << itemName() ;
// 	printBacktrace(24);
	if (BackBox != NULL) {
//		qDebug("textframe: len=%d, going back", itemText.length());
		PageItem_TextFrame* firstInvalid = NULL;
		PageItem_TextFrame* prevInChain  = dynamic_cast<PageItem_TextFrame*>(BackBox);
		while (prevInChain)
		{
			if (prevInChain->invalid)
				firstInvalid = prevInChain;
			prevInChain = dynamic_cast<PageItem_TextFrame*>(prevInChain->BackBox);
		}
		PageItem_TextFrame* nextInChain = firstInvalid;
		while (nextInChain && (nextInChain != this))
		{
			nextInChain->layout();
			nextInChain = dynamic_cast<PageItem_TextFrame*>(nextInChain->NextBox);
		}
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

//	qDebug() << QString("textframe(%1,%2): len=%3, start relayout at %4").arg(m_xPos).arg(m_yPos).arg(itemText.length()).arg(firstInFrame());
	QPoint pt1, pt2;
	QRect pt;
	double chs, chsd = 0;
	double EndX, OFs;
	ParagraphStyle style;
	int opticalMargins = ParagraphStyle::OM_None;

	bool outs = false;
	bool goNoRoom = false;
	bool goNextColumn = false;

	TabControl tabs;
	tabs.active    = false;     // RTab
	tabs.status    = TabNONE;   // TabCode
	tabs.tabGlyph  = 0;         // was int charIndex ~ StartRT
	tabs.xPos      = 0;         // RTabX

	QList<ParagraphStyle::TabRecord> tTabValues;
	tTabValues.clear();

	bool BulNumMode = false; //when bullet or counter should be inserted
	bool   DropCmode = false, FlopBaseline = false;
	double desc=0, asce=0, realAsce=0, realDesc = 0, offset = 0;
	double maxDY=0, maxDX=0;
	double DropCapDrop = 0;
	int    DropLines = 0;
	int    DropLinesCount = 0;

	textLayout.clear();
	incompleteLines = 0;
	incompletePositions.clear();

	double lineCorr = 0;
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth / 2.0;

	//hold Y position of last computed line of text (with glyphs descent)
	//for moving next line if glyphs are higher than that
	double lastLineY = 0;

	QMap<int, Mark*> noteMarksPosMap;  //maping notes marks and its position in text

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
	int itLen = itemText.length();
	//fast validate empty frames
	if (itLen == 0 || firstInFrame() == itLen)
	{
		PageItem_TextFrame * next = this;
		while (next != NULL)
		{
			next->invalid = false;
			next->firstChar = itLen;
			next->MaxChars = itLen;
			next->textLayout.clear();
			next = dynamic_cast<PageItem_TextFrame*>(next->nextInChain());
		}
		// TODO layout() shouldn't delete any frame here, as it breaks any loop
		// over the doc->Items or doc->MasterItems lists when done with indexes
		// see Bug #12685 for an example
		if (!isNoteFrame() && m_Doc->notesChanged() && !m_notesFramesMap.isEmpty())
		{ //if notes are used
			UndoManager::instance()->setUndoEnabled(false);
			QList<PageItem_NoteFrame*> delList;
			foreach (PageItem_NoteFrame* nF, m_notesFramesMap.keys())
			{
				if (nF->notesList().isEmpty() && !nF->isAutoNoteFrame())
					delList.append(nF);
			}
			while (!delList.isEmpty())
				m_Doc->delNoteFrame(delList.takeFirst(), false);
			UndoManager::instance()->setUndoEnabled(true);
		}
		return;
	}

	if ((itLen != 0)) // || (NextBox != 0))
	{
		// determine layout area
		m_availableRegion = calcAvailableRegion();
		if (m_availableRegion.isEmpty())
		{
			MaxChars = firstInFrame();
			goto NoRoom;
		}

		if (imageFlippedH() || imageFlippedV())
		{
			QTransform matrix;
			if (imageFlippedH())
			{
				matrix.translate(m_width, 0);
				matrix.scale(-1, 1);
			}
			if (imageFlippedV())
			{
				matrix.translate(0, m_height);
				matrix.scale(1, -1);
			}
			m_availableRegion = matrix.map(m_availableRegion);
		}

		ITextContext* context = this;
		//TextShaper textShaper(this, itemText, firstInFrame());
		ShapedTextFeed shapedText(&itemText, firstInFrame(), context);
		
		QList<GlyphCluster> glyphClusters; // = textShaper.shape();
		// std::sort(glyphClusters.begin(), glyphClusters.end(), logicalGlyphRunComp);

		LineControl current(m_width, m_height, m_textDistanceMargins, lineCorr, m_Doc, context,columnWidth(), ColGap);
		current.nextColumn(textLayout);

		lastLineY = m_textDistanceMargins.top();

		//automatic line spacing factor (calculated once)
		double autoLS = static_cast<double>(context->typographicPrefs().autoLineSpacing) / 100.0;

		// find start of first line
		if (firstInFrame() < itLen)
		{
			const CharStyle& cstyle = itemText.charStyle(firstInFrame());
			style = itemText.paragraphStyle(firstInFrame());
			style.setLineSpacing (calculateLineSpacing (style, this));

//			qDebug() << QString("style @0: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//				   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():"");
			if (style.hasDropCap())
			{
				chs = calculateLineSpacing (style, this) * style.dropCapLines() * 10;
			}
			else
				chs = cstyle.fontSize();
			desc = -cstyle.font().descent(chs / 10.0);
			current.yPos = m_textDistanceMargins.top() + lineCorr;
//			qDebug() << QString("first line at y=%1").arg(current.yPos);

		}
		else // empty itemText:
		{
			desc = -itemText.defaultStyle().charStyle().font().descent(itemText.defaultStyle().charStyle().fontSize() / 10.0);
			current.yPos = itemText.defaultStyle().lineSpacing() + m_textDistanceMargins.top() + lineCorr - desc;
		}
		// update Bullet & number list if any.
		updateBulletsNum();

		current.startLine(0);

		outs = false;
		OFs = 0;
		MaxChars = 0;
		double realEnd = 0;
		current.restartIndex = current.restartRowIndex = 0;
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

		//why emit invalidating signals each time text is changed by appling styles?
		//this speed up layouting in case of using notes marks and drop caps
		itemText.blockSignals(true);
		setMaxY(-1);
		double maxYAsc = 0.0, maxYDesc = 0.0;
		int regionMinY = 0, regionMaxY= 0;

		double autoLeftIndent = 0.0;
		for (int i = 0; shapedText.haveMoreText(i, glyphClusters); ++i)
		{
			int currentIndex = i - current.lineData.firstCluster;
			GlyphCluster newRun = glyphClusters[i];
			if (currentIndex >= current.glyphs.count())
				current.glyphs.append(newRun);
			else
				current.glyphs[currentIndex] = newRun;

			if (current.glyphs[currentIndex].glyphs().isEmpty())
				continue;

			int a = current.glyphs[currentIndex].firstChar();
			bool HasObject = itemText.hasObject(a);
			PageItem* currentObject = itemText.object(a).getPageItem(m_Doc);
			QRectF currentObjectBox = QRectF();
			if (HasObject)
				currentObjectBox = currentObject->getVisualBoundingRect();

			bool HasMark = itemText.hasMark(a);

			if (HasMark)
			{
				Mark* mark = itemText.mark(a);
				//store mark pointer and position in text
				if (mark->isType(MARKNoteMasterType))
					noteMarksPosMap.insert(a, mark);
			}

			BulNumMode = false;
			if (itemText.isBlockStart(a))
			{
				autoLeftIndent = 0.0;
				style = itemText.paragraphStyle(a);
				if (style.hasBullet() || style.hasNum())
				{
					BulNumMode = true;
				}
			}

			if (current.isEmpty)
				opticalMargins = style.opticalMargins();
			//-->#13490
			if (isNoteFrame() && !HasMark)
			{
				StyleFlag s(itemText.charStyle(a).effects());
				s &= ~ScStyle_Superscript;
				CharStyle haveSuperscript;
				haveSuperscript.setFeatures(s.featureList());
				itemText.applyCharStyle(a, 1, haveSuperscript);
			}
			//--<#13490
			CharStyle charStyle = ((itemText.text(a) != SpecialChars::PARSEP) ? itemText.charStyle(a) : style.charStyle());

			double hlcsize10 = charStyle.fontSize() / 10.0;
			double scaleV = charStyle.scaleV() / 1000.0;
			double scaleH = charStyle.scaleH() / 1000.0;
			double offset = hlcsize10 * (charStyle.baselineOffset() / 1000.0);
			style.setLineSpacing (calculateLineSpacing (style, this));
			FlopBaseline = (current.startOfCol && firstLineOffset() == FLOPBaselineGrid);

			// find out about par gap and dropcap
			if (a == firstInFrame())
			{
				if (itemText.isBlockStart(a))
				{
					if (!itemText.isBlockStart(a+1))
					{
						DropCmode = style.hasDropCap();
						if (DropCmode)
							DropLines = style.dropCapLines();
					}
					else
						DropCmode = false;
					current.lineData.isFirstLine = true;
				}
			}

			const ScFace& font = charStyle.font();

			current.glyphs[currentIndex].clearFlag(ScLayout_DropCap);
			current.glyphs[currentIndex].clearFlag(ScLayout_SoftHyphenVisible);

			// No space at begin of line,
			if (legacy)
			{
				// unless at begin of par (eeks)
				if ( (current.isEmpty) && (SpecialChars::isBreakingSpace(itemText.text(a)))
					 && (a > 0 && ! SpecialChars::isBreak(itemText.text(a-1)))
					 && ! (a > 0 && SpecialChars::isBreakingSpace(itemText.text(a-1))
						   && (!glyphClusters[i - 1].hasFlag(ScLayout_SuppressSpace))))
				{
					current.glyphs[currentIndex].setFlag(ScLayout_SuppressSpace);
					continue;
				}
				else
					current.glyphs[currentIndex].clearFlag(ScLayout_SuppressSpace);
			}
			else // from 134 on use NBSPACE for this effect
			{
				if ( current.isEmpty && (SpecialChars::isBreakingSpace(itemText.text(a)) || itemText.text(a).isSpace()))
				{
					current.glyphs[currentIndex].setFlag(ScLayout_SuppressSpace);
					continue;
				}
				else
					current.glyphs[currentIndex].clearFlag(ScLayout_SuppressSpace);
			}
			if (current.isEmpty)
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
					current.yPos = qMax(current.yPos, m_textDistanceMargins.top());
				// more about par gap and dropcaps
				if ((a > firstInFrame() && itemText.isBlockStart(a)) || (a == 0 && BackBox == 0 && current.startOfCol))
				{
					if (!current.afterOverflow && current.recalculateY && !current.startOfCol)
						current.yPos += style.gapBefore();
					DropCapDrop = 0;
					if (!itemText.isBlockStart(a+1))
						DropCmode = style.hasDropCap();
					else
						DropCmode = false;
					if (DropCmode && !current.afterOverflow)
					{
						DropLines = style.dropCapLines();
//						DropCapDrop = calculateLineSpacing (style, this) * (DropLines - 1);
//						qDebug() << QString("dropcapdrop: y=%1+%2").arg(current.yPos).arg(DropCapDrop);
					}
					current.lineData.isFirstLine = true;
				}
			}
			// find charsize factors
			if (DropCmode)
			{
				DropCapDrop = calculateLineSpacing (style, this) * (DropLines - 1);

				//text height, width, ascent and descent should be calculated for whole text provided by ScText in current position
				//and that may be more than one char (variable text for example)
				double realCharHeight = 0.0, realCharAscent = 0.0;
				foreach (const GlyphLayout& gl, current.glyphs[currentIndex].glyphs()) {
					GlyphMetrics gm = font.glyphBBox(gl.glyph);
					realCharHeight = qMax(realCharHeight, gm.ascent + gm.descent);
					realCharAscent = qMax(realCharAscent, gm.ascent);
				}
				double fontAscent = font.ascent(style.charStyle().fontSize() / 10.0);
				if (realCharHeight == 0.0)
					realCharHeight = font.height(style.charStyle().fontSize() / 10.0);
				if (realCharAscent == 0.0)
					realCharAscent = fontAscent;
				chsd = (10 * ((DropCapDrop + fontAscent) / realCharHeight));
				chs  = (10 * ((DropCapDrop + fontAscent) / realCharAscent));
				current.glyphs[currentIndex].setFlag(ScLayout_DropCap);
				if (HasObject)
				{
				//	chs = qRound((currentObject->height() + currentObject->lineWidth()) * 10);
				//	chsd = qRound((currentObject->height() + currentObject->lineWidth()) * 10);
					chs = currentObjectBox.height() * 10;
					chsd = currentObjectBox.height() * 10;
				}
			}
			else // ! dropCapMode
			{
				if (HasObject)
					chs = currentObjectBox.height() * 10;
				//	chs = qRound((currentObject->height() + currentObject->lineWidth()) * 10);
				else
					chs = charStyle.fontSize();
			}
			// set StartOfLine
			if (current.isEmpty)
				current.glyphs[currentIndex].setFlag(ScLayout_StartOfLine);
			else
				current.glyphs[currentIndex].clearFlag(ScLayout_StartOfLine);

			if (!current.glyphs[currentIndex].hasFlag(ScLayout_StartOfLine))
			{
				double tracking = charStyle.fontSize() * charStyle.tracking() / 10000.0;
				current.glyphs[currentIndex].xoffset += tracking;
				if (current.glyphs[currentIndex].width() != 0)
					current.glyphs[currentIndex].extraWidth += tracking;
			}

//			glyphs->yadvance = 0;

			if (i == current.lineData.firstCluster && current.glyphs[currentIndex].hasFlag(ScLayout_CJKFence))
			{
				current.glyphs[currentIndex].extraWidth -= (charStyle.fontSize() / 10 / 2);
				current.glyphs[currentIndex].xoffset -= (charStyle.fontSize() / 10 / 2);
			}
			// find out width, ascent and descent of char
			double wide = current.glyphs[currentIndex].width();

			if (DropCmode)
			{
				// drop caps are wider...
				GlyphCluster& glyphCluster = current.glyphs[currentIndex];
				if (HasObject)
				{
				//	double itemHeight = currentObject->height() + currentObject->lineWidth();
				//	if (itemHeight == 0)
				//		itemHeight = font.height(style.charStyle().fontSize() / 10.0);
				//	asce = currentObject->height() + currentObject->lineWidth();
				//	wide = currentObject->width() + currentObject->lineWidth();
					double itemHeight = currentObjectBox.height();
					if (itemHeight == 0)
						itemHeight = font.height(style.charStyle().fontSize() / 10.0);
					asce = currentObjectBox.height();
					wide = currentObjectBox.width();
					realAsce = calculateLineSpacing (style, this) * DropLines;
					glyphCluster.setScaleH(glyphCluster.scaleH() / glyphCluster.scaleV());
					glyphCluster.setScaleV(realAsce / itemHeight);
					glyphCluster.setScaleH(glyphCluster.scaleH() * glyphCluster.scaleV());
					wide *= glyphCluster.scaleH();
				}
				else
				{
					double realCharHeight = 0.0;
					wide = 0.0; realAsce = 0.0;
					foreach (const GlyphLayout& gl, glyphCluster.glyphs()) {
						GlyphMetrics gm;
						gm = font.glyphBBox(gl.glyph, charStyle.fontSize() / 10.0);
						realCharHeight = qMax(realCharHeight, gm.ascent + gm.descent);
						gm = font.glyphBBox(gl.glyph, chsd / 10.0);
						realAsce = qMax(realAsce, gm.ascent + gm.descent);
						wide += gm.width;
					}
					wide = (wide* scaleH) + (1 - scaleH);
					realAsce = realAsce  * scaleV + offset;
					if (realCharHeight == 0)
						realCharHeight = font.height(style.charStyle().fontSize() / 10.0);
					asce = font.ascent(hlcsize10);
					glyphCluster.setScaleH(glyphCluster.scaleH() / glyphCluster.scaleV());
					glyphCluster.setScaleV(realAsce / realCharHeight);
					glyphCluster.setScaleH(glyphCluster.scaleH() * glyphCluster.scaleV());
					glyphCluster.xoffset -= 0.5; //drop caps are always to far from column left edge
				}
				// This is to mimic pre-boxes branches in case first character of paragraph is a space
				// If we don't do this, paragraph offset will not apply correctly to first line
				if ((glyphCluster.scaleH() == 0.0) || (glyphCluster.scaleV() == 0.0))
				{
					glyphCluster.setScaleH(1.0);
					glyphCluster.setScaleV(1.0);
					wide = 0.0;
				}
				desc = realDesc = 0;
			}
			else // !DropCMode
			{
				// find ascent / descent
				if (HasObject)
					desc = realDesc = 0;
				else
				{
					if (itemText.text(a) != SpecialChars::OBJECT)
					{
						foreach (const GlyphLayout& gl, current.glyphs[currentIndex].glyphs())
						{
							GlyphMetrics gm = font.glyphBBox(gl.glyph, hlcsize10);
							realDesc = qMax(realDesc, gm.descent * scaleV - offset);
							realAsce = gm.ascent;
						}
					}
					desc = -font.descent(hlcsize10);
					current.rememberShrinkStretch(itemText.text(a), wide, style);
				}
				if (HasObject)
				{
				//	asce = currentObject->height() + currentObject->lineWidth();
					asce = currentObjectBox.height();
					realAsce = asce * scaleV + offset;
					wide = currentObjectBox.width() * scaleH;
				}
				else
				{
					asce = font.ascent(hlcsize10);
					if (HasMark && !BulNumMode)
						realAsce = asce * scaleV + offset;
					else
					{
						foreach (const GlyphLayout& gl, current.glyphs[currentIndex].glyphs())
							realAsce = qMax(realAsce, font.glyphBBox(gl.glyph, hlcsize10).ascent * scaleV + offset);
					}
				}
			}
//			if (BulNumMode)
//				hl->glyph.last()->xadvance += style.parEffectOffset();
			//check for Y position at beginning of line
			if (current.isEmpty && !current.afterOverflow)
			{
				if (current.recalculateY)
				{
					//if top of column Y position depends on first line offset
					if (current.startOfCol)
					{
						lastLineY = qMax(lastLineY, m_textDistanceMargins.top() + lineCorr);
						if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing || FlopBaseline)
						{
							if (current.yPos <= lastLineY)
								current.yPos = lastLineY +1;
							double by = m_yPos;
							if (OwnPage != -1)
								by = m_yPos - m_Doc->Pages->at(OwnPage)->yOffset();
							int ol1 = qRound((by + current.yPos - m_Doc->guidesPrefs().offsetBaselineGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_Doc->guidesPrefs().valueBaselineGrid);
							current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->guidesPrefs().valueBaselineGrid + m_Doc->guidesPrefs().offsetBaselineGrid - by;
						}
						else if (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing)
						{
							if (firstLineOffset() == FLOPRealGlyphHeight)
							{
								if (DropCmode || (itemText.text(a) == SpecialChars::PARSEP))
									current.yPos += asce;
								else
									current.yPos += realAsce;
							}
							else if (firstLineOffset() == FLOPLineSpacing)
								current.yPos += style.lineSpacing();
							else
								current.yPos += asce;
						}
					}
					else
					{
						if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						{
							current.yPos += m_Doc->guidesPrefs().valueBaselineGrid;
							double by = m_yPos;
							if (OwnPage != -1)
								by = m_yPos - m_Doc->Pages->at(OwnPage)->yOffset();
							int ol1 = qRound((by + current.yPos - m_Doc->guidesPrefs().offsetBaselineGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_Doc->guidesPrefs().valueBaselineGrid);
							current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->guidesPrefs().valueBaselineGrid + m_Doc->guidesPrefs().offsetBaselineGrid - by;
						}
						else
							current.yPos += style.lineSpacing();
					}
					if (DropCmode)
						current.yPos += DropCapDrop;
				}
				//set left indentation
				current.leftIndent = 0.0;
				if (current.addLeftIndent && ((maxDX == 0) || DropCmode || BulNumMode))
				{
					current.leftIndent = style.leftMargin() + autoLeftIndent;
					if (itemText.isBlockStart(a))
					{
						current.leftIndent += style.firstIndent();
						if (BulNumMode || DropCmode)
						{
							if (style.parEffectIndent())
							{
								current.leftIndent -= style.parEffectOffset() + wide;
								if (current.leftIndent < 0.0)
								{
									autoLeftIndent = abs(current.leftIndent);
									current.leftIndent = 0.0;
								}
							}
						}
					}
					current.addLeftIndent = false;
				}
			}
			current.recalculateY = true;
			maxYAsc = 0.0, maxYDesc = 0.0;
			double addAsce = 0.0;
			if (current.startOfCol)
			{
				if (DropCmode)
					addAsce = qMax(realAsce, asce + offset);
				else
					addAsce = asce + offset;
				if (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing)
				{
					if (firstLineOffset() == FLOPRealGlyphHeight)
						addAsce = realAsce;
					else if (firstLineOffset() == FLOPLineSpacing)
						addAsce = style.lineSpacing();
				}
				maxYAsc = current.yPos - addAsce;
			}
			else
				maxYAsc = current.yPos - realAsce;
			//fix for glyphs with negative realAsce value
			maxYAsc = qMax(maxYAsc, 0.0);
			maxYDesc = current.yPos + realDesc;

			if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
			{
				regionMinY = static_cast<int>(floor(maxYAsc));
				regionMaxY = static_cast<int>(floor(maxYDesc));
			}
			else // #11727, #11628, etc.
			{
				regionMinY = static_cast<int>(qMax(0.0, floor(current.yPos - (asce + offset))));
				regionMaxY = static_cast<int>(floor(current.yPos + desc));
			}

			if (current.isEmpty && !current.afterOverflow)
			{
				//start a new line
				goNoRoom = false;

				// find line`s start
				pt1 = QPoint(static_cast<int>(floor(current.xPos)), regionMinY);
				pt2 = QPoint(static_cast<int>(floor(current.xPos + (style.minGlyphExtension() * wide))), regionMaxY -1);
				pt = QRect(pt1, pt2);
				realEnd = 0;
				//check if there is overflow at start of line, if so jump behind it and check again
				double Xpos, Xend;
				bool done = false;
				bool newColumn = false;

				//FIX ME - that should be paragraph style`s properties
				//if set then indent is add to possible line start point (after overflow)
				//if not then indent is calculated from column left edge
				//if you don't agree that adding indent to overflow should be default behaviour
				//then change it to false
				bool addIndent2overflow = false; // should be addIndent2Overflow = style.addIndent2Overlow();
				bool addFirstIndent2overflow = true; // should be addFirstIndent2Overflow = style.addFirstIndent2Overlow();
				//if first line indent is negative and left indent should not be added to overflow
				//then don't add first line ident either
				if ((style.firstIndent() < 0) && !addIndent2overflow)
					addFirstIndent2overflow = false;

				while (!done)
				{
					Xpos = current.xPos + (addIndent2overflow ? 0 : current.leftIndent);
					Xend = current.xPos + current.leftIndent;
					//check if in indent any overflow occurs
					while (Xpos <= Xend && Xpos < current.colRight)
					{
						pt.moveTopLeft(QPoint(static_cast<int>(floor(Xpos)), regionMinY));
						if (!regionContainsRect(m_availableRegion, pt))
						{
							Xpos = current.xPos = realEnd = findRealOverflowEnd(m_availableRegion, pt, current.colRight);
							Xend = current.xPos + (addIndent2overflow ? current.leftIndent : 0);
							//for first paragraph`s line - if first line offset should be added
							if ( addFirstIndent2overflow && itemText.isBlockStart(a))
								Xend += style.firstIndent();
						}
						else
							Xpos++;
					}
					current.xPos = Xend;
					done = true;
					if (current.isEndOfLine((style.minGlyphExtension() * wide) + current.rightMargin))
					{
						// new line
						current.xPos = qMax(current.colLeft, maxDX);
						if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing || FlopBaseline)
						{
							current.yPos++;
							double by = m_yPos;
							if (OwnPage != -1)
								by = m_yPos - m_Doc->Pages->at(OwnPage)->yOffset();
							int ol1 = qRound((by + current.yPos - m_Doc->guidesPrefs().offsetBaselineGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_Doc->guidesPrefs().valueBaselineGrid);
							current.yPos = ceil(  ol2 / 10000.0 ) * m_Doc->guidesPrefs().valueBaselineGrid + m_Doc->guidesPrefs().offsetBaselineGrid - by;
						}
						else if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
							current.yPos += (current.startOfCol ? 1 : style.lineSpacing());
						else
							current.yPos += (current.startOfCol ? 1 : style.lineSpacing());
						lastLineY = maxYAsc;
						if (current.startOfCol)
						{
//							double addAsce;
//							if (DropCmode)
//								addAsce = qMax(realAsce, asce + offset);
//							else
//								addAsce = asce + offset;
//							if (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing)
//							{
//								if (firstLineOffset() == FLOPRealGlyphHeight)
//									addAsce = realAsce;
//								else if (firstLineOffset() == FLOPLineSpacing)
//									addAsce = style.lineSpacing() + offset;
//							}
							maxYAsc = current.yPos - addAsce;
						}
						else
							maxYAsc = current.yPos - realAsce;
						maxYDesc = current.yPos + realDesc;

						if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
						{
							regionMinY = static_cast<int>(floor(maxYAsc));
							regionMaxY = static_cast<int>(floor(maxYDesc));
						}
						else // #11727, #11628, etc.
						{
							regionMinY = static_cast<int>(qMax(0.0, floor(current.yPos - (asce + offset))));
							regionMaxY = static_cast<int>(floor(current.yPos + desc));
						}

						pt.moveTopLeft(QPoint(static_cast<int>(floor(current.xPos)), regionMinY));
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
					current.nextColumn(textLayout);
					current.mustLineEnd = current.colRight;
					current.restartX = current.xPos;
					lastLineY = current.yPos;
					current.rowDesc = 0;
					i--;
					current.recalculateY = true;
					current.addLeftIndent = true;
					continue;
				}
				current.lineData.x = current.restartX = current.xPos;
				current.lineData.y = current.yPos;
//				if (glyphClusters[current.line.firstCluster].hasFlag(ScLayout_DropCap))
//					current.line.y -= DropCapDrop;
			}

			//check if line must start at new Y position due to current glyph height or previous line descent
			if (!SpecialChars::isBreak(itemText.text(a), true)
				&& !SpecialChars::isBreakingSpace(itemText.text(a))
				&& !SpecialChars::isExpandingSpace(itemText.text(a))
				&& itemText.text(a) != SpecialChars::TAB)
			{
				double diff = 0;
				if (current.startOfCol || DropCmode)
					diff = realAsce - (current.yPos - lastLineY);
				else if (style.lineSpacingMode() != ParagraphStyle::FixedLineSpacing)
				{
					if (HasObject)
					//	diff = (currentObject->height() + currentObject->lineWidth()) * scaleV + offset - (current.yPos - lastLineY);
						diff = (currentObjectBox.height() * scaleV + offset) - (current.yPos - lastLineY);
					else
					{
					//	Pre-boxes code equivalent below:
					//	int glyphID = font.char2CMap(QChar('l'));
					//	diff = font.glyphBBox(glyphID, hlcsize10).ascent * scaleV + offset - (current.yPos - lastLineY);
						diff = realAsce - (current.yPos - lastLineY);
					}
				}
				else
				{
					if (HasObject)
					//	diff = (currentObject->height() + currentObject->lineWidth()) * scaleV + offset - (current.yPos - lastLineY);
						diff = currentObjectBox.height() * scaleV + offset - (current.yPos - lastLineY);
				}
				if (diff >= 1 || (!DropCmode && diff > 0))
				{
					if (current.hasDropCap && DropLinesCount == 0)
					{
						current.hasDropCap = false;
						current.yPos = maxDY;
						maxDX = 0;
					}
					int linesDrop = 0;
					if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing || FlopBaseline)
					{
						linesDrop = ceil(diff / m_Doc->guidesPrefs().valueBaselineGrid);
						current.yPos += m_Doc->guidesPrefs().valueBaselineGrid * linesDrop;
						maxDX = 0;
					}
					else /*if (current.startOfCol)*/
					{
						//FIX ME - that is ugly hack I must made, because simply expression
						//current.yPos += diff; stop working, don't know why (compiler bug?)
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
					i = current.restartRow(false);
					tabs.active = false;
					tabs.status = TabNONE;
					tabs.xPos   = 0.0;
					continue;
				}
			}
			// right tab stuff
			if (tabs.active)
			{
				if (((itemText.text(a) == '.') && (tabs.status == TabPOINT)) || ((itemText.text(a) == ',') && (tabs.status == TabCOMMA)) || (itemText.text(a) == SpecialChars::TAB))
				{
					tabs.active = false;
					tabs.status = TabNONE;
				}
			}
			// tab positioning
			if (itemText.text(a) == SpecialChars::TAB)
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
						current.xPos = nextAutoTab (current, this);
						tabs.status = TabNONE;
						tabs.active = false;
					}
					else
					{
						double tCurX = current.xPos;
						double oCurX = current.xPos - current.colLeft + wide;
						for (int yg = static_cast<int>(tTabValues.count()-1); yg > -1; yg--)
						{
							if (oCurX < tTabValues.at(yg).tabPosition)
							{
								tabs.status = static_cast<int>(tTabValues.at(yg).tabType);
								tabs.fillChar = tTabValues.at(yg).tabFillChar;
								current.xPos = current.colLeft + tTabValues.at(yg).tabPosition;
							}
						}
						tabs.active = (tabs.status != TabLEFT);
						if (current.xPos == tCurX)  // no more tabs found
						{
							current.xPos = nextAutoTab (current, this);
							tabs.status = TabNONE;
							tabs.active = false;
							tabs.fillChar = QChar();
						}

						// remember fill char
						if (!tabs.fillChar.isNull())
						{
							current.glyphs[currentIndex].setFlag(ScLayout_TabLeaders);
							GlyphLayout tglyph;
							tglyph.glyph	= font.char2CMap(tabs.fillChar.unicode());
							tglyph.scaleV   = tglyph.scaleH = chs / charStyle.fontSize();
							tglyph.xadvance = 0;
							current.glyphs[currentIndex].append(tglyph);
						}
					}
					current.xPos -= (legacy ? 1.0 : 0.0);
					GlyphLayout& firstGlyph = current.glyphs[currentIndex].glyphs().first();
					firstGlyph.xadvance = (current.xPos + wide - tabs.xPos) / firstGlyph.scaleH;
					tabs.tabGlyph = &firstGlyph;
				}
			}

			// remember y pos
			if (DropCmode)
			{
				double yoffset = 0.0;
				foreach (const GlyphLayout& gl, current.glyphs[currentIndex].glyphs())
					yoffset = qMax(yoffset, font.glyphBBox(gl.glyph, chsd / 10.0).descent);
				current.glyphs[currentIndex].yoffset -= yoffset;
			}
			// remember x pos
			double breakPos = current.xPos;

			if (!tabs.active) // normal case
			{
				current.xPos += wide;
			}
			else if (tabs.active && tabs.status == TabCENTER) 	// center tab
			{
				current.xPos += wide / 2;
				current.xPos = qMax(current.xPos, current.colLeft);
			}
			else // other tabs.active
			{
				current.xPos = qMax(current.xPos, current.colLeft);
			}
			// remember possible break
			if (shapedText.haveMoreText(i + 1, glyphClusters))
			{
				const GlyphCluster& nextCluster = glyphClusters[i + 1];
				if (nextCluster.hasFlag(ScLayout_LineBoundary))
				{
					if (!current.glyphs[currentIndex].hasFlag(ScLayout_LineBoundary)
						&& !current.glyphs[currentIndex].hasFlag(ScLayout_HyphenationPossible)
						&& (itemText.text(a) != '-')
						&& (itemText.text(a) != SpecialChars::SHYPHEN))
					{
						current.rememberBreak(i, breakPos, style.rightMargin());
					}
				}
			}
			if (HasObject)
			{
//				qDebug() << "rememberBreak object @" << i;
				current.rememberBreak(i, breakPos, style.rightMargin());
			}

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
			if (current.glyphs[currentIndex].hasFlag(ScLayout_HyphenationPossible) || itemText.text(a) == SpecialChars::SHYPHEN)
				hyphWidth = font.hyphenWidth(charStyle, hlcsize10) * (charStyle.scaleH() / 1000.0);
			if ((current.isEndOfLine(style.rightMargin() + hyphWidth)) || current.isEndOfCol(realDesc) || SpecialChars::isBreak(itemText.text(a), Cols > 1) || (current.xPos - current.maxShrink + hyphWidth) >= current.mustLineEnd)
			{
				//end of row reached - right column, end of column, break char or line must end
				if (current.isEmpty && !current.afterOverflow && !SpecialChars::isBreak(itemText.text(a), Cols > 1))
				{
					//no glyphs in line, so start new row
					if (SpecialChars::isBreak(itemText.text(a), Cols > 1))
						current.restartRowIndex = i + 1;
					i = current.restartRow(true);
					inOverflow = false;
					outs = false;
					continue;
				}
				//there we have some glyphs
				if (current.breakIndex < 0)
				{
					//force break
					if (!SpecialChars::isBreak(itemText.text(a), Cols > 1))
					{
						//force line end at previouse glyph
						i--;
						currentIndex = i - current.lineData.firstCluster;
						a = (i >= 0) ? glyphClusters.at(i).firstChar() : (a - 1);
						current.mustLineEnd = current.lineData.x;
					}
//					qDebug() << "breakline forced @" << i;
					current.breakLine(i);
				}
				if (!current.addLine && !current.lastInRowLine && current.afterOverflow)
				{
					//we reach right edge of column
					//if we are after overflow area we have to go back and try to insert text before it
					//if we have some text here - insert text WITHOUT right margin
					//if there is no place for text - insert text WITH right margin and end line
					current.lastInRowLine = false;
					if (current.lineData.firstCluster == current.restartIndex)
						current.lastInRowLine = true;
					if (current.hasDropCap && DropLinesCount == 0 && current.restartIndex == current.restartRowIndex)
					{
						current.hasDropCap = false;
						maxDX = 0;
						current.yPos = maxDY;
					}
					i = current.restartLine(false,true);
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
				if (current.isEmpty)
				{
					charStart = static_cast<int>(floor(current.lineData.x));
					charEnd = static_cast<int>(ceil(current.xPos));
				}
				else
				{
					charStart = static_cast<int>(qMax(floor(current.xPos - current.maxShrink - (style.minGlyphExtension() * wide)),0.0));
					charEnd = static_cast<int>(ceil(current.xPos - current.maxShrink));
				}
				if (legacy &&
						(((itemText.text(a) == '-' || current.glyphs[currentIndex].hasFlag(ScLayout_HyphenationPossible)) && (current.hyphenCount < style.hyphenConsecutiveLines() || style.hyphenConsecutiveLines() == 0))
						 || itemText.text(a) == SpecialChars::SHYPHEN))
				{
					if (current.glyphs[currentIndex].hasFlag(ScLayout_HyphenationPossible) || itemText.text(a) == SpecialChars::SHYPHEN)
					{
						pt1 = QPoint(charStart,  regionMinY);
						pt2 = QPoint(static_cast<int>(charEnd + hyphWidth), regionMaxY -1);
					}
					else
					{
						pt1 = QPoint(charStart, regionMinY);
						pt2 = QPoint(charEnd, regionMaxY -1);
					}
				}
				else if (!legacy && SpecialChars::isBreakingSpace(itemText.text(a)))
				{
					pt1 = QPoint(static_cast<int>(qMax(floor(breakPos - current.maxShrink - (style.minGlyphExtension() * wide)),0.0)), regionMinY);
					pt2 = QPoint(charEnd, regionMaxY -1);
				}
				else
				{
					pt1 = QPoint(charStart, regionMinY);
					pt2 = QPoint(charEnd, regionMaxY -1);
				}
				pt = QRect(pt1, pt2);
				if (!regionContainsRect(m_availableRegion, pt))
				{
					realEnd = findRealOverflowEnd(m_availableRegion, pt, current.colRight);
					outs = true;
				}
				else if (style.rightMargin() > 0.0)
				{
					if (current.lastInRowLine || current.xPos - current.maxShrink + style.rightMargin() > current.colRight - style.rightMargin())
						//condition after || is for find overflows in right margin area
					{
						pt.translate(static_cast<int>(ceil(style.rightMargin())), 0);
						if (!regionContainsRect(m_availableRegion, pt))
						{
							realEnd = findRealOverflowEnd(m_availableRegion, pt, current.colRight);
							outs = true;
						}
					}
				}
				if (outs)
				{
					if (current.isEmpty)
					{
						current.lineData.x = current.xPos = realEnd;
						i--;
						current.startLine(i+1);
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
						i--;
//						qDebug() << "forced break at glyph" << i;
						currentIndex = i - current.lineData.firstCluster;
						a = current.glyphs[currentIndex].firstChar();
						current.breakLine(i);
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
									current.mustLineEnd = current.lineData.x;
								}
								else if (current.lineData.firstCluster == current.restartIndex)
									current.lastInRowLine = true;
							}
							else
								current.lastInRowLine = true;
							if (current.hasDropCap && DropLinesCount == 0)
							{
								current.hasDropCap = false;
								maxDX = 0;
							}
							i = current.restartLine(false, true);
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
				else
					setMaxY(maxYDesc);
			}

			// hyphenation
			if ((current.glyphs[currentIndex].hasFlag(ScLayout_HyphenationPossible)
				  || itemText.text(a) == '-'
				  || itemText.text(a) == SpecialChars::SHYPHEN)
				 && (!outs) && ((i == 0) || !itemText.text(glyphClusters[i - 1].lastChar()).isSpace()) )
			{
				breakPos = current.xPos;
				if (itemText.text(a) != '-')
				{
					breakPos += hyphWidth;
				}

				double rightHang = 0;
				if (opticalMargins & ParagraphStyle::OM_RightHangingPunct) {
					rightHang = 0.7 * hyphWidth;
				}

				if (legacy || (breakPos - rightHang < current.colRight - style.rightMargin()))
				{
					if ((current.hyphenCount < style.hyphenConsecutiveLines()) || (style.hyphenConsecutiveLines() == 0) || itemText.text(a) == SpecialChars::SHYPHEN)
					{
//						qDebug() << "rememberBreak hyphen @" << i;
						current.rememberBreak(i, breakPos, style.rightMargin() + hyphWidth);
					}
				}
			}

			if ((itemText.text(a) == SpecialChars::FRAMEBREAK) && (a < itemText.length()-1))
				goNoRoom = true;
			if ((itemText.text(a) == SpecialChars::COLBREAK) && (Cols > 1))
				goNextColumn = true;

			if (i != 0 && implicitBreak(itemText.text(glyphClusters[i - 1].lastChar()), itemText.text(current.glyphs[currentIndex].firstChar())))
			{
//				qDebug() << "rememberBreak implicitbreak @" << i-1;
				current.rememberBreak(i - 1, breakPos);
			}
			current.isEmpty = (i - current.lineData.firstCluster + 1) == 0;

			if (tabs.active)
			{
				double cen = 1;
				if (tabs.status == TabCENTER)
					cen = 2;

				double newTabAdvance = tabs.tabGlyph->xadvance;
				newTabAdvance -= wide / cen / tabs.tabGlyph->scaleH;

				if (newTabAdvance >= 0) {
					tabs.tabGlyph->xadvance = newTabAdvance;
				}
				else {
					tabs.active = false;
					tabs.status = TabNONE;
				}
			}
			if ((DropCmode || BulNumMode) && !outs)
			{
				current.xPos += style.parEffectOffset();
				if (style.hasNum())
				{
					// make sure that Offset inserted only after the suffix
					// loop over previous current.glyphs and set their extraWidth to 0.0
					for (int j = 0; j < current.glyphs.size(); j++)
					{
						if (j != currentIndex)
						{
							GlyphCluster& currentGlyph = current.glyphs[j];
							current.xPos -= currentGlyph.extraWidth;
							currentGlyph.extraWidth = 0.0;
						}
					}
				}
				// set the offset for Drop Cap, Bullet & Number List
				current.glyphs[currentIndex].extraWidth += style.parEffectOffset();

				if (DropCmode)
				{
					DropCmode = false;
					DropLinesCount = 0;
					maxDY = current.yPos;
					current.hasDropCap = true;
					maxDX = current.xPos;
					double spacing = calculateLineSpacing (style, this);
					current.yPos -= spacing * (DropLines-1);
					if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
						current.yPos = adjustToBaselineGrid (current, this, OwnPage);
					current.recalculateY = false;
				}
			}
			// end of line
			if (outs)
			{
				tabs.active = false;
				tabs.status = TabNONE;
				tabs.xPos   = 0.0;
				if (SpecialChars::isBreak(itemText.text(a), Cols > 1))
				{
					// find end of line
//					qDebug() << "breakline isBreak @" << i;
					current.breakLine(i);
					regionMinY = qMax(0.0, current.lineData.y - current.lineData.ascent);
					regionMaxY = current.lineData.y + current.lineData.descent;
					EndX = current.endOfLine(m_availableRegion, style.rightMargin(), regionMinY, regionMaxY);
					current.finishLine(EndX);
					//addLine = true;
					assert(current.addLine);
					//current.startOfCol = false;
					//addLeftIndent = true;
					if (itemText.isBlockStart(a+1))
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
							current.lineData.width += current.opticalRightMargin(itemText);

						OFs = 0;
						if (style.alignment() == ParagraphStyle::Rightaligned)
							OFs = current.lineData.width - current.lineData.naturalWidth;
						if (style.alignment() == ParagraphStyle::Centered)
							OFs = (current.lineData.width - current.lineData.naturalWidth) / 2;
						if (style.alignment() == ParagraphStyle::Justified ||
							style.alignment() == ParagraphStyle::Extended)
						{
							if (style.direction() == ParagraphStyle::RTL)
								OFs = current.lineData.width - current.lineData.naturalWidth;
							else
								OFs = 0;
						}

						if (style.alignment() == ParagraphStyle::Extended
							|| (style.alignment() == ParagraphStyle::Justified
								&&  (itemText.text(a) == SpecialChars::LINEBREAK ||
									 itemText.text(a) == SpecialChars::FRAMEBREAK ||
									 itemText.text(a) == SpecialChars::COLBREAK)
								&&  (current.lineData.lastCluster - 1 >= 0)
								&&  !itemText.text(glyphClusters[current.lineData.lastCluster - 1].lastChar()).isSpace()))
						{
							current.justifyLine(style);
						}
						else
						{
							if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
								current.lineData.naturalWidth += current.opticalRightMargin(itemText);
							double optiWidth = current.colRight - style.rightMargin() - style.lineSpacing()/2.0 - current.lineData.x;
							if (current.lineData.naturalWidth > optiWidth)
								current.lineData.width = qMax(current.lineData.width - current.maxShrink, optiWidth);
							// simple offset
							current.indentLine(style, OFs);
						}
						current.xPos = current.colRight;
					}
				}
				else // outs -- last char went outside the columns (or into flow-around shape)
				{
					if (current.breakIndex >= 0)
					{
						// go back to last break position
						i = current.breakIndex;
						a = glyphClusters.at(i).firstChar();
						currentIndex = i - current.lineData.firstCluster;
						style = itemText.paragraphStyle(a);
						const_cast<ScFace&>(font) = itemText.charStyle(a).font();
						if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
							style.setLineSpacing(font.height(hlcsize10) * autoLS);
						else if (style.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
							style.setLineSpacing(m_Doc->guidesPrefs().valueBaselineGrid);
						charStyle = itemText.charStyle(a);
					}
					assert( i >= 0 );
					assert( i < glyphClusters.length() );
					//glyphs = itemText.getGlyphs(a);
					current.isEmpty = (i - current.lineData.firstCluster + 1) == 0;
					if (current.addLine && !current.isEmpty)
					{
						if (itemText.text(a) == ' ') {
							current.glyphs[currentIndex].setFlag(ScLayout_SuppressSpace);
						}

						current.updateHeightMetrics();
						//current.updateLineOffset(itemText, style, firstLineOffset());
						//current.xPos = current.breakXPos;
						EndX = current.endOfLine(m_availableRegion, current.rightMargin, regionMinY, regionMaxY);
						current.finishLine(EndX);

						hyphWidth = 0.0;
						if (current.glyphs[currentIndex].hasFlag(ScLayout_HyphenationPossible) || itemText.text(a) == SpecialChars::SHYPHEN)
						{
							// insert hyphen
							if (current.lastInRowLine)
								//increase hyphen count only for hyphens a the end of text row, omit hyphens before overflow
								current.hyphenCount++;
							current.glyphs[currentIndex].setFlag(ScLayout_SoftHyphenVisible);
							GlyphLayout hyphen;
							hyphen.glyph = font.hyphenGlyph(charStyle);
							hyphen.xadvance = font.hyphenWidth(charStyle, charStyle.fontSize() / 10.0);
							hyphWidth = hyphen.xadvance * scaleH;
							current.glyphs[currentIndex].append(hyphen);
						}
						else
						{
							if (itemText.text(a) != '-')
								current.hyphenCount = 0;
							if (current.glyphs[currentIndex].hasFlag(ScLayout_SoftHyphenVisible))
							{
								current.glyphs[currentIndex].clearFlag(ScLayout_SoftHyphenVisible);
								current.glyphs[currentIndex].glyphs().removeLast();
							}
						}

						// Justification
						if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
							current.lineData.width += current.opticalRightMargin(itemText);
						// #12565: Right alignment of hyphens
						// The additional character width has already been taken into account
						// above via the line break position, so it's not necessary to increase
						// the line natural width again:
						// line 2604: breakPos = current.xPos;
						// line 2605: if (currentCh != '-')
						// line 2606: {
						// line 2607: 	breakPos += hyphWidth;
						// line 2608: }
						/*else
							current.line.naturalWidth += hyphWidth;*/

						OFs = 0;
						if (style.alignment() == ParagraphStyle::Rightaligned)
							OFs = current.lineData.width - current.lineData.naturalWidth;
						if (style.alignment() == ParagraphStyle::Centered)
							OFs = (current.lineData.width - current.lineData.naturalWidth) / 2;
						if (style.alignment() == ParagraphStyle::Justified && style.direction() == ParagraphStyle::RTL)
							OFs = current.lineData.width - current.lineData.naturalWidth;

						if ((style.alignment() == ParagraphStyle::Justified) || (style.alignment() == ParagraphStyle::Extended))
							current.justifyLine(style);
						else
						{
							if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
								current.lineData.naturalWidth += current.opticalRightMargin(itemText);
							current.indentLine(style, OFs);
						}
						current.xPos = current.lineData.x + current.lineData.width;
					}
				}
				if (inOverflow)
					current.xPos = realEnd;

				// line break and drop caps
				// #11250: in case of a forced line break, we must not stop
				// the drop cap layout process. This break case such as
				// layout of poetry.
				if (itemText.isBlockStart(a+1) && current.hasDropCap)
				{
					current.hasDropCap = false;
					if (current.yPos < maxDY)
						current.yPos = maxDY;
					maxDX = 0;
				}
				// end of column
				if (current.isEndOfCol(desc))
				{
					//check if really line extends bottom margin
					current.updateHeightMetrics();
					if (current.isEndOfCol(current.lineData.descent))
					{
						if (current.isEmpty || current.column+1 == Cols)
						{
							goNoRoom = true;
							MaxChars = a + 1;
							break;
						}
						goNextColumn = true;
					}
					else
						setMaxY(maxYDesc);
				}
				else
					setMaxY(maxYDesc);
				if (current.lineData.firstCluster <= current.lineData.lastCluster && !current.isEmpty)
				{
					if (current.addLine && current.breakIndex >= 0)
					{
						if (current.glyphs[0].hasFlag(ScLayout_DropCap))
						{
							// put line back to top
							current.lineData.y -= DropCapDrop;
							current.glyphs[0].yoffset += DropCapDrop;
						}
						current.fillInTabLeaders();
						//if right margin is set we temporally save line, not append it
						textLayout.appendLine(current.createLineBox());
						setMaxY(maxYDesc);
						current.restartIndex = current.lineData.lastCluster + 1;
						i = current.lineData.lastCluster;
						currentIndex = i - current.lineData.firstCluster;
						a = current.glyphs[currentIndex].firstChar();
						current.rowDesc = qMax(current.rowDesc,current.yPos + current.lineData.descent);
					//	if (glyphClusters[current.line.firstCluster].hasFlag(ScLayout_DropCap))
					//		current.rowDesc = qMax(current.rowDesc,current.yPos + current.line.descent);
					//	else
							current.rowDesc = qMax(current.rowDesc,current.yPos - current.lineData.descent);
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
						i = current.restartLine(true, true);
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
				if ( SpecialChars::isBreak(itemText.text(a)) )
				{
					if (itemText.isBlockStart(a+1))
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
				// WTF does i+1 mean here, what if i is the last run we have!
				current.startLine(i+1);
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
						current.nextColumn(textLayout);
						current.mustLineEnd = current.colRight;
						current.addLeftIndent = true;
						lastLineY = m_textDistanceMargins.top();
						current.rowDesc = 0;
						current.recalculateY = true;
					}
					else
					{
						MaxChars = a; // Always a+1???
						if (itemText.text(a) == SpecialChars::COLBREAK)
							++MaxChars;
						goto NoRoom;
					}
				}
			}
			if (!shapedText.haveMoreText(i + 1, glyphClusters))
			{
				if (!current.afterOverflow || current.addLine)
				{
					current.addLine = true;
					current.lastInRowLine = true;
//					qDebug() << "breakline A no more text @" << i;
					current.breakLine(i);
				}
				if (current.afterOverflow && !current.addLine)
				{
					if (current.restartIndex < current.lineData.firstCluster)
					{
						i = current.restartLine(false,true);
						continue;
					}
					else
					{
//						qDebug() << "breakline B no more text @" << i;
						current.breakLine(i);
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
// now place the last line
		if (!current.isEmpty)
		{
			int a = itemText.length()-1;
			int i = glyphClusters.length() - 1;
//			qDebug() << "breakline end of text @" << i;
			current.breakLine(i);

			if (current.startOfCol)
			{
				double addAsce;
				if (DropCmode)
					addAsce = qMax(realAsce, asce + offset);
				else
					addAsce = asce + offset;
				if (style.lineSpacingMode() != ParagraphStyle::BaselineGridLineSpacing)
				{
					if (firstLineOffset() == FLOPRealGlyphHeight)
						addAsce = realAsce;
					else if (firstLineOffset() == FLOPLineSpacing)
						addAsce = style.lineSpacing() + offset;
				}
				maxYAsc = current.yPos - addAsce;
			}
			else
				maxYAsc = current.yPos - realAsce;
			//fix for glyphs with negative realAsce value
			maxYAsc = qMax(maxYAsc, 0.0);
			maxYDesc = current.yPos + realDesc;

			if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
			{
				regionMinY = static_cast<int>(floor(maxYAsc));
				regionMaxY = static_cast<int>(floor(maxYDesc));
			}
			else // #11727, #11628, etc.
			{
				regionMinY = static_cast<int>(qMax(0.0, floor(current.yPos - (realAsce + offset))));
				regionMaxY = static_cast<int>(floor(current.yPos + realDesc));
			}

			EndX = current.endOfLine(m_availableRegion, style.rightMargin(), regionMinY, regionMaxY);
			current.finishLine(EndX);

			if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
				current.lineData.width += current.opticalRightMargin(itemText);

			OFs = 0;
			if (style.alignment() == ParagraphStyle::Rightaligned)
				OFs = current.lineData.width - current.lineData.naturalWidth;
			if (style.alignment() == ParagraphStyle::Centered)
				OFs = (current.lineData.width - current.lineData.naturalWidth) / 2;
			if (style.alignment() == ParagraphStyle::Justified ||
				style.alignment() == ParagraphStyle::Extended)
			{
				if (style.direction() == ParagraphStyle::RTL)
					OFs = current.lineData.width - current.lineData.naturalWidth;
				else
					OFs = 0;
			}

			if (style.alignment() == ParagraphStyle::Extended
				|| (style.alignment() == ParagraphStyle::Justified
					&&  (itemText.text(a) == SpecialChars::LINEBREAK ||
						 itemText.text(a) == SpecialChars::FRAMEBREAK ||
						 itemText.text(a) == SpecialChars::COLBREAK)
					&&  !itemText.text(a).isSpace()))
			{
				current.justifyLine(style);
			}
			else
			{
				if (opticalMargins & ParagraphStyle::OM_RightHangingPunct)
					current.lineData.naturalWidth += current.opticalRightMargin(itemText);
				current.indentLine(style, OFs);
			}
			if (current.glyphs[0].hasFlag(ScLayout_DropCap))
			{
				// put line back to top
				current.lineData.y -= DropCapDrop;
				current.glyphs[0].yoffset += DropCapDrop;
			}
			current.fillInTabLeaders();
			current.startOfCol = false;
			goNextColumn = false;

			textLayout.appendLine(current.createLineBox());
			setMaxY(maxYDesc);
			current.startOfCol = false;

			if (moveLinesFromPreviousFrame ()) {
				layout ();  // line moving ensures that this won't be an endless loop
				itemText.blockSignals(false);
				return;
			}
		}
	}
	MaxChars = itemText.length();
	if (verticalAlign > 0)
	{
		double hAdjust = height() - textLayout.box()->naturalHeight() - m_textDistanceMargins.bottom();
		if (hAdjust > 0)
		{
			if (verticalAlign == 1)
				hAdjust /= 2;
			if (FrameType == 0) // Rectangular frame
				textLayout.box()->moveBy(0, hAdjust);
			else
			{
				int vertAlign = verticalAlign;
				double topDist = m_textDistanceMargins.top();
				m_textDistanceMargins.setTop(topDist + hAdjust);
				verticalAlign = 0;
				layout();
				verticalAlign = vertAlign;
				m_textDistanceMargins.setTop(topDist);
			}
		}
	}
	invalid = false;
	if (!isNoteFrame() && (!m_Doc->notesList().isEmpty() || m_Doc->notesChanged()))
	{ //if notes are used
		UndoManager::instance()->setUndoEnabled(false);
		NotesInFrameMap notesMap = updateNotesFrames(noteMarksPosMap);
		if (notesMap != m_notesFramesMap)
		{
			updateNotesMarks(notesMap);
			notesFramesLayout();
		}
		UndoManager::instance()->setUndoEnabled(true);
	}
	if (NextBox != NULL)
	{
		PageItem_TextFrame * nextFrame = dynamic_cast<PageItem_TextFrame*>(NextBox);
		while (nextFrame)
		{
			nextFrame->invalid   = true;
			nextFrame->firstChar = MaxChars;
			nextFrame = dynamic_cast<PageItem_TextFrame*>(nextFrame->NextBox);
		}
	}
	itemText.blockSignals(false);
//	qDebug("textframe: len=%d, done relayout", itemText.length());
	return;

NoRoom:
	invalid = false;
	
	adjustParagraphEndings ();

	if (verticalAlign > 0)
	{
		double hAdjust = height() - textLayout.box()->naturalHeight() - m_textDistanceMargins.bottom();
		if (hAdjust > 0)
		{
			if (verticalAlign == 1)
				hAdjust /= 2;
			if (FrameType == 0) // Rectangular frame
				textLayout.box()->moveBy(0, hAdjust);
			else
			{
				int vertAlign = verticalAlign;
				double topDist = m_textDistanceMargins.top();
				m_textDistanceMargins.setTop(topDist + hAdjust);
				verticalAlign = 0;
				layout();
				verticalAlign = vertAlign;
				m_textDistanceMargins.setTop(topDist);
			}
		}
	}

	if (!isNoteFrame() && (!m_Doc->notesList().isEmpty() || m_Doc->notesChanged()))
	{
		UndoManager::instance()->setUndoEnabled(false);
		NotesInFrameMap notesMap = updateNotesFrames(noteMarksPosMap);
		if (notesMap != m_notesFramesMap)
		{
			updateNotesMarks(notesMap);
			notesFramesLayout();
		}
		UndoManager::instance()->setUndoEnabled(true);
	}

	PageItem_TextFrame * next = dynamic_cast<PageItem_TextFrame*>(NextBox);
	if (next != NULL)
	{
		if (itemText.cursorPosition() > signed(MaxChars))
		{
			int nCP = itemText.cursorPosition();
			if (m_Doc->appMode == modeEdit)
				next->itemText.setCursorPosition( qMax(nCP, signed(MaxChars)) );
		}
		while (next)
		{
			next->invalid   = true;
			next->firstChar = MaxChars;
			next = dynamic_cast<PageItem_TextFrame*>(next->NextBox);
		}
	}
//	qDebug("textframe: len=%d, done relayout (no room %d)", itemText.length(), MaxChars);
	itemText.blockSignals(false);
}

void PageItem_TextFrame::invalidateLayout(bool wholeChain)
{
	//const bool wholeChain = true;
	invalid = true;
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

void PageItem_TextFrame::slotInvalidateLayout(int firstItem, int endItem)
{
	PageItem* firstFrame = firstInChain();
	firstItem = itemText.prevParagraph(firstItem);

	PageItem_TextFrame* firstInvalid = dynamic_cast<PageItem_TextFrame*>(firstFrame);
	while (firstInvalid)
	{
		if (firstInvalid->invalid)
			break;
		if (firstInvalid->firstChar <= firstItem && firstItem <= firstInvalid->MaxChars)
			break;
		firstInvalid = dynamic_cast<PageItem_TextFrame*>(firstInvalid->NextBox);
	}

	PageItem_TextFrame* invalidFrame = firstInvalid;
	while (invalidFrame)
	{
		invalidFrame->invalid = true;
		invalidFrame = dynamic_cast<PageItem_TextFrame*>(invalidFrame->NextBox);
	}
}

bool PageItem_TextFrame::isValidChainFromBegin()
{
	if (invalid)
		return false;
	if (BackBox == NULL)
		return !invalid;

	PageItem* prev = prevInChain();
	while (prev != NULL)
	{
		if (prev->invalid)
			return false;
		prev = prev->prevInChain();
	}
	return true;
}

void PageItem_TextFrame::DrawObj_Item(ScPainter *p, QRectF cullingArea)
{
	if (invalid)
	{
		if (isNoteFrame() && asNoteFrame()->deleteIt)
		//do not layout notes frames which should be deleted
			return;
		layout();
	}
	if (invalid)
		return;
	if (m_Doc->RePos)
		return;
	QTransform pf2;
	//	tTabValues.clear();
	p->save(); //SA1

	if (!isEmbedded)
		pf2.translate(m_xPos, m_yPos);
	pf2.rotate(m_rotation);

	if (!m_Doc->layerOutline(LayerID))
	{
		if ((fillColor() != CommonStrings::None) || (GrType != 0))
		{
			if (isAnnotation() && !((m_Doc->appMode == modeEdit) && (m_Doc->m_Selection->findItem(this) != -1)) && ((annotation().Type() == 2) || (annotation().Type() == 5) || (annotation().Type() == 6)))
			{
				if ((annotation().Feed() == 1) && annotation().IsOn())
					p->setBrush(QColor(255 - fillQColor.red(), 255 - fillQColor.green(), 255 - fillQColor.blue(), fillQColor.alpha()));
			}
			p->setupPolygon(&PoLine);
			p->fillPath();
		}
	}
	MarginStruct savedTextDistanceMargins(m_textDistanceMargins);
	if (isAnnotation() && !((m_Doc->appMode == modeEdit) && (m_Doc->m_Selection->findItem(this) != -1)) && (((annotation().Type() > 1) && (annotation().Type() < 11)) || (annotation().Type() > 12)))
	{

		if ((m_Doc->drawAsPreview && !m_Doc->editOnPreview) && ((annotation().Vis() == 1) || (annotation().Vis() == 3)))
		{
			p->restore();
			return;
		}
		QColor fontColor;
		SetQColor(&fontColor, itemText.defaultStyle().charStyle().fillColor(), itemText.defaultStyle().charStyle().fillShade());
		double fontSize = itemText.defaultStyle().charStyle().fontSize() / 10.0;
		ScFace font = itemText.defaultStyle().charStyle().font();
		QString bmUtf16("");
		QString cc;
		if (!((itemText.length() == 1) && (itemText.text(0, 1) == QChar(13))))
		{
			for (uint d = 0; d < static_cast<uint>(itemText.length()); ++d)
			{
				if (d == 0)
				{
					const CharStyle& charStyle(itemText.charStyle(d));
					SetQColor(&fontColor, charStyle.fillColor(), charStyle.fillShade());
					fontSize = charStyle.fontSize() / 10.0;
					font = charStyle.font();
				}
				cc = itemText.text(d, 1);
				bmUtf16 += (cc == QChar(13) ? QChar(10) : cc);
			}
		}
		p->save();
		if ((annotation().Bwid() > 0) && (annotation().borderColor() != CommonStrings::None) && (annotation().Type() != Annotation::Text))
		{
			QColor tmp;
			SetQColor(&tmp, annotation().borderColor(), 100);
			int BStyle = annotation().Bsty();
			if (annotation().IsOn())
			{
				if (annotation().Feed() == 2)
					tmp = QColor(255 - tmp.red(), 255 - tmp.green(), 255 - tmp.blue(), tmp.alpha());
				if (annotation().Feed() == 3)
					BStyle = 4;
			}
			if (annotation().Type() == Annotation::RadioButton)
			{
				double bwh = annotation().Bwid() / 2.0;
				if (annotation().IsOn())
				{
					if (BStyle == 4)
						BStyle = 3;
					else
						BStyle = 4;
				}
				if ((BStyle == 0) || (BStyle == 1))
				{
					QPainterPath clp;
					clp.addEllipse(QRectF(bwh, bwh, m_width - annotation().Bwid(), m_height - annotation().Bwid()));
					FPointArray clpArr;
					clpArr.fromQPainterPath(clp);
					p->setupPolygon(&clpArr);
					p->setPen(tmp, annotation().Bwid(), BStyle == 0 ? Qt::SolidLine : Qt::DashLine, Qt::FlatCap, Qt::MiterJoin);
					p->setFillMode(ScPainter::None);
					p->setStrokeMode(ScPainter::Solid);
					p->strokePath();
				}
				else if (BStyle == 3)
					p->drawShadeCircle(QRectF(0, 0, m_width, m_height), tmp, false, annotation().Bwid());
				else if (BStyle == 4)
					p->drawShadeCircle(QRectF(0, 0, m_width, m_height), tmp, true, annotation().Bwid());
			}
			else
			{
				if ((annotation().Type() == Annotation::Checkbox) && annotation().IsOn())
				{
					if (BStyle == 4)
						BStyle = 3;
					else
						BStyle = 4;
				}
				if (BStyle == 3)
					p->drawShadePanel(QRectF(0, 0, m_width, m_height), tmp, false, annotation().Bwid());
				else if (BStyle == 4)
					p->drawShadePanel(QRectF(0, 0, m_width, m_height), tmp, true, annotation().Bwid());
				else
				{
					p->setPen(tmp, annotation().Bwid(), BStyle == 0 ? Qt::SolidLine : Qt::DashLine, Qt::FlatCap, Qt::MiterJoin);
					p->setStrokeMode(ScPainter::Solid);
					p->drawRect(0, 0, m_width, m_height);
				}
			}
		}
		if (annotation().Type() == Annotation::Button)
		{
			int wdt = annotation().Bwid();
			QPainterPath clp;
			clp.addRect(QRectF(wdt, wdt, m_width - (2 * wdt), m_height - (2 * wdt)));
			FPointArray clpArr;
			clpArr.fromQPainterPath(clp);
			p->setupPolygon(&clpArr);
			p->setClipPath();
			if (!bmUtf16.isEmpty())
			{
				if ((annotation().Feed() == 1) && annotation().IsOn())
					p->setPen(QColor(255 - fontColor.red(), 255 - fontColor.green(), 255 - fontColor.blue(), fontColor.alpha()));
				else
					p->setPen(fontColor);
				p->setFont(font, fontSize);
				p->drawText(QRectF(wdt, wdt, m_width - (2 * wdt), m_height - (2 * wdt)), bmUtf16, false);
			}
			if ((!Pfile.isEmpty()) && (imageIsAvailable) && (m_imageVisible) && (annotation().UseIcons()))
			{
				p->save();//SA2
				p->setupPolygon(&PoLine);
				p->setClipPath();
				p->scale(m_imageXScale, m_imageYScale);
				p->translate(m_imageXOffset*m_imageXScale, m_imageYOffset*m_imageYScale);
				p->rotate(m_imageRotation);
				if (pixm.width() > 0 && pixm.height() > 0)
					p->drawImage(pixm.qImagePtr());
				p->restore();//RE2
			}
			p->restore();
			p->restore();
			return;
		}
		else if (annotation().Type() == Annotation::Textfield)
		{
			int wdt = annotation().Bwid();
			m_textDistanceMargins.set(wdt, wdt, wdt, wdt);
			invalid = true;
			layout();
		}
		else if (annotation().Type() == Annotation::RadioButton)
		{
			if (annotation().IsChecked())
			{
				QPainterPath clp2;
				double siz = qMin(width(), height()) * 0.4;
				QRectF sizR(0, 0, siz, siz);
				sizR.moveCenter(QPointF(width() / 2.0, height() / 2.0));
				clp2.addEllipse(sizR);
				FPointArray clpArr2;
				clpArr2.fromQPainterPath(clp2);
				p->setBrush(fontColor);
				p->setFillMode(ScPainter::Solid);
				p->setupPolygon(&clpArr2);
				p->fillPath();
			}
			p->restore();
			p->restore();
			return;
		}
		else if (annotation().Type() == Annotation::Checkbox)
		{
			if (annotation().IsChecked())
			{
				p->setBrush(fontColor);
				p->setFillMode(ScPainter::Solid);
				p->setStrokeMode(ScPainter::None);
				QPainterPath chkPath;
				if (annotation().ChkStil() == 0)
				{
					chkPath.moveTo(6.60156, 0);
					chkPath.lineTo(6.7725, 0.244063);
					chkPath.cubicTo(6.7725, 0.244063, 5.7275, 1.03031, 4.44813, 2.66609);
					chkPath.cubicTo(4.44813, 2.66609, 3.16891, 4.30172, 2.49516, 5.72266);
					chkPath.lineTo(2.13375, 5.96672);
					chkPath.cubicTo(2.13375, 5.96672, 1.68453, 6.27922, 1.52344, 6.43062);
					chkPath.cubicTo(1.52344, 6.43062, 1.46, 6.20109, 1.24516, 5.67875);
					chkPath.lineTo(1.10844, 5.36125);
					chkPath.cubicTo(1.10844, 5.36125, 0.815469, 4.67766, 0.563906, 4.35062);
					chkPath.cubicTo(0.563906, 4.35062, 0.3125, 4.02344, 0, 3.91594);
					chkPath.cubicTo(0, 3.91594, 0.527344, 3.35938, 0.966875, 3.35938);
					chkPath.cubicTo(0.966875, 3.35938, 1.34281, 3.35938, 1.80172, 4.37984);
					chkPath.lineTo(1.95312, 4.72172);
					chkPath.cubicTo(1.95312, 4.72172, 2.77828, 3.33, 4.07219, 2.01656);
					chkPath.cubicTo(4.07219, 2.01656, 5.36625, 0.703125, 6.60156, 0);
					chkPath.closeSubpath();
				}
				else if (annotation().ChkStil() == 1)
				{
					chkPath.moveTo(2.42672, 3.30078);
					chkPath.lineTo(2.35359, 3.40328);
					chkPath.cubicTo(2.35359, 3.40328, 1.59187, 4.55563, 1.10359, 4.55563);
					chkPath.cubicTo(1.10359, 4.55563, 0.542031, 4.55563, 0, 3.64266);
					chkPath.cubicTo(0, 3.64266, 0.0732812, 3.6475, 0.1075, 3.6475);
					chkPath.cubicTo(0.1075, 3.6475, 0.747031, 3.6475, 1.53328, 2.67094);
					chkPath.lineTo(1.665, 2.50484);
					chkPath.lineTo(1.49422, 2.31937);
					chkPath.cubicTo(1.49422, 2.31937, 0.776406, 1.55766, 0.776406, 1.02547);
					chkPath.cubicTo(0.776406, 1.02547, 0.776406, 0.590781, 1.5625, 0);
					chkPath.cubicTo(1.5625, 0, 1.67484, 0.756875, 2.23141, 1.47469);
					chkPath.lineTo(2.37797, 1.66016);
					chkPath.lineTo(2.49516, 1.50875);
					chkPath.cubicTo(2.49516, 1.50875, 3.31062, 0.46875, 3.97469, 0.46875);
					chkPath.cubicTo(3.97469, 0.46875, 4.49219, 0.46875, 4.76078, 1.25484);
					chkPath.cubicTo(4.76078, 1.25484, 4.68266, 1.24516, 4.64844, 1.24516);
					chkPath.cubicTo(4.64844, 1.24516, 4.41406, 1.24516, 3.98922, 1.56016);
					chkPath.cubicTo(3.98922, 1.56016, 3.56453, 1.875, 3.27156, 2.27047);
					chkPath.lineTo(3.125, 2.47078);
					chkPath.lineTo(3.26656, 2.6075);
					chkPath.cubicTo(3.26656, 2.6075, 4.05281, 3.36922, 4.90719, 3.36922);
					chkPath.cubicTo(4.90719, 3.36922, 4.44828, 4.29687, 3.95016, 4.29687);
					chkPath.cubicTo(3.95016, 4.29687, 3.50094, 4.29687, 2.65141, 3.50594);
					chkPath.lineTo(2.42672, 3.30078);
					chkPath.closeSubpath();
				}
				else if (annotation().ChkStil() == 2)
				{
					chkPath.moveTo(0, 4.09187);
					chkPath.lineTo(2.89062, 0);
					chkPath.lineTo(5.78125, 4.09187);
					chkPath.lineTo(2.89062, 8.17875);
					chkPath.closeSubpath();
				}
				else if (annotation().ChkStil() == 3)
				{
					chkPath.moveTo(0, 2.89062);
					chkPath.cubicTo(0, 2.89062, 0, 1.69437, 0.847187, 0.847187);
					chkPath.cubicTo(0.847187, 0.847187, 1.69437, 0, 2.89062, 0);
					chkPath.cubicTo(2.89062, 0, 4.08703, 0, 4.93656, 0.847187);
					chkPath.cubicTo(4.93656, 0.847187, 5.78625, 1.69437, 5.78625, 2.89062);
					chkPath.cubicTo(5.78625, 2.89062, 5.78625, 4.08688, 4.93656, 4.93406);
					chkPath.cubicTo(4.93656, 4.93406, 4.08703, 5.78125, 2.89062, 5.78125);
					chkPath.cubicTo(2.89062, 5.78125, 1.69437, 5.78125, 0.847187, 4.93406);
					chkPath.cubicTo(0.847187, 4.93406, 0, 4.08688, 0, 2.89062);
					chkPath.closeSubpath();
				}
				else if (annotation().ChkStil() == 4)
				{
					chkPath.moveTo(0, 2.49516);
					chkPath.lineTo(2.62219, 2.49516);
					chkPath.lineTo(3.43266, 0);
					chkPath.lineTo(4.24328, 2.49516);
					chkPath.lineTo(6.86531, 2.49516);
					chkPath.lineTo(4.74609, 4.03812);
					chkPath.lineTo(5.55672, 6.53313);
					chkPath.lineTo(3.43266, 4.99016);
					chkPath.lineTo(1.30859, 6.53313);
					chkPath.lineTo(2.11922, 4.03812);
					chkPath.closeSubpath();
				}
				else if (annotation().ChkStil() == 5)
				{
					chkPath.moveTo(0, 5.78125);
					chkPath.lineTo(0, 0);
					chkPath.lineTo(5.78125, 0);
					chkPath.lineTo(5.78125, 5.78125);
					chkPath.closeSubpath();
				}
				if (!chkPath.isEmpty())
				{
					QTransform mm;
					mm.scale(fontSize / 10.0, fontSize / 10.0);
					chkPath = mm.map(chkPath);
					QRectF bb = chkPath.boundingRect();
					QRectF bi = QRectF(0.0, 0.0, m_width, m_height);
					double dx = bi.center().x() - (bb.width() / 2.0);
					double dy = bi.center().y() - (bb.height() / 2.0);
					p->translate(dx, dy);
					FPointArray chArr;
					chArr.fromQPainterPath(chkPath);
					p->setupPolygon(&chArr);
					p->fillPath();
				}
				p->restore();
				p->restore();
				return;
			}
		}
		else if (annotation().Type() == Annotation::Combobox)
		{
			int wdt = annotation().Bwid();
			if (m_width > 2 * wdt + 15)
			{
				if (!bmUtf16.isEmpty())
				{
					p->save();
					QPainterPath clp;
					clp.addRect(QRectF(wdt + 1, wdt + 1, m_width - (2 * wdt) - 17, m_height - (2 * wdt) - 2));
					FPointArray clpArr;
					clpArr.fromQPainterPath(clp);
					p->setupPolygon(&clpArr);
					p->setClipPath();
					p->setPen(fontColor);
					p->setFont(font, fontSize);
					QStringList textList = bmUtf16.split("\n");
					p->drawText(QRectF(wdt + 1, wdt + 1, m_width - (2 * wdt) - 17, m_height - (2 * wdt) - 2), textList[0], false, 1);
					p->restore();
				}
				p->setFillMode(ScPainter::Solid);
				p->setStrokeMode(ScPainter::None);
				p->setBrush(QColor(200, 200, 200));
				QRectF bi;
				if ((annotation().Bsty() == 3) || (annotation().Bsty() == 4))
					bi = QRectF(m_width - wdt - 15, wdt, 15, m_height - (2 * wdt));
				else
					bi = QRectF(m_width - (wdt / 2.0) - 15, wdt / 2.0, 15, m_height - wdt);
				p->drawRect(bi.x(), bi.y(), bi.width(), bi.height());
				QPainterPath chkPath;
				chkPath.moveTo(bi.center().x() - 3, bi.center().y());
				chkPath.lineTo(bi.center().x() + 3, bi.center().y());
				chkPath.lineTo(bi.center().x(), bi.center().y() + 4);
				chkPath.closeSubpath();
				FPointArray chArr;
				chArr.fromQPainterPath(chkPath);
				p->setupPolygon(&chArr);
				p->setBrush(QColor(0, 0, 0));
				p->fillPath();
				p->restore();
				p->restore();
				return;
			}
		}
		else if (annotation().Type() == Annotation::Listbox)
		{
			int wdt = annotation().Bwid();
			if (m_width > 2 * wdt + 15)
			{
				if (!bmUtf16.isEmpty())
				{
					p->save();
					QPainterPath clp;
					clp.addRect(QRectF(wdt + 1, wdt + 1, m_width - (2 * wdt) - 17, m_height - (2 * wdt) - 2));
					FPointArray clpArr;
					clpArr.fromQPainterPath(clp);
					p->setupPolygon(&clpArr);
					p->setClipPath();
					p->setPen(fontColor);
					p->setFont(font, fontSize);
					p->drawText(QRectF(wdt + 1, wdt + 1, m_width - (2 * wdt) - 17, m_height - (2 * wdt) - 2), bmUtf16, false, 2);
					p->restore();
				}
				p->restore();
				p->restore();
				return;
			}
		}
		else if (annotation().Type() == Annotation::Text)
		{
			if (m_Doc->drawAsPreview && !m_Doc->editOnPreview)
			{
				if (annotation().IsOpen())
				{
					p->save();
					p->translate(m_origAnnotPos.x() - xPos(), m_origAnnotPos.y() - yPos());
					drawNoteIcon(p);
					p->restore();
					p->save();
					double basX = 15;
					double basY = 15;
					QPainterPath clp;
					clp.addRoundedRect(basX, basY, 250, 250, 5, 5);
					p->setBrush(QColor(240, 240, 0));
					p->setFillMode(ScPainter::Solid);
					p->setStrokeMode(ScPainter::None);
					FPointArray clpArr;
					clpArr.fromQPainterPath(clp);
					p->setupPolygon(&clpArr);
					p->fillPath();
					p->setPen(QColor(0, 0, 0), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					p->setBrush(QColor(255, 255, 255));
					p->setStrokeMode(ScPainter::Solid);
					p->drawRect(basX + 10, basY + 20, 230, 220);
					p->setFillMode(ScPainter::None);
					p->drawRect(basX + 230, basY + 5, 11, 11);
					p->setPen(QColor(0, 0, 0), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					p->drawLine(QPointF(basX + 232, basY + 13), QPointF(basX + 239, basY + 13));
					p->save();
					p->translate(basX + 10, basY + 5);
					p->scale(0.5, 0.5);
					drawNoteIcon(p);
					p->restore();
					clp = QPainterPath();
					clp.addRect(basX + 10, basY + 20, 230, 220);
					clpArr.fromQPainterPath(clp);
					p->setupPolygon(&clpArr);
					p->setClipPath();
					p->setPen(fontColor);
					p->setFont(font, fontSize);
					p->drawText(QRectF(basX + 11, basY + 21, 228, 218), bmUtf16, false, 2);
					p->restore();
				}
				else
					drawNoteIcon(p);
				p->restore();
				p->restore();
				return;
			}
		}
		p->restore();
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
			p->translate(m_width, 0);
			p->scale(-1, 1);
			pf2.translate(m_width, 0);
			pf2.scale(-1, 1);
		}
		if (imageFlippedV())
		{
			p->translate(0, m_height);
			p->scale(1, -1);
			pf2.translate(0, m_height);
			pf2.scale(1, -1);
		}
		assert( firstInFrame() >= 0 );
		assert( lastInFrame() < itemText.length() );

		int fm = p->fillMode();
		int sm = p->strokeMode();
		p->setFillMode(1);
		p->setStrokeMode(1);
		ScreenPainter painter(p, this);
		textLayout.renderBackground(&painter);
		textLayout.render(&painter, this);
		p->setFillMode(fm);
		p->setStrokeMode(sm);
	}
	m_textDistanceMargins=savedTextDistanceMargins;
	p->restore();//RE1
}

void PageItem_TextFrame::DrawObj_Post(ScPainter *p)
{
	if (m_Doc->layerOutline(LayerID))
	{
		p->setPen(m_Doc->layerMarker(LayerID), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::None);
		p->setBrushOpacity(1.0);
		p->setPenOpacity(1.0);
		p->setupPolygon(&PoLine);
		p->strokePath();
	}
	else
	{
		if (fillBlendmode() != 0)
			p->setBlendModeFill(0);
		p->setMaskMode(0);
		
		if (!isAnnotation() || (isAnnotation() && ((annotation().Type() == 0) || (annotation().Type() > 6))))
		{
			p->setBlendModeStroke(lineBlendmode());
			p->setPenOpacity(1.0 - lineTransparency());
			p->setupPolygon(&PoLine);
			if (NamedLStyle.isEmpty())
			{
				if ((lineColor() != CommonStrings::None) || (!patternStrokeVal.isEmpty()) || (GrTypeStroke > 0))
				{
					p->setPen(strokeQColor, m_lineWidth, PLineArt, PLineEnd, PLineJoin);
					if (DashValues.count() != 0)
						p->setDash(DashValues, DashOffset);
				}
				ScPattern *strokePattern = m_Doc->checkedPattern(patternStrokeVal);
				if (strokePattern)
				{
					if (patternStrokePath)
					{
						QPainterPath guidePath = PoLine.toQPainterPath(false);
						DrawStrokePattern(p, guidePath);
					}
					else
					{
						p->setPattern(&m_Doc->docPatterns[patternStrokeVal], patternStrokeScaleX, patternStrokeScaleY, patternStrokeOffsetX, patternStrokeOffsetY, patternStrokeRotation, patternStrokeSkewX, patternStrokeSkewY, patternStrokeMirrorX, patternStrokeMirrorY);
						p->setStrokeMode(ScPainter::Pattern);
						p->strokePath();
					}
				}
				else if (GrTypeStroke > 0)
				{
					if ((!gradientStrokeVal.isEmpty()) && (!m_Doc->docGradients.contains(gradientStrokeVal)))
						gradientStrokeVal = "";
					if (!(gradientStrokeVal.isEmpty()) && (m_Doc->docGradients.contains(gradientStrokeVal)))
						stroke_gradient = m_Doc->docGradients[gradientStrokeVal];
					if (stroke_gradient.Stops() < 2) // fall back to solid stroking if there are not enough colorstops in the gradient.
					{
						if (lineColor() != CommonStrings::None)
						{
							p->setBrush(strokeQColor);
							p->setStrokeMode(ScPainter::Solid);
						}
						else
						{
							no_stroke = true;
							p->setStrokeMode(ScPainter::None);
						}
					}
					else
					{
						p->setStrokeMode(ScPainter::Gradient);
						p->stroke_gradient = stroke_gradient;
						if (GrTypeStroke == 6)
							p->setGradient(VGradient::linear, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeStartX, GrStrokeStartY), GrStrokeScale, GrStrokeSkew);
						else
							p->setGradient(VGradient::radial, FPoint(GrStrokeStartX, GrStrokeStartY), FPoint(GrStrokeEndX, GrStrokeEndY), FPoint(GrStrokeFocalX, GrStrokeFocalY), GrStrokeScale, GrStrokeSkew);
					}
					p->strokePath();
				}
				else if (lineColor() != CommonStrings::None)
				{
					p->setStrokeMode(ScPainter::Solid);
					p->setPen(strokeQColor, m_lineWidth, PLineArt, PLineEnd, PLineJoin);
					if (DashValues.count() != 0)
						p->setDash(DashValues, DashOffset);
					p->strokePath();
				}
				else
					no_stroke = true;
			}
			else
			{
				p->setStrokeMode(ScPainter::Solid);
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
			if (lineBlendmode() != 0)
				p->setBlendModeStroke(0);
		}
		else if (isAnnotation())
		{
			if (annotation().borderColor() == CommonStrings::None)
				no_stroke = true;
		}
	}
	p->setFillMode(ScPainter::Solid);
	p->setStrokeMode(ScPainter::Solid);
	p->restore();
}

void PageItem_TextFrame::DrawObj_Decoration(ScPainter *p)
{
	//#12405 if (isAnnotation() && ((annotation().Type() > 1) && (annotation().Type() < 7)) && (annotation().Bwid() > 0))
	//	return;
	p->save();
//	p->setAntialiasing(false);
	if (!isEmbedded)
		p->translate(m_xPos, m_yPos);
	p->rotate(m_rotation);
	if ((!isEmbedded) && (!m_Doc->RePos))
	{
		double scpInv = 0.0;
		if ((drawFrame()) && (m_Doc->guidesPrefs().framesShown) && (no_stroke))
		{
			p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameNormColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if ((isBookmark) || (m_isAnnotation))
				p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameAnnotationColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if ((BackBox != 0) || (NextBox != 0))
				p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameLinkColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if (m_Locked)
				p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameLockColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

			p->setFillMode(0);
			p->setupSharpPolygon(&PoLine);
			p->strokePath();
		}
		if ((m_Doc->guidesPrefs().framesShown) && textFlowUsesContourLine() && (ContourLine.size() != 0))
		{
			p->setPen(Qt::darkGray, 0, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin);
			p->setupSharpPolygon(&ContourLine);
			p->strokePath();
		}

		//Draw the overflow icon
		if (frameOverflows())
		{
			if ((!m_Doc->drawAsPreview) && (!isAnnotation()))
				drawOverflowMarker(p);
		}
		if ((m_Doc->guidesPrefs().colBordersShown) && (!m_Doc->drawAsPreview))
			drawColumnBorders(p);
		if ((m_Doc->guidesPrefs().layerMarkersShown) &&
			(m_Doc->layerCount() > 1) &&
			(!m_Doc->layerOutline(LayerID)) &&
			(!m_Doc->drawAsPreview))
		{
			p->setPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setPenOpacity(1.0);
			p->setBrush(m_Doc->layerMarker(LayerID));
			p->setBrushOpacity(1.0);
			p->setFillMode(ScPainter::Solid);
			double ofwh = 10;
			double ofx = m_width - ofwh/2;
			double ofy = m_height - ofwh*3;
			p->drawSharpRect(ofx, ofy, ofwh, ofwh);
		}
		if (no_fill && no_stroke && m_Doc->guidesPrefs().framesShown)
		{
			p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameNormColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			if (m_Locked)
				p->setPen(PrefsManager::instance()->appPrefs.displayPrefs.frameLockColor, scpInv, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->setFillMode(ScPainter::None);
			p->drawSharpRect(0, 0, m_width, m_height);
			no_fill = false;
			no_stroke = false;
		}
		//if (m_Doc->selection->findItem(this)!=-1)
		//	drawLockedMarker(p);
	}
	FrameOnly = false;
//	p->setAntialiasing(true);
	p->restore();
}


void PageItem_TextFrame::clearContents()
{
	if (itemText.length() <= 0)
		return;

	PageItem *nextItem = this;
	while (nextItem->prevInChain() != 0)
		nextItem = nextItem->prevInChain();

	ParagraphStyle defaultStyle = nextItem->itemText.defaultStyle();
	nextItem->itemText.selectAll();
	nextItem->asTextFrame()->deleteSelectedTextFromFrame();
	if (!isNoteFrame())
	{
		if (UndoManager::undoEnabled() && undoManager->getLastUndo())
			undoManager->getLastUndo()->setName(Um::ClearText);
		nextItem->itemText.setDefaultStyle(defaultStyle);
	}

	while (nextItem != 0)
	{
		nextItem->invalid = true;
		nextItem = nextItem->nextInChain();
	}
}

void PageItem_TextFrame::truncateContents()
{
	if ((this->nextInChain() == NULL) && frameOverflows())
	{
		ParagraphStyle defaultStyle = this->itemText.defaultStyle();
		int pos = itemText.cursorPosition();
		if (itemText.lengthOfSelection() == 0)
		{
			itemText.select(maxCharsInFrame(), itemText.length() - maxCharsInFrame(), true);
			deleteSelectedTextFromFrame();
		}
		itemText.setCursorPosition(pos, true);

		if (UndoManager::undoEnabled() && undoManager->getLastUndo())
			undoManager->getLastUndo()->setName(Um::TruncateText);
		this->itemText.setDefaultStyle(defaultStyle);
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
	QString uc = k->text();
#ifdef Q_OS_HAIKU
	if (kk == Qt::Key_Return)
		uc = "\r";
#endif
	int as = uc[0].unicode();
	QString Tcha, Twort;
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

	ScribusView* view = m_Doc->view();

	//<< ISO 14755
	//Check if we are trying to enter Unicode sequence mode first
	if (view->m_ScMW->actionManager->compareKeySeqToShortcut(k->key(), buttonModifiers, "specialUnicodeSequenceBegin"))
	{
		unicodeTextEditMode = true;
		unicodeInputCount = 0;
		unicodeInputString = "";
		keyRepeat = false;
		return;
	}
	//>>


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
		if (UndoManager::undoEnabled())
		{
			SimpleState *ss = dynamic_cast<SimpleState*>(undoManager->getLastUndo());
			if (ss)
				ss->set("ETEA", QString(""));
			else
			{
				TransactionState *ts = dynamic_cast<TransactionState*>(undoManager->getLastUndo());
				if (ts)
					ss = dynamic_cast<SimpleState*>(ts->last());
				if (ss)
					ss->set("ETEA", QString(""));
			}
		}
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
				UndoTransaction trans;
				if (UndoManager::undoEnabled())
					trans = undoManager->beginTransaction(Um::Selection,Um::ITextFrame,Um::InsertText,"",Um::IDelete);
				if (itemText.lengthOfSelection() > 0)
					deleteSelectedTextFromFrame();
				if (conv < 31)
					conv = 32;
				if (UndoManager::undoEnabled())
				{
					ScItemState<ParagraphStyle> *ip = 0;
					SimpleState *ss = dynamic_cast<SimpleState*>(undoManager->getLastUndo());
					UndoObject *undoTarget = this;
					int cursorPos = itemText.cursorPosition();
					if (conv == SpecialChars::PARSEP.unicode())
					{
						ip = new ScItemState<ParagraphStyle>(Um::InsertText, "", Um::ICreate);
						ip->set("INSERT_FRAMEPARA");
						ip->set("ETEA", "insert_framepara");
						ip->set("START", cursorPos);
						ip->setItem(itemText.paragraphStyle(cursorPos));
						if (isNoteFrame())
						{
							undoTarget = dynamic_cast<UndoObject*>(m_Doc);
							ip->set("noteframeName", getUName());
						}
						undoManager->action(undoTarget, ip);
					}
					else if (ss && ss->get("ETEA") == "insert_frametext")
						ss->set("TEXT_STR", ss->get("TEXT_STR") + QString(QChar(conv)));
					else {
						ss = new SimpleState(Um::InsertText,"",Um::ICreate);
						ss->set("INSERT_FRAMETEXT");
						ss->set("ETEA", QString("insert_frametext"));
						ss->set("TEXT_STR", QString(QChar(conv)));
						ss->set("START", itemText.cursorPosition());
						if (isNoteFrame())
						{
							undoTarget = dynamic_cast<UndoObject*>(m_Doc);
							ss->set("noteframeName", getUName());
						}
						undoManager->action(undoTarget, ss);
					}
				}
				itemText.insertChars(QString(QChar(conv)), true);
				if (trans)
					trans.commit();
//				Tinput = true;
				m_Doc->scMW()->setTBvals(this);
				if (isAutoNoteFrame() && m_Doc->notesChanged())
				{
					Q_ASSERT(asNoteFrame()->masterFrame());
					asNoteFrame()->masterFrame()->invalid = true;
				}
				else
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
			pos = textLayout.startOfLine(pos);
		}
		else
		{
			//Control Home for start of frame text
			pos = textLayout.startOfFrame();
		}
		itemText.setCursorPosition(pos);
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(oldPos);
//		if ( this->itemText.lengthOfSelection() > 0 )
//			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_End:
		// go to end of line
		len = lastInFrame();
		if ( itemText.cursorPosition() >= (len + 1))
			break; // at end of frame
		if ( (buttonModifiers & Qt::ControlModifier) == 0 )
		{
			itemText.setCursorPosition( textLayout.endOfLine(itemText.cursorPosition()) );
		}
		else
		{
			//Control End for end of frame text
			itemText.setCursorPosition( textLayout.endOfFrame() );
		}
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(oldPos);
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
				ExpandSel(oldPos);
		}
		else
		{
			if (itemText.cursorPosition() <= lastInFrame())
			{
				itemText.setCursorPosition( textLayout.nextLine(itemText.cursorPosition()) );
				if ( buttonModifiers & Qt::ShiftModifier )
				{
					if ( buttonModifiers & Qt::AltModifier )
						itemText.setCursorPosition (lastInFrame()+1);
					ExpandSel(oldPos);
				}
				else
					if ((textLayout.lines() > 0) && (oldPos >= textLayout.line(textLayout.lines()-1)->firstChar()) && (itemText.cursorPosition() >= lastInFrame()) && (NextBox != 0))
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
				ExpandSel(oldPos);
		}
		else
		{
			if (itemText.cursorPosition() > firstInFrame())
			{
				if (itemText.cursorPosition() > lastInFrame() || itemText.cursorPosition() >= itemText.length())
					itemText.setCursorPosition(lastInFrame());
				itemText.setCursorPosition( textLayout.prevLine(itemText.cursorPosition()) );
				if ( buttonModifiers & Qt::ShiftModifier )
				{
					if ( buttonModifiers & Qt::AltModifier )
						itemText.setCursorPosition( firstInFrame() );
					ExpandSel(oldPos);
				}
				else
					if ((textLayout.lines() > 0) && (oldPos <= textLayout.line(0)->lastChar()) && (itemText.cursorPosition()  == firstInFrame()) && (BackBox != 0))
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
			itemText.setCursorPosition( textLayout.startOfFrame() );
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(oldPos);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_PageDown:
		if (!frameDisplays(itemText.length()-1) && itemText.cursorPosition() >= lastInFrame() && NextBox != 0)
		{
			view->Deselect(true);
			itemText.setCursorPosition( NextBox->lastInFrame() );
			m_Doc->scMW()->selectItemsFromOutlines(NextBox);
			//currItem = currItem->BackBox;
		}
		else
			itemText.setCursorPosition( textLayout.endOfFrame() );
		if ( buttonModifiers & Qt::ShiftModifier )
			ExpandSel(oldPos);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_Left:
		if ( buttonModifiers & Qt::ControlModifier )
		{
			itemText.moveCursorWordLeft();
			if ( buttonModifiers & Qt::ShiftModifier )
				ExpandSel(oldPos);
		}
		else if ( buttonModifiers & Qt::ShiftModifier )
		{
			itemText.moveCursorLeft();
			ExpandSel(oldPos);
		}
		else
		{
			itemText.moveCursorLeft();
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

		while ((itemText.cursorPosition() > 1) && (itemText.flags(itemText.cursorPosition() - 1) & ScLayout_SuppressSpace))
		{
			itemText.moveCursorLeft();
			if (itemText.cursorPosition() == 0)
				break;
		}

//		if ( itemText.lengthOfSelection() > 0 )
//			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Qt::Key_Right:
		if ( buttonModifiers & Qt::ControlModifier )
		{
			itemText.moveCursorWordRight();
			if ( buttonModifiers & Qt::ShiftModifier )
				ExpandSel(oldPos);
		}
		else if ( buttonModifiers & Qt::ShiftModifier )
		{
			itemText.moveCursorRight();
			ExpandSel(oldPos);
		}
		else
		{
			itemText.moveCursorRight(); // new position within text ?
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
				deleteSelectedTextFromFrame();
				m_Doc->scMW()->setTBvals(this);
				if (isAutoNoteFrame() && asNoteFrame()->notesList().isEmpty())
				{
					if (!asNoteFrame()->isEndNotesFrame())
					{
						Q_ASSERT(asNoteFrame()->masterFrame());
						asNoteFrame()->masterFrame()->invalid = true;
					}
				}
				else
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
		if (itemText.lengthOfSelection() == 0)
		{
			int pos1 = itemText.cursorPosition();
			itemText.moveCursorForward();
			int pos2 = itemText.cursorPosition();
			itemText.select(pos1, pos2 - pos1, true);
		}
		deleteSelectedTextFromFrame();
		if (isAutoNoteFrame() && asNoteFrame()->notesList().isEmpty())
		{
			if (!asNoteFrame()->isEndNotesFrame())
			{
				Q_ASSERT(asNoteFrame()->masterFrame());
				asNoteFrame()->masterFrame()->invalid = true;
			}
		}
		else
		{
			layout();
			if (oldLast != lastInFrame() && NextBox != 0 && NextBox->invalid)
				NextBox->updateLayout();
		}
//		Tinput = false;
//		if ((cr == QChar(13)) && (itemText.length() != 0))
//		{
//			m_Doc->chAbStyle(this, findParagraphStyle(m_Doc, itemText.paragraphStyle(qMax(itemText.cursorPosition()-1,0))));
//			Tinput = false;
//		}
		m_Doc->scMW()->setTBvals(this);
//		view->RefreshItem(this);
		break;
	case Qt::Key_Backspace:
		if (itemText.cursorPosition() == 0)
		{
			if (itemText.lengthOfSelection() > 0)
			{
				deleteSelectedTextFromFrame();
				m_Doc->scMW()->setTBvals(this);
				if (isAutoNoteFrame() && asNoteFrame()->notesList().isEmpty())
				{
					if (!asNoteFrame()->isEndNotesFrame())
					{
						Q_ASSERT(asNoteFrame()->masterFrame());
						asNoteFrame()->masterFrame()->invalid = true;
					}
				}
				else
					update();
			}
			break;
		}
		if (itemText.length() == 0)
			break;
		if (itemText.lengthOfSelection() == 0)
		{
			itemText.setCursorPosition(-1, true);
			itemText.select(itemText.cursorPosition(), 1, true);
		}
		deleteSelectedTextFromFrame();
//		Tinput = false;
//		if ((cr == QChar(13)) && (itemText.length() != 0))
//		{
//			m_Doc->chAbStyle(this, findParagraphStyle(m_Doc, itemText.paragraphStyle(qMax(CPos-1,0))));
//			Tinput = false;
//		}
		if (isAutoNoteFrame() && asNoteFrame()->notesList().isEmpty())
		{
			if (!asNoteFrame()->isEndNotesFrame())
			{
				Q_ASSERT(asNoteFrame()->masterFrame());
				asNoteFrame()->masterFrame()->invalid = true;
			}
		}
		else
		{
			layout();
			if (oldLast != lastInFrame() && NextBox != 0 && NextBox->invalid)
				NextBox->updateLayout();
		}
		if (itemText.cursorPosition() < firstInFrame())
		{
			itemText.setCursorPosition( firstInFrame() );
			if (BackBox != 0)
			{
				view->Deselect(true);
				if (BackBox->invalid)
					BackBox->updateLayout();
				itemText.setCursorPosition( BackBox->lastInFrame() );
				m_Doc->scMW()->selectItemsFromOutlines(BackBox);
				//currItem = currItem->BackBox;
			}
		}
		m_Doc->scMW()->setTBvals(this);
//		update();
//		view->RefreshItem(this);
		break;
	default:
		if (isNoteFrame() && itemText.cursorPosition() == 0)
			break; //avoid inserting chars before first note mark
		bool doUpdate = false;
		UndoTransaction activeTransaction;
		if (itemText.lengthOfSelection() > 0) //(kk < 0x1000)
		{
			bool x11Hack=false;
			bool controlCharHack=false;
			if ((k->text().size() == 1))
			{
				ushort uni = k->text().at(0).unicode();
				controlCharHack = ((uni < 32) && (uni != SpecialChars::TAB.unicode()));
			}
#if defined (Q_OS_LINUX)
			if ((k->text().size()==1) && (k->modifiers() & Qt::ShiftModifier) && (k->modifiers() & Qt::ControlModifier) && (k->nativeVirtualKey() < 1000))
				x11Hack=true;
#endif
			if (!controlCharHack && !x11Hack && !k->text().isEmpty())
			{
				if (UndoManager::undoEnabled())
					activeTransaction = undoManager->beginTransaction(Um::Selection, Um::IGroup, Um::ReplaceText, "", Um::IDelete);

				deleteSelectedTextFromFrame();
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
			if (UndoManager::undoEnabled())
			{
				SimpleState *ss = dynamic_cast<SimpleState*>(undoManager->getLastUndo());
				if (ss && ss->get("ETEA") == "insert_frametext")
					ss->set("TEXT_STR",ss->get("TEXT_STR") + QString(SpecialChars::TAB));
				else
				{
					ss = new SimpleState(Um::InsertText,"",Um::ICreate);
					ss->set("INSERT_FRAMETEXT");
					ss->set("ETEA", QString("insert_frametext"));
					ss->set("TEXT_STR", QString(SpecialChars::TAB));
					ss->set("START", itemText.cursorPosition());
					UndoObject * undoTarget = this;
					if (isNoteFrame())
					{
						undoTarget = m_Doc;
						ss->set("noteframeName", getUName());
					}
					undoManager->action(undoTarget, ss);
				}
			}
			itemText.insertChars(QString(SpecialChars::TAB), true);
//			Tinput = true;
//			view->RefreshItem(this);
			doUpdate = true;
		}
		else if (uc[0] > QChar(31) || (as == 13) || (as == 30))
		{
			if (UndoManager::undoEnabled())
			{
				ScItemState<ParagraphStyle> *ip = 0;
				SimpleState *ss = dynamic_cast<SimpleState*>(undoManager->getLastUndo());
				UndoObject *undoTarget = this;
				int cursorPos = itemText.cursorPosition();
				if (uc[0] == SpecialChars::PARSEP)
				{
					ip = new ScItemState<ParagraphStyle>(Um::InsertText, "", Um::ICreate);
					ip->set("INSERT_FRAMEPARA");
					ip->set("ETEA", "insert_framepara");
					ip->set("START", cursorPos);
					ip->setItem(itemText.paragraphStyle(cursorPos));
					if (isNoteFrame())
					{
						undoTarget = dynamic_cast<UndoObject*>(m_Doc);
						ip->set("noteframeName", getUName());
					}
					undoManager->action(undoTarget, ip);
				}
				else if (ss && ss->get("ETEA") == "insert_frametext")
					ss->set("TEXT_STR", ss->get("TEXT_STR") + uc);
				else
				{
					ss = new SimpleState(Um::InsertText, "", Um::ICreate);
					ss->set("INSERT_FRAMETEXT");
					ss->set("ETEA", QString("insert_frametext"));
					ss->set("TEXT_STR", uc);
					ss->set("START", cursorPos);
					if (isNoteFrame())
					{
						undoTarget = dynamic_cast<UndoObject*>(m_Doc);
						ss->set("noteframeName", getUName());
					}
					undoManager->action(undoTarget, ss);
				}
			}
			itemText.insertChars(uc, true);
			if ((m_Doc->docHyphenator->AutoCheck) && (itemText.cursorPosition() > 1))
			{
				Twort = "";
				Tcoun = 0;
				for (int hych = itemText.cursorPosition()-1; hych > -1; hych--)
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
			doUpdate = true;
		}
		if (doUpdate)
		{
			if (activeTransaction)
			{
				activeTransaction.commit();
			}
			// update layout immediately, we need MaxChars to be correct to detect
			// if we need to move to next frame or not
			if (isAutoNoteFrame() && asNoteFrame()->notesList().isEmpty())
			{
				if (!asNoteFrame()->isEndNotesFrame())
				{
					Q_ASSERT(asNoteFrame()->masterFrame());
					asNoteFrame()->masterFrame()->invalid = true;
				}
			}
			else
				update();
			if (oldLast != lastInFrame() && NextBox != 0 && NextBox->invalid)
				NextBox->updateLayout();
		}
		//check if cursor need to jump to next linked frame
		//but not for notes frames can`t be updated as may disapper during update
		if ((itemText.cursorPosition() > lastInFrame() + 1) && (lastInFrame() < (itemText.length() - 2)) && NextBox != 0)
		{
			view->Deselect(true);
			NextBox->update();
			m_Doc->scMW()->selectItemsFromOutlines(NextBox);
		}
		break;
	}
// 	update();
//	view->slotDoCurs(true);
	if ((kk == Qt::Key_Left) || (kk == Qt::Key_Right) || (kk == Qt::Key_Up) || (kk == Qt::Key_Down))
		keyRepeat = false;
}

void PageItem_TextFrame::deleteSelectedTextFromFrame(/*bool findNotes*/)
{
	if (itemText.length() <= 0)
		return;
	if (itemText.lengthOfSelection() == 0)
	{
		itemText.select(itemText.cursorPosition(), 1);
		HasSel = true;
	}
	int start = itemText.startOfSelection();
	int stop = itemText.endOfSelection();
	int marksNum = 0;
	if (UndoManager::undoEnabled()) 
	{
		int lastPos = start;
		CharStyle lastParent = itemText.charStyle(start);
		UndoState* state = undoManager->getLastUndo();
		ScItemState<ParagraphStyle> *ip = NULL;
		ScItemState<CharStyle> *is = NULL;
		TransactionState *ts = NULL;
		bool added = false;
		bool lastIsDelete = false;
		while (state && state->isTransaction()) {
			ts = dynamic_cast<TransactionState*>(state);
			is = dynamic_cast<ScItemState<CharStyle>*>(ts->last());
			state = ts->last();
		}
		QString eteaString = (m_Doc->appMode == modeEdit) ? "delete_frametext" : "";
		UndoTransaction trans = undoManager->beginTransaction(Um::Selection,Um::IDelete,Um::Delete,"",Um::IDelete);

		//find and delete notes and marks in selected text
		QList<QPair<TextNote*, int> > notes2DEL;
		if (isNoteFrame()/* && findNotes*/)
		{
			//find and delete notes
			//if marks are in notes then they will be deleted further while note is physically deleted
			for (int i=start; i < stop; ++i)
			{
				if (i == itemText.length())
					break;
				Mark* mark = itemText.mark(i);
				if (itemText.hasMark(i) && mark->isType(MARKNoteFrameType))
					notes2DEL.append(QPair<TextNote*, int>(mark->getNotePtr(), i));
			}
		}
		else
		{
			//delete marks from selected text (with undo)
			marksNum = removeMarksFromText(true);
			stop -= marksNum;
		}
		//delete text
		for (int i=start; i <= stop; ++i)
		{
			Mark* mark = i < itemText.length() && itemText.hasMark(i) ? itemText.mark(i) : NULL;
			const CharStyle& curParent = itemText.charStyle(i);
			bool needParaAction = ((i < stop) && (itemText.text(i) == SpecialChars::PARSEP));
			if (i==stop || !curParent.equiv(lastParent) || (mark && mark->isType(MARKNoteFrameType)) || needParaAction)
			{
				added = false;
				lastIsDelete = false;
				if (is && ts && is->get("ETEA") == "delete_frametext" && lastPos < is->getInt("START"))
				{
					int oldStart = is->getInt("START");
					if (is->getItem().equiv(lastParent) && (i - lastPos > 0) && (start + i - lastPos == oldStart))
					{
						is->set("START", start);
						is->set("TEXT_STR", itemText.text(lastPos, i - lastPos) + is->get("TEXT_STR"));
						added = true;
						lastIsDelete = true;
					}
				}
				else if (is && ts && is->get("ETEA") == "delete_frametext"  && lastPos >= is->getInt("START"))
				{
					int oldStart = is->getInt("START");
					if (is->getItem().equiv(lastParent) && (i - lastPos > 0) && (oldStart == start))
					{
						is->set("TEXT_STR", is->get("TEXT_STR") + itemText.text(lastPos, i - lastPos));
						added = true;
						lastIsDelete = true;
					}
				}
				if (!added)
				{
					UndoObject * undoTarget = this;
					is = NULL;
					if (i - lastPos > 0)
					{
						is = new ScItemState<CharStyle>(Um::DeleteText, "", Um::IDelete);
						is->set("DELETE_FRAMETEXT");
						is->set("ETEA", eteaString);
						is->set("TEXT_STR", itemText.text(lastPos, i - lastPos));
						is->set("START", start);
						is->setItem(lastParent);
					}
					//delete selected notes from notes frame
					if (isNoteFrame())
					{
						undoTarget = m_Doc; //undo target is doc for notes as after deleting last note notesframe can be deleted
						if (is)
							is->set("noteframeName", getUName());
						//remove marks from notes
						for (int ii = notes2DEL.count() -1; ii >= 0; --ii)
						{
							TextNote* note = notes2DEL.at(ii).first;
							Q_ASSERT(note != NULL);
							if (note->textLen > 0)
							{
								itemText.deselectAll();
								itemText.select(notes2DEL.at(ii).second + 1, note->textLen);
								removeMarksFromText(true);
							}
						}
						asNoteFrame()->updateNotesText();
						for (int ii = notes2DEL.count() -1; ii >= 0; --ii)
						{
							TextNote* note = notes2DEL.at(ii).first;
							Q_ASSERT(note != NULL);
							m_Doc->setUndoDelNote(note);
							if (note->isEndNote())
								m_Doc->flag_updateEndNotes = true;
							m_Doc->deleteNote(note);
						}
						if (is)
						{
							if (!ts || !lastIsDelete) {
								undoManager->action(undoTarget, is);
								ts = NULL;
							}
							else
								ts->pushBack(undoTarget, is);
						}
						break;
					}
					if (is)
					{
						if (!ts || !lastIsDelete) {
							undoManager->action(undoTarget, is);
							ts = NULL;
						}
						else
							ts->pushBack(undoTarget, is);
					}
				}
				lastPos = i;
				lastParent = curParent;
			}
			if (needParaAction)
			{
				UndoObject * undoTarget = this;
				if (isNoteFrame())
					undoTarget = m_Doc;
				ip = new ScItemState<ParagraphStyle>(Um::DeleteText, "", Um::IDelete);
				ip->set("DELETE_FRAMEPARA");
				ip->set("ETEA", "delete_framepara");
				ip->set("START", start);
				ip->setItem(itemText.paragraphStyle(i));
				lastPos = i + 1;
				if (lastPos < itemText.length())
					lastParent = itemText.charStyle(lastPos);
				QString etea;
				SimpleState* ss = ts ? dynamic_cast<SimpleState*>(ts->last()) : NULL;
				if (ss)
					etea = ss->get("ETEA");
				if (ts && ((etea == "delete_frametext") || (etea == "delete_framepara")))
					ts->pushBack(undoTarget, ip);
				else
					undoManager->action(undoTarget, ip);
				is = NULL;
				ts = NULL;
			}
		}
		if (trans)
		{
			trans.commit();
		}
	}
	else //remove marks without undo
		marksNum = removeMarksFromText(false);

	start = itemText.startOfSelection();
	stop = itemText.endOfSelection();

	itemText.setCursorPosition(start);
	//for sure text is still selected
	itemText.select(start, stop - start - marksNum);
	itemText.removeSelection();
	HasSel = false;
//	m_Doc->updateFrameItems();
	m_Doc->scMW()->DisableTxEdit();
}

bool PageItem_TextFrame::checkKeyIsShortcut(QKeyEvent *k)
{
	QMap<QString, Keys> keyMap=PrefsManager::instance()->appPrefs.keyShortcutPrefs.KeyActions;

	bool ret = false;
	int keyCode =0;
	if (k->modifiers() & Qt::ShiftModifier)
		keyCode |= Qt::SHIFT;
	if (k->modifiers() & Qt::ControlModifier)
		keyCode |= Qt::CTRL;
	if (k->modifiers() & Qt::AltModifier)
		keyCode |= Qt::ALT;
	keyCode|=k->key();

	QKeySequence key = QKeySequence(keyCode);
	for (QMap<QString,Keys>::Iterator it=keyMap.begin(); it!=keyMap.end(); ++it)
	{
		if (key.matches(it.value().keySequence) != QKeySequence::NoMatch)
		{
			ret = true;
			break;
		}
	}
	return ret;

}

void PageItem_TextFrame::updateBulletsNum()
{
	for (int index = 0; index < itemText.length(); ++index)
	{
		Mark* mark = itemText.mark(index);
		if ((mark != NULL) && (itemText.hasMark(index)))
		{
			mark->OwnPage = this->OwnPage;
			//itemPtr and itemName set to this frame only if mark type is different than MARK2ItemType
			if (!mark->isType(MARK2ItemType))
			{
				mark->setItemPtr(this);
				mark->setItemName(itemName());
			}

			//anchors and indexes has no visible inserts in text
			if (mark->isType(MARKAnchorType) || mark->isType(MARKIndexType))
				continue;

			//set note marker charstyle
			if (mark->isNoteType())
			{
				TextNote* note = mark->getNotePtr();
				if (note == NULL)
					continue;
				mark->setItemPtr(this);
				NotesStyle* nStyle = note->notesStyle();
				Q_ASSERT(nStyle != NULL);
				QString chsName = nStyle->marksChStyle();
				CharStyle currStyle(itemText.charStyle(index));
				if (!chsName.isEmpty())
				{
					CharStyle marksStyle(m_Doc->charStyle(chsName));
					if (!currStyle.equiv(marksStyle))
					{
						currStyle.setParent(chsName);
						itemText.applyCharStyle(index, 1, currStyle);
					}
				}

				StyleFlag s(itemText.charStyle(index).effects());
				if (mark->isType(MARKNoteMasterType))
				{
					if (nStyle->isSuperscriptInMaster())
						s |= ScStyle_Superscript;
					else
						s &= ~ScStyle_Superscript;
				}
				else
				{
					if (nStyle->isSuperscriptInNote())
						s |= ScStyle_Superscript;
					else
						s &= ~ScStyle_Superscript;
				}
				if (s != itemText.charStyle(index).effects())
				{
					CharStyle haveSuperscript;
					haveSuperscript.setFeatures(s.featureList());
					itemText.applyCharStyle(index, 1, haveSuperscript);
				}
			}
		}

		bool bullet = false;
		if (index == 0 || itemText.text(index - 1) == SpecialChars::PARSEP)
		{
			const ParagraphStyle& style = itemText.paragraphStyle(index);
			if (style.hasBullet() || style.hasNum())
			{
				bullet = true;
				if (mark == NULL || !mark->isType(MARKBullNumType))
				{
					itemText.insertMark(new BulNumMark(), index);
					index--;
					continue;
				}
				if (style.hasBullet())
					mark->setString(style.bulletStr());
				else if (style.hasNum() && mark->getString().isEmpty())
				{
					mark->setString("?");
				}
			}
		}

		if (!bullet && mark && mark->isType(MARKBullNumType))
		{
			itemText.removeChars(index, 1);
			index--;
			continue;
		}

	}
	m_Doc->updateNumbers();

}


// jjsa added on 15-mar-2004 expand / decrease selection

// jjsa added on 14-mar-2004 text selection with pressed
// shift button and <-, -> cursor keys
// Parameters
//   PageItem *currItem text item to be processed
//   inc < 0 for left key > 0 for right key
//  if value is +/-1 work on slection
//  if value is +/-2 refresh if text under cursor is selected -- not used

void PageItem_TextFrame::ExpandSel(int oldPos)
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
}

double PageItem_TextFrame::columnWidth()
{
	double lineCorr;
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth / 2.0;
	else
		lineCorr = 0;
	return (m_width - (ColGap * (Cols - 1)) - m_textDistanceMargins.left() - m_textDistanceMargins.right() - 2 * lineCorr) / Cols;
//	return (Width - (ColGap * (Cols - 1)) - m_textDistanceMargins.left() - m_textDistanceMargins.right() - lineCorr) / Cols;
}

/*
void PageItem_TextFrame::drawOverflowMarker(ScPainter *p)
{
	qreal sideLength = 10 / qMax(p->zoomFactor(), 1.0);
	qreal left  = Width  - sideLength;// / 2;
	qreal right = left   + sideLength;
	qreal top   = Height - sideLength;// * 1.5;
	qreal bottom = top   + sideLength;

	QColor color(PrefsManager::instance()->appPrefs.displayPrefs.frameNormColor);
	if ((isBookmark) || (m_isAnnotation))
		color = PrefsManager::instance()->appPrefs.displayPrefs.frameAnnotationColor;
	if ((BackBox != 0) || (NextBox != 0))
		color = PrefsManager::instance()->appPrefs.displayPrefs.frameLinkColor;
	if (m_Locked)
		color = PrefsManager::instance()->appPrefs.displayPrefs.frameLockColor;
	if (m_Doc->m_Selection->containsItem(this))
		color = Qt::red;

	p->setPen(color, 0.5 / p->zoomFactor(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setPenOpacity(1.0);
	p->setBrush(Qt::white);
	p->setBrushOpacity(1.0);
	p->setFillMode(ScPainter::Solid);
	p->drawRect(left, top, sideLength, sideLength);
	p->drawLine(FPoint(left, top), FPoint(right, bottom));
	p->drawLine(FPoint(left, bottom), FPoint(right, top));
}
*/

void PageItem_TextFrame::drawColumnBorders(ScPainter *p)
{
	p->setPen(Qt::gray, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
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
	if (m_textDistanceMargins.top() + lineCorr!=0.0)
		p->drawSharpLine(FPoint(m_textDistanceMargins.left() + lineCorr, m_textDistanceMargins.top() + lineCorr), FPoint(m_width - m_textDistanceMargins.right() - lineCorr, m_textDistanceMargins.top() + lineCorr));
	if (m_textDistanceMargins.bottom() + lineCorr!=0.0)
		p->drawSharpLine(FPoint(m_textDistanceMargins.left() + lineCorr, m_height - m_textDistanceMargins.bottom() - lineCorr), FPoint(m_width - m_textDistanceMargins.right() - lineCorr, m_height - m_textDistanceMargins.bottom() - lineCorr));
	while(curCol < Cols)
	{
		colLeft=(colWidth + ColGap) * curCol + m_textDistanceMargins.left() + lineCorr;
		if (colLeft != 0.0)
			p->drawSharpLine(FPoint(colLeft, m_textDistanceMargins.top() + lineCorr), FPoint(colLeft, m_height - m_textDistanceMargins.bottom() - lineCorr));
		if (colLeft + colWidth != m_width)
			p->drawSharpLine(FPoint(colLeft + colWidth, m_textDistanceMargins.top() + lineCorr), FPoint(colLeft + colWidth, m_height - m_textDistanceMargins.bottom() - lineCorr));
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
	linesT->setText(QString::number(textLayout.lines()));
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

void PageItem_TextFrame::toggleEditModeActions()
{
	bool editMode = (m_Doc->appMode == modeEdit);
	bool masterMode = m_Doc->masterPageMode();
	m_Doc->scMW()->scrActions["insertMarkVariableText"]->setEnabled(editMode);
	m_Doc->scMW()->scrActions["insertMarkAnchor"]->setEnabled(editMode && !masterMode);
	m_Doc->scMW()->scrActions["insertMarkItem"]->setEnabled(editMode && !masterMode);
	m_Doc->scMW()->scrActions["insertMark2Mark"]->setEnabled(editMode && !masterMode);
	m_Doc->scMW()->scrActions["insertMarkNote"]->setEnabled(editMode && !masterMode && !isNoteFrame());
	//	scrActions["insertMarkIndex"]->setEnabled(editMode && !masterMode);
	bool enableEditMark = false;
	if (editMode && (itemText.cursorPosition() < itemText.length()))
	{
		if (itemText.hasMark(itemText.cursorPosition()))
		{
			Mark* mrk = itemText.mark(itemText.cursorPosition());
			//notes marks in note frames are not editable
			if (!mrk->isType(MARKNoteFrameType))
				enableEditMark = true;
		}
	}
	m_Doc->scMW()->scrActions["editMark"]->setEnabled(enableEditMark);
}

void PageItem_TextFrame::applicableActions(QStringList & actionList)
{
	actionList << "insertMarkVariableText";
	if (!m_Doc->masterPageMode())
		actionList << "insertMarkAnchor";
	//notes frames are not simply text frames
	if (isNoteFrame())
		return;
	actionList << "fileImportText";
	actionList << "fileImportAppendText";
	actionList << "toolsEditWithStoryEditor";
	actionList << "insertSampleText";
	actionList << "itemPDFIsAnnotation";
	if (doc()->currentPage()->pageName().isEmpty())
		actionList << "itemPDFIsBookmark";
	if (isAnnotation())
	{
		if ((annotation().Type() == 0) || (annotation().Type() == 1) || ((annotation().Type() > Annotation::Listbox) && (annotation().Type() < Annotation::RadioButton)))
			actionList << "itemPDFAnnotationProps";
		else
			actionList << "itemPDFFieldProps";
	}
	if ((prevInChain() == 0) && (nextInChain() == 0))
	{
		actionList << "itemConvertToImageFrame";
		actionList << "itemConvertToPolygon";
	}
	actionList << "itemConvertToOutlines";
	if (textLayout.lines() != 0)
	{
		actionList << "editClearContents";
		if ((this->nextInChain() == NULL) && frameOverflows())
			actionList << "editTruncateContents";
		actionList << "itemAdjustFrameHeightToText";
	}
}

void PageItem_TextFrame::drawNoteIcon(ScPainter *p)
{
	p->save();
	p->translate(0, 24);
	p->scale(1, -1);
	p->setFillMode(ScPainter::None);
	p->setStrokeMode(ScPainter::Solid);
	FPointArray chArr;
	if (annotation().Icon() == Annotation::Icon_Note)
	{
		chArr.svgInit();
		chArr.svgMoveTo(9, 18);
		chArr.svgLineTo(4, 18);
		chArr.svgCurveToCubic(4, 7, 4, 4, 6, 3);
		chArr.svgLineTo(20, 3);
		chArr.svgCurveToCubic(18, 4, 18, 7, 18, 18);
		chArr.svgLineTo(17, 18);
		p->setupPolygon(&chArr, false);
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->strokePath();
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 1.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawLine(QPointF(10, 16), QPointF(14, 21));
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 1.85625, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.resize(0);
		chArr.svgInit();
		chArr.svgMoveTo(15.07, 20.523);
		chArr.svgCurveToCubic(15.07, 19.672, 14.379, 18.977, 13.523, 18.977);
		chArr.svgCurveToCubic(12.672, 18.977, 11.977, 19.672, 11.977, 20.523);
		chArr.svgCurveToCubic(11.977, 21.379, 12.672, 22.07, 13.523, 22.07);
		chArr.svgCurveToCubic(14.379, 22.07, 15.07, 21.379, 15.07, 20.523);
		chArr.svgClosePath();
		p->setupPolygon(&chArr);
		p->strokePath();
		p->drawLine(QPointF(6.5, 13.5), QPointF(15.5, 13.5));
		p->drawLine(QPointF(6.5, 10.5), QPointF(13.5, 10.5));
		p->drawLine(QPointF(6.801, 7.5), QPointF(15.5, 7.5));
		chArr.resize(0);
		chArr.svgInit();
		chArr.svgMoveTo(9, 19);
		chArr.svgLineTo(4, 19);
		chArr.svgCurveToCubic(4, 8, 4, 5, 6, 4);
		chArr.svgLineTo(20, 4);
		chArr.svgCurveToCubic(18, 5, 18, 8, 18, 19);
		chArr.svgLineTo(17, 19);
		p->setupPolygon(&chArr, false);
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->strokePath();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 1.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawLine(QPointF(10, 17), QPointF(14, 22));
		chArr.resize(0);
		chArr.svgInit();
		chArr.svgMoveTo(15.07, 21.523);
		chArr.svgCurveToCubic(15.07, 20.672, 14.379, 19.977, 13.523, 19.977);
		chArr.svgCurveToCubic(12.672, 19.977, 11.977, 20.672, 11.977, 21.523);
		chArr.svgCurveToCubic(11.977, 22.379, 12.672, 23.07, 13.523, 23.07);
		chArr.svgCurveToCubic(14.379, 23.07, 15.07, 22.379, 15.07, 21.523);
		chArr.svgClosePath();
		p->setupPolygon(&chArr);
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 1.85625, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->strokePath();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawLine(QPointF(6.5, 14.5), QPointF(15.5, 14.5));
		p->drawLine(QPointF(6.5, 11.5), QPointF(13.5, 11.5));
		p->drawLine(QPointF(6.801, 8.5), QPointF(15.5, 8.5));
	}
	else if (annotation().Icon() == Annotation::Icon_Comment)
	{
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.svgInit();
		chArr.parseSVG("M 8 20 L 16 20 C 18.363 20 20 18.215 20 16 L 20 13 C 20 10.785 18.363 9 16 9 L 13 9 L 8 3 L 8 9 C 5.637 9 4 10.785 4 13 L 4 16 C 4 18.215 5.637 20 8 20 Z");
		p->setupPolygon(&chArr);
		p->strokePath();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.resize(0);
		chArr.svgInit();
		chArr.parseSVG("M 8 21 L 16 21 C 18.363 21 20 19.215 20 17 L 20 14 C 20 11.785 18.363 10 16 10 L 13 10 L 8 4 L 8 10 L 8 10 C 5.637 10 4 11.785 4 14 L 4 17 C 4 19.215 5.637 21 8 21 Z");
		p->setupPolygon(&chArr);
		p->strokePath();
	}
	else if (annotation().Icon() == Annotation::Icon_Key)
	{
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.svgInit();
		QString svg = "M 11.895 18.754 C 13.926 20.625 17.09 20.496 18.961 18.465 C 20.832 16.434 20.699 13.27 18.668 11.398 C 17.164 10.016 15.043 9.746 13.281 10.516";
		svg += " L 12.473 9.324 L 11.281 10.078 L 9.547 8.664 L 9.008 6.496 L 7.059 6.059 L 6.34 4.121 L 5.543 3.668 L 3.375 4.207 L 2.938 6.156";
		svg += " L 10.57 13.457 C 9.949 15.277 10.391 17.367 11.895 18.754 Z";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 1.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.resize(0);
		chArr.svgInit();
		svg = "M 16.059 15.586 C 16.523 15.078 17.316 15.043 17.824 15.512 C 18.332 15.98 18.363 16.77 17.895 17.277 C 17.43 17.785 16.637 17.816 16.129 17.352";
		svg += " C 15.621 16.883 15.59 16.094 16.059 15.586 Z";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.resize(0);
		chArr.svgInit();
		svg = "M 11.895 19.754 C 13.926 21.625 17.09 21.496 18.961 19.465 C 20.832 17.434 20.699 14.27 18.668 12.398 C 17.164 11.016 15.043 10.746 13.281 11.516";
		svg += " L 12.473 10.324 L 11.281 11.078 L 9.547 9.664 L 9.008 7.496 L 7.059 7.059 L 6.34 5.121 L 5.543 4.668 L 3.375 5.207 L 2.938 7.156";
		svg += " L 10.57 14.457 C 9.949 16.277 10.391 18.367 11.895 19.754 Z";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 1.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.resize(0);
		chArr.svgInit();
		svg = "M 16.059 16.586 C 16.523 16.078 17.316 16.043 17.824 16.512 C 18.332 16.98 18.363 17.77 17.895 18.277";
		svg += " C 17.43 18.785 16.637 18.816 16.129 18.352 C 15.621 17.883 15.59 17.094 16.059 16.586 Z";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
	}
	else if (annotation().Icon() == Annotation::Icon_Help)
	{
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.svgInit();
		QString svg = "M 8.289 16.488 C 8.824 17.828 10.043 18.773 11.473 18.965 C 12.902 19.156 14.328 18.559 15.195 17.406 C 16.062 16.254 16.242 14.723 15.664 13.398";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		svg = "M 12 8 C 12 12 16 11 16 15";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 1.539286, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->save();
		p->translate(0, 24);
		p->scale(1, -1);
		svg = "M 12.684 20.891 C 12.473 21.258 12.004 21.395 11.629 21.196 C 11.254 20.992 11.105 20.531 11.297 20.149 C 11.488 19.77 11.945 19.61 12.332 19.789";
		svg += " C 12.719 19.969 12.891 20.426 12.719 20.817";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		p->restore();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 2.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.resize(0);
		chArr.svgInit();
		svg = "M 8.289 17.488 C 9.109 19.539 11.438 20.535 13.488 19.711 C 15.539 18.891 16.535 16.562 15.711 14.512 C 15.699 14.473 15.684 14.438 15.664 14.398";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		svg = "M 12 9 C 12 13 16 12 16 16";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 1.539286, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->save();
		p->translate(0, 24);
		p->scale(1, -1);
		svg = "M 12.684 19.891 C 12.473 20.258 12.004 20.395 11.629 20.195 C 11.254 19.992 11.105 19.531 11.297 19.149 C 11.488 18.77 11.945 18.61 12.332 18.789";
		svg += " C 12.719 18.969 12.891 19.426 12.719 19.817";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		p->restore();
	}
	else if (annotation().Icon() == Annotation::Icon_NewParagraph)
	{
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 4, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.svgInit();
		p->save();
		p->translate(0, 24);
		p->scale(1, -1);
		QString svg = "M 9.211 11.988 C 8.449 12.07 7.711 11.707 7.305 11.059 C 6.898 10.41 6.898 9.59 7.305 8.941 C 7.711 8.293 8.449 7.93 9.211 8.012";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 1.004413, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		svg = "M 18.07 11.511 L 15.113 10.014 L 12.199 11.602 L 12.711 8.323 L 10.301 6.045 L 13.574 5.517 L 14.996 2.522 L 16.512 5.474 L 19.801 5.899 L 17.461 8.252 L 18.07 11.511 Z";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
		p->restore();
		chArr.resize(0);
		chArr.svgInit();
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		svg = "M 11 17 L 10 17 L 10 3";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		svg = "M 14 3 L 14 13";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 4, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->save();
		p->translate(0, 24);
		p->scale(1, -1);
		svg = "M 9.211 10.988 C 8.109 11.105 7.125 10.309 7.012 9.211 C 6.895 8.109 7.691 7.125 8.789 7.012 C 8.93 6.996 9.07 6.996 9.211 7.012";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 1.004413, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		svg = "M 18.07 10.502 L 15.113 9.005 L 12.199 10.593 L 12.711 7.314 L 10.301 5.036 L 13.574 4.508 L 14.996 1.513 L 16.512 4.465 L 19.801 4.891 L 17.461 7.243 L 18.07 10.502 Z";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
		p->restore();
		chArr.resize(0);
		chArr.svgInit();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		svg = "M 11 18 L 10 18 L 10 4";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		svg = "M 14 4 L 14 14";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
	}
	else if (annotation().Icon() == Annotation::Icon_Paragraph)
	{
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.svgInit();
		QString svg = "M 15 3 L 15 18 L 11 18 L 11 3";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 4, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->save();
		p->translate(0, 24);
		p->scale(1, -1);
		svg = "M 9.777 10.988 C 8.746 10.871 7.973 9.988 8 8.949 C 8.027 7.91 8.844 7.066 9.879 7.004";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
		p->restore();
		chArr.resize(0);
		chArr.svgInit();
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		svg = "M 15 4 L 15 19 L 11 19 L 11 4";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		p->save();
		p->translate(0, 24);
		p->scale(1, -1);
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 4, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		svg = "M 9.777 9.988 C 8.746 9.871 7.973 8.988 8 7.949 C 8.027 6.91 8.844 6.066 9.879 6.004";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
		p->restore();
	}
	else if (annotation().Icon() == Annotation::Icon_Insert)
	{
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawLine(QPointF(9, 10), QPointF(17, 10));
		p->drawLine(QPointF(12, 14.012), QPointF(20, 14));
		p->drawLine(QPointF(12, 6.012), QPointF(20, 6.012));
		chArr.svgInit();
		QString svg = "M 4 12 L 6 10 L 4 8";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		chArr.resize(0);
		chArr.svgInit();
		p->drawLine(QPointF(4, 12), QPointF(4, 8));
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawLine(QPointF(12, 19.012), QPointF(20, 19));
		p->drawLine(QPointF(12, 15.012), QPointF(20, 15));
		p->drawLine(QPointF(12, 7.012), QPointF(20, 7.012));
		svg = "M 4 13 L 6 11 L 4 9";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr, false);
		p->strokePath();
		p->drawLine(QPointF(4, 13), QPointF(4, 9));
	}
	else if (annotation().Icon() == Annotation::Icon_Cross)
	{
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 2.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawLine(QPointF(18, 5), QPointF(6, 17));
		p->drawLine(QPointF(6, 5), QPointF(18, 17));
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 2.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawLine(QPointF(18, 6), QPointF(6, 18));
		p->drawLine(QPointF(6, 6), QPointF(18, 18));
	}
	else if (annotation().Icon() == Annotation::Icon_Circle)
	{
		p->setPen(QColor(qRound(255 * 0.533333), qRound(255 * 0.541176), qRound(255 * 0.521569)), 2.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.svgInit();
		QString svg = "M 19.5 11.5 C 19.5 7.359 16.141 4 12 4 C 7.859 4 4.5 7.359 4.5 11.5 C 4.5 15.641 7.859 19 12 19 C 16.141 19 19.5 15.641 19.5 11.5 Z";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
		chArr.resize(0);
		p->setPen(QColor(qRound(255 * 0.729412), qRound(255 * 0.741176), qRound(255 * 0.713725)), 2.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		chArr.svgInit();
		svg = "M 19.5 12.5 C 19.5 8.359 16.141 5 12 5 C 7.859 5 4.5 8.359 4.5 12.5 C 4.5 16.641 7.859 20 12 20 C 16.141 20 19.5 16.641 19.5 12.5 Z";
		chArr.parseSVG(svg);
		p->setupPolygon(&chArr);
		p->strokePath();
	}
	p->restore();
}


void PageItem_TextFrame::setTextAnnotationOpen(bool open)
{
	if (annotation().Type() == Annotation::Text)
	{
		if (open)
		{
			m_origAnnotPos = QRectF(xPos(), yPos(), width(), height());
			setWidthHeight(265, 265, true);
		}
		else
		{
			setXYPos(m_origAnnotPos.x(), m_origAnnotPos.y(), true);
			setWidthHeight(m_origAnnotPos.width(), m_origAnnotPos.height(), true);
		}
	}
}

QString PageItem_TextFrame::infoDescription()
{
	return QString();
}

bool PageItem_TextFrame::hasNoteMark(NotesStyle *NS)
{
	if (isNoteFrame())
		return (asNoteFrame()->notesStyle() == NS);

	if (NS == NULL)
	{
		//find any mark
		if (!m_notesFramesMap.isEmpty())
			return true;
		for (int i=firstInFrame(); i <= lastInFrame(); ++i)
			if (itemText.hasMark(i))
				return true;
	}
	else
	{
		for (int pos = firstInFrame(); pos <= lastInFrame(); ++pos)
		{
			if (itemText.hasMark(pos))
			{
				TextNote* note = itemText.mark(pos)->getNotePtr();
				if (note != NULL && (note->notesStyle() == NS))
					return true;
			}
		}
	}
	return false;
}

bool PageItem_TextFrame::hasNoteFrame(NotesStyle *NS, bool inChain)
{
	if (isNoteFrame())
		return false;
	if (m_notesFramesMap.isEmpty())
		return false;
	if (NS == NULL)
	{ //check if any notes are in frame or whole chain
		if (!inChain)
			return !m_notesFramesMap.isEmpty();
		else
		{
			PageItem* item = this;
			item = firstInChain();
			while (item != NULL)
			{
				if (item->asTextFrame()->hasNoteFrame(NULL, false))
					return true;
				item = item->nextInChain();
			}
			return false;
		}
	}
	PageItem* item = this;
	if (inChain)
		item = firstInChain();
	while (item != NULL)
	{
		NotesInFrameMap::iterator it = m_notesFramesMap.begin();
		NotesInFrameMap::iterator end = m_notesFramesMap.end();
		while (it != end)
		{
			if (it.key()->notesStyle() == NS)
				return true;
			++it;
		}
		if (!inChain)
			break;
		item = item->nextInChain();
	}
	return false;
}

void PageItem_TextFrame::delAllNoteFrames(bool doUpdate)
{
	int oldItemsCount = m_Doc->Items->count();

	QList<PageItem_NoteFrame*> delList;
	foreach (PageItem_NoteFrame* nF, m_notesFramesMap.keys())
	{
		if (nF->notesList().isEmpty() && !nF->isAutoNoteFrame())
			delList.append(nF);
	}
	while (!delList.isEmpty())
	{
		PageItem_NoteFrame* nF = delList.takeFirst();
		m_Doc->delNoteFrame(nF);
	}

	//check if doc need update
	if (doUpdate && (oldItemsCount != m_Doc->Items->count()))
	{
		m_Doc->changed();
		m_Doc->regionsChanged()->update(QRectF());
	}
	m_Doc->setNotesChanged(true);
}

Mark* PageItem_TextFrame::selectedMark(bool onlySelection)
{ //return pointer to first mark in selected (or whole) text

	bool omitNotes = true; //do not return notes marks (for searching notes use selectedNotesMark()
	int start = 0;
	int stop = 0;
	if (onlySelection)
	{
		if (itemText.lengthOfSelection() > 0)
		{
			//only selection
			start = itemText.startOfSelection();
			stop = start + itemText.lengthOfSelection();
		}
		else //in edit mode only one char in cursor position
		{
			if (m_Doc->appMode == modeEdit)
			{
				//only char after cursor
				start = itemText.cursorPosition();
				stop = start + 1;
				if (stop > itemText.length())
					return NULL;
			}
			else
			{
				//only frame
				start = firstInFrame();
				stop = lastInFrame();
				if (start == stop)
					return NULL;
			}
		}

	}
	else //in whole text
		stop = itemText.length();

	for (int pos = start; pos < stop; ++pos)
	{
		if (itemText.hasMark(pos))
		{
			Mark* mark = itemText.mark(pos);
			if (omitNotes && (mark->isType(MARKNoteMasterType) || mark->isType(MARKNoteFrameType)))
				continue;
			if (mark->isType(MARKBullNumType))
				continue;
			return mark;
		}
	}
	return NULL;
}

TextNote* PageItem_TextFrame::selectedNoteMark(int &foundPos, bool onlySelection)
{
	//return pointer to note from first mark found in text
	int start = 0;
	int stop = itemText.length();
	if (onlySelection)
	{
		if (itemText.lengthOfSelection() > 0)
		{
			start = itemText.startOfSelection();
			stop = start + itemText.lengthOfSelection();
		}
		else
			return NULL;
	}
	MarkType typ = isNoteFrame()? MARKNoteFrameType : MARKNoteMasterType;
	for (int pos = start; pos < stop; ++pos)
	{
		if (itemText.hasMark(pos) && itemText.mark(pos)->isType(typ))
		{
			foundPos = pos;
			return itemText.mark(pos)->getNotePtr();
		}
	}
	return NULL;
}

TextNote* PageItem_TextFrame::selectedNoteMark(bool onlySelection)
{
	int dummy;
	return selectedNoteMark(dummy, onlySelection);
}

NotesInFrameMap PageItem_TextFrame::updateNotesFrames(QMap<int, Mark*> noteMarksPosMap)
{
	NotesInFrameMap notesMap; // = m_notesFramesMap;
	QMap<int, Mark*>::Iterator it = noteMarksPosMap.begin();
	QMap<int, Mark*>::Iterator end = noteMarksPosMap.end();
	PageItem* lastItem = this;
	while (it != end)
	{
		if (it.key() <= lastInFrame())
		{
			Mark* mark = it.value();
			mark->setItemPtr(this);
			mark->setItemName(itemName());

			TextNote* note = mark->getNotePtr();
			Q_ASSERT(note != NULL);
			if (note == NULL)
			{
				qWarning() << "note mark without valid note pointer";
				note = m_Doc->newNote(m_Doc->m_docNotesStylesList.at(0));
				note->setMasterMark(mark);
				mark->setNotePtr(note);
			}
			NotesStyle* NS = note->notesStyle();
			PageItem_NoteFrame* nF = NULL;
			if (NS->isEndNotes())
				nF = m_Doc->endNoteFrame(NS, this);
			else
				nF = itemNoteFrame(NS);
			if (nF == NULL)
			{
				//creating new noteframe
				if (NS->isEndNotes())
				{
					//create new endnotes frame
					double x,y,w,h;
					const ScPage* scP = m_Doc->page4EndNotes(NS, this);
					x = scP->Margins.left() + m_Doc->rulerXoffset + scP->xOffset();
					y = scP->Margins.top() + m_Doc->rulerYoffset + scP->yOffset();
					w = scP->width() - scP->Margins.left() - scP->Margins.right();
					h = calculateLineSpacing(itemText.defaultStyle(), this);
					nF = m_Doc->createNoteFrame(note->notesStyle(), x, y, w, h, m_Doc->itemToolPrefs().shapeLineWidth, CommonStrings::None, m_Doc->itemToolPrefs().textFont);
					switch (NS->range())
					{ //insert pointer to endnoteframe into m_Doc->m_endNotesFramesMap
						case NSRdocument:
							m_Doc->setEndNoteFrame(nF, (void*) NULL);
							break;
						case NSRsection:
							m_Doc->setEndNoteFrame(nF, m_Doc->getSectionKeyForPageIndex(OwnPage));
							break;
						case NSRstory:
							m_Doc->setEndNoteFrame(nF, (void*) firstInChain());
							break;
						case NSRpage:
							m_Doc->setEndNoteFrame(nF, (void*) m_Doc->DocPages.at(OwnPage));
							break;
						case NSRframe:
							qDebug() << "Frame range is prohibited for end-notes";
							Q_ASSERT(false);
							break;
					}
				}
				else
					//create new footnotes frame for that text frame
					nF = m_Doc->createNoteFrame(this, note->notesStyle(), m_Doc->DocItems.indexOf(lastItem));
				//insert in map noteframe with empty list of notes
				m_notesFramesMap.insert(nF, QList<TextNote*>());
				m_Doc->setNotesChanged(true);
			}
			else if (NS->isEndNotes())
			{//check endnotes frame proper page
				const ScPage* scP = m_Doc->page4EndNotes(NS, this);
				if (scP->pageNr() != nF->OwnPage)
				{
					double x,y;
					x = scP->Margins.left() + m_Doc->rulerXoffset + scP->xOffset();
					y = scP->Margins.top() + m_Doc->rulerYoffset + scP->yOffset();
					if ((scP->pageNr() != nF->OwnPage) || (nF->xPos() > (x + scP->width())) || nF->yPos() > (y + scP->height()))
					{
						undoManager->setUndoEnabled(false);
						nF->setXYPos(x,y);
						undoManager->setUndoEnabled(true);
					}
				}
			}
			QList<TextNote*> nList;//list of notes in current noteFrame
			nList = notesMap.value(nF);
			if (!nList.contains(note))
			{
				nList.append(note);
				notesMap.insert(nF, nList);
			}
			if (!nF->isEndNotesFrame())
				lastItem = nF;
		}
		else
			break;
		++it;
	}
	return notesMap;
}

void PageItem_TextFrame::updateNotesMarks(NotesInFrameMap notesMap)
{
	bool docWasChanged = false;

//	QList<PageItem_NoteFrame*> curr_footNotesList;
//	QList<PageItem_NoteFrame*> old_footNotesList;
//	QList<PageItem_NoteFrame*> curr_endNotesList;
//	QList<PageItem_NoteFrame*> old_endNotesList;


//	foreach(PageItem_NoteFrame* nF, notesMap.keys())
//	{
//		if (nF->isEndNotesFrame())
//			curr_endNotesList.append(nF);
//		else if (!notesMap.value(nF).isEmpty())
//			curr_footNotesList.append(nF);
//	}
//	foreach(PageItem_NoteFrame* nF, m_notesFramesMap.keys())
//	{
//		if (nF->isEndNotesFrame())
//			old_endNotesList.append(nF);
//		else
//			old_footNotesList.append(nF);
//	}
//	//check for endnotes marks change in current frame
//	foreach (PageItem_NoteFrame* nF, old_endNotesList)
//	{
//		if (nF->deleteIt)
//		{
//			m_Doc->delNoteFrame(nF,true);
//			docWasChanged = true;
//		}
//		else if (!notesMap.contains(nF) || (m_notesFramesMap.value(nF) != notesMap.value(nF)))
//		{
//			m_Doc->endNoteFrameChanged(nF);
//			docWasChanged = true;
//		}
//	}
	//check if some notes frames are not used anymore
	foreach (PageItem_NoteFrame* nF, m_notesFramesMap.keys())
	{
		if (nF->deleteIt || (nF->isAutoNoteFrame() && !notesMap.keys().contains(nF)))
		{
			m_Doc->delNoteFrame(nF, true);
			m_notesFramesMap.remove(nF);
			notesMap.remove(nF);
			docWasChanged = true;
		}
		else
		{
			QList<TextNote*> nList = notesMap.value(nF);
			if (nList != nF->notesList() || m_Doc->notesChanged())
			{
				nF->updateNotes(nList, (!nF->isEndNotesFrame() && !nF->notesList().isEmpty()));
				docWasChanged = true;
			}
		}
	}
	if (m_notesFramesMap != notesMap)
	{
		docWasChanged = true;
		foreach (PageItem_NoteFrame* nF, m_notesFramesMap.keys())
		{
			if (notesMap.contains(nF))
			{
				m_notesFramesMap.insert(nF, notesMap.value(nF));
				notesMap.remove(nF);
			}
			else if (nF->isAutoNoteFrame() || nF->isEndNotesFrame())
				m_notesFramesMap.remove(nF);
		}
		m_notesFramesMap.unite(notesMap);
	}
	if (docWasChanged)
	{
		m_Doc->flag_restartMarksRenumbering = true;
		m_Doc->setNotesChanged(true);
	}
}

void PageItem_TextFrame::notesFramesLayout()
{
	foreach (PageItem_NoteFrame* nF, m_notesFramesMap.keys())
	{
		if (nF == NULL)
			continue;
		if (nF->deleteIt)
			continue;
		if (nF->isEndNotesFrame() && m_Doc->flag_updateEndNotes)
			m_Doc->updateEndNotesFrameContent(nF);
		nF->invalid = true;
		nF->layout();
	}
}

int PageItem_TextFrame::removeMarksFromText(bool doUndo)
{
	int num = 0;
	if (!isNoteFrame())
	{
		TextNote* note = selectedNoteMark(true);
		while (note != NULL)
		{
			if (doUndo && UndoManager::undoEnabled())
				m_Doc->setUndoDelNote(note);
			if (note->isEndNote())
				m_Doc->flag_updateEndNotes = true;
			m_Doc->deleteNote(note);
			note = selectedNoteMark(true);
			++num;
		}
	}

	Mark* mrk = selectedMark(true);
	while (mrk != NULL)
	{
		if (!mrk->isType(MARKBullNumType))
		{
			if (doUndo)
				m_Doc->setUndoDelMark(mrk);
			m_Doc->eraseMark(mrk, true, this);
			++num;
		}
		mrk = selectedMark(true);
	}
	if (num > 0)
	{
		m_Doc->changed();
		m_Doc->regionsChanged()->update(QRectF());
		m_Doc->scMW()->marksManager->updateListView();
	}
	return num;
}

PageItem_NoteFrame *PageItem_TextFrame::itemNoteFrame(NotesStyle *nStyle)
{
	foreach (PageItem_NoteFrame* nF, m_notesFramesMap.keys())
		if (nF->notesStyle() == nStyle)
			return nF;
	return NULL;
}

void PageItem_TextFrame::setNoteFrame(PageItem_NoteFrame *nF)
{
	 m_notesFramesMap.insert(nF, nF->notesList());
}

void PageItem_TextFrame::setMaxY(double y)
{
	if (y == -1)
		maxY = 0.0;
	else
		maxY = qMax(y, maxY);
}

void PageItem_TextFrame::setTextFrameHeight()
{
	if (textLayout.lines() <= 0)
		return;

	if (NextBox == 0) // Vertical alignment is not used inside a text chain
	{
		textLayout.box()->moveTo(textLayout.box()->x(), 0);
		double newHeight = textLayout.box()->naturalHeight();
		newHeight += m_textDistanceMargins.bottom();

		UndoTransaction undoTransaction;
		if (UndoManager::undoEnabled())
		{
			QString unitSuffix = unitGetStrFromIndex(m_Doc->unitIndex());
			int unitPrecision  = unitGetPrecisionFromIndex(m_Doc->unitIndex());
			double unitRatio   = m_Doc->unitRatio();
			QString owString  = QString::number(oldWidth * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
			QString ohString  = QString::number(oldHeight * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
			QString nwString  = QString::number(m_width * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
			QString nhString  = QString::number(m_height * unitRatio, 'f', unitPrecision) + " " + unitSuffix;
			QString tooltip   = QString(Um::ResizeFromTo).arg(owString, ohString, nwString, nhString);
			undoTransaction = undoManager->beginTransaction(Um::Selection, Um::ITextFrame, Um::Resize, tooltip, Um::IResize);
		}

		setHeight(newHeight);

		if (undoTransaction)
			undoTransaction.commit();
	}

	updateClip();
	invalid = true;
	m_Doc->changed();
	m_Doc->regionsChanged()->update(QRect());
}
