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
#include "scribusview.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "units.h"
#include "prefsmanager.h"

#if QT_VERSION  > 0x030102
	#define SPLITHC SplitVCursor
#else
	#define SPLITHC SplitHCursor
#endif

extern QPixmap loadIcon(QString nam);
extern ScribusApp* ScApp;

#ifdef QT_MAC
    #define topline 1
#else
    #define topline 3
#endif


Hruler::Hruler(ScribusView *pa, ScribusDoc *doc) : QWidget(pa)
{
	prefsManager=PrefsManager::instance();
	setEraseColor(QColor(255,255,255));
	currDoc = doc;
	currView = pa;
	offs = 0;
	Markp = 0;
	Mpressed = false;
	ItemPosValid = false;
	RulerCode = 0;
	setMouseTracking(true);
	unitChange();
}

void Hruler::mousePressEvent(QMouseEvent *m)
{
	Mpressed = true;
	if (currDoc->isLoading())
		return;
	if (ItemPosValid)
	{
		RulerCode = 0;
		Markp = -1;
		double Pos = (ItemPos-offs+Extra+lineCorr)*Scaling;
		if ((static_cast<int>(Pos) < (m->x()+currDoc->guidesSettings.grabRad)) && (static_cast<int>(Pos) > (m->x()-currDoc->guidesSettings.grabRad)))
			RulerCode = 1;
		Pos = (ItemEndPos-offs-RExtra-lineCorr)*Scaling;
		if ((static_cast<int>(Pos) < (m->x()+currDoc->guidesSettings.grabRad)) && (static_cast<int>(Pos) > (m->x()-currDoc->guidesSettings.grabRad)))
			RulerCode = 2;
		double ColWidth = (ItemEndPos - ItemPos - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
		QRect fpo;
		ActCol = 0;
		for (int CurrCol = 0; CurrCol < Cols; ++CurrCol)
		{
			Pos = (ItemPos-offs+(ColWidth+ColGap)*CurrCol+Extra+lineCorr)*Scaling;
			fpo = QRect(static_cast<int>(Pos), topline, static_cast<int>(ColWidth*Scaling), 12);
			if (fpo.contains(m->pos()))
			{
				ActCol = CurrCol+1;
				break;
			}
		}
		if (ActCol == 0)
			return;
		if (currDoc->currentParaStyle > 4)
		{
			Pos = (ItemPos-offs+First+Indent+(ColWidth+ColGap)*(ActCol-1)+Extra+lineCorr)*Scaling;
			fpo = QRect(static_cast<int>(Pos)-3, topline, 6, 6);
			if (fpo.contains(m->pos()))
			{
				RulerCode = 3;
				MouseX = m->x();
				return;
			}
			Pos = (ItemPos-offs+Indent+(ColWidth+ColGap)*(ActCol-1)+Extra+lineCorr)*Scaling;
			fpo = QRect(static_cast<int>(Pos)-3, 9, 6, 6);
			if (fpo.contains(m->pos()))
			{
				RulerCode = 4;
				MouseX = m->x();
				return;
			}
		}
		if (TabValues.count() != 0)
		{
			for (int yg = 0; yg < static_cast<int>(TabValues.count()); yg++)
			{
				Pos = (ItemPos-offs+TabValues[yg].tabPosition+(ColWidth+ColGap)*(ActCol-1)+Extra+lineCorr)*Scaling;
				fpo = QRect(static_cast<int>(Pos)-3, 7, 8, 8);
				if (fpo.contains(m->pos()))
				{
					RulerCode = 5;
					ActTab = yg;
					break;
				}
			}
		}
		if ((RulerCode == 0) && (ActCol != 0) && (m->button() == LeftButton))
		{
			double Pos = (ItemPos-offs+Extra+lineCorr)*Scaling;
			int newY = m->x() - static_cast<int>(Pos);
			struct PageItem::TabRecord tb;
			tb.tabPosition = newY / Scaling;
			tb.tabType = 0;
			tb.tabFillChar = currDoc->toolSettings.tabFillChar[0];
			TabValues.prepend(tb);
			ActTab = 0;
			RulerCode = 5;
			UpdateTabList();
			qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
			emit DocChanged(false);
		}
		MouseX = m->x();
	}
	else
	{
		if (prefsManager->appPrefs.guidesSettings.guidesShown)
		{
			QPoint py = currView->viewport()->mapFromGlobal(m->globalPos());
			currView->DrHY = py.y();
			qApp->setOverrideCursor(QCursor(SPLITHC), true);
		}
	}
}

void Hruler::mouseReleaseEvent(QMouseEvent *m)
{
	if (currDoc->isLoading())
	{
		Mpressed = false;
		return;
	}
	if (ItemPosValid)
	{
		if ((m->y() < height()) && (m->y() > 0))
		{
			switch (RulerCode)
			{
				case 1:
					currView->SelItem.at(0)->Extra = Extra;
					emit DocChanged(false);
					break;
				case 2:
					currView->SelItem.at(0)->RExtra = RExtra;
					emit DocChanged(false);
					break;
				case 3:
					currDoc->docParagraphStyles[currDoc->currentParaStyle].First = First;
					emit DocChanged(false);
					break;
				case 4:
					currDoc->docParagraphStyles[currDoc->currentParaStyle].Indent = Indent;
					currDoc->docParagraphStyles[currDoc->currentParaStyle].First = First;
					emit DocChanged(false);
					break;
				case 5:
					if (m->button() == RightButton)
					{
						TabValues[ActTab].tabType += 1;
						if (TabValues[ActTab].tabType > 4)
							TabValues[ActTab].tabType = 0;
					}
					if (currDoc->currentParaStyle > 4)
						currDoc->docParagraphStyles[currDoc->currentParaStyle].TabValues = TabValues;
					else
						currView->SelItem.at(0)->TabValues = TabValues;
					emit DocChanged(false);
					break;
				default:
					break;
			}
		}
		else
		{
			if (RulerCode == 5)
			{
				QValueList<PageItem::TabRecord>::Iterator it;
				it = TabValues.at(ActTab);
				TabValues.remove(it);
				ActTab = 0;
				if (currDoc->currentParaStyle > 4)
					currDoc->docParagraphStyles[currDoc->currentParaStyle].TabValues = TabValues;
				else
					currView->SelItem.at(0)->TabValues = TabValues;
				emit DocChanged(false);
				qApp->setOverrideCursor(QCursor(ArrowCursor), true);
			}
		}
		RulerCode = 0;
		currView->DrawNew();
		currView->EmitValues(currView->SelItem.at(0));
	}
	else
	{
		if ((Mpressed) && (m->pos().y() > height()))
		{
			currView->DrHY = -1;
			currView->SetYGuide(m, -1);
		}
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		emit DocChanged(false);
		currView->updateContents();
	}
	Mpressed = false;
}

void Hruler::mouseMoveEvent(QMouseEvent *m)
{
	if (currDoc->isLoading())
		return;
	if (ItemPosValid)
	{
		double ColWidth = (ItemEndPos - ItemPos - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
		int ColEnd, ColStart;
		double oldInd;
		if (RulerCode > 2)
		{
			ColStart  = static_cast<int>((ItemPos-offs+(ColWidth+ColGap)*(ActCol-1)+Extra+lineCorr)*Scaling);
			ColEnd = static_cast<int>((ItemPos-offs+(ColWidth+ColGap)*(ActCol-1)+ColWidth+Extra+lineCorr)*Scaling);
		}
		else
		{
			ColStart =static_cast<int>((ItemPos-offs+lineCorr)*Scaling);
			ColEnd = static_cast<int>((ItemEndPos-offs-lineCorr)*Scaling);
		}
		if ((Mpressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > ColStart) && (m->x() < ColEnd))
		{
			qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
			double toplimit = ItemEndPos-ItemPos-2*lineCorr-Extra - (ColGap * (Cols - 1))-1;
			double toplimit2 = ItemEndPos-ItemPos-2*lineCorr-RExtra - (ColGap * (Cols - 1))-1;
			switch (RulerCode)
			{
				case 1:
					Extra -= (MouseX - m->x()) / Scaling;
					if (Extra < 0)
						Extra = 0;
					if (Extra > toplimit2)
						Extra = toplimit2;
					emit MarkerMoved(Extra, 0);
					repaint();
					break;
				case 2:
					RExtra += (MouseX - m->x()) / Scaling;
					if (RExtra < 0)
						RExtra = 0;
					if (RExtra > toplimit)
						RExtra = toplimit;
					emit MarkerMoved(RExtra, 0);
					repaint();
					break;
				case 3:
					First -= (MouseX - m->x()) / Scaling;
					if ((ItemPos-offs+(ColWidth+ColGap)*ActCol+First+Indent)*Scaling < (ItemPos-offs+(ColWidth+ColGap)*ActCol)*Scaling)
						First += (MouseX - m->x()) / Scaling;
					if (First+Indent > ColWidth)
						First  = ColWidth-Indent;
					emit MarkerMoved(First, 0);
					repaint();
					break;
				case 4:
					oldInd = Indent+First;
					Indent -= (MouseX - m->x()) / Scaling;
					if (Indent < 0)
						Indent = 0;
					if (Indent > ColWidth-1)
						Indent  = ColWidth-1;
					First = oldInd - Indent;
					emit MarkerMoved(Indent, 0);
					repaint();
					break;
				case 5:
					TabValues[ActTab].tabPosition -= (MouseX - m->x()) / Scaling;
					if (TabValues[ActTab].tabPosition < 0)
						TabValues[ActTab].tabPosition = 0;
					if (TabValues[ActTab].tabPosition > ColWidth-1)
						TabValues[ActTab].tabPosition  = ColWidth-1;
					emit MarkerMoved(TabValues[ActTab].tabPosition, 0);
					UpdateTabList();
					repaint();
					break;
				default:
					break;
			}
			MouseX = m->x();
			if (RulerCode != 0)
			{
				QPoint py = currView->viewport()->mapFromGlobal(m->globalPos());
				QPainter p;
				p.begin(currView->viewport());
				p.setRasterOp(XorROP);
				p.setPen(QPen(white, 1, DotLine, FlatCap, MiterJoin));
				QPoint out = currView->contentsToViewport(QPoint(0, qRound(currDoc->currentPage->yOffset() * Scaling)));
				p.drawLine(Markp, out.y(), Markp, out.y()+qRound(currDoc->currentPage->height() * Scaling));
				p.drawLine(py.x(), out.y(), py.x(), out.y()+qRound(currDoc->currentPage->height() * Scaling));
				p.end();
				Markp = py.x();
			}
			return;
		}
		if ((!Mpressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > ColStart) && (m->x() < ColEnd))
		{
			qApp->setOverrideCursor(QCursor(loadIcon("tab.png"), 3), true);
			double Pos = (ItemPos-offs+Extra+lineCorr)*Scaling;
			if ((static_cast<int>(Pos) < (m->x()+currDoc->guidesSettings.grabRad)) && (static_cast<int>(Pos) > (m->x()-currDoc->guidesSettings.grabRad)))
				qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
			Pos = (ItemEndPos-offs-RExtra-lineCorr)*Scaling;
			if ((static_cast<int>(Pos) < (m->x()+currDoc->guidesSettings.grabRad)) && (static_cast<int>(Pos) > (m->x()-currDoc->guidesSettings.grabRad)))
				qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
			QRect fpo;
			double ColWidth = (ItemEndPos - ItemPos - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
			if (currDoc->currentParaStyle > 4)
			{
				for (int CurrCol = 0; CurrCol < Cols; ++CurrCol)
				{
					Pos = (ItemPos-offs+First+Indent+(ColWidth+ColGap)*CurrCol+Extra+lineCorr)*Scaling;
					fpo = QRect(static_cast<int>(Pos)-3, topline, 6, 6);
					if (fpo.contains(m->pos()))
					{
						qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
						break;
					}
					Pos = (ItemPos-offs+Indent+(ColWidth+ColGap)*CurrCol+Extra+lineCorr)*Scaling;
					fpo = QRect(static_cast<int>(Pos)-3, 9, 6, 6);
					if (fpo.contains(m->pos()))
					{
						qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
						break;
					}
				}
			}
			if (TabValues.count() != 0)
			{
				for (int CurrCol = 0; CurrCol < Cols; ++CurrCol)
				{
					for (int yg = 0; yg < static_cast<int>(TabValues.count()); yg++)
					{
						Pos = (ItemPos-offs+TabValues[yg].tabPosition+(ColWidth+ColGap)*CurrCol+Extra+lineCorr)*Scaling;
						fpo = QRect(static_cast<int>(Pos)-3, 7, 8, 8);
						if (fpo.contains(m->pos()))
						{
							qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
							break;
						}
					}
				}
			}
			emit MarkerMoved((m->x() - static_cast<int>((ItemPos-offs+Extra+lineCorr)*Scaling)) / Scaling, 0);
			return;
		}
		if ((Mpressed) && (RulerCode == 5) && ((m->y() > height()) || (m->y() < 0)))
		{
			qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 4, 3), true);
			return;
		}
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	}
	else
	{
		if ((Mpressed) && (m->pos().y() > height()))
			currView->FromHRuler(m);
	}
}

void Hruler::paintEvent(QPaintEvent *e)
{
	if (currDoc->isLoading())
		return;
	QString tx = "";
	double xl, frac;
	double sc = currView->getScale();
	Scaling = sc;
	QFont ff = font();
	ff.setPointSize(8);
	setFont(ff);
	QPainter p;
	p.begin(this);
	p.setClipRect(e->rect());
	p.drawLine(0, 16, width(), 16);
	p.setBrush(black);
	p.setPen(black);
	p.setFont(font());
	double cc = width() / sc;
	double firstMark = ceil(offs / iter) * iter - offs;
	while (firstMark < cc)
	{
		p.drawLine(qRound(firstMark * sc), 10, qRound(firstMark * sc), 16);
		firstMark += iter;
	}
	firstMark = ceil(offs / iter2) * iter2 - offs;
	int markC = static_cast<int>(ceil(offs / iter2));
	while (firstMark < cc)
	{
		p.drawLine(qRound(firstMark * sc), topline, qRound(firstMark * sc), 16);
		switch (currDoc->unitIndex())
		{
			case 1:
				tx = QString::number(markC * iter2 / (iter2 / 100) / cor);
				break;
			case 2:
				xl = (markC * iter2 / iter2) / cor;
				tx = QString::number(static_cast<int>(xl));
				frac = xl - static_cast<int>(xl);
				if ((static_cast<int>(xl) == 0) && (frac > 0.1))
					tx = "";
				if ((frac > 0.24) && (frac < 0.26))
					tx += QChar(0xBC);
				if ((frac > 0.49) && (frac < 0.51))
					tx += QChar(0xBD);
				if ((frac > 0.74) && (frac < 0.76))
					tx += QChar(0xBE);
				tx = tx;
				break;
			case 3:
			case 5:
				tx = QString::number(markC * iter2 / (iter2 / 10) / cor);
				break;
			case 4:
				tx = QString::number(markC * iter2 / iter2 / cor);
				break;
			default:
				tx = QString::number(markC * iter2);
				break;
		}
		drawNumber(tx, qRound(firstMark * sc) + 2, 9, p);
		//p.drawText(qRound(firstMark * sc)+2, 9, tx);
		firstMark += iter2;
		markC++;
	}
	if (ItemPosValid)
	{
		double Pos = ItemPos-offs;
		double EndPos = ItemEndPos-offs;
		p.eraseRect(QRect(QPoint(qRound((Pos+Extra)*sc), 1), QPoint(qRound((EndPos-RExtra)*sc), 15)));
		p.drawLine(qRound((Pos+Extra)*sc), 16, qRound((EndPos-RExtra)*sc), 16);
		p.save();
		if (Revers)
		{
			p.translate(qRound((Pos)*sc), 0);
			p.scale(-1, 1);
			p.translate(qRound((Pos+Extra)*sc-(EndPos-RExtra)*sc), 0);
			p.translate(-qRound(Pos*sc), 0);
		}
		for (int CurrCol = 0; CurrCol < Cols; ++CurrCol)
		{
			double ColWidth = (ItemEndPos - ItemPos - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
			Pos = ItemPos-offs + (ColWidth + ColGap) * CurrCol+Extra + lineCorr;
			EndPos = Pos+ColWidth;
			p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
			for (xl = Pos; xl < EndPos; xl += iter)
				p.drawLine(qRound(xl*sc), 10, qRound(xl*sc), 16);
			for (xl = Pos; xl < EndPos; xl += iter2)
			{
				p.drawLine(qRound(xl*sc), topline, qRound(xl*sc), 16);
				switch (currDoc->unitIndex())
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
							p.translate(qRound((xl-2/sc) * sc),0);
							p.scale(-1,1);
							drawNumber(tx, 0, 17, p);
							//p.drawText(0, 17, tx);
							p.restore();
						}
						else
							drawNumber(tx, qRound((xl+2/sc) * sc), 9, p);
							//p.drawText(qRound((xl+2/sc) * sc), 9, tx);
						break;
					}
					case 3:
						if (Revers)
						{
							p.save();
							p.translate(qRound((xl-2/sc) * sc),0);
							p.scale(-1,1);
							drawNumber(QString::number((xl-Pos) / iter / cor), 0, 17, p); 
							//p.drawText(0, 17, QString::number((xl-Pos) / iter / cor));
							p.restore();
						}
						else
							drawNumber(QString::number((xl-Pos) / iter / cor), qRound((xl+2/sc) * sc), 9, p);
							//p.drawText(qRound((xl+2/sc) * sc), 9, QString::number((xl-Pos) / iter / cor));
						break;
					case 4:
						if (Revers)
						{
							p.save();
							p.translate(qRound((xl-2/sc) * sc),0);
							p.scale(-1,1);
							drawNumber(QString::number((xl-Pos) / iter / 10 / cor), 0, 9, p);
							//p.drawText(0, 9, QString::number((xl-Pos) / iter / 10 / cor));
							p.restore();
						}
						else
							drawNumber(QString::number((xl-Pos) / iter / 10 / cor), qRound((xl+2/sc) * sc), 9, p);
							//p.drawText(qRound((xl+2/sc) * sc), 9, QString::number((xl-Pos) / iter / 10 / cor));
						break;
					case 5:
						if (Revers)
						{
							p.save();
							p.translate(qRound((xl-2/sc) * sc),0);
							p.scale(-1,1);
							drawNumber(QString::number((xl-Pos) / iter  / cor), 0, 9, p);
							//p.drawText(0, 9, QString::number((xl-Pos) / iter  / cor));
							p.restore();
						}
						else
							drawNumber(QString::number((xl-Pos) / iter  / cor), qRound((xl+2/sc) * sc), 9, p);
							//p.drawText(qRound((xl+2/sc) * sc), 9, QString::number((xl-Pos) / iter  / cor));
						break;
					default:
						if (Revers)
						{
							p.save();
							p.translate(qRound((xl-2/sc) * sc),0);
							p.scale(-1,1);
							drawNumber(QString::number((xl-Pos) / iter * 10 / cor), 0, 9, p);
							//p.drawText(0, 9, QString::number((xl-Pos) / iter * 10 / cor));
							p.restore();
						}
						else
							drawNumber(QString::number((xl-Pos) / iter * 10 / cor), qRound((xl+2/sc) * sc), 9, p);
							//p.drawText(qRound((xl+2/sc) * sc), 9, QString::number((xl-Pos) / iter * 10 / cor));
						break;
				}
			}
			p.setPen(QPen(blue, 2, SolidLine, FlatCap, MiterJoin));
			p.drawLine(qRound(Pos*sc), topline, qRound(Pos*sc), 15);
			if (CurrCol == 0)
			{
				p.drawLine(qRound(Pos*sc), 15, qRound((Pos+4/sc)*sc), 15);
				p.drawLine(qRound(Pos*sc), topline, qRound((Pos+4/sc)*sc), topline);
			}
			if (currDoc->currentParaStyle > 4)
			{
				p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
				double fpos = Pos+First+Indent;
				QPointArray cr;
				cr.setPoints(3, qRound(fpos*sc), 9, qRound((fpos+3/sc)*sc), topline, qRound((fpos-3/sc)*sc), topline);
				p.drawPolygon(cr);
				QPointArray cr2;
				cr2.setPoints(3, qRound((Pos+Indent)*sc), 9, qRound((Pos+Indent+3/sc)*sc), 15, qRound((Pos+Indent-3/sc)*sc), 15);
				p.drawPolygon(cr2);
			}
			p.setPen(QPen(blue, 2, SolidLine, FlatCap, MiterJoin));
			if (TabValues.count() != 0)
			{
				p.setPen(QPen(black, 2, SolidLine, FlatCap, MiterJoin));
				for (int yg = 0; yg < static_cast<int>(TabValues.count()); yg++)
				{
					if (Pos+TabValues[yg].tabPosition < EndPos)
					{
						switch (static_cast<int>(TabValues[yg].tabType))
						{
							case 0:
								if (Revers)
								{
									p.save();
									p.translate(qRound((Pos+TabValues[yg].tabPosition)*sc),0);
									p.scale(-1,1);
									p.drawLine(0, 7, 0, 15);
									p.drawLine(0, 15, 8, 15);
									p.restore();
								}
								else
								{
									p.drawLine(qRound((Pos+TabValues[yg].tabPosition)*sc), 7, qRound((Pos+TabValues[yg].tabPosition)*sc), 15);
									p.drawLine(qRound((Pos+TabValues[yg].tabPosition)*sc), 15, qRound((Pos+TabValues[yg].tabPosition+8/sc)*sc), 15);
								}
								break;
							case 1:
								if (Revers)
								{
									p.save();
									p.translate(qRound((Pos+TabValues[yg].tabPosition)*sc),0);
									p.scale(-1,1);
									p.drawLine(0, 7, 0, 15);
									p.drawLine(0, 15, -8, 15);
									p.restore();
								}
								else
								{
									p.drawLine(qRound((Pos+TabValues[yg].tabPosition)*sc), 7, qRound((Pos+TabValues[yg].tabPosition)*sc), 15);
									p.drawLine(qRound((Pos+TabValues[yg].tabPosition)*sc), 15, qRound((Pos+TabValues[yg].tabPosition-8/sc)*sc), 15);
								}
								break;
							case 2:
							case 3:
								p.drawLine(qRound((Pos+TabValues[yg].tabPosition)*sc), 7, qRound((Pos+TabValues[yg].tabPosition)*sc), 15);
								p.drawLine(qRound((Pos+TabValues[yg].tabPosition-4/sc)*sc), 15, qRound((Pos+TabValues[yg].tabPosition+4/sc)*sc), 15);
								p.drawLine(qRound((Pos+TabValues[yg].tabPosition+3/sc)*sc), 12, qRound((Pos+TabValues[yg].tabPosition+2/sc)*sc), 12);
								break;
							case 4:
								p.drawLine(qRound((Pos+TabValues[yg].tabPosition)*sc), 7, qRound((Pos+TabValues[yg].tabPosition)*sc), 15);
								p.drawLine(qRound((Pos+TabValues[yg].tabPosition-4/sc)*sc), 15, qRound((Pos+TabValues[yg].tabPosition+4/sc)*sc), 15);
								break;
							default:
								break;
						}
					}
				}
			}
			p.setPen(QPen(blue, 2, SolidLine, FlatCap, MiterJoin));
			p.drawLine(qRound(EndPos*sc), topline, qRound(EndPos*sc), 15);
			if (CurrCol == Cols-1)
			{
				p.drawLine(qRound(EndPos*sc), 15, qRound((EndPos-4/sc)*sc), 15);
				p.drawLine(qRound(EndPos*sc), topline, qRound((EndPos-4/sc)*sc), topline);
			}
		}
		p.restore();
	}
	p.end();
}


