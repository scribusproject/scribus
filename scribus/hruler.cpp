/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QCursor>
#include <QMouseEvent>
// #include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPixmap>
#include <QPolygon>
#include <QRect>

#include "canvasgesture_rulermove.h"
#include "hruler.h"
#include "page.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "units.h"


#if QT_VERSION  > 0x030102
	#define SPLITHC Qt::SplitVCursor
#else
	#define SPLITHC Qt::SplitHCursor
#endif

#include "util_icon.h"


#ifdef Q_WS_MAC
    #define topline 1
#else
    #define topline 3
#endif

enum ruler_code 
{ 
	rc_none = 0, 
	rc_leftFrameDist = 1, 
	rc_rightFrameDist = 2,
	rc_indentFirst = 3,
	rc_leftMargin = 4,
	rc_tab = 5,
	rc_rightMargin = 6
};


Hruler::Hruler(ScribusView *pa, ScribusDoc *doc) : QWidget(pa)
{
	prefsManager=PrefsManager::instance();
	setBackgroundRole(QPalette::Window);
	setAutoFillBackground(true);
	QPalette palette;
	palette.setBrush(QPalette::Window, QColor(255, 255, 255));
	setPalette(palette);
	currDoc = doc;
	currView = pa;
	offs = 0;
	Markp = 0;
	oldMark = 0;
	ActCol = 1;
	Mpressed = false;
	ItemPosValid = false;
	drawMark = false;
	RulerCode = rc_none;
	setMouseTracking(true);
	rulerGesture = new RulerGesture(currView, RulerGesture::HORIZONTAL);
	unitChange();
}

