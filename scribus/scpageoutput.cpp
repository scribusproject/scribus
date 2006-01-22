#include "scpageoutput.h"

#include "qpainter.h"
#include "pageitem.h"
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

//extern SCRIBUS_API ScribusMainWindow* ScMW;

ScPageOutput::ScPageOutput(ScribusDoc* doc, bool reloadImages, imageLoadMode loadMode, int resolution, bool useProfiles)
{
	m_doc = doc;
	m_reloadImages = reloadImages;
	m_imageRes = resolution;
	m_useProfiles = useProfiles;
	m_imageMode = loadMode;
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
			QPainter p;
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
						if (!currItem->printable())
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
							if (currItem->lineColor() != "None")
							{
								ScColorShade tmp( m_doc->PageColors[currItem->lineColor()], currItem->lineShade());
								if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
								{
									painter->setPen(tmp, currItem->Pwidth, currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
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
		QPainter p;
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
					if (!currItem->printable())
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
						if (currItem->lineColor() != "None")
						{
							ScColorShade tmp( m_doc->PageColors[currItem->lineColor()], currItem->lineShade() );
							if ((currItem->TopLine) || (currItem->RightLine) || (currItem->BottomLine) || (currItem->LeftLine))
							{
								painter->setPen(tmp, currItem->Pwidth, currItem->PLineArt, Qt::SquareCap, currItem->PLineJoin);
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
	painter->setLineWidth(item->Pwidth);
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
		if (item->fillColor() != "None")
		{
			painter->setBrush( ScColorShade(m_doc->PageColors[item->fillColor()], item->fillShade()) );
			painter->setFillMode(ScPainterExBase::Solid);
		}
		else
			painter->setFillMode(ScPainterExBase::None);
	}
	if (item->lineColor() != "None")
	{
		if ((item->Pwidth == 0) && !item->asLine())
			painter->setLineWidth(0);
		else
		{
			ScColorShade tmp(m_doc->PageColors[item->lineColor()], item->lineShade());
			painter->setPen( tmp , item->Pwidth, item->PLineArt, item->PLineEnd, item->PLineJoin);
			if (item->DashValues.count() != 0)
				painter->setDash(item->DashValues, item->DashOffset);
		}
	}
	else
		painter->setLineWidth(0);
	painter->setBrushOpacity(1.0 - item->fillTransparency());
	painter->setPenOpacity(1.0 - item->lineTransparency());
}

void ScPageOutput::DrawItem_Post( PageItem* item, ScPainterExBase* painter )
{
	bool doStroke=true;
	if ( item->itemType() == PageItem::PathText || item->itemType() == PageItem::PolyLine || item->itemType() == PageItem::Line )
		doStroke=false;
	if ((doStroke))
	{
		if (item->lineColor() != "None")
		{
			ScColorShade tmp(m_doc->PageColors[item->lineColor()], item->lineShade());
			painter->setPen(tmp, item->Pwidth, item->PLineArt, item->PLineEnd, item->PLineJoin);
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
				if ((hl->Style & 256) && (hl->Farb2 != "None"))
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
				if (hl->Farb != "None")
					painter->fillPath();
				if ((hl->Style & 4) && (hl->Farb2 != "None") && ((hl->Siz * hl->outline / 10000.0) != 0))
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
			struct ParagraphStyle vg;
			QValueList<ParagraphStyle> savedParagraphStyles;
			for (int xxx=0; xxx<5; ++xxx)
			{
				vg.LineSpaMode = m_doc->docParagraphStyles[xxx].LineSpaMode;
				vg.BaseAdj = m_doc->docParagraphStyles[xxx].BaseAdj;
				vg.LineSpa = m_doc->docParagraphStyles[xxx].LineSpa;
				vg.FontSize = m_doc->docParagraphStyles[xxx].FontSize;
				vg.Indent = m_doc->docParagraphStyles[xxx].Indent;
				vg.First = m_doc->docParagraphStyles[xxx].First;
				vg.gapBefore = m_doc->docParagraphStyles[xxx].gapBefore;
				vg.gapAfter = m_doc->docParagraphStyles[xxx].gapAfter;
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
			double pws = embedded->Pwidth;
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
					embedded->Pwidth = pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0);
					DrawItem_Line((PageItem_Line*) embedded, p);
					break;
				case PageItem::Polygon:
					DrawItem_Polygon((PageItem_Polygon*) embedded, p);
					break;
				case PageItem::PolyLine:
					embedded->Pwidth = pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0);
					DrawItem_PolyLine((PageItem_PolyLine*) embedded, p);
					break;
				case PageItem::PathText:
					DrawItem_PathText( (PageItem_PathText*) embedded, p, sc);
					break;
				default:
					break;
			}
			embedded->Pwidth = pws * QMIN(hl->scale / 1000.0, hl->scalev / 1000.0);
			DrawItem_Post(embedded, p);
			p->restore();
			embedded->Pwidth = pws;
			for (int xxx=0; xxx<5; ++xxx)
			{
				m_doc->docParagraphStyles[xxx].LineSpaMode = savedParagraphStyles[xxx].LineSpaMode;
				m_doc->docParagraphStyles[xxx].BaseAdj = savedParagraphStyles[xxx].BaseAdj;
				m_doc->docParagraphStyles[xxx].LineSpa = savedParagraphStyles[xxx].LineSpa;
				m_doc->docParagraphStyles[xxx].FontSize = savedParagraphStyles[xxx].FontSize;
				m_doc->docParagraphStyles[xxx].Indent = savedParagraphStyles[xxx].Indent;
				m_doc->docParagraphStyles[xxx].First = savedParagraphStyles[xxx].First;
				m_doc->docParagraphStyles[xxx].gapBefore = savedParagraphStyles[xxx].gapBefore;
				m_doc->docParagraphStyles[xxx].gapAfter = savedParagraphStyles[xxx].gapAfter;
			}
			savedParagraphStyles.clear();
		}
	}
}

void ScPageOutput::DrawItem_ImageFrame( PageItem_ImageFrame* item, ScPainterExBase* painter, double scale  )
{
	if ((item->fillColor() != "None") || (item->GrType != 0))
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
		if ((!item->PicArt) || (!item->PicAvail))
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
				QFileInfo fInfo(item->Pfile);
				QString ext = fInfo.extension(false);
				scImg.imgInfo.valid = false;
				scImg.imgInfo.clipPath = "";
				scImg.imgInfo.PDSpathData.clear();
				scImg.imgInfo.layerInfo.clear();
				scImg.imgInfo.RequestProps = item->pixm.imgInfo.RequestProps;
				scImg.imgInfo.isRequest = item->pixm.imgInfo.isRequest;
				scImg.LoadPicture(item->Pfile, item->IProfile, 0, item->UseEmbedded, m_useProfiles, (int) m_imageMode, m_imageRes, &dummy);
				if( ext == "eps" || ext == "pdf" || ext == "ps" )
				{
					imScaleX *= (72.0 / (double) m_imageRes);
					imScaleY *= (72.0 / (double) m_imageRes);
				}
				scImg.applyEffect(item->effectsInUse, m_doc->PageColors, true);
				pImage = &scImg;
			}
			else
				pImage = &item->pixm;

			painter->save();
			if (item->imageClip.size() != 0)
				painter->setupPolygon(&item->imageClip);
			else
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
			painter->drawImage(pImage);
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
	struct ScText *hl;
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
	if (item->lineColor() != "None" && item->PoShow)
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
		if (hl->ccolor != "None")
		{
			ScColorShade tmp(m_doc->PageColors[hl->ccolor], hl->cshade);
			painter->setBrush(tmp);
		}
		if (hl->cstroke != "None")
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
		if ((item->fillColor() != "None") || (item->GrType != 0))
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
 double CurX = 0; // item->CurX = item->textToFrameDistLeft()
 double CurY = 0;

	switch (item->itemType())
	{
		case PageItem::TextFrame:
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
			struct PageItem::ZZ *Zli;
			struct PageItem::ZZ *Zli2;

			bool outs = false;
			bool fBorder = false;
			bool RTab = false;
			bool goNoRoom = false;
			bool goNextColumn = false;
			uint StartRT, StartRT2;
			int TabCode = 0;
			int HyphenCount = 0;
			QValueList<PageItem::TabRecord> tTabValues;
			bool DropCmode = false;
			bool AbsHasDrop = false;
			double desc, asce, maxDY, firstDes, desc2, maxDX, tabDist;
			int DropLines;
			bool StartOfCol = true;
			tTabValues.clear();

			for (int xxx=0; xxx<5; ++xxx)
			{
				m_doc->docParagraphStyles[xxx].LineSpaMode = item->LineSpMode;
				if (item->LineSpMode == 2)
					m_doc->docParagraphStyles[xxx].BaseAdj = true;
				else
					m_doc->docParagraphStyles[xxx].BaseAdj = false;
				m_doc->docParagraphStyles[xxx].LineSpa = item->LineSp;
				m_doc->docParagraphStyles[xxx].FontSize = item->fontSize();
				m_doc->docParagraphStyles[xxx].Indent = 0;
				m_doc->docParagraphStyles[xxx].First = 0;
				m_doc->docParagraphStyles[xxx].gapBefore = 0;
				m_doc->docParagraphStyles[xxx].gapAfter = 0;
				m_doc->docParagraphStyles[xxx].textAlignment = xxx;
			}

			QPtrList<PageItem::ZZ> LiList;
			LiList.setAutoDelete(true);
			QRect e2 = QRect(qRound(e.x()  / scale + m_doc->minCanvasCoordinate.x()), qRound(e.y()  / scale + m_doc->minCanvasCoordinate.y()), qRound(e.width() / scale), qRound(e.height() / scale));
			painter->save();
			pf2.begin(ScMW->view->viewport());
			pf2.translate(item->xPos(), item->yPos());
			pf2.rotate(item->rotation());
			//painter->translate(item->xPos(), item->yPos());
			//painter->rotate(item->rotation());
			if ((item->fillColor() != "None") || (item->GrType != 0))
			{
				painter->setupPolygon(&item->PoLine);
				painter->fillPath();
			}
			if (item->lineColor() != "None")
				lineCorr = item->lineWidth() / 2.0;
			else
				lineCorr = 0;
			if ((item->isAnnotation()) && (item->annotation().Type() == 2) && (!item->Pfile.isEmpty()) && (item->PicAvail) && (item->PicArt) && (item->annotation().UseIcons()))
			{
				painter->setupPolygon(&item->PoLine);
				painter->setClipPath();
				painter->save();
				painter->scale(item->imageXScale(), item->imageYScale());
				painter->translate(static_cast<int>(item->imageXOffset() * item->imageXScale()), static_cast<int>(item->imageYOffset()  * item->imageYScale()));
				if (!item->pixm.isNull())
					painter->drawImage(&item->pixm);
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
						tTabValues = m_doc->docParagraphStyles[hl->cab].TabValues;
					if (hl->cstyle & 16384)
						tabCc = 0;
					chx = hl->ch;
					if (hl->yp == 0)
						continue;
					if (hl->ch == QChar(30))
						chx = item->ExpandToken(a);
					if (hl->ccolor != "None")
					{
						ScColorShade tmp(m_doc->PageColors[hl->ccolor], hl->cshade);
						painter->setBrush(tmp);
					}
					if (hl->cstroke != "None")
					{
						ScColorShade tmp(m_doc->PageColors[hl->cstroke], hl->cshade2);
						painter->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					}
					chs = hl->csize;
					if (hl->cstyle & 2048)
					{
						if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
							chs = qRound(10 * ((m_doc->typographicSettings.valueBaseGrid * (m_doc->docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_doc, hl->cfont, chx, 10))));
						else
						{
							if (m_doc->docParagraphStyles[hl->cab].LineSpaMode == 0)
								chs = qRound(10 * ((m_doc->docParagraphStyles[hl->cab].LineSpa * (m_doc->docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_doc, hl->cfont, chx, 10))));
							else
							{
								double currasce = RealFHeight(m_doc, hl->cfont, m_doc->docParagraphStyles[hl->cab].FontSize);
								chs = qRound(10 * ((currasce * (m_doc->docParagraphStyles[hl->cab].DropLin-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCHeight(m_doc, hl->cfont, chx, 10)));
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
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
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
						Zli3.wide = (hl->cembedded->gWidth + hl->cembedded->Pwidth) * (hl->cscale / 1000.0);
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
					/*if (!m_doc->RePos)*/
					{
						double xcoZli = Zli3.xco;
						desc = Zli3.ZFo->numDescender * (-Zli3.Siz / 10.0);
						asce = Zli3.ZFo->numAscent * (Zli3.Siz / 10.0);
						if ((((Zli3.Sele) && (item->isSelected())) || (((item->NextBox != 0) || (item->BackBox != 0)) && (Zli3.Sele))) && (m_doc->appMode == modeEdit))
						{
							wide = Zli3.wide;
							painter->setFillMode(1);
							painter->setBrush(ScColorShade(Qt::darkBlue, 100));
							painter->setLineWidth(0);
							if ((a > 0) && (Zli3.Zeich == QChar(9)))
							{
								xcoZli = item->itemText.at(a-1)->xp+Cwidth(m_doc, item->itemText.at(a-1)->cfont, item->itemText.at(a-1)->ch, item->itemText.at(a-1)->csize);
								wide = Zli3.xco - xcoZli + Zli3.wide;
							}
							//if (!m_doc->RePos)
								painter->drawRect(xcoZli, qRound(Zli3.yco-asce * (Zli3.scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli3.scalev / 1000.0)));
							painter->setBrush(ScColorShade(Qt::white, 100));
						}
						if (Zli3.Farb2 != "None")
						{
							ScColorShade tmp(m_doc->PageColors[Zli3.Farb2], Zli3.shade2);
							painter->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
						}
						if (((chx == QChar(13)) || (chx == QChar(28))) && (m_doc->guidesSettings.showControls))
						{
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco+Zli3.wide),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
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
										ytrans = Zli3.yco-m_doc->docParagraphStyles[hl->cab].LineSpa-((Zli3.Siz / 10.0) * 0.8);
								}
								else
								{
									points = m_doc->symNewLine.copy();
									if (a > 0)
										ytrans = item->itemText.at(a-1)->yp-((Zli3.Siz / 10.0) * 0.4);
									else
										ytrans = Zli3.yco-m_doc->docParagraphStyles[hl->cab].LineSpa-((Zli3.Siz / 10.0) * 0.4);
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
						if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
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
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli3.xco),qRound(Zli3.yco-asce), qRound(Zli3.wide+1), qRound(asce+desc)))))
								DrawCharacters(item, painter, &Zli3);
						}
					}
					tabDist = Zli3.xco+Zli3.wide;
				}
				//Dirty = false;
				//Redrawn = true;
				pf2.end();
				painter->restore();
				break;
			}
			if ((item->itemText.count() != 0) || (item->NextBox != 0))
			{
				if (item->NextBox != 0)
				{
					nextItem = item->NextBox;
					while (nextItem != 0)
					{
						a = nextItem->itemText.count();
						for (uint s=0; s<a; ++s)
						{
							item->itemText.append(nextItem->itemText.take(0));
						}
						nextItem->MaxChars = 0;
						nextItem = nextItem->NextBox;
					}
					nextItem = item->NextBox;
				}
				m_doc->docParagraphStyles[0].LineSpa = item->LineSp;
				QRegion cl = QRegion(pf2.xForm(item->Clip));
				int LayerLev = m_doc->layerLevelFromNumber(item->LayerNr);
				uint docItemsCount=m_doc->Items->count();
				if (!item->isEmbedded)
				{
					if (!item->OnMasterPage.isEmpty())
					{
						Page* Mp = m_doc->MasterPages.at(m_doc->MasterNames[item->OnMasterPage]);
						Page* Dp = m_doc->Pages->at(item->savedOwnPage);
						for (a = 0; a < m_doc->MasterItems.count(); ++a)
						{
							PageItem* docItem = m_doc->MasterItems.at(a);
							int LayerLevItem = m_doc->layerLevelFromNumber(docItem->LayerNr);
							if (((docItem->ItemNr > item->ItemNr) && (docItem->LayerNr == item->LayerNr)) || (LayerLevItem > LayerLev))
							{
								if (docItem->textFlowsAroundFrame())
								{
									pp.begin(ScMW->view->viewport());
									pp.translate(docItem->xPos() - Mp->xOffset() + Dp->xOffset(), docItem->yPos() - Mp->yOffset() + Dp->yOffset());
									pp.rotate(docItem->rotation());
									if (docItem->textFlowUsesBoundingBox())
									{
										QPointArray tcli;
										tcli.resize(4);
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
						if (!m_doc->masterPageMode())
						{
							for (a = 0; a < docItemsCount; ++a)
							{
								PageItem* docItem = m_doc->Items->at(a);
								Page* Mp = m_doc->MasterPages.at(m_doc->MasterNames[item->OnMasterPage]);
								Page* Dp = m_doc->Pages->at(item->OwnPage);
								if (docItem->textFlowsAroundFrame() && (docItem->OwnPage == item->OwnPage))
								{
									pp.begin(ScMW->view->viewport());
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
							PageItem* docItem = m_doc->Items->at(a);
							int LayerLevItem = m_doc->layerLevelFromNumber(docItem->LayerNr);
							if (((docItem->ItemNr > item->ItemNr) && (docItem->LayerNr == item->LayerNr)) || (LayerLevItem > LayerLev))
							{
								if (docItem->textFlowsAroundFrame())
								{
									pp.begin(ScMW->view->viewport());
									pp.translate(docItem->xPos(), docItem->yPos());
									pp.rotate(docItem->rotation());
									if (docItem->textFlowUsesBoundingBox())
									{
										QPointArray tcli;
										tcli.resize(4);
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
				if (item->imageFlippedH())
				{
					painter->translate(item->width() * scale, 0);
					painter->scale(-1, 1);
					pf2.translate(item->width(), 0);
					pf2.scale(-1, 1);
				}
				if (item->imageFlippedV())
				{
					painter->translate(0, item->height() * scale);
					painter->scale(1, -1);
					pf2.translate(0, item->height());
					pf2.scale(1, -1);
				}
				CurrCol = 0;
				//ColWidth = (item->width() - (item->ColGap * (item->Cols - 1)) - item->textToFrameDistLeft() - item->textToFrameDistRight() - 2*lineCorr) / item->Cols;
				ColWidth = item->columnWidth();
				ColBound = FPoint((ColWidth + item->ColGap) * CurrCol + item->textToFrameDistLeft() + lineCorr, ColWidth * (CurrCol+1) + item->ColGap * CurrCol + item->textToFrameDistLeft() + lineCorr);
				ColBound = FPoint(ColBound.x(), ColBound.y() + item->textToFrameDistRight() + lineCorr);
				CurX = ColBound.x();
				if (item->itemText.count() > 0)
				{
					hl = item->itemText.at(0);
					if (m_doc->docParagraphStyles[hl->cab].Drop)
					{
						if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
							chs = qRound(m_doc->typographicSettings.valueBaseGrid  * m_doc->docParagraphStyles[hl->cab].DropLin * 10);
						else
							chs = qRound(m_doc->docParagraphStyles[hl->cab].LineSpa * m_doc->docParagraphStyles[hl->cab].DropLin * 10);
					}
					else
						chs = hl->csize;
					desc2 = -hl->cfont->numDescender * (chs / 10.0);
					CurY = item->textToFrameDistTop() + lineCorr;
				}
				else
				{
					desc2 = -(*m_doc->AllFonts)[item->font()]->numDescender * (item->fontSize() / 10.0);
					CurY = m_doc->docParagraphStyles[0].LineSpa + item->textToFrameDistTop() + lineCorr - desc2;
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
				item->MaxChars = 0;
				tabDist = 0;
				MaxText = item->itemText.count();
				StartOfCol = true;
				for (a = 0; a < MaxText; ++a)
				{
					hl = item->itemText.at(a);
					chx = hl->ch;
					if (hl->ch == QChar(30))
						chx = item->ExpandToken(a);
					absa = hl->cab;
					if (m_doc->docParagraphStyles[absa].LineSpaMode == 1)
						m_doc->docParagraphStyles[absa].LineSpa = RealFHeight(m_doc, hl->cfont, hl->csize);
					if (a == 0)
					{
						if (item->BackBox != 0)
						{
							nextItem = item->BackBox;
							while (nextItem != 0)
							{
								uint nextItemTextCount=nextItem->itemText.count();
								if (nextItemTextCount != 0)
								{
									if (nextItem->itemText.at(nextItemTextCount-1)->ch == QChar(13))
									{
										CurY += m_doc->docParagraphStyles[absa].gapBefore;
										if (chx != QChar(13))
										{
											DropCmode = m_doc->docParagraphStyles[absa].Drop;
											if (DropCmode)
												DropLines = m_doc->docParagraphStyles[absa].DropLin;
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
								DropCmode = m_doc->docParagraphStyles[absa].Drop;
								if (DropCmode)
									DropLines = m_doc->docParagraphStyles[absa].DropLin;
							}
							else
								DropCmode = false;
							CurY += m_doc->docParagraphStyles[absa].gapBefore;
						}
					}
					hl->cstyle &= 0xF7FF; // 2047;
					hl->cstyle &= 8191;
					if (((m_doc->docParagraphStyles[absa].textAlignment == 3) || (m_doc->docParagraphStyles[absa].textAlignment == 4)) && (LiList.count() == 0) && (hl->ch == " "))
					{
						hl->cstyle |= 4096;
						continue;
					}
					else
						hl->cstyle &= 0xEFFF; // 4095;
					if (LiList.count() == 0)
					{
						if (((a > 0) && (item->itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (item->BackBox == 0)) && (!StartOfCol))
						{
							CurY += m_doc->docParagraphStyles[absa].gapBefore;
							if (chx != QChar(13))
								DropCmode = m_doc->docParagraphStyles[absa].Drop;
							else
								DropCmode = false;
							if (DropCmode)
							{
								DropLines = m_doc->docParagraphStyles[absa].DropLin;
								if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
									CurY += m_doc->typographicSettings.valueBaseGrid * (DropLines-1);
								else
								{
									if (m_doc->docParagraphStyles[absa].LineSpaMode == 0)
										CurY += m_doc->docParagraphStyles[absa].LineSpa * (DropLines-1);
									else
										CurY += RealFHeight(m_doc, hl->cfont, m_doc->docParagraphStyles[absa].FontSize) * (DropLines-1);
								}
							}
						}
					}
					if (DropCmode)
					{
						if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
						{
							chsd = qRound(10 * ((m_doc->typographicSettings.valueBaseGrid * (DropLines-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_doc, hl->cfont, chx, 10))));
							chs = qRound(10 * ((m_doc->typographicSettings.valueBaseGrid * (DropLines-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCAscent(m_doc, hl->cfont, chx, 10)));
						}
						else
						{
							if (m_doc->docParagraphStyles[absa].LineSpaMode == 0)
							{
								chsd = qRound(10 * ((m_doc->docParagraphStyles[absa].LineSpa * (DropLines-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_doc, hl->cfont, chx, 10))));
								chs = qRound(10 * ((m_doc->docParagraphStyles[absa].LineSpa * (DropLines-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCAscent(m_doc, hl->cfont, chx, 10)));
							}
							else
							{
								double currasce = RealFHeight(m_doc, hl->cfont, m_doc->docParagraphStyles[hl->cab].FontSize);
								chsd = qRound(10 * ((currasce * (DropLines-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / (RealCHeight(m_doc, hl->cfont, chx, 10))));
								chs = qRound(10 * ((currasce * (DropLines-1)+(hl->cfont->numAscent * (m_doc->docParagraphStyles[hl->cab].FontSize / 10.0))) / RealCAscent(m_doc, hl->cfont, chx, 10)));
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
					oldCurY = item->SetZeichAttr(hl, &chs, &chx);
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
					}
					if (DropCmode)
					{
						if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						{
							wide = hl->cembedded->gWidth + hl->cembedded->Pwidth;
							if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
								asce = m_doc->typographicSettings.valueBaseGrid * DropLines;
							else
							{
								if (m_doc->docParagraphStyles[absa].LineSpaMode == 0)
									asce = m_doc->docParagraphStyles[absa].LineSpa * DropLines;
								else
									asce = RealFHeight(m_doc, hl->cfont, m_doc->docParagraphStyles[absa].FontSize) * DropLines;
							}
							hl->cscalev = qRound(asce / (hl->cembedded->gHeight + hl->cembedded->Pwidth) * 1000.0);
							hl->cscale = hl->cscalev;
						}
						else
						{
							wide = RealCWidth(m_doc, hl->cfont, chx2, chsd);
							asce = RealCHeight(m_doc, hl->cfont, chx2, chsd);
						}
						desc2 = 0;
						desc = 0;
					}
					else
					{
						if ((hl->ch == QChar(25)) && (hl->cembedded != 0))
						{
							asce = hl->cfont->numAscent * (hl->csize / 10.0);
							desc2 = 0;
							desc = 0;
						}
						else
						{
							desc2 = -hl->cfont->numDescender * (hl->csize / 10.0);
							desc = -hl->cfont->numDescender * (hl->csize / 10.0);
							asce = hl->cfont->numAscent * (hl->csize / 10.0);
						}
					}
					wide = wide * (hl->cscale / 1000.0);
					fBorder = false;
					if (CurY + item->textToFrameDistBottom() + lineCorr > item->height())
					{
						StartOfCol = true;
						CurrCol++;
						if (CurrCol < item->Cols)
						{
							//ColWidth = (item->width() - (item->ColGap * (item->Cols - 1)) - item->textToFrameDistLeft() - item->textToFrameDistRight() - 2*lineCorr) / item->Cols;
							ColWidth = item->columnWidth();
							ColBound = FPoint((ColWidth + item->ColGap) * CurrCol + item->textToFrameDistLeft() + lineCorr, ColWidth * (CurrCol+1) + item->ColGap * CurrCol + item->textToFrameDistLeft() + lineCorr);
							CurX = ColBound.x();
							ColBound = FPoint(ColBound.x(), ColBound.y() + item->textToFrameDistRight() + lineCorr);
							CurY = asce + item->textToFrameDistTop() + lineCorr + 1;
							if (((a > 0) && (item->itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (item->BackBox == 0)))
							{
								if (chx != QChar(13))
									DropCmode = m_doc->docParagraphStyles[hl->cab].Drop;
								else
									DropCmode = false;
								if (DropCmode)
								{
									if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
										desc2 = -hl->cfont->numDescender * m_doc->typographicSettings.valueBaseGrid * m_doc->docParagraphStyles[hl->cab].DropLin;
									else
										desc2 = -hl->cfont->numDescender * m_doc->docParagraphStyles[hl->cab].LineSpa * m_doc->docParagraphStyles[hl->cab].DropLin;
								}
								if (DropCmode)
									DropLines = m_doc->docParagraphStyles[hl->cab].DropLin;
							}
							if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
							{
								double by = item->yPos();
								if (item->OwnPage != -1)
									by = item->yPos() - m_doc->Pages->at(item->OwnPage)->yOffset();
								int ol1 = qRound((by + CurY - m_doc->typographicSettings.offsetBaseGrid) * 10000.0);
								int ol2 = static_cast<int>(ol1 / m_doc->typographicSettings.valueBaseGrid);
								CurY = ceil(  ol2 / 10000.0 ) * m_doc->typographicSettings.valueBaseGrid + m_doc->typographicSettings.offsetBaseGrid - by;
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
							CurY = asce + item->textToFrameDistTop() + lineCorr+1;
//							if (((a > 0) && (item->itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (BackBox == 0)))
//								CurY += m_doc->docParagraphStyles[hl->cab].gapBefore;
						}
						if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
						{
							double by = item->yPos();
							if (item->OwnPage != -1)
								by = item->yPos() - m_doc->Pages->at(item->OwnPage)->yOffset();
							int ol1 = qRound((by + CurY - m_doc->typographicSettings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_doc->typographicSettings.valueBaseGrid);
							CurY = ceil(  ol2 / 10000.0 ) * m_doc->typographicSettings.valueBaseGrid + m_doc->typographicSettings.offsetBaseGrid - by;
						}
						if (CurY-TopOffset < 0.0)
							CurY = TopOffset+1;
						pt1 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(CurY+BotOffset));
						pt2 = QPoint(static_cast<int>(ceil(CurX)), static_cast<int>(ceil(CurY-TopOffset)));
						while ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))))
						{
							fBorder = true;
							CurX++;
							if (CurX + item->textToFrameDistRight() + lineCorr > ColBound.y())
							{
								fBorder = false;
								if (StartOfCol)
								{
									CurX = ColBound.x();
									CurY++;
								}
								else
								{
									CurY += m_doc->docParagraphStyles[hl->cab].LineSpa;
									CurX = ColBound.x();
								}
								if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
								{
									double by = item->yPos();
									if (item->OwnPage != -1)
										by = item->yPos() - m_doc->Pages->at(item->OwnPage)->yOffset();
									int ol1 = qRound((by + CurY - m_doc->typographicSettings.offsetBaseGrid) * 10000.0);
									int ol2 = static_cast<int>(ol1 / m_doc->typographicSettings.valueBaseGrid);
									CurY = ceil(  ol2 / 10000.0 ) * m_doc->typographicSettings.valueBaseGrid + m_doc->typographicSettings.offsetBaseGrid - by;
								}
								if (CurY + item->textToFrameDistBottom() + lineCorr > item->height())
								{
									StartOfCol = true;
									fBorder = false;
									CurrCol++;
									if (CurrCol < item->Cols)
									{
										//ColWidth = (item->width() - (item->ColGap * (item->Cols - 1)) - item->textToFrameDistLeft() - item->textToFrameDistRight() - 2*lineCorr) / item->Cols;
										ColWidth = item->columnWidth();
										ColBound = FPoint((ColWidth + item->ColGap) * CurrCol + item->textToFrameDistLeft() + lineCorr, ColWidth * (CurrCol+1) + item->ColGap * CurrCol + item->textToFrameDistLeft() + lineCorr);
										CurX = ColBound.x();
										ColBound = FPoint(ColBound.x(), ColBound.y() + item->textToFrameDistRight() + lineCorr);
										CurY = asce + item->textToFrameDistTop() + lineCorr + 1;
										if (((a > 0) && (item->itemText.at(a-1)->ch == QChar(13))) || ((a == 0) && (item->BackBox == 0)))
										{
											if (chx != QChar(13))
												DropCmode = m_doc->docParagraphStyles[hl->cab].Drop;
											else
												DropCmode = false;
											if (DropCmode)
											{
												if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
													desc2 = -hl->cfont->numDescender * m_doc->typographicSettings.valueBaseGrid * m_doc->docParagraphStyles[hl->cab].DropLin;
												else
													desc2 = -hl->cfont->numDescender * m_doc->docParagraphStyles[hl->cab].LineSpa * m_doc->docParagraphStyles[hl->cab].DropLin;
											}
											if (DropCmode)
												DropLines = m_doc->docParagraphStyles[hl->cab].DropLin;
										}
										if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
										{
											double by = item->yPos();
											if (item->OwnPage != -1)
												by = item->yPos() - m_doc->Pages->at(item->OwnPage)->yOffset();
											int ol1 = qRound((by + CurY - m_doc->typographicSettings.offsetBaseGrid) * 10000.0);
											int ol2 = static_cast<int>(ol1 / m_doc->typographicSettings.valueBaseGrid);
											CurY = ceil(  ol2 / 10000.0 ) * m_doc->typographicSettings.valueBaseGrid + m_doc->typographicSettings.offsetBaseGrid - by;
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
							CurX += item->textToFrameDistLeft();
						if (a > 0)
						{
							if (item->itemText.at(a-1)->ch == QChar(13))
								CurX += m_doc->docParagraphStyles[hl->cab].First;
						}
						else
						{
							if (item->BackBox == 0)
								CurX += m_doc->docParagraphStyles[hl->cab].First;
							else
							{
								if (item->BackBox->itemText.count() != 0)
								{
									if (item->BackBox->itemText.at(item->BackBox->itemText.count()-1)->ch == QChar(13))
										CurX += m_doc->docParagraphStyles[hl->cab].First;
								}
								else
									CurX += m_doc->docParagraphStyles[hl->cab].First;
							}
						}
						if (!AbsHasDrop)
							CurX += m_doc->docParagraphStyles[hl->cab].Indent;
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
								tTabValues = item->TabValues;
							else
								tTabValues = m_doc->docParagraphStyles[hl->cab].TabValues;
							if (tTabValues.isEmpty())
							{
								if ((CurX - ColBound.x()) != 0)
								{
									if (CurX == ColBound.x() + ceil((CurX-ColBound.x()) / m_doc->toolSettings.dTabWidth) * m_doc->toolSettings.dTabWidth)
										CurX += m_doc->toolSettings.dTabWidth;
									else
										CurX = ColBound.x() + ceil((CurX-ColBound.x()) / m_doc->toolSettings.dTabWidth) * m_doc->toolSettings.dTabWidth;
								}
								else
									CurX = ColBound.x() + m_doc->toolSettings.dTabWidth;
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
									CurX = ColBound.x() + ceil((CurX-ColBound.x()) / m_doc->toolSettings.dTabWidth) * m_doc->toolSettings.dTabWidth;
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
						hl->yp -= RealCHeight(m_doc, hl->cfont, chx2, chsd) - RealCAscent(m_doc, hl->cfont, chx2, chsd);
					if (LiList.count() == 0)
					{
						item->itemText.at(a)->cstyle |= 16384;
						kernVal = 0;
					}
					else
					{
						kernVal = chs * hl->cextra / 10000.0;
						item->itemText.at(a)->cstyle &= 16383;
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
					if (((hl->cstyle & 128) || (hl->ch == "-")) && ((HyphenCount < m_doc->HyCount) || (m_doc->HyCount == 0)))
					{
						if (hl->cstyle & 128)
						{
							pt1 = QPoint(qRound(ceil(CurX+item->textToFrameDistRight()+Cwidth(m_doc, hl->cfont, "-", hl->csize) * (hl->cscale / 1000.0))), qRound(CurY+desc));
							pt2 = QPoint(qRound(ceil(CurX+item->textToFrameDistRight()+Cwidth(m_doc, hl->cfont, "-", hl->csize) * (hl->cscale / 1000.0))), qRound(ceil(CurY-asce)));
						}
						else
						{
							pt1 = QPoint(qRound(ceil(CurX+item->textToFrameDistRight())), qRound(CurY+desc));
							pt2 = QPoint(qRound(ceil(CurX+item->textToFrameDistRight())), qRound(ceil(CurY-asce)));
						}
					}
					else
					{
						pt1 = QPoint(qRound(ceil(CurX+item->textToFrameDistRight())), qRound(CurY+desc));
						pt2 = QPoint(qRound(ceil(CurX+item->textToFrameDistRight())), qRound(ceil(CurY-asce)));
					}
					if ((!cl.contains(pf2.xForm(pt1))) || (!cl.contains(pf2.xForm(pt2))) || (CurX+item->textToFrameDistRight()+lineCorr > ColBound.y()))
						outs = true;
					if (CurY > (item->height() - item->textToFrameDistBottom() - lineCorr))
						outs = true;
					if ((hl->ch == QChar(27)) && (a < item->itemText.count()-1))
						goNoRoom = true;
					if ((hl->ch == QChar(26)) && (item->Cols > 1))
						goNextColumn = true;
					Zli = new PageItem::ZZ;
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
						Zli->realSiz = m_doc->docParagraphStyles[hl->cab].FontSize;
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
							if (item->itemText.at(a-1)->ch !=  " ")
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
						if ((HyphenCount < m_doc->HyCount) || (m_doc->HyCount == 0))
						{
							if (hl->ch == "-")
								LastXp = CurX;
							else
								LastXp = CurX + Cwidth(m_doc, hl->cfont, "-", hl->csize) * (hl->cscale / 1000.0);
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
							item->itemText.at(StartRT2+rtx2)->xp = QMAX(item->itemText.at(StartRT2+rtx2)->xp-(wide+kernVal) / cen, 0.0);
							if (item->itemText.at(StartRT2+rtx2)->xp < RTabX)
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
						CurX += m_doc->docParagraphStyles[hl->cab].DropDist;
						CurX = QMAX(CurX, ColBound.x());
						maxDX = CurX;
						QPointArray tcli;
						tcli.resize(4);
						if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
						{
							CurY -= m_doc->typographicSettings.valueBaseGrid * (DropLines-1);
							double by = item->yPos();
							if (item->OwnPage != -1)
								by = item->yPos() - m_doc->Pages->at(item->OwnPage)->yOffset();
							int ol1 = qRound((by + CurY - m_doc->typographicSettings.offsetBaseGrid) * 10000.0);
							int ol2 = static_cast<int>(ol1 / m_doc->typographicSettings.valueBaseGrid);
							CurY = ceil(  ol2 / 10000.0 ) * m_doc->typographicSettings.valueBaseGrid + m_doc->typographicSettings.offsetBaseGrid - by;
							tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*m_doc->typographicSettings.valueBaseGrid)));
							tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*m_doc->typographicSettings.valueBaseGrid)));
						}
						else
						{
							if (m_doc->docParagraphStyles[absa].LineSpaMode == 0)
							{
								CurY -= m_doc->docParagraphStyles[absa].LineSpa * (DropLines-1);
								tcli.setPoint(0, QPoint(qRound(hl->xp), qRound(maxDY-DropLines*m_doc->docParagraphStyles[absa].LineSpa)));
								tcli.setPoint(1, QPoint(qRound(maxDX), qRound(maxDY-DropLines*m_doc->docParagraphStyles[absa].LineSpa)));
							}
							else
							{
								double currasce = RealFHeight(m_doc, hl->cfont, m_doc->docParagraphStyles[hl->cab].FontSize);
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
					if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27))  || ((hl->ch == QChar(26)) && (item->Cols > 1)) || (outs))
					{
						RTab = false;
						TabCode = 0;
						if ((hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27))  || ((hl->ch == QChar(26)) && (item->Cols > 1)))
						{
							if (m_doc->docParagraphStyles[absa].textAlignment != 0)
							{
								EndX = CurX;
								do
								{
									pt1 = QPoint(qRound(EndX+item->textToFrameDistRight()), static_cast<int>(CurY+desc));
									pt2 = QPoint(qRound(EndX+item->textToFrameDistRight()), static_cast<int>(ceil(CurY-asce)));
									EndX++;
								}
								while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+item->textToFrameDistRight()+lineCorr < ColBound.y()));
								if (m_doc->docParagraphStyles[absa].textAlignment == 2)
									OFs = EndX - CurX;
								if (m_doc->docParagraphStyles[absa].textAlignment == 1)
									OFs = (EndX - CurX) / 2;
								if (m_doc->docParagraphStyles[absa].textAlignment == 3)
									OFs = 0;
								if (m_doc->docParagraphStyles[absa].textAlignment == 4)
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
								if (item->itemText.at(a)->cstyle & 128)
								{
									HyphenCount++;
									item->itemText.at(a)->cstyle |= 8192;
									Zli = new PageItem::ZZ;
									Zli->Zeich = "-";
									Zli->Farb = item->itemText.at(a)->ccolor;
									Zli->Farb2 = item->itemText.at(a)->cstroke;
									Zli->shade = item->itemText.at(a)->cshade;
									Zli->shade2 = item->itemText.at(a)->cshade2;
									Zli->xco = LastXp - Cwidth(m_doc, item->itemText.at(a)->cfont, "-", item->itemText.at(a)->csize) * (item->itemText.at(a)->cscale / 1000.0);
									Zli->yco = item->itemText.at(a)->yp;
									Zli->Sele = item->itemText.at(a)->cselect;
									Zli->Siz = item->itemText.at(a)->csize;
									Zli->realSiz = item->itemText.at(a)->csize;
									Zli->Style = item->itemText.at(a)->cstyle;
									Zli->ZFo = item->itemText.at(a)->cfont;
									Zli->wide = Cwidth(m_doc, item->itemText.at(a)->cfont, "-", item->itemText.at(a)->csize) * (item->itemText.at(a)->cscale / 1000.0);
									Zli->kern = item->itemText.at(a)->csize * item->itemText.at(a)->cextra / 10000.0;
									Zli->scale = item->itemText.at(a)->cscale;
									Zli->scalev = item->itemText.at(a)->cscalev;
									Zli->base = item->itemText.at(a)->cbase;
									Zli->shadowX = item->itemText.at(a)->cshadowx;
									Zli->shadowY = item->itemText.at(a)->cshadowy;
									Zli->outline = item->itemText.at(a)->coutline;
									Zli->underpos = item->itemText.at(a)->cunderpos;
									Zli->underwidth = item->itemText.at(a)->cunderwidth;
									Zli->strikepos = item->itemText.at(a)->cstrikepos;
									Zli->strikewidth = item->itemText.at(a)->cstrikewidth;
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
								if (m_doc->docParagraphStyles[absa].textAlignment != 0)
								{
									EndX = LastXp;
									do
									{
										pt1 = QPoint(qRound(EndX+item->textToFrameDistRight()), static_cast<int>(CurY+desc));
										pt2 = QPoint(qRound(EndX+item->textToFrameDistRight()), static_cast<int>(ceil(CurY-asce)));
										EndX++;
									}
									while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+item->textToFrameDistRight()+lineCorr < ColBound.y()));
									if (m_doc->docParagraphStyles[absa].textAlignment == 2)
										OFs = EndX - LastXp;
									if (m_doc->docParagraphStyles[absa].textAlignment == 1)
										OFs = (EndX - LastXp) / 2;
									if ((m_doc->docParagraphStyles[absa].textAlignment == 3) || (m_doc->docParagraphStyles[absa].textAlignment == 4))
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
						if ((outs) || (hl->ch == QChar(13)) || (hl->ch == QChar(28)) || (hl->ch == QChar(27)) || ((hl->ch == QChar(26)) && (item->Cols > 1)))
						{
							if ((outs) && (CurX+item->textToFrameDistRight()+lineCorr < ColBound.y()))
							{
								if (((hl->ch == QChar(13)) || (hl->ch == QChar(28))) && (AbsHasDrop))
								{
									AbsHasDrop = false;
									if (CurY < maxDY)
										CurY = maxDY;
								}
								bool fromOut = true;
								double BotOffset = desc + item->textToFrameDistBottom() + lineCorr;
								pt1 = QPoint(qRound(CurX+item->textToFrameDistRight()), static_cast<int>(CurY+BotOffset));
								pt2 = QPoint(qRound(CurX+item->textToFrameDistRight()), static_cast<int>(ceil(CurY-asce)));
								while (CurX+item->textToFrameDistRight()+lineCorr < ColBound.y())
								{
									CurX++;
									if (CurX+item->textToFrameDistRight()+lineCorr > ColBound.y())
									{
										fromOut = false;
										if (m_doc->docParagraphStyles[absa].BaseAdj)
											CurY += m_doc->typographicSettings.valueBaseGrid;
										else
											CurY += m_doc->docParagraphStyles[absa].LineSpa;
										if ((CurY + desc + item->textToFrameDistBottom() + lineCorr > item->height()) && (CurrCol+1 == item->Cols))
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
												CurY += m_doc->docParagraphStyles[hl->cab].gapAfter;
											if (BuPos3 > 0)
												BuPos3 -= 1;
											HyphenCount = 0;
										}
										break;
									}
									pt1 = QPoint(qRound(CurX+item->textToFrameDistRight()), static_cast<int>(CurY+BotOffset));
									pt2 = QPoint(qRound(CurX+item->textToFrameDistRight()), static_cast<int>(ceil(CurY-asce)));
									if ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))))
										break;
								}
								if (fromOut)
								{
									if ((CurY + desc + item->textToFrameDistBottom() + lineCorr > item->height()) && (CurrCol+1 == item->Cols))
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
								if (m_doc->docParagraphStyles[hl->cab].BaseAdj)
									CurY += m_doc->typographicSettings.valueBaseGrid;
								else
								{
									if (a < MaxText-1)
										CurY += m_doc->docParagraphStyles[item->itemText.at(a+1)->cab].LineSpa;
									else
										CurY += m_doc->docParagraphStyles[hl->cab].LineSpa;
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
										CurY += m_doc->docParagraphStyles[hl->cab].gapAfter;
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
							if ((!AbsHasDrop) && (StartOfCol) && (!m_doc->docParagraphStyles[hl->cab].BaseAdj))
							{
								Zli2 = LiList.at(0);
								double firstasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
								double currasce = 0;
								if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
									currasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
								else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
									currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
								else
									currasce = RealCAscent(m_doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz);
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
										currasce = QMAX(currasce, RealCAscent(m_doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz));
								}
								double adj = firstasce - currasce;
								for (uint zc = 0; zc < LiList.count(); ++zc)
								{
									LiList.at(zc)->yco -= adj;
								}
								CurY -= adj;
							}
							if ((!StartOfCol) && (!m_doc->docParagraphStyles[hl->cab].BaseAdj) && (m_doc->docParagraphStyles[hl->cab].LineSpaMode == 1))
							{
								Zli2 = LiList.at(0);
								double firstasce = m_doc->docParagraphStyles[hl->cab].LineSpa;
								double currasce = 0;
								if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
									currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
								else
									currasce = RealFHeight(m_doc, Zli2->ZFo, Zli2->realSiz);
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
									currasce = QMAX(currasce, RealFHeight(m_doc, Zli2->ZFo, Zli2->realSiz));
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
						if (m_doc->guidesSettings.showControls)
							loopC++;
						for (uint zc = 0; zc<loopC; ++zc)
						{
							double wide2 = 0;
							Zli2 = LiList.at(zc);
							double xcoZli = Zli2->xco;
							item->itemText.at(startLin+zc)->xp = Zli2->xco;
							item->itemText.at(startLin+zc)->yp = Zli2->yco;
							if (item->itemText.at(startLin+zc)->cab < 5)
								tTabValues = item->TabValues;
							else
								tTabValues = m_doc->docParagraphStyles[item->itemText.at(startLin+zc)->cab].TabValues;
							if (Zli2->Farb != "None")
							{
								ScColorShade tmp(m_doc->PageColors[Zli2->Farb],  Zli2->shade);
								painter->setBrush(tmp);
							}
							desc = Zli2->ZFo->numDescender * (-Zli2->Siz / 10.0);
							asce = Zli2->ZFo->numAscent * (Zli2->Siz / 10.0);
							if ((((Zli2->Sele) && (item->isSelected())) || (((item->NextBox != 0) || (item->BackBox != 0)) && (Zli2->Sele))) && (m_doc->appMode == modeEdit))
							{
								wide = Zli2->wide;
								painter->setFillMode(1);
								painter->setBrush(ScColorShade(Qt::darkBlue, 100));
								painter->setLineWidth(0);
								if ((zc > 0) && (Zli2->Zeich == QChar(9)))
								{
									wide2 = LiList.at(zc-1)->wide;
									xcoZli = LiList.at(zc-1)->xco+wide2;
									wide = Zli2->xco - xcoZli + Zli2->wide;
								}
								/*if (!m_doc->RePos)*/
									painter->drawRect(xcoZli, qRound(Zli2->yco-asce * (Zli2->scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli2->scalev / 1000.0)));
								painter->setBrush(ScColorShade(Qt::white, 100));
							}
							if (Zli2->Farb2 != "None")
							{
								ScColorShade tmp(m_doc->PageColors[Zli2->Farb2],  Zli2->shade2);
								painter->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
							}
							/*if (!m_doc->RePos)
							{*/
								if ((Zli2->Zeich == QChar(9)) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
								{
									QString tabFillCharQStr(tTabValues[tabCc].tabFillChar);
									double wt = Cwidth(m_doc, Zli2->ZFo, tabFillCharQStr, Zli2->Siz);
									int coun = static_cast<int>((Zli2->xco - tabDist) / wt);
									double sPos = Zli2->xco - (Zli2->xco - tabDist) + 1;
									Zli = new PageItem::ZZ;
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
											DrawCharacters(item, painter, Zli);
									}
									delete Zli;
								}
								if (Zli2->Zeich == QChar(9))
									tabCc++;
								if (((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28))) && (m_doc->guidesSettings.showControls) && (LiList.count() != 0))
								{
									if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco+Zli2->wide),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
									{
										FPointArray points;
										QWMatrix chma, chma2, chma4, chma5;
										double ytrans, xtrans;
										if (Zli2->Zeich == QChar(13))
										{
											points = m_doc->symReturn.copy();
											if (zc > 0)
												ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.8);
											else
												ytrans = Zli2->yco-m_doc->docParagraphStyles[item->itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.8);
										}
										else
										{
											points = m_doc->symNewLine.copy();
											if (zc > 0)
												ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.4);
											else
												ytrans = Zli2->yco-m_doc->docParagraphStyles[item->itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.4);
										}
										if (zc > 0)
											xtrans =  LiList.at(zc-1)->xco + Cwidth(m_doc, LiList.at(zc-1)->ZFo, LiList.at(zc-1)->Zeich, LiList.at(zc-1)->Siz);
										else
										{
											if (startLin > 0)
												xtrans = item->itemText.at(startLin-1)->xp + Cwidth(m_doc, item->itemText.at(startLin-1)->cfont, item->itemText.at(startLin-1)->ch, item->itemText.at(startLin-1)->csize);
											else
												xtrans = Zli2->xco;
										}
										chma4.translate(xtrans, ytrans);
										chma.scale(Zli2->Siz / 100.0, Zli2->Siz / 100.0);
										chma2.scale(Zli2->scale / 1000.0, Zli2->scalev / 1000.0);
										chma5.scale(painter->zoomFactor(), painter->zoomFactor());
										points.map(chma * chma2 * chma4 * chma5);
										painter->setupTextPolygon(&points);
										painter->setFillMode(1);
										painter->fillPath();
									}
								}
								if ((m_doc->guidesSettings.showControls) && (zc == BuPos3))
									break;
								if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
								{
									if (Zli2->Zeich == QChar(25))
										DrawItem_Embedded(item, painter, e, Zli2);
									else
										DrawCharacters(item, painter, Zli2);
								}
							/*}*/
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
							if (CurrCol < item->Cols)
							{
								//ColWidth = (item->width() - (item->ColGap * (item->Cols - 1)) - item->textToFrameDistLeft() - item->textToFrameDistRight() - 2*lineCorr) / item->Cols;
								ColWidth = item->columnWidth();
								ColBound = FPoint((ColWidth + item->ColGap) * CurrCol + item->textToFrameDistLeft() + lineCorr, ColWidth * (CurrCol+1) + item->ColGap * CurrCol + item->textToFrameDistLeft() + lineCorr);
								CurX = ColBound.x();
								ColBound = FPoint(ColBound.x(), ColBound.y() + item->textToFrameDistRight() + lineCorr);
							}
							else
							{
								nrc = a;
								goto NoRoom;
							}
						}
					}
				}
				if (m_doc->docParagraphStyles[absa].textAlignment != 0)
				{
					EndX = CurX;
					do
					{
						pt1 = QPoint(qRound(EndX+item->textToFrameDistRight()), static_cast<int>(CurY+desc));
						pt2 = QPoint(qRound(EndX+item->textToFrameDistRight()), static_cast<int>(ceil(CurY-asce)));
						EndX++;
					}
					while ((cl.contains(pf2.xForm(pt1))) && (cl.contains(pf2.xForm(pt2))) && (EndX+item->textToFrameDistRight()+lineCorr < ColBound.y()));
					if (m_doc->docParagraphStyles[absa].textAlignment == 2)
						OFs = EndX - CurX;
					if (m_doc->docParagraphStyles[absa].textAlignment == 1)
						OFs = (EndX - CurX) / 2;
					if (m_doc->docParagraphStyles[absa].textAlignment == 3)
						OFs = 0;
					if (m_doc->docParagraphStyles[absa].textAlignment == 4)
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
					if ((!AbsHasDrop) && (StartOfCol) && (!m_doc->docParagraphStyles[hl->cab].BaseAdj))
					{
						Zli2 = LiList.at(0);
						double firstasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
						double currasce = 0;
						if ((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28)))
							currasce = Zli2->ZFo->numAscent * (Zli2->realSiz / 10.0);
						else if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
						else
							currasce = RealCAscent(m_doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz);
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
								currasce = QMAX(currasce, RealCAscent(m_doc, Zli2->ZFo, Zli2->Zeich, Zli2->realSiz));
						}
						double adj = firstasce - currasce;
						for (uint zc = 0; zc < LiList.count(); ++zc)
						{
							LiList.at(zc)->yco -= adj;
						}
						CurY -= adj;
					}
					if ((!StartOfCol) && (!m_doc->docParagraphStyles[hl->cab].BaseAdj) && (m_doc->docParagraphStyles[hl->cab].LineSpaMode == 1))
					{
						Zli2 = LiList.at(0);
						double firstasce = m_doc->docParagraphStyles[hl->cab].LineSpa;
						double currasce = 0;
						if ((Zli2->Zeich == QChar(25)) && (Zli2->embedded != 0))
							currasce = QMAX(currasce, (Zli2->embedded->gHeight + Zli2->embedded->Pwidth) * (Zli2->scalev / 1000.0));
						else
							currasce = RealFHeight(m_doc, Zli2->ZFo, Zli2->realSiz);
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
								currasce = QMAX(currasce, RealFHeight(m_doc, Zli2->ZFo, Zli2->realSiz));
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
					item->itemText.at(startLin+zc)->xp = Zli2->xco;
					item->itemText.at(startLin+zc)->yp = Zli2->yco;
					if (item->itemText.at(startLin+zc)->cab < 5)
						tTabValues = item->TabValues;
					else
						tTabValues = m_doc->docParagraphStyles[item->itemText.at(startLin+zc)->cab].TabValues;
					if (Zli2->Farb != "None")
					{
						ScColorShade tmp(m_doc->PageColors[Zli2->Farb],  Zli2->shade);
						painter->setBrush(tmp);
					}
					desc = Zli2->ZFo->numDescender * (-Zli2->Siz / 10.0);
					asce = Zli2->ZFo->numAscent * (Zli2->Siz / 10.0);
					if ((((Zli2->Sele) && (item->isSelected())) || (((item->NextBox != 0) || (item->BackBox != 0)) && (Zli2->Sele))) && (m_doc->appMode == modeEdit))
					{
						wide = Zli2->wide;
						painter->setFillMode(1);
						painter->setBrush(ScColorShade(Qt::darkBlue, 100));
						painter->setLineWidth(0);
						if ((zc > 0) && (Zli2->Zeich == QChar(9)))
						{
							wide2 = LiList.at(zc-1)->wide;
							xcoZli = LiList.at(zc-1)->xco+wide2;
							wide = Zli2->xco - xcoZli + Zli2->wide;
						}
						/*if (!m_doc->RePos)*/
							painter->drawRect(xcoZli, qRound(Zli2->yco-asce * (Zli2->scalev / 1000.0)), wide+1, qRound((asce+desc) * (Zli2->scalev / 1000.0)));
						painter->setBrush(ScColorShade(Qt::white, 100));
					}
					if (Zli2->Farb2 != "None")
					{
						ScColorShade tmp(m_doc->PageColors[Zli2->Farb2],  Zli2->shade2);
						painter->setPen(tmp, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
					}
					/*if (!m_doc->RePos)
					{*/
						if ((Zli2->Zeich == QChar(9)) && (tTabValues.count() != 0) && (tabCc < tTabValues.count()) && (!tTabValues[tabCc].tabFillChar.isNull()))
						{
							QString tabFillCharQStr(tTabValues[tabCc].tabFillChar);
							double wt = Cwidth(m_doc, Zli2->ZFo, tabFillCharQStr, Zli2->Siz);
							int coun = static_cast<int>((Zli2->xco - tabDist) / wt);
							double sPos = Zli2->xco - (Zli2->xco - tabDist) + 1;
							Zli = new PageItem::ZZ;
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
									DrawCharacters(item, painter, Zli);
							}
							delete Zli;
						}
						if (Zli2->Zeich == QChar(9))
							tabCc++;
						if (((Zli2->Zeich == QChar(13)) || (Zli2->Zeich == QChar(28))) && (m_doc->guidesSettings.showControls) && (LiList.count() != 0))
						{
							if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco+Zli2->wide),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
							{
								FPointArray points;
								QWMatrix chma, chma2, chma4, chma5;
								double ytrans, xtrans;
								if (Zli2->Zeich == QChar(13))
								{
									points = m_doc->symReturn.copy();
									if (zc > 0)
										ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.8);
									else
										ytrans = Zli2->yco-m_doc->docParagraphStyles[item->itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.8);
								}
								else
								{
									points = m_doc->symNewLine.copy();
									if (zc > 0)
										ytrans = LiList.at(zc-1)->yco-((Zli2->Siz / 10.0) * 0.4);
									else
										ytrans = Zli2->yco-m_doc->docParagraphStyles[item->itemText.at(startLin+zc)->cab].LineSpa-((Zli2->Siz / 10.0) * 0.4);
								}
								if (zc > 0)
									xtrans =  LiList.at(zc-1)->xco + Cwidth(m_doc, LiList.at(zc-1)->ZFo, LiList.at(zc-1)->Zeich, LiList.at(zc-1)->Siz);
								else
								{
									if (startLin > 0)
										xtrans = item->itemText.at(startLin-1)->xp + Cwidth(m_doc, item->itemText.at(startLin-1)->cfont, item->itemText.at(startLin-1)->ch, item->itemText.at(startLin-1)->csize);
									else
										xtrans = Zli2->xco;
								}
								chma4.translate(xtrans, ytrans);
								chma.scale(Zli2->Siz / 100.0, Zli2->Siz / 100.0);
								chma2.scale(Zli2->scale / 1000.0, Zli2->scalev / 1000.0);
								chma5.scale(painter->zoomFactor(), painter->zoomFactor());
								points.map(chma * chma2 * chma4 * chma5);
								painter->setupTextPolygon(&points);
								painter->setFillMode(1);
								painter->fillPath();
							}
						}
						if (e2.intersects(pf2.xForm(QRect(qRound(Zli2->xco),qRound(Zli2->yco-asce), qRound(Zli2->wide+1), qRound(asce+desc)))))
						{
							if (Zli2->Zeich == QChar(25))
								DrawItem_Embedded(item, painter, e, Zli2);
							else
								DrawCharacters(item, painter, Zli2);
						}
					}
					tabDist = Zli2->xco+Zli2->wide;
				/*}*/
				goNextColumn = false;
				LiList.clear();
				BuPos = 0;
				LastSP = 0;
				outs = false;
			}
			item->MaxChars = item->itemText.count();
			//Redrawn = true;
			//Dirty = false;
			pf2.end();
			painter->restore();
			break;
			NoRoom: pf2.end();
			if (item->NextBox != 0)
			{
				nrc2 = item->itemText.count();
				for (uint ss=nrc; ss<nrc2; ++ss)
				{
					item->NextBox->itemText.append(item->itemText.take(nrc));
				}
				if (uint(item->CPos) > nrc)
				{
					int nCP = item->CPos - nrc;
					item->CPos = nrc;
					if ((m_doc->appMode == modeEdit) && (item->Tinput))
					{
//							OwnPage->Deselect(true);
						item->NextBox->CPos = QMAX(nCP, 1);
//							m_doc->currentPage = NextBox->OwnPage;
//							NextBox->OwnPage->SelectItemNr(NextBox->ItemNr);
						break;
					}
				}
				painter->save();
				bool rep = m_doc->RePos;
				m_doc->RePos = true;
				//NextBox->Dirty = false;
				QPixmap pgPix(1, 1);
				ScPainter *scp = new ScPainter(&pgPix, 1, 1);
				scp->translate(0.5, 0.5);
				item->NextBox->DrawObj(scp, QRect(0, 0, 1, 1));
				//NextBox->Dirty = true;
				scp->end();
				delete scp;
				painter->restore();
				m_doc->RePos = rep;
			}
			item->MaxChars = nrc;
			//Redrawn = true;
			painter->restore();
		}
			break;
		default:
			break;
	}
}


