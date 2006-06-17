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

#include <ft2build.h>
#include FT_GLYPH_H

#include "scfontmetrics.h"
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

#ifdef NLS_PROTO
#include "text/pageitem_textframe.cpp"
#else

static const bool opticalMargins = true;

void PageItem_TextFrame::layout() 
{
	if (BackBox != NULL && BackBox->invalid) {
//		qDebug("textframe: len=%d, going back", itemText.length());
		dynamic_cast<PageItem_TextFrame*>(BackBox)->layout();
		return;
	}
	else if (!invalid) {
//		qDebug("textframe: len=%d, no relayout", itemText.length());
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
	int aSpa, chs, chsd, CurrCol;
	uint BuPos, LastSP, MaxText;
	double oldCurY, LastXp, EndX, OFs, OFs2, wide, ColWidth, kernVal, RTabX;
	QString chstr, chstr2, chstr3;
	ScText *hl;
	ParagraphStyle style;
	struct ZZ *Zli;
	struct ZZ *Zli2;
	
	bool outs = false;
	bool fBorder = false;
	bool RTab = false;
	bool goNoRoom = false;
	bool goNextColumn = false;
	uint StartRT, StartRT2;
	int TabCode = 0;
	int HyphenCount = 0;
	QValueList<ParagraphStyle::TabRecord> tTabValues;
	bool DropCmode = false;
	bool AbsHasDrop = false;
	double desc, asce, maxDY, firstDes, desc2, maxDX, tabDist;
	int DropLines;
	bool StartOfCol = true;
	tTabValues.clear();
	
/*
	for (int xxx=0; xxx<5; ++xxx)
	{
		m_Doc->docParagraphStyles[xxx].setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(LineSpMode));
		m_Doc->docParagraphStyles[xxx].setUseBaselineGrid(LineSpMode == ParagraphStyle::BaselineGridLineSpacing);
		m_Doc->docParagraphStyles[xxx].setLineSpacing(LineSp);
		m_Doc->docParagraphStyles[xxx].charStyle().fontSize() = m_FontSize;
		m_Doc->docParagraphStyles[xxx].setLeftMargin(0);
		m_Doc->docParagraphStyles[xxx].setFirstIndent(0);
		m_Doc->docParagraphStyles[xxx].setGapBefore(0);
		m_Doc->docParagraphStyles[xxx].setGapAfter(0);
		m_Doc->docParagraphStyles[xxx].setAlignment(xxx);
	}
*/
	
	QPtrList<ZZ> LiList;
	LiList.setAutoDelete(true);

	pf2.begin(view->viewport());
	pf2.translate(Xpos, Ypos);
	pf2.rotate(Rot);

	double lineCorr = 0;
	if (lineColor() != CommonStrings::None)
		lineCorr = m_lineWidth / 2.0;

	if ((itemText.length() != 0)) // || (NextBox != 0))
	{
/*		// steal text from next boxes
		if (NextBox != 0)
		{
			nextItem = NextBox;
			while (nextItem != 0)
			{
				nextItem->invalid = true;
				a = nextItem->itemText.count();
				for (uint s=0; s<a; ++s)
				{
					itemText.append(nextItem->itemText.take(0));
				}
//FIXME:av				nextItem->MaxChars = 0;
				nextItem = nextItem->NextBox;
			}
			nextItem = NextBox;
		}
//		m_Doc->docParagraphStyles[0].setLineSpacing(LineSp);
*/		
		
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
			desc2 = -hl->font()->descent() * (chs / 10.0);
			CurY = TExtra+lineCorr;
		}
		else // no dropcap:
		{
			desc2 = -m_Doc->currentStyle.charStyle().font()->descent() * (m_Doc->currentStyle.charStyle().fontSize() / 10.0);
			CurY = itemText.paragraphStyle(0).lineSpacing() + TExtra+lineCorr-desc2;
		}
		firstDes = desc2;
		LiList.clear();
		BuPos = 0;
		LastSP = 0;
		LastXp = 0;
		outs = false;
		OFs = 0;
		OFs2 = 0;
		aSpa = 0;
		MaxChars = 0;
		tabDist = 0;
		MaxText = itemText.length();
		StartOfCol = true;
		for (int a = firstInFrame(); a < itemText.length(); ++a)
		{
			hl = itemText.item(a);
			style = itemText.paragraphStyle(a);
//			qDebug(QString("style @%6: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//				   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():"")
//				   .arg(a));

			chstr = hl->ch;
			if (hl->ch == QChar(30)) // pagenumber
				chstr = ExpandToken(a);
			if (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
				style.setLineSpacing(RealFHeight(m_Doc, hl->font(), hl->fontSize()));
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
			{ 
				StyleFlag fl = hl->effects();
				fl &= ~ScStyle_DropCap;
				fl &= ~ScStyle_SmartHyphenVisible;
				hl->setEffects(fl);
			}
			// No space at begin of line
			if ( //((m_Doc->docParagraphStyles[absa].alignment() == 3) || (m_Doc->docParagraphStyles[absa].alignment() == 4)) && 
				 (LiList.count() == 0) && (hl->ch == " "))
			{
				hl->setEffects(hl->effects() | ScStyle_SuppressSpace);
				continue;
			}
			else
				hl->setEffects(hl->effects() & ~ScStyle_SuppressSpace);

			if (LiList.count() == 0)
			{
//				qDebug(QString("textframe(%1,%2): len=%3, linespacing %4: %5").arg(Xpos).arg(Ypos).arg(itemText.length())
//					   .arg(style.lineSpacingMode()).arg(style.lineSpacing()));				
 				// more about par gap and dropcaps
				if (((a > firstInFrame()) && (itemText.text(a-1) == SpecialChars::PARSEP)) || ((a == 0) && (BackBox == 0)) && (!StartOfCol))
				{
					CurY += style.gapBefore();
					if (chstr != SpecialChars::PARSEP)
						DropCmode = style.hasDropCap();
					else
						DropCmode = false;
					if (DropCmode)
					{
						DropLines = style.dropCapLines();
						if (style.useBaselineGrid())
							CurY += m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
						else
						{
							if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
								CurY += style.lineSpacing() * (DropLines-1);
							else
								CurY += RealFHeight(m_Doc, hl->font(), style.charStyle().fontSize()) * (DropLines-1);
						}
					}
				}
			}
			// find charsize & y pos
			if (DropCmode)
			{
				// dropcap active?
				if (style.useBaselineGrid())
				{
					chsd = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1)+(hl->font()->ascent() * (style.charStyle().fontSize() / 10.0))) / (RealCHeight(m_Doc, hl->font(), chstr, 10))));
					chs = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1)+(hl->font()->ascent() * (style.charStyle().fontSize() / 10.0))) / RealCAscent(m_Doc, hl->font(), chstr, 10)));
				}
				else
				{
					if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
					{
						chsd = qRound(10 * ((style.lineSpacing() * (DropLines-1)+(hl->font()->ascent() * (style.charStyle().fontSize() / 10.0))) / (RealCHeight(m_Doc, hl->font(), chstr, 10))));
						chs = qRound(10 * ((style.lineSpacing() * (DropLines-1)+(hl->font()->ascent() * (style.charStyle().fontSize() / 10.0))) / RealCAscent(m_Doc, hl->font(), chstr, 10)));
					}
					else
					{
						double currasce = RealFHeight(m_Doc, hl->font(), style.charStyle().fontSize());
						chsd = qRound(10 * ((currasce * (DropLines-1)+(hl->font()->ascent() * (style.charStyle().fontSize() / 10.0))) / (RealCHeight(m_Doc, hl->font(), chstr, 10))));
						chs = qRound(10 * ((currasce * (DropLines-1)+(hl->font()->ascent() * (style.charStyle().fontSize() / 10.0))) / RealCAscent(m_Doc, hl->font(), chstr, 10)));
					}
				}
				hl->setEffects(hl->effects() | ScStyle_DropCap);
			}
			else // ! dropCapMode
			{
				if ((hl->ch == SpecialChars::OBJECT) && (hl->cembedded != 0))
					chs = qRound((hl->cembedded->gHeight + hl->cembedded->lineWidth()) * 10);
				else
					chs = hl->fontSize();
			}
			// Smallcaps and such
			oldCurY = SetZeichAttr(*hl, &chs, &chstr);
			// some special cases TODO: move to ScFace
			if (chstr == SpecialChars::NBSPACE)
				chstr2 = " ";
			else if (chstr == SpecialChars::NBHYPHEN)
				chstr2 = "-";
			else
				chstr2 = chstr;
			// find out width of char
			if ((hl->ch == SpecialChars::OBJECT) && (hl->cembedded != 0))
				wide = hl->cembedded->gWidth + hl->cembedded->lineWidth();
			else
			{
				if (a < lastInFrame())
				{
					// apply kerning
					if (itemText.text(a+1) == SpecialChars::NBSPACE)
						chstr3 = " ";
					else if (itemText.text(a+1) == SpecialChars::NBHYPHEN)
						chstr3 = "-";
					else
						chstr3 = itemText.text(a+1);
					wide = Cwidth(m_Doc, hl->font(), chstr2, chs, chstr3);
				}
				else
					wide = Cwidth(m_Doc, hl->font(), chstr2, chs);
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
							asce = RealFHeight(m_Doc, hl->font(), style.charStyle().fontSize()) * DropLines;
					}
					hl->setScaleV(qRound(asce / (hl->cembedded->gHeight + hl->cembedded->lineWidth()) * 1000.0));
					hl->setScaleH(hl->scaleV());
				}
				else
				{
					wide = RealCWidth(m_Doc, hl->font(), chstr2, chsd);
					asce = RealCHeight(m_Doc, hl->font(), chstr2, chsd);
				}
				desc2 = 0;
				desc = 0;
			}
			else
			{
				// find ascent / descent
				double hlcsize10=hl->fontSize() / 10.0;
				if ((hl->ch == SpecialChars::OBJECT) && (hl->cembedded != 0))
				{
					desc = desc2 = 0;
				}
				else
				{
					desc = desc2 = -hl->font()->descent() * hlcsize10;
				}
				asce = hl->font()->ascent() * hlcsize10;
			}
			wide = wide * (hl->scaleH() / 1000.0);
			fBorder = false;
			// end of col reached?
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
								desc2 = -hl->font()->descent() * m_Doc->typographicSettings.valueBaseGrid * style.dropCapLines();
							else
								desc2 = -hl->font()->descent() * style.lineSpacing() * style.dropCapLines();
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
					goto NoRoom;
				}
			}
			// ok, there's more room in col
			if (LiList.isEmpty())
			{
				// start a new line
				startLin = a;
				double TopOffset = asce;
				double BotOffset = desc2;
				goNoRoom = false;
				// ??? AV
				bool specialCase = false;
				if (StartOfCol)
				{
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
											desc2 = -hl->font()->descent() * m_Doc->typographicSettings.valueBaseGrid * style.dropCapLines();
										else
											desc2 = -hl->font()->descent() * style.lineSpacing() * style.dropCapLines();
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
					int chs = static_cast<int>(itemText.charStyle(a).fontSize() * (itemText.charStyle(a).scaleH() / 1000.0));
					double leftCorr = RealCWidth(m_Doc, itemText.charStyle(a).font(), itemText.text(a), chs);
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
						leftCorr = 0;
//						leftCorr = Cwidth(m_Doc, itemText.charStyle(a).font(), QChar('o'), chs, itemText.text(a));
//						leftCorr -= Cwidth(m_Doc, itemText.charStyle(a).font(), QChar('o'), chs);
					}
					CurX += leftCorr;
				}
				fBorder = false;
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
					StartRT = LiList.count();
					StartRT2 = a;
				}
			}
			
			// remember y pos
			hl->glyph.yoffset = CurY + oldCurY;
			if (DropCmode)
				hl->glyph.yoffset -= RealCHeight(m_Doc, hl->font(), chstr2, chsd) - RealCAscent(m_Doc, hl->font(), chstr2, chsd);
			// find tracking
			if (LiList.count() == 0)
			{
				itemText.item(a)->setEffects(itemText.item(a)->effects() | ScStyle_StartOfLine);
				kernVal = 0;
			}
			else
			{
				kernVal = chs * hl->tracking() / 10000.0;
				itemText.item(a)->setEffects(itemText.item(a)->effects() & ~ScStyle_StartOfLine);
			}
			// remember x pos
			if (!RTab)
			{
//				hl->glyph.xoffset = QMAX(CurX+kernVal, ColBound.x());
				hl->glyph.xoffset = CurX+kernVal; // needed for left optical margin
				CurX += wide+kernVal;
//				CurX = QMAX(CurX, ColBound.x());
			}
			else
			{
				CurX = QMAX(CurX, ColBound.x());
				hl->glyph.xoffset = CurX;
			}
			// more right tab stuff
			if ((TabCode == 4) && (RTab))
			{
				CurX += (wide+kernVal) / 2;
				CurX = QMAX(CurX, ColBound.x());
			}
			// hyphenation
			if (((hl->effects() & ScStyle_HyphenationPossible) || hl->ch == "-") && (HyphenCount < m_Doc->HyCount || m_Doc->HyCount == 0))
			{
				if (hl->effects() & ScStyle_HyphenationPossible)
				{
					pt1 = QPoint(qRound(ceil(CurX+RExtra+Cwidth(m_Doc, hl->font(), "-", hl->fontSize()) * (hl->scaleH() / 1000.0))), qRound(CurY+desc));
					pt2 = QPoint(qRound(ceil(CurX+RExtra+Cwidth(m_Doc, hl->font(), "-", hl->fontSize()) * (hl->scaleH() / 1000.0))), qRound(ceil(CurY-asce)));
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
			
//Hm.
			Zli = new ZZ;
			Zli->Zeich = chstr;
			Zli->Farb = hl->fillColor();
			Zli->shade = hl->fillShade();
			Zli->Farb2 = hl->strokeColor();
			Zli->shade2 = hl->strokeShade();
			Zli->xco = hl->glyph.xoffset;
			Zli->yco = hl->glyph.yoffset;
			Zli->Sele = itemText.selected(a);
			if (DropCmode)
			{
				Zli->Siz = chsd;
				Zli->realSiz = style.charStyle().fontSize();
			}
			else
			{
				Zli->Siz = chs;
				Zli->realSiz = hl->fontSize();
			}
			Zli->Style = hl->effects();
			Zli->ZFo = hl->font();
			Zli->wide = wide;
			Zli->kern = kernVal;
			Zli->scale = hl->scaleH();
			Zli->scalev = hl->scaleV();
			Zli->base = hl->baselineOffset();
			Zli->shadowX = hl->shadowXOffset();
			Zli->shadowY = hl->shadowYOffset();
			Zli->outline = hl->outlineWidth();
			Zli->underpos = hl->underlineOffset();
			Zli->underwidth = hl->underlineWidth();
			Zli->strikepos = hl->strikethruOffset();
			Zli->strikewidth = hl->strikethruWidth();
			Zli->embedded = hl->cembedded;
			if (((hl->ch == " ") || (hl->ch == SpecialChars::TAB)) && (!outs))
			{
				if ( a == firstInFrame() || itemText.text(a-1) !=  ' ')
				{
					LastXp = hl->glyph.xoffset;
					LastSP = BuPos;
				}
			}
			if (((hl->effects() & ScStyle_HyphenationPossible) || (hl->ch == "-")) && (!outs))
			{
				if ((HyphenCount < m_Doc->HyCount) || (m_Doc->HyCount == 0))
				{
					if (hl->ch == "-")
						LastXp = CurX;
					else
						LastXp = CurX + Cwidth(m_Doc, hl->font(), "-", hl->fontSize()) * (hl->scaleH() / 1000.0);
					LastSP = BuPos + 1;
				}
			}
			LiList.append(Zli);
			if (RTab)
			{
				uint rtx2 = 0;
				double cen = 1;
				if (TabCode == 4)
					cen = 2;
				for (uint rtx = StartRT; rtx < LiList.count(); ++rtx)
				{
					LiList.at(rtx)->xco = QMAX(LiList.at(rtx)->xco-(wide+kernVal) / cen, 0.0);
					itemText.item(StartRT2+rtx2)->glyph.xoffset = QMAX(itemText.item(StartRT2+rtx2)->glyph.xoffset-(wide+kernVal) / cen, 0.0);
					if (itemText.item(StartRT2+rtx2)->glyph.xoffset < RTabX)
					{
						RTab = false;
						TabCode = 0;
					}
					rtx2++;
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
						double currasce = RealFHeight(m_Doc, hl->font(), style.charStyle().fontSize());
						CurY -= currasce * (DropLines-1);
						tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY-DropLines*currasce)));
						tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*currasce)));
					}
				}
				tcli.setPoint(2, QPoint(qRound(maxDX), qRound(maxDY)));
				tcli.setPoint(3, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY)));
				cm = QRegion(pf2.xForm(tcli));
				cl = cl.subtract(cm);
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
						EndX = floor(hl->glyph.xoffset - 1);