void Hruler::drawNumber(QString txt, int x, int y0, QPainter & p)
{
	const int y = y0 - 3 + topline;
#ifndef QT_MAC
	p.drawText(x,y,txt);
#else
	static const int SCALE = 16;
	QFontMetrics fm = p.fontMetrics();
	QRect bbox = fm.boundingRect(txt);
	static QPixmap pix;
	if (pix.width() < bbox.width()*SCALE || pix.height() < bbox.height()*SCALE)
		pix.resize(bbox.width()*SCALE, bbox.height()*SCALE);
	QFont fnt = p.font();
	QPainter p2;
	pix.fill();
	p2.begin( &pix );
	if (fnt.pointSize() > 0)
		fnt.setPointSize(SCALE*fnt.pointSize()-SCALE/2);
	else if (fnt.pixelSize() > 0)
		fnt.setPixelSize(SCALE*fnt.pixelSize()-SCALE/2);
	else
		fnt.setPixelSize(SCALE);
	p2.setFont(fnt);
	p2.drawText(-bbox.x()*SCALE, -bbox.y()*SCALE, txt);
	p2.end();
	p.scale(1.0/SCALE,1.0/SCALE);
	p.drawPixmap(x*SCALE, (y+bbox.top())*SCALE, pix, 0, 0, bbox.width()*SCALE, bbox.height()*SCALE);
	p.scale(SCALE,SCALE);
#endif
}  


