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
#include "pageitem_textframe.moc"
#include <qpainter.h>
#include <qpen.h>
#include <qfont.h>
#include <qregion.h>
#include <qpoint.h>
#include <qfileinfo.h>
#include <qdrawutil.h>
#include <qbitmap.h>
#include <qregexp.h>
#include <qmessagebox.h>
#include <cmath>
#include <cassert>

#include "hyphenator.h"
#include "mpalette.h"
#include "page.h"
#include "pageitem.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "selection.h"
#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"
#include "commonstrings.h"
#include "guidemanager.h"

#include "util.h"
#include "text/nlsconfig.h"
#ifdef HAVE_CAIRO
#include <cairo.h>
#endif

using namespace std;

PageItem_TextFrame::PageItem_TextFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::TextFrame, x, y, w, h, w2, fill, outline)
{
	unicodeTextEditMode = false;
	unicodeInputCount = 0;
	unicodeInputString = "";
}


static QRegion itemShape(PageItem* docItem, ScribusView* view, double xOffset, double yOffset)
{
	QRegion res;
	QPainter pp;
	pp.begin(view->viewport());
	pp.translate(docItem->xPos() - xOffset, docItem->yPos() - yOffset);
	pp.rotate(docItem->rotation());
	if (docItem->textFlowUsesBoundingBox())
	{
		QPointArray tcli(4);
		tcli.setPoint(0, QPoint(0,0));
		tcli.setPoint(1, QPoint(qRound(docItem->width()), 0));
		tcli.setPoint(2, QPoint(qRound(docItem->width()), qRound(docItem->height())));
		tcli.setPoint(3, QPoint(0, qRound(docItem->height())));
		res = QRegion(pp.xForm(tcli));
	}
	else
	{
		if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
		{
			QValueList<uint> Segs;
			QPointArray Clip2 = FlattenPath(docItem->ContourLine, Segs);
			res = QRegion(pp.xForm(Clip2));
		}
		else
			res = QRegion(pp.xForm(docItem->Clip));
	}
	pp.end();
	return  res;
}

QRegion PageItem_TextFrame::availableRegion(QRegion clip)
{
	QRegion result = clip;
	int LayerLev = m_Doc->layerLevelFromNumber(LayerNr);
	uint docItemsCount=m_Doc->Items->count();
	if (!isEmbedded)
	{
		if (!OnMasterPage.isEmpty())
		{
			if ((savedOwnPage == -1) || (savedOwnPage >= signed(m_Doc->Pages->count())))
				return result;
			Page* Mp = m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]);
			Page* Dp = m_Doc->Pages->at(savedOwnPage);
			for (uint a = 0; a < m_Doc->MasterItems.count(); ++a)
			{
				PageItem* docItem = m_Doc->MasterItems.at(a);
				int LayerLevItem = m_Doc->layerLevelFromNumber(docItem->LayerNr);
				if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev) && (m_Doc->layerFlow(docItem->LayerNr)))
				{
					if (docItem->textFlowAroundObject())
					{
						result = result.subtract(itemShape(docItem, m_Doc->view(),  Mp->xOffset() - Dp->xOffset(), Mp->yOffset() - Dp->yOffset()));
					}
				}
			} // for all masterItems
			if (!m_Doc->masterPageMode())
			{
				for (uint a = 0; a < docItemsCount; ++a)
				{
					PageItem* docItem = m_Doc->Items->at(a);
					Page* Mp = m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]);
					Page* Dp = m_Doc->Pages->at(OwnPage);
					if ((docItem->textFlowAroundObject()) && (docItem->OwnPage == OwnPage))
					{
						result = result.subtract(itemShape(docItem, m_Doc->view(), Mp->xOffset() - Dp->xOffset(), Mp->yOffset() - Dp->yOffset()));
					}
				} // for all docItems
			} // if (! masterPageMode)
		} // if (!OnMasterPage.isEmpty())
		else
		{
			for (uint a = 0; a < docItemsCount; ++a)
			{
				PageItem* docItem = m_Doc->Items->at(a);
				int LayerLevItem = m_Doc->layerLevelFromNumber(docItem->LayerNr);
				if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev) && (m_Doc->layerFlow(docItem->LayerNr)))
				{
					 if (docItem->textFlowAroundObject())
						result = result.subtract(itemShape(docItem, m_Doc->view(), 0, 0));
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
	
//	QString newShadow = m_Doc->masterPageMode() ? QString::number(OwnPage) : OnMasterPage;
//	QString newShadow = QString::number(OwnPage);
	QString newShadow = m_Doc->masterPageMode() ? OnMasterPage : QString::number(OwnPage);
	qDebug(QString("Pageitem_Textframe: shadow %1 ... %2").arg(currentShadow).arg(newShadow));
	if (newShadow != currentShadow) {
		if (currentShadow == OnMasterPage) {
			// masterpage was edited, clear all shadows
			shadows.clear();
		}
		if (!shadows.contains(newShadow)) {
			if (!shadows.contains(OnMasterPage)) {
				shadows[OnMasterPage] = itemText;
//				qDebug(QString("Pageitem_Textframe: shadow itemText->%1").arg(OnMasterPage));
			}
			shadows[newShadow] = shadows[OnMasterPage].copy();
//			qDebug(QString("Pageitem_Textframe: shadow %1<-%2").arg(newShadow).arg(OnMasterPage));
		}
		itemText = shadows[newShadow];
		invalid = true;
		currentShadow = newShadow;
	}
//	qDebug(QString("Pageitem_Textframe: shadow=%1").arg(itemText.text(0, itemText.length())));
}
		
#ifdef NLS_PROTO
#include "text/pageitem_textframe.cpp"
#else

static void dumpIt(const ParagraphStyle& pstyle, QString indent = QString("->"))
{
	qDebug(QString("%6%1/%2 @ %3: %4--%5 linespa%6: %7 align%8")
		   .arg(pstyle.name())
		   .arg(pstyle.parent())
		   .arg( (unsigned long int) &pstyle)
		   .arg(pstyle.leftMargin())
		   .arg(pstyle.rightMargin())
		   .arg(indent)
		   .arg(pstyle.lineSpacingMode())
		   .arg(pstyle.lineSpacing())
		   .arg(pstyle.alignment()));
	static QString more("  ");
	if (pstyle.hasParent())
		dumpIt(*dynamic_cast<const ParagraphStyle*>(pstyle.parentStyle()), more + indent);
}

static const bool opticalMargins = false;

static void layoutDropCap(GlyphLayout layout, double curX, double curY, double offsetX, double offsetY, double dropCapDrop) {
	
}


