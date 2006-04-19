/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scpageoutput.h"

#include "qpainter.h"
#include "pageitem.h"
#include "commonstrings.h"
#include "pageitem_imageframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_textframe.h"
#include "scfontmetrics.h"
#include "scribus.h"
#include "scimage.h"
#include "util.h"

ScPageOutput::ScPageOutput(ScribusDoc* doc, bool reloadImages, int resolution, bool useProfiles)
{
	m_doc = doc;
	m_reloadImages = reloadImages;
	m_imageRes = resolution;
	m_useProfiles = useProfiles;
}

void ScPageOutput::DrawPage( Page* page, ScPainterExBase* painter)
{
	int clipx = static_cast<int>(page->xOffset());
	int clipy = static_cast<int>(page->yOffset());
	int clipw = qRound(page->width());
	int cliph = qRound(page->height());
	DrawMasterItems(painter, page, QRect(clipx, clipy, clipw, cliph));
	DrawPageItems(painter, page, QRect(clipx, clipy, clipw, cliph));
}

void ScPageOutput::DrawMasterItems(ScPainterExBase *painter, Page *page, QRect clip)
{
	double z = painter->zoomFactor();
	if (!page->MPageNam.isEmpty())
	{
		Page* Mp = m_doc->MasterPages.at(m_doc->MasterNames[page->MPageNam]);
		if (page->FromMaster.count() != 0)
		{
			int Lnr;
			struct Layer ll;
			PageItem *currItem;
			ll.isViewable = false;
			ll.LNr = 0;
			Lnr = 0;
			uint layerCount = m_doc->layerCount();
			for (uint la = 0; la < layerCount; ++la)
			{
				Level2Layer(m_doc, &ll, Lnr);
				bool pr = true;
				if ( !ll.isPrintable )
					pr = false;
				if ((ll.isViewable) && (pr))
				{
					uint pageFromMasterCount=page->FromMaster.count();
					for (uint a = 0; a < pageFromMasterCount; ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->pageNr())))
							continue;
						if (!currItem->printEnabled())
							continue;
						int savedOwnPage = currItem->OwnPage;
						double OldX = currItem->xPos();
						double OldY = currItem->yPos();
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						currItem->OwnPage = page->pageNr();
						if (!currItem->ChangedMasterItem)
						{
							currItem->moveBy(-Mp->xOffset() + page->xOffset(), -Mp->yOffset() + page->yOffset());
							currItem->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
							currItem->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
						}
						/*if (evSpon)
							currItem->Dirty = true;*/
						QRect oldR(currItem->getRedrawBounding(m_scale));
						if (clip.intersects(oldR))
							DrawItem(currItem, painter, clip);
						currItem->OwnPage = savedOwnPage;
						if (!currItem->ChangedMasterItem)
						{
							currItem->setXPos(OldX);
							currItem->setYPos(OldY);
							currItem->BoundingX = OldBX;
							currItem->BoundingY = OldBY;
						}
					}
					for (uint a = 0; a < pageFromMasterCount; ++a)
					{
						currItem = page->FromMaster.at(a);
						if (currItem->LayerNr != ll.LNr)
							continue;
						if (!currItem->isTableItem)
							continue;
						if ((currItem->OwnPage != -1) && (currItem->OwnPage != static_cast<int>(Mp->pageNr())))
							continue;
						double OldX = currItem->xPos();
						double OldY = currItem->yPos();
						double OldBX = currItem->BoundingX;
						double OldBY = currItem->BoundingY;
						if (!currItem->ChangedMasterItem)
						{
							currItem->setXPos(OldX - Mp->xOffset() + page->xOffset());
							currItem->setYPos(OldY - Mp->yOffset() + page->yOffset());
							currItem->BoundingX = OldBX - Mp->xOffset() + page->xOffset();
							currItem->BoundingY = OldBY - Mp->yOffset() + page->yOffset();
						}
						QRect oldR(currItem->getRedrawBounding(m_scale));
						if (clip.intersects(oldR))
						{
							painter->setZoomFactor(m_scale);
							painter->save();
							painter->translate(currItem->xPos() * m_scale, currItem->yPos() * m_scale);
							painter->rotate(currItem->rotation());
							if (currItem->lineColor() != CommonStrings::None)
							{
								ScColorShade tmp( m_doc->PageColors[currItem->lineColor()], currItem->lineShade());
								if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
								{
									painter->setPen(tmp, currItem->lineWidth(), currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
									if (currItem->TopLine)
										painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->width(), 0.0));
									if (currItem->RightLine)
										painter->drawLine(FPoint(currItem->width(), 0.0), FPoint(currItem->width(), currItem->height()));
									if (currItem->BottomLine)
										painter->drawLine(FPoint(currItem->width(), currItem->height()), FPoint(0.0, currItem->height()));
									if (currItem->LeftLine)
										painter->drawLine(FPoint(0.0, currItem->height()), FPoint(0.0, 0.0));
								}
							}
							painter->restore();
						}
						if (!currItem->ChangedMasterItem)
						{
							currItem->setXPos(OldX);
							currItem->setYPos(OldY);
							currItem->BoundingX = OldBX;
							currItem->BoundingY = OldBY;
						}
					}
				}
				Lnr++;
			}
		}
	}
	painter->setZoomFactor(z);
}

