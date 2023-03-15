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

#include <cmath>

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
#include "scpage.h"

#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "units.h"

#include "iconmanager.h"

#ifdef Q_OS_MACOS
constexpr int topline = 1;
#else
constexpr int topline = 3;
#endif
constexpr int bottomline = 15;
constexpr int rulerheight = (bottomline - topline);
constexpr int midline = (topline + rulerheight / 2);
constexpr int tabline = 7;

Hruler::Hruler(ScribusView *pa, ScribusDoc *doc) : QWidget(pa),
	m_doc(doc),
	m_view(pa)
{
	setBackgroundRole(QPalette::Window);
	//setAutoFillBackground(true);
	setMouseTracking(true);
	rulerGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
	unitChange();
}

double Hruler::textBase() const
{
	return m_itemPos + m_lineCorr + m_distLeft;
}

double Hruler::textWidth() const
{
	return m_itemEndPos - m_itemPos - 2 * m_lineCorr - m_distLeft - m_distRight;
}


double Hruler::textPosToCanvas(double x) const
{
	return x + textBase();
}

int Hruler::textPosToLocal(double x) const
{
	return qRound(textPosToCanvas(x) * m_scaling)- m_view->contentsX();
}

double Hruler::localToTextPos(int x) const
{
	return ((x + m_view->contentsX()) / m_scaling - textBase());
}

void Hruler::shift(double pos)
{
	m_offset = pos;
}

void Hruler::shiftRel(double dist)
{
	m_offset += dist;
}


int Hruler::findRulerHandle(QPoint mp, int grabRadius)
{
	int mx = mp.x();
	QRect fpo;
	
	int result = rc_none;
	
	int pos = textPosToLocal(0);
	if (pos - 1 < (mx + grabRadius) && pos - 1 > (mx - grabRadius))
		result = rc_leftFrameDist;
	
	pos = textPosToLocal(textWidth());
	if (pos + 1 < (mx + grabRadius) && pos + 1 > (mx - grabRadius))
		result = rc_rightFrameDist;
	
	double colWidth = (textWidth() - m_colGap * (m_cols - 1.0)) / m_cols;
	
	m_currCol = 0;
	for (int currCol = 0; currCol < m_cols; ++currCol)
	{
		pos = textPosToLocal((colWidth + m_colGap) * currCol);
		fpo = QRect(pos, topline, static_cast<int>(colWidth * m_scaling), rulerheight);
		if (fpo.contains(mp))
		{
			m_currCol = currCol + 1;
			break;
		}
	}
	if (m_currCol == 0)
	{
		m_currCol = 1;
		return result;
	}
	
	pos = textPosToLocal(m_firstIndent + m_leftMargin + (colWidth + m_colGap) * (m_currCol - 1.0));
	fpo = QRect(pos - 1, topline, grabRadius + 1, rulerheight / 2);
	if (fpo.contains(mp))
	{
		return rc_indentFirst;
	}
	pos = textPosToLocal(m_leftMargin + (colWidth + m_colGap) * (m_currCol - 1.0));
	fpo = QRect(pos - 1, midline, grabRadius + 1, rulerheight / 2);
	if (fpo.contains(mp))
	{
		return rc_leftMargin;
	}
	pos = textPosToLocal(m_rightMargin + (colWidth + m_colGap) * (m_currCol - 1.0));
	fpo = QRect(pos - grabRadius, midline, grabRadius, rulerheight / 2);
	if (fpo.contains(mp))
	{
		return rc_rightMargin;
	}
	if (m_tabValues.count() != 0)
	{
		for (int yg = 0; yg < m_tabValues.count(); yg++)
		{
			pos = textPosToLocal(m_tabValues[yg].tabPosition + (colWidth + m_colGap) * (m_currCol - 1.0));
			fpo = QRect(pos - grabRadius, tabline, 2 * grabRadius, rulerheight / 2 + 2);
			if (fpo.contains(mp))
			{
				result = rc_tab;
				m_currTab = yg;
				break;
			}
		}
	}
	return result;
}