/** Zeichnet den Pfeil */
void Hruler::Draw(int wo)
{
	// erase old marker
	int currentCoor = wo - currView->contentsX();
	repaint(oldMark-3, 0, 7, 17);
	QPointArray cr;
	QPainter p;
#ifdef QT_MAC
	// draw new marker to pixmap
	static const int SCALE = 16;
	static const QColor BACKGROUND(255, 255, 255);
	static QPixmap pix( 4*SCALE, 16*SCALE );
	static bool initpix = true;
	if (initpix) {
		initpix = false;
		p.begin( &pix );
		p.setBrush( BACKGROUND );
		p.drawRect( 0, 0, 4*SCALE, 16*SCALE );
		
		p.setPen(red);
		p.setBrush(red);
		cr.setPoints(3, 2*SCALE, 16*SCALE, 4*SCALE, 0, 0, 0);
		p.drawPolygon(cr);
		p.end();
	}
	// draw pixmap
	p.begin(this);
	p.translate(-currView->contentsX(), 0);
	p.scale(1.0/SCALE, 1.0/(SCALE+1));
	p.drawPixmap((wo-2)*SCALE, 1, pix);
	p.end();
	// restore marks
	p.begin(this);
	p.setBrush(black);
	p.setPen(black);
	p.setFont(font());
	double sc = currView->getScale();
	double cc = width() / sc;
	double firstMark = ceil(offs / iter) * iter - offs;
	while (firstMark < cc)
	{
		p.drawLine(qRound(firstMark * sc), 10, qRound(firstMark * sc), 16);
		firstMark += iter;
	}
	p.end();
#else
	// draw slim marker
	p.begin(this);
	p.translate(-currView->contentsX(), 0);
	p.setPen(red);
	p.setBrush(red);
	cr.setPoints(5,  wo, 5, wo, 16, wo, 5, wo+2, 0, wo-2, 0);
	p.drawPolygon(cr);
	p.end();
#endif
	oldMark = currentCoor;
}