void ScPageOutput::DrawPageItems(ScPainterExBase *painter, Page *page, QRect clip)
{
	//linkedFramesToShow.clear();
	double z = painter->zoomFactor();
	if (m_doc->Items->count() != 0)
	{
		//QPainter p;
		int Lnr=0;
		struct Layer ll;
		PageItem *currItem;
		ll.isViewable = false;
		ll.LNr = 0;
		uint layerCount = m_doc->layerCount();
		//int docCurrPageNo=static_cast<int>(m_doc->currentPageNumber());
		int docCurrPageNo=static_cast<int>(page->pageNr());
		for (uint la2 = 0; la2 < layerCount; ++la2)
		{
			Level2Layer(m_doc, &ll, Lnr);
			bool pr = true;
			if (!ll.isPrintable)
				pr = false;
			if ((ll.isViewable) && (pr))
			{
				QPtrListIterator<PageItem> docItem(*m_doc->Items);
				while ( (currItem = docItem.current()) != 0)
				{
					++docItem;
					if (currItem->LayerNr != ll.LNr)
						continue;
					if (!currItem->printEnabled())
						continue;
					if ((m_doc->masterPageMode()) && ((currItem->OwnPage != -1) && (currItem->OwnPage != docCurrPageNo)))
						continue;
					if (!m_doc->masterPageMode() && !currItem->OnMasterPage.isEmpty())
					{
						if (currItem->OnMasterPage != page->PageNam)
							continue;
					}
					QRect oldR(currItem->getRedrawBounding(m_scale));
					if (clip.intersects(oldR))
					{
						/*if (evSpon)
							currItem->Dirty = true;*/
						/*if (forceRedraw)
							currItem->Dirty = false;*/
//						if ((!Mpressed) || (m_doc->EditClip))
							DrawItem( currItem, painter, clip );
						//currItem->Redrawn = true;
						if ((currItem->asTextFrame()) && ((currItem->NextBox != 0) || (currItem->BackBox != 0)))
						{
							PageItem *nextItem = currItem;
							while (nextItem != 0)
							{
								if (nextItem->BackBox != 0)
									nextItem = nextItem->BackBox;
								else
									break;
							}
							/*if (linkedFramesToShow.find(nextItem) == -1)
								linkedFramesToShow.append(nextItem);*/
						}
					}
				}
				QPtrListIterator<PageItem> docItem2(*m_doc->Items);
				while ( (currItem = docItem2.current()) != 0 )
				{
					++docItem2;
					if (currItem->LayerNr != ll.LNr)
						continue;
					if (!currItem->isTableItem)
						continue;
					QRect oldR(currItem->getRedrawBounding(m_scale));
					if (clip.intersects(oldR))
					{
						painter->setZoomFactor(m_scale);
						painter->save();
						painter->translate(currItem->xPos() * m_scale, currItem->yPos() * m_scale);
						painter->rotate(currItem->rotation());
						if (currItem->lineColor() != CommonStrings::None)
						{
							ScColorShade tmp( m_doc->PageColors[currItem->lineColor()], currItem->lineShade() );
							if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
							{
								painter->setPen(tmp, currItem->lineWidth(), currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
								if (currItem->TopLine)
									painter->drawLine(FPoint(0.0, 0.0), FPoint(currItem->width(), 0.0));
								if (currItem->RightLine)
									painter->drawLine(FPoint(currItem->width(), 0.0), FPoint(currItem->width(), currItem->height()));
								if (currItem->BottomLine)
									painter->drawLine(FPoint(currItem->width(), currItem->height()), FPoint(0.0, currItem->height()));
								if (currItem->LeftLine)
									painter->drawLine(FPoint(0.0, currItem->height()), FPoint(0.0, 0.0));
							}
						}
						painter->restore();
					}
				}
			}
			Lnr++;
		}
	}
	painter->setZoomFactor(z);
}

void ScPageOutput::DrawItem( PageItem* item, ScPainterExBase* painter, QRect rect )
{
	double sc = m_scale;
	DrawItem_Pre(item, painter, sc);
	PageItem::ItemType itemType = item->itemType();
	if( itemType == PageItem::ImageFrame )
		DrawItem_ImageFrame( (PageItem_ImageFrame*) item, painter, sc );
	else if( itemType == PageItem::Line )
		DrawItem_Line( (PageItem_Line*) item, painter );
	else if( itemType == PageItem::PathText )
		DrawItem_PathText(  (PageItem_PathText*) item, painter, sc );
	else if( itemType == PageItem::Polygon )
		DrawItem_Polygon( (PageItem_Polygon*) item, painter );
	else if( itemType == PageItem::PolyLine )
		DrawItem_PolyLine( (PageItem_PolyLine*) item, painter );
	else if( itemType == PageItem::TextFrame )
		DrawItem_TextFrame( (PageItem_TextFrame*) item, painter, rect, sc);
	DrawItem_Post(item, painter);
}

void ScPageOutput::DrawItem_Pre( PageItem* item, ScPainterExBase* painter, double scale  )
{
	double sc = scale;
	painter->save();
	if (!item->isEmbedded)
	{
		painter->setZoomFactor(sc);
		painter->translate( item->xPos() * sc, item->yPos() * sc);
//		painter->rotate(item->rotation());
	}
	painter->rotate(item->rotation());
	painter->setLineWidth(item->lineWidth());
	if (item->GrType != 0)
	{
		painter->setFillMode(ScPainterExBase::Gradient);
		painter->fill_gradient = VGradientEx(item->fill_gradient, *m_doc);
		QWMatrix grm;
		grm.rotate(item->rotation());
		FPointArray gra;
		switch (item->GrType)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 6:
				gra.setPoints(2, item->GrStartX, item->GrStartY, item->GrEndX, item->GrEndY);
				gra.map(grm);
				painter->setGradient(VGradientEx::linear, gra.point(0), gra.point(1));
				break;
			case 5:
			case 7:
				gra.setPoints(2, item->GrStartX, item->GrStartY, item->GrEndX, item->GrEndY);
				painter->setGradient(VGradientEx::radial, gra.point(0), gra.point(1), gra.point(0));
				break;
		}
	}
	else
	{
		painter->fill_gradient = VGradientEx(VGradientEx::linear);
		if (item->fillColor() != CommonStrings::None)
		{
			painter->setBrush( ScColorShade(m_doc->PageColors[item->fillColor()], item->fillShade()) );
			painter->setFillMode(ScPainterExBase::Solid);
		}
		else
			painter->setFillMode(ScPainterExBase::None);
	}
	if (item->lineColor() != CommonStrings::None)
	{
		if ((item->lineWidth() == 0) && !item->asLine())
			painter->setLineWidth(0);
		else
		{
			ScColorShade tmp(m_doc->PageColors[item->lineColor()], item->lineShade());
			painter->setPen( tmp , item->lineWidth(), item->PLineArt, item->PLineEnd, item->PLineJoin);
			if (item->DashValues.count() != 0)
				painter->setDash(item->DashValues, item->DashOffset);
		}
	}
	else
		painter->setLineWidth(0);
	painter->setBrushOpacity(1.0 - item->fillTransparency());
	painter->setPenOpacity(1.0 - item->lineTransparency());
	painter->setFillRule(item->fillRule);
}