void Hruler::mousePressEvent(QMouseEvent *m)
{
	if (currDoc->isLoading())
		return;
	Mpressed = true;
	MouseX = m->x();
	if (ItemPosValid)
	{
		RulerCode = rc_none;
		Markp = -1;
		double Pos = (ItemPos-offs+Extra+lineCorr)*Scaling;
		if ((static_cast<int>(Pos-1) < (m->x()+currDoc->guidesSettings.grabRad))
				   && (static_cast<int>(Pos-1) > (m->x()-currDoc->guidesSettings.grabRad)))
			RulerCode = rc_leftFrameDist;
		Pos = (ItemEndPos-offs-RExtra-lineCorr)*Scaling;
		if ((static_cast<int>(Pos+1) < (m->x()+currDoc->guidesSettings.grabRad))
				   && (static_cast<int>(Pos+1) > (m->x()-currDoc->guidesSettings.grabRad)))
			RulerCode = rc_rightFrameDist;
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
		{
			ActCol = 1;
			return;
		}
		Pos = (ItemPos-offs+First+Indent+(ColWidth+ColGap)*(ActCol-1)+Extra+lineCorr)*Scaling;
		fpo = QRect(static_cast<int>(Pos)-3, topline, 6, 6);
		if (fpo.contains(m->pos()))
		{
			RulerCode = rc_indentFirst;
			return;
		}
		Pos = (ItemPos-offs+Indent+(ColWidth+ColGap)*(ActCol-1)+Extra+lineCorr)*Scaling;
		fpo = QRect(static_cast<int>(Pos)-3, 9, 6, 6);
		if (fpo.contains(m->pos()))
		{
			RulerCode = rc_leftMargin;
			return;
		}
		Pos = (ItemPos-offs+RMargin+(ColWidth+ColGap)*(ActCol-1)+Extra+lineCorr)*Scaling;
		fpo = QRect(static_cast<int>(Pos)-5, 9, 8, 6);
		if (fpo.contains(m->pos()))
		{
			RulerCode = rc_rightMargin;
			return;
		}
		if (TabValues.count() != 0)
		{
			for (int yg = 0; yg < static_cast<int>(TabValues.count()); yg++)
			{
				Pos = (ItemPos-offs+TabValues[yg].tabPosition+(ColWidth+ColGap)*(ActCol-1)+Extra+lineCorr)*Scaling;
				fpo = QRect(static_cast<int>(Pos)-3, 7, 8, 8);
				if (fpo.contains(m->pos()))
				{
					RulerCode = rc_tab;
					ActTab = yg;
					break;
				}
			}
		}
		if ((RulerCode == rc_none) && (ActCol != 0) && (m->button() == Qt::LeftButton))
		{
			double Pos = (ItemPos-offs+Extra+lineCorr)*Scaling;
			int newY = m->x() - static_cast<int>(Pos);
			ParagraphStyle::TabRecord tb;
			tb.tabPosition = newY / Scaling;
			tb.tabType = 0;
			tb.tabFillChar = currDoc->toolSettings.tabFillChar[0];
			TabValues.prepend(tb);
			ActTab = 0;
			RulerCode = rc_tab;
			UpdateTabList();
			qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
			emit DocChanged(false);
		}
	}
	else
	{
		if (currDoc->guidesSettings.guidesShown)
		{
			qApp->changeOverrideCursor(QCursor(SPLITHC));
			currView->startGesture(rulerGesture);
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
			double ColWidth = (ItemEndPos - ItemPos - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
			switch (RulerCode)
			{
				case rc_leftFrameDist:
					currDoc->m_Selection->itemAt(0)->setTextToFrameDistLeft(Extra);
					emit DocChanged(false);
					break;
				case rc_rightFrameDist:
					currDoc->m_Selection->itemAt(0)->setTextToFrameDistRight(RExtra);
					emit DocChanged(false);
					break;
				case rc_indentFirst:
					currItem->changeCurrentStyle().setFirstIndent(First);
					emit DocChanged(false);
					break;
				case rc_leftMargin:
					currItem->changeCurrentStyle().setLeftMargin(Indent);
					currItem->changeCurrentStyle().setFirstIndent(First);
					emit DocChanged(false);
					break;
				case rc_rightMargin:
					currItem->changeCurrentStyle().setRightMargin(ColWidth - RMargin);
					emit DocChanged(false);
					break;
				case rc_tab:
					if (m->button() == Qt::RightButton)
					{
						TabValues[ActTab].tabType += 1;
						if (TabValues[ActTab].tabType > 4)
							TabValues[ActTab].tabType = 0;
					}
					currItem->changeCurrentStyle().setTabValues(TabValues);
					emit DocChanged(false);
					break;
				default:
					break;
			}
		}
		else
		{
			if (RulerCode == rc_tab)
			{
				TabValues.removeAt(ActTab);
				ActTab = 0;
				currItem->changeCurrentStyle().setTabValues(TabValues);
				emit DocChanged(false);
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			}
		}
		RulerCode = rc_none;
		currView->DrawNew();
		currDoc->m_Selection->itemAt(0)->emitAllToGUI();
	}
	else
	{
		if (Mpressed)
		{
			rulerGesture->mouseReleaseEvent(m);
			Mpressed = false;
		}
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
		if (RulerCode == rc_leftFrameDist || RulerCode == rc_rightFrameDist)
		{
			ColStart =static_cast<int>((ItemPos-offs+lineCorr)*Scaling);
			ColEnd = static_cast<int>((ItemEndPos-offs-lineCorr)*Scaling);
		}
		else
		{
			ColStart  = static_cast<int>((ItemPos-offs+(ColWidth+ColGap)*(ActCol-1)+Extra+lineCorr)*Scaling);
			ColEnd = static_cast<int>((ItemPos-offs+(ColWidth+ColGap)*(ActCol-1)+ColWidth+Extra+lineCorr)*Scaling);
		}
		if ((Mpressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > ColStart) && (m->x() < ColEnd))
		{
			qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
			double toplimit = ItemEndPos-ItemPos-2*lineCorr-Extra - (ColGap * (Cols - 1))-1;
			double toplimit2 = ItemEndPos-ItemPos-2*lineCorr-RExtra - (ColGap * (Cols - 1))-1;
			switch (RulerCode)
			{
				case rc_leftFrameDist:
					Extra -= (MouseX - m->x()) / Scaling;
					if (Extra < 0)
						Extra = 0;
					if (Extra > toplimit2)
						Extra = toplimit2;
					emit MarkerMoved(Extra, 0);
					repaint();
					break;
				case rc_rightFrameDist:
					RExtra += (MouseX - m->x()) / Scaling;
					if (RExtra < 0)
						RExtra = 0;
					if (RExtra > toplimit)
						RExtra = toplimit;
					emit MarkerMoved(RExtra, 0);
					repaint();
					break;
				case rc_indentFirst:
					First -= (MouseX - m->x()) / Scaling;
					if ((ItemPos-offs+(ColWidth+ColGap)*ActCol+First+Indent)*Scaling < (ItemPos-offs+(ColWidth+ColGap)*ActCol)*Scaling)
						First += (MouseX - m->x()) / Scaling;
					if (First+Indent > ColWidth)
						First  = ColWidth-Indent;
					emit MarkerMoved(First, 0);
					repaint();
					break;
				case rc_leftMargin:
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
				case rc_rightMargin:
					RMargin -= (MouseX - m->x()) / Scaling;
					if (RMargin < 0)
						RMargin = 0;
					if (RMargin > ColWidth-1)
						RMargin  = ColWidth-1;
					emit MarkerMoved(RMargin, 0);
					repaint();
					break;
				case rc_tab:
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
			if (RulerCode != rc_none)
			{
				QPoint py = currView->viewport()->mapFromGlobal(m->globalPos());
/*				QPainter p;
				p.begin(currView->viewport());
				p.setCompositionMode(QPainter::CompositionMode_Xor);
				p.setPen(QPen(Qt::white, 1, Qt::DotLine, Qt::FlatCap, Qt::MiterJoin));
				QPoint out = currView->contentsToViewport(QPoint(0, qRound(currDoc->currentPage()->yOffset() * Scaling)));
				p.drawLine(Markp, out.y(), Markp, out.y()+qRound(currDoc->currentPage()->height() * Scaling));
				p.drawLine(py.x(), out.y(), py.x(), out.y()+qRound(currDoc->currentPage()->height() * Scaling));
				p.end(); */
				Markp = py.x();
			}
			return;
		}
		if ((!Mpressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > ColStart) && (m->x() < ColEnd))
		{
			qApp->changeOverrideCursor(QCursor(loadIcon("tab.png"), 3));
			double Pos = (ItemPos-offs+Extra+lineCorr)*Scaling;
			if ((static_cast<int>(Pos) < (m->x()+currDoc->guidesSettings.grabRad)) && (static_cast<int>(Pos) > (m->x()-currDoc->guidesSettings.grabRad)))
				qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
			Pos = (ItemEndPos-offs-RExtra-lineCorr)*Scaling;
			if ((static_cast<int>(Pos) < (m->x()+currDoc->guidesSettings.grabRad)) && (static_cast<int>(Pos) > (m->x()-currDoc->guidesSettings.grabRad)))
				qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
			QRect fpo;
			double ColWidth = (ItemEndPos - ItemPos - (ColGap * (Cols - 1)) - Extra - RExtra - 2*lineCorr) / Cols;
//			if (currDoc->currentParaStyle > 4)
			{
				for (int CurrCol = 0; CurrCol < Cols; ++CurrCol)
				{
					Pos = (ItemPos-offs+First+Indent+(ColWidth+ColGap)*CurrCol+Extra+lineCorr)*Scaling;
					fpo = QRect(static_cast<int>(Pos)-3, topline, 6, 6);
					if (fpo.contains(m->pos()))
					{
						qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
						break;
					}
					Pos = (ItemPos-offs+Indent+(ColWidth+ColGap)*CurrCol+Extra+lineCorr)*Scaling;
					fpo = QRect(static_cast<int>(Pos)-3, 9, 6, 6);
					if (fpo.contains(m->pos()))
					{
						qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
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
							qApp->changeOverrideCursor(QCursor(Qt::SizeHorCursor));
							break;
						}
					}
				}
			}
			emit MarkerMoved((m->x() - static_cast<int>((ItemPos-offs+Extra+lineCorr)*Scaling)) / Scaling, 0);
			return;
		}
		if ((Mpressed) && (RulerCode == rc_tab) && ((m->y() > height()) || (m->y() < 0)))
		{
			qApp->changeOverrideCursor(QCursor(loadIcon("DelPoint.png"), 1, 1));
			return;
		}
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	else
	{
		if (Mpressed)
		{
			rulerGesture->mouseMoveEvent(m);
		}
	}
}

void Hruler::paintEvent(QPaintEvent *e)
{
	if (currDoc->isLoading())
		return;
	QString tx = "";
	double xl, frac;
	double sc = currView->scale();
	Scaling = sc;
	QFont ff = font();
	ff.setPointSize(8);
	setFont(ff);
	QPainter p;
	p.begin(this);
	p.setClipRect(e->rect());
	p.drawLine(0, 16, width(), 16);
	p.setBrush(Qt::black);
	p.setPen(Qt::black);
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
			case SC_MM:
				tx = QString::number(markC * iter2 / (iter2 / 100) / cor);
				break;
			case SC_IN:
				xl = (markC * iter2 / iter2) / cor;
				tx = QString::number(static_cast<int>(xl));
				frac = fabs(xl - static_cast<int>(xl));
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
			case SC_P:
			case SC_C:
				tx = QString::number(markC * iter2 / (iter2 / 5) / cor);
				break;
			case SC_CM:
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
			p.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			for (xl = Pos; xl < EndPos; xl += iter)
				p.drawLine(qRound(xl*sc), 10, qRound(xl*sc), 16);
			for (xl = Pos; xl < EndPos; xl += iter2)
			{
				p.drawLine(qRound(xl*sc), topline, qRound(xl*sc), 16);
				switch (currDoc->unitIndex())
				{
					case SC_IN:
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
					case SC_P:
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
					case SC_CM:
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
					case SC_C:
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
			p.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			p.drawLine(qRound(Pos*sc), topline, qRound(Pos*sc), 15);
			if (CurrCol == 0)
			{
				p.drawLine(qRound(Pos*sc), 15, qRound((Pos+4/sc)*sc), 15);
				p.drawLine(qRound(Pos*sc), topline, qRound((Pos+4/sc)*sc), topline);
			}
//			if (currDoc->currentParaStyle > 4)
			{
				p.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				double fpos = Pos+First+Indent;
				QPolygon cr;
				cr.setPoints(3, qRound(fpos*sc), 9, qRound((fpos+3/sc)*sc), topline, qRound((fpos-3/sc)*sc), topline);
				p.drawPolygon(cr);
				QPolygon cr2;
				cr2.setPoints(3, qRound((Pos+Indent)*sc), 9, qRound((Pos+Indent+3/sc)*sc), 15, qRound((Pos+Indent-3/sc)*sc), 15);
				p.drawPolygon(cr2);
				QPolygon cr3;
				cr3.setPoints(3, qRound((Pos+RMargin)*sc), topline, qRound((Pos+RMargin)*sc), 15, qRound((Pos+RMargin-3/sc)*sc), 9);
				p.drawPolygon(cr3);
			}
			if (TabValues.count() != 0)
			{
				p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
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
			p.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			p.drawLine(qRound(EndPos*sc), topline, qRound(EndPos*sc), 15);
			if (CurrCol == Cols-1)
			{
				p.drawLine(qRound(EndPos*sc), 15, qRound((EndPos-4/sc)*sc), 15);
				p.drawLine(qRound(EndPos*sc), topline, qRound((EndPos-4/sc)*sc), topline);
			}
		}
		p.restore();
	}
	if (drawMark)
	{
		QPolygon cr;
#ifdef OPTION_SMOOTH_MARKERS
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
	
			p.setPen(Qt::red);
			p.setBrush(Qt::red);
			cr.setPoints(3, 2*SCALE, 16*SCALE, 4*SCALE, 0, 0, 0);
			p.drawPolygon(cr);
			p.end();
		}
		// draw pixmap
		p.resetMatrix();
		p.translate(-currView->contentsX(), 0);
		p.scale(1.0/SCALE, 1.0/(SCALE+1));
		p.drawPixmap((where-2)*SCALE, 1, pix);
		p.end();
		// restore marks
		p.begin(this);
		p.setBrush(Qt::black);
		p.setPen(Qt::black);
		p.setFont(font());
		double sc = currView->getScale();
		double cc = width() / sc;
		double firstMark = ceil(offs / iter) * iter - offs;
		while (firstMark < cc)
		{
			p.drawLine(qRound(firstMark * sc), 10, qRound(firstMark * sc), 16);
			firstMark += iter;
		}
#else
		// draw slim marker
		p.resetMatrix();
		p.translate(-currView->contentsX(), 0);
		p.setPen(Qt::red);
		p.setBrush(Qt::red);
		cr.setPoints(5,  whereToDraw, 5, whereToDraw, 16, whereToDraw, 5, whereToDraw+2, 0, whereToDraw-2, 0);
		p.drawPolygon(cr);
#endif
	p.end();
	}
}


void Hruler::drawNumber(QString txt, int x, int y0, QPainter & p)
{
	const int y = y0 - 3 + topline;
#ifndef Q_WS_MAC
	p.drawText(x,y,txt);
#else
	static const int SCALE = 16;
	QFontMetrics fm = p.fontMetrics();
	QRect bbox = fm.boundingRect(txt);
	static QPixmap pix;
	if (pix.width() < bbox.width()*SCALE || pix.height() < bbox.height()*SCALE)
		pix = QPixmap(bbox.width()*SCALE, bbox.height()*SCALE);
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

void Hruler::Draw(int where)
{
	// erase old marker
	int currentCoor = where - currView->contentsX();
	whereToDraw = where;
	drawMark = true;
	repaint(oldMark-3, 0, 7, 17);
//	drawMark = false;
	oldMark = currentCoor;
}

void Hruler::setItem(PageItem * item)
{
	currItem = item;
	ItemPos = item->xPos();
	ItemEndPos = ItemPos + item->width();
	if (currDoc->guidesSettings.rulerMode)
	{
		ItemPos -= currDoc->currentPage()->xOffset();
		ItemEndPos -= currDoc->currentPage()->xOffset();
	}
}

void Hruler::UpdateTabList()
{
	ParagraphStyle::TabRecord tb;
	tb.tabPosition = TabValues[ActTab].tabPosition;
	tb.tabType = TabValues[ActTab].tabType;
	tb.tabFillChar =  TabValues[ActTab].tabFillChar;
	int gg = static_cast<int>(TabValues.count());
	int g = gg;
	TabValues.removeAt(ActTab);
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
		TabValues.insert(ActTab, tb);
	}
}

void Hruler::unitChange()
{
	double sc = currView->scale();
	cor=1;
	int docUnitIndex=currDoc->unitIndex();
	switch (docUnitIndex)
	{
		case SC_PT:
			if (sc > 1 && sc <= 4)
				cor = 2;
			if (sc > 4)
				cor = 10;
			if (sc < 0.3)
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) * 3;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) * 3;
			}
			else if (sc < 0.5)
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) * 2;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) * 2;
			}
			else
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
	  		}
			break;
		case SC_MM:
			if (sc > 1)
				cor = 10;
			iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
			break;
		case SC_IN:
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
			if (sc < 0.25)
			{
				cor = 0.5;
				iter = 72.0*16.0;
				iter2 = 72.0*2.0;
			}
			break;
		case SC_P:
			iter = unitRulerGetIter1FromIndex(docUnitIndex);
			iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
			qDebug() << sc;
			if (sc >= 1 && sc <= 4)
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
			if (sc < 0.3)
			{
				cor = 0.25;
				iter = 12.0*4;
				iter2 = 60.0*4;
			}
			else
			if (sc < 1)
			{
				cor = 1;
				iter = 12.0;
				iter2 = 60.0;
			}
			break;
		case SC_CM:
			if (sc > 1 && sc <= 4)
				cor = 1;
			if (sc > 4)
				cor = 10;
			if (sc < 0.3)
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) * 4;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) * 4;
			}
			else if (sc < 0.6)
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) * 3;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) * 3;
			}
			else
			{
				iter = unitRulerGetIter1FromIndex(docUnitIndex) / cor;
	  			iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / cor;
	  		}
			break;
		case SC_C:
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