void Hruler::mousePressEvent(QMouseEvent *m)
{
	if (m_doc->isLoading())
		return;
	m_mousePressed = true;
	m_mouseX = m->x();
	if (m_textEditMode)
	{
		m_rulerCode = findRulerHandle(m->pos(), m_doc->guidesPrefs().grabRadius);
		
		double tabPos = localToTextPos(m->x());
		bool isInTextRange = (tabPos >= 0) && (tabPos <= textWidth());
		if (isInTextRange && (m_rulerCode == rc_none) && (m_currCol != 0) && (m->button() == Qt::LeftButton))
		{
			const QString& textTabFillChar = m_doc->itemToolPrefs().textTabFillChar;
			ParagraphStyle::TabRecord tb;
			tb.tabPosition = tabPos;
			tb.tabType = 0;
			tb.tabFillChar = !textTabFillChar.isEmpty() ? textTabFillChar[0] : QChar();
			m_tabValues.prepend(tb);
			m_currTab = 0;
			m_rulerCode = rc_tab;
			updateTabList();
			QApplication::setOverrideCursor(QCursor(Qt::SizeHorCursor));
			emit DocChanged(false);
		}
	}
	else
	{
		if (m_doc->guidesPrefs().guidesShown)
		{
			QApplication::setOverrideCursor(QCursor(Qt::SplitVCursor));
			m_view->startGesture(rulerGesture);
			m_view->registerMousePress(m->globalPos());
		}
	}
}

void Hruler::mouseReleaseEvent(QMouseEvent *m)
{
	if (m_doc->isLoading())
	{
		m_mousePressed = false;
		return;
	}
	if (m_textEditMode && m_currItem)
	{
		if ((m->y() < height()) && (m->y() > 0))
		{
			bool mustApplyStyle = false;
			ParagraphStyle paraStyle;
			double colWidth = (textWidth() - m_colGap * (m_cols - 1)) / m_cols;
			switch (m_rulerCode)
			{
				case rc_leftFrameDist:
					m_doc->m_Selection->itemAt(0)->setTextToFrameDistLeft(m_distLeft);
					emit DocChanged(false);
					break;
				case rc_rightFrameDist:
					m_doc->m_Selection->itemAt(0)->setTextToFrameDistRight(m_distRight);
					emit DocChanged(false);
					break;
				case rc_indentFirst:
					paraStyle.setFirstIndent(m_firstIndent);
					mustApplyStyle = true;
					emit DocChanged(false);
					break;
				case rc_leftMargin:
					paraStyle.setLeftMargin(m_leftMargin);
					paraStyle.setFirstIndent(m_firstIndent);
					mustApplyStyle = true;
					emit DocChanged(false);
					break;
				case rc_rightMargin:
					paraStyle.setRightMargin(colWidth - m_rightMargin);
					mustApplyStyle = true;
					emit DocChanged(false);
					break;
				case rc_tab:
					if (m->button() == Qt::RightButton)
					{
						m_tabValues[m_currTab].tabType += 1;
						if (m_tabValues[m_currTab].tabType > 4)
							m_tabValues[m_currTab].tabType = 0;
					}
					paraStyle.setTabValues(m_tabValues);
					mustApplyStyle = true;
					emit DocChanged(false);
					break;
				default:
					break;
			}
			if (mustApplyStyle)
			{
				Selection tempSelection(this, false);
				tempSelection.addItem(m_currItem);
				m_doc->itemSelection_ApplyParagraphStyle(paraStyle, &tempSelection);
			}
			else
			{
				m_currItem->update();
			}
		}
		else
		{
			if (m_rulerCode == rc_tab)
			{
				m_tabValues.removeAt(m_currTab);
				m_currTab = 0;
				ParagraphStyle paraStyle;
				paraStyle.setTabValues(m_tabValues);
				Selection tempSelection(this, false);
				tempSelection.addItem(m_currItem);
				m_doc->itemSelection_ApplyParagraphStyle(paraStyle, &tempSelection);
				emit DocChanged(false);
			}
		}
		m_rulerCode = rc_none;
		m_view->DrawNew();
		m_doc->m_Selection->itemAt(0)->emitAllToGUI();
	}
	else if (m_mousePressed && rulerGesture->isActive())
		rulerGesture->mouseReleaseEvent(m);
	m_mousePressed = false;
	QApplication::restoreOverrideCursor();
}