//						qDebug(QString("endx start=%1, hl is '%2'").arg(EndX).arg(hl->ch));
						do
						{
							pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
							pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
							EndX += 0.125;
						}
						while ((cl.contains(pf2.xForm(pt1))) 
							   && (cl.contains(pf2.xForm(pt2))) 
							   && (EndX+RExtra+lineCorr < ColBound.y() - style.rightMargin()));
						// end do
//						qDebug(QString("endx @ %1").arg(EndX));
						
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
							for (uint sof = 0; sof<LiList.count(); ++sof)
							{
								if ((LiList.at(sof)->Zeich == QChar(32)) 
									|| (LiList.at(sof)->Zeich == SpecialChars::NBSPACE)
									)
									aSpa++;
							}
							if (aSpa != 0)
								OFs2 = (EndX - CurX) / aSpa;
							else
								OFs2 = 0;
							OFs = 0;
							// distribute whitespace on spaces
							for (uint yof = 0; yof < LiList.count(); ++yof)
							{
								LiList.at(yof)->xco += OFs;
								if ((LiList.at(yof)->Zeich == QChar(32)) 
									|| (LiList.at(yof)->Zeich == SpecialChars::NBSPACE)
									)
									OFs += OFs2;
							}
						}
						else
						{
							// simple offset
							for (uint xof = 0; xof<LiList.count(); ++xof)
							{
								LiList.at(xof)->xco += OFs;
							}
						}
						CurX = EndX;
					}
				}
				else // outs -- last char went outside the columns
				{
					if (LastSP != 0)            // Hier koenen auch andere Trennungen eingebaut werden
					{
						// go back to last break position
						a -= BuPos - LastSP;
						hl = itemText.item(a);
						style = itemText.paragraphStyle(a);
//						qDebug(QString("style <@%6: %1 -- %2, %4/%5 char: %3").arg(style.leftMargin()).arg(style.rightMargin())
//							   .arg(style.charStyle().asString()).arg(style.name()).arg(style.parent()?style.parent()->name():"")
//							   .arg(a));
//not needed any more:	a++;
						// Fix incorrect right flush for full lines: ? -AV
						LastXp = hl->glyph.xoffset + Cwidth(m_Doc, hl->font(), hl->ch, hl->fontSize()) * (hl->scaleH() / 1000.0);
						if (hl->effects() & ScStyle_HyphenationPossible)
						{
							// insert hyphen
							HyphenCount++;
							hl->setEffects(hl->effects() | ScStyle_SmartHyphenVisible);
							Zli = new ZZ;
							Zli->Zeich = "-";
							Zli->Farb = itemText.charStyle(a).fillColor();
							Zli->Farb2 = itemText.charStyle(a).strokeColor();
							Zli->shade = itemText.charStyle(a).fillShade();
							Zli->shade2 = itemText.charStyle(a).strokeShade();
							Zli->xco = LastXp - Cwidth(m_Doc, itemText.charStyle(a).font(), "-", itemText.charStyle(a).fontSize()) * (itemText.charStyle(a).scaleH() / 1000.0);
							Zli->yco = itemText.item(a)->glyph.yoffset;
							Zli->Sele = itemText.selected(a);
							Zli->Siz = itemText.charStyle(a).fontSize();
							Zli->realSiz = itemText.charStyle(a).fontSize();
							Zli->Style = itemText.charStyle(a).effects();
							Zli->ZFo = itemText.charStyle(a).font();
							Zli->wide = Cwidth(m_Doc, itemText.charStyle(a).font(), "-", itemText.charStyle(a).fontSize()) * (itemText.charStyle(a).scaleH() / 1000.0);
							Zli->kern = itemText.charStyle(a).fontSize() * itemText.charStyle(a).tracking() / 10000.0;
							Zli->scale = itemText.charStyle(a).scaleH();
							Zli->scalev = itemText.charStyle(a).scaleV();
							Zli->base = itemText.charStyle(a).baselineOffset();
							Zli->shadowX = itemText.charStyle(a).shadowXOffset();
							Zli->shadowY = itemText.charStyle(a).shadowYOffset();
							Zli->outline = itemText.charStyle(a).outlineWidth();
							Zli->underpos = itemText.charStyle(a).underlineOffset();
							Zli->underwidth = itemText.charStyle(a).underlineWidth();
							Zli->strikepos = itemText.charStyle(a).strikethruOffset();
							Zli->strikewidth = itemText.charStyle(a).strikethruWidth();
							Zli->embedded = 0;
							LiList.insert(LastSP+1, Zli);
							LastSP += 2;
							LastXp += Zli->wide;
						}
						else 
						{
							HyphenCount = 0;
							hl->setEffects(hl->effects() & ~ScStyle_SmartHyphenVisible);
						}
						BuPos = LastSP;
						// Justification
						if (style.alignment() != 0)
						{
							// find end of line
							EndX = floor(hl->glyph.xoffset);
//							qDebug(QString("endx start=%1, hl is '%2'").arg(EndX).arg(hl->ch));
							do
							{
								pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
								pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
								EndX += 0.125;
							}
							while ((cl.contains(pf2.xForm(pt1))) 
								   && (cl.contains(pf2.xForm(pt2))) 
								   && (EndX+RExtra+lineCorr < ColBound.y() - style.rightMargin()));
							// end do
//							qDebug(QString("endx @ %1").arg(EndX));

							if (opticalMargins && BuPos > 0) {
								int chs = static_cast<int>(LiList.at(BuPos-1)->Siz * (LiList.at(BuPos-1)->scale / 1000.0));
								QString chr = LiList.at(BuPos-1)->Zeich;
								double rightCorr = RealCWidth(m_Doc, LiList.at(BuPos-1)->ZFo, chr, chs);
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
									rightCorr = Cwidth(m_Doc, LiList.at(BuPos-1)->ZFo, chr, chs);
									rightCorr -= Cwidth(m_Doc, LiList.at(BuPos-1)->ZFo, chr, chs, QChar('.'));
								}
								EndX += rightCorr;
								//									qDebug(QString("orm %1 @ %2: %3 %4").arg(rightCorr).arg(a).arg(chr).arg(LiList.at(BuPos-1)->Style&ScStyle_HyphenationPossible? "smart" : ""));
							}
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
									if ( LiList.at(sof)->Zeich == QChar(32)
										 || LiList.at(sof)->Zeich == SpecialChars::NBSPACE
										)
										aSpa++;
								}
								if (aSpa > 0)
									OFs2 = (EndX - LastXp) / aSpa;
								else
									OFs2 = 0;
								
								OFs = 0;
//								qDebug(QString("just %1").arg(OFs2));
								for (uint yof = 0; yof < LiList.count(); ++yof)
								{
									LiList.at(yof)->xco += OFs;
									if ( LiList.at(yof)->Zeich == QChar(32)
										 || (LiList.at(yof)->Zeich == SpecialChars::NBSPACE)
										 )
										OFs += OFs2;
								}
							}
							else
							{
								for (uint xof = 0; xof<LiList.count(); ++xof)
								{
									LiList.at(xof)->xco += OFs;
								}
							}