void ScPageOutput::DrawItem_Post( PageItem* item, ScPainterExBase* painter )
{
	bool doStroke=true;
	if ( item->itemType() == PageItem::PathText || item->itemType() == PageItem::PolyLine || item->itemType() == PageItem::Line )
		doStroke=false;
	if ((doStroke))
	{
		if (item->lineColor() != CommonStrings::None)
		{
			ScColorShade tmp(m_doc->PageColors[item->lineColor()], item->lineShade());
			painter->setPen(tmp, item->lineWidth(), item->PLineArt, item->PLineEnd, item->PLineJoin);
			if (item->DashValues.count() != 0)
				painter->setDash(item->DashValues, item->DashOffset);
		}
		else
			painter->setLineWidth(0);
		if (!item->isTableItem)
		{
			painter->setupPolygon(&item->PoLine);
			if (item->NamedLStyle.isEmpty())
				painter->strokePath();
			else
			{
				multiLine ml = m_doc->MLineStyles[item->NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					ScColorShade tmp( m_doc->PageColors[ml[it].Color], ml[it].Shade );
					painter->setPen(tmp, ml[it].Width,
							static_cast<Qt::PenStyle>(ml[it].Dash),
							static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
							static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
					painter->strokePath();
				}
			}
		}
	}
	if ((!item->isEmbedded))
	{
		double scpInv = 1.0 / (QMAX(m_scale, 1));
	}
	item->Tinput = false;
	item->FrameOnly = false;
	painter->restore();
}

void ScPageOutput::DrawCharacters( PageItem* item, ScPainterExBase *painter, struct PageItem::ZZ *hl)
{
	double csi = static_cast<double>(hl->Siz) / 100.0;
	QString ccx = hl->Zeich;
	if ((ccx == QChar(13)) || (ccx == QChar(9)) || (ccx == QChar(28)) || (ccx == QChar(27)) || (ccx == QChar(26)))
		return;
	if (ccx == QChar(29))
		ccx = " ";
	if (ccx == QChar(24))
		ccx = "-";
	uint chr = ccx[0].unicode();
	if (hl->ZFo->CharWidth.contains(chr))
	{
		QWMatrix chma, chma2, chma3, chma4, chma5, chma6;
		chma.scale(csi, csi);
		chma5.scale(painter->zoomFactor(), painter->zoomFactor());
		FPointArray gly = hl->ZFo->GlyphArray[chr].Outlines.copy();
		if (gly.size() > 3)
		{
			chma2.scale(hl->scale / 1000.0, hl->scalev / 1000.0);
			if (item->reversed())
			{
				chma3.scale(-1, 1);
				chma3.translate(-hl->wide, 0);
				chma4.translate(hl->xco, hl->yco-((hl->Siz / 10.0) * (hl->scalev / 1000.0)));
			}
			else
				chma4.translate(hl->xco, hl->yco-((hl->Siz / 10.0) * (hl->scalev / 1000.0)));
			if (hl->base != 0)
				chma6.translate(0, -(hl->Siz / 10.0) * (hl->base / 1000.0) * painter->zoomFactor());
			gly.map(chma * chma2 * chma3 * chma4 * chma5 * chma6);
			painter->setFillMode(1);
			bool fr = painter->fillRule();
			painter->setFillRule(false);
			painter->setupTextPolygon(&gly);
			if ((hl->ZFo->isStroked) && ((hl->Siz * hl->outline / 10000.0) != 0))
			{
				painter->setPen(painter->brush(), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				painter->setLineWidth(hl->Siz * hl->outline / 10000.0);
				painter->strokePath();
			}
			else
			{
				if ((hl->Style & 256) && (hl->Farb2 != CommonStrings::None))
				{
					painter->save();
					painter->translate((hl->Siz * hl->shadowX / 10000.0) * painter->zoomFactor(), -(hl->Siz * hl->shadowY / 10000.0) * painter->zoomFactor());
					ScColorShade tmp = painter->brush();
					painter->setBrush(painter->pen());
					painter->setupTextPolygon(&gly); // Necessary if path is not part of the graphic state
					painter->fillPath();
					painter->setBrush(tmp);
					painter->restore();
					painter->setupTextPolygon(&gly); // Necessary if path is not part of the graphic state
				}
				if (hl->Farb != CommonStrings::None)
					painter->fillPath();
				if ((hl->Style & 4) && (hl->Farb2 != CommonStrings::None) && ((hl->Siz * hl->outline / 10000.0) != 0))
				{
					painter->setLineWidth(hl->Siz * hl->outline / 10000.0);
					painter->strokePath();
				}
			}
			painter->setFillRule(fr);
		}
		if (hl->Style & 16)
		{
			double st, lw;
			if ((hl->strikepos != -1) || (hl->strikewidth != -1))
			{
				if (hl->strikepos != -1)
					st = (hl->strikepos / 1000.0) * (hl->ZFo->numAscent * (hl->realSiz / 10.0));
				else
					st = hl->ZFo->strikeout_pos * (hl->realSiz / 10.0);
				if (hl->strikewidth != -1)
					lw = (hl->strikewidth / 1000.0) * (hl->realSiz / 10.0);
				else
					lw = QMAX(hl->ZFo->strokeWidth * (hl->realSiz / 10.0), 1);
			}
			else
			{
				st = hl->ZFo->strikeout_pos * (hl->realSiz / 10.0);
				lw = QMAX(hl->ZFo->strokeWidth * (hl->realSiz / 10.0), 1);
			}
			if (hl->base != 0)
				st += (hl->Siz / 10.0) * (hl->base / 1000.0);
			painter->setPen(painter->brush());
			painter->setLineWidth(lw);
			painter->drawLine(FPoint(hl->xco-hl->kern, hl->yco-st), FPoint(hl->xco+hl->wide, hl->yco-st));
		}
		if ((hl->Style & 8) || ((hl->Style & 512) && (!ccx[0].isSpace())))
		{
			double st, lw;
			if ((hl->underpos != -1) || (hl->underwidth != -1))
			{
				if (hl->underpos != -1)
					st = (hl->underpos / 1000.0) * (hl->ZFo->numDescender * (hl->realSiz / 10.0));
				else
					st = hl->ZFo->underline_pos * (hl->realSiz / 10.0);
				if (hl->underwidth != -1)
					lw = (hl->underwidth / 1000.0) * (hl->realSiz / 10.0);
				else
					lw = QMAX(hl->ZFo->strokeWidth * (hl->realSiz / 10.0), 1);
			}
			else
			{
				st = hl->ZFo->underline_pos * (hl->realSiz / 10.0);
				lw = QMAX(hl->ZFo->strokeWidth * (hl->realSiz / 10.0), 1);
			}
			if (hl->base != 0)
				st += (hl->Siz / 10.0) * (hl->base / 1000.0);
			painter->setPen(painter->brush());
			painter->setLineWidth(lw);
			painter->drawLine(FPoint(hl->xco-hl->kern, hl->yco-st), FPoint(hl->xco+hl->wide, hl->yco-st));
		}
	}
	else
	{
		ScColorShade colorRed( ScColor(255, 0, 0), 100 );
		painter->setLineWidth(1);
		painter->setPen(colorRed);
		painter->setBrush(colorRed);
		painter->setFillMode(1);
		painter->drawRect(hl->xco, hl->yco-(hl->Siz / 10.0), (hl->Siz / 10.0)*(hl->scale / 1000.0), (hl->Siz / 10.0));
	}
}

void ScPageOutput::DrawItem_Embedded( PageItem* item, ScPainterExBase *p, QRect e, struct PageItem::ZZ *hl)
{
	QPtrList<PageItem> emG;
	emG.clear();
	if (hl->embedded != 0)
	{
		if (!m_doc->DoDrawing)
		{
			hl->embedded->Redrawn = true;
			hl->embedded->Tinput = false;
			hl->embedded->FrameOnly = false;
			return;
		}
		emG.append(hl->embedded);
		if (hl->embedded->Groups.count() != 0)
		{
			for (uint ga=0; ga<m_doc->FrameItems.count(); ++ga)
			{
				if (m_doc->FrameItems.at(ga)->Groups.count() != 0)
				{
					if (m_doc->FrameItems.at(ga)->Groups.top() == hl->embedded->Groups.top())
					{
						if (m_doc->FrameItems.at(ga)->ItemNr != hl->embedded->ItemNr)
						{
							if (emG.find(m_doc->FrameItems.at(ga)) == -1)
								emG.append(m_doc->FrameItems.at(ga));
						}
					}
				}
			}
		}
		for (uint em = 0; em < emG.count(); ++em)
		{
			PageItem* embedded = emG.at(em);
			ParagraphStyle vg;
			QValueList<ParagraphStyle> savedParagraphStyles;
			for (int xxx=0; xxx<5; ++xxx)
			{
				vg.setLineSpacingMode( (ParagraphStyle::LineSpacingMode) m_doc->docParagraphStyles[xxx].lineSpacingMode() );
				vg.setUseBaselineGrid( m_doc->docParagraphStyles[xxx].useBaselineGrid() );
				vg.setLineSpacing( m_doc->docParagraphStyles[xxx].lineSpacing() );
				vg.charStyle().csize = m_doc->docParagraphStyles[xxx].charStyle().csize;
				vg.setLeftMargin( m_doc->docParagraphStyles[xxx].leftMargin() );
				vg.setFirstIndent( m_doc->docParagraphStyles[xxx].firstIndent());
				vg.setGapBefore( m_doc->docParagraphStyles[xxx].gapBefore() );
				vg.setGapAfter( m_doc->docParagraphStyles[xxx].gapAfter() );
				savedParagraphStyles.append(vg);
			}
			p->save();
			embedded->setXPos(embedded->xPos() + hl->xco + embedded->gXpos);
			embedded->setYPos(embedded->yPos() + (hl->yco - (embedded->gHeight * (hl->scalev / 1000.0))) + embedded->gYpos);
			p->translate((hl->xco + embedded->gXpos * (hl->scale / 1000.0)) * p->zoomFactor(), (hl->yco - (embedded->gHeight * (hl->scalev / 1000.0)) + embedded->gYpos * (hl->scalev / 1000.0)) * p->zoomFactor());
			if (hl->base != 0)
			{
				p->translate(0, -embedded->gHeight * (hl->base / 1000.0) * p->zoomFactor());
				embedded->setYPos(embedded->yPos() - embedded->gHeight * (hl->base / 1000.0));
			}
			p->scale(hl->scale / 1000.0, hl->scalev / 1000.0);
			//embedded->Dirty = Dirty;
			double sc = 1;
			double pws = embedded->lineWidth();
			DrawItem_Pre(embedded, p, sc);
			switch(embedded->itemType())
			{
				case PageItem::ImageFrame:
					DrawItem_ImageFrame((PageItem_ImageFrame*) embedded, p, sc);
					break;
				case PageItem::TextFrame:
					DrawItem_TextFrame((PageItem_TextFrame*) embedded, p, e, sc);
					break;
				case PageItem::Line:
					embedded->setLineWidth(pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0));
					DrawItem_Line((PageItem_Line*) embedded, p);
					break;
				case PageItem::Polygon:
					DrawItem_Polygon((PageItem_Polygon*) embedded, p);
					break;
				case PageItem::PolyLine:
					embedded->setLineWidth(pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0));
					DrawItem_PolyLine((PageItem_PolyLine*) embedded, p);
					break;
				case PageItem::PathText:
					DrawItem_PathText( (PageItem_PathText*) embedded, p, sc);
					break;
				default:
					break;
			}
			embedded->setLineWidth(pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0));
			DrawItem_Post(embedded, p);
			p->restore();
			embedded->setLineWidth(pws);
			for (int xxx=0; xxx<5; ++xxx)
			{
				m_doc->docParagraphStyles[xxx].setLineSpacingMode( (ParagraphStyle::LineSpacingMode) savedParagraphStyles[xxx].lineSpacingMode() );
				m_doc->docParagraphStyles[xxx].setUseBaselineGrid( savedParagraphStyles[xxx].useBaselineGrid() );
				m_doc->docParagraphStyles[xxx].setLineSpacing( savedParagraphStyles[xxx].lineSpacing() );
				m_doc->docParagraphStyles[xxx].charStyle().csize = savedParagraphStyles[xxx].charStyle().csize;
				m_doc->docParagraphStyles[xxx].setLeftMargin( savedParagraphStyles[xxx].leftMargin() );
				m_doc->docParagraphStyles[xxx].setFirstIndent( savedParagraphStyles[xxx].firstIndent() );
				m_doc->docParagraphStyles[xxx].setGapBefore( savedParagraphStyles[xxx].gapBefore() );
				m_doc->docParagraphStyles[xxx].setGapAfter( savedParagraphStyles[xxx].gapAfter() );
			}
			savedParagraphStyles.clear();
		}
	}
}

