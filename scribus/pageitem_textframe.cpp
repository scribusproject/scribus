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

#include <ft2build.h>
#include FT_GLYPH_H

#include "scfontmetrics.h"
#include "util.h"

using namespace std;

PageItem_TextFrame::PageItem_TextFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::TextFrame, x, y, w, h, w2, fill, outline)
{
	unicodeTextEditMode = false;
	unicodeInputCount = 0;
	unicodeInputString = "";
}

void PageItem_TextFrame::DrawObj_Item(ScPainter *p, QRect e, double sc)
{
	ScribusView* view = m_Doc->view();
	switch (itemType())
	{
		case TextFrame:
		{

			QPainter pp, pf2;
			PageItem *nextItem;
			QPoint pt1, pt2;
			FPoint ColBound;
			QRegion cm;
			uint a, nrc, nrc2, startLin;
			int absa, aSpa, chs, chsd, CurrCol;
			uint BuPos, LastSP, MaxText;
			double oldCurY, LastXp, EndX, OFs, OFs2, wide, lineCorr, ColWidth, kernVal, RTabX;
			QString chx, chx2, chx3;
			struct ScText *hl;
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
			QValueList<TabRecord> tTabValues;
			bool DropCmode = false;
			bool AbsHasDrop = false;
			double desc, asce, maxDY, firstDes, desc2, maxDX, tabDist;
			int DropLines;
			bool StartOfCol = true;
			tTabValues.clear();

			for (int xxx=0; xxx<5; ++xxx)
			{
				m_Doc->docParagraphStyles[xxx].LineSpaMode = LineSpMode;
				if (LineSpMode == 2)
					m_Doc->docParagraphStyles[xxx].BaseAdj = true;
				else
					m_Doc->docParagraphStyles[xxx].BaseAdj = false;
				m_Doc->docParagraphStyles[xxx].LineSpa = LineSp;
				m_Doc->docParagraphStyles[xxx].FontSize = m_FontSize;
				m_Doc->docParagraphStyles[xxx].Indent = 0;
				m_Doc->docParagraphStyles[xxx].First = 0;
				m_Doc->docParagraphStyles[xxx].gapBefore = 0;
				m_Doc->docParagraphStyles[xxx].gapAfter = 0;
				m_Doc->docParagraphStyles[xxx].textAlignment = xxx;
			}

			QPtrList<ZZ> LiList;
			LiList.setAutoDelete(true);
			QRect e2 = QRect(qRound(e.x()  / sc + m_Doc->minCanvasCoordinate.x()), qRound(e.y()  / sc + m_Doc->minCanvasCoordinate.y()), qRound(e.width() / sc), qRound(e.height() / sc));
			p->save();
			pf2.begin(view->viewport());
			pf2.translate(Xpos, Ypos);
			pf2.rotate(Rot);
			if ((fillColor() != "None") || (GrType != 0))
			{
				p->setupPolygon(&PoLine);
				p->fillPath();
			}
			if (lineColor() != "None")
				lineCorr = Pwidth / 2.0;
			else
				lineCorr = 0;
			if ((isAnnotation()) && (annotation().Type() == 2) && (!Pfile.isEmpty()) && (PicAvail) && (PicArt) && (annotation().UseIcons()))
			{
				p->setupPolygon(&PoLine);
				p->setClipPath();
				p->save();
				p->scale(LocalScX, LocalScY);
				p->translate(static_cast<int>(LocalX*LocalScX), static_cast<int>(LocalY*LocalScY));
				if (!pixm.isNull())
					p->drawImage(&pixm);
				p->restore();
			}
			if ((itemText.count() != 0) && (Dirty))
			{
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
				for (a = 0; a < itemText.count(); ++a)
				{
					if (a >= MaxChars)
						break;
					hl = itemText.at(a);
					if (hl->cab < 5)
						tTabValues = TabValues;
					else
						tTabValues = m_Doc->docParagraphStyles[hl->cab].TabValues;
					if (hl->cstyle & 16384)
						tabCc = 0;
					chx = hl->ch;
					if (hl->yp == 0)
						continue;
					if (hl->ch == QChar(30))
						chx = ExpandToken(a);
					if (hl->ccolor != "None")
					{
						QColor tmp;
						SetFarbe(&tmp, hl->ccolor, hl->cshade);
						p->setBrush(tmp);
					}
					if (hl->cstroke != "None")
					{
						QColor tmp;
						SetFarbe(&tmp, hl->cstroke, hl->cshade2);
						p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
					}
					chs = hl->csize;
					if (hl->cstyle & 2048)
					{
						if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
							chs = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (m_Doc->docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (m_Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_Doc, hl->cfont, chx, 10))));
						else
						{
							if (m_Doc->docParagraphStyles[hl->cab].LineSpaMode == 0)
								chs = qRound(10 * ((m_Doc->docParagraphStyles[hl->cab].LineSpa * (m_Doc->docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (m_Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_Doc, hl->cfont, chx, 10))));
							else
							{
								double currasce = RealFHeight(m_Doc, hl->cfont, m_Doc->docParagraphStyles[hl->cab].FontSize);
								chs = qRound(10 * ((currasce * (m_Doc->docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (m_Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCHeight(m_Doc, hl->cfont, chx, 10)));
							}
						}
					}
					oldCurY = SetZeichAttr(hl, &chs, &chx);
					if ((chx == QChar(9)) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
					{
						QString tabFillCharQStr(tTabValues[tabCc].tabFillChar);
						double wt = Cwidth(m_Doc, hl->cfont, tabFillCharQStr, chs);
						int coun = static_cast<int>((hl->xp - tabDist) / wt);
						double sPos = hl->xp - (hl->xp - tabDist) + 1;
						desc = hl->cfont->numDescender * (-chs / 10.0);
						asce = hl->cfont->numAscent * (chs / 10.0);
						Zli3.Zeich = tabFillCharQStr;
						Zli3.Farb = hl->ccolor;
						Zli3.Farb2 = hl->cstroke;
						Zli3.shade = hl->cshade;
						Zli3.shade2 = hl->cshade2;
						Zli3.yco = hl->yp;
						Zli3.Sele = hl->cselect;
						Zli3.Siz = chs;
						Zli3.realSiz = hl->csize;
						Zli3.Style = hl->cstyle;
						Zli3.ZFo = hl->cfont;
						Zli3.wide = wt;
						Zli3.kern = 0;
						Zli3.scale = 1000;
						Zli3.scalev = 1000;
						Zli3.shadowX = hl->cshadowx;
						Zli3.shadowY = hl->cshadowy;
						Zli3.outline = hl->coutline;
						Zli3.base = hl->cbase;
						Zli3.underpos = hl->cunderpos;
						Zli3.underwidth = hl->cunderwidth;
						Zli3.strikepos = hl->cstrikepos;
						Zli3.strikewidth = hl->cstrikewidth;
						for (int cx = 0; cx < coun; ++cx)
						{
							Zli3.xco =  sPos + wt * cx;
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
								DrawZeichenS(p, &Zli3);
						}
					}
					if (chx == QChar(9))
						tabCc++;
					Zli3.Zeich = chx;
					Zli3.Farb = hl->ccolor;
					Zli3.Farb2 = hl->cstroke;
					Zli3.shade = hl->cshade;
					Zli3.shade2 = hl->cshade2;
					Zli3.xco = hl->xp;
					Zli3.yco = hl->yp;
					Zli3.Sele = hl->cselect;
					Zli3.Siz = chs;
					Zli3.realSiz = hl->csize;
					Zli3.Style = hl->cstyle;
					Zli3.ZFo = hl->cfont;
					if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						Zli3.wide = (hl->cembedded->gWidth + hl->cembedded->Pwidth) * (hl->cscale / 1000.0);
					else
						Zli3.wide = Cwidth(m_Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 1000.0);
					if (hl->cstyle & 16384)
						Zli3.kern = 0;
					else
						Zli3.kern = chs * hl->cextra / 10000.0;
					Zli3.scale = hl->cscale;
					Zli3.scalev = hl->cscalev;
					Zli3.base = hl->cbase;
					Zli3.shadowX = hl->cshadowx;
					Zli3.shadowY = hl->cshadowy;
					Zli3.outline = hl->coutline;
					Zli3.underpos = hl->cunderpos;
					Zli3.underwidth = hl->cunderwidth;
					Zli3.strikepos = hl->cstrikepos;
					Zli3.strikewidth = hl->cstrikewidth;
					Zli3.embedded = hl->cembedded;
					if (!m_Doc->RePos)
					{
						double xcoZli = Zli3.xco;
						desc = Zli3.ZFo->numDescender * (-Zli3.Siz / 10.0);
						asce = Zli3.ZFo->numAscent * (Zli3.Siz / 10.0);
						if ((((Zli3.Sele) && (Select)) || (((NextBox != 0) || (BackBox != 0)) && (Zli3.Sele))) && (m_Doc->appMode == modeEdit))
						{
							wide = Zli3.wide;
							p->setFillMode(1);
							p->setBrush(darkBlue);
							p->setLineWidth(0);
							if ((a > 0) && (Zli3.Zeich == QChar(9)))
							{
								xcoZli = itemText.at(a-1)->xp+Cwidth(m_Doc, itemText.at(a-1)->cfont, itemText.at(a-1)->ch, itemText.at(a-1)->csize);
								wide = Zli3.xco - xcoZli + Zli3.wide;
							}
							if (!m_Doc->RePos)
								p->drawRect(xcoZli, qRound(Zli3.yco-asce * (Zli3.scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli3.scalev / 1000.0)));
							p->setBrush(white);
						}
						if (Zli3.Farb2 != "None")
						{
							QColor tmp;
							SetFarbe(&tmp, Zli3.Farb2, Zli3.shade2);
							p->setPen(tmp, 1, SolidLine, FlatCap, MiterJoin);
						}
						if (((chx == QChar(13)) || (chx == QChar(28))) && (m_Doc->guidesSettings.showControls))
						{
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco+Zli3.wide),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
							{
								FPointArray points;
								double ytrans, xtrans;
								if (chx == QChar(13))
								{
									points = m_Doc->symReturn.copy();
									if (a > 0)
										ytrans = itemText.at(a-1)->yp-((Zli3.Siz / 10.0) * 0.8);
									else
										ytrans = Zli3.yco-m_Doc->docParagraphStyles[hl->cab].LineSpa-((Zli3.Siz / 10.0) * 0.8);
								}
								else
								{
									points = m_Doc->symNewLine.copy();
									if (a > 0)
										ytrans = itemText.at(a-1)->yp-((Zli3.Siz / 10.0) * 0.4);
									else
										ytrans = Zli3.yco-m_Doc->docParagraphStyles[hl->cab].LineSpa-((Zli3.Siz / 10.0) * 0.4);
								}
								if (hl->cstyle & 16384)
									xtrans = Zli3.xco;
								else
								{
									if (a > 0)
										xtrans = itemText.at(a-1)->xp+ Cwidth(m_Doc, itemText.at(a-1)->cfont, itemText.at(a-1)->ch, itemText.at(a-1)->csize);
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
						if (hl->cstyle & 8192)
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
				Dirty = false;
				Redrawn = true;
				pf2.end();
				p->restore();
				break;
			}
			if ((itemText.count() != 0) || (NextBox != 0))
			{
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
				m_Doc->docParagraphStyles[0].LineSpa = LineSp;
				QRegion cl = QRegion(pf2.xForm(Clip));
				int LayerLev = m_Doc->layerLevelFromNumber(LayerNr);
				uint docItemsCount=m_Doc->Items->count();
				if (!isEmbedded)
				{
					if (!OnMasterPage.isEmpty())
					{
						Page* Mp = m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]);
						Page* Dp = m_Doc->Pages->at(savedOwnPage);
						for (a = 0; a < m_Doc->MasterItems.count(); ++a)
						{
							PageItem* docItem = m_Doc->MasterItems.at(a);
							int LayerLevItem = m_Doc->layerLevelFromNumber(docItem->LayerNr);
							if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev))
							{
								if (docItem->textFlowsAroundFrame())
								{
									pp.begin(view->viewport());
									pp.translate(docItem->xPos() - Mp->xOffset() + Dp->xOffset(), docItem->yPos() - Mp->yOffset() + Dp->yOffset());
									pp.rotate(docItem->rotation());
									if (docItem->textFlowUsesBoundingBox())
									{
										QPointArray tcli(4);
										tcli.setPoint(0, QPoint(0,0));
										tcli.setPoint(1, QPoint(qRound(docItem->width()), 0));
										tcli.setPoint(2, QPoint(qRound(docItem->width()), qRound(docItem->height())));
										tcli.setPoint(3, QPoint(0, qRound(docItem->height())));
										cm = QRegion(pp.xForm(tcli));
									}
									else
									{
										if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
										{
											QValueList<uint> Segs;
											QPointArray Clip2 = FlattenPath(docItem->ContourLine, Segs);
											cm = QRegion(pp.xForm(Clip2));
										}
										else
											cm = QRegion(pp.xForm(docItem->Clip));
									}
									pp.end();
									cl = cl.subtract(cm);
								}
							}
						}
						if (!m_Doc->masterPageMode())
						{
						for (a = 0; a < docItemsCount; ++a)
						{
							PageItem* docItem = m_Doc->Items->at(a);
							Page* Mp = m_Doc->MasterPages.at(m_Doc->MasterNames[OnMasterPage]);
							Page* Dp = m_Doc->Pages->at(OwnPage);
							if ((docItem->textFlowsAroundFrame()) && (docItem->OwnPage == OwnPage))
							{
								pp.begin(view->viewport());
//								pp.translate(docItem->xPos(), docItem->yPos());
								pp.translate(docItem->xPos() - Mp->xOffset() + Dp->xOffset(), docItem->yPos() - Mp->yOffset() + Dp->yOffset());
								pp.rotate(docItem->rotation());
								if (docItem->textFlowUsesBoundingBox())
								{
									QPointArray tcli(4);
									tcli.setPoint(0, QPoint(0,0));
									tcli.setPoint(1, QPoint(qRound(docItem->width()), 0));
									tcli.setPoint(2, QPoint(qRound(docItem->width()), qRound(docItem->height())));
									tcli.setPoint(3, QPoint(0, qRound(docItem->height())));
									cm = QRegion(pp.xForm(tcli));
								}
								else
								{
									if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
									{
										QValueList<uint> Segs;
										QPointArray Clip2 = FlattenPath(docItem->ContourLine, Segs);
										cm = QRegion(pp.xForm(Clip2));
									}
									else
										cm = QRegion(pp.xForm(docItem->Clip));
								}
								pp.end();
								cl = cl.subtract(cm);
							}
						}
						}
					}
					else
					{
					for (a = 0; a < docItemsCount; ++a)
					{
						PageItem* docItem = m_Doc->Items->at(a);
						int LayerLevItem = m_Doc->layerLevelFromNumber(docItem->LayerNr);
						if (((docItem->ItemNr > ItemNr) && (docItem->LayerNr == LayerNr)) || (LayerLevItem > LayerLev))
						{
							if (docItem->textFlowsAroundFrame())
							{
								pp.begin(view->viewport());
								pp.translate(docItem->xPos(), docItem->yPos());
								pp.rotate(docItem->rotation());
								if (docItem->textFlowUsesBoundingBox())
								{
									QPointArray tcli(4);
									tcli.setPoint(0, QPoint(0,0));
									tcli.setPoint(1, QPoint(qRound(docItem->width()), 0));
									tcli.setPoint(2, QPoint(qRound(docItem->width()), qRound(docItem->height())));
									tcli.setPoint(3, QPoint(0, qRound(docItem->height())));
									cm = QRegion(pp.xForm(tcli));
								}
								else
								{
									if ((docItem->textFlowUsesContourLine()) && (docItem->ContourLine.size() != 0))
									{
										QValueList<uint> Segs;
										QPointArray Clip2 = FlattenPath(docItem->ContourLine, Segs);
										cm = QRegion(pp.xForm(Clip2));
									}
									else
										cm = QRegion(pp.xForm(docItem->Clip));
								}
								pp.end();
								cl = cl.subtract(cm);
							}
						}
					}
					}
				}
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
					if (m_Doc->docParagraphStyles[hl->cab].Drop)
					{
						if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
							chs = qRound(m_Doc->typographicSettings.valueBaseGrid  * m_Doc->docParagraphStyles[hl->cab].DropLin * 10);
						else
							chs = qRound(m_Doc->docParagraphStyles[hl->cab].LineSpa * m_Doc->docParagraphStyles[hl->cab].DropLin * 10);
					}
					else
						chs = hl->csize;
					desc2 = -hl->cfont->numDescender * (chs / 10.0);
					CurY = TExtra+lineCorr;
				}
				else
				{
					desc2 = -(*m_Doc->AllFonts)[m_Font]->numDescender * (m_FontSize / 10.0);
					CurY = m_Doc->docParagraphStyles[0].LineSpa+TExtra+lineCorr-desc2;
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
					chx = hl->ch;
					if (hl->ch == QChar(30))
						chx = ExpandToken(a);
					absa = hl->cab;
					if (m_Doc->docParagraphStyles[absa].LineSpaMode == 1)
						m_Doc->docParagraphStyles[absa].LineSpa = RealFHeight(m_Doc, hl->cfont, hl->csize);
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
										CurY += m_Doc->docParagraphStyles[absa].gapBefore;
										if (chx != QChar(13))
										{
											DropCmode = m_Doc->docParagraphStyles[absa].Drop;
											if (DropCmode)
												DropLines = m_Doc->docParagraphStyles[absa].DropLin;
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
								DropCmode = m_Doc->docParagraphStyles[absa].Drop;
								if (DropCmode)
									DropLines = m_Doc->docParagraphStyles[absa].DropLin;
							}
							else
								DropCmode = false;
							CurY += m_Doc->docParagraphStyles[absa].gapBefore;
						}
					}
					hl->cstyle &= 0xF7FF; // 2047;
					hl->cstyle &= 8191;
					if (((m_Doc->docParagraphStyles[absa].textAlignment == 3) || (m_Doc->docParagraphStyles[absa].textAlignment == 4)) && (LiList.count() == 0) && (hl->ch == " "))
					{
						hl->cstyle |= 4096;
						continue;
					}
					else
						hl->cstyle &= 0xEFFF; // 4095;
					if (LiList.count() == 0)
					{
						if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)) && (!StartOfCol))
						{
							CurY += m_Doc->docParagraphStyles[absa].gapBefore;
							if (chx != QChar(13))
								DropCmode = m_Doc->docParagraphStyles[absa].Drop;
							else
								DropCmode = false;
							if (DropCmode)
							{
								DropLines = m_Doc->docParagraphStyles[absa].DropLin;
								if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
									CurY += m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
								else
								{
									if (m_Doc->docParagraphStyles[absa].LineSpaMode == 0)
										CurY += m_Doc->docParagraphStyles[absa].LineSpa * (DropLines-1);
									else
										CurY += RealFHeight(m_Doc, hl->cfont, m_Doc->docParagraphStyles[absa].FontSize) * (DropLines-1);
								}
							}
						}
					}
					if (DropCmode)
					{
						if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
						{
							chsd = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1)+(hl->cfont->numAscent * (m_Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_Doc, hl->cfont, chx, 10))));
							chs = qRound(10 * ((m_Doc->typographicSettings.valueBaseGrid * (DropLines-1)+(hl->cfont->numAscent * (m_Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCAscent(m_Doc, hl->cfont, chx, 10)));
						}
						else
						{
							if (m_Doc->docParagraphStyles[absa].LineSpaMode == 0)
							{
								chsd = qRound(10 * ((m_Doc->docParagraphStyles[absa].LineSpa * (DropLines-1)+(hl->cfont->numAscent * (m_Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_Doc, hl->cfont, chx, 10))));
								chs = qRound(10 * ((m_Doc->docParagraphStyles[absa].LineSpa * (DropLines-1)+(hl->cfont->numAscent * (m_Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCAscent(m_Doc, hl->cfont, chx, 10)));
							}
							else
							{
								double currasce = RealFHeight(m_Doc, hl->cfont, m_Doc->docParagraphStyles[hl->cab].FontSize);
								chsd = qRound(10 * ((currasce * (DropLines-1)+(hl->cfont->numAscent * (m_Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_Doc, hl->cfont, chx, 10))));
								chs = qRound(10 * ((currasce * (DropLines-1)+(hl->cfont->numAscent * (m_Doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCAscent(m_Doc, hl->cfont, chx, 10)));
							}
						}
						hl->cstyle |= 2048;
					}
					else
					{
						if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
							chs = qRound((hl->cembedded->gHeight + hl->cembedded->Pwidth) * 10);
						else
							chs = hl->csize;
					}
					oldCurY = SetZeichAttr(hl, &chs, &chx);
					if (chx == QChar(29))
						chx2 = " ";
					else if (chx == QChar(24))
						chx2 = "-";
					else
						chx2 = chx;
					if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						wide = hl->cembedded->gWidth + hl->cembedded->Pwidth;
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
							wide = Cwidth(m_Doc, hl->cfont, chx2, chs, chx3);
						}
						else
							wide = Cwidth(m_Doc, hl->cfont, chx2, chs);
					}
					if (DropCmode)
					{
						if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						{
							wide = hl->cembedded->gWidth + hl->cembedded->Pwidth;
							if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
								asce = m_Doc->typographicSettings.valueBaseGrid * DropLines;
							else
							{
								if (m_Doc->docParagraphStyles[absa].LineSpaMode == 0)
									asce = m_Doc->docParagraphStyles[absa].LineSpa * DropLines;
								else
									asce = RealFHeight(m_Doc, hl->cfont, m_Doc->docParagraphStyles[absa].FontSize) * DropLines;
							}
							hl->cscalev = qRound(asce / (hl->cembedded->gHeight + hl->cembedded->Pwidth) * 1000.0);
							hl->cscale = hl->cscalev;
						}
						else
						{
							wide = RealCWidth(m_Doc, hl->cfont, chx2, chsd);
							asce = RealCHeight(m_Doc, hl->cfont, chx2, chsd);
						}
						desc2 = 0;
						desc = 0;
					}
					else
					{
						double hlcsize10=hl->csize / 10.0;
						if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						{
							desc = desc2 = 0;
						}
						else
						{
							desc = desc2 = -hl->cfont->numDescender * hlcsize10;
						}
						asce = hl->cfont->numAscent * hlcsize10;
					}
					wide = wide * (hl->cscale / 1000.0);
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
									DropCmode = m_Doc->docParagraphStyles[hl->cab].Drop;
								else
									DropCmode = false;
								if (DropCmode)
								{
									if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
										desc2 = -hl->cfont->numDescender * m_Doc->typographicSettings.valueBaseGrid * m_Doc->docParagraphStyles[hl->cab].DropLin;
									else
										desc2 = -hl->cfont->numDescender * m_Doc->docParagraphStyles[hl->cab].LineSpa * m_Doc->docParagraphStyles[hl->cab].DropLin;
								}
								if (DropCmode)
									DropLines = m_Doc->docParagraphStyles[hl->cab].DropLin;
							}
							if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
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
						if (StartOfCol)
						{
							CurY = asce+TExtra+lineCorr+1;
//							if (((a > 0) && (itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
//								CurY += m_Doc->docParagraphStyles[hl->cab].gapBefore;
						}
						if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
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
									CurY += m_Doc->docParagraphStyles[hl->cab].LineSpa;
									CurX = ColBound.x();
								}
								if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
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
												DropCmode = m_Doc->docParagraphStyles[hl->cab].Drop;
											else
												DropCmode = false;
											if (DropCmode)
											{
												if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
													desc2 = -hl->cfont->numDescender * m_Doc->typographicSettings.valueBaseGrid * m_Doc->docParagraphStyles[hl->cab].DropLin;
												else
													desc2 = -hl->cfont->numDescender * m_Doc->docParagraphStyles[hl->cab].LineSpa * m_Doc->docParagraphStyles[hl->cab].DropLin;
											}
											if (DropCmode)
												DropLines = m_Doc->docParagraphStyles[hl->cab].DropLin;
										}
										if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
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
						if ((fBorder) && (!AbsHasDrop))
							CurX += Extra;
						if (a > 0)
						{
							if (itemText.at(a-1)->ch == QChar(13))
								CurX += m_Doc->docParagraphStyles[hl->cab].First;
						}
						else
						{
							if (BackBox == 0)
								CurX += m_Doc->docParagraphStyles[hl->cab].First;
							else
							{
								if (BackBox->itemText.count() != 0)
								{
									if (BackBox->itemText.at(BackBox->itemText.count()-1)->ch == QChar(13))
										CurX += m_Doc->docParagraphStyles[hl->cab].First;
								}
								else
									CurX += m_Doc->docParagraphStyles[hl->cab].First;
							}
						}
						if (!AbsHasDrop)
							CurX += m_Doc->docParagraphStyles[hl->cab].Indent;
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
								tTabValues = m_Doc->docParagraphStyles[hl->cab].TabValues;
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
					hl->yp = CurY + oldCurY;
					if (DropCmode)
						hl->yp -= RealCHeight(m_Doc, hl->cfont, chx2, chsd) - RealCAscent(m_Doc, hl->cfont, chx2, chsd);
					if (LiList.count() == 0)
					{
						itemText.at(a)->cstyle |= 16384;
						kernVal = 0;
					}
					else
					{
						kernVal = chs * hl->cextra / 10000.0;
						itemText.at(a)->cstyle &= 16383;
					}
					if (!RTab)
					{
						hl->xp = QMAX(CurX+kernVal, ColBound.x());
						CurX += wide+kernVal;
						CurX = QMAX(CurX, ColBound.x());
					}
					else
					{
						CurX = QMAX(CurX, ColBound.x());
						hl->xp = CurX;
					}
					if ((TabCode == 4) && (RTab))
					{
						CurX += (wide+kernVal) / 2;
						CurX = QMAX(CurX, ColBound.x());
					}
					if (((hl->cstyle & 128) || (hl->ch == "-")) && ((HyphenCount < m_Doc->HyCount) || (m_Doc->HyCount == 0)))
					{
						if (hl->cstyle & 128)
						{
							pt1 = QPoint(qRound(ceil(CurX+RExtra+Cwidth(m_Doc, hl->cfont, "-", hl->csize) * (hl->cscale / 1000.0))), qRound(CurY+desc));
							pt2 = QPoint(qRound(ceil(CurX+RExtra+Cwidth(m_Doc, hl->cfont, "-", hl->csize) * (hl->cscale / 1000.0))), qRound(ceil(CurY-asce)));
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
					Zli->Farb = hl->ccolor;
					Zli->shade = hl->cshade;
					Zli->Farb2 = hl->cstroke;
					Zli->shade2 = hl->cshade2;
					Zli->xco = hl->xp;
					Zli->yco = hl->yp;
					Zli->Sele = hl->cselect;
					if (DropCmode)
					{
						Zli->Siz = chsd;
						Zli->realSiz = m_Doc->docParagraphStyles[hl->cab].FontSize;
					}
					else
					{
						Zli->Siz = chs;
						Zli->realSiz = hl->csize;
					}
					Zli->Style = hl->cstyle;
					Zli->ZFo = hl->cfont;
					Zli->wide = wide;
					Zli->kern = kernVal;
					Zli->scale = hl->cscale;
					Zli->scalev = hl->cscalev;
					Zli->base = hl->cbase;
					Zli->shadowX = hl->cshadowx;
					Zli->shadowY = hl->cshadowy;
					Zli->outline = hl->coutline;
					Zli->underpos = hl->cunderpos;
					Zli->underwidth = hl->cunderwidth;
					Zli->strikepos = hl->cstrikepos;
					Zli->strikewidth = hl->cstrikewidth;
					Zli->embedded = hl->cembedded;
					if (((hl->ch == " ") || (hl->ch == QChar(9))) && (!outs))
					{
						if (a > 0)
						{
							if (itemText.at(a-1)->ch !=  " ")
							{
								LastXp = hl->xp;
								LastSP = BuPos;
							}
						}
						else
						{
							LastXp = hl->xp;
							LastSP = BuPos;
						}
					}
					if (((hl->cstyle & 128) || (hl->ch == "-")) && (!outs))
					{
						if ((HyphenCount < m_Doc->HyCount) || (m_Doc->HyCount == 0))
						{
							if (hl->ch == "-")
								LastXp = CurX;
							else
								LastXp = CurX + Cwidth(m_Doc, hl->cfont, "-", hl->csize) * (hl->cscale / 1000.0);
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
							itemText.at(StartRT2+rtx2)->xp = QMAX(itemText.at(StartRT2+rtx2)->xp-(wide+kernVal) / cen, 0.0);
							if (itemText.at(StartRT2+rtx2)->xp < RTabX)
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
						CurX += m_Doc->docParagraphStyles[hl->cab].DropDist;
						CurX = QMAX(CurX, ColBound.x());
						maxDX = CurX;
						QPointArray tcli(4);
						if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
						{
							CurY -= m_Doc->typographicSettings.valueBaseGrid * (DropLines-1);
							double by = Ypos;
							if (OwnPage != -1)
								by = Ypos - m_Doc->Pages->at(OwnPage)->yOffset();
							int ol1 = qRound((by + CurY - m_Doc->typographicSettings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_Doc->typographicSettings.valueBaseGrid);
							CurY = ceil(  ol2 / 10000.0 ) * m_Doc->typographicSettings.valueBaseGrid + m_Doc->typographicSettings.offsetBaseGrid - by;
							tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*m_Doc->typographicSettings.valueBaseGrid)));
							tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*m_Doc->typographicSettings.valueBaseGrid)));
						}
						else
						{
							if (m_Doc->docParagraphStyles[absa].LineSpaMode == 0)
							{
								CurY -= m_Doc->docParagraphStyles[absa].LineSpa * (DropLines-1);
								tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*m_Doc->docParagraphStyles[absa].LineSpa)));
								tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*m_Doc->docParagraphStyles[absa].LineSpa)));
							}
							else
							{
								double currasce = RealFHeight(m_Doc, hl->cfont, m_Doc->docParagraphStyles[hl->cab].FontSize);
								CurY -= currasce * (DropLines-1);
								tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*currasce)));
								tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*currasce)));
							}
						}
						tcli.setPoint(2, QPoint(qRound(maxDX), qRound(maxDY)));
						tcli.setPoint(3, QPoint(qRound(hl->xp), qRound(maxDY)));
						cm = QRegion(pf2.xForm(tcli));
						cl = cl.subtract(cm);
					}
					if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27))  || ((hl->ch == QChar(26)) && (Cols > 1)) || (outs))
					{
						RTab = false;
						TabCode = 0;
						if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27))  || ((hl->ch == QChar(26)) && (Cols > 1)))
						{
							if (m_Doc->docParagraphStyles[absa].textAlignment != 0)
							{
								EndX = CurX;
								do
								{
									pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
									pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
									EndX++;
								}
								while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
								if (m_Doc->docParagraphStyles[absa].textAlignment == 2)
									OFs = EndX - CurX;
								if (m_Doc->docParagraphStyles[absa].textAlignment == 1)
									OFs = (EndX - CurX) / 2;
								if (m_Doc->docParagraphStyles[absa].textAlignment == 3)
									OFs = 0;
								if (m_Doc->docParagraphStyles[absa].textAlignment == 4)
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
								if (itemText.at(a)->cstyle & 128)
								{
									HyphenCount++;
									itemText.at(a)->cstyle |= 8192;
									Zli = new ZZ;
									Zli->Zeich = "-";
									Zli->Farb = itemText.at(a)->ccolor;
									Zli->Farb2 = itemText.at(a)->cstroke;
									Zli->shade = itemText.at(a)->cshade;
									Zli->shade2 = itemText.at(a)->cshade2;
									Zli->xco = LastXp - Cwidth(m_Doc, itemText.at(a)->cfont, "-", itemText.at(a)->csize) * (itemText.at(a)->cscale / 1000.0);
									Zli->yco = itemText.at(a)->yp;
									Zli->Sele = itemText.at(a)->cselect;
									Zli->Siz = itemText.at(a)->csize;
									Zli->realSiz = itemText.at(a)->csize;
									Zli->Style = itemText.at(a)->cstyle;
									Zli->ZFo = itemText.at(a)->cfont;
									Zli->wide = Cwidth(m_Doc, itemText.at(a)->cfont, "-", itemText.at(a)->csize) * (itemText.at(a)->cscale / 1000.0);
									Zli->kern = itemText.at(a)->csize * itemText.at(a)->cextra / 10000.0;
									Zli->scale = itemText.at(a)->cscale;
									Zli->scalev = itemText.at(a)->cscalev;
									Zli->base = itemText.at(a)->cbase;
									Zli->shadowX = itemText.at(a)->cshadowx;
									Zli->shadowY = itemText.at(a)->cshadowy;
									Zli->outline = itemText.at(a)->coutline;
									Zli->underpos = itemText.at(a)->cunderpos;
									Zli->underwidth = itemText.at(a)->cunderwidth;
									Zli->strikepos = itemText.at(a)->cstrikepos;
									Zli->strikewidth = itemText.at(a)->cstrikewidth;
									Zli->embedded = 0;
									LiList.insert(LastSP+1, Zli);
									LastSP += 1;
								}
								else
								{
									HyphenCount = 0;
									hl->cstyle &= 8191;
								}
								BuPos = LastSP+1;
								if (m_Doc->docParagraphStyles[absa].textAlignment != 0)
								{
									EndX = LastXp;
									do
									{
										pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
										pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
										EndX++;
									}
									while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
									if (m_Doc->docParagraphStyles[absa].textAlignment == 2)
										OFs = EndX - LastXp;
									if (m_Doc->docParagraphStyles[absa].textAlignment == 1)
										OFs = (EndX - LastXp) / 2;
									if ((m_Doc->docParagraphStyles[absa].textAlignment == 3) || (m_Doc->docParagraphStyles[absa].textAlignment == 4))
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
										if (m_Doc->docParagraphStyles[absa].BaseAdj)
											CurY += m_Doc->typographicSettings.valueBaseGrid;
										else
											CurY += m_Doc->docParagraphStyles[absa].LineSpa;
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
												CurY += m_Doc->docParagraphStyles[hl->cab].gapAfter;
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
								if (m_Doc->docParagraphStyles[hl->cab].BaseAdj)
									CurY += m_Doc->typographicSettings.valueBaseGrid;
								else
								{
									if (a < MaxText-1)
										CurY += m_Doc->docParagraphStyles[itemText.at(a+1)->cab].LineSpa;
									else
										CurY += m_Doc->docParagraphStyles[hl->cab].LineSpa;
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
										CurY += m_Doc->docParagraphStyles[hl->cab].gapAfter;
									if (BuPos3 > 0)
										BuPos3 -= 1;
									HyphenCount = 0;
								}
							}
						}
						hl->xp = CurX;
						hl->yp = CurY;
						LiList.at(LiList.count()-1)->xco = hl->xp;
						LiList.at(LiList.count()-1)->yco = hl->yp;
						if (LiList.count() != 0)
						{
							if ((!AbsHasDrop) && (StartOfCol) && (!m_Doc->docParagraphStyles[hl->cab].BaseAdj))
							{
								Zli2 = LiList.at(0);
								double firstasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
								double currasce;
								if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
									currasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
								else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
									currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
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
										currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
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
							if ((!StartOfCol) && (!m_Doc->docParagraphStyles[hl->cab].BaseAdj) && (m_Doc->docParagraphStyles[hl->cab].LineSpaMode == 1))
							{
								Zli2 = LiList.at(0);
								double firstasce = m_Doc->docParagraphStyles[hl->cab].LineSpa;
								double currasce;
								if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
									currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
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
										currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
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
							itemText.at(startLin+zc)->xp = Zli2->xco;
							itemText.at(startLin+zc)->yp = Zli2->yco;
							if (itemText.at(startLin+zc)->cab < 5)
								tTabValues = TabValues;
							else
								tTabValues = m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].TabValues;
							if (Zli2->Farb != "None")
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
								p->setBrush(darkBlue);
								p->setLineWidth(0);
								if ((zc > 0) && (Zli2->Zeich == QChar(9)))
								{
									wide2 = LiList.at(zc-1)->wide;
									xcoZli = LiList.at(zc-1)->xco+wide2;
									wide = Zli2->xco - xcoZli + Zli2->wide;
								}
								if (!m_Doc->RePos)
									p->drawRect(xcoZli, qRound(Zli2->yco-asce * (Zli2->scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli2->scalev / 1000.0)));
								p->setBrush(white);
							}
							if (Zli2->Farb2 != "None")
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
												ytrans = Zli2->yco-m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.8);
										}
										else
										{
											points = m_Doc->symNewLine.copy();
											if (zc > 0)
												ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.4);
											else
												ytrans = Zli2->yco-m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.4);
										}
										if (zc > 0)
											xtrans =  LiList.at(zc-1)->xco + Cwidth(m_Doc, LiList.at(zc-1)->ZFo, LiList.at(zc-1)->Zeich, LiList.at(zc-1)->Siz);
										else
										{
											if (startLin > 0)
												xtrans = itemText.at(startLin-1)->xp + Cwidth(m_Doc, itemText.at(startLin-1)->cfont, itemText.at(startLin-1)->ch, itemText.at(startLin-1)->csize);
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
				if (m_Doc->docParagraphStyles[absa].textAlignment != 0)
				{
					EndX = CurX;
					do
					{
						pt1 = QPoint(qRound(EndX+RExtra), static_cast<int>(CurY+desc));
						pt2 = QPoint(qRound(EndX+RExtra), static_cast<int>(ceil(CurY-asce)));
						EndX++;
					}
					while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+RExtra+lineCorr < ColBound.y()));
					if (m_Doc->docParagraphStyles[absa].textAlignment == 2)
						OFs = EndX - CurX;
					if (m_Doc->docParagraphStyles[absa].textAlignment == 1)
						OFs = (EndX - CurX) / 2;
					if (m_Doc->docParagraphStyles[absa].textAlignment == 3)
						OFs = 0;
					if (m_Doc->docParagraphStyles[absa].textAlignment == 4)
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
					if ((!AbsHasDrop) && (StartOfCol) && (!m_Doc->docParagraphStyles[hl->cab].BaseAdj))
					{
						Zli2 = LiList.at(0);
						double firstasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
						double currasce = 0;
						if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
							currasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
						else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
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
								currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
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
					if ((!StartOfCol) && (!m_Doc->docParagraphStyles[hl->cab].BaseAdj) && (m_Doc->docParagraphStyles[hl->cab].LineSpaMode == 1))
					{
						Zli2 = LiList.at(0);
						double firstasce = m_Doc->docParagraphStyles[hl->cab].LineSpa;
						double currasce = 0;
						if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
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
								currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
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
				for (uint zc = 0; zc<LiList.count(); ++zc)
				{
					double wide2 = 0;
					Zli2 = LiList.at(zc);
					double xcoZli = Zli2->xco;
					itemText.at(startLin+zc)->xp = Zli2->xco;
					itemText.at(startLin+zc)->yp = Zli2->yco;
					if (itemText.at(startLin+zc)->cab < 5)
						tTabValues = TabValues;
					else
						tTabValues = m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].TabValues;
					if (Zli2->Farb != "None")
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
						p->setBrush(darkBlue);
						p->setLineWidth(0);
						if ((zc > 0) && (Zli2->Zeich == QChar(9)))
						{
							wide2 = LiList.at(zc-1)->wide;
							xcoZli = LiList.at(zc-1)->xco+wide2;
							wide = Zli2->xco - xcoZli + Zli2->wide;
						}
						if (!m_Doc->RePos)
							p->drawRect(xcoZli, qRound(Zli2->yco-asce * (Zli2->scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli2->scalev / 1000.0)));
						p->setBrush(white);
					}
					if (Zli2->Farb2 != "None")
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
										ytrans = Zli2->yco-m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.8);
								}
								else
								{
									points = m_Doc->symNewLine.copy();
									if (zc > 0)
										ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.4);
									else
										ytrans = Zli2->yco-m_Doc->docParagraphStyles[itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.4);
								}
								if (zc > 0)
									xtrans =  LiList.at(zc-1)->xco + Cwidth(m_Doc, LiList.at(zc-1)->ZFo, LiList.at(zc-1)->Zeich, LiList.at(zc-1)->Siz);
								else
								{
									if (startLin > 0)
										xtrans = itemText.at(startLin-1)->xp + Cwidth(m_Doc, itemText.at(startLin-1)->cfont, itemText.at(startLin-1)->ch, itemText.at(startLin-1)->csize);
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
				goNextColumn = false;
				LiList.clear();
				BuPos = 0;
				LastSP = 0;
				outs = false;
			}
			MaxChars = itemText.count();
			Redrawn = true;
			Dirty = false;
			pf2.end();
			p->restore();
			break;
			NoRoom: pf2.end();
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
						break;
					}
				}
				p->save();
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
				p->restore();
				m_Doc->RePos = rep;
			}/*
			else
			{
				//CB && added here for jghali prior to commit access
				if (!m_Doc->RePos && !ScMW->view->previewMode)
					drawOverflowMarker(p);
			}*/
			MaxChars = nrc;
			Redrawn = true;
			p->restore();
		}
			break;
		default:
			break;
	}
	Dirty = false;
}

