/***************************************************************************
                          hruler.cpp  -  description
                             -------------------
    begin                : Tue Apr 10 2001
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

#include "hruler.h"
#include "hruler.moc"
#include "page.h"
#include <qcursor.h>
#include <qcolor.h>
#include <qrect.h>
#include <qpointarray.h>

#if QT_VERSION  > 0x030102
	#define SPLITHC SplitVCursor
#else
	#define SPLITHC SplitHCursor
#endif

Hruler::Hruler(QScrollView *pa, ScribusDoc *doc) : QWidget(pa)
{
	setEraseColor(QColor(255,255,255));
	doku = doc;
	offs = -10;
	Markp = 0;
	repX = false;
	Mpressed = false;
	ItemPosValid = false;
	RulerCode = 0;
}

void Hruler::mousePressEvent(QMouseEvent *m)
{
	Mpressed = true;
	if (ItemPosValid)
	{
		RulerCode = 0;
		double Pos = (ItemPos+Extra+lineCorr+Offset)*Scaling-offs;
		if ((static_cast<int>(Pos) < (m->x()+doku->GrabRad)) && (static_cast<int>(Pos) > (m->x()-doku->GrabRad)))
			RulerCode = 1;
		Pos = (ItemEndPos-RExtra-lineCorr+Offset)*Scaling-offs;
		if ((static_cast<int>(Pos) < (m->x()+doku->GrabRad)) && (static_cast<int>(Pos) > (m->x()-doku->GrabRad)))
			RulerCode = 2;
		MouseX = m->x();
	}
	else
		qApp->setOverrideCursor(QCursor(SPLITHC), true);
}

void Hruler::mouseReleaseEvent(QMouseEvent *m)
{
	Mpressed = false;
	if ((ItemPosValid) && (m->y() < height()) && (m->y() > 0))
	{
		switch (RulerCode)
		{
			case 1:
				doku->ActPage->SelItem.at(0)->Extra = Extra;
				break;
			case 2:
				doku->ActPage->SelItem.at(0)->RExtra = RExtra;
				break;
			default:
				break;
		}
		doku->ActPage->RefreshItem(doku->ActPage->SelItem.at(0),true);
		doku->ActPage->EmitValues(doku->ActPage->SelItem.at(0));
	}
	else
	{
		doku->ActPage->DrHY = -1;
		doku->ActPage->SetYGuide(m);
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	}
}

void Hruler::mouseMoveEvent(QMouseEvent *m)
{
	if (ItemPosValid)
	{
		if ((Mpressed) && (m->y() < height()) && (m->y() > 0))
		{
			double toplimit = ItemEndPos-ItemPos-2*lineCorr-Extra - (ColGap * (Cols - 1))-1;
			double toplimit2 = ItemEndPos-ItemPos-2*lineCorr-RExtra - (ColGap * (Cols - 1))-1;
			switch (RulerCode)
			{
				case 1:
					Extra -= MouseX - m->x();
					if (Extra < 0)
					{
						Extra = 0;
						QCursor::setPos(mapToGlobal(QPoint(static_cast<int>((ItemPos+Extra+lineCorr+Offset)*Scaling-offs), m->y())));
					}
					if (Extra > toplimit2)
					{
						Extra = toplimit2;
						QCursor::setPos(mapToGlobal(QPoint(static_cast<int>((ItemEndPos-RExtra-lineCorr-(ColGap*(Cols-1))-1+Offset)*Scaling-offs), m->y())));
					}
					repaint();
					break;
				case 2:
					RExtra += MouseX - m->x();
					if (RExtra < 0)
					{
						RExtra = 0;
						QCursor::setPos(mapToGlobal(QPoint(static_cast<int>((ItemEndPos-RExtra-lineCorr+Offset)*Scaling-offs), m->y())));
					}
					if (RExtra > toplimit)
					{
						RExtra = toplimit;
						QCursor::setPos(mapToGlobal(QPoint(static_cast<int>((ItemPos+Extra+lineCorr+(ColGap*(Cols-1))-1+Offset)*Scaling-offs), m->y())));
					}
					repaint();
					break;
				default:
					break;
			}
			MouseX = m->x();
		}
	}
	else
	{
		if ((Mpressed) && (m->pos().y() > height()))
			doku->ActPage->FromHRuler(m);
	}
}

void Hruler::paintEvent(QPaintEvent *)
{
	int pc, xx;
	double of, xl, iter, iter2;
	double sc = doku->Scale;
	int cor = 1;
	switch (doku->Einheit)
	{
		case 0:
			/* PFJ - 29.02.04 - Altered so the if isn't executed twice. Repeated */
			if (sc > 1 && sc <= 4)
				cor = 2;
			if (sc > 4)
				cor = 10;
			iter = 10.0 / cor;
	  		iter2 = iter * 10.0;
			break;
		case 1:
			if (sc > 1)
				cor = 10;
			iter = ((10.0 / 25.4) * 72.0) / cor;
  			iter2 = iter * 10.0;
			break;
		case 2:
			iter = 18.0;
			iter2 = 72.0;
			if (sc > 1 && sc <= 4)
			{
				cor = 2;
				iter = 9.0;
				iter2 = 36.0;
			}
			if (sc > 4)
			{
				iter = 9.0;
				iter2 = 18.0;
				cor = 4;
			}
			break;
		case 3:
			iter = 12.0;
			iter2 = 120.0;
			if (sc > 1 && sc <= 4)
			{
				cor = 1;
				iter = 12.0;
				iter2 = 60.0;
			}
			if (sc > 4)
			{
				cor = 2;
				iter = 6.0;
				iter2 = 12.0;
			}
			break;
	}
	QPainter p;
	p.begin(this);
	p.drawLine(0, 24, width(), 24);
	p.translate(-offs, 0);
	if (repX)
	{
		p.setPen(red);
		p.setBrush(red);
		QPointArray cr;
		cr.setPoints(3, Markp, 9, Markp+2, 0, Markp-2, 0);
		p.drawPolygon(cr);
		p.end();
		repX = false;
		return;
	}
	p.setBrush(black);
	p.setPen(black);
	p.setFont(font());
	((doku->PageFP) && (doku->PagesSbS)) ? pc = 2 : pc = 1;
	if (doku->MasterP)
		pc = 1;
	for (xx = 0; xx < pc; ++xx)
	{
		p.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
		of = xx * (doku->PageB+30.0);
		Offset = of;
		Scaling = sc;
		for (xl = 0; xl < doku->PageB; xl += iter)
			p.drawLine(qRound((xl+of)*sc), 18, qRound((xl+of)*sc), 24);
		for (xl = 0; xl < doku->PageB+(iter2/2); xl += iter2)
		{
			p.drawLine(qRound((xl+of)*sc), 11, qRound((xl+of)*sc), 24);
			switch (doku->Einheit)
			{
				case 2:
				{
					QString tx = "";
					int num1 = static_cast<int>(xl / iter2 / cor);
					if (num1 != 0)
						tx = QString::number(num1);
					double frac = (xl / iter2 / cor) - num1;
					if ((frac > 0.24) && (frac < 0.26))
						tx += QChar(0xBC);
					if ((frac > 0.49) && (frac < 0.51))
						tx += QChar(0xBD);
					if ((frac > 0.74) && (frac < 0.76))
						tx += QChar(0xBE);
					p.drawText(qRound((xl+of+2/sc) * sc), 17, tx);
					break;
				}
				case 3:
					p.drawText(qRound((xl+of+2/sc) * sc), 17, QString::number(xl / iter / cor));
					break;
				default:
					p.drawText(qRound((xl+of+2/sc) * sc), 17, QString::number(xl / iter * 10 / cor));
					break;
			}
		}
		if (((xx == 0) && (doku->ActPage->PageNr % 2 == 0) && (doku->FirstPageLeft))
		    || ((xx == 1) && (doku->ActPage->PageNr % 2 != 0) && (doku->FirstPageLeft))
		    || ((xx == 0) && (doku->ActPage->PageNr % 2 != 0) && (!doku->FirstPageLeft))
		    || ((xx == 1) && (doku->ActPage->PageNr % 2 == 0) && (!doku->FirstPageLeft))
			|| (pc == 1))
		{
			if (ItemPosValid)
			{
				p.eraseRect(QRect(QPoint(qRound((ItemPos+Extra+of)*sc), 10), QPoint(qRound((ItemEndPos-RExtra+of)*sc), 23)));
				p.drawLine(qRound((ItemPos+Extra+of)*sc), 24, qRound((ItemEndPos-RExtra+of)*sc), 24);
				p.save();
				if (Revers)
				{
					p.translate(qRound((ItemPos+of)*sc), 0);
					p.scale(-1, 1);
					p.translate(qRound((ItemPos+Extra+of)*sc-(ItemEndPos-RExtra+of)*sc), 0);
					p.translate(-qRound((ItemPos+of)*sc), 0);
				}
				double Pos = ItemPos;
				double EndPos = ItemEndPos;
				for (int CurrCol = 0; CurrCol < Cols; ++CurrCol)
				{
					double ColWidth = (ItemEndPos - ItemPos - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
					Pos = ItemPos + (ColWidth + ColGap) * CurrCol+Extra + lineCorr;
					EndPos = Pos+ColWidth;
					p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
					for (xl = Pos; xl < EndPos; xl += iter)
						p.drawLine(qRound((xl+of)*sc), 18, qRound((xl+of)*sc), 24);
					for (xl = Pos; xl < EndPos; xl += iter2)
					{
						p.drawLine(qRound((xl+of)*sc), 11, qRound((xl+of)*sc), 24);
						switch (doku->Einheit)
						{
							case 2:
							{
								QString tx = "";
								int num1 = static_cast<int>((xl-Pos) / iter2 / cor);
								if (num1 != 0)
									tx = QString::number(num1);
								double frac = (xl / iter2 / cor) - num1;
								if ((frac > 0.24) && (frac < 0.26))
									tx += QChar(0xBC);
								if ((frac > 0.49) && (frac < 0.51))
									tx += QChar(0xBD);
								if ((frac > 0.74) && (frac < 0.76))
									tx += QChar(0xBE);
								if (Revers)
								{
									p.save();
									p.translate(qRound((xl+of-2/sc) * sc),0);
									p.scale(-1,1);
									p.drawText(0, 17, tx);
									p.restore();
								}
								else
									p.drawText(qRound((xl+of+2/sc) * sc), 17, tx);
								break;
							}
							case 3:
								if (Revers)
								{
									p.save();
									p.translate(qRound((xl+of-2/sc) * sc),0);
									p.scale(-1,1);
									p.drawText(0, 17, QString::number((xl-Pos) / iter / cor));
									p.restore();
								}
								else
									p.drawText(qRound((xl+of+2/sc) * sc), 17, QString::number((xl-Pos) / iter / cor));
								break;
							default:
								if (Revers)
								{
									p.save();
									p.translate(qRound((xl+of-2/sc) * sc),0);
									p.scale(-1,1);
									p.drawText(0, 17, QString::number((xl-Pos) / iter * 10 / cor));
									p.restore();
								}
								else
									p.drawText(qRound((xl+of+2/sc) * sc), 17, QString::number((xl-Pos) / iter * 10 / cor));
								break;
						}
					}
					p.setPen(QPen(blue, 2, SolidLine, FlatCap, MiterJoin));
					p.drawLine(qRound((Pos+of)*sc), 11, qRound((Pos+of)*sc), 23);
					if (CurrCol == 0)
					{
						p.drawLine(qRound((Pos+of)*sc), 23, qRound((Pos+of+4/sc)*sc), 23);
						p.drawLine(qRound((Pos+of)*sc), 11, qRound((Pos+of+4/sc)*sc), 11);
					}
					if (doku->CurrentABStil > 4)
					{
						double fpos = Pos+First+Indent+of;
						QPointArray cr;
						cr.setPoints(3, qRound(fpos*sc), 17, qRound((fpos+3/sc)*sc), 11, qRound((fpos-3/sc)*sc), 11);
						p.drawPolygon(cr);
						QPointArray cr2;
						cr2.setPoints(3, qRound((Pos+Indent+of)*sc), 17, qRound((Pos+Indent+of+3/sc)*sc), 23, qRound((Pos+Indent+of-3/sc)*sc), 23);
						p.drawPolygon(cr2);
					}
					if (TabValues.count() != 0)
					{
						p.setPen(QPen(black, 2, SolidLine, FlatCap, MiterJoin));
						for (int yg = 0; yg < static_cast<int>(TabValues.count()-1); yg += 2)
						{
							if (Pos+TabValues[yg+1] < EndPos)
							{
								switch (static_cast<int>(TabValues[yg]))
								{
									case 0:
										if (Revers)
										{
											p.save();
											p.translate(qRound((Pos+TabValues[yg+1]+of)*sc),0);
											p.scale(-1,1);
											p.drawLine(0, 15, 0, 23);
											p.drawLine(0, 23, 8, 23);
											p.restore();
										}
										else
										{
											p.drawLine(qRound((Pos+TabValues[yg+1]+of)*sc), 15, qRound((Pos+TabValues[yg+1]+of)*sc), 23);
											p.drawLine(qRound((Pos+TabValues[yg+1]+of)*sc), 23, qRound((Pos+TabValues[yg+1]+of+8/sc)*sc), 23);
										}
										break;
									case 1:
										if (Revers)
										{
											p.save();
											p.translate(qRound((Pos+TabValues[yg+1]+of)*sc),0);
											p.scale(-1,1);
											p.drawLine(0, 15, 0, 23);
											p.drawLine(0, 23, -8, 23);
											p.restore();
										}
										else
										{
											p.drawLine(qRound((Pos+TabValues[yg+1]+of)*sc), 15, qRound((Pos+TabValues[yg+1]+of)*sc), 23);
											p.drawLine(qRound((Pos+TabValues[yg+1]+of)*sc), 23, qRound((Pos+TabValues[yg+1]+of-8/sc)*sc), 23);
										}
										break;
									case 2:
										p.drawLine(qRound((Pos+TabValues[yg+1]+of)*sc), 15, qRound((Pos+TabValues[yg+1]+of)*sc), 23);
										p.drawLine(qRound((Pos+TabValues[yg+1]+of-4/sc)*sc), 23, qRound((Pos+TabValues[yg+1]+of+4/sc)*sc), 23);
										break;
									case 3:
									case 4:
										p.drawLine(qRound((Pos+TabValues[yg+1]+of)*sc), 15, qRound((Pos+TabValues[yg+1]+of)*sc), 23);
										p.drawLine(qRound((Pos+TabValues[yg+1]+of-4/sc)*sc), 23, qRound((Pos+TabValues[yg+1]+of+4/sc)*sc), 23);
										p.drawLine(qRound((Pos+TabValues[yg+1]+of+3/sc)*sc), 20, qRound((Pos+TabValues[yg+1]+of+2/sc)*sc), 20);
									default:
										break;
								}
							}
						}
					}
					p.setPen(QPen(blue, 2, SolidLine, FlatCap, MiterJoin));
					p.drawLine(qRound((EndPos+of)*sc), 11, qRound((EndPos+of)*sc), 23);
					if (CurrCol == Cols-1)
					{
						p.drawLine(qRound((EndPos+of)*sc), 23, qRound((EndPos+of-4/sc)*sc), 23);
						p.drawLine(qRound((EndPos+of)*sc), 11, qRound((EndPos+of-4/sc)*sc), 11);
					}
				}
			p.restore();
			}
		}
	}
	p.end();
}

/** Zeichnet den Pfeil */
void Hruler::Draw(int wo)
{
	repX = true;
	Markp = wo-qRound(10*doku->Scale);
	repaint(QRect(0, 0, width(), 9));
}