void Hruler::enterEvent(QEvent* e)
{
	if (m_textEditMode)
		QApplication::changeOverrideCursor(IconManager::instance().loadCursor("tab.png", 3));
}

void Hruler::leaveEvent(QEvent *m)
{
	emit MarkerMoved(0, -1);
	QApplication::restoreOverrideCursor();
	m_view->m_canvasMode->setModeCursor();
}

void Hruler::mouseMoveEvent(QMouseEvent *m)
{
	if (m_doc->isLoading())
		return;
	if (m_textEditMode)
	{
		int colEnd, colStart;
		double colWidth = (textWidth() - m_colGap * (m_cols - 1.0)) / m_cols;
		double oldInd;
		if (m_rulerCode == rc_leftFrameDist || m_rulerCode == rc_rightFrameDist)
		{
			colStart = 0; //textPosToLocal(0);
			colEnd   = width(); //textPosToLocal(textWidth());
		}
		else
		{
			colStart = textPosToLocal((colWidth + m_colGap) * (m_currCol - 1.0));
			colEnd   = textPosToLocal((colWidth + m_colGap) * (m_currCol - 1.0) + colWidth);
		}
		if (m_mousePressed && (m->y() < height()) && (m->y() > 0) && (m->x() > colStart - m_doc->guidesPrefs().grabRadius) && (m->x() < colEnd + m_doc->guidesPrefs().grabRadius))
		{
			QApplication::changeOverrideCursor(QCursor(Qt::SizeHorCursor));
			double toplimit = textWidth() + m_distRight - (m_colGap * (m_cols - 1.0)) - 1.0;
			double toplimit2 = textWidth() + m_distLeft - (m_colGap * (m_cols - 1.0)) - 1.0;
			switch (m_rulerCode)
			{
				case rc_leftFrameDist:
					m_distLeft -= (m_mouseX - m->x()) / m_scaling;
					if (m_distLeft < 0)
						m_distLeft = 0;
					if (m_distLeft > toplimit2)
						m_distLeft = toplimit2;
					emit MarkerMoved(m_currItem->xPos(), textBase() - m_currItem->xPos());
					repaint();
					break;
				case rc_rightFrameDist:
					m_distRight += (m_mouseX - m->x()) / m_scaling;
					if (m_distRight < 0)
						m_distRight = 0;
					if (m_distRight > toplimit)
						m_distRight = toplimit;
					emit MarkerMoved(textBase(), toplimit - m_distRight);
					repaint();
					break;
				case rc_indentFirst:
					m_firstIndent -= (m_mouseX - m->x()) / m_scaling;
					if (m_firstIndent + m_leftMargin < 0)
						m_firstIndent = -m_leftMargin;
					if (m_firstIndent + m_leftMargin > colWidth)
						m_firstIndent  = colWidth - m_leftMargin;
					emit MarkerMoved(textBase(), m_firstIndent + m_leftMargin);
					repaint();
					break;
				case rc_leftMargin:
					oldInd = m_leftMargin + m_firstIndent;
					m_leftMargin -= (m_mouseX - m->x()) / m_scaling;
					if (m_leftMargin < 0)
						m_leftMargin = 0;
					if (m_leftMargin > colWidth - 1)
						m_leftMargin  = colWidth - 1;
					m_firstIndent = oldInd - m_leftMargin;
					emit MarkerMoved(textBase(), m_leftMargin);
					repaint();
					break;
				case rc_rightMargin:
					m_rightMargin -= (m_mouseX - m->x()) / m_scaling;
					if (m_rightMargin < 0)
						m_rightMargin = 0;
					if (m_rightMargin > colWidth - 1)
						m_rightMargin  = colWidth - 1;
					emit MarkerMoved(textBase(), m_rightMargin);
					repaint();
					break;
				case rc_tab:
					m_tabValues[m_currTab].tabPosition -= (m_mouseX - m->x()) / m_scaling;
					if (m_tabValues[m_currTab].tabPosition < 0)
						m_tabValues[m_currTab].tabPosition = 0;
					if (m_tabValues[m_currTab].tabPosition > colWidth - 1)
						m_tabValues[m_currTab].tabPosition  = colWidth - 1;
					emit MarkerMoved(textBase(), m_tabValues[m_currTab].tabPosition);
					updateTabList();
					repaint();
					break;
				default:
					break;
			}
			m_mouseX = m->x();
			return;
		}
		if ((!m_mousePressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > colStart - 2 * m_doc->guidesPrefs().grabRadius) && (m->x() < colEnd + 2 * m_doc->guidesPrefs().grabRadius))
		{
			setCursor(IconManager::instance().loadCursor("tab.png", 3));
			switch (findRulerHandle(m->pos(), m_doc->guidesPrefs().grabRadius))
			{
				case rc_leftFrameDist:
					setCursor(QCursor(Qt::SplitHCursor));
					break;
				case rc_rightFrameDist:
					setCursor(QCursor(Qt::SplitHCursor));
					break;
				case rc_indentFirst:
					setCursor(QCursor(Qt::SizeHorCursor));
					break;
				case rc_leftMargin:
					setCursor(QCursor(Qt::SizeHorCursor));
					break;
				case rc_rightMargin:
					setCursor(QCursor(Qt::SizeHorCursor));
					break;
				case rc_tab:
					setCursor(QCursor(Qt::SizeHorCursor));
					break;
				default:
					break;
			}
			draw(m->x());
			double marker = localToTextPos(m->x());
			emit MarkerMoved(textBase(), marker);
			return;
		}
		if (m_mousePressed && (m_rulerCode == rc_tab) && ((m->y() > height()) || (m->y() < 0)))
		{
			setCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));
			return;
		}
		setCursor(QCursor(Qt::ArrowCursor));
	}
	else
	{
		if (m_mousePressed && rulerGesture->isActive())
			rulerGesture->mouseMoveEvent(m);
		else
			setCursor(QCursor(Qt::ArrowCursor));
	}
}