void Hruler::setItemPosition(double pos, double width)
{
	ItemPos = pos;
	ItemEndPos = pos+width;
	if (currDoc->guidesSettings.rulerMode)
	{
		ItemPos -= currDoc->currentPage->xOffset();
		ItemEndPos -= currDoc->currentPage->xOffset();
	}
}

void Hruler::UpdateTabList()
{
	struct PageItem::TabRecord tb;
	tb.tabPosition = TabValues[ActTab].tabPosition;
	tb.tabType = TabValues[ActTab].tabType;
	tb.tabFillChar =  TabValues[ActTab].tabFillChar;
	QValueList<PageItem::TabRecord>::Iterator it;
	int gg = static_cast<int>(TabValues.count());
	int g = gg;
	it = TabValues.at(ActTab);
	TabValues.remove(it);
	for (int yg = static_cast<int>(TabValues.count()-1); yg > -1; yg--)
	{
		if (tb.tabPosition < TabValues[yg].tabPosition)
			g = yg;
	}
	ActTab = g;
	if (gg == g)
	{
		TabValues.append(tb);
		ActTab = static_cast<int>(TabValues.count()-1);
	}
	else
	{
		it = TabValues.at(ActTab);
		TabValues.insert(it, tb);
	}
}

void Hruler::unitChange()
{
	double sc = currView->getScale();
	cor=1;
	int docUnitIndex=currDoc->unitIndex();
	switch (docUnitIndex)
	{
		case 0:
			if (sc > 1 && sc <= 4)
				cor = 2;
			if (sc > 4)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
	  		iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
			break;
		case 1:
			if (sc > 1)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
			break;
		case 2:
			iter = unitRulerGetIter1FromIndex(docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
			if (sc > 1 && sc <= 4)
			{
				cor = 2;
				iter /= cor;
				iter2 /= cor;
			}
			if (sc > 4)
			{
				cor = 4;
				iter /= cor;
				iter2 /= cor;
			}
			break;
		case 3:
			iter = unitRulerGetIter1FromIndex(docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
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
		case 4:
			if (sc > 1 && sc <= 4)
				cor = 1;
			if (sc > 4)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
			break;
		case 5:
			iter = unitRulerGetIter1FromIndex(docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
			if (sc > 1 && sc <= 4)
			{
				cor = 1;
				iter = 72.0/25.4*4.512;
				iter2 = 72.0/25.4*4.512*5.0;
			}
			if (sc > 4)
			{
				cor = 2;
				iter = 72.0/25.4*4.512/2.0;
				iter2 = 72.0/25.4*4.512;
			}
			break;			
		default:
			if (sc > 1 && sc <= 4)
				cor = 2;
			if (sc > 4)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(0) / cor;
	 		iter2 = unitRulerGetIter2FromIndex(0) / cor;
			break;
	}
}