void PageItem_TextFrame::layout() 
{
	if (BackBox != NULL && BackBox->invalid) {
//		qDebug("textframe: len=%d, going back", itemText.length());
		dynamic_cast<PageItem_TextFrame*>(BackBox)->layout();
		return;
	}
	else if (!invalid && OnMasterPage.isEmpty()) {
//		qDebug(QString("textframe: len=%1, invalid=%2 OnMasterPage=%3: no relayout").arg(itemText.length()).arg(invalid).arg(OnMasterPage));
		return;
	}
//	qDebug(QString("textframe(%1,%2): len=%3, start relayout at %4").arg(Xpos).arg(Ypos).arg(itemText.length()).arg(firstInFrame()));
	ScribusView* view = m_Doc->view();
	QPainter pf2;
	PageItem *nextItem;
	QPoint pt1, pt2;
	FPoint ColBound;
	QRegion cm;
	uint nrc, startLin;
	int aSpa, CurrCol;
	double chs, chsd;
	uint BuPos, LastSP, MaxText;
	double oldCurY, LastXp, EndX, OFs, OFs2, wide, ColWidth, kernVal, RTabX;
	QString chstr, chstr2, chstr3;
	ScText *hl;
	ParagraphStyle style;
	
	bool outs = false;
	bool fBorder = false;
	bool RTab = false;
	bool goNoRoom = false;
	bool goNextColumn = false;
	uint StartRT;
	int TabCode = 0;
	int HyphenCount = 0;
	QValueList<ParagraphStyle::TabRecord> tTabValues;
	bool DropCmode = false;
	bool AbsHasDrop = false;
	double desc, asce, maxDY, desc2, maxDX;
	int DropLines;
	double DropCapDrop = 0;
	bool StartOfCol = true;
	tTabValues.clear();
	
	itemText.clearLines();
	LineSpec curLine;
		
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
	
	pf2.begin(view->viewport());
	pf2.translate(Xpos, Ypos);
	pf2.rotate(Rot);

	double lineCorr = 0;
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth / 2.0;

	
	setShadow();
	if ((itemText.length() != 0)) // || (NextBox != 0))
	{		
		// determine layout area
		QRegion cl = availableRegion(QRegion(pf2.xForm(Clip)));
		
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
		
		CurrCol = 0;
		ColWidth = columnWidth();
		// ColBound.x() is left border, ColBound.y() is right(!) border
		ColBound = FPoint((ColWidth + ColGap) * CurrCol+Extra + lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
		ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
		CurX = ColBound.x();
		// find start of first line
		if (itemText.length() > 0)
		{
			hl = itemText.item(0);
			style = itemText.paragraphStyle(0);
			if (style.rightMargin() < 0) {
				qDebug(QString("style pos 0:"));
				dumpIt(style);
			}
//			qDebug(QString("style @0: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//				   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():""));
			if (style.hasDropCap())
			{
				if (style.useBaselineGrid())
					chs = qRound(m_Doc->typographicSettings.valueBaseGrid  * style.dropCapLines() * 10);
				else
					chs = qRound(style.lineSpacing() * style.dropCapLines() * 10);
			}
			else 
				chs = hl->fontSize();
			desc2 = -hl->font().descent(chs / 10.0);
			CurY = TExtra+lineCorr;
		}
		else // empty itemText:
		{
			desc2 = -itemText.defaultStyle().charStyle().font().descent(itemText.defaultStyle().charStyle().fontSize() / 10.0);
			CurY = itemText.defaultStyle().lineSpacing() + TExtra+lineCorr-desc2;
		}
		int itemsInLine = 0;
		curLine.firstItem = firstInFrame();
		curLine.ascent = 10;
		curLine.descent = 0;
		BuPos = 0;
		LastSP = 0;
		LastXp = 0;
		outs = false;
		OFs = 0;
		OFs2 = 0;
		aSpa = 0;
		MaxChars = 0;
		MaxText = itemText.length();
		StartOfCol = true;
		for (int a = firstInFrame(); a < itemText.length(); ++a)
		{
			hl = itemText.item(a);
			style = itemText.paragraphStyle(a);
			const CharStyle& charStyle = itemText.charStyle(a);
//			qDebug(QString("charstyle pos %1: %2 (%3)").arg(a).arg(charStyle.asString()).arg(charStyle.fontSize()));
			if (style.rightMargin() < 0) 
	//		if (itemsInLine == 0) 
			{
				qDebug(QString("style pos %1:").arg(a));
				dumpIt(style);
				//dumpIt(itemText.paragraphStyle(a));
			}
//			qDebug(QString("style @%6: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//				   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():"")
//				   .arg(a));

//			chstr = hl->ch;
//			if (hl->ch == QChar(30)) // pagenumber
			chstr = ExpandToken(a);
//			qDebug(QString("expanded token: '%1'").arg(chstr));
			if (chstr.isEmpty())
				continue;
			if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
				style.setLineSpacing(charStyle.font().height(charStyle.fontSize() / 10.0));
			// find out about par gap and dropcap
			if (a == firstInFrame())
			{
				if (BackBox != 0)
				{
					nextItem = BackBox;
					while (nextItem != 0)
					{
						if (nextItem->frameDisplays(a-1))
						{
							if (nextItem->itemText.text(a-1) == SpecialChars::PARSEP)
							{
								CurY += style.gapBefore();
								if (chstr != SpecialChars::PARSEP)
								{
									DropCmode = style.hasDropCap();
									if (DropCmode)
										DropLines = style.dropCapLines();
								}
								else
									DropCmode = false;
								break;
							}
							else
								break;
						}
						nextItem = nextItem->BackBox;
					}
				}
				else
				{
					if (chstr != SpecialChars::PARSEP)
					{
						DropCmode = style.hasDropCap();
						if (DropCmode)
							DropLines = style.dropCapLines();
					}
					else
						DropCmode = false;
					CurY += style.gapBefore();
				}
			}
			{  // local block for 'fl'
				StyleFlag fl = hl->effects();
				fl &= ~ScStyle_DropCap;
				fl &= ~ScStyle_SmartHyphenVisible;
				hl->setEffects(fl);
			}
			// No space at begin of line
			if ( //((m_Doc->docParagraphStyles[absa].alignment() == 3) || (m_Doc->docParagraphStyles[absa].alignment() == 4)) && 
				 (itemsInLine == 0) && (hl->ch == " "))
			{
				hl->setEffects(hl->effects() | ScStyle_SuppressSpace);
				hl->glyph.xadvance = 0;
				continue;
			}
			else
				hl->setEffects(hl->effects() & ~ScStyle_SuppressSpace);

//			qDebug(QString("textframe(%1,%2): len=%3, fontsize=%4, ascent=%5").arg(Xpos).arg(Ypos).arg(itemText.length())
//					   .arg(charStyle.fontSize()).arg(charStyle.font().ascent()));				
			if (itemsInLine == 0)
			{
 				// more about par gap and dropcaps
				if (((a > firstInFrame()) && (itemText.text(a-1) == SpecialChars::PARSEP)) || ((a == 0) && (BackBox == 0)) && (!StartOfCol))
				{
					CurY += style.gapBefore();
					DropCapDrop = 0;
					if (chstr != SpecialChars::PARSEP)
						DropCmode = style.hasDropCap();
					else
						DropCmode = false;
					if (DropCmode)
					{
						DropLines = style.dropCapLines();
						if (style.useBaselineGrid())
							DropCapDrop = m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
						else
						{
							if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
								DropCapDrop = style.lineSpacing() * (DropLines-1);
							else
								DropCapDrop = charStyle.font().height(style.charStyle().fontSize() / 10.0) * (DropLines-1);
						}
						CurY += DropCapDrop;
					}
				}
			}
			// find charsize & y pos
			if (DropCmode)
			{
				// dropcap active?
				if (style.useBaselineGrid())
					DropCapDrop = m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
				else
				{
					if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
						DropCapDrop = style.lineSpacing() * (DropLines-1);
					else
						DropCapDrop = charStyle.font().height(style.charStyle().fontSize() / 10.0) * (DropLines-1);
				}

				if (style.useBaselineGrid())
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
				if ((hl->ch == SpecialChars::OBJECT) && (hl->cembedded != 0))
					chs = qRound((hl->cembedded->gHeight + hl->cembedded->lineWidth()) * 10);
				else
					chs = charStyle.fontSize();
			}
			// Smallcaps and such
			hl->glyph.yadvance = 0;
			oldCurY = layoutGlyphs(*hl, chstr, hl->glyph);
			chstr2 = chstr;
			// find out width of char
			if ((hl->ch == SpecialChars::OBJECT) && (hl->cembedded != 0))
				wide = hl->cembedded->gWidth + hl->cembedded->lineWidth();
			else
			{
/*				if (a+1 < itemText.length())
				{
						chstr3 = itemText.text(a+1);
					// apply kerning
					wide = charStyle.font().charWidth(chstr2[0], chs / 10.0, chstr3[0]);
				}
				else
					wide = charStyle.font().charWidth(chstr2[0], chs / 10.0);
*/			
				wide = hl->glyph.wide();
				if (a+1 < itemText.length())
				{
					uint glyph2 = charStyle.font().char2CMap(itemText.text(a+1));
					wide += charStyle.font().glyphKerning(hl->glyph.glyph, glyph2, chs / 10.0) * hl->glyph.scaleH;
				}
			}
			if (DropCmode)
			{
				// drop caps are wider...
				if ((hl->ch == SpecialChars::OBJECT) && (hl->cembedded != 0))
				{
					wide = hl->cembedded->gWidth + hl->cembedded->lineWidth();
					if (style.useBaselineGrid())
						asce = m_Doc->typographicSettings.valueBaseGrid * DropLines;
					else
					{
						if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
							asce = style.lineSpacing() * DropLines;
						else
							asce = charStyle.font().height(style.charStyle().fontSize() / 10.0) * DropLines;
					}
					hl->glyph.scaleH /= hl->glyph.scaleV;
					hl->glyph.scaleV = (asce / (hl->cembedded->gHeight + hl->cembedded->lineWidth()));
					hl->glyph.scaleH *= hl->glyph.scaleV;
				}
				else
				{
					wide = charStyle.font().realCharWidth(chstr2[0], chsd / 10.0);
					asce = charStyle.font().realCharHeight(chstr2[0], chsd / 10.0);
//					qDebug(QString("dropcaps pre: chsd=%1 realCharHeight = %2 chstr=%3").arg(chsd).arg(asce).arg(chstr2[0]));
					hl->glyph.scaleH /= hl->glyph.scaleV;
					hl->glyph.scaleV = (asce / charStyle.font().realCharAscent(chstr2[0], charStyle.fontSize() / 10.0));
					hl->glyph.scaleH *= hl->glyph.scaleV;
				}
				hl->glyph.xadvance = wide;
				desc2 = 0;
				desc = 0;
			}
			else
			{
//				qDebug(QString("textframe ascent/descent: fontsize=%1, ascent=%2, descent=%3")
//					   .arg(charStyle.fontSize()).arg(charStyle.font().ascent()).arg(charStyle.font().descent()));				
				// find ascent / descent
				double hlcsize10=charStyle.fontSize() / 10.0;
				if ((hl->ch == SpecialChars::OBJECT) && (hl->cembedded != 0))
				{
					desc = desc2 = 0;
				}
				else
				{
					desc = desc2 = -charStyle.font().descent(hlcsize10);
				}
				asce = charStyle.font().ascent(hlcsize10);
//				wide = wide * hl->glyph.scaleH;
			}
			fBorder = false;
			// end of col reached?
//			qDebug(QString("eocol? %1 + %2 + %3 > %4 -- asce=%5").arg(CurY).arg(BExtra).arg(lineCorr).arg(Height).arg(asce));
			if (CurY+BExtra+lineCorr > Height)
			{
				StartOfCol = true;
				CurrCol++;
				if (CurrCol < Cols)
				{
					// start next col
					ColWidth = columnWidth();
					ColBound = FPoint((ColWidth + ColGap) * CurrCol + Extra+lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
					CurX = ColBound.x();
					ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
					CurY = asce+TExtra+lineCorr+1;
					if (((a > firstInFrame()) && (itemText.text(a-1) == SpecialChars::PARSEP)) 
						|| ((a == firstInFrame()) && (BackBox == 0)))
					{
						if (chstr != SpecialChars::PARSEP)
							DropCmode = style.hasDropCap();
						else
							DropCmode = false;
						if (DropCmode)
						{
							if (style.useBaselineGrid())
								desc2 = -charStyle.font().descent() * m_Doc->typographicSettings.valueBaseGrid * style.dropCapLines();
							else
								desc2 = -charStyle.font().descent() * style.lineSpacing() * style.dropCapLines();
						}
						if (DropCmode)
							DropLines = style.dropCapLines();
					}
					if (style.useBaselineGrid())
					{
						double by = Ypos;
						if (OwnPage != -1)
							by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
						int ol1 = qRound((by + CurY - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
						int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
						CurY = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
					}
				}
				else
				{
					nrc = a;
//					qDebug(QString("no room A: %1").arg(a));
					goto NoRoom;
				}
			}
			// ok, there's more room in col
			if (itemsInLine == 0)
			{
//				qDebug(QString("newline: '%1' %2 %3").arg(a).arg(CurY).arg(asce));
				// start a new line
				startLin = a;
				double TopOffset = asce;
				double BotOffset = desc2;
				goNoRoom = false;
				// ??? AV
				bool specialCase = false;
				if (StartOfCol)
				{
//					qDebug(QString("StartOfCol: %1 + %2 + %3").arg(asce).arg(TExtra).arg(lineCorr));
					CurY = asce+TExtra+lineCorr+1;
//							if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
//								CurY += m_Doc->docParagraphStyles[hl->cab].gapBefore;
				}
				if (style.useBaselineGrid())
				{
					double by = Ypos;
					if (OwnPage != -1)
						by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
					int ol1 = qRound((by + CurY - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
					int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
//					qDebug(QString("useBaselIneGrid: %1 * %2 + %3 - %4").arg(ol2 / 10000.0).arg(m_Doc->typographicSettings.valueBaseGrid).arg(m_Doc->typographicSettings.offsetBaseGrid).arg(by));
					CurY = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
				}
				if (CurY-TopOffset < 0.0)
					CurY = TopOffset+1;
				// find linelength:
				pt1 = QPoint(static_cast<int>(ceil(CurX-Extra)), static_cast<int>(CurY+BotOffset));
				pt2 = QPoint(static_cast<int>(ceil(CurX-Extra)), static_cast<int>(ceil(CurY-TopOffset)));

				if ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))))
				{
					specialCase = true;
					CurX -= Extra;
				}
				pt1 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(CurY+BotOffset));
				pt2 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(ceil(CurY-TopOffset)));
				// increase pt1/pt2 until i-beam reaches end of line
				while ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))))
				{
					fBorder = true;
					CurX++;
					if (CurX+RExtra+lineCorr > ColBound.y() - style.rightMargin())
					{
//						qDebug(QString("eocol %5? %1 + %2 + %3 + %4").arg(CurY).arg(StartOfCol).arg(style.useBaselineGrid()).arg(style.lineSpacing()).arg(CurrCol));
						// new line
						fBorder = false;
						if (StartOfCol)
						{
							CurX = ColBound.x();
							CurY++;
						}
						else
						{
							CurY += style.lineSpacing();
							CurX = ColBound.x();
						}
						if (style.useBaselineGrid())
						{
							double by = Ypos;
							if (OwnPage != -1)
								by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
							int ol1 = qRound((by + CurY - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
							CurY = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
						}
						if (CurY+BExtra+lineCorr > Height)
						{
							StartOfCol = true;
							fBorder = false;
							CurrCol++;
							if (CurrCol < Cols)
							{
								ColWidth = columnWidth();
								ColBound = FPoint((ColWidth + ColGap) * CurrCol + Extra+lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
								CurX = ColBound.x();
								ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
								CurY = asce+TExtra+lineCorr+1;
								if (((a > firstInFrame()) && (itemText.text(a-1) == SpecialChars::PARSEP)) || ((a == firstInFrame()) && (BackBox == 0)))
								{
									if (chstr != SpecialChars::PARSEP)
										DropCmode = style.hasDropCap();
									else
										DropCmode = false;
									if (DropCmode)
									{
										if (style.useBaselineGrid())
											desc2 = -charStyle.font().descent() * m_Doc->typographicSettings.valueBaseGrid * style.dropCapLines();
										else
											desc2 = -charStyle.font().descent() * style.lineSpacing() * style.dropCapLines();
									}
									if (DropCmode)
										DropLines = style.dropCapLines();
								}
								if (style.useBaselineGrid())
								{
									double by = Ypos;
									if (OwnPage != -1)
										by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
									int ol1 = qRound((by + CurY - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
									int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
									CurY = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
								}
							}
							else
							{
								nrc = a;
//								qDebug(QString("no room B: %1").arg(a));
								goto NoRoom;
							}
						}
					}
					pt1 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(CurY+BotOffset));
					pt2 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(ceil(CurY-TopOffset)));
				}

				if (((fBorder)) && (!AbsHasDrop))
					CurX += Extra;
				// indent first line of par
				if (a > 0)
				{
					if (itemText.text(a-1) == SpecialChars::PARSEP)
						CurX += style.firstIndent();
				}
				else
				{
					if (BackBox == 0)
						CurX += style.firstIndent();
					else
					{
						if (BackBox->frameDisplays(firstInFrame()-1))
						{
							if (BackBox->itemText.text(BackBox->lastInFrame()) == SpecialChars::PARSEP)
								CurX += style.firstIndent();
						}
						else
							CurX += style.firstIndent();
					}
				}
				// add left margin
				if (!AbsHasDrop)
					CurX += style.leftMargin();
				if (opticalMargins) {
					double chs = itemText.charStyle(a).fontSize() * (itemText.charStyle(a).scaleH() / 1000.0);
					double leftCorr = itemText.charStyle(a).font().realCharWidth(itemText.text(a), chs / 10.0);
					if (QString("'´`").find(itemText.text(a)) >= 0
						|| itemText.text(a) == QChar(0x2018)
						|| itemText.text(a) == QChar(0x2019)
						|| itemText.text(a) == QChar(0x201a)
						|| itemText.text(a) == QChar(0x201b)
						|| itemText.text(a) == QChar(0x2039)
						|| itemText.text(a) == QChar(0x203a)
						)
						leftCorr *= -0.7;
					else if (QString("\"").find(itemText.text(a)) >= 0
							 || itemText.text(a) == QChar(0x00ab)
							 || itemText.text(a) == QChar(0x00bb)
							 || itemText.text(a) == QChar(0x201c)
							 || itemText.text(a) == QChar(0x201d)
							 || itemText.text(a) == QChar(0x201e)
							 || itemText.text(a) == QChar(0x201f)
							 ) 
						leftCorr *= -0.5;
					else {
						leftCorr = itemText.charStyle(a).font().charWidth(QChar(' '), chs / 10.0, itemText.text(a));
						leftCorr -= itemText.charStyle(a).font().charWidth(QChar(' '), chs / 10.0);
		//				double leftCorr2 = itemText.charStyle(a).font().charWidth(QChar('K'), chs / 10.0, itemText.text(a));
		//				leftCorr2 -= itemText.charStyle(a).font().charWidth(QChar('K'), chs / 10.0);
		//				leftCorr = QMIN(leftCorr, leftCorr2);
					}
					CurX += leftCorr;
				}
				curLine.x = CurX;
				curLine.y = CurY;
				fBorder = false;
			}
			else {
//				qDebug(QString("cont.' line: '%1'").arg(a));
			}
			// right tab stuff
			if (RTab)
			{
				if (((hl->ch == ".") && (TabCode == 2)) || ((hl->ch == ",") && (TabCode == 3)) || (hl->ch == SpecialChars::TAB))
				{
					RTab = false;
					TabCode = 0;
				}
			}
			// tab positioning
			if (hl->ch == SpecialChars::TAB)
			{
				wide = 1;
				if (RTab)
					RTab = false;
				else
				{
					RTabX = CurX+wide;
					tTabValues = style.tabValues();
					if (tTabValues.isEmpty())
					{
						if ((CurX - ColBound.x()) != 0)
						{
							if (CurX == ColBound.x() + ceil((CurX-ColBound.x()) / m_Doc->toolSettings.dTabWidth) * m_Doc->toolSettings.dTabWidth)
								CurX += m_Doc->toolSettings.dTabWidth;
							else
								CurX = ColBound.x() + ceil((CurX-ColBound.x()) / m_Doc->toolSettings.dTabWidth) * m_Doc->toolSettings.dTabWidth;
						}
						else
							CurX = ColBound.x() + m_Doc->toolSettings.dTabWidth;
						TabCode = 0;
						RTab = false;
					}
					else
					{
						double tCurX = CurX - ColBound.x();
						double oCurX = tCurX + wide;
						for (int yg = static_cast<int>(tTabValues.count()-1); yg > -1; yg--)
						{
							if (oCurX < tTabValues[yg].tabPosition)
							{
								tCurX = tTabValues[yg].tabPosition;
								TabCode = static_cast<int>(tTabValues[yg].tabType);
							}
						}
						if (TabCode == 0)
							RTab = false;
						else
							RTab = true;
						if (tCurX == oCurX-wide)
							CurX = ColBound.x() + ceil((CurX-ColBound.x()) / m_Doc->toolSettings.dTabWidth) * m_Doc->toolSettings.dTabWidth;
						else
							CurX = ColBound.x() + tCurX;
					}
					CurX -= 1;
					StartRT = a;
				}
			}
			
			// remember y pos
//			hl->glyph.yoffset = CurY + oldCurY;
//			hl->glyph.yoffset = 0;
			if (DropCmode)
				hl->glyph.yoffset -= charStyle.font().realCharHeight(chstr2[0], chsd / 10.0) - charStyle.font().realCharAscent(chstr2[0], chsd / 10.0);
			// find tracking
			if (itemsInLine == 0)
			{
				itemText.item(a)->setEffects(itemText.item(a)->effects() | ScStyle_StartOfLine);
				kernVal = 0;
			}
			else
			{
				kernVal = chs * charStyle.tracking() / 10000.0;
				itemText.item(a)->setEffects(itemText.item(a)->effects() & ~ScStyle_StartOfLine);
			}
			// remember x pos
			if (!RTab)
			{
//				hl->glyph.xoffset = QMAX(CurX+kernVal, ColBound.x());
				//hl->glyph.xoffset = CurX+kernVal; // needed for left optical margin
				CurX += wide+kernVal;
//				CurX = QMAX(CurX, ColBound.x());
			}
			else
			{
				CurX = QMAX(CurX, ColBound.x());
				//hl->glyph.xoffset = CurX;
			}
			// more right tab stuff
			if ((TabCode == 4) && (RTab))
			{
				CurX += (wide+kernVal) / 2;
				CurX = QMAX(CurX, ColBound.x());
			}
			// hyphenation
			if ((((hl->effects() & ScStyle_HyphenationPossible) || hl->ch == "-") && (HyphenCount < m_Doc->HyCount || m_Doc->HyCount == 0))  || hl->ch[0] == SpecialChars::SHYPHEN)
			{
				if (hl->effects() & ScStyle_HyphenationPossible || hl->ch[0] == SpecialChars::SHYPHEN)
				{
					pt1 = QPoint(qRound(ceil(CurX+RExtra+ charStyle.font().charWidth('-', charStyle.fontSize() / 10.0) * (charStyle.scaleH() / 1000.0))), qRound(CurY+desc));
					pt2 = QPoint(qRound(ceil(CurX+RExtra+ charStyle.font().charWidth('-', charStyle.fontSize() / 10.0) * (charStyle.scaleH() / 1000.0))), qRound(ceil(CurY-asce)));
				}
				else
				{
					pt1 = QPoint(qRound(ceil(CurX+RExtra)), qRound(CurY+desc));
					pt2 = QPoint(qRound(ceil(CurX+RExtra)), qRound(ceil(CurY-asce)));
				}
			}
			else
			{
				pt1 = QPoint(qRound(ceil(CurX+RExtra)), qRound(CurY+desc));
				pt2 = QPoint(qRound(ceil(CurX+RExtra)), qRound(ceil(CurY-asce)));
			}

			// test if end of line reached
			if ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))) || (CurX+RExtra+lineCorr > ColBound.y() - style.rightMargin()))
				outs = true;
			if (CurY > (Height - BExtra - lineCorr))
				outs = true;
			if ((hl->ch == QChar(27)) && (a < itemText.length()-1))
				goNoRoom = true;
			if ((hl->ch == QChar(26)) && (Cols > 1))
				goNextColumn = true;

			// remember possible break
			if (((hl->ch == " ") || (hl->ch == SpecialChars::TAB)) && (!outs))
			{
				if ( a == firstInFrame() || itemText.text(a-1) !=  ' ')
				{
					LastXp = CurX;
					LastSP = BuPos;
				}
			}
			if (((hl->effects() & ScStyle_HyphenationPossible) || (hl->ch == "-") || hl->ch[0] == SpecialChars::SHYPHEN) && (!outs))
			{
				if ((HyphenCount < m_Doc->HyCount) || (m_Doc->HyCount == 0))
				{
					if (hl->ch == "-")
						LastXp = CurX;
					else
						LastXp = CurX + charStyle.font().charWidth('-', charStyle.fontSize() / 10.0) * (charStyle.scaleH() / 1000.0);
					LastSP = BuPos + 1;
				}
			}
			++itemsInLine;

			if (RTab)
			{
				double cen = 1;
				if (TabCode == 4)
					cen = 2;
				for (uint rtx = StartRT; rtx < StartRT + itemsInLine; ++rtx)
				{
					itemText.item(rtx)->glyph.xoffset = QMAX(itemText.item(rtx)->glyph.xoffset-(wide+kernVal) / cen, 0.0);
					if (itemText.item(rtx)->glyph.xoffset < RTabX)
					{
						RTab = false;
						TabCode = 0;
					}
				}
			}
			BuPos++;
			if (DropCmode)
			{
				DropCmode = false;
				AbsHasDrop = true;
				maxDY = CurY;
				CurX += style.dropCapOffset();
				CurX = QMAX(CurX, ColBound.x());
				maxDX = CurX;
				QPointArray tcli(4);
				if (style.useBaselineGrid())
				{
					CurY -= m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
					double by = Ypos;
					if (OwnPage != -1)
						by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
					int ol1 = qRound((by + CurY - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
					int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
					CurY = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
					//FIXME: use colBound.x() instead of xOffset?
					tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY-DropLines*m_Doc->typographicSettings.valueBaseGrid)));
					tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*m_Doc->typographicSettings.valueBaseGrid)));
				}
				else
				{
					if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
					{
						CurY -= style.lineSpacing() * (DropLines-1);
						tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY - DropLines * style.lineSpacing())));
						tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*style.lineSpacing())));
					}
					else
					{
						double currasce = charStyle.font().height(style.charStyle().fontSize() / 10.0);
						CurY -= currasce * (DropLines-1);
						tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY-DropLines*currasce)));
						tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*currasce)));
					}
				}
				tcli.setPoint(2, QPoint(qRound(maxDX), qRound(maxDY)));
				tcli.setPoint(3, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY)));
				cm = QRegion(pf2.xForm(tcli));
				cl = cl.subtract(cm);