void ScPageOutput::DrawItem_ImageFrame( PageItem_ImageFrame* item, ScPainterExBase* painter, double scale  )
{
	ScPainterExBase::ImageMode mode = ScPainterExBase::rgbImages;
	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		painter->setupPolygon(&item->PoLine);
		painter->fillPath();
	}
	if (item->Pfile.isEmpty())
	{
		if ((item->Frame) && (m_doc->guidesSettings.framesShown))
		{
			painter->setPen( ScColorShade(Qt::black, 100), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			painter->drawLine(FPoint(0, 0), FPoint(item->width(), item->height()));
			painter->drawLine(FPoint(0, item->height()), FPoint(item->width(), 0));
		}
	}
	else
	{
		if ((!item->imageShown()) || (!item->PicAvail))
		{
			if ((item->Frame) && (m_doc->guidesSettings.framesShown))
			{
				painter->setPen( ScColorShade(Qt::red, 100), 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
				painter->drawLine(FPoint(0, 0), FPoint(item->width(), item->height()));
				painter->drawLine(FPoint(0, item->height()), FPoint(item->width(), 0));
			}
		}
		else
		{
			ScImage scImg;
			ScImage* pImage = NULL;
			double imScaleX = item->imageXScale();
			double imScaleY = item->imageYScale();
			if( m_reloadImages )
			{
				bool dummy;
				bool useCmyk = false;
				ScPainterExBase::ImageMode imageMode = painter->imageMode();
				if ( imageMode == ScPainterExBase::cmykImages )
					useCmyk = true;
				QFileInfo fInfo(item->Pfile);
				QString ext = fInfo.extension(false);
				scImg.imgInfo.valid = false;
				scImg.imgInfo.clipPath = "";
				scImg.imgInfo.PDSpathData.clear();
				scImg.imgInfo.layerInfo.clear();
				scImg.imgInfo.RequestProps = item->pixm.imgInfo.RequestProps;
				scImg.imgInfo.isRequest = item->pixm.imgInfo.isRequest;
				scImg.LoadPicture(item->Pfile, item->IProfile, 0, item->UseEmbedded, m_useProfiles, (int) imageMode, m_imageRes, &dummy);
				if( ext == "eps" || ext == "pdf" || ext == "ps" )
				{
					imScaleX *= (72.0 / (double) m_imageRes);
					imScaleY *= (72.0 / (double) m_imageRes);
				}
				scImg.applyEffect(item->effectsInUse, m_doc->PageColors, useCmyk);
				mode = imageMode;
				pImage = &scImg;
			}
			else
				pImage = &item->pixm;

			painter->save();
			if (item->imageClip.size() != 0)
			{
				painter->setupPolygon(&item->imageClip);
				painter->setClipPath();
			}
			painter->setupPolygon(&item->PoLine);
			painter->setClipPath();
			if (item->imageFlippedH())
			{
				painter->translate(item->width() * scale, 0);
				painter->scale(-1, 1);
			}
			if (item->imageFlippedV())
			{
				painter->translate(0, item->height() * scale);
				painter->scale(1, -1);
			}
			painter->translate(item->imageXOffset() * item->imageXScale() * scale, item->imageYOffset() * item->imageYScale() * scale);
			//painter->translate(item->LocalX * imScaleX * scale, item->LocalY * imScaleY * scale); ??
			painter->scale( imScaleX, imScaleY );
			if (pImage->imgInfo.lowResType != 0)
				painter->scale(pImage->imgInfo.lowResScale, pImage->imgInfo.lowResScale);
			painter->drawImage(pImage, mode);
			painter->restore();
		}
	}
}

void ScPageOutput::DrawItem_Line( PageItem_Line* item, ScPainterExBase* painter )
{
 int startArrowIndex;
 int endArrowIndex;

	startArrowIndex = item->startArrowIndex();
	endArrowIndex = item->endArrowIndex();

	if (item->NamedLStyle.isEmpty())
		painter->drawLine(FPoint(0, 0), FPoint(item->width(), 0));
	else
	{
		multiLine ml = m_doc->MLineStyles[item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			ScColorShade colorShade(m_doc->PageColors[ml[it].Color], ml[it].Shade);
			painter->setPen(colorShade, ml[it].Width,
						static_cast<Qt::PenStyle>(ml[it].Dash),
						static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
						static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
			painter->drawLine(FPoint(0, 0), FPoint(item->width(), 0));
		}
	}
	if (startArrowIndex != 0)
	{
		QWMatrix arrowTrans;
		FPointArray arrow = ( *m_doc->arrowStyles.at(startArrowIndex - 1) ).points.copy();
		arrowTrans.translate( 0, 0 );
		arrowTrans.scale( item->lineWidth(), item->lineWidth());
		arrowTrans.scale( -1 , 1 );
		arrow.map( arrowTrans );
		painter->setBrush( painter->pen() );
		painter->setBrushOpacity( 1.0 - item->lineTransparency() );
		painter->setLineWidth( 0 );
		painter->setFillMode(ScPainterExBase::Solid);
		painter->setupPolygon( &arrow );
		painter->fillPath();
	}
	if (endArrowIndex != 0)
	{
		QWMatrix arrowTrans;
		FPointArray arrow = (*m_doc->arrowStyles.at(endArrowIndex-1) ).points.copy();
		arrowTrans.translate( item->width(), 0 );
		arrowTrans.scale( item->lineWidth(), item->lineWidth());
		arrow.map( arrowTrans );
		painter->setBrush( painter->pen() );
		painter->setBrushOpacity( 1.0 - item->lineTransparency() );
		painter->setLineWidth( 0 );
		painter->setFillMode( ScPainterExBase::Solid );
		painter->setupPolygon( &arrow );
		painter->fillPath();
	}
}

void ScPageOutput::DrawItem_PathText( PageItem_PathText* item, ScPainterExBase* painter, double scale )
{
	uint a;
	int chs;
	double wide;
	QString chx, chx2, chx3;
	ScText *hl;
	struct PageItem::ZZ *Zli;
	double dx;
	double sp = 0;
	double oldSp = 0;
	double oCurX = 0;
	FPoint point = FPoint(0, 0);
	FPoint normal = FPoint(0, 0);
	FPoint tangent = FPoint(0, 0);
	FPoint extPoint = FPoint(0, 0);
	bool ext = false;
	bool first = true;
	double fsx = 0;
	uint seg = 0;
	double segLen = 0;
	double distCurX;
	double CurX = item->textToFrameDistLeft(); // item->CurX = item->textToFrameDistLeft()
	double CurY = 0;
	if (item->lineColor() != CommonStrings::None && item->PoShow)
	{
		painter->setupPolygon(&item->PoLine, false);
		painter->strokePath();
	}
	if (item->itemText.count() != 0)
		CurX += item->itemText.at(0)->csize * item->itemText.at(0)->cextra / 10000.0;
	segLen = item->PoLine.lenPathSeg(seg);
	for (a = 0; a < item->itemText.count(); ++a)
	{
		CurY = 0;
		hl = item->itemText.at(a);
		chx = hl->ch;
		if ((chx == QChar(30)) || (chx == QChar(13)) || (chx == QChar(9)) || (chx == QChar(28)))
			continue;
		chs = hl->csize;
		item->SetZeichAttr(hl, &chs, &chx);
		if (chx == QChar(29))
			chx2 = " ";
		else if (chx == QChar(24))
			chx2 = "-";
		else
			chx2 = chx;
		if (a < item->itemText.count()-1)
		{
			if (item->itemText.at(a+1)->ch == QChar(29))
				chx3 = " ";
			else if (item->itemText.at(a+1)->ch == QChar(24))
				chx3 = "-";
			else
				chx3 = item->itemText.at(a+1)->ch;
			wide = Cwidth(m_doc, hl->cfont, chx2, chs, chx3);
		}
		else
			wide = Cwidth(m_doc, hl->cfont, chx2, chs);
		wide = wide * (hl->cscale / 1000.0);
		dx = wide / 2.0;
		CurX += dx;
		ext = false;
		while ( (seg < item->PoLine.size()-3) && (CurX > fsx + segLen))
		{
			fsx += segLen;
			seg += 4;
			if (seg > item->PoLine.size()-3)
				break;
			segLen = item->PoLine.lenPathSeg(seg);
			ext = true;
		}
		if (seg > item->PoLine.size()-3)
			break;
		if (CurX > fsx + segLen)
			break;
		if (ext)
		{
			sp = 0;
			distCurX = item->PoLine.lenPathDist(seg, 0, sp);
			while (distCurX <= ((CurX - oCurX) - (fsx - oCurX)))
			{
				sp += 0.001;
				distCurX = item->PoLine.lenPathDist(seg, 0, sp);
			}
			item->PoLine.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
			CurX = (CurX - (CurX - fsx)) + distCurX;
			oldSp = sp;
			ext = false;
		}
		else
		{
			if( seg < item->PoLine.size()-3 )
			{
				if (CurX > fsx + segLen)
					break;
				distCurX = item->PoLine.lenPathDist(seg, oldSp, sp);
				while (distCurX <= (CurX - oCurX))
				{
					sp += 0.001;
					if (sp >= 1.0)
					{
						sp = 0.9999;
						break;
					}
					distCurX = item->PoLine.lenPathDist(seg, oldSp, sp);
				}
				item->PoLine.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
				CurX = oCurX + distCurX;
				oldSp = sp;
			}
			else
				break;
		}
		hl->xp = point.x();
		hl->yp = point.y();
		hl->PtransX = tangent.x();
		hl->PtransY = tangent.y();
		hl->PRot = dx;
		QWMatrix trafo = QWMatrix( 1, 0, 0, -1, -dx * scale, 0 );
		trafo *= QWMatrix( tangent.x(), tangent.y(), tangent.y(), -tangent.x(), point.x() * scale, point.y() * scale);
		QWMatrix sca = painter->worldMatrix();
		trafo *= sca;
		painter->save();
		QWMatrix savWM = painter->worldMatrix();
		painter->setWorldMatrix(trafo);
		Zli = new PageItem::ZZ;
		Zli->Zeich = chx;
		if (hl->ccolor != CommonStrings::None)
		{
			ScColorShade tmp(m_doc->PageColors[hl->ccolor], hl->cshade);
			painter->setBrush(tmp);
		}
		if (hl->cstroke != CommonStrings::None)
		{
			ScColorShade tmp(m_doc->PageColors[hl->cstroke], hl->cshade2);
			painter->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		}
		Zli->Farb = hl->ccolor;
		Zli->Farb2 = hl->cstroke;
		Zli->shade = hl->cshade;
		Zli->shade2 = hl->cshade2;
		Zli->xco = 0;
		Zli->yco = item->BaseOffs;
		Zli->Sele = hl->cselect;
		Zli->Siz = chs;
		Zli->realSiz = hl->csize;
		Zli->Style = hl->cstyle;
		Zli->ZFo = hl->cfont;
		Zli->wide = wide;
		Zli->kern = hl->csize * hl->cextra / 10000.0;
		Zli->scale = hl->cscale;
		Zli->scalev = hl->cscalev;
		Zli->base = hl->cbase;
		Zli->shadowX = hl->cshadowx;
		Zli->shadowY = hl->cshadowx;
		Zli->outline = hl->coutline;
		Zli->underpos = hl->cunderpos;
		Zli->underwidth = hl->cunderwidth;
		Zli->strikepos = hl->cstrikepos;
		Zli->strikewidth = hl->cstrikewidth;
		Zli->embedded = 0;
		DrawCharacters(item, painter, Zli);
		delete Zli;
		painter->setWorldMatrix(savWM);
		painter->restore();
		painter->setZoomFactor(scale);
		item->MaxChars = a+1;
		oCurX = CurX;
		CurX -= dx;
		CurX += wide+hl->csize * hl->cextra / 10000.0;
		first = false;
	}
}

void ScPageOutput::DrawItem_Polygon ( PageItem_Polygon* item , ScPainterExBase* painter )
{
	painter->setupPolygon(&item->PoLine);
	painter->fillPath();
}

void ScPageOutput::DrawItem_PolyLine( PageItem_PolyLine* item, ScPainterExBase* painter )
{
 int startArrowIndex;
 int endArrowIndex;

	startArrowIndex = item->startArrowIndex();
	endArrowIndex = item->endArrowIndex();

	if (item->PoLine.size()>=4)
	{
		if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
		{
			FPointArray cli;
			FPoint Start;
			bool firstp = true;
			for (uint n = 0; n < item->PoLine.size()-3; n += 4)
			{
				if (firstp)
				{
					Start = item->PoLine.point(n);
					firstp = false;
				}
				if (item->PoLine.point(n).x() > 900000)
				{
					cli.addPoint(item->PoLine.point(n-2));
					cli.addPoint(item->PoLine.point(n-2));
					cli.addPoint(Start);
					cli.addPoint(Start);
					cli.setMarker();
					firstp = true;
					continue;
				}
				cli.addPoint(item->PoLine.point(n));
				cli.addPoint(item->PoLine.point(n+1));
				cli.addPoint(item->PoLine.point(n+2));
				cli.addPoint(item->PoLine.point(n+3));
			}
			if (cli.size() > 2)
			{
				FPoint l1 = cli.point(cli.size()-2);
				cli.addPoint(l1);
				cli.addPoint(l1);
				cli.addPoint(Start);
				cli.addPoint(Start);
			}
			painter->setupPolygon(&cli);
			painter->fillPath();
		}
		painter->setupPolygon(&item->PoLine, false);
		if (item->NamedLStyle.isEmpty())
			painter->strokePath();
		else
		{
			multiLine ml = m_doc->MLineStyles[item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				ScColorShade tmp(m_doc->PageColors[ml[it].Color], ml[it].Shade);
				painter->setPen(tmp, ml[it].Width,
							static_cast<Qt::PenStyle>(ml[it].Dash),
							static_cast<Qt::PenCapStyle>(ml[it].LineEnd),
							static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
				painter->strokePath();
			}
		}
		if (startArrowIndex != 0)
		{
			FPoint Start = item->PoLine.point(0);
			for (uint xx = 1; xx < item->PoLine.size(); xx += 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
					QWMatrix arrowTrans;
					FPointArray arrow = (*m_doc->arrowStyles.at(startArrowIndex-1)).points.copy();
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(item->lineWidth(), item->lineWidth());
					arrow.map(arrowTrans);
					painter->setBrush(painter->pen());
					painter->setBrushOpacity(1.0 - item->lineTransparency());
					painter->setLineWidth(0);
					painter->setFillMode(ScPainterExBase::Solid);
					painter->setupPolygon(&arrow);
					painter->fillPath();
					break;
				}
			}
		}
		if (endArrowIndex != 0)
		{
			FPoint End = item->PoLine.point(item->PoLine.size()-2);
			for (uint xx = item->PoLine.size()-1; xx > 0; xx -= 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
					QWMatrix arrowTrans;
					FPointArray arrow = (*m_doc->arrowStyles.at(endArrowIndex-1)).points.copy();
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(r);
					arrowTrans.scale( item->lineWidth(), item->lineWidth() );
					arrow.map(arrowTrans);
					painter->setBrush(painter->pen());
					painter->setBrushOpacity(1.0 - item->lineTransparency());
					painter->setLineWidth(0);
					painter->setFillMode(ScPainterExBase::Solid);
					painter->setupPolygon(&arrow);
					painter->fillPath();
					break;
				}
			}
		}
	}
}

void ScPageOutput::DrawItem_TextFrame( PageItem_TextFrame* item, ScPainterExBase* painter, QRect e, double scale )
{
	switch (item->itemType())
	{
		case PageItem::TextFrame:
		{
			QWMatrix wm;
			QPoint pt1, pt2;
			FPoint ColBound;
			QRegion cm;
			uint a;
			int chs, CurrCol;
			double oldCurY, wide, lineCorr, ColWidth;
			QString chx, chx2, chx3;
			ScText *hl;

			bool outs = false;
			bool fBorder = false;
			bool RTab = false;
			bool goNoRoom = false;
			bool goNextColumn = false;
			int TabCode = 0;
			int HyphenCount = 0;
			QValueList<ParagraphStyle::TabRecord> tTabValues;
			bool DropCmode = false;
			bool AbsHasDrop = false;
			double desc, asce, tabDist;
			bool StartOfCol = true;
			tTabValues.clear();

			for (int xxx=0; xxx<5; ++xxx)
			{
				m_doc->docParagraphStyles[xxx].setLineSpacingMode( (ParagraphStyle::LineSpacingMode) item->lineSpacingMode() );
				if (item->lineSpacingMode() == 2)
					m_doc->docParagraphStyles[xxx].setUseBaselineGrid( true );
				else
					m_doc->docParagraphStyles[xxx].setUseBaselineGrid( false );
				m_doc->docParagraphStyles[xxx].setLineSpacing( item->lineSpacing() );
				m_doc->docParagraphStyles[xxx].charStyle().csize = item->fontSize();
				m_doc->docParagraphStyles[xxx].setLeftMargin( 0 );
				m_doc->docParagraphStyles[xxx].setFirstIndent( 0 );
				m_doc->docParagraphStyles[xxx].setGapBefore( 0 );
				m_doc->docParagraphStyles[xxx].setGapAfter( 0 );
				m_doc->docParagraphStyles[xxx].setAlignment( xxx );
			}

			QPtrList<PageItem::ZZ> LiList;
			LiList.setAutoDelete(true);
			QRect e2 = QRect(qRound(e.x()  / scale + m_doc->minCanvasCoordinate.x()), qRound(e.y()  / scale + m_doc->minCanvasCoordinate.y()), qRound(e.width() / scale), qRound(e.height() / scale));
			painter->save();
			wm.translate(item->xPos(), item->yPos());
			wm.rotate(item->rotation());
			if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
			{
				painter->setupPolygon(&item->PoLine);
				painter->fillPath();
			}
			if (item->lineColor() != CommonStrings::None)
				lineCorr = item->lineWidth() / 2.0;
			else
				lineCorr = 0;
			if ((item->isAnnotation()) && (item->annotation().Type() == 2) && (!item->Pfile.isEmpty()) && (item->PicAvail) && (item->imageShown()) && (item->annotation().UseIcons()))
			{
				painter->setupPolygon(&item->PoLine);
				painter->setClipPath();
				painter->save();
				painter->scale(item->imageXScale(), item->imageYScale());
				painter->translate(static_cast<int>(item->imageXOffset() * item->imageXScale()), static_cast<int>(item->imageYOffset()  * item->imageYScale()));
				if (!item->pixm.isNull())
					painter->drawImage(&item->pixm, ScPainterExBase::rgbImages);
				painter->restore();
			}
			if ((item->itemText.count() != 0))
			{
				if (item->imageFlippedH())
				{
					painter->translate(item->width() * scale, 0);
					painter->scale(-1, 1);
				}
				if (item->imageFlippedV())
				{
					painter->translate(0, item->height() * scale);
					painter->scale(1, -1);
				}
				struct PageItem::ZZ Zli3;
				CurrCol = 0;
				ColWidth = item->columnWidth();
				ColBound = FPoint((ColWidth + item->ColGap) * CurrCol + item->textToFrameDistLeft() + lineCorr, ColWidth * (CurrCol+1) + item->ColGap * CurrCol + item->textToFrameDistLeft()+lineCorr);
				ColBound = FPoint(ColBound.x(), ColBound.y() + item->textToFrameDistRight() + lineCorr);
				tabDist = ColBound.x();
				uint tabCc = 0;
				for (a = 0; a < item->itemText.count(); ++a)
				{
					if (a >= item->MaxChars)
						break;
					hl = item->itemText.at(a);
					if (hl->cab < 5)
						tTabValues = item->TabValues;
					else
						tTabValues = m_doc->docParagraphStyles[hl->cab].tabValues();
					if (hl->cstyle & 16384)
						tabCc = 0;
					chx = hl->ch;
					if (hl->yp == 0)
						continue;
					if (hl->ch == QChar(30))
						chx = item->ExpandToken(a);
					if (hl->ccolor != CommonStrings::None)
					{
						ScColorShade tmp(m_doc->PageColors[hl->ccolor], hl->cshade);
						painter->setBrush(tmp);
					}
					if (hl->cstroke != CommonStrings::None)
					{
						ScColorShade tmp(m_doc->PageColors[hl->cstroke], hl->cshade2);
						painter->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					}
					chs = hl->csize;
					if (hl->cstyle & 2048)
					{
						if (m_doc->docParagraphStyles[hl->cab].useBaselineGrid())
							chs = qRound(10 * ((m_doc->typographicSettings.valueBaseGrid * (m_doc->docParagraphStyles[hl->cab].dropCapLines()-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].charStyle().csize / 10.0))) / (RealCHeight(m_doc, hl->cfont, chx, 10))));
						else
						{
							if (m_doc->docParagraphStyles[hl->cab].lineSpacingMode() == 0)
								chs = qRound(10 * ((m_doc->docParagraphStyles[hl->cab].lineSpacing() * (m_doc->docParagraphStyles[hl->cab].dropCapLines()-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].charStyle().csize / 10.0))) / (RealCHeight(m_doc, hl->cfont, chx, 10))));
							else
							{
								double currasce = RealFHeight(m_doc, hl->cfont, m_doc->docParagraphStyles[hl->cab].charStyle().csize);
								chs = qRound(10 * ((currasce * (m_doc->docParagraphStyles[hl->cab].dropCapLines()-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].charStyle().csize / 10.0))) / RealCHeight(m_doc, hl->cfont, chx, 10)));
							}
						}
					}
					oldCurY = item->SetZeichAttr(hl, &chs, &chx);
					if ((chx == QChar(9)) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
					{
						QString tabFillCharQStr(tTabValues[tabCc].tabFillChar);
						double wt = Cwidth(m_doc, hl->cfont, tabFillCharQStr, chs);
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
							if (e2.intersects(wm.mapRect(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
								DrawCharacters(item, painter, &Zli3);
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
						Zli3.wide = (hl->cembedded->gWidth + hl->cembedded->lineWidth()) * (hl->cscale / 1000.0);
					else
						Zli3.wide = Cwidth(m_doc, hl->cfont, chx, hl->csize) * (hl->cscale / 1000.0);
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
					//if (!m_doc->RePos)
					{
						double xcoZli = Zli3.xco;
						desc = Zli3.ZFo->numDescender * (-Zli3.Siz / 10.0);
						asce = Zli3.ZFo->numAscent * (Zli3.Siz / 10.0);
						if ((((Zli3.Sele) && (item->isSelected())) || (((item->NextBox != 0) || (item->BackBox != 0)) && (Zli3.Sele))) && (m_doc->appMode == modeEdit))
						{
							wide = Zli3.wide;
							painter->setFillMode(1);
							//paintersetBrush( ScColorShade( Qt::darkBlue, 100) );
							painter->setBrush( ScColorShade(qApp->palette().color(QPalette::Active, QColorGroup::Highlight), 100) );
							painter->setLineWidth(0);
							if ((a > 0) && (Zli3.Zeich == QChar(9)))
							{
								xcoZli = item->itemText.at(a-1)->xp+Cwidth(m_doc, item->itemText.at(a-1)->cfont, item->itemText.at(a-1)->ch, item->itemText.at(a-1)->csize);
								wide = Zli3.xco - xcoZli + Zli3.wide;
							}
							//if (!m_doc->RePos)
								painter->drawRect(xcoZli, qRound(Zli3.yco-asce * (Zli3.scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli3.scalev / 1000.0)));
							//painter->setBrush(ScColorShade(Qt::white, 100));
							painter->setBrush(ScColorShade(qApp->palette().color(QPalette::Active, QColorGroup::HighlightedText), 100));
						}
						if (Zli3.Farb2 != CommonStrings::None)
						{
							ScColorShade tmp(m_doc->PageColors[Zli3.Farb2], Zli3.shade2);
							painter->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
						}
						if (((chx == QChar(13)) || (chx == QChar(28))) && (m_doc->guidesSettings.showControls))
						{
							if (e2.intersects(wm.mapRect(QRect(qRound(Zli3.xco+Zli3.wide),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
							{
								FPointArray points;
								QWMatrix chma, chma2, chma4, chma5;
								double ytrans, xtrans;
								if (chx == QChar(13))
								{
									points = m_doc->symReturn.copy();
									if (a > 0)
										ytrans = item->itemText.at(a-1)->yp-((Zli3.Siz / 10.0) * 0.8);
									else
										ytrans = Zli3.yco-m_doc->docParagraphStyles[hl->cab].lineSpacing()-((Zli3.Siz / 10.0) * 0.8);
								}
								else
								{
									points = m_doc->symNewLine.copy();
									if (a > 0)
										ytrans = item->itemText.at(a-1)->yp-((Zli3.Siz / 10.0) * 0.4);
									else
										ytrans = Zli3.yco-m_doc->docParagraphStyles[hl->cab].lineSpacing()-((Zli3.Siz / 10.0) * 0.4);
								}
								if (hl->cstyle & 16384)
									xtrans = Zli3.xco;
								else
								{
									if (a > 0)
										xtrans = item->itemText.at(a-1)->xp+ Cwidth(m_doc, item->itemText.at(a-1)->cfont, item->itemText.at(a-1)->ch, item->itemText.at(a-1)->csize);
									else
										xtrans = Zli3.xco;
								}
								chma4.translate(xtrans, ytrans);
								chma.scale(Zli3.Siz / 100.0, Zli3.Siz / 100.0);
								chma2.scale(Zli3.scale / 1000.0, Zli3.scalev / 1000.0);
								chma5.scale(painter->zoomFactor(), painter->zoomFactor());
								points.map(chma * chma2 * chma4 * chma5);
								painter->setupTextPolygon(&points);
								painter->setFillMode(1);
								painter->fillPath();
							}
						}
						if (e2.intersects(wm.mapRect(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
						{
							if (Zli3.Zeich == QChar(25))
								DrawItem_Embedded(item, painter, e, &Zli3);
							else
								DrawCharacters(item, painter, &Zli3);
						}
						if (hl->cstyle & 8192)
						{
							Zli3.Zeich = "-";
							Zli3.xco = Zli3.xco + Zli3.wide;
							if (e2.intersects(wm.mapRect(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
								DrawCharacters(item, painter, &Zli3);
						}
					}
					tabDist = Zli3.xco+Zli3.wide;
				}
				painter->restore();
				break;
			}
			painter->restore();
		}
			break;
		default:
			break;
	}
}


