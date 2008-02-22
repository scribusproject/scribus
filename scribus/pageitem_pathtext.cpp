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

#include "pageitem_pathtext.h"
#include <QGridLayout>
#include <QImage>
#include <QList>
#include <QPainterPath>

#include <cmath>
#include <cassert>

#include "page.h"
#include "pageitem.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scpainter.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "commonstrings.h"
#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include "util.h"
#include "util_math.h"

#include "text/nlsconfig.h"

using namespace std;

PageItem_PathText::PageItem_PathText(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
	: PageItem(pa, PageItem::PathText, x, y, w, h, w2, fill, outline)
{
	firstChar = 0;
	MaxChars = itemText.length();
}


void PageItem_PathText::layout()
{
	QImage pgPix(10, 10, QImage::Format_ARGB32);
	QRectF rd; // = QRect(0,0,9,9);
	ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());	
	DrawObj(painter, rd);
	painter->end();
	delete painter;
	Frame = true;
	updatePolyClip();
}


void PageItem_PathText::DrawObj_Item(ScPainter *p, QRectF cullingArea, double sc)
{
	itemText.invalidateAll();
	firstChar = 0;
	MaxChars = 0;
	int a;
	double chs;
	QString chstr, chstr2, chstr3;
	ScText *hl;
	double dx;
//	double sp = 0;
//	double oldSp = 0;
//	double oCurX = 0;
	FPoint point = FPoint(0, 0);
//	FPoint normal = FPoint(0, 0);
	FPoint tangent = FPoint(0, 0);
//	FPoint extPoint = FPoint(0, 0);
//	bool ext = false;
//	bool first = true;
//	double fsx = 0;
	uint seg = 0;
	double segLen = 0;
//	double distCurX;
	QColor tmp;
	CurX = Extra;
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
	if (!m_Doc->layerOutline(LayerNr))
	{
		if (PoShow)
		{
			if (lineColor() != CommonStrings::None && PoShow)
			{
				p->setupPolygon(&PoLine, false);
				p->strokePath();
			}
			else
			{
				if (NamedLStyle.isEmpty())
					p->drawLine(FPoint(0, 0), FPoint(Width, 0));
				else
				{
					multiLine ml = m_Doc->MLineStyles[NamedLStyle];
					for (int it = ml.size()-1; it > -1; it--)
					{
						if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
						{
							SetQColor(&tmp, ml[it].Color, ml[it].Shade);
							p->setPen(tmp, ml[it].Width, static_cast<Qt::PenStyle>(ml[it].Dash), static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
							p->drawLine(FPoint(0, 0), FPoint(Width, 0));
						}
					}
				}
			}
		}
	}
	double totalTextLen = 0.0;
	double totalCurveLen = 0.0;
	double extraOffset = 0.0;
	if (itemText.length() != 0)
	{
		CurX += itemText.charStyle(0).fontSize() * itemText.charStyle(0).tracking() / 10000.0;
		totalTextLen += itemText.charStyle(0).fontSize() * itemText.charStyle(0).tracking() / 10000.0;
	}
	segLen = PoLine.lenPathSeg(seg);
	for (a = firstChar; a < itemText.length(); ++a)
	{
		hl = itemText.item(a);
		chstr = hl->ch;
		if (chstr[0] == SpecialChars::PAGENUMBER || chstr[0] == SpecialChars::PARSEP || chstr[0] == SpecialChars::PAGECOUNT
			|| chstr[0] == SpecialChars::TAB || chstr[0] == SpecialChars::LINEBREAK)
			continue;
		if (a < itemText.length()-1)
			chstr += itemText.text(a+1, 1);
		hl->glyph.yadvance = 0;
		layoutGlyphs(itemText.charStyle(a), chstr, hl->glyph);
		hl->glyph.shrink();
		if (hl->ch == SpecialChars::OBJECT)
			totalTextLen += (hl->embedded.getItem()->gWidth + hl->embedded.getItem()->lineWidth());
		else
			totalTextLen += hl->glyph.wide()+hl->fontSize() * hl->tracking() / 10000.0;
	}
	for (uint segs = 0; segs < PoLine.size()-3; segs += 4)
	{
		totalCurveLen += PoLine.lenPathSeg(segs);
	}
	if ((itemText.defaultStyle().alignment() != 0) && (totalCurveLen >= totalTextLen + Extra))
	{
		if (itemText.defaultStyle().alignment() == 2)
		{
			CurX = totalCurveLen  - totalTextLen;
			CurX -= Extra;
		}
		if (itemText.defaultStyle().alignment() == 1)
			CurX = (totalCurveLen - totalTextLen) / 2.0;
		if ((itemText.defaultStyle().alignment() == 3) || (itemText.defaultStyle().alignment() == 4))
			extraOffset = (totalCurveLen - Extra  - totalTextLen) / static_cast<double>(itemText.length());
	}
#ifndef NLS_PROTO
	QPainterPath guidePath = PoLine.toQPainterPath(false);
	QList<QPainterPath> pathList = decomposePath(guidePath);
	QPainterPath currPath = pathList[0];
	int currPathIndex = 0;
	for (a = firstChar; a < itemText.length(); ++a)
	{
		CurY = 0;
		hl = itemText.item(a);
		chstr = hl->ch;
		if (chstr[0] == SpecialChars::PAGENUMBER || chstr[0] == SpecialChars::PARSEP || chstr[0] == SpecialChars::PAGECOUNT
			|| chstr[0] == SpecialChars::TAB || chstr[0] == SpecialChars::LINEBREAK)
			continue;
		chs = hl->fontSize();
		if (a < itemText.length()-1)
			chstr += itemText.text(a+1, 1);
		hl->glyph.yadvance = 0;
		layoutGlyphs(itemText.charStyle(a), chstr, hl->glyph);
		hl->glyph.shrink();                                                           // HACK
		if (hl->ch == SpecialChars::OBJECT)
			dx = (hl->embedded.getItem()->gWidth + hl->embedded.getItem()->lineWidth()) / 2.0;
		else
			dx = hl->glyph.wide() / 2.0;
//		qDebug(QString("pathtext-draw: parent %1 parentc %2").arg((uint)itemText.paragraphStyle(a).parentStyle()).arg((uint)itemText.charStyle(a).parentStyle()));
//		qDebug(QString("pathtext-draw: co %1 %2 %3 %4").arg(itemText.charStyle(a).fillColor()).arg(itemText.charStyle(a).fillShade()).arg(itemText.charStyle(a).strokeColor()).arg(itemText.charStyle(a).strokeShade()));
//		qDebug(QString("pathtext-draw: fo %1 %2").arg(itemText.charStyle(a).font().scName()).arg(hl->glyph.glyph));
		CurX += dx;

		double currPerc = currPath.percentAtLength(CurX);
		if (currPerc >= 0.9999999)
		{
			currPathIndex++;
			if (currPathIndex == pathList.count())
				break;
			currPath = pathList[currPathIndex];
			CurX = dx;
			currPerc = currPath.percentAtLength(CurX);
		}
		double currAngle = currPath.angleAtPercent(currPerc);
		QPointF currPoint = currPath.pointAtPercent(currPerc);
		tangent = FPoint(cos(currAngle * M_PI / 180.0), sin(currAngle * M_PI / 180.0));
		point = FPoint(currPoint.x(), currPoint.y());

/*
		ext = false;
		while ( (seg < PoLine.size()-3) && (CurX > fsx + segLen))
		{
			fsx += segLen;
			seg += 4;
			if (seg > PoLine.size()-3)
				break;
			segLen = PoLine.lenPathSeg(seg);
			ext = true;
		}
		if (seg > PoLine.size()-3)
			break;
		if (CurX > fsx + segLen)
			break;
		if (ext)
		{
			sp = 0;
			distCurX = PoLine.lenPathDist(seg, 0, sp);
			while (distCurX <= ((CurX - oCurX) - (fsx - oCurX)))
			{
				sp += 0.001;
				distCurX = PoLine.lenPathDist(seg, 0, sp);
			}
			PoLine.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
			CurX = (CurX - (CurX - fsx)) + distCurX;
			oldSp = sp;
			ext = false;
		}
		else
		{
			if( seg < PoLine.size()-3 )
			{
				if (CurX > fsx + segLen)
					break;
				distCurX = PoLine.lenPathDist(seg, oldSp, sp);
				while (distCurX <= (CurX - oCurX))
				{
					sp += 0.001;
					if (sp >= 1.0)
					{
						sp = 0.9999;
						break;
					}
					distCurX = PoLine.lenPathDist(seg, oldSp, sp);
				}
				PoLine.pointTangentNormalAt(seg, sp, &point, &tangent, &normal );
				CurX = oCurX + distCurX;
				oldSp = sp;
			}
			else
				break;
		}
*/
		hl->glyph.xoffset = 0;
		hl->glyph.yoffset = BaseOffs;
		hl->PtransX = tangent.x();
		hl->PtransY = tangent.y();
		hl->PRot = dx;
		QMatrix trafo = QMatrix( 1, 0, 0, -1, -dx, 0 );
		if (textPathFlipped)
			trafo *= QMatrix(1, 0, 0, -1, 0, 0);
		if (textPathType == 0)
			trafo *= QMatrix( tangent.x(), tangent.y(), tangent.y(), -tangent.x(), point.x(), point.y() ); // ID's Rainbow mode
		else if (textPathType == 1)
			trafo *= QMatrix( 1, 0, 0, -1, point.x(), point.y() ); // ID's Stair Step mode
		else if (textPathType == 2)
		{
			double a = 1;
			if (tangent.x() < 0)
				a = -1;
			if (fabs(tangent.x()) > 0.1)
				trafo *= QMatrix( a, (tangent.y() / tangent.x()) * a, 0, -1, point.x(), point.y() ); // ID's Skew mode
			else
				trafo *= QMatrix( a, 4 * a, 0, -1, point.x(), point.y() );
		}
		QMatrix sca = p->worldMatrix();
		trafo *= sca;
		p->save();
		QMatrix savWM = p->worldMatrix();
		p->setWorldMatrix(trafo);
		if (!m_Doc->RePos)
		{
			actFill = itemText.charStyle(a).fillColor();
			actFillShade = itemText.charStyle(a).fillShade();
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
			actStroke = itemText.charStyle(a).strokeColor();
			actStrokeShade = itemText.charStyle(a).strokeShade();
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
			if (hl->ch == SpecialChars::OBJECT)
			{
				p->translate(0.0, BaseOffs);
				DrawObj_Embedded(p, cullingArea, itemText.charStyle(a), hl->embedded.getItem());
			}
			else
				drawGlyphs(p, itemText.charStyle(a), hl->glyph);
		}
		hl->glyph.xoffset = point.x();
		hl->glyph.yoffset = point.y();
		p->setWorldMatrix(savWM);
		p->restore();
		MaxChars = a+1;
//		oCurX = CurX;
		CurX -= dx;
		if (hl->ch == SpecialChars::OBJECT)
			CurX += (hl->embedded.getItem()->gWidth + hl->embedded.getItem()->lineWidth());
		else
			CurX += hl->glyph.wide()+hl->fontSize() * hl->tracking() / 10000.0 + extraOffset;
//		first = false;
	}
	MaxChars++;  // ugly Hack
#endif
//	qDebug(QString("PageItem_PathText::DrawObj_Item repos=%1, %2 chars, [%3 %4 %5 %6 %7 %8] with %9").arg(m_Doc->RePos).arg(MaxChars)
//		   .arg(p->worldMatrix().m11()).arg(p->worldMatrix().m12()).arg(p->worldMatrix().m21()).arg(p->worldMatrix().m22()).arg(p->worldMatrix().dx()).arg(p->worldMatrix().dy())
//		   .arg(QString("pen %1 brush%2 device%3 isPainting=%4").arg(p->pen().rgb()).arg(p->brush().rgb()).arg(p->device()? p->device()->paintingActive() : -999))
//		   );
}

bool PageItem_PathText::createInfoGroup(QFrame *infoGroup, QGridLayout *infoGroupLayout)
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
	
	
	infoCT->setText("Text on a Path");
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

bool PageItem_PathText::createContextMenu(QMenu *menu, int step)
{
	QMap<QString, QPointer<ScrAction> > actions = doc()->scMW()->scrActions;
	
	if (menu == 0) return false;
	
	switch(step) {
		case 10:
			menu->addSeparator();
			menu->addAction(actions["toolsEditWithStoryEditor"]);
			break;
		case 30:
			menu->addAction(actions["itemConvertToOutlines"]);
		break;
		default:
			return false;
	}
	return true;
}

void PageItem_PathText::applicableActions(QStringList & actionList)
{
	actionList << "toolsEditWithStoryEditor";
	actionList << "itemConvertToOutlines";
}

QString PageItem_PathText::infoDescription()
{
	return QString();
}