//				CurY = maxDY;
			}
			// end of line
			if ((hl->ch == SpecialChars::PARSEP) 
				|| (hl->ch == SpecialChars::LINEBREAK) 
				|| (hl->ch == SpecialChars::FRAMEBREAK)  
				|| ((hl->ch == SpecialChars::COLBREAK) && (Cols > 1)) 
				|| (outs))
			{
				RTab = false;
				TabCode = 0;
				if ((hl->ch == SpecialChars::PARSEP) 
					|| (hl->ch == SpecialChars::LINEBREAK) 
					|| (hl->ch == SpecialChars::FRAMEBREAK)  
					|| ((hl->ch == SpecialChars::COLBREAK) && (Cols > 1)))
				{
					if (style.alignment() != 0)
					{
						// find end of line
						EndX = floor(QMAX(curLine.x, LastXp - 10));
//						qDebug(QString("endx start=%1, hl is '%2'").arg(EndX).arg(hl->ch));
						do {
							EndX += 0.125;
							pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
							pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
						} while ((cl.contains(pf2.xForm(pt1))) 
								 && (cl.contains(pf2.xForm(pt2))) 
								 && (EndX+RExtra+lineCorr < ColBound.y() - style.rightMargin()));
							
						// end do
//						qDebug(QString("endx @ %1").arg(EndX));
						
						curLine.naturalWidth = CurX - curLine.x;
						curLine.width = EndX - curLine.x;
						if (style.alignment() == 2)
							OFs = EndX - CurX;
						if (style.alignment() == 1)
							OFs = (EndX - CurX) / 2;
						if (style.alignment() == 3)
							OFs = 0;
						if (style.alignment() == 4)
						{
							// count the available spaces
							aSpa = 0;
							for (int sof = 0; sof < itemsInLine; ++sof)
							{
								if ((itemText.text(curLine.firstItem + sof) == QChar(32)
									|| itemText.text(curLine.firstItem + sof) == SpecialChars::NBSPACE)
									&& ! (itemText.item(curLine.firstItem + sof)->effects() & ScStyle_SuppressSpace)
									)
									aSpa++;
							}
							if (aSpa != 0)
								OFs2 = (EndX - CurX) / aSpa;
							else
								OFs2 = 0;
							OFs = 0;
							// distribute whitespace on spaces
							for (int yof = 0; yof < itemsInLine; ++yof)
							{
								/*
								for (GlyphLayout* gp = &itemText.item(curLine.firstItem + yof)->glyph; gp; gp = gp->more)
									gp->xoffset += OFs;
								 */
								if ((itemText.text(curLine.firstItem + yof) == QChar(32) 
									|| itemText.text(curLine.firstItem + yof) == SpecialChars::NBSPACE)
									&& ! (itemText.item(curLine.firstItem + yof)->effects() & ScStyle_SuppressSpace)
									)
								{
									itemText.item(curLine.firstItem + yof)->glyph.last()->xadvance += OFs2;
									OFs += OFs2;
								}
							}
						}
						else
						{
							// simple offset
							curLine.x += OFs;
/*							for (int xof = 0; xof < itemsInLine; ++xof)
							{
								itemText.item(curLine.firstItem + xof)->glyph.xoffset += OFs;
							}
*/
						}
						CurX = EndX;
					}
					else {
						curLine.naturalWidth = CurX - curLine.x;
						curLine.width = CurX - curLine.x;
					}
				}
				else // outs -- last char went outside the columns
				{
					if (LastSP != 0)            // Hier koenen auch andere Trennungen eingebaut werden
					{
						// go back to last break position
//						qDebug(QString("new break pos a=%1 BuPos=%2 LastSP=%3").arg(a).arg(BuPos).arg(LastSP));
						a -= BuPos - LastSP;
						assert( a >= 0 );
						assert( a < itemText.length() );
						hl = itemText.item(a);
						style = itemText.paragraphStyle(a);
						if (style.rightMargin() < 0) {
							qDebug(QString("style pos < %1:").arg(a));
							dumpIt(style);
						}
//						qDebug(QString("style <@%6: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//							   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():"")
//							   .arg(a));
//not needed any more:	a++;
						// Fix incorrect right flush for full lines: ? -AV
						LastXp = curLine.x;
						for (int j=curLine.firstItem; j <= a; ++j)
							LastXp += itemText.item(j)->glyph.wide();
						
						if (hl->effects() & ScStyle_HyphenationPossible || hl->ch[0] == SpecialChars::SHYPHEN)
						{
							// insert hyphen
							HyphenCount++;
							hl->setEffects(hl->effects() | ScStyle_SmartHyphenVisible);
							hl->glyph.more = new GlyphLayout();
							hl->glyph.more->glyph = charStyle.font().char2CMap(QChar('-'));
							hl->glyph.more->xadvance = charStyle.font().charWidth('-', itemText.charStyle(a).fontSize() / 10.0) * (itemText.charStyle(a).scaleH() / 1000.0);
							LastSP += 2;
							LastXp += hl->glyph.more->xadvance;
						}
						else 
						{
							HyphenCount = 0;
							hl->setEffects(hl->effects() & ~ScStyle_SmartHyphenVisible);
							if (hl->glyph.more) {
								delete hl->glyph.more;
								hl->glyph.more = 0;
							}
						}
						BuPos = LastSP;
						// Justification
						if (style.alignment() != 0)
						{
							// find end of line
							EndX = floor(QMAX(curLine.x, LastXp - 10));
//							qDebug(QString("endx start=%1, hl is '%2'").arg(EndX).arg(hl->ch));
							do {
								EndX += 0.125;
								pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
								pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
							} while ((cl.contains(pf2.xForm(pt1))) 
									 && (cl.contains(pf2.xForm(pt2))) 
									 && (EndX+RExtra+lineCorr < ColBound.y() - style.rightMargin()));
								
							// end do
//							qDebug(QString("endx @ %1").arg(EndX));

							if (opticalMargins && a >= curLine.firstItem) {
								double chs = itemText.charStyle(a).fontSize() * (itemText.charStyle(a).scaleH() / 1000.0);
								QString chr = itemText.item(a)->glyph.more ? "-" : itemText.text(a,1);
								double rightCorr = itemText.charStyle(a).font().realCharWidth(chr[0], chs / 10.0);
								if (QString("-,.`´'~").find(chr) >= 0
									|| chr[0] == QChar(0x2018)
									|| chr[0] == QChar(0x2019)
									|| chr[0] == QChar(0x201a)
									|| chr[0] == QChar(0x201b)
									|| chr[0] == QChar(0x2039)
									|| chr[0] == QChar(0x203a)
									|| chr[0] == QChar(0x2032)
									)
									rightCorr *= 0.7;
								else if (QString(";:\"").find(chr) >= 0
										 || chr[0] == QChar(0x00ab)
										 || chr[0] == QChar(0x00bb)
										 || chr[0] == QChar(0x201c)
										 || chr[0] == QChar(0x201d)
										 || chr[0] == QChar(0x201e)
										 || chr[0] == QChar(0x201f)
										 || chr[0] == QChar(0x2013)
										 || chr[0] == QChar(0x2033)
										 )
									rightCorr *= 0.5;
								else {
									rightCorr = itemText.charStyle(a).font().charWidth(chr[0], chs / 10.0);
									rightCorr -= itemText.charStyle(a).font().charWidth(chr[0], chs / 10.0, QChar('.'));
								}
								EndX += rightCorr;
//								qDebug(QString("orm %1 @ %2: %3 %4").arg(rightCorr).arg(a).arg(chr).arg(itemText.charStyle(a).effects() & ScStyle_HyphenationPossible? "smart" : ""));
							}
							curLine.naturalWidth = LastXp - curLine.x;
							curLine.width = EndX - curLine.x;
							if (style.alignment() == 2)
								OFs = EndX - LastXp;
							if (style.alignment() == 1)
								OFs = (EndX - LastXp) / 2;
							if ((style.alignment() == 3) 
								|| (style.alignment() == 4))
							{
								// count available spaces
								aSpa = 0; 
								for (uint sof = 0; sof<LastSP; ++sof)
								{
									if ((itemText.item(curLine.firstItem + sof)->ch == QChar(32)
										 || itemText.item(curLine.firstItem + sof)->ch == SpecialChars::NBSPACE)
										&& ! (itemText.item(curLine.firstItem + sof)->effects() & ScStyle_SuppressSpace)
										)
										aSpa++;
								}
								if (aSpa > 0)
									OFs2 = (EndX - LastXp) / aSpa;
								else
									OFs2 = 0;
								
								OFs = 0;
//								qDebug(QString("just %1").arg(OFs2));
								for (int yof = 0; yof < itemsInLine; ++yof)
								{
									if ( (itemText.item(curLine.firstItem + yof)->ch == QChar(32)
										 || itemText.item(curLine.firstItem + yof)->ch == SpecialChars::NBSPACE)
										 && ! (itemText.item(curLine.firstItem + yof)->effects() & ScStyle_SuppressSpace)
										 )
									{
										itemText.item(curLine.firstItem + yof)->glyph.last()->xadvance += OFs2;
										OFs += OFs2;
									}
								}
							}
							else
							{
								curLine.x += OFs;
							}
//							qDebug(QString("line: endx=%1 lastchar=%2").arg(EndX).arg(LiList.at(BuPos-1)->xco + LiList.at(BuPos-1)->wide));
							CurX = EndX;
						}
						else {
							curLine.naturalWidth = LastXp - curLine.x;
							curLine.width = LastXp - curLine.x;
						}
					}
					else if (a > 0) // no break position
					{
						a--;
						assert( a >= 0 );
						assert( a < itemText.length() );
						hl = itemText.item(a);
						style = itemText.paragraphStyle(a);
						if (style.rightMargin() < 0) {
							qDebug(QString("style nb pos %1:").arg(a));
							dumpIt(style);
						}
//						qDebug(QString("style nb @%6: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//							   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():"")
//							   .arg(a));
						curLine.naturalWidth = CurX - curLine.x;
						curLine.width = CurX - curLine.x;
						BuPos--;
					}
				}
				uint BuPos3 = BuPos;
				if ((outs) 
					|| (hl->ch == SpecialChars::PARSEP) 
					|| (hl->ch == SpecialChars::LINEBREAK) 
					|| (hl->ch == SpecialChars::FRAMEBREAK) 
					|| ((hl->ch == SpecialChars::COLBREAK) && (Cols > 1)))
				{
					if ((outs) && (CurX+RExtra+lineCorr < ColBound.y() - style.rightMargin()))
					{
						if (( hl->ch == SpecialChars::PARSEP || hl->ch == SpecialChars::LINEBREAK) 
							&& AbsHasDrop)
						{
							AbsHasDrop = false;
							if (CurY < maxDY)
								CurY = maxDY;
						}
						bool fromOut = true;
						double BotOffset = desc+BExtra+lineCorr;
						pt1 = QPoint(qRound(CurX+RExtra), static_cast<int>(CurY+BotOffset));
						pt2 = QPoint(qRound(CurX+RExtra), static_cast<int>(ceil(CurY-asce)));
						while (CurX+RExtra+lineCorr < ColBound.y() - style.rightMargin())
						{
							CurX++;
							if (CurX+RExtra+lineCorr > ColBound.y() - style.rightMargin())
							{
								fromOut = false;
								if (style.useBaselineGrid())
									CurY += m_Doc->typographicSettings.valueBaseGrid;
								else
									CurY += style.lineSpacing();
								if ((CurY+desc+BExtra+lineCorr > Height) && (CurrCol+1 == Cols))
								{
									goNoRoom = true;
//									qDebug(QString("go no room 1: %1").arg(a));
									break;
								}
								if (AbsHasDrop)
								{
									if ((CurY > maxDY) && (CurY - asce > maxDY))
									{
										AbsHasDrop = false;
										CurX = ColBound.x();
									}
									else
										CurX = maxDX;
								}
								else
									CurX = ColBound.x();
								if ((hl->ch == SpecialChars::PARSEP) 
									|| (hl->ch == SpecialChars::LINEBREAK) 
									|| (hl->ch == SpecialChars::FRAMEBREAK) 
									|| (hl->ch == SpecialChars::COLBREAK))
								{
									if (hl->ch == SpecialChars::PARSEP)
										CurY += style.gapAfter();
									if (BuPos3 > 0)
										BuPos3 -= 1;
									HyphenCount = 0;
								}
								break;
							}
							pt1 = QPoint(qRound(CurX+RExtra), static_cast<int>(CurY+BotOffset));
							pt2 = QPoint(qRound(CurX+RExtra), static_cast<int>(ceil(CurY-asce)));
							if ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))))
								break;
						}
						if (fromOut)
						{
							if ((CurY+desc+BExtra+lineCorr > Height) && (CurrCol+1 == Cols))
							{
								goNoRoom = true;
//								qDebug(QString("go no room 2: %1").arg(a));
								break;
							}
							CurX--;
							CurX = QMAX(CurX, ColBound.x());
						}
					}
					else
					{
						if (( hl->ch == SpecialChars::PARSEP || hl->ch == SpecialChars::LINEBREAK ) 
							&& AbsHasDrop)
						{
							AbsHasDrop = false;
							if (CurY < maxDY)
								CurY = maxDY;
						}
						if (style.useBaselineGrid())
							CurY += m_Doc->typographicSettings.valueBaseGrid;
						else
						{
							if (a < lastInFrame())
								CurY += itemText.paragraphStyle(a+1).lineSpacing();
							else
								CurY += style.lineSpacing();
						}
						if (AbsHasDrop)
						{
							if ((CurY > maxDY) && (CurY - asce > maxDY))
							{
								AbsHasDrop = false;
								CurX = ColBound.x();
							}
							else
								CurX = maxDX;
						}
						else
							CurX = ColBound.x();
						if ( hl->ch == SpecialChars::PARSEP
							 || hl->ch == SpecialChars::LINEBREAK
							 || hl->ch == SpecialChars::FRAMEBREAK 
							 || hl->ch == SpecialChars::COLBREAK )
						{
							if (hl->ch == SpecialChars::PARSEP)
								CurY += style.gapAfter();
							if (BuPos3 > 0)
								BuPos3 -= 1;
							HyphenCount = 0;
						}
					}
				}
				

				// calc. needed vertical space
				if (itemsInLine != 0)
				{
					if ((!AbsHasDrop) && (StartOfCol) && (!style.useBaselineGrid()))
					{
						double firstasce = itemText.charStyle(curLine.firstItem).font().ascent(itemText.charStyle(curLine.firstItem).fontSize() / 10.0);
						double currasce = 0;
						if ((itemText.text(curLine.firstItem) == QChar(13)) || (itemText.text(curLine.firstItem) == QChar(28)))
							currasce = itemText.charStyle(curLine.firstItem).font().ascent(itemText.charStyle(curLine.firstItem).fontSize() / 10.0);
						else if (itemText.object(curLine.firstItem) != 0)
							currasce = QMAX(currasce, (itemText.object(curLine.firstItem)->gHeight + itemText.object(curLine.firstItem)->lineWidth()) * (itemText.charStyle(curLine.firstItem).scaleV() / 1000.0));
						else //if (itemText.charStyle(curLine.firstItem).effects() & ScStyle_DropCap == 0)
							currasce = itemText.charStyle(curLine.firstItem).font().realCharAscent(itemText.text(curLine.firstItem), itemText.charStyle(curLine.firstItem).fontSize() / 10.0);
						for (int zc = 0; zc < itemsInLine; ++zc)
						{
							QChar ch = itemText.text(curLine.firstItem + zc);
							const CharStyle& cStyle(itemText.charStyle(curLine.firstItem + zc));
							if ((ch == SpecialChars::TAB) || (ch == QChar(10))
								|| (ch == SpecialChars::PARSEP) || (ch == SpecialChars::NBHYPHEN)
								|| (ch == SpecialChars::COLBREAK) || (ch == SpecialChars::LINEBREAK)
								|| (ch == SpecialChars::FRAMEBREAK) || (ch == SpecialChars::NBSPACE))
								continue;
							if (itemText.object(curLine.firstItem + zc) != 0)
								currasce = QMAX(currasce, (itemText.object(curLine.firstItem + zc)->gHeight + itemText.object(curLine.firstItem + zc)->lineWidth()) * (cStyle.scaleV() / 1000.0));
							else //if (itemText.charStyle(curLine.firstItem + zc).effects() & ScStyle_DropCap == 0)
								currasce = QMAX(currasce, cStyle.font().realCharAscent(ch, cStyle.fontSize() / 10.0));
						}
						double adj = firstasce - currasce;
						curLine.ascent = currasce;
						curLine.y -= adj;
						CurY -= adj;
					}
					else if ((!StartOfCol) && (!style.useBaselineGrid()) && (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
					{
						QChar ch = itemText.text(curLine.firstItem);
						double firstasce = style.lineSpacing();
						double currasce = 0;
						if (itemText.object(curLine.firstItem) != 0)
							currasce = QMAX(currasce, (itemText.object(curLine.firstItem)->gHeight + itemText.object(curLine.firstItem)->lineWidth()) * (itemText.charStyle(curLine.firstItem).scaleV() / 1000.0));
						else //if (itemText.charStyle(curLine.firstItem).effects() & ScStyle_DropCap == 0)
							currasce = itemText.charStyle(curLine.firstItem).font().height(itemText.charStyle(curLine.firstItem).fontSize() / 10.0);
						for (int zc = 0; zc < itemsInLine; ++zc)
						{
							QChar ch = itemText.text(curLine.firstItem + zc);
							if ((ch == SpecialChars::TAB) || (ch == QChar(10))
								|| (ch == SpecialChars::PARSEP) || (ch == QChar(24))
								|| (ch == QChar(26)) || (ch == QChar(27))
								|| (ch == QChar(28)) || (ch == QChar(29)))
								continue;
							if (itemText.object(curLine.firstItem + zc) != 0)
								currasce = QMAX(currasce, (itemText.object(curLine.firstItem + zc)->gHeight + itemText.object(curLine.firstItem + zc)->lineWidth()) * (itemText.charStyle(curLine.firstItem + zc).scaleV() / 1000.0));
							else //if (itemText.charStyle(curLine.firstItem + zc).effects() & ScStyle_DropCap == 0)
								currasce = QMAX(currasce, itemText.charStyle(curLine.firstItem + zc).font().height(itemText.charStyle(curLine.firstItem + zc).fontSize() / 10.0));
						}
							
						double adj = firstasce - currasce;
						curLine.ascent = currasce;
						curLine.y -= adj;
						CurY -= adj;
					}
					if ( itemText.charStyle(curLine.firstItem).effects() & ScStyle_DropCap)
					{
						// put line back to top
						curLine.y -= DropCapDrop;
						itemText.item(curLine.firstItem)->glyph.yoffset += DropCapDrop;
					}
				}
				
				StartOfCol = false;
/* has no effect except setting wide, asce and desc
 				uint loopC = BuPos3;
				if (m_Doc->guidesSettings.showControls)
					loopC++;  // ??? AV
				for (int zc = 0; zc < loopC; ++zc)
				{
					if ( zc >= itemsInLine) {
						qDebug("layout: zc too large %d / %d", zc, itemsInLine);
						continue;
					}
					if ( startLin + zc >= itemText.length()) {
						qDebug("layout: startLin+zc too large %d+%d / %d", startLin, zc, itemText.length());
						continue;
					}
//					else
//						qDebug("layout: startLin, zc ok. %d, %d, %d", startLin, zc, LiList.count());
					double wide2 = 0;
					double xcoZli = itemText.item(curLine.firstItem + zc)->glyph.xoffset;
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
//				LiList.clear();
				curLine.lastItem = a;
				itemText.appendLine(curLine);
				itemsInLine = 0;
				curLine.firstItem = a+1;
				BuPos = 0;
				LastSP = 0;
				LastXp = 0;
				outs = false;
				if (goNoRoom)
				{
					goNoRoom = false;
					nrc = a+1;
//					qDebug(QString("no room C: %1").arg(a+1));
					goto NoRoom;
				}
				if (goNextColumn)
				{
					goNextColumn = false;
					StartOfCol = true;
					CurrCol++;
					if (CurrCol < Cols)
					{
						ColWidth = columnWidth();
						ColBound = FPoint((ColWidth + ColGap) * CurrCol + Extra+lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
						CurX = ColBound.x();
						ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
					}
					else
					{
						nrc = a;
//						qDebug(QString("no room D: %1").arg(a));
						goto NoRoom;
					}
				}
			}
		}
// end of itemText
		uint a = itemText.length()-1;
		curLine.lastItem = a;
		if (style.alignment() != 0)
		{
			EndX = floor(QMAX(curLine.x, CurX - 10));
			do {
				EndX += 0.125;
				pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
				pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
			} while ((cl.contains(pf2.xForm(pt1))) 
					 && (cl.contains(pf2.xForm(pt2))) 
					 && (EndX+RExtra+lineCorr < ColBound.y() - style.rightMargin()));
				
			if (opticalMargins && itemsInLine > 0) {
				double chs = itemText.charStyle(a).fontSize() * (itemText.charStyle(a).scaleH() / 1000.0);
				QString chr = itemText.item(a)->glyph.more ? "-" : itemText.text(a,1);
				double rightCorr = itemText.charStyle(a).font().realCharWidth(chr[0], chs / 10.0);
				if (QString("-,.`´'~").find(chr) >= 0
					|| chr[0] == QChar(0x2018)
					|| chr[0] == QChar(0x2019)
					|| chr[0] == QChar(0x201a)
					|| chr[0] == QChar(0x201b)
					|| chr[0] == QChar(0x2039)
					|| chr[0] == QChar(0x203a)
					|| chr[0] == QChar(0x2032)
					)
					rightCorr *= 0.7;
				else if (QString(";:\"").find(chr) >= 0
						 || chr[0] == QChar(0x00ab)
						 || chr[0] == QChar(0x00bb)
						 || chr[0] == QChar(0x201c)
						 || chr[0] == QChar(0x201d)
						 || chr[0] == QChar(0x201e)
						 || chr[0] == QChar(0x201f)
						 || chr[0] == QChar(0x2013)
						 || chr[0] == QChar(0x2033)
						 )
					rightCorr *= 0.5;
				else {
					rightCorr = itemText.charStyle(a).font().charWidth(chr[0], chs / 10.0);
					rightCorr -= itemText.charStyle(a).font().charWidth(chr[0], chs / 10.0, QChar('.'));
				}
				EndX += rightCorr;
//				qDebug(QString("orm %1 @ %2: %3 par").arg(rightCorr).arg(a).arg(chr));
			}
			curLine.naturalWidth = CurX - curLine.x;
			curLine.width = EndX - curLine.x;
			if (style.alignment() == 2)
				OFs = EndX - CurX;
			if (style.alignment() == 1)
				OFs = (EndX - CurX) / 2;
			if (style.alignment() == 3)
				OFs = 0;
			if (style.alignment() == 4)
			{
				aSpa = 0;
				for (int sof = 0; sof < itemsInLine; ++sof)
				{
					if ((itemText.item(curLine.firstItem + sof)->ch[0] == QChar(32)
						 || itemText.item(curLine.firstItem + sof)->ch[0] == SpecialChars::NBSPACE)
						&& ! (itemText.item(curLine.firstItem + sof)->effects() & ScStyle_SuppressSpace)
						 )
						aSpa++;
				}
				if (aSpa != 0)
					OFs2 = (EndX - CurX) / aSpa;
				else
					OFs2 = 0;
				OFs = 0;
				for (int yof = 0; yof < itemsInLine; ++yof)
				{
					if ((itemText.item(curLine.firstItem + yof)->ch[0] == QChar(32) ||
						 itemText.item(curLine.firstItem + yof)->ch[0] == QChar(29))
						&& ! (itemText.item(curLine.firstItem + yof)->effects() & ScStyle_SuppressSpace)
						)
					{
						itemText.item(curLine.firstItem + yof)->glyph.last()->xadvance += OFs2;
						OFs += OFs2;
					}
				}
			}
			else
			{
				curLine.x += OFs;
			}
		}
		else {
			curLine.naturalWidth = CurX - curLine.x;
			curLine.width = CurX - curLine.x;
		}
		if (itemsInLine != 0)
		{
			if ((!AbsHasDrop) && (StartOfCol) && (!style.useBaselineGrid()))
			{
				double firstasce = itemText.charStyle(curLine.firstItem).font().ascent(itemText.charStyle(curLine.firstItem).fontSize() / 10.0);
				double currasce = 0;
				if ((itemText.text(curLine.firstItem) == QChar(13)) || (itemText.text(curLine.firstItem) == QChar(28)))
					currasce = itemText.charStyle(curLine.firstItem).font().ascent(itemText.charStyle(curLine.firstItem).fontSize() / 10.0);
				else if (itemText.object(curLine.firstItem) != 0)
					currasce = QMAX(currasce, (itemText.object(curLine.firstItem)->gHeight + itemText.object(curLine.firstItem)->lineWidth()) * (itemText.charStyle(curLine.firstItem).scaleV() / 1000.0));
				else //if (itemText.charStyle(curLine.firstItem).effects() & ScStyle_DropCap == 0)
					currasce = itemText.charStyle(curLine.firstItem).font().realCharAscent(itemText.text(curLine.firstItem), itemText.charStyle(curLine.firstItem).fontSize() / 10.0);
				for (int zc = 0; zc < itemsInLine; ++zc)
				{
					if ((itemText.text(curLine.firstItem+zc) == SpecialChars::TAB) || (itemText.text(curLine.firstItem+zc) == QChar(10))
						|| (itemText.text(curLine.firstItem+zc) == SpecialChars::PARSEP) || (itemText.text(curLine.firstItem+zc) == SpecialChars::NBHYPHEN)
						|| (itemText.text(curLine.firstItem+zc) == SpecialChars::FRAMEBREAK) || (itemText.text(curLine.firstItem+zc) == SpecialChars::COLBREAK)
						|| (itemText.text(curLine.firstItem+zc) == SpecialChars::LINEBREAK) || (itemText.text(curLine.firstItem+zc) == SpecialChars::NBSPACE))
						continue;
					if (itemText.object(curLine.firstItem+zc) != 0)
						currasce = QMAX(currasce, (itemText.object(curLine.firstItem+zc)->gHeight + itemText.object(curLine.firstItem+zc)->lineWidth()) * itemText.charStyle(curLine.firstItem+zc).scaleV() / 1000.0);
					else //if (itemText.charStyle(curLine.firstItem+zc).effects() & ScStyle_DropCap == 0)
						currasce = QMAX(currasce, itemText.charStyle(curLine.firstItem+zc).font().realCharAscent(itemText.text(curLine.firstItem+zc), itemText.charStyle(curLine.firstItem+zc).fontSize() / 10.0));
				}
				double adj = firstasce - currasce;
				curLine.ascent = currasce;
				curLine.y -= adj;
				CurY -= adj;
			}
			else if ((!StartOfCol) && (!style.useBaselineGrid()) && (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
			{
				double firstasce = style.lineSpacing();
				double currasce = 0;
				if (itemText.object(curLine.firstItem) != 0)
					currasce = QMAX(currasce, (itemText.object(curLine.firstItem)->gHeight + itemText.object(curLine.firstItem)->lineWidth()) * (itemText.charStyle(curLine.firstItem).scaleV() / 1000.0));
				else //if (itemText.charStyle(curLine.firstItem).effects() & ScStyle_DropCap == 0)
					currasce = itemText.charStyle(curLine.firstItem).font().height(itemText.charStyle(curLine.firstItem).fontSize() / 10.0);
				for (int zc = 0; zc < itemsInLine; ++zc)
				{
					if ((itemText.text(curLine.firstItem+zc) == SpecialChars::TAB) || (itemText.text(curLine.firstItem+zc) == QChar(10))
						|| (itemText.text(curLine.firstItem+zc) == QChar(13)) || (itemText.text(curLine.firstItem+zc) == QChar(24))
						|| (itemText.text(curLine.firstItem+zc) == QChar(26)) || (itemText.text(curLine.firstItem+zc) == QChar(27))
						|| (itemText.text(curLine.firstItem+zc) == QChar(28)) || (itemText.text(curLine.firstItem+zc) == QChar(29)))
						continue;
					if (itemText.object(curLine.firstItem+zc) != 0)
						currasce = QMAX(currasce, (itemText.object(curLine.firstItem+zc)->gHeight + itemText.object(curLine.firstItem+zc)->lineWidth()) * (itemText.charStyle(curLine.firstItem+zc).scaleV() / 1000.0));
					else //if (itemText.charStyle(curLine.firstItem+zc).effects() & ScStyle_DropCap == 0)
						currasce = QMAX(currasce, itemText.charStyle(curLine.firstItem+zc).font().height(itemText.charStyle(curLine.firstItem+zc).fontSize() / 10.0));
				}
				double adj = firstasce - currasce;
				curLine.ascent = currasce;
				curLine.y -= adj;
				CurY -= adj;
			}
			if ( itemText.charStyle(curLine.firstItem).effects() & ScStyle_DropCap )
			{
				// put line back to top
				curLine.y -= DropCapDrop;
				itemText.item(curLine.firstItem)->glyph.yoffset += DropCapDrop;
			}
		}
		StartOfCol = false;
		goNextColumn = false;
		itemText.appendLine(curLine);
		itemsInLine = 0;
		curLine.firstItem = a + 1;
		BuPos = 0;
		LastSP = 0;
		outs = false;
	}
	MaxChars = itemText.length();
	invalid = false;
	pf2.end();
	if (NextBox != NULL) {
		NextBox->invalid = true;
		dynamic_cast<PageItem_TextFrame*>(NextBox)->firstChar = MaxChars;
	}
//	qDebug("textframe: len=%d, done relayout", itemText.length());
	return;
			
NoRoom:     
	pf2.end();
	MaxChars = nrc;
	invalid = false;
	PageItem_TextFrame * next = dynamic_cast<PageItem_TextFrame*>(NextBox);
	if (next != 0)
	{
		next->invalid = true;
		next->firstChar = nrc;
		if (uint(CPos) > nrc)
		{
			int nCP = CPos;
//			CPos = nrc;
			if ((m_Doc->appMode == modeEdit) && (Tinput))
			{
				//							OwnPage->Deselect(true);
				next->CPos = QMAX(nCP, signed(nrc));
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


void PageItem_TextFrame::DrawObj_Item(ScPainter *p, QRect e, double sc)
{
	layout();
	if (invalid)
		return;
	
	ScribusView* view = m_Doc->view();
	QPainter pf2;
	QPoint pt1, pt2;
	FPoint ColBound;
	QRegion cm;
	int CurrCol;
	double wide, lineCorr, ColWidth;
	QString chstr;
	ScText *hl;

	QValueList<ParagraphStyle::TabRecord> tTabValues;
	double desc, asce, maxDY, tabDist;
	tTabValues.clear();
	p->save();
	pf2.begin(view->viewport());
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
		p->setupPolygon(&PoLine);
		p->setClipPath();
		p->save();
		p->scale(LocalScX, LocalScY);
		p->translate(static_cast<int>(LocalX*LocalScX), static_cast<int>(LocalY*LocalScY));
		if (pixm.width() > 0 && pixm.height() > 0) {
			QImage img(pixm.qImage());
			p->drawImage(&img);
		}
		p->restore();
	}
	if (itemText.length() != 0)
	{
//		qDebug("drawing textframe: len=%d", itemText.count());
		if (imageFlippedH())
		{
			p->translate(Width * sc, 0);
			p->scale(-1, 1);
		}
		if (imageFlippedV())
		{
			p->translate(0, Height * sc);
			p->scale(1, -1);
		}
		uint tabCc = 0;
		assert( firstInFrame() >= 0 );
		assert( lastInFrame() < itemText.length() );
		for (uint ll=0; ll < itemText.lines(); ++ll) {
			LineSpec ls = itemText.line(ll);
			tabDist = ls.x;
			double CurX = ls.x;
//			p->setLineWidth(0);
//			p->setBrush(Qt::yellow);
//			p->drawRect(ls.x, ls.y-ls.ascent, ls.width, ls.ascent);
//			p->setBrush(Qt::red);
//			p->drawRect(ls.x, ls.y, ls.width, ls.descent);
			
			for (int a = ls.firstItem; a <= ls.lastItem; ++a)
			{
				hl = itemText.item(a);
				const CharStyle& charStyle(itemText.charStyle(a));
				const ParagraphStyle& style(itemText.paragraphStyle(a));
				tTabValues = style.tabValues();
				double chs = charStyle.fontSize() * hl->glyph.scaleV;
				bool selected = itemText.selected(a);
				if (charStyle.effects() & ScStyle_StartOfLine)
					tabCc = 0;
				chstr = hl->ch;
				if (hl->glyph.glyph == 0)
					continue;
				if (charStyle.fillColor() != CommonStrings::None)
				{
					QColor tmp;
					SetFarbe(&tmp, charStyle.fillColor(), charStyle.fillShade());
					p->setBrush(tmp);
				}
				if (charStyle.strokeColor() != CommonStrings::None)
				{
					QColor tmp;
					SetFarbe(&tmp, charStyle.strokeColor(), charStyle.strokeShade());
					p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
				}
				if (charStyle.effects() & ScStyle_DropCap)
				{
					if (style.useBaselineGrid())
						chs = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (style.dropCapLines()-1) + (charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / charStyle.font().realCharHeight(chstr[0], 10)));
					else
					{
						if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
							chs = qRound(10 * ((style.lineSpacing() * (style.dropCapLines()-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / charStyle.font().realCharHeight(chstr[0], 10)));
						else
						{
							double currasce = charStyle.font().height(style.charStyle().fontSize() / 10.0);
							chs = qRound(10 * ((currasce * (style.dropCapLines()-1)+(charStyle.font().ascent(style.charStyle().fontSize() / 10.0))) / charStyle.font().realCharHeight(chstr[0], 10)));
						}
					}
				}
				// tabs & fill chars
				if ((chstr == SpecialChars::TAB) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
				{
					QChar tabFillChar(tTabValues[tabCc].tabFillChar);
					double wt = charStyle.font().charWidth(tabFillChar, chs / 10.0);
					int coun = static_cast<int>((CurX - tabDist) / wt);
					double sPos = tabDist - CurX + hl->glyph.xoffset + 1;
					desc = -charStyle.font().descent(chs / 10.0);
					asce = charStyle.font().ascent(chs / 10.0);
					GlyphLayout tglyph;
					tglyph.glyph = tabFillChar.unicode();
					tglyph.yoffset = hl->glyph.yoffset;
					tglyph.scaleV = tglyph.scaleH = chs / charStyle.fontSize();
					tglyph.xadvance = wt;
					p->save();
					for (int cx = 0; cx < coun; ++cx)
					{
						tglyph.xoffset =  sPos + wt * cx;
						if (e2.intersects(pf2.xForm(QRect(qRound(CurX + tglyph.xoffset),qRound(ls.y + tglyph.yoffset-asce), qRound(tglyph.xadvance+1), qRound(asce+desc)))))
							drawGlyphs(p, charStyle, tglyph);
					}
					p->restore();
				}
				if (chstr[0] == SpecialChars::TAB)
					tabCc++;
				// paint selection
				if (!m_Doc->RePos)
				{
					double xcoZli = CurX + hl->glyph.xoffset;
					desc = - charStyle.font().descent(charStyle.fontSize() / 10.0);
					asce = charStyle.font().ascent(charStyle.fontSize() / 10.0);
					if ((selected && Select) || ((NextBox != 0 || BackBox != 0) && selected) && (m_Doc->appMode == modeEdit))
					{
						wide = hl->glyph.xadvance;
						p->setFillMode(1);
						//							p->setBrush(darkBlue);
						p->setBrush(qApp->palette().color(QPalette::Active, QColorGroup::Highlight));
						p->setLineWidth(0);
						if ((a > 0) && (QChar(hl->glyph.glyph) == SpecialChars::TAB))
						{
							xcoZli = CurX + itemText.item(a-1)->glyph.xoffset + itemText.charStyle(a-1).font().charWidth(itemText.text(a-1), itemText.charStyle(a-1).fontSize() / 10.0);
							wide = CurX + hl->glyph.xoffset - xcoZli + hl->glyph.xadvance;
						}
						if (!m_Doc->RePos)
							p->drawRect(xcoZli, qRound(ls.y + hl->glyph.yoffset - asce * hl->glyph.scaleV), wide+1, qRound((asce+desc) * (hl->glyph.scaleV)));
						p->setBrush(qApp->palette().color(QPalette::Active, QColorGroup::HighlightedText));
						//							p->setBrush(white);
					}
					if (charStyle.strokeColor() != CommonStrings::None)
					{
						QColor tmp;
						SetFarbe(&tmp, charStyle.strokeColor(), charStyle.strokeShade());
						p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
					}
/* see drawGlyphs()
                if (((chstr[0] == QChar(13)) || (chstr[0] == QChar(28))) && (m_Doc->guidesSettings.showControls))
				{
					if (e2.intersects(pf2.xForm(QRect(qRound(hl->glyph.xoffset + hl->glyph.xadvance),qRound(hl->glyph.yoffset - asce), qRound(hl->glyph.xadvance+1), qRound(asce+desc)))))
					{
						FPointArray points;
						double ytrans, xtrans;
						if (chstr[0] == QChar(13))
						{
							points = m_Doc->symReturn.copy();
							if (a > 0)
								ytrans = itemText.item(a-1)->glyph.yoffset-((charStyle.fontSize() / 10.0) * 0.8);
							else
								ytrans = Zli3.yco - style.lineSpacing()-((charStyle.fontSize() / 10.0) * 0.8);
						}
						else
						{
							points = m_Doc->symNewLine.copy();
							if (a > 0)
								ytrans = itemText.item(a-1)->glyph.yoffset-((charStyle.fontSize() / 10.0) * 0.4);
							else
								ytrans = hl->glyph.yoffset - style.lineSpacing()-((charStyle.fontSize() / 10.0) * 0.4);
						}
						if (charStyle.effects() & ScStyle_StartOfLine)
							xtrans = hl->glyph.xoffset;
						else
						{
							if (a > 0)
								xtrans = itemText.item(a-1)->glyph.xoffset+ itemText.charStyle(a-1).font().charWidth(itemText.text(a-1), itemText.charStyle(a-1).fontSize() / 10.0);
							else
								xtrans = hl->glyph.xoffset;
						}
						QWMatrix chma, chma2, chma4, chma5;
						chma4.translate(xtrans, ytrans);
						chma.scale(Zli3.Siz / 100.0, Zli3.Siz / 100.0);
						chma2.scale(Zli3.scale / 1000.0, Zli3.scalev / 1000.0);
						chma5.scale(p->zoomFactor(), p->zoomFactor());
						points.map(chma * chma2 * chma4 * chma5);
						p->setupTextPolygon(&points);
						p->setFillMode(1);
						p->fillPath();
					}
				}
*/
					// paint glyphs
					if (e2.intersects(pf2.xForm(QRect(qRound(CurX + hl->glyph.xoffset),qRound(ls.y + hl->glyph.yoffset-asce), qRound(hl->glyph.xadvance+1), qRound(asce+desc)))))
					{
						p->save();
						p->translate(CurX * p->zoomFactor(), ls.y * p->zoomFactor());
						if (hl->ch[0] == SpecialChars::OBJECT) {
							DrawObj_Embedded(p, e, charStyle, hl->cembedded);
							CurX += (hl->cembedded->gWidth + hl->cembedded->lineWidth()) *  p->zoomFactor();
						}
						else {
							drawGlyphs(p, charStyle, hl->glyph);
							CurX += hl->glyph.wide();
						}
						p->restore();
					}
/*
 see layout()   if (charStyle.effects() & ScStyle_SmartHyphenVisible)
				{
					Zli3.Zeich = "-";
					Zli3.xco = Zli3.xco + Zli3.wide;
					if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
						DrawZeichenS(p, &Zli3);
				}
 */			
				}
				tabDist = CurX;
			}
		}
		/*
		if (itemText.count() > MaxChars)
		{//CB && added here for jghali prior to commit access
		//Draw the overflow icon
			if (!m_Doc->RePos && !ScMW->view->previewMode)
				drawOverflowMarker(p);
		}
		*/
	}
	else {
//		qDebug("skipping textframe: len=%d", itemText.count());
	}

	pf2.end();
	p->restore();
}