//							qDebug(QString("line: endx=%1 lastchar=%2").arg(EndX).arg(LiList.at(BuPos-1)->xco + LiList.at(BuPos-1)->wide));
							CurX = EndX;
						}
					}
					else // no break position
					{
						a--;
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
				
				hl->glyph.xoffset = CurX;
				hl->glyph.yoffset = CurY;
				LiList.at(LiList.count()-1)->xco = hl->glyph.xoffset;
				LiList.at(LiList.count()-1)->yco = hl->glyph.yoffset;

				if (LiList.count() != 0)
				{
					if ((!AbsHasDrop) && (StartOfCol) && (!style.useBaselineGrid()))
					{
						Zli2 = LiList.at(0);
						double firstasce = Zli2->ZFo->ascent() * (Zli2->realSiz / 10.0);
						double currasce;
						if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
							currasce = Zli2->ZFo->ascent() * (Zli2->realSiz / 10.0);
						else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
						else
							currasce = RealCAscent(m_Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz);
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							Zli2 = LiList.at(zc);
							if ((Zli2->Zeich == SpecialChars::TAB) || (Zli2->Zeich == QChar(10))
								|| (Zli2->Zeich == SpecialChars::PARSEP) || (Zli2->Zeich == SpecialChars::NBHYPHEN)
								|| (Zli2->Zeich == SpecialChars::COLBREAK) || (Zli2->Zeich == SpecialChars::LINEBREAK)
								|| (Zli2->Zeich == SpecialChars::FRAMEBREAK) || (Zli2->Zeich == SpecialChars::NBSPACE))
								continue;
							if ((Zli2->Zeich == SpecialChars::OBJECT) && (Zli2->embedded != 0))
								currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
							else
								currasce = QMAX(currasce, RealCAscent(m_Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz));
						}
						double adj = firstasce - currasce;
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							LiList.at(zc)->yco -= adj;
						}
						CurY -= adj;
					}
					if ((!StartOfCol) && (!style.useBaselineGrid()) && (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
					{
						Zli2 = LiList.at(0);
						double firstasce = style.lineSpacing();
						double currasce = 0;
						if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
						else
							currasce = RealFHeight(m_Doc, Zli2->ZFo, Zli2->realSiz);
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							Zli2 = LiList.at(zc);
							if ((Zli2->Zeich == SpecialChars::TAB) || (Zli2->Zeich == QChar(10))
								|| (Zli2->Zeich == SpecialChars::PARSEP) || (Zli2->Zeich == QChar(24))
								|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
								|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
								continue;
							if ((Zli2->Zeich == SpecialChars::OBJECT) && (Zli2->embedded != 0))
								currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
							else
								currasce = QMAX(currasce, RealFHeight(m_Doc, Zli2->ZFo, Zli2->realSiz));
						}
						double adj = firstasce - currasce;
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							LiList.at(zc)->yco -= adj;
						}
						CurY -= adj;
					}
				}
				StartOfCol = false;
				tabDist = ColBound.x();
				uint loopC = BuPos3;
				if (m_Doc->guidesSettings.showControls)
					loopC++;  // ??? AV
				for (uint zc = 0; zc < loopC; ++zc)
				{
					if ( zc >= LiList.count()) {
						qDebug("layout: zc too large %d / %d", zc, LiList.count());
						continue;
					}
					if ( startLin+zc >= itemText.length()) {
						qDebug("layout: startLin+zc too large %d+%d / %d", startLin, zc, itemText.length());
						continue;
					}
//					else
//						qDebug("layout: startLin, zc ok. %d, %d, %d", startLin, zc, LiList.count());
					double wide2 = 0;
					Zli2 = LiList.at(zc);
					double xcoZli = Zli2->xco;
					itemText.item(startLin+zc)->glyph.xoffset = Zli2->xco;
					itemText.item(startLin+zc)->glyph.yoffset = Zli2->yco;
					tTabValues = itemText.paragraphStyle(startLin+zc).tabValues();
					desc = Zli2->ZFo->descent() * (-Zli2->Siz / 10.0);
					asce = Zli2->ZFo->ascent() * (Zli2->Siz / 10.0);
					if ((((Zli2->Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli2->Sele))) && (m_Doc->appMode == modeEdit))
					{
						wide = Zli2->wide;
						if ((zc > 0) && (Zli2->Zeich == SpecialChars::TAB))
						{
							wide2 = LiList.at(zc-1)->wide;
							xcoZli = LiList.at(zc-1)->xco+wide2;
							wide = Zli2->xco - xcoZli + Zli2->wide;
						}
					}
					tabDist = Zli2->xco+Zli2->wide;
				}
				LiList.clear();
				BuPos = 0;
				LastSP = 0;
				LastXp = 0;
				outs = false;
				if (goNoRoom)
				{
					goNoRoom = false;
					nrc = a+1;
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
						goto NoRoom;
					}
				}
			}
		}
		if (style.alignment() != 0)
		{
			EndX = floor(CurX);
			do
			{
				pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
				pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
				EndX += 0.125;
			}
			while ((cl.contains(pf2.xForm(pt1))) 
				   && (cl.contains(pf2.xForm(pt2))) 
				   && (EndX+RExtra+lineCorr < ColBound.y() - style.rightMargin()));
			
			if (opticalMargins && LiList.count() > 0) {
				int chs = static_cast<int>(LiList.at(LiList.count()-1)->Siz * (LiList.at(LiList.count()-1)->scale / 1000.0));
				QString chr = LiList.at(LiList.count()-1)->Zeich;
				double rightCorr = RealCWidth(m_Doc, LiList.at(LiList.count()-1)->ZFo, chr, chs);
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
					rightCorr = Cwidth(m_Doc, LiList.at(BuPos-1)->ZFo, chr, chs);
					rightCorr -= Cwidth(m_Doc, LiList.at(BuPos-1)->ZFo, chr, chs, QChar('.'));
				}
				EndX += rightCorr;
				//					qDebug(QString("orm %1 @ %2: %3 par").arg(rightCorr).arg(a).arg(chr));
			}

			if (style.alignment() == 2)
				OFs = EndX - CurX;
			if (style.alignment() == 1)
				OFs = (EndX - CurX) / 2;
			if (style.alignment() == 3)
				OFs = 0;
			if (style.alignment() == 4)
			{
				aSpa = 0;
				for (uint sof = 0; sof<LiList.count(); ++sof)
				{
					if ((LiList.at(sof)->Zeich == QChar(32)) 
						|| (LiList.at(sof)->Zeich == SpecialChars::NBSPACE)
						)
						aSpa++;
				}
				if (aSpa != 0)
					OFs2 = (EndX - CurX) / aSpa;
				else
					OFs2 = 0;
				OFs = 0;
				for (uint yof = 0; yof < LiList.count(); ++yof)
				{
					LiList.at(yof)->xco += OFs;
					if ((LiList.at(yof)->Zeich == QChar(32)) || (LiList.at(yof)->Zeich == QChar(29)))
						OFs += OFs2;
				}
			}
			else
			{
				for (uint xof = 0; xof<LiList.count(); ++xof)
				{
					LiList.at(xof)->xco += OFs;
				}
			}
		}
		if (LiList.count() != 0)
		{
			if ((!AbsHasDrop) && (StartOfCol) && (!style.useBaselineGrid()))
			{
				Zli2 = LiList.at(0);
				double firstasce = Zli2->ZFo->ascent() * (Zli2->realSiz / 10.0);
				double currasce = 0;
				if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
					currasce = Zli2->ZFo->ascent() * (Zli2->realSiz / 10.0);
				else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
					currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
				else
					currasce = RealCAscent(m_Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz);
				for (uint zc = 0; zc < LiList.count(); ++zc)
				{
					Zli2 = LiList.at(zc);
					if ((Zli2->Zeich == SpecialChars::TAB) || (Zli2->Zeich == QChar(10))
						|| (Zli2->Zeich == SpecialChars::PARSEP) || (Zli2->Zeich == SpecialChars::NBHYPHEN)
						|| (Zli2->Zeich == SpecialChars::FRAMEBREAK) || (Zli2->Zeich == SpecialChars::COLBREAK)
						|| (Zli2->Zeich == SpecialChars::LINEBREAK) || (Zli2->Zeich == SpecialChars::NBSPACE))
						continue;
					if ((Zli2->Zeich == SpecialChars::OBJECT) && (Zli2->embedded != 0))
						currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
					else
						currasce = QMAX(currasce, RealCAscent(m_Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz));
				}
				double adj = firstasce - currasce;
				for (uint zc = 0; zc < LiList.count(); ++zc)
				{
					LiList.at(zc)->yco -= adj;
				}
				CurY -= adj;
			}
			if ((!StartOfCol) && (!style.useBaselineGrid()) && (style.lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
			{
				Zli2 = LiList.at(0);
				double firstasce = style.lineSpacing();
				double currasce = 0;
				if ((Zli2->Zeich == SpecialChars::OBJECT) && (Zli2->embedded != 0))
					currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
				else
					currasce = RealFHeight(m_Doc, Zli2->ZFo, Zli2->realSiz);
				for (uint zc = 0; zc < LiList.count(); ++zc)
				{
					Zli2 = LiList.at(zc);
					if ((Zli2->Zeich == SpecialChars::TAB) || (Zli2->Zeich == QChar(10))
						|| (Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(24))
						|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
						|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
						continue;
					if ((Zli2->Zeich == SpecialChars::OBJECT) && (Zli2->embedded != 0))
						currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
					else
						currasce = QMAX(currasce, RealFHeight(m_Doc, Zli2->ZFo, Zli2->realSiz));
				}
				double adj = firstasce - currasce;
				for (uint zc = 0; zc < LiList.count(); ++zc)
				{
					LiList.at(zc)->yco -= adj;
				}
				CurY -= adj;
			}
		}
		StartOfCol = false;
		tabDist = ColBound.x();
		for (uint zc = 0; zc<LiList.count(); ++zc)
		{
			double wide2 = 0;
			Zli2 = LiList.at(zc);
			double xcoZli = Zli2->xco;
			
			itemText.item(startLin+zc)->glyph.xoffset = Zli2->xco;
			itemText.item(startLin+zc)->glyph.yoffset = Zli2->yco;
			tTabValues = itemText.paragraphStyle(startLin+zc).tabValues();
			
			desc = Zli2->ZFo->descent() * (-Zli2->Siz / 10.0);
			asce = Zli2->ZFo->ascent() * (Zli2->Siz / 10.0);
			if ((((Zli2->Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli2->Sele))) && (m_Doc->appMode == modeEdit))
			{
				wide = Zli2->wide;
				if ((zc > 0) && (Zli2->Zeich == SpecialChars::TAB))
				{
					wide2 = LiList.at(zc-1)->wide;
					xcoZli = LiList.at(zc-1)->xco+wide2;
					wide = Zli2->xco - xcoZli + Zli2->wide;
				}
			}
			tabDist = Zli2->xco+Zli2->wide;
		}
		goNextColumn = false;
		LiList.clear();
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
			CPos = nrc;
			if ((m_Doc->appMode == modeEdit) && (Tinput))
			{
				//							OwnPage->Deselect(true);
				next->CPos = QMAX(nCP, itemText.length());
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
	int chs, CurrCol;
	double oldCurY, wide, lineCorr, ColWidth;
	QString chstr;
	ScText *hl;

	QValueList<ParagraphStyle::TabRecord> tTabValues;
	double desc, asce, maxDY, tabDist;
	tTabValues.clear();
	
	QPtrList<ZZ> LiList;
	LiList.setAutoDelete(true);
	QRect e2 = QRect(qRound(e.x()  / sc + m_Doc->minCanvasCoordinate.x()), qRound(e.y()  / sc + m_Doc->minCanvasCoordinate.y()), qRound(e.width() / sc), qRound(e.height() / sc));

	p->save();
	pf2.begin(view->viewport());
	pf2.translate(Xpos, Ypos);
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
		struct ZZ Zli3;
		CurrCol = 0;
		ColWidth = columnWidth();
		ColBound = FPoint((ColWidth + ColGap) * CurrCol+Extra + lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
		ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
		tabDist = ColBound.x();
		uint tabCc = 0;
		for (int a = firstInFrame(); a <= lastInFrame(); ++a)
		{
			hl = itemText.item(a);
			const CharStyle& charStyle(itemText.charStyle(a));
			const ParagraphStyle& style(itemText.paragraphStyle(a));
			tTabValues = style.tabValues();
			if (charStyle.effects() & ScStyle_StartOfLine)
				tabCc = 0;
			chstr = hl->ch;
			if (hl->glyph.yoffset == 0)
				continue;
			if (hl->ch == QChar(30))
				chstr = ExpandToken(a);
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
			chs = charStyle.fontSize();
			if (charStyle.effects() & ScStyle_DropCap)
			{
				if (style.useBaselineGrid())
					chs = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (style.dropCapLines()-1)+(charStyle.font()->ascent() * (style.charStyle().fontSize() / 10.0))) / (RealCHeight(m_Doc, charStyle.font(), chstr, 10))));
				else
				{
					if (style.lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
						chs = qRound(10 * ((style.lineSpacing() * (style.dropCapLines()-1)+(charStyle.font()->ascent() * (style.charStyle().fontSize() / 10.0))) / (RealCHeight(m_Doc, charStyle.font(), chstr, 10))));
					else
					{
						double currasce = RealFHeight(m_Doc, charStyle.font(), style.charStyle().fontSize());
						chs = qRound(10 * ((currasce * (style.dropCapLines()-1)+(charStyle.font()->ascent() * (style.charStyle().fontSize() / 10.0))) / RealCHeight(m_Doc, charStyle.font(), chstr, 10)));
					}
				}
			}
			oldCurY = SetZeichAttr(*hl, &chs, &chstr);
			if ((chstr == SpecialChars::TAB) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
			{
				QString tabFillCharQStr(tTabValues[tabCc].tabFillChar);
				double wt = Cwidth(m_Doc, charStyle.font(), tabFillCharQStr, chs);
				int coun = static_cast<int>((hl->glyph.xoffset - tabDist) / wt);
				double sPos = hl->glyph.xoffset - (hl->glyph.xoffset - tabDist) + 1;
				desc = charStyle.font()->descent() * (-chs / 10.0);
				asce = charStyle.font()->ascent() * (chs / 10.0);
				Zli3.Zeich = tabFillCharQStr;
				Zli3.Farb = charStyle.fillColor();
				Zli3.Farb2 = charStyle.strokeColor();
				Zli3.shade = charStyle.fillShade();
				Zli3.shade2 = charStyle.strokeShade();
				Zli3.yco = hl->glyph.yoffset;
				Zli3.Sele = itemText.selected(a);
				Zli3.Siz = chs;
				Zli3.realSiz = charStyle.fontSize();
				Zli3.Style = charStyle.effects();
				Zli3.ZFo = charStyle.font();
				Zli3.wide = wt;
				Zli3.kern = 0;
				Zli3.scale = 1000;
				Zli3.scalev = 1000;
				Zli3.shadowX = charStyle.shadowXOffset();
				Zli3.shadowY = charStyle.shadowYOffset();
				Zli3.outline = charStyle.outlineWidth();
				Zli3.base = charStyle.baselineOffset();
				Zli3.underpos = charStyle.underlineOffset();
				Zli3.underwidth = charStyle.underlineWidth();
				Zli3.strikepos = charStyle.strikethruOffset();
				Zli3.strikewidth = charStyle.strikethruWidth();
				for (int cx = 0; cx < coun; ++cx)
				{
					Zli3.xco =  sPos + wt * cx;
					if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
						DrawZeichenS(p, &Zli3);
				}
			}
			if (chstr == SpecialChars::TAB)
				tabCc++;
			Zli3.Zeich = chstr;
			Zli3.Farb = charStyle.fillColor();
			Zli3.Farb2 = charStyle.strokeColor();
			Zli3.shade = charStyle.fillShade();
			Zli3.shade2 = charStyle.strokeShade();
			Zli3.xco = hl->glyph.xoffset;
			Zli3.yco = hl->glyph.yoffset;
			Zli3.Sele = itemText.selected(a);
			Zli3.Siz = chs;
			Zli3.realSiz = charStyle.fontSize();
			Zli3.Style = charStyle.effects();
			Zli3.ZFo = charStyle.font();
			if ((hl->ch == SpecialChars::OBJECT) && (hl->cembedded != 0))
				Zli3.wide = (hl->cembedded->gWidth + hl->cembedded->lineWidth()) * (charStyle.scaleH() / 1000.0);
			else
				Zli3.wide = Cwidth(m_Doc, charStyle.font(), chstr, charStyle.fontSize()) * (charStyle.scaleH() / 1000.0);
			if (charStyle.effects() & ScStyle_StartOfLine)
				Zli3.kern = 0;
			else
				Zli3.kern = chs * charStyle.tracking() / 10000.0;
			Zli3.scale = charStyle.scaleH();
			Zli3.scalev = charStyle.scaleV();
			Zli3.base = charStyle.baselineOffset();
			Zli3.shadowX = charStyle.shadowXOffset();
			Zli3.shadowY = charStyle.shadowYOffset();
			Zli3.outline = charStyle.outlineWidth();
			Zli3.underpos = charStyle.underlineOffset();
			Zli3.underwidth = charStyle.underlineWidth();
			Zli3.strikepos = charStyle.strikethruOffset();
			Zli3.strikewidth = charStyle.strikethruWidth();
			Zli3.embedded = hl->cembedded;
			if (!m_Doc->RePos)
			{
				double xcoZli = Zli3.xco;
				desc = Zli3.ZFo->descent() * (-Zli3.Siz / 10.0);
				asce = Zli3.ZFo->ascent() * (Zli3.Siz / 10.0);
				if ((((Zli3.Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli3.Sele))) && (m_Doc->appMode == modeEdit))
				{
					wide = Zli3.wide;
					p->setFillMode(1);
//							p->setBrush(darkBlue);
					p->setBrush(qApp->palette().color(QPalette::Active, QColorGroup::Highlight));
					p->setLineWidth(0);
					if ((a > 0) && (Zli3.Zeich == SpecialChars::TAB))
					{
						xcoZli = itemText.item(a-1)->glyph.xoffset+Cwidth(m_Doc, itemText.charStyle(a-1).font(), itemText.text(a-1), itemText.charStyle(a-1).fontSize());
						wide = Zli3.xco - xcoZli + Zli3.wide;
					}
					if (!m_Doc->RePos)
						p->drawRect(xcoZli, qRound(Zli3.yco-asce * (Zli3.scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli3.scalev / 1000.0)));
					p->setBrush(qApp->palette().color(QPalette::Active, QColorGroup::HighlightedText));
//							p->setBrush(white);
				}
				if (Zli3.Farb2 != CommonStrings::None)
				{
					QColor tmp;
					SetFarbe(&tmp, Zli3.Farb2, Zli3.shade2);
					p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
				}
				if (((chstr == QChar(13)) || (chstr == QChar(28))) && (m_Doc->guidesSettings.showControls))
				{
					if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco+Zli3.wide),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
					{
						FPointArray points;
						double ytrans, xtrans;
						if (chstr == QChar(13))
						{
							points = m_Doc->symReturn.copy();
							if (a > 0)
								ytrans = itemText.item(a-1)->glyph.yoffset-((Zli3.Siz / 10.0) * 0.8);
							else
								ytrans = Zli3.yco - style.lineSpacing()-((Zli3.Siz / 10.0) * 0.8);
						}
						else
						{
							points = m_Doc->symNewLine.copy();
							if (a > 0)
								ytrans = itemText.item(a-1)->glyph.yoffset-((Zli3.Siz / 10.0) * 0.4);
							else
								ytrans = Zli3.yco - style.lineSpacing()-((Zli3.Siz / 10.0) * 0.4);
						}
						if (charStyle.effects() & ScStyle_StartOfLine)
							xtrans = Zli3.xco;
						else
						{
							if (a > 0)
								xtrans = itemText.item(a-1)->glyph.xoffset+ Cwidth(m_Doc, itemText.charStyle(a-1).font(), itemText.text(a-1), itemText.charStyle(a-1).fontSize());
							else
								xtrans = Zli3.xco;
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
				if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
				{
					if (Zli3.Zeich == QChar(25))
						DrawObj_Embedded(p, e, &Zli3);
					else
						DrawZeichenS(p, &Zli3);
				}
				if (charStyle.effects() & ScStyle_SmartHyphenVisible)
				{
					Zli3.Zeich = "-";
					Zli3.xco = Zli3.xco + Zli3.wide;
					if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
						DrawZeichenS(p, &Zli3);
				}
			}
			tabDist = Zli3.xco+Zli3.wide;
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
	return;   // the following is done by layout()
	
/*	
	if ((itemText.count() != 0) || (NextBox != 0))
	{
		// stealing text from NextBoxes
		if (NextBox != 0)
		{
			nextItem = NextBox;
			while (nextItem != 0)
			{
				a = nextItem->itemText.count();
				for (uint s=0; s<a; ++s)
				{
					itemText.append(nextItem->itemText.take(0));
				}
				nextItem->MaxChars = 0;
				nextItem = nextItem->NextBox;
			}
			nextItem = NextBox;
		}
		// ??? AV
		m_Doc->docParagraphStyles[0].setLineSpacing(LineSp);

		QRegion cl = availableRegion(QRegion(pf2.xForm(Clip)));
		
		if (imageFlippedH())
		{
			p->translate(Width * sc, 0);
			p->scale(-1, 1);
			pf2.translate(Width, 0);
			pf2.scale(-1, 1);
		}
		if (imageFlippedV())
		{
			p->translate(0, Height * sc);
			p->scale(1, -1);
			pf2.translate(0, Height);
			pf2.scale(1, -1);
		}
		CurrCol = 0;
		ColWidth = columnWidth();
		ColBound = FPoint((ColWidth + ColGap) * CurrCol+Extra + lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
		ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
		CurX = ColBound.x();
		if (itemText.count() > 0)
		{
			hl = itemText.at(0);
			if (hl->cab < 0) {
				qDebug(QString("correcting parstyle %1 for first char %2 / 533").arg(hl->cab).arg(hl->ch));
				hl->cab = 0;
			}
			if (m_Doc->docParagraphStyles[hl->cab].hasDropCap())
			{
				if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
					chs = qRound(m_Doc->typographicSettings.valueBaseGrid  * m_Doc->docParagraphStyles[hl->cab].dropCapLines() * 10);
				else
					chs = qRound(m_Doc->docParagraphStyles[hl->cab].lineSpacing() * m_Doc->docParagraphStyles[hl->cab].dropCapLines() * 10);
			}
			else
				chs = hl->fontSize();
			desc2 = -hl->font()->numDescender * (chs / 10.0);
			CurY = TExtra+lineCorr;
		}
		else
		{
			desc2 = -(*m_Doc->AllFonts)[m_Font]->numDescender * (m_FontSize / 10.0);
			CurY = m_Doc->docParagraphStyles[0].lineSpacing() + TExtra+lineCorr-desc2;
		}
		firstDes = desc2;
		LiList.clear();
		BuPos = 0;
		LastSP = 0;
		LastXp = 0;
		outs = false;
		OFs = 0;
		OFs2 = 0;
		aSpa = 0;
		absa = 0;
		MaxChars = 0;
		tabDist = 0;
		MaxText = itemText.count();
		StartOfCol = true;
		for (a = 0; a < MaxText; ++a)
		{
			hl = itemText.at(a);
			if (hl->cab < 0) {
				qDebug(QString("correcting parstyle %1 for index %2 char %3 / 591").arg(hl->cab).arg(a).arg(hl->ch));
				hl->cab = 0;
			}
			chx = hl->ch;
			if (hl->ch == QChar(30))
				chx = ExpandToken(a);
			absa = hl->cab;
			if (m_Doc->docParagraphStyles[absa].lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing)
				m_Doc->docParagraphStyles[absa].setLineSpacing(RealFHeight(m_Doc, hl->font(), hl->fontSize()));
			if (a == 0)
			{
				if (BackBox != 0)
				{
					nextItem = BackBox;
					while (nextItem != 0)
					{
						uint nextItemTextCount=nextItem->itemText.count();
						if (nextItemTextCount != 0)
						{
							if (nextItem->itemText.at(nextItemTextCount-1)->ch == QChar(13))
							{
								CurY += m_Doc->docParagraphStyles[absa].gapBefore();
								if (chx != QChar(13))
								{
									DropCmode = m_Doc->docParagraphStyles[absa].hasDropCap();
									if (DropCmode)
										DropLines = m_Doc->docParagraphStyles[absa].dropCapLines();
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
					if (chx != QChar(13))
					{
						DropCmode = m_Doc->docParagraphStyles[absa].hasDropCap();
						if (DropCmode)
							DropLines = m_Doc->docParagraphStyles[absa].dropCapLines();
					}
					else
						DropCmode = false;
					CurY += m_Doc->docParagraphStyles[absa].gapBefore();
				}
			}
			hl->effects() &= ~ScStyle_DropCap;
			hl->cstyle &= ~ScStyle_SmartHyphenVisible;
			if (((m_Doc->docParagraphStyles[absa].alignment() == 3) || (m_Doc->docParagraphStyles[absa].alignment() == 4)) && (LiList.count() == 0) && (hl->ch == " "))
			{
				hl->cstyle |= ScStyle_SuppressSpace;
				continue;
			}
			else
				hl->cstyle &= ~ScStyle_SuppressSpace;
			if (LiList.count() == 0)
			{
				if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)) && (!StartOfCol))
				{
					CurY += m_Doc->docParagraphStyles[absa].gapBefore();
					if (chx != QChar(13))
						DropCmode = m_Doc->docParagraphStyles[absa].hasDropCap();
					else
						DropCmode = false;
					if (DropCmode)
					{
						DropLines = m_Doc->docParagraphStyles[absa].dropCapLines();
						if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
							CurY += m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
						else
						{
							if (m_Doc->docParagraphStyles[absa].lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
								CurY += m_Doc->docParagraphStyles[absa].lineSpacing() * (DropLines-1);
							else
								CurY += RealFHeight(m_Doc, hl->font(), m_Doc->docParagraphStyles[absa].charStyle().fontSize()) * (DropLines-1);
						}
					}
				}
			}
			if (DropCmode)
			{
				if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
				{
					chsd = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1)+(hl->font()->numAscent * (m_Doc->docParagraphStyles[hl->cab].charStyle().fontSize() / 10.0))) / (RealCHeight(m_Doc, hl->font(), chx, 10))));
					chs = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1)+(hl->font()->numAscent * (m_Doc->docParagraphStyles[hl->cab].charStyle().fontSize() / 10.0))) / RealCAscent(m_Doc, hl->font(), chx, 10)));
				}
				else
				{
					if (m_Doc->docParagraphStyles[absa].lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
					{
						chsd = qRound(10 * ((m_Doc->docParagraphStyles[absa].lineSpacing() * (DropLines-1)+(hl->font()->numAscent * (m_Doc->docParagraphStyles[hl->cab].charStyle().fontSize() / 10.0))) / (RealCHeight(m_Doc, hl->font(), chx, 10))));
						chs = qRound(10 * ((m_Doc->docParagraphStyles[absa].lineSpacing() * (DropLines-1)+(hl->font()->numAscent * (m_Doc->docParagraphStyles[hl->cab].charStyle().fontSize() / 10.0))) / RealCAscent(m_Doc, hl->font(), chx, 10)));
					}
					else
					{
						double currasce = RealFHeight(m_Doc, hl->font(), m_Doc->docParagraphStyles[hl->cab].charStyle().fontSize());
						chsd = qRound(10 * ((currasce * (DropLines-1)+(hl->font()->numAscent * (m_Doc->docParagraphStyles[hl->cab].charStyle().fontSize() / 10.0))) / (RealCHeight(m_Doc, hl->font(), chx, 10))));
						chs = qRound(10 * ((currasce * (DropLines-1)+(hl->font()->numAscent * (m_Doc->docParagraphStyles[hl->cab].charStyle().fontSize() / 10.0))) / RealCAscent(m_Doc, hl->font(), chx, 10)));
					}
				}
				hl->cstyle |= ScStyle_DropCap;
			}
			else
			{
				if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
					chs = qRound((hl->cembedded->gHeight + hl->cembedded->lineWidth()) * 10);
				else
					chs = hl->fontSize();
			}
			oldCurY = SetZeichAttr(*hl, &chs, &chx);
			if (chx == QChar(29))
				chx2 = " ";
			else if (chx == QChar(24))
				chx2 = "-";
			else
				chx2 = chx;
			if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
				wide = hl->cembedded->gWidth + hl->cembedded->lineWidth();
			else
			{
				if (a < MaxText-1)
				{
					if (itemText.at(a+1)->ch == QChar(29))
						chx3 = " ";
					else if (itemText.at(a+1)->ch == QChar(24))
						chx3 = "-";
					else
						chx3 = itemText.at(a+1)->ch;
					wide = Cwidth(m_Doc, hl->font(), chx2, chs, chx3);
				}
				else
					wide = Cwidth(m_Doc, hl->font(), chx2, chs);
			}
			if (DropCmode)
			{
				if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
				{
					wide = hl->cembedded->gWidth + hl->cembedded->lineWidth();
					if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
						asce = m_Doc->typographicSettings.valueBaseGrid * DropLines;
					else
					{
						if (m_Doc->docParagraphStyles[absa].lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
							asce = m_Doc->docParagraphStyles[absa].lineSpacing() * DropLines;
						else
							asce = RealFHeight(m_Doc, hl->font(), m_Doc->docParagraphStyles[absa].charStyle().fontSize()) * DropLines;
					}
					hl->scaleV() = qRound(asce / (hl->cembedded->gHeight + hl->cembedded->lineWidth()) * 1000.0);
					hl->scaleH() = hl->scaleV();
				}
				else
				{
					wide = RealCWidth(m_Doc, hl->font(), chx2, chsd);
					asce = RealCHeight(m_Doc, hl->font(), chx2, chsd);
				}
				desc2 = 0;
				desc = 0;
			}
			else
			{
				double hlcsize10=hl->fontSize() / 10.0;
				if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
				{
					desc = desc2 = 0;
				}
				else
				{
					desc = desc2 = -hl->font()->numDescender * hlcsize10;
				}
				asce = hl->font()->numAscent * hlcsize10;
			}
			wide = wide * (hl->scaleH() / 1000.0);
			fBorder = false;
			if (CurY+BExtra+lineCorr > Height)
			{
				StartOfCol = true;
				CurrCol++;
				if (CurrCol < Cols)
				{
					ColWidth = columnWidth();
					ColBound = FPoint((ColWidth + ColGap) * CurrCol + Extra+lineCorr, ColWidth * (CurrCol+1) + ColGap * CurrCol + Extra+lineCorr);
					CurX = ColBound.x();
					ColBound = FPoint(ColBound.x(), ColBound.y()+RExtra+lineCorr);
					CurY = asce+TExtra+lineCorr+1;
					if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
					{
						if (chx != QChar(13))
							DropCmode = m_Doc->docParagraphStyles[hl->cab].hasDropCap();
						else
							DropCmode = false;
						if (DropCmode)
						{
							if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
								desc2 = -hl->font()->numDescender * m_Doc->typographicSettings.valueBaseGrid * m_Doc->docParagraphStyles[hl->cab].dropCapLines();
							else
								desc2 = -hl->font()->numDescender * m_Doc->docParagraphStyles[hl->cab].lineSpacing() * m_Doc->docParagraphStyles[hl->cab].dropCapLines();
						}
						if (DropCmode)
							DropLines = m_Doc->docParagraphStyles[hl->cab].dropCapLines();
					}
					if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
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
					goto NoRoom;
				}
			}
			if (LiList.isEmpty())
			{
				startLin = a;
				double TopOffset = asce;
				double BotOffset = desc2;
				goNoRoom = false;
				bool specialCase = false;
				if (StartOfCol)
				{
					CurY = asce+TExtra+lineCorr+1;
//							if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
//								CurY += m_Doc->docParagraphStyles[hl->cab].gapBefore;
				}
				if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
				{
					double by = Ypos;
					if (OwnPage != -1)
						by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
					int ol1 = qRound((by + CurY - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
					int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
					CurY = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
				}
				if (CurY-TopOffset < 0.0)
					CurY = TopOffset+1;
				pt1 = QPoint(static_cast<int>(ceil(CurX-Extra)), static_cast<int>(CurY+BotOffset));
				pt2 = QPoint(static_cast<int>(ceil(CurX-Extra)), static_cast<int>(ceil(CurY-TopOffset)));
				if ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))))
				{
					specialCase = true;
					CurX -= Extra;
				}
				pt1 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(CurY+BotOffset));
				pt2 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(ceil(CurY-TopOffset)));
				while ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))))
				{
					fBorder = true;
					CurX++;
					if (CurX+RExtra+lineCorr > ColBound.y())
					{
						fBorder = false;
						if (StartOfCol)
						{
							CurX = ColBound.x();
							CurY++;
						}
						else
						{
							CurY += m_Doc->docParagraphStyles[hl->cab].lineSpacing();
							CurX = ColBound.x();
						}
						if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
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
								if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
								{
									if (chx != QChar(13))
										DropCmode = m_Doc->docParagraphStyles[hl->cab].hasDropCap();
									else
										DropCmode = false;
									if (DropCmode)
									{
										if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
											desc2 = -hl->font()->numDescender * m_Doc->typographicSettings.valueBaseGrid * m_Doc->docParagraphStyles[hl->cab].dropCapLines();
										else
											desc2 = -hl->font()->numDescender * m_Doc->docParagraphStyles[hl->cab].lineSpacing() * m_Doc->docParagraphStyles[hl->cab].dropCapLines();
									}
									if (DropCmode)
										DropLines = m_Doc->docParagraphStyles[hl->cab].dropCapLines();
								}
								if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
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
								goto NoRoom;
							}
						}
					}
					pt1 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(CurY+BotOffset));
					pt2 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(ceil(CurY-TopOffset)));
				}
				if (((fBorder)) && (!AbsHasDrop))
					CurX += Extra;
				if (a > 0)
				{
					if (itemText.at(a-1)->ch == QChar(13))
						CurX += m_Doc->docParagraphStyles[hl->cab].firstIndent();
				}
				else
				{
					if (BackBox == 0)
						CurX += m_Doc->docParagraphStyles[hl->cab].firstIndent();
					else
					{
						if (BackBox->itemText.count() != 0)
						{
							if (BackBox->itemText.at(BackBox->itemText.count()-1)->ch == QChar(13))
								CurX += m_Doc->docParagraphStyles[hl->cab].firstIndent();
						}
						else
							CurX += m_Doc->docParagraphStyles[hl->cab].firstIndent();
					}
				}
				if (!AbsHasDrop)
					CurX += m_Doc->docParagraphStyles[hl->cab].leftMargin();
				fBorder = false;
			}
			if (RTab)
			{
				if (((hl->ch == ".") && (TabCode == 2)) || ((hl->ch == ",") && (TabCode == 3)) || (hl->ch == QChar(9)))
				{
					RTab = false;
					TabCode = 0;
				}
			}
			if (hl->ch == QChar(9))
			{
				wide = 1;
				if (RTab)
					RTab = false;
				else
				{
					RTabX = CurX+wide;
					if (hl->cab < 5)
						tTabValues = TabValues;
					else
						tTabValues = m_Doc->docParagraphStyles[hl->cab].tabValues();
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
					StartRT = LiList.count();
					StartRT2 = a;
				}
			}
			hl->glyph.yoffset = CurY + oldCurY;
			if (DropCmode)
				hl->glyph.yoffset -= RealCHeight(m_Doc, hl->font(), chx2, chsd) - RealCAscent(m_Doc, hl->font(), chx2, chsd);
			if (LiList.count() == 0)
			{
				itemText.at(a)->cstyle |= ScStyle_StartOfLine;
				kernVal = 0;
			}
			else
			{
				kernVal = chs * hl->tracking() / 10000.0;
				itemText.at(a)->cstyle &= ~ScStyle_StartOfLine;
			}
			if (!RTab)
			{
				hl->glyph.xoffset = QMAX(CurX+kernVal, ColBound.x());
				CurX += wide+kernVal;
				CurX = QMAX(CurX, ColBound.x());
			}
			else
			{
				CurX = QMAX(CurX, ColBound.x());
				hl->glyph.xoffset = CurX;
			}
			if ((TabCode == 4) && (RTab))
			{
				CurX += (wide+kernVal) / 2;
				CurX = QMAX(CurX, ColBound.x());
			}
			if (((hl->effects() & ScStyle_HyphenationPossible) || (hl->ch == "-")) && ((HyphenCount < m_Doc->HyCount) || (m_Doc->HyCount == 0)))
			{
				if (hl->effects() & ScStyle_HyphenationPossible)
				{
					pt1 = QPoint(qRound(ceil(CurX+RExtra+Cwidth(m_Doc, hl->font(), "-", hl->fontSize()) * (hl->scaleH() / 1000.0))), qRound(CurY+desc));
					pt2 = QPoint(qRound(ceil(CurX+RExtra+Cwidth(m_Doc, hl->font(), "-", hl->fontSize()) * (hl->scaleH() / 1000.0))), qRound(ceil(CurY-asce)));
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
			if ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))) || (CurX+RExtra+lineCorr > ColBound.y()))
				outs = true;
			if (CurY > (Height - BExtra - lineCorr))
				outs = true;
			if ((hl->ch == QChar(27)) && (a < itemText.count()-1))
				goNoRoom = true;
			if ((hl->ch == QChar(26)) && (Cols > 1))
				goNextColumn = true;
			Zli = new ZZ;
			Zli->Zeich = chx;
			Zli->Farb = hl->fillColor();
			Zli->shade = hl->fillShade();
			Zli->Farb2 = hl->strokeColor();
			Zli->shade2 = hl->strokeShade();
			Zli->xco = hl->glyph.xoffset;
			Zli->yco = hl->glyph.yoffset;
			Zli->Sele = hl->cselect;
			if (DropCmode)
			{
				Zli->Siz = chsd;
				Zli->realSiz = m_Doc->docParagraphStyles[hl->cab].charStyle().fontSize();
			}
			else
			{
				Zli->Siz = chs;
				Zli->realSiz = hl->fontSize();
			}
			Zli->Style = hl->effects();
			Zli->ZFo = hl->font();
			Zli->wide = wide;
			Zli->kern = kernVal;
			Zli->scale = hl->scaleH();
			Zli->scalev = hl->scaleV();
			Zli->base = hl->baselineOffset();
			Zli->shadowX = hl->shadowXOffset();
			Zli->shadowY = hl->shadowYOffset();
			Zli->outline = hl->outlineWidth();
			Zli->underpos = hl->underlineOffset();
			Zli->underwidth = hl->underlineWidth();
			Zli->strikepos = hl->strikethruOffset();
			Zli->strikewidth = hl->strikethruWidth();
			Zli->embedded = hl->cembedded;
			if (((hl->ch == " ") || (hl->ch == QChar(9))) && (!outs))
			{
				if (a > 0)
				{
					if (itemText.at(a-1)->ch !=  " ")
					{
						LastXp = hl->glyph.xoffset;
						LastSP = BuPos;
					}
				}
				else
				{
					LastXp = hl->glyph.xoffset;
					LastSP = BuPos;
				}
			}
			if (((hl->effects() & ScStyle_HyphenationPossible) || (hl->ch == "-")) && (!outs))
			{
				if ((HyphenCount < m_Doc->HyCount) || (m_Doc->HyCount == 0))
				{
					if (hl->ch == "-")
						LastXp = CurX;
					else
						LastXp = CurX + Cwidth(m_Doc, hl->font(), "-", hl->fontSize()) * (hl->scaleH() / 1000.0);
					LastSP = BuPos;
				}
			}
			LiList.append(Zli);
			if (RTab)
			{
				uint rtx2 = 0;
				double cen = 1;
				if (TabCode == 4)
					cen = 2;
				for (uint rtx = StartRT; rtx < LiList.count(); ++rtx)
				{
					LiList.at(rtx)->xco = QMAX(LiList.at(rtx)->xco-(wide+kernVal) / cen, 0.0);
					itemText.at(StartRT2+rtx2)->glyph.xoffset = QMAX(itemText.at(StartRT2+rtx2)->glyph.xoffset-(wide+kernVal) / cen, 0.0);
					if (itemText.at(StartRT2+rtx2)->glyph.xoffset < RTabX)
					{
						RTab = false;
						TabCode = 0;
					}
					rtx2++;
				}
			}
			BuPos++;
			if (DropCmode)
			{
				DropCmode = false;
				AbsHasDrop = true;
				maxDY = CurY;
				CurX += m_Doc->docParagraphStyles[hl->cab].dropCapOffset();
				CurX = QMAX(CurX, ColBound.x());
				maxDX = CurX;
				QPointArray tcli(4);
				if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
				{
					CurY -= m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
					double by = Ypos;
					if (OwnPage != -1)
						by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
					int ol1 = qRound((by + CurY - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
					int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
					CurY = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
					tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY-DropLines*m_Doc->typographicSettings.valueBaseGrid)));
					tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*m_Doc->typographicSettings.valueBaseGrid)));
				}
				else
				{
					if (m_Doc->docParagraphStyles[absa].lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
					{
						CurY -= m_Doc->docParagraphStyles[absa].lineSpacing() * (DropLines-1);
						tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY-DropLines*m_Doc->docParagraphStyles[absa].lineSpacing())));
						tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*m_Doc->docParagraphStyles[absa].lineSpacing())));
					}
					else
					{
						double currasce = RealFHeight(m_Doc, hl->font(), m_Doc->docParagraphStyles[hl->cab].charStyle().fontSize());
						CurY -= currasce * (DropLines-1);
						tcli.setPoint(0, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY-DropLines*currasce)));
						tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*currasce)));
					}
				}
				tcli.setPoint(2, QPoint(qRound(maxDX), qRound(maxDY)));
				tcli.setPoint(3, QPoint(qRound(hl->glyph.xoffset), qRound(maxDY)));
				cm = QRegion(pf2.xForm(tcli));
				cl = cl.subtract(cm);
			}
			if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27))  || ((hl->ch == QChar(26)) && (Cols > 1)) || (outs))
			{
				RTab = false;
				TabCode = 0;
				if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27))  || ((hl->ch == QChar(26)) && (Cols > 1)))
				{
					if (m_Doc->docParagraphStyles[absa].alignment() != 0)
					{
						EndX = CurX;
						do
						{
							pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
							pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
							EndX++;
						}
						while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
						if (m_Doc->docParagraphStyles[absa].alignment() == 2)
							OFs = EndX - CurX;
						if (m_Doc->docParagraphStyles[absa].alignment() == 1)
							OFs = (EndX - CurX) / 2;
						if (m_Doc->docParagraphStyles[absa].alignment() == 3)
							OFs = 0;
						if (m_Doc->docParagraphStyles[absa].alignment() == 4)
						{
							aSpa = 0;
							for (uint sof = 0; sof<LiList.count(); ++sof)
							{
								if ((LiList.at(sof)->Zeich == QChar(32)) || (LiList.at(sof)->Zeich == QChar(29)))
									aSpa++;
							}
							if (aSpa != 0)
								OFs2 = (EndX - CurX) / aSpa;
							else
								OFs2 = 0;
							OFs = 0;
							for (uint yof = 0; yof < LiList.count(); ++yof)
							{
								LiList.at(yof)->xco += OFs;
								if ((LiList.at(yof)->Zeich == QChar(32)) || (LiList.at(yof)->Zeich == QChar(29)))
									OFs += OFs2;
							}
						}
						else
						{
							for (uint xof = 0; xof<LiList.count(); ++xof)
							{
								LiList.at(xof)->xco += OFs;
							}
						}
						CurX = EndX;
					}
				}
				else
				{
					if (LastSP != 0)            // Hier koenen auch andere Trennungen eingebaut werden
					{
						a -= BuPos - LastSP;
						a++;
						if (itemText.at(a)->effects() & ScStyle_HyphenationPossible)
						{
							HyphenCount++;
							itemText.at(a)->cstyle |= ScStyle_SmartHyphenVisible;
							Zli = new ZZ;
							Zli->Zeich = "-";
							Zli->Farb = itemText.at(a)->fillColor();
							Zli->Farb2 = itemText.at(a)->strokeColor();
							Zli->shade = itemText.at(a)->fillShade();
							Zli->shade2 = itemText.at(a)->strokeShade();
							Zli->xco = LastXp - Cwidth(m_Doc, itemText.at(a)->font(), "-", itemText.at(a)->fontSize()) * (itemText.at(a)->scaleH() / 1000.0);
							Zli->yco = itemText.at(a)->glyph.yoffset;
							Zli->Sele = itemText.at(a)->cselect;
							Zli->Siz = itemText.at(a)->fontSize();
							Zli->realSiz = itemText.at(a)->fontSize();
							Zli->Style = itemText.at(a)->effects();
							Zli->ZFo = itemText.at(a)->font();
							Zli->wide = Cwidth(m_Doc, itemText.at(a)->font(), "-", itemText.at(a)->fontSize()) * (itemText.at(a)->scaleH() / 1000.0);
							Zli->kern = itemText.at(a)->fontSize() * itemText.at(a)->tracking() / 10000.0;
							Zli->scale = itemText.at(a)->scaleH();
							Zli->scalev = itemText.at(a)->scaleV();
							Zli->base = itemText.at(a)->baselineOffset();
							Zli->shadowX = itemText.at(a)->shadowXOffset();
							Zli->shadowY = itemText.at(a)->shadowYOffset();
							Zli->outline = itemText.at(a)->outlineWidth();
							Zli->underpos = itemText.at(a)->underlineOffset();
							Zli->underwidth = itemText.at(a)->underlineWidth();
							Zli->strikepos = itemText.at(a)->strikethruOffset();
							Zli->strikewidth = itemText.at(a)->strikethruWidth();
							Zli->embedded = 0;
							LiList.insert(LastSP+1, Zli);
							LastSP += 1;
						}
						else
						{
							HyphenCount = 0;
							hl->cstyle &= ~ScStyle_SmartHyphenVisible;
						}
						BuPos = LastSP+1;
						if (m_Doc->docParagraphStyles[absa].alignment() != 0)
						{
							EndX = LastXp;
							do
							{
								pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
								pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
								EndX++;
							}
							while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
							if (m_Doc->docParagraphStyles[absa].alignment() == 2)
								OFs = EndX - LastXp;
							if (m_Doc->docParagraphStyles[absa].alignment() == 1)
								OFs = (EndX - LastXp) / 2;
							if ((m_Doc->docParagraphStyles[absa].alignment() == 3) || (m_Doc->docParagraphStyles[absa].alignment() == 4))
							{
								aSpa = 0;
								for (uint sof = 0; sof<BuPos-1; ++sof)
								{
									if ((LiList.at(sof)->Zeich == QChar(32)) || (LiList.at(sof)->Zeich == QChar(29)))
										aSpa++;
								}
								if (aSpa > 1)
									OFs2 = (EndX - LastXp) / aSpa;
								else
								{
									if (aSpa == 0)
										OFs2 = 0;
									else
										OFs2 = (EndX - LastXp);
								}
								OFs = 0;
								for (uint yof = 0; yof < LiList.count(); ++yof)
								{
									LiList.at(yof)->xco += OFs;
									if ((LiList.at(yof)->Zeich == QChar(32)) || (LiList.at(yof)->Zeich == QChar(29)))
										OFs += OFs2;
								}
							}
							else
							{
								for (uint xof = 0; xof<LiList.count(); ++xof)
								{
									LiList.at(xof)->xco += OFs;
								}
							}
							CurX = EndX;
						}
					}
					else
					{
						a--;
						BuPos--;
					}
				}
				uint BuPos3 = BuPos;
				if ((outs) || (hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27)) || ((hl->ch == QChar(26)) && (Cols > 1)))
				{
					if ((outs) && (CurX+RExtra+lineCorr < ColBound.y()))
					{
						if (((hl->ch == QChar(13)) || (hl->ch == QChar(28))) && (AbsHasDrop))
						{
							AbsHasDrop = false;
							if (CurY < maxDY)
								CurY = maxDY;
						}
						bool fromOut = true;
						double BotOffset = desc+BExtra+lineCorr;
						pt1 = QPoint(qRound(CurX+RExtra), static_cast<int>(CurY+BotOffset));
						pt2 = QPoint(qRound(CurX+RExtra), static_cast<int>(ceil(CurY-asce)));
						while (CurX+RExtra+lineCorr < ColBound.y())
						{
							CurX++;
							if (CurX+RExtra+lineCorr > ColBound.y())
							{
								fromOut = false;
								if (m_Doc->docParagraphStyles[absa].useBaselineGrid())
									CurY += m_Doc->typographicSettings.valueBaseGrid;
								else
									CurY += m_Doc->docParagraphStyles[absa].lineSpacing();
								if ((CurY+desc+BExtra+lineCorr > Height) && (CurrCol+1 == Cols))
								{
									goNoRoom = true;
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
								if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27)) || (hl->ch == QChar(26)))
								{
									if (hl->ch == QChar(13))
										CurY += m_Doc->docParagraphStyles[hl->cab].gapAfter();
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
								break;
							}
							CurX--;
							CurX = QMAX(CurX, ColBound.x());
						}
					}
					else
					{
						if (((hl->ch == QChar(13)) || (hl->ch == QChar(28))) && (AbsHasDrop))
						{
							AbsHasDrop = false;
							if (CurY < maxDY)
								CurY = maxDY;
						}
						if (m_Doc->docParagraphStyles[hl->cab].useBaselineGrid())
							CurY += m_Doc->typographicSettings.valueBaseGrid;
						else
						{
							if (a < MaxText-1)
								CurY += m_Doc->docParagraphStyles[itemText.at(a+1)->cab].lineSpacing();
							else
								CurY += m_Doc->docParagraphStyles[hl->cab].lineSpacing();
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
						if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27)) || (hl->ch == QChar(26)))
						{
							if (hl->ch == QChar(13))
								CurY += m_Doc->docParagraphStyles[hl->cab].gapAfter();
							if (BuPos3 > 0)
								BuPos3 -= 1;
							HyphenCount = 0;
						}
					}
				}
				hl->glyph.xoffset = CurX;
				hl->glyph.yoffset = CurY;
				LiList.at(LiList.count()-1)->xco = hl->glyph.xoffset;
				LiList.at(LiList.count()-1)->yco = hl->glyph.yoffset;
				if (LiList.count() != 0)
				{
					if ((!AbsHasDrop) && (StartOfCol) && (!m_Doc->docParagraphStyles[hl->cab].useBaselineGrid()))
					{
						Zli2 = LiList.at(0);
						double firstasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
						double currasce;
						if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
							currasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
						else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
						else
							currasce = RealCAscent(m_Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz);
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							Zli2 = LiList.at(zc);
							if ((Zli2->Zeich == QChar(9)) || (Zli2->Zeich == QChar(10))
								|| (Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(24))
								|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
								|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
								continue;
							if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
								currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
							else
								currasce = QMAX(currasce, RealCAscent(m_Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz));
						}
						double adj = firstasce - currasce;
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							LiList.at(zc)->yco -= adj;
						}
						CurY -= adj;
					}
					if ((!StartOfCol) && (!m_Doc->docParagraphStyles[hl->cab].useBaselineGrid()) && (m_Doc->docParagraphStyles[hl->cab].lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
					{
						Zli2 = LiList.at(0);
						double firstasce = m_Doc->docParagraphStyles[hl->cab].lineSpacing();
						double currasce = 0;
						if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
						else
							currasce = RealFHeight(m_Doc, Zli2->ZFo, Zli2->realSiz);
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							Zli2 = LiList.at(zc);
							if ((Zli2->Zeich == QChar(9)) || (Zli2->Zeich == QChar(10))
								|| (Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(24))
								|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
								|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
								continue;
							if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
								currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
							else
								currasce = QMAX(currasce, RealFHeight(m_Doc, Zli2->ZFo, Zli2->realSiz));
						}
						double adj = firstasce - currasce;
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							LiList.at(zc)->yco -= adj;
						}
						CurY -= adj;
					}
				}
				StartOfCol = false;
				tabDist = ColBound.x();
				uint tabCc = 0;
				uint loopC = BuPos3;
				if (m_Doc->guidesSettings.showControls)
					loopC++;
				for (uint zc = 0; zc<loopC; ++zc)
				{
					double wide2 = 0;
					Zli2 = LiList.at(zc);
					double xcoZli = Zli2->xco;
					itemText.at(startLin+zc)->glyph.xoffset = Zli2->xco;
					itemText.at(startLin+zc)->glyph.yoffset = Zli2->yco;
					if (itemText.at(startLin+zc)->cab < 5)
						tTabValues = TabValues;
					else
						tTabValues = m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].tabValues();
					if (Zli2->Farb != CommonStrings::None)
					{
						QColor tmp;
						SetFarbe(&tmp, Zli2->Farb, Zli2->shade);
						p->setBrush(tmp);
					}
					desc = Zli2->ZFo->numDescender * (-Zli2->Siz / 10.0);
					asce = Zli2->ZFo->numAscent * (Zli2->Siz / 10.0);
					if ((((Zli2->Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli2->Sele))) && (m_Doc->appMode == modeEdit))
					{
						wide = Zli2->wide;
						p->setFillMode(1);
						p->setBrush(qApp->palette().color(QPalette::Active, QColorGroup::Highlight));
//								p->setBrush(darkBlue);
						p->setLineWidth(0);
						if ((zc > 0) && (Zli2->Zeich == QChar(9)))
						{
							wide2 = LiList.at(zc-1)->wide;
							xcoZli = LiList.at(zc-1)->xco+wide2;
							wide = Zli2->xco - xcoZli + Zli2->wide;
						}
						if (!m_Doc->RePos)
							p->drawRect(xcoZli, qRound(Zli2->yco-asce * (Zli2->scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli2->scalev / 1000.0)));
						p->setBrush(qApp->palette().color(QPalette::Active, QColorGroup::HighlightedText));
//								p->setBrush(white);
					}
					if (Zli2->Farb2 != CommonStrings::None)
					{
						QColor tmp;
						SetFarbe(&tmp, Zli2->Farb2, Zli2->shade2);
						p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
					}
					if (!m_Doc->RePos)
					{
						if ((Zli2->Zeich == QChar(9)) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
						{
							QString tabFillCharQStr(tTabValues[tabCc].tabFillChar);
							double wt = Cwidth(m_Doc, Zli2->ZFo, tabFillCharQStr, Zli2->Siz);
							int coun = static_cast<int>((Zli2->xco - tabDist) / wt);
							double sPos = Zli2->xco - (Zli2->xco - tabDist) + 1;
							Zli = new ZZ;
							Zli->Zeich = tabFillCharQStr;
							Zli->Farb = Zli2->Farb;
							Zli->Farb2 = Zli2->Farb2;
							Zli->shade = Zli2->shade;
							Zli->shade2 = Zli2->shade2;
							Zli->yco = Zli2->yco;
							Zli->Sele = Zli2->Sele;
							Zli->Siz = Zli2->Siz;
							Zli->realSiz = Zli2->realSiz;
							Zli->Style = Zli2->Style;
							Zli->ZFo = Zli2->ZFo;
							Zli->wide = wt;
							Zli->kern = 0;
							Zli->scale = 1000;
							Zli->scalev = 1000;
							Zli->base = Zli2->base;
							Zli->shadowX = Zli2->shadowX;
							Zli->shadowY = Zli2->shadowY;
							Zli->outline = Zli2->outline;
							Zli->underpos = Zli2->underpos;
							Zli->underwidth = Zli2->underwidth;
							Zli->strikepos = Zli2->strikepos;
							Zli->strikewidth = Zli2->strikewidth;
							Zli->embedded = 0;
							for (int cx = 0; cx < coun; ++cx)
							{
								Zli->xco = sPos + wt * cx;
								if (e2.intersects(pf2.xForm(QRect(qRound(Zli->xco),qRound(Zli->yco-asce), qRound(Zli->wide+1), qRound(asce+desc)))))
									DrawZeichenS(p, Zli);
							}
							delete Zli;
						}
						if (Zli2->Zeich == QChar(9))
							tabCc++;
						if (((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28))) && (m_Doc->guidesSettings.showControls) && (LiList.count() != 0))
						{
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco+Zli2->wide),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
							{
								FPointArray points;
								QWMatrix chma, chma2, chma4, chma5;
								double ytrans, xtrans;
								if (Zli2->Zeich == QChar(13))
								{
									points = m_Doc->symReturn.copy();
									if (zc > 0)
										ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.8);
									else
										ytrans = Zli2->yco-m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].lineSpacing()-((Zli2->Siz / 10.0) * 0.8);
								}
								else
								{
									points = m_Doc->symNewLine.copy();
									if (zc > 0)
										ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.4);
									else
										ytrans = Zli2->yco-m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].lineSpacing()-((Zli2->Siz / 10.0) * 0.4);
								}
								if (zc > 0)
									xtrans =  LiList.at(zc-1)->xco + Cwidth(m_Doc, LiList.at(zc-1)->ZFo, LiList.at(zc-1)->Zeich, LiList.at(zc-1)->Siz);
								else
								{
									if (startLin > 0)
										xtrans = itemText.at(startLin-1)->glyph.xoffset + Cwidth(m_Doc, itemText.at(startLin-1)->font(), itemText.at(startLin-1)->ch, itemText.at(startLin-1)->fontSize());
									else
										xtrans = Zli2->xco;
								}
								chma4.translate(xtrans, ytrans);
								chma.scale(Zli2->Siz / 100.0, Zli2->Siz / 100.0);
								chma2.scale(Zli2->scale / 1000.0, Zli2->scalev / 1000.0);
								chma5.scale(p->zoomFactor(), p->zoomFactor());
								points.map(chma * chma2 * chma4 * chma5);
								p->setupTextPolygon(&points);
								p->setFillMode(1);
								p->fillPath();
							}
						}
						if ((m_Doc->guidesSettings.showControls) && (zc == BuPos3))
							break;
						if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
						{
							if (Zli2->Zeich == QChar(25))
								DrawObj_Embedded(p, e, Zli2);
							else
								DrawZeichenS(p, Zli2);
						}
					}
					tabDist = Zli2->xco+Zli2->wide;
				}
				LiList.clear();
				BuPos = 0;
				LastSP = 0;
				LastXp = 0;
				outs = false;
				if (goNoRoom)
				{
					goNoRoom = false;
					nrc = a+1;
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
						goto NoRoom;
					}
				}
			}
		}
//		qDebug(QString("textframe: len=%1, done relayout").arg(itemText.count()));

		// justifying
		if (m_Doc->docParagraphStyles[absa].alignment() != 0)
		{
			EndX = CurX;
			do
			{
				pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
				pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
				EndX++;
			}
			while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
			if (m_Doc->docParagraphStyles[absa].alignment() == 2)
				OFs = EndX - CurX;
			if (m_Doc->docParagraphStyles[absa].alignment() == 1)
				OFs = (EndX - CurX) / 2;
			if (m_Doc->docParagraphStyles[absa].alignment() == 3)
				OFs = 0;
			if (m_Doc->docParagraphStyles[absa].alignment() == 4)
			{
				aSpa = 0;
				for (uint sof = 0; sof<LiList.count(); ++sof)
				{
					if ((LiList.at(sof)->Zeich == QChar(32)) || (LiList.at(sof)->Zeich == QChar(29)))
						aSpa++;
				}
				if (aSpa != 0)
					OFs2 = (EndX - CurX) / aSpa;
				else
					OFs2 = 0;
				OFs = 0;
				for (uint yof = 0; yof < LiList.count(); ++yof)
				{
					LiList.at(yof)->xco += OFs;
					if ((LiList.at(yof)->Zeich == QChar(32)) || (LiList.at(yof)->Zeich == QChar(29)))
						OFs += OFs2;
				}
			}
			else
			{
				for (uint xof = 0; xof<LiList.count(); ++xof)
				{
					LiList.at(xof)->xco += OFs;
				}
			}
		}
		if (LiList.count() != 0)
		{
			if ((!AbsHasDrop) && (StartOfCol) && (!m_Doc->docParagraphStyles[hl->cab].useBaselineGrid()))
			{
				Zli2 = LiList.at(0);
				double firstasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
				double currasce = 0;
				if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
					currasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
				else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
					currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
				else
					currasce = RealCAscent(m_Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz);
				for (uint zc = 0; zc < LiList.count(); ++zc)
				{
					Zli2 = LiList.at(zc);
					if ((Zli2->Zeich == QChar(9)) || (Zli2->Zeich == QChar(10))
						|| (Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(24))
						|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
						|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
						continue;
					if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
						currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
					else
						currasce = QMAX(currasce, RealCAscent(m_Doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz));
				}
				double adj = firstasce - currasce;
				for (uint zc = 0; zc < LiList.count(); ++zc)
				{
					LiList.at(zc)->yco -= adj;
				}
				CurY -= adj;
			}
			if ((!StartOfCol) && (!m_Doc->docParagraphStyles[hl->cab].useBaselineGrid()) && (m_Doc->docParagraphStyles[hl->cab].lineSpacingMode() == ParagraphStyle::AutomaticLineSpacing))
			{
				Zli2 = LiList.at(0);
				double firstasce = m_Doc->docParagraphStyles[hl->cab].lineSpacing();
				double currasce = 0;
				if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
					currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
				else
					currasce = RealFHeight(m_Doc, Zli2->ZFo, Zli2->realSiz);
				for (uint zc = 0; zc < LiList.count(); ++zc)
				{
					Zli2 = LiList.at(zc);
					if ((Zli2->Zeich == QChar(9)) || (Zli2->Zeich == QChar(10))
						|| (Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(24))
						|| (Zli2->Zeich == QChar(26)) || (Zli2->Zeich == QChar(27))
						|| (Zli2->Zeich == QChar(28)) || (Zli2->Zeich == QChar(29)))
						continue;
					if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
						currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->lineWidth()) * (Zli2->scalev / 1000.0));
					else
						currasce = QMAX(currasce, RealFHeight(m_Doc, Zli2->ZFo, Zli2->realSiz));
				}
				double adj = firstasce - currasce;
				for (uint zc = 0; zc < LiList.count(); ++zc)
				{
					LiList.at(zc)->yco -= adj;
				}
				CurY -= adj;
			}
		}
		StartOfCol = false;
		tabDist = ColBound.x();
		uint tabCc = 0;
		// painting
		for (uint zc = 0; zc<LiList.count(); ++zc)
		{
			double wide2 = 0;
			Zli2 = LiList.at(zc);
			double xcoZli = Zli2->xco;
			itemText.at(startLin+zc)->glyph.xoffset = Zli2->xco;
			itemText.at(startLin+zc)->glyph.yoffset = Zli2->yco;
			if (itemText.at(startLin+zc)->cab < 5)
				tTabValues = TabValues;
			else
				tTabValues = m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].tabValues();
			if (Zli2->Farb != CommonStrings::None)
			{
				QColor tmp;
				SetFarbe(&tmp, Zli2->Farb, Zli2->shade);
				p->setBrush(tmp);
			}
			desc = Zli2->ZFo->numDescender * (-Zli2->Siz / 10.0);
			asce = Zli2->ZFo->numAscent * (Zli2->Siz / 10.0);
			if ((((Zli2->Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli2->Sele))) && (m_Doc->appMode == modeEdit))
			{
				wide = Zli2->wide;
				p->setFillMode(1);
				p->setBrush(qApp->palette().color(QPalette::Active, QColorGroup::Highlight));
//						p->setBrush(darkBlue);
				p->setLineWidth(0);
				if ((zc > 0) && (Zli2->Zeich == QChar(9)))
				{
					wide2 = LiList.at(zc-1)->wide;
					xcoZli = LiList.at(zc-1)->xco+wide2;
					wide = Zli2->xco - xcoZli + Zli2->wide;
				}
				if (!m_Doc->RePos)
					p->drawRect(xcoZli, qRound(Zli2->yco-asce * (Zli2->scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli2->scalev / 1000.0)));
				p->setBrush(qApp->palette().color(QPalette::Active, QColorGroup::HighlightedText));
//						p->setBrush(white);
			}
			if (Zli2->Farb2 != CommonStrings::None)
			{
				QColor tmp;
				SetFarbe(&tmp, Zli2->Farb2, Zli2->shade2);
				p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
			}
			if (!m_Doc->RePos)
			{
				if ((Zli2->Zeich == QChar(9)) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
				{
					QString tabFillCharQStr(tTabValues[tabCc].tabFillChar);
					double wt = Cwidth(m_Doc, Zli2->ZFo, tabFillCharQStr, Zli2->Siz);
					int coun = static_cast<int>((Zli2->xco - tabDist) / wt);
					double sPos = Zli2->xco - (Zli2->xco - tabDist) + 1;
					Zli = new ZZ;
					Zli->Zeich = tabFillCharQStr;
					Zli->Farb = Zli2->Farb;
					Zli->Farb2 = Zli2->Farb2;
					Zli->shade = Zli2->shade;
					Zli->shade2 = Zli2->shade2;
					Zli->yco = Zli2->yco;
					Zli->Sele = Zli2->Sele;
					Zli->Siz = Zli2->Siz;
					Zli->realSiz = Zli2->realSiz;
					Zli->Style = Zli2->Style;
					Zli->ZFo = Zli2->ZFo;
					Zli->wide = wt;
					Zli->kern = 0;
					Zli->scale = 1000;
					Zli->scalev = 1000;
					Zli->base = Zli2->base;
					Zli->shadowX = Zli2->shadowX;
					Zli->shadowY = Zli2->shadowY;
					Zli->outline = Zli2->outline;
					Zli->underpos = Zli2->underpos;
					Zli->underwidth = Zli2->underwidth;
					Zli->strikepos = Zli2->strikepos;
					Zli->strikewidth = Zli2->strikewidth;
					Zli->embedded = 0;
					for (int cx = 0; cx < coun; ++cx)
					{
						Zli->xco =  sPos + wt * cx;
						if (e2.intersects(pf2.xForm(QRect(qRound(Zli->xco),qRound(Zli->yco-asce), qRound(Zli->wide+1), qRound(asce+desc)))))
							DrawZeichenS(p, Zli);
					}
					delete Zli;
				}
				if (Zli2->Zeich == QChar(9))
					tabCc++;
				if (((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28))) && (m_Doc->guidesSettings.showControls) && (LiList.count() != 0))
				{
					if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco+Zli2->wide),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
					{
						FPointArray points;
						QWMatrix chma, chma2, chma4, chma5;
						double ytrans, xtrans;
						if (Zli2->Zeich == QChar(13))
						{
							points = m_Doc->symReturn.copy();
							if (zc > 0)
								ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.8);
							else
								ytrans = Zli2->yco-m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].lineSpacing()-((Zli2->Siz / 10.0) * 0.8);
						}
						else
						{
							points = m_Doc->symNewLine.copy();
							if (zc > 0)
								ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.4);
							else
								ytrans = Zli2->yco-m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].lineSpacing()-((Zli2->Siz / 10.0) * 0.4);
						}
						if (zc > 0)
							xtrans =  LiList.at(zc-1)->xco + Cwidth(m_Doc, LiList.at(zc-1)->ZFo, LiList.at(zc-1)->Zeich, LiList.at(zc-1)->Siz);
						else
						{
							if (startLin > 0)
								xtrans = itemText.at(startLin-1)->glyph.xoffset + Cwidth(m_Doc, itemText.at(startLin-1)->font(), itemText.at(startLin-1)->ch, itemText.at(startLin-1)->fontSize());
							else
								xtrans = Zli2->xco;
						}
						chma4.translate(xtrans, ytrans);
						chma.scale(Zli2->Siz / 100.0, Zli2->Siz / 100.0);
						chma2.scale(Zli2->scale / 1000.0, Zli2->scalev / 1000.0);
						chma5.scale(p->zoomFactor(), p->zoomFactor());
						points.map(chma * chma2 * chma4 * chma5);
						p->setupTextPolygon(&points);
						p->setFillMode(1);
						p->fillPath();
					}
				}
				if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
				{
					if (Zli2->Zeich == QChar(25))
						DrawObj_Embedded(p, e, Zli2);
					else
						DrawZeichenS(p, Zli2);
				}
			}
			tabDist = Zli2->xco+Zli2->wide;
		}
//		qDebug(QString("textframe: len=%1, done").arg(itemText.count()));

		goNextColumn = false;
		LiList.clear();
		BuPos = 0;
		LastSP = 0;
		outs = false;
	}
	MaxChars = itemText.count();
	Dirty = false;
	pf2.end();
	p->restore();
	return;
	
NoRoom:
//	qDebug(QString("no room in textframe: len=%1").arg(itemText.count()));
	pf2.end();
	MaxChars = nrc;
	if (NextBox != 0)
	{
		nrc2 = itemText.count();
		for (uint ss=nrc; ss<nrc2; ++ss)
		{
			NextBox->itemText.append(itemText.take(nrc));
		}
		if (uint(CPos) > nrc)
		{
			int nCP = CPos - nrc;
			CPos = nrc;
			if ((m_Doc->appMode == modeEdit) && (Tinput))
			{
//							OwnPage->Deselect(true);
				NextBox->CPos = QMAX(nCP, 1);
//							Doc->currentPage = NextBox->OwnPage;
//							NextBox->OwnPage->SelectItemNr(NextBox->ItemNr);
				Dirty = false;
				p->restore();
				return;
			}
		}
//		qDebug(QString("textframe: len=%1, goto nextbox").arg(itemText.count()));
		bool rep = m_Doc->RePos;
		m_Doc->RePos = true;
		NextBox->Dirty = false;
		QPixmap pgPix(1, 1);
		ScPainter *painter = new ScPainter(&pgPix, 1, 1);
		painter->translate(0.5, 0.5);
		NextBox->DrawObj(painter, QRect(0, 0, 1, 1));
		NextBox->Dirty = true;
		painter->end();
		delete painter;
		m_Doc->RePos = rep;
	}
//	else
//	{
//		//CB && added here for jghali prior to commit access
//		if (!m_Doc->RePos && !ScMW->view->previewMode)
//			drawOverflowMarker(p);
//	}
	p->restore();
	Dirty = false;
*/
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
			p->setPen(black, scpInv, DotLine, FlatCap, MiterJoin);
			if ((isBookmark) || (m_isAnnotation))
				p->setPen(blue, scpInv, DotLine, FlatCap, MiterJoin);
			if ((BackBox != 0) || (NextBox != 0))
				p->setPen(red, scpInv, SolidLine, FlatCap, MiterJoin);
			if (m_Locked)
				p->setPen(darkRed, scpInv, SolidLine, FlatCap, MiterJoin);

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
		nextItem = nextItem->NextBox;
	}
	nextItem->itemText.clear();
}

void PageItem_TextFrame::handleModeEditKey(QKeyEvent *k, bool& keyRepeat)
{
	int oldPos = CPos; // 15-mar-2004 jjsa for cursor movement with Shift + Arrow key
	ScText *hg;
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
				ScMW->setTBvals(this);
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
		ScMW->setTBvals(this);
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
			alty =  itemText.item(CPos)->glyph.yoffset - SetZeichAttr(itemText.charStyle(CPos), &nextChs, &nextCh);
			double nextY;
			while (CPos < len-1)
			{
				nextCh = itemText.text(CPos+1, 1);
				nextChs = itemText.charStyle(CPos+1).fontSize();
				nextY = itemText.item(CPos+1)->glyph.yoffset - SetZeichAttr(itemText.charStyle(CPos+1), &nextChs, &nextCh);
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
		ScMW->setTBvals(this);
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
		ScMW->setTBvals(this);
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
		ScMW->setTBvals(this);
		break;
	case Key_Prior:
		CPos = firstInFrame();
		if ( buttonState & ShiftButton )
			ExpandSel(-1, oldPos);
		ScMW->setTBvals(this);
		break;
	case Key_Next:
		CPos = lastInFrame();
		if ( buttonState & ShiftButton )
			ExpandSel(1, oldPos);
		ScMW->setTBvals(this);
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
		ScMW->setTBvals(this);
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
		ScMW->setTBvals(this);
		break;
	case Key_Delete:
		if (CPos == itemText.length())
		{
			if (itemText.lengthOfSelection() > 0)
			{
				deleteSelectedTextFromFrame();
				ScMW->setTBvals(this);
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
			m_Doc->chAbStyle(this, findParagraphStyle(m_Doc, itemText.paragraphStyle(QMAX(CPos-1,0))));
			Tinput = false;
		}
		ScMW->setTBvals(this);
		view->RefreshItem(this);
		break;
	case Key_Backspace:
		if (CPos == 0)
		{
			if (itemText.lengthOfSelection() > 0)
			{
				deleteSelectedTextFromFrame();
				ScMW->setTBvals(this);
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
			m_Doc->chAbStyle(this, findParagraphStyle(m_Doc, itemText.paragraphStyle(QMAX(CPos-1,0))));
			Tinput = false;
		}
		ScMW->setTBvals(this);
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
		if (((uc[0] > QChar(31)) || (as == 13) || (as == 30)) && (m_Doc->currentStyle.charStyle().font()->canRender(uc[0])))
		{
			// should go when hyphenator respects charstyle settings
			QString Language = CPos < itemText.length()? 
				itemText.charStyle(CPos).language() : m_Doc->currentStyle.charStyle().language();
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
					// should go when hyphenator respects charstyle settings
					if (m_Doc->docHyphenator->Language != Language)
						m_Doc->docHyphenator->slotNewDict(Language);
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
	ScMW->DisableTxEdit();
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
			ScMW->DisableTxEdit();
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
		ScMW->EnableTxEdit();
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
		ScMW->DisableTxEdit();
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
	ScMW->DisableTxEdit();
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
	double ofwh = 10 * scp1;
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