void PageItem_TextFrame::DrawObj_Post(ScPainter *p)
{
	ScribusView* view = m_Doc->view();
	if (!m_Doc->RePos)
	{
		if (lineColor() != "None")
		{
			p->setPen(strokeQColor, Pwidth, PLineArt, PLineEnd, PLineJoin);
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
	}
	if ((!isEmbedded) && (!m_Doc->RePos))
	{
		double scpInv = 1.0 / (QMAX(view->getScale(), 1));
		if ((Frame) && (m_Doc->guidesSettings.framesShown) && ((itemType() == ImageFrame) || (itemType() == TextFrame) || (itemType() == PathText)))
		{
			p->setPen(black, scpInv, DotLine, FlatCap, MiterJoin);
			if ((isBookmark) || (m_isAnnotation))
				p->setPen(blue, scpInv, DotLine, FlatCap, MiterJoin);
			if ((BackBox != 0) || (NextBox != 0))
				p->setPen(red, scpInv, SolidLine, FlatCap, MiterJoin);
			if (Locked)
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
		if (itemText.count() > MaxChars)
		{//CB && added here for jghali prior to commit access
			if (!view->previewMode)
				drawOverflowMarker(p);
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
	//FIXME use PageItem_TextFrame pointers
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
		for (ScText *it = nextItem->itemText.first(); it != 0; it = nextItem->itemText.next())
		{
			if ((it->ch == QChar(25)) && (it->cembedded != 0))
			{
				m_Doc->FrameItems.remove(it->cembedded);
				delete it->cembedded;
			}
		}
		nextItem->itemText.clear();
		nextItem->CPos = 0;
		nextItem = nextItem->NextBox;
	}
}

/**
 * Handle keyboard interaction with the text frame while in edit mode
 * @param k 
 */
void PageItem_TextFrame::handleModeEditKey(QKeyEvent *k, bool& keyRepeat)
{
	int oldPos = CPos; // 15-mar-2004 jjsa for cursor movement with Shift + Arrow key
	struct ScText *hg;
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
				if (HasSel)
					deleteSelectedTextFromFrame();
				if (conv < 31)
					conv = 32;
				hg = new ScText;
				hg->ch = QString(QChar(conv));
				m_Doc->setScTextDefaultsFromDoc(hg);
				hg->cselect = false;
				hg->cextra = 0;
				hg->xp = 0;
				hg->yp = 0;
				hg->PRot = 0;
				hg->PtransX = 0;
				hg->PtransY = 0;
				hg->cembedded = 0;
				itemText.insert(CPos, hg);
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
		if ( (pos = CPos) == 0 )
			break; // at begin of frame
		len = static_cast<int>(itemText.count());
		if ( pos == len )
			pos--;
		if ( (buttonState & ControlButton) == 0 )
		{
			alty =  itemText.at(pos)->yp;
			c = itemText.at(pos)->ch.at(0).latin1();
			if ( c == 13 ) // new line, position is wrong
				if ( --pos > 0 )
					alty =  itemText.at(pos)->yp;
			// check for yp at actual position
			if ( pos < len )
			{
				altx =  itemText.at(pos)->yp;
				if ( altx > alty )
				{
					// we was at begin of line
					break;
				}
			}
			while (  pos > 0 && itemText.at(pos-1)->yp == alty )
				--pos;
			if ( itemText.at(pos)->ch.at(0).latin1() == 13 )
				++pos;
		}
		else
		{
			// paragraph begin
			if ( pos < len &&
					itemText.at(pos)->ch.at(0).latin1() == 13 )
				--pos;
			while(pos > 0 )
				if ( itemText.at(pos)->ch.at(0).latin1() == 13 )
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
		break;
	case Key_End:
		// go to end of line
		len = static_cast<int>(itemText.count());
		if ( CPos >= len )
			break; // at end of frame
		if ( (buttonState & ControlButton) == 0 )
		{
			if ((CPos < len) && ((itemText.at(CPos)->ch.at(0).latin1() == 13) || (itemText.at(CPos)->ch.at(0).latin1() == 28)))
			{
				// at end of paragraph and therefore line
				break;
			}
			QString nextCh = itemText.at(CPos)->ch;
			int nextChs = itemText.at(CPos)->csize;
			alty =  itemText.at(CPos)->yp - SetZeichAttr(itemText.at(CPos), &nextChs, &nextCh);
			double nextY;
			while (CPos < len-1)
			{
				nextCh = itemText.at(CPos+1)->ch;
				nextChs = itemText.at(CPos+1)->csize;
				nextY = itemText.at(CPos+1)->yp - SetZeichAttr(itemText.at(CPos+1), &nextChs, &nextCh);
				if (fabs(nextY - alty) > 1.0)
					break;
				CPos++;
				if ( CPos == len-1)
					break;
			}
			if ( CPos < len -1 )
				c = itemText.at(CPos+1)->ch.at(0).latin1();
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
			// go to end of paragraph
			if ( itemText.at(CPos)->ch.at(0).latin1() == 13 )
			{
				break;
			}
			pos = CPos;
			while ( pos < len )
			{
				if ( itemText.at(pos)->ch.at(0).latin1() == 13 )
					break;
				else
					++pos;
			}
			CPos = pos;
		}
		if ( buttonState & ShiftButton )
			ExpandSel(1, oldPos);
		break;
	case Key_Down:
		if (CPos != static_cast<int>(itemText.count()))
		{
			bool down1=false;
			double newy;
			newy = alty = itemText.at(CPos)->yp;
			altx = itemText.at(CPos)->xp;
			do
			{
				++CPos;
				if (CPos == static_cast<int>(itemText.count()))
					break;
				if (itemText.at(CPos)->yp > alty)
				{
					if (down1 && itemText.at(CPos)->yp > newy)
					{
						--CPos;
						break;
					}
					if (itemText.at(CPos)->xp >= altx)
						break;
					down1=true;
				}
				//CB Make the cursor move forward a column
				if (itemText.at(CPos)->yp < alty)
				{
					double newX=altx+columnWidth();
					while (itemText.at(CPos)->xp<newX && CPos < static_cast<int>(itemText.count())-1)
						++CPos;
					break;
				}
				newy=itemText.at(CPos)->yp;
			}
			while (CPos < static_cast<int>(itemText.count()));
			if ( buttonState & ShiftButton )
			{
				if ( buttonState & AltButton )
					CPos = itemText.count();
				ExpandSel(1, oldPos);
			}
			else
				if (CPos == static_cast<int>(itemText.count()))
					if (NextBox != 0)
					{
						if (NextBox->itemText.count() != 0)
						{
							view->Deselect(true);
							NextBox->CPos = 0;
							view->SelectItemNr(NextBox->ItemNr);
							//currItem = currItem->NextBox;
						}
					}
		}
		else
		{
			if (NextBox != 0)
			{
				if (NextBox->itemText.count() != 0)
				{
					view->Deselect(true);
					NextBox->CPos = 0;
					view->SelectItemNr(NextBox->ItemNr);
					//currItem = currItem->NextBox;
				}
			}
		}
		if ( this->HasSel )
			view->RefreshItem(this);
		ScMW->setTBvals(this);
		break;
	case Key_Up:
		if (CPos > 0)
		{
			if (CPos == static_cast<int>(itemText.count()))
				--CPos;
			alty = itemText.at(CPos)->yp;
			altx = itemText.at(CPos)->xp;
			if (CPos > 0)
			{
				do
				{
					--CPos;
					if (CPos == 0)
						break;
					if  ( itemText.at(CPos)->ch.at(0).latin1() == 13 )
						break;
					if (itemText.at(CPos)->yp < alty)
					{
						if (itemText.at(CPos)->xp <= altx)
							break;
					}
					//CB Make the cursor move back a column
					if (itemText.at(CPos)->yp > alty)
					{
						double newX=altx-columnWidth();
						while (itemText.at(CPos)->xp>newX && CPos > 0)
							--CPos;
						break;
					}
				}
				while (CPos > 0);
			}
			if ( buttonState & ShiftButton )
			{
				if ( buttonState & AltButton )
					CPos = 0;
				ExpandSel(-1, oldPos);
			}
			else
				if (CPos == 0)
				{
					if (BackBox != 0)
					{
						view->Deselect(true);
						BackBox->CPos = BackBox->itemText.count();
						view->SelectItemNr(BackBox->ItemNr);
						//currItem = currItem->BackBox;
					}
				}
		}
		else
		{
			CPos = 0;
			if (BackBox != 0)
			{
				view->Deselect(true);
				BackBox->CPos = BackBox->itemText.count();
				view->SelectItemNr(BackBox->ItemNr);
				//currItem = currItem->BackBox;
			}
		}
		if ( this->HasSel )
			view->RefreshItem(this);
		ScMW->setTBvals(this);
		break;
	case Key_Prior:
		CPos = 0;
		if ( buttonState & ShiftButton )
			ExpandSel(-1, oldPos);
		ScMW->setTBvals(this);
		break;
	case Key_Next:
		CPos = static_cast<int>(itemText.count());
		if ( buttonState & ShiftButton )
			ExpandSel(1, oldPos);
		ScMW->setTBvals(this);
		break;
	case Key_Left:
		if ( buttonState & ControlButton )
		{
			setNewPos(oldPos, itemText.count(),-1);
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
			if (CPos < 0)
			{
				CPos = 0;
				if (BackBox != 0)
				{
					view->Deselect(true);
					BackBox->CPos = BackBox->itemText.count();
					view->SelectItemNr(BackBox->ItemNr);
					//currItem = currItem->BackBox;
				}
			}
		}
		if ((CPos > 0) && (CPos == static_cast<int>(itemText.count())))
		{
			if (itemText.at(CPos-1)->cstyle & 4096)
			{
				--CPos;
				while ((CPos > 0) && (itemText.at(CPos)->cstyle & 4096))
				{
					--CPos;
					if (CPos == 0)
						break;
				}
			}
		}
		else
		{
			while ((CPos > 0) && (itemText.at(CPos)->cstyle & 4096))
			{
				--CPos;
				if (CPos == 0)
					break;
			}
		}
		if ( HasSel )
			view->RefreshItem(this);
		ScMW->setTBvals(this);
		break;
	case Key_Right:
		if ( buttonState & ControlButton )
		{
			setNewPos(oldPos, itemText.count(),1);
			if ( buttonState & ShiftButton )
				ExpandSel(1, oldPos);
		}
		else if ( buttonState & ShiftButton )
		{
			++CPos;
			if ( CPos > static_cast<int>(itemText.count()) )
				--CPos;
			else
				ExpandSel(1, oldPos);
		}
		else
		{
			++CPos; // new position within text ?
			if (CPos > static_cast<int>(itemText.count()))
			{
				--CPos;
				if (NextBox != 0)
				{
					if (NextBox->itemText.count() != 0)
					{
						view->Deselect(true);
						NextBox->CPos = 0;
						view->SelectItemNr(NextBox->ItemNr);
						//currItem = currItem->NextBox;
					}
				}
			}
		}
		if ( HasSel )
			view->RefreshItem(this);
		ScMW->setTBvals(this);
		break;
	case Key_Delete:
		if (CPos == static_cast<int>(itemText.count()))
		{
			if (HasSel)
			{
				deleteSelectedTextFromFrame();
				ScMW->setTBvals(this);
				view->RefreshItem(this);
			}
			keyRepeat = false;
			return;
		}
		if (itemText.count() == 0)
		{
			keyRepeat = false;
			return;
		}
		cr = itemText.at(CPos)->ch;
		if (!HasSel)
			itemText.at(CPos)->cselect = true;
		deleteSelectedTextFromFrame();
		Tinput = false;
		if ((cr == QChar(13)) && (itemText.count() != 0))
		{
			view->chAbStyle(this, itemText.at(QMAX(CPos-1,0))->cab);
			Tinput = false;
		}
		ScMW->setTBvals(this);
		view->RefreshItem(this);
		break;
	case Key_Backspace:
		if (CPos == 0)
		{
			if (HasSel)
			{
				deleteSelectedTextFromFrame();
				ScMW->setTBvals(this);
				view->RefreshItem(this);
			}
			break;
		}
		if (itemText.count() == 0)
			break;
		cr = itemText.at(QMAX(CPos-1,0))->ch;
		if (!HasSel)
		{
			--CPos;
			itemText.at(CPos)->cselect = true;
		}
		deleteSelectedTextFromFrame();
		Tinput = false;
		if ((cr == QChar(13)) && (itemText.count() != 0))
		{
			view->chAbStyle(this, itemText.at(QMAX(CPos-1,0))->cab);
			Tinput = false;
		}
		ScMW->setTBvals(this);
		view->RefreshItem(this);
		break;
	default:
		if ((HasSel) && (kk < 0x1000))
			deleteSelectedTextFromFrame();
		//if ((kk == Key_Tab) || ((kk == Key_Return) && (buttonState & ShiftButton)))
		if (kk == Key_Tab)
		{
			hg = new ScText;
			//	if (kk == Key_Return)
			//		hg->ch = QString(QChar(28));
			if (kk == Key_Tab)
				hg->ch = QString(QChar(9));
			m_Doc->setScTextDefaultsFromDoc(hg);
			hg->cselect = false;
			hg->cextra = 0;
			hg->xp = 0;
			hg->yp = 0;
			hg->PRot = 0;
			hg->PtransX = 0;
			hg->PtransY = 0;
			hg->cembedded = 0;
			itemText.insert(CPos, hg);
			CPos += 1;
			Tinput = true;
			view->RefreshItem(this);
			break;
		}
		if (((uc[0] > QChar(31)) || (as == 13) || (as == 30)) && ((*m_Doc->AllFonts)[m_Doc->CurrFont]->CharWidth.contains(uc[0].unicode())))
		{
			hg = new ScText;
			hg->ch = uc;
			m_Doc->setScTextDefaultsFromDoc(hg);
			hg->cextra = 0;
			hg->cselect = false;
			hg->xp = 0;
			hg->yp = 0;
			hg->PRot = 0;
			hg->PtransX = 0;
			hg->PtransY = 0;
			hg->cembedded = 0;
			itemText.insert(CPos, hg);
			CPos += 1;
			if ((m_Doc->docHyphenator->AutoCheck) && (CPos > 1))
			{
				Twort = "";
				Tcoun = 0;
				for (int hych = CPos-1; hych > -1; hych--)
				{
					Tcha = itemText.at(hych)->ch;
					if (Tcha == " ")
					{
						Tcoun = hych+1;
						break;
					}
					Twort.prepend(Tcha);
				}
				if (!Twort.isEmpty())
				{
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
	int firstSelection = 0;
	bool first = false;
	for (ScText *it = itemText.first(); it != 0; it = itemText.next())
	{
		if (it->cselect)
		{
			first = true;
			if ((it->ch == QChar(25)) && (it->cembedded != 0))
			{
				m_Doc->FrameItems.remove(it->cembedded);
				delete it->cembedded;
			}
			itemText.remove();
			it = itemText.prev();
			if (it == 0)
				it = itemText.first();
		}
		if (!first)
			firstSelection++;
	}
	if (itemText.count() != 0)
	{
		if (itemText.first()->cselect)
		{
			itemText.remove();
			CPos = 0;
		}
		else
			CPos = firstSelection;
	}
	else
		CPos = 0;
	HasSel = false;
	m_Doc->updateFrameItems();
	ScMW->DisableTxEdit();
}

// jjsa added on 15-mar-2004
// calculate the end position while ctrl + arrow pressed

void PageItem_TextFrame::setNewPos(int oldPos, int len, int dir)
{
	int i;
	bool isSpace;
	if ( dir > 0 && oldPos < len )
	{
		isSpace = itemText.at(oldPos)->ch.at(0).isSpace();
		CPos = oldPos +1;
		for (i=oldPos+1; i < len; i++)
		{
			if ( itemText.at(i)->ch.at(0).isSpace() != isSpace )
				break;
			CPos++;
		}
	}
	else if ( dir < 0 && oldPos > 0 )
	{
		oldPos--;
		isSpace = itemText.at(oldPos)->ch.at(0).isSpace();
		for (i=oldPos; i >= 0; i--)
		{
			if (  itemText.at(i)->ch.at(0).isSpace() != isSpace )
				break;
			CPos--;
		}
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
	int len = itemText.count();
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
	if ( HasSel ) /* selection already present */
	{
		if (dir > 0 && oldPos < len) // -> key
		{
			if ( oldPos == 0 )
				leftSel = false;
			else
				leftSel = itemText.at(oldPos-1)->cselect;
			actSel = itemText.at(oldPos)->cselect;
			rightSel = false; // not relevant
		}
		else if ( dir > 0 && oldPos == len) // -> key
		{
			return;
		}
		else if ( dir < 0 && oldPos > 0 ) // <- key
		{
			actSel = itemText.at(oldPos-1)->cselect;
			leftSel = false; // not relevant
			if ( oldPos < len  )
				rightSel = itemText.at(oldPos)->cselect;
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
         // deselect all
			int i;
			for (i=0; i < len; i++ )
			{
				itemText.at(i)->cselect = false;
			}
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
	for ( i = start; i < end; i++)
		itemText.at(i)->cselect = sel;
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
		if ( item->HasSel )
		{
			uint l = item->itemText.count();
			for (uint n=0; n < l; ++n )
				item->itemText.at(n)->cselect = false;
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
	if (lineColor() != "None")
		lineCorr = Pwidth;
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
	double scpwidth16 = Width - scp16;
	double scpheight16 = Height - scp16;
	double scpwidth3 = Width - scp3;
	double scpheight3 = Height - scp3;
	p->setPen(black, scp1, SolidLine, FlatCap, MiterJoin);
	p->setBrush(white);
	p->drawRect(scpwidth16, scpheight16, scp14, scp14);
	p->drawLine(FPoint(scpwidth16, scpheight16), FPoint(scpwidth3, scpheight3));
	p->drawLine(FPoint(scpwidth16, scpheight3), FPoint(scpwidth3, scpheight16));
	*/
	//TODO: CB clean
	ScribusView* view = m_Doc->view();
	double scp1 = 1 ;// / ScMW->view->getScale();
	double ofwh = 6 * scp1;
	//CB moved down while locked marker disabled
	//double ofx = Width - ofwh/2;
	//double ofy = Height - ofwh*3.0;
	double ofx = Width - ofwh/2;
	double ofy = Height - ofwh*1.5;
	double lx1= ofx;
	double ly1= ofy;
	double lx2= ofx+ofwh;
	double ly2= ofy+ofwh;
	p->setPen(black, 0.5/ view->getScale(), SolidLine, FlatCap, MiterJoin);
	p->setPenOpacity(1.0);
	p->setBrush(Qt::white);
	p->setBrushOpacity(1.0);
	p->setFillMode(ScPainter::Solid);
	p->drawRect(ofx, ofy, ofwh, ofwh);
	p->drawLine(FPoint(lx1, ly1), FPoint(lx2, ly2));
	p->drawLine(FPoint(lx1, ly2), FPoint(lx2, ly1));
}