void PageItem_TextFrame::DrawObj_Post(ScPainter *p)
{
	ScribusView* view = m_Doc->view();
	if (m_Doc->layerOutline(LayerNr))
	{
		p->setPen(m_Doc->layerMarker(LayerNr), 1, SolidLine, FlatCap, MiterJoin);
		p->setFillMode(ScPainter::None);
		p->setBrushOpacity(1.0);
		p->setPenOpacity(1.0);
		p->setupPolygon(&PoLine);
		p->strokePath();
	}
	else
	{
#ifdef HAVE_CAIRO
#if CAIRO_VERSION > CAIRO_VERSION_ENCODE(1, 1, 6)
		if (fillBlendmode() != 0)
			p->endLayer();
#endif
#endif
		if (!m_Doc->RePos)
		{
#ifdef HAVE_CAIRO
#if CAIRO_VERSION > CAIRO_VERSION_ENCODE(1, 1, 6)
			if (lineBlendmode() != 0)
				p->beginLayer(1.0 - lineTransparency(), lineBlendmode());
#endif
#endif
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
								static_cast<PenStyle>(ml[it].Dash),
								static_cast<PenCapStyle>(ml[it].LineEnd),
								static_cast<PenJoinStyle>(ml[it].LineJoin));
						p->strokePath();
					}
				}
			}