void Hruler::paintEvent(QPaintEvent *e)
{
	if (m_doc->isLoading())
		return;
	double sc = m_view->scale();
	m_scaling = sc;
	QFont ff = font();
	ff.setPointSize(6);
	setFont(ff);
	QPainter p;
	p.begin(this);
	p.setClipRect(e->rect());
	p.setFont(font());
	
	drawMarks(p);

	if (m_textEditMode)
	{
		int rectX1 = textPosToLocal(m_distLeft);
		int rectX2 = textPosToLocal(m_itemEndPos - m_itemPos - m_distRight);
		const QPalette& palette = this->palette();
		const QColor& textColor = palette.color(QPalette::Text);
		p.fillRect(QRect(QPoint(rectX1, 1), QPoint(rectX2, 15)), palette.window());
		p.drawLine(rectX1, 16, rectX2, 16);
		p.save();
		p.setRenderHints(QPainter::Antialiasing, true);
		if (m_reverse)
		{
			p.translate( textPosToLocal(0), 0);
			p.scale(-1, 1);
			p.translate( textPosToLocal(m_distLeft) - textPosToLocal(m_itemEndPos - m_itemPos - m_distRight), 0);
			p.translate(-textPosToLocal(0), 0);
		}
		for (int currCol = 0; currCol < m_cols; ++currCol)
		{
			double colWidth = (textWidth() - m_colGap * (m_cols - 1.0)) / m_cols;
			double pos = (colWidth + m_colGap) * currCol;
			double endPos = pos + colWidth;
			drawTextMarks(pos, endPos, p);
			
			p.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			int xPos = textPosToLocal(pos);
			p.drawLine(xPos, topline, xPos, bottomline);
			if (currCol == 0)
			{
				p.drawLine(xPos, 15, (xPos + 4), 15);
				p.drawLine(xPos, topline, (xPos + 4), topline);
			}
			
			p.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			
			xPos = textPosToLocal(pos + m_firstIndent + m_leftMargin);
			QPolygon cr;
			cr.setPoints(3, xPos, midline, xPos + 3, topline, xPos - 3, topline);
			p.drawPolygon(cr);

			xPos = textPosToLocal(pos + m_leftMargin);
			QPolygon cr2;
			cr2.setPoints(3, xPos, midline, xPos + 3, bottomline, xPos - 3, bottomline);
			p.drawPolygon(cr2);
			
			xPos = textPosToLocal(pos + m_rightMargin);
			QPolygon cr3;
			cr3.setPoints(3, xPos, topline, xPos, bottomline, xPos - 3, midline);
			p.drawPolygon(cr3);

			if (!m_tabValues.isEmpty())
			{
				p.setPen(QPen(textColor, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				for (int yg = 0; yg < m_tabValues.count(); ++yg)
				{
					xPos = textPosToLocal(pos + m_tabValues[yg].tabPosition);
					if (pos + m_tabValues[yg].tabPosition >= endPos)
						continue;
					switch (m_tabValues[yg].tabType)
					{
						case 0:
							if (m_reverse)
							{
								p.save();
								p.translate(pos + m_tabValues[yg].tabPosition, 0);
								p.scale(-1, 1);
								p.drawLine(0, tabline, 0, bottomline);
								p.drawLine(0, bottomline, 8, bottomline);
								p.restore();
							}
							else
							{
								p.drawLine(xPos, tabline, xPos, bottomline);
								p.drawLine(xPos, bottomline, xPos + 8, bottomline);
							}
							break;
						case 1:
							if (m_reverse)
							{
								p.save();
								p.translate(pos + m_tabValues[yg].tabPosition, 0);
								p.scale(-1, 1);
								p.drawLine(0, tabline, 0, bottomline);
								p.drawLine(0, bottomline, -8, bottomline);
								p.restore();
							}
							else
							{
								p.drawLine(xPos, tabline, xPos, bottomline);
								p.drawLine(xPos, bottomline, xPos - 8, bottomline);
							}
							break;
						case 2:
						case 3:
							p.drawLine(xPos, tabline, xPos, bottomline);
							p.drawLine(xPos - 4, bottomline, xPos + 4, bottomline);
							p.drawLine(xPos + 3, bottomline - 3, xPos + 2, bottomline - 3);
							break;
						case 4:
							p.drawLine(xPos, tabline, xPos, bottomline);
							p.drawLine(xPos - 4, bottomline, xPos + 4, bottomline);
							break;
						default:
							break;
					}
				}
			}
			
			p.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			xPos = textPosToLocal(endPos);
			p.drawLine(xPos, topline, xPos, bottomline);
			if (currCol == m_cols - 1)
			{
				p.drawLine(xPos, bottomline, xPos - 4 , bottomline);
				p.drawLine(xPos, topline, xPos - 4, topline);
			}
		}
		p.restore();
	}
	if (m_drawMark && !m_mousePressed)
	{
		drawMarker(p);
	}
	p.end();
}


void Hruler::drawMarker(QPainter& p) const
{
	// draw slim marker
	QPolygon cr;
	p.resetTransform();
	p.translate(-m_view->contentsX(), 0);
	p.setPen(Qt::red);
	p.setBrush(Qt::red);
	cr.setPoints(5,  m_whereToDraw, 5, m_whereToDraw, 16, m_whereToDraw, 5, m_whereToDraw + 2, 0, m_whereToDraw - 2, 0);
	p.drawPolygon(cr);
}


void Hruler::drawMarks(QPainter& p) const
{
	const QPalette& palette = this->palette();
	const QColor& textColor = palette.color(QPalette::Text);
	p.setBrush(textColor);
	p.setPen(textColor);

	double sc = m_scaling;
	double cc = width() / sc;
	double firstMark = ceil(m_offset / m_iter) * m_iter - m_offset;
	while (firstMark < cc)
	{
		p.drawLine(qRound(firstMark * sc), 13, qRound(firstMark * sc), 16);
		firstMark += m_iter;
	}
	firstMark = ceil(m_offset / m_iter2) * m_iter2 - m_offset;
	int markC = static_cast<int>(ceil(m_offset / m_iter2));

	QString tx;
	double xl, frac;
	while (firstMark < cc)
	{
		p.drawLine(qRound(firstMark * sc), topline + 5, qRound(firstMark * sc), 16);
		switch (m_doc->unitIndex())
		{
			case SC_MM:
				tx = QString::number(markC * m_iter2 / (m_iter2 / 100) / m_cor);
				break;
			case SC_IN:
				xl = (markC * m_iter2 / m_iter2) / m_cor;
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
				break;
			case SC_P:
			case SC_C:
				tx = QString::number(markC * m_iter2 / (m_iter2 / 5) / m_cor);
				break;
			case SC_CM:
				tx = QString::number(markC * m_iter2 / m_iter2 / m_cor);
				break;
			default:
				tx = QString::number(markC * m_iter2);
				break;
		}
		drawNumber(tx, qRound(firstMark * sc) + 2, 9, p);
		//p.drawText(qRound(firstMark * sc) + 2, 9, tx);
		firstMark += m_iter2;
		markC++;
	}
}	

void Hruler::drawTextMarks(double pos, double endPos, QPainter& p) const
{
	double xl;
	
	p.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p.setBrush(Qt::blue);
	for (xl = pos; xl < endPos; xl += m_iter)
	{
		int xli = textPosToLocal(xl);
		p.drawLine(xli, 10, xli, 16);
	}
	for (xl = pos; xl < endPos; xl += m_iter2)
	{
		int xli = textPosToLocal(xl);
		p.drawLine(xli, topline, xli, 16);
		switch (m_doc->unitIndex())
		{
			case SC_IN:
				{
					QString tx;
					int num1 = static_cast<int>((xl - pos) / m_iter2 / m_cor);
					if (num1 != 0)
						tx = QString::number(num1);
					double frac = (xl / m_iter2 / m_cor) - num1;
					if ((frac > 0.24) && (frac < 0.26))
						tx += QChar(0xBC);
					if ((frac > 0.49) && (frac < 0.51))
						tx += QChar(0xBD);
					if ((frac > 0.74) && (frac < 0.76))
						tx += QChar(0xBE);
					if (m_reverse)
					{
						p.save();
						p.translate(xli - 2.0, 0);
						p.scale(-1, 1);
						drawNumber(tx, 0, 17, p);
						p.restore();
					}
					else
						drawNumber(tx, xli + 2, 9, p);
					break;
				}
			case SC_P:
				if (m_reverse)
				{
					p.save();
					p.translate(xli - 2.0, 0);
					p.scale(-1, 1);
					drawNumber(QString::number((xl - pos) / m_iter / m_cor), 0, 17, p);
					p.restore();
				}
				else
					drawNumber(QString::number((xl - pos) / m_iter / m_cor), xli + 2, 9, p);
				break;
			case SC_CM:
				if (m_reverse)
				{
					p.save();
					p.translate(xli - 2.0, 0);
					p.scale(-1, 1);
					drawNumber(QString::number((xl - pos) / m_iter / 10 / m_cor), 0, 9, p);
					p.restore();
				}
				else
					drawNumber(QString::number((xl - pos) / m_iter / 10 / m_cor), xli + 2, 9, p);
				break;
			case SC_C:
				if (m_reverse)
				{
					p.save();
					p.translate(xli - 2.0, 0);
					p.scale(-1, 1);
					drawNumber(QString::number((xl - pos) / m_iter  / m_cor), 0, 9, p);
					p.restore();
				}
				else
					drawNumber(QString::number((xl - pos) / m_iter  / m_cor), xli + 2, 9, p);
				break;
			default:
				if (m_reverse)
				{
					p.save();
					p.translate(xli - 2.0, 0);
					p.scale(-1, 1);
					drawNumber(QString::number((xl - pos) / m_iter * 10 / m_cor), 0, 9, p);
					p.restore();
				}
				else
					drawNumber(QString::number((xl - pos) / m_iter * 10 / m_cor), xli + 2, 9, p);
				break;
		}
	}
}

void Hruler::drawNumber(const QString& txt, int x, int y0, QPainter & p) const
{
	const int y = y0 - 3 + topline;
	p.drawText(x,y,txt);
}

double Hruler::ruleSpacing() const
{
	return m_iter;
}

void Hruler::draw(int where)
{
	// erase old marker
	int currentCoor = where - m_view->contentsX();
	m_whereToDraw = where;
	m_drawMark = true;
	repaint(m_oldMark - 3, 0, 7, 17);
	//	m_drawMark = false;
	m_oldMark = currentCoor;
}

void Hruler::setItem(PageItem * item)
{
	m_currItem = item;
	QTransform mm = m_currItem->getTransform();
	QPointF itPos = mm.map(QPointF(0, m_currItem->yPos()));
	m_itemScale = mm.m11();
	m_itemPos = itPos.x();
	m_itemEndPos = m_itemPos + item->width() * m_itemScale;
	/*if (m_doc->guidesPrefs().rulerMode)
	{
		m_itemPos -= m_doc->currentPage()->xOffset();
		m_itemEndPos -= m_doc->currentPage()->xOffset();
	}*/
	
	if ((item->lineColor() != CommonStrings::None) || (!item->strokePattern().isEmpty()))
		m_lineCorr = item->lineWidth() / 2.0;
	else
		m_lineCorr = 0;
	m_colGap = item->m_columnGap;
	m_cols = item->m_columns;
	m_distLeft  = item->textToFrameDistLeft();
	m_distRight = item->textToFrameDistRight();

	const ParagraphStyle& currentStyle = item->currentStyle();
	m_firstIndent = currentStyle.firstIndent();
	m_leftMargin = currentStyle.leftMargin();
	double columnWidth = (item->width() - (item->columnGap() * (item->columns() - 1.0))
						  - item->textToFrameDistLeft() - item->textToFrameDistLeft()
						  - 2 * m_lineCorr) / item->columns();
	m_rightMargin = columnWidth - currentStyle.rightMargin();
	m_reverse = item->imageFlippedH();
	m_textEditMode = true;
	m_tabValues = currentStyle.tabValues();
}

void Hruler::updateTabList()
{
	ParagraphStyle::TabRecord tb;
	tb.tabPosition = m_tabValues[m_currTab].tabPosition;
	tb.tabType = m_tabValues[m_currTab].tabType;
	tb.tabFillChar =  m_tabValues[m_currTab].tabFillChar;
	int tab_n = m_tabValues.count();
	int tab_i = tab_n;
	m_tabValues.removeAt(m_currTab);
	for (int i = m_tabValues.count() - 1; i > -1; --i)
	{
		if (tb.tabPosition < m_tabValues[i].tabPosition)
			tab_i = i;
	}
	m_currTab = tab_i;
	if (tab_n == tab_i)
	{
		m_tabValues.append(tb);
		m_currTab = m_tabValues.count() - 1;
	}
	else
	{
		m_tabValues.insert(m_currTab, tb);
	}
}

void Hruler::unitChange()
{
	double sc = m_view->scale();
	m_cor = 1;
	int docUnitIndex = m_doc->unitIndex();
	switch (docUnitIndex)
	{
		case SC_PT:
			if (sc > 1 && sc <= 4)
				m_cor = 2;
			if (sc > 4)
				m_cor = 10;
			if (sc < 0.3)
			{
				m_iter = unitRulerGetIter1FromIndex(docUnitIndex) * 3;
				m_iter2 = unitRulerGetIter2FromIndex(docUnitIndex) * 3;
			}
			else if (sc < 0.5)
			{
				m_iter = unitRulerGetIter1FromIndex(docUnitIndex) * 2;
				m_iter2 = unitRulerGetIter2FromIndex(docUnitIndex) * 2;
			}
			else
			{
				m_iter = unitRulerGetIter1FromIndex(docUnitIndex) / m_cor;
				m_iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / m_cor;
			}
			break;
		case SC_MM:
			if (sc > 1)
				m_cor = 10;
			m_iter = unitRulerGetIter1FromIndex(docUnitIndex) / m_cor;
			m_iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / m_cor;
			break;
		case SC_IN:
			m_iter = unitRulerGetIter1FromIndex(docUnitIndex);
			m_iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
			if (sc > 1 && sc <= 4)
			{
				m_cor = 2;
				m_iter /= m_cor;
				m_iter2 /= m_cor;
			}
			if (sc > 4)
			{
				m_cor = 4;
				m_iter /= m_cor;
				m_iter2 /= m_cor;
			}
			if (sc < 0.25)
			{
				m_cor = 0.5;
				m_iter = 72.0 * 16.0;
				m_iter2 = 72.0 * 2.0;
			}
			break;
		case SC_P:
			m_iter = unitRulerGetIter1FromIndex(docUnitIndex);
			m_iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
			if (sc >= 1 && sc <= 4)
			{
				m_cor = 1;
				m_iter = 12.0;
				m_iter2 = 60.0;
			}
			if (sc > 4)
			{
				m_cor = 2;
				m_iter = 6.0;
				m_iter2 = 12.0;
			}
			if (sc < 0.3)
			{
				m_cor = 0.25;
				m_iter = 12.0 * 4;
				m_iter2 = 60.0 * 4;
			}
			else
				if (sc < 1)
				{
					m_cor = 1;
					m_iter = 12.0;
					m_iter2 = 60.0;
				}
			break;
		case SC_CM:
			if (sc > 1 && sc <= 4)
				m_cor = 1;
			if (sc > 4)
				m_cor = 10;
			if (sc < 0.6)
			{
				m_cor=0.1;
				m_iter = 720.0 / 25.4;
				m_iter2 = 7200.0 / 25.4;
			}
			else
			{
				m_iter = unitRulerGetIter1FromIndex(docUnitIndex) / m_cor;
				m_iter2 = unitRulerGetIter2FromIndex(docUnitIndex) / m_cor;
			}
			break;
		case SC_C:
			m_iter = unitRulerGetIter1FromIndex(docUnitIndex);
			m_iter2 = unitRulerGetIter2FromIndex(docUnitIndex);
			if (sc >= 1 && sc <= 4)
			{
				m_cor = 1;
				m_iter = 72.0 / 25.4 * 4.512;
				m_iter2 = 72.0 / 25.4 * 4.512 * 5.0;
			}
			if (sc > 4)
			{
				m_cor = 2;
				m_iter = 72.0 / 25.4 * 4.512 / 2.0;
				m_iter2 = 72.0 / 25.4 * 4.512;
			}
			if (sc < 0.3)
			{
				m_cor = 0.1;
				m_iter = 72.0 / 25.4 * 4.512 * 10;
				m_iter2 = 72.0 / 25.4 * 4.512 * 5.0 * 10;
			}
			else
				if (sc < 1)
				{
					m_cor = 1;
					m_iter = 72.0 / 25.4 * 4.512;
					m_iter2 = 72.0 / 25.4 * 4.512 * 5.0;
				}
			break;
		default:
			if (sc > 1 && sc <= 4)
				m_cor = 2;
			if (sc > 4)
				m_cor = 10;
			m_iter = unitRulerGetIter1FromIndex(0) / m_cor;
			m_iter2 = unitRulerGetIter2FromIndex(0) / m_cor;
			break;
	}
}