#ifdef HAVE_CAIRO
#if CAIRO_VERSION > CAIRO_VERSION_ENCODE(1, 1, 6)
			if (lineBlendmode() != 0)
				p->endLayer();
#endif
#endif
		}
	}
	if ((!isEmbedded) && (!m_Doc->RePos))
	{
		double scpInv = 1.0 / (QMAX(view->scale(), 1));
		if ((Frame) && (m_Doc->guidesSettings.framesShown) && ((itemType() == ImageFrame) || (itemType() == TextFrame) || (itemType() == PathText)))
		{
			p->setPen(PrefsManager::instance()->appPrefs.DFrameNormColor, scpInv, DotLine, FlatCap, MiterJoin);
			if ((isBookmark) || (m_isAnnotation))
				p->setPen(PrefsManager::instance()->appPrefs.DFrameAnnotationColor, scpInv, DotLine, FlatCap, MiterJoin);
			if ((BackBox != 0) || (NextBox != 0))
				p->setPen(PrefsManager::instance()->appPrefs.DFrameLinkColor, scpInv, SolidLine, FlatCap, MiterJoin);
			if (m_Locked)
				p->setPen(PrefsManager::instance()->appPrefs.DFrameLockColor, scpInv, SolidLine, FlatCap, MiterJoin);

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
					for (uint a = 1; a < Clip.size(); ++a)
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
			p->setPen(lightGray, scpInv, DotLine, FlatCap, MiterJoin);
			p->setupPolygon(&ContourLine);
			p->strokePath();
		}

		//Draw the overflow icon
		if (frameOverflows())
		{//CB && added here for jghali prior to commit access
			if (!view->previewMode)
				drawOverflowMarker(p);
		}
		if (m_Doc->guidesSettings.colBordersShown && !view->previewMode)
			drawColumnBorders(p);
		if ((m_Doc->guidesSettings.layerMarkersShown) && (m_Doc->layerCount() > 1) && (!m_Doc->layerOutline(LayerNr)))
		{
			p->setPen(black, 0.5/ m_Doc->view()->scale(), SolidLine, FlatCap, MiterJoin);
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
	Tinput = false;
	FrameOnly = false;
	p->restore();
}


void PageItem_TextFrame::clearContents()
{
	PageItem *nextItem = this;
	while (nextItem != 0)
	{
		if (nextItem->BackBox != 0)
			nextItem = nextItem->BackBox;
		else
			break;
	}
	while (nextItem != 0)
	{
		nextItem->CPos = 0;
		nextItem->itemText.clear();
		nextItem->invalid = true;
		nextItem = nextItem->NextBox;
	}
}

void PageItem_TextFrame::handleModeEditKey(QKeyEvent *k, bool& keyRepeat)
{
	int oldPos = CPos; // 15-mar-2004 jjsa for cursor movement with Shift + Arrow key
	int kk = k->key();
	int as = k->ascii();
	double altx, alty;
	QString uc = k->text();
	QString cr, Tcha, Twort;
	uint Tcoun;
	int len, pos, c;
	int KeyMod;
	ScribusView* view = m_Doc->view();
	ButtonState buttonState = k->state();
	switch (buttonState)
	{
	case ShiftButton:
		KeyMod = SHIFT;
		break;
	case AltButton:
		KeyMod = ALT;
		break;
	case ControlButton:
		KeyMod = CTRL;
		break;
	default:
		KeyMod = 0;
		break;
	}

	view->slotDoCurs(false);
	switch (kk)
	{
	case Key_Prior:
	case Key_Next:
	case Key_End:
	case Key_Home:
	case Key_Right:
	case Key_Left:
	case Key_Up:
	case Key_Down:
		if ( (buttonState & ShiftButton) == 0 )
			deselectAll();
	}
	//<< ISO 14755
	//if ((buttonState & ControlButton) && (buttonState & ShiftButton))
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
				itemText.insertChars(CPos, QString(QChar(conv)));
				CPos += 1;
				Tinput = true;
				m_Doc->scMW()->setTBvals(this);
				view->RefreshItem(this);
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
	case Key_F12:
		unicodeTextEditMode = true;
		unicodeInputCount = 0;
		unicodeInputString = "";
		keyRepeat = false;
		return;
		break;
	case Key_Home:
		// go to begin of line
		if ( (pos = CPos) == firstInFrame() )
			break; // at begin of frame
		len = lastInFrame();
		if ( pos == len )
			pos--;
		if ( (buttonState & ControlButton) == 0 )
		{
			alty =  itemText.item(pos)->glyph.yoffset;
			c = itemText.text(pos);
			if ( c == 13 ) // new line, position is wrong
				if ( --pos > firstInFrame() )
					alty =  itemText.item(pos)->glyph.yoffset;
			// check for yp at actual position
			if ( pos < len )
			{
				altx =  itemText.item(pos)->glyph.yoffset;
				if ( altx > alty )
				{
					// we were at begin of line
					break;
				}
			}
			while (  pos > firstInFrame() && itemText.item(pos-1)->glyph.yoffset == alty )
				--pos;
			if ( itemText.text(pos) == SpecialChars::PARSEP )
				++pos;
		}
		else
		{
			//Control Home for start of frame text
			pos=firstInFrame();
		}
		CPos = pos;
		if ( buttonState & ShiftButton )
			ExpandSel(-1, oldPos);
		if ( this->itemText.lengthOfSelection() > 0 )
			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Key_End:
		// go to end of line
		len = lastInFrame();
		if ( CPos >= len )
			break; // at end of frame
		if ( (buttonState & ControlButton) == 0 )
		{
			if ((CPos < len) && ((itemText.text(CPos) == SpecialChars::PARSEP) 
								 || (itemText.text(CPos) == SpecialChars::LINEBREAK)))
			{
				// at end of paragraph and therefore line
				break;
			}
			QString nextCh = itemText.text(CPos,1);
			int nextChs = itemText.charStyle(CPos).fontSize();
			GlyphLayout dummy;
			alty =  itemText.item(CPos)->glyph.yoffset - layoutGlyphs(itemText.charStyle(CPos), nextCh, dummy);
			double nextY;
			while (CPos < len-1)
			{
				nextCh = itemText.text(CPos+1, 1);
				nextChs = itemText.charStyle(CPos+1).fontSize();
				GlyphLayout dummy;
				nextY = itemText.item(CPos+1)->glyph.yoffset - layoutGlyphs(itemText.charStyle(CPos+1), nextCh, dummy);
				if (fabs(nextY - alty) > 1.0)
					break;
				CPos++;
				if ( CPos == len-1)
					break;
			}
			if ( CPos < len -1 )
				c = itemText.text(CPos+1);
			else if ( CPos == len - 1 )
				c = 13;
			else
				c = 0;
			//CB 2740: Apart from not knowing what 28 did in 1.2.1, this forces us to go to
			//the next line, not the end of the current
			//if (( c == 13 ) || (c = 28))
			//CB 2995 13 allows end to be pressed and actually get to the last character
			//28 must be end of line not at end of para
			if ( c == 13 )
				CPos++;
		}
		else
		{
			//Control End for end of frame text
			pos=lastInFrame();
			CPos = pos;
		}
		if ( buttonState & ShiftButton )
			ExpandSel(1, oldPos);
		if ( this->itemText.lengthOfSelection() > 0 )
			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Key_Down:
		if (buttonState & ControlButton)
		{
			// go to end of paragraph
			len = itemText.length();
			if ( CPos >= lastInFrame() )
				break; // at end of frame
			if ( itemText.text(CPos) == SpecialChars::PARSEP )
			{
				break;
			}
			pos = CPos;
			while ( pos < lastInFrame() )
			{
				if ( itemText.text(pos) == SpecialChars::PARSEP )
					break;
				else
					++pos;
			}
			CPos = pos;
			if ( buttonState & ShiftButton )
				ExpandSel(1, oldPos);
		}
		else
		{
			if (CPos != lastInFrame())
			{
				bool down1=false;
				double newy;
				newy = alty = itemText.item(CPos)->glyph.yoffset;
				altx = itemText.item(CPos)->glyph.xoffset;
				do
				{
					++CPos;
					if (CPos == lastInFrame())
						break;
					//CB #3088 Catch some funny empty lines. still testing, currently causes to go to start/end of lines incorrectly
					//if ((CPos < static_cast<int>(itemText.count())-1) && itemText.at(CPos+1)->ch[0].digitValue()==-1 && itemText.at(CPos)->ch[0].digitValue()==-1 && itemText.at(CPos)->glyph.xoffset == altx && itemText.at(CPos)->glyph.yoffset == alty)
					//	break;
					if (itemText.item(CPos)->glyph.yoffset > alty)
					{
						if (down1 && itemText.item(CPos)->glyph.yoffset > newy)
						{
							--CPos;
							break;
						}
						if (itemText.item(CPos)->glyph.xoffset >= altx)
							break;
						down1=true;
					}
					//CB Make the cursor move forward a column
					if (itemText.item(CPos)->glyph.yoffset < alty)
					{
						double newX=altx+columnWidth();
						while (itemText.item(CPos)->glyph.xoffset<newX && CPos < lastInFrame()-1)
							++CPos;
						break;
					}
					newy=itemText.item(CPos)->glyph.yoffset;
				}
				while (CPos < lastInFrame());
				if ( buttonState & ShiftButton )
				{
					if ( buttonState & AltButton )
						CPos = lastInFrame();
					ExpandSel(1, oldPos);
				}
				else
					if (CPos == lastInFrame())
						if (NextBox != 0)
						{
							if (NextBox->frameDisplays(CPos+1))
							{
								view->Deselect(true);
								NextBox->CPos = CPos+1;
								view->SelectItemNr(NextBox->ItemNr);
								//currItem = currItem->NextBox;
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
						//currItem = currItem->NextBox;
					}
				}
			}
		}
		if ( this->itemText.lengthOfSelection() > 0 )
			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Key_Up:
		if (buttonState & ControlButton)
		{
			if ( (pos = CPos) == firstInFrame() )
				break; // at begin of frame
			len = itemText.length();
			if ( pos == len )
				pos--;
			// paragraph begin
			if ( pos < len &&
				 itemText.text(pos) == SpecialChars::PARSEP )
				--pos;
			while (pos > firstInFrame()) {
				if ( itemText.text(pos) == SpecialChars::PARSEP )
				{
					++pos;
					break;
				}
				else
					--pos;
			}
			CPos = pos;
			if ( buttonState & ShiftButton )
				ExpandSel(-1, oldPos);
		}
		else
		{
			if (CPos > firstInFrame())
			{
				if (CPos == lastInFrame())
					--CPos;
				alty = itemText.item(CPos)->glyph.yoffset;
				altx = itemText.item(CPos)->glyph.xoffset;
				if (CPos > firstInFrame())
				{
					do
					{
						--CPos;
						if (CPos == firstInFrame())
							break;
						if  ( itemText.text(CPos) == SpecialChars::PARSEP )
							break;
						if (itemText.item(CPos)->glyph.yoffset < alty)
						{
							if (itemText.item(CPos)->glyph.xoffset <= altx)
								break;
						}
						//CB Make the cursor move back a column
						if (itemText.item(CPos)->glyph.yoffset > alty)
						{
							double newX=altx-columnWidth();
							while (itemText.item(CPos)->glyph.xoffset>newX && CPos > firstInFrame())
								--CPos;
							break;
						}
					}
					while (CPos > firstInFrame());
				}
				if ( buttonState & ShiftButton )
				{
					if ( buttonState & AltButton )
						CPos = firstInFrame();
					ExpandSel(-1, oldPos);
				}
				else
					if (CPos == firstInFrame())
					{
						if (BackBox != 0)
						{
							view->Deselect(true);
							BackBox->CPos = BackBox->lastInFrame();
							view->SelectItemNr(BackBox->ItemNr);
							//currItem = currItem->BackBox;
						}
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
					//currItem = currItem->BackBox;
				}
			}
		}
		if ( this->itemText.lengthOfSelection() > 0 )
			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Key_Prior:
		CPos = firstInFrame();
		if ( buttonState & ShiftButton )
			ExpandSel(-1, oldPos);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Key_Next:
		CPos = lastInFrame();
		if ( buttonState & ShiftButton )
			ExpandSel(1, oldPos);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Key_Left:
		if ( buttonState & ControlButton )
		{
			setNewPos(oldPos, itemText.length(), -1);
			if ( buttonState & ShiftButton )
				ExpandSel(-1, oldPos);
		}
		else if ( buttonState & ShiftButton )
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
		if ((CPos > 0) && (CPos == lastInFrame()))
		{
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
		if ( itemText.lengthOfSelection() > 0 )
			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Key_Right:
		if ( buttonState & ControlButton )
		{
			setNewPos(oldPos, itemText.length(), 1);
			if ( buttonState & ShiftButton )
				ExpandSel(1, oldPos);
		}
		else if ( buttonState & ShiftButton )
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
			view->RefreshItem(this);
		m_Doc->scMW()->setTBvals(this);
		break;
	case Key_Delete:
		if (CPos == itemText.length())
		{
			if (itemText.lengthOfSelection() > 0)
			{
				deleteSelectedTextFromFrame();
				m_Doc->scMW()->setTBvals(this);
				view->RefreshItem(this);
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
		Tinput = false;
		if ((cr == QChar(13)) && (itemText.length() != 0))
		{
//			m_Doc->chAbStyle(this, findParagraphStyle(m_Doc, itemText.paragraphStyle(QMAX(CPos-1,0))));
			Tinput = false;
		}
		m_Doc->scMW()->setTBvals(this);
		view->RefreshItem(this);
		break;
	case Key_Backspace:
		if (CPos == 0)
		{
			if (itemText.lengthOfSelection() > 0)
			{
				deleteSelectedTextFromFrame();
				m_Doc->scMW()->setTBvals(this);
				view->RefreshItem(this);
			}
			break;
		}
		if (itemText.length() == 0)
			break;
		cr = itemText.text(QMAX(CPos-1,0),1);
		if (itemText.lengthOfSelection() == 0)
		{
			--CPos;
			itemText.select(CPos, 1, true);
		}
		deleteSelectedTextFromFrame();
		Tinput = false;
		if ((cr == QChar(13)) && (itemText.length() != 0))
		{
//			m_Doc->chAbStyle(this, findParagraphStyle(m_Doc, itemText.paragraphStyle(QMAX(CPos-1,0))));
			Tinput = false;
		}
		m_Doc->scMW()->setTBvals(this);
		view->RefreshItem(this);
		break;
	default:
		if ((itemText.lengthOfSelection() > 0) && (kk < 0x1000))
			deleteSelectedTextFromFrame();
		//if ((kk == Key_Tab) || ((kk == Key_Return) && (buttonState & ShiftButton)))
		if (kk == Key_Tab)
		{
			itemText.insertChars(CPos, QString(SpecialChars::TAB));
			CPos += 1;
			Tinput = true;
			view->RefreshItem(this);
			break;
		}
		if ((uc[0] > QChar(31) && m_Doc->currentStyle.charStyle().font().canRender(uc[0])) || (as == 13) || (as == 30))
		{
			itemText.insertChars(CPos, uc);
			CPos += 1;
			if ((m_Doc->docHyphenator->AutoCheck) && (CPos > 1))
			{
				Twort = "";
				Tcoun = 0;
				for (int hych = CPos-1; hych > -1; hych--)
				{
					Tcha = itemText.text(hych,1);
					if (Tcha == " ")
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
			Tinput = true;
			view->RefreshItem(this);
		}
		break;
	}
	view->slotDoCurs(true);
	if ((kk == Key_Left) || (kk == Key_Right) || (kk == Key_Up) || (kk == Key_Down))
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
	m_Doc->updateFrameItems();
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
	ScribusView* view = m_Doc->view();
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
				view->RefreshItem(this);
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
	int i;
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
	view->RefreshItem(this);
}

void PageItem_TextFrame::deselectAll()
{
	PageItem *item = this;
	while( item->BackBox )
		item=item->BackBox;

	while ( item )
	{
		if ( item->itemText.lengthOfSelection() > 0 )
		{
			item->itemText.deselectAll();
			m_Doc->view()->RefreshItem(this);
			item->HasSel = false;
		}
		item = item->NextBox;
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
	return (Width - (ColGap * (Cols - 1)) - Extra - RExtra - lineCorr) / Cols;
}

void PageItem_TextFrame::drawOverflowMarker(ScPainter *p)
{
	/*CB Old large corner indicator.
	double scp1 = 1.0/QMAX(ScMW->view->getScale(), 1);
	double scp16 = 16.0*scp1;
	double scp14 = 14.0*scp1;
	double scp3 = 3.0*scp1;
	double scm_lineWidth16 = Width - scp16;
	double scpheight16 = Height - scp16;
	double scm_lineWidth3 = Width - scp3;
	double scpheight3 = Height - scp3;
	p->setPen(black, scp1, SolidLine, FlatCap, MiterJoin);
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
	p->setPen(black, 0.5/ view->scale(), SolidLine, FlatCap, MiterJoin);
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
	p->setPen(black, 0.5/ view->scale(), SolidLine, FlatCap, MiterJoin);
	p->setPenOpacity(1.0);
	p->setBrush(Qt::white);
	p->setBrushOpacity(1.0);
	p->setFillMode(ScPainter::Solid);
#ifdef HAVE_CAIRO
	p->setupPolygon(&PoLine);
	p->setClipPath();
#endif
	double startpos=Xpos;
	double cols=0;
	double colWidth = columnWidth();
	double colLeft=0;
	int currCol=0;
	double lineCorr=0;
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth / 2.0;
	if (TExtra + lineCorr!=0.0)
		p->drawLine(FPoint(0, TExtra + lineCorr), FPoint(Width, TExtra + lineCorr));
	if (BExtra + lineCorr!=0.0)
		p->drawLine(FPoint(0, Height - BExtra - lineCorr), FPoint(Width, Height - BExtra - lineCorr));
	double oldColRightX=Extra+lineCorr;
	while(currCol<Cols)
	{
		colLeft=(colWidth + ColGap) * currCol + Extra + lineCorr;
		if (colLeft!=0.0)
			p->drawLine(FPoint(colLeft, 0), FPoint(colLeft, 0+Height));
		if (colLeft+colWidth!=Width)
		p->drawLine(FPoint(colLeft+colWidth, 0), FPoint(colLeft+colWidth, 0+Height));
		oldColRightX=colLeft+colWidth;
		++currCol;
	}
	
}

#endif  //NLS_PROTO
