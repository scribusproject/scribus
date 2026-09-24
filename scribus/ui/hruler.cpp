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
#include <QPolygon>
#include <QRect>

#include "canvasgesture_rulermove.h"
#include "hruler.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "tabmanager.h"
#include "units.h"
#include "util_gui.h"

#include "iconmanager.h"

#ifdef Q_OS_MACOS
constexpr int topline = 1;
#else
constexpr int topline = 3;
#endif
constexpr int bottomline = 24;
constexpr int rulerheight = (bottomline - topline);
constexpr int midline = bottomline / 2;
constexpr int tabline = bottomline - 10;
constexpr int scaleS = bottomline - 4;
constexpr int scaleM = bottomline - 8;
constexpr int scaleL = bottomline - 12;
constexpr int textline = scaleL;

Hruler::Hruler(ScribusView *pa, ScribusDoc *doc) : QWidget(pa),
	m_doc(doc),
	m_view(pa)
{
	//setBackgroundRole(QPalette::Window);
	//setAutoFillBackground(true);
	setMouseTracking(true);
	m_contextMenu = new QMenu(this);
	rulerGesture = new RulerGesture(m_view, RulerGesture::HORIZONTAL);
	unitChange();
	languageChange();
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(contextMenuRequested(QPoint)));
	connect(m_contextMenu, SIGNAL(triggered(QAction*)), SLOT(tabTypeChanged(QAction*)));
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(languageChange())); // replace languageChange() if there are other icons than in context menu.
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
	double canvasPos = (x + m_view->contentsX()) / m_scaling;
	if (m_rtl && m_currItem)
	{
		double colWidth = (textWidth() - m_colGap * (m_cols - 1)) / m_cols;
		double colEnd = textBase() + (colWidth + m_colGap) * (m_currCol - 1.0) + colWidth;
		return colEnd - canvasPos;
	}
	return canvasPos - textBase();
}

void Hruler::shift(double pos)
{
	m_offset = pos;
}

void Hruler::shiftRel(double dist)
{
	m_offset += dist;
}

int Hruler::rulerHeight()
{
	return bottomline;
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
	
	double colWidth = (textWidth() - m_colGap * (m_cols - 1)) / m_cols;
	
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
	
	double colOffset = (colWidth + m_colGap) * (m_currCol - 1.0);
	double colEnd    = colOffset + colWidth;

	// Map a logical value (distance from logical start) to canvas text position
	auto logicalToCanvas = [&](double v) -> double {
		return m_rtl ? (colEnd - v) : (colOffset + v);
	};

	// First Indent handle
	pos = textPosToLocal(logicalToCanvas(m_firstIndent + m_leftMargin));
	fpo = QRect(m_rtl ? (pos - grabRadius) : (pos - 1), topline, grabRadius + 1, rulerheight / 2);
	if (fpo.contains(mp))
		return rc_indentFirst;

	// Left Margin handle
	pos = textPosToLocal(logicalToCanvas(m_leftMargin));
	fpo = QRect(m_rtl ? (pos - grabRadius) : (pos - 1), midline, grabRadius + 1, rulerheight / 2);
	if (fpo.contains(mp))
		return rc_leftMargin;

	// Right Margin handle — rightMargin is always from the logical end side
	pos = textPosToLocal(colOffset + m_rightMargin);
	fpo = QRect(m_rtl ? (pos - 1) : (pos - grabRadius), midline, grabRadius, rulerheight / 2);
	if (fpo.contains(mp))
		return rc_rightMargin;

	// Tab handles
	if (!m_tabValues.isEmpty())
	{
		for (int yg = 0; yg < m_tabValues.count(); yg++)
		{
			pos = textPosToLocal(logicalToCanvas(m_tabValues[yg].tabPosition));
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
	m_mouseX = m->position().x();
	if (m_textEditMode)
	{
		m_rulerCode = findRulerHandle(m->pos(), m_doc->guidesPrefs().grabRadius);
		
		double tabPos = localToTextPos(m->position().x());
		bool isInTextRange = (tabPos >= 0) && (tabPos <= textWidth());
		if (isInTextRange && (m_rulerCode == rc_none) && (m_currCol != 0) && (m->button() == Qt::LeftButton))
		{
			const QString& textTabFillChar = m_doc->itemToolPrefs().textTabFillChar;
			ParagraphStyle::TabRecord tb;
			tb.tabPosition = tabPos;
			tb.tabType = ParagraphStyle::LeftTab;
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
			m_view->registerMousePress(m->globalPosition());
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
		if ((m->position().y() < height()) && (m->position().y() > 0))
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

void Hruler::enterEvent(QEnterEvent* e)
{
	if (m_textEditMode)
		QApplication::changeOverrideCursor(tabulatorCursor());
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
		int colStart, colEnd;
		double colWidth = (textWidth() - m_colGap * (m_cols - 1)) / m_cols;
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
		QPointF mousePos = m->position();
		if (m_mousePressed && (mousePos.y() < height()) && (mousePos.y() > 0) && (mousePos.x() > colStart - m_doc->guidesPrefs().grabRadius) && (mousePos.x() < colEnd + m_doc->guidesPrefs().grabRadius))
		{
			QApplication::changeOverrideCursor(QCursor(Qt::SizeHorCursor));
			double toplimit = textWidth() + m_distRight - (m_colGap * (m_cols - 1.0)) - 1.0;
			double toplimit2 = textWidth() + m_distLeft - (m_colGap * (m_cols - 1.0)) - 1.0;
			// In RTL dragging right moves away from the logical start (right edge),
			// so negate the delta for all logical-position markers.
			double dx = (m_mouseX - mousePos.x()) / m_scaling;
			double logicalDx = m_rtl ? -dx : dx;
			// Helper: convert a logical value (distance from logical start) to
			// canvas-relative position (distance from textBase) for MarkerMoved.
			// In LTR: canvasRel = logicalValue (distance from left edge)
			// In RTL:  canvasRel = colEnd - textBase() - logicalValue
			double colOffset = (colWidth + m_colGap) * (m_currCol - 1.0);
			auto toCanvasRel = [&](double logicalValue) -> double {
				if (m_rtl)
					return colOffset + colWidth - logicalValue;
				return logicalValue;
			};
			switch (m_rulerCode)
			{
				case rc_leftFrameDist:
					m_distLeft -= dx;
					if (m_distLeft < 0)
						m_distLeft = 0;
					if (m_distLeft > toplimit2)
						m_distLeft = toplimit2;
					emit MarkerMoved(m_currItem->xPos(), textBase() - m_currItem->xPos());
					repaint();
					break;
				case rc_rightFrameDist:
					m_distRight += dx;
					if (m_distRight < 0)
						m_distRight = 0;
					if (m_distRight > toplimit)
						m_distRight = toplimit;
					emit MarkerMoved(textBase(), toplimit - m_distRight);
					repaint();
					break;
				case rc_indentFirst:
					m_firstIndent -= logicalDx;
					if (m_firstIndent + m_leftMargin < 0)
						m_firstIndent = -m_leftMargin;
					if (m_firstIndent + m_leftMargin > colWidth)
						m_firstIndent  = colWidth - m_leftMargin;
					emit MarkerMoved(textBase(), toCanvasRel(m_firstIndent + m_leftMargin));
					repaint();
					break;
				case rc_leftMargin:
					oldInd = m_leftMargin + m_firstIndent;
					m_leftMargin -= logicalDx;
					if (m_leftMargin < 0)
						m_leftMargin = 0;
					if (m_leftMargin > colWidth - 1)
						m_leftMargin  = colWidth - 1;
					m_firstIndent = oldInd - m_leftMargin;
					emit MarkerMoved(textBase(), toCanvasRel(m_leftMargin));
					repaint();
					break;
				case rc_rightMargin:
					m_rightMargin -= logicalDx;
					if (m_rightMargin < 0)
						m_rightMargin = 0;
					if (m_rightMargin > colWidth - 1)
						m_rightMargin  = colWidth - 1;
					emit MarkerMoved(textBase(), toCanvasRel(m_rightMargin));
					repaint();
					break;
				case rc_tab:
					m_tabValues[m_currTab].tabPosition -= logicalDx;
					if (m_tabValues[m_currTab].tabPosition < 0)
						m_tabValues[m_currTab].tabPosition = 0;
					if (m_tabValues[m_currTab].tabPosition > colWidth - 1)
						m_tabValues[m_currTab].tabPosition  = colWidth - 1;
					emit MarkerMoved(textBase(), toCanvasRel(m_tabValues[m_currTab].tabPosition));
					updateTabList();
					repaint();
					break;
				default:
					break;
			}
			m_mouseX = mousePos.x();
			return;
		}
		if ((!m_mousePressed) && (mousePos.y() < height()) && (mousePos.y() > 0) && (mousePos.x() > colStart - 2 * m_doc->guidesPrefs().grabRadius) && (mousePos.x() < colEnd + 2 * m_doc->guidesPrefs().grabRadius))
		{
			setCursor(tabulatorCursor());
			switch (findRulerHandle(m->pos(), m_doc->guidesPrefs().grabRadius))
			{
				case rc_leftFrameDist:
				case rc_rightFrameDist:
					setCursor(QCursor(Qt::SplitHCursor));
					break;
				case rc_indentFirst:
				case rc_leftMargin:
				case rc_rightMargin:
				case rc_tab:
					setCursor(QCursor(Qt::SizeHorCursor));
					break;
				default:
					break;
			}
			draw(mousePos.x() + m_view->contentsX());
			double canvasX = (mousePos.x() + m_view->contentsX()) / m_scaling;
			emit MarkerMoved(textBase(), canvasX - textBase());
			return;
		}
		if (m_mousePressed && (m_rulerCode == rc_tab) && ((mousePos.y() > height()) || (mousePos.y() < 0)))
		{
			setCursor(IconManager::instance().loadCursor("cursor-remove-point", 1, 1));
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

void Hruler::mouseDoubleClickEvent(QMouseEvent* m)
{
	QWidget::mouseDoubleClickEvent( m );

	if (!m_doc || !m_currItem || m_doc->isLoading())
		return;

	m_rulerCode = findRulerHandle(m->pos(), m_doc->guidesPrefs().grabRadius);

	if ( m->button() == Qt::LeftButton && m_textEditMode && m_rulerCode == rc_tab)
	{
		m_mousePressed = false;
		m_rulerCode = rc_none;

		PageItem_TextFrame *tItem = m_currItem->asTextFrame();
		if (tItem == nullptr)
			return;

		const ParagraphStyle& style(m_doc->appMode == modeEdit ? tItem->currentStyle() : tItem->itemText.defaultStyle());
		TabManager *dia = new TabManager(this, m_doc->unitIndex(), style.tabValues(), tItem->columnWidth());
		dia->setRtl(m_rtl);
		if (dia->exec())
		{
			ParagraphStyle paraStyle;
			paraStyle.setTabValues(dia->tabList());
			Selection tempSelection(this, false);
			tempSelection.addItem(m_currItem);
			m_doc->itemSelection_ApplyParagraphStyle(paraStyle, &tempSelection);
			emit DocChanged(false);
		}
		delete dia;
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

	const QPalette& palette = this->palette();
	const QColor& backgroundColor = palette.color(QPalette::Base);

	QPainter p;
	p.begin(this);
	p.setClipRect(e->rect());
	p.setFont(font());
	p.fillRect(rect(), backgroundColor);

	drawMarks(p);

	if (m_textEditMode)
	{
		const QColor& markerColor = blendColor(isDarkColor(backgroundColor), QColor(117, 182, 240), QColor(51, 132, 204));
		const QColor& marginColor = markerColor;
		QColor columnColor = markerColor;
		columnColor.setAlphaF(0.2f);
		const QColor& textColor = palette.color(QPalette::Text);
		p.save();

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

			p.fillRect(QRect(QPoint(textPosToLocal(pos), 0), QPoint(textPosToLocal(endPos), bottomline)), backgroundColor);
			p.fillRect(QRect(QPoint(textPosToLocal(pos), 0), QPoint(textPosToLocal(endPos), bottomline)), columnColor);

			drawTextMarks(pos, endPos, p);

			if (m_rtl)
			{
				// RTL: logical start is the RIGHT edge of the column

				// start bracket on the right
				p.setPen(QPen(marginColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				int xPos = textPosToLocal(endPos);
				p.drawLine(xPos, 0, xPos, bottomline);
				p.drawLine(xPos, 0, xPos - 8, 0);

				// First Indent - measured from right edge inward
				xPos = textPosToLocal(endPos - m_leftMargin - m_firstIndent);
				p.setPen(QPen(textColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				p.drawLine(xPos, tabline - 2, xPos, bottomline);
				p.setPen(Qt::NoPen);
				p.setRenderHints(QPainter::Antialiasing, true);
				p.drawRect(QRect(xPos - 8, tabline - 8, 8, 6));

				// Left Margin (logical start = right side), triangle opens left
				xPos = textPosToLocal(endPos - m_leftMargin);
				QPolygon cr2;
				cr2.setPoints(3, xPos, tabline, xPos - 8, tabline, xPos, bottomline);
				p.drawPolygon(cr2);

				// Right Margin (logical end = left side), triangle opens right
				xPos = textPosToLocal(endPos - m_rightMargin);
				QPolygon cr3;
				cr3.setPoints(3, xPos, tabline, xPos + 8, tabline, xPos, bottomline);
				p.drawPolygon(cr3);

				p.setRenderHints(QPainter::Antialiasing, false);

				// Tabulators - measured from right edge
				if (!m_tabValues.isEmpty())
				{
					int pWidth = 1;
					p.setPen(QPen(textColor, pWidth * 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
					for (int yg = 0; yg < m_tabValues.count(); ++yg)
					{
						xPos = textPosToLocal(endPos - m_tabValues[yg].tabPosition);
						if (m_tabValues[yg].tabPosition >= colWidth)
							continue;
						switch (m_tabValues[yg].tabType)
						{
							case ParagraphStyle::LeftTab:
								p.drawLine(xPos, tabline, xPos, bottomline - pWidth);
								p.drawLine(xPos, bottomline - pWidth, xPos - 8, bottomline - pWidth);
								break;
							case ParagraphStyle::RightTab:
								p.drawLine(xPos, tabline, xPos, bottomline - pWidth);
								p.drawLine(xPos, bottomline - pWidth, xPos + 8, bottomline - pWidth);
								break;
							case ParagraphStyle::CommaTab:
							case ParagraphStyle::PeriodTab:
								p.drawLine(xPos, tabline, xPos, bottomline - pWidth);
								p.drawLine(xPos - 4, bottomline - pWidth, xPos + 4, bottomline - pWidth);
								p.drawLine(xPos - 3, bottomline - 3 - pWidth, xPos - 2, bottomline - 3 - pWidth);
								break;
							case ParagraphStyle::CenterTab:
								p.drawLine(xPos, tabline, xPos, bottomline - pWidth);
								p.drawLine(xPos - 4, bottomline - pWidth, xPos + 4, bottomline - pWidth);
								break;
							default:
								break;
						}
					}
				}

				// end bracket on the left
				p.setPen(QPen(marginColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				xPos = textPosToLocal(pos);
				p.drawLine(xPos, 0, xPos, bottomline - 1);
				p.drawLine(xPos, 0, xPos + 8, 0);
			}
			else
			{
				// LTR: original logic

				// start bracket
				p.setPen(QPen(marginColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				int xPos = textPosToLocal(pos);
				p.drawLine(xPos, 0, xPos, bottomline);
				p.drawLine(xPos, 0, (xPos + 8), 0);

				// First Indent
				xPos = textPosToLocal(pos + m_firstIndent + m_leftMargin);
				p.setPen(QPen(textColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				p.drawLine(xPos, tabline - 2, xPos, bottomline);
				p.setPen(Qt::NoPen);
				p.setRenderHints(QPainter::Antialiasing, true);
				p.drawRect(QRect(xPos, tabline - 8, 8, 6));

				// Left Margin
				xPos = textPosToLocal(pos + m_leftMargin);
				QPolygon cr2;
				cr2.setPoints(3, xPos, tabline, xPos + 8, tabline, xPos, bottomline);
				p.drawPolygon(cr2);

				// Right Margin
				xPos = textPosToLocal(pos + m_rightMargin);
				QPolygon cr3;
				cr3.setPoints(3, xPos - 8, tabline, xPos, tabline, xPos, bottomline);
				p.drawPolygon(cr3);

				p.setRenderHints(QPainter::Antialiasing, false);

				// Tabulator
				if (!m_tabValues.isEmpty())
				{
					int pWidth = 1;
					p.setPen(QPen(textColor, pWidth * 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
					for (int yg = 0; yg < m_tabValues.count(); ++yg)
					{
						xPos = textPosToLocal(pos + m_tabValues[yg].tabPosition);
						if (pos + m_tabValues[yg].tabPosition >= endPos)
							continue;
						switch (m_tabValues[yg].tabType)
						{
							case ParagraphStyle::LeftTab:
								if (m_reverse)
								{
									p.save();
									p.translate(pos + m_tabValues[yg].tabPosition, 0);
									p.scale(-1, 1);
									p.drawLine(0, tabline, 0, bottomline - pWidth);
									p.drawLine(0, bottomline - pWidth, 8, bottomline - pWidth);
									p.restore();
								}
								else
								{
									p.drawLine(xPos, tabline, xPos, bottomline - pWidth);
									p.drawLine(xPos, bottomline - pWidth, xPos + 8, bottomline - pWidth);
								}
								break;
							case ParagraphStyle::RightTab:
								if (m_reverse)
								{
									p.save();
									p.translate(pos + m_tabValues[yg].tabPosition, 0);
									p.scale(-1, 1);
									p.drawLine(0, tabline, 0, bottomline - pWidth);
									p.drawLine(0, bottomline - pWidth, -8, bottomline - pWidth);
									p.restore();
								}
								else
								{
									p.drawLine(xPos, tabline, xPos, bottomline - pWidth);
									p.drawLine(xPos, bottomline - pWidth, xPos - 8, bottomline - pWidth);
								}
								break;
							case ParagraphStyle::CommaTab:
							case ParagraphStyle::PeriodTab:
								p.drawLine(xPos, tabline, xPos, bottomline - pWidth);
								p.drawLine(xPos - 4, bottomline - pWidth, xPos + 4, bottomline - pWidth);
								p.drawLine(xPos + 3, bottomline - 3 - pWidth, xPos - 2, bottomline - 3 - pWidth);
								break;
							case ParagraphStyle::CenterTab:
								p.drawLine(xPos, tabline, xPos, bottomline - pWidth);
								p.drawLine(xPos - 4, bottomline - pWidth, xPos + 4, bottomline - pWidth);
								break;
							default:
								break;
						}
					}
				}

				// end bracket
				p.setPen(QPen(marginColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
				xPos = textPosToLocal(endPos);
				p.drawLine(xPos, 0, xPos, bottomline - 1);
				p.drawLine(xPos, 0, xPos - 8, 0);
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
	const QColor& markerColor = blendColor(isDarkColor(palette().color(QPalette::Base)), QColor(255, 117, 102), QColor(255, 71, 51));
	QPolygon cr;
	cr.setPoints(3, m_whereToDraw, 5, m_whereToDraw + 2, 0, m_whereToDraw - 2, 0);

	p.resetTransform();
	p.setPen(markerColor);
	p.drawLine(m_whereToDraw, 0, m_whereToDraw, bottomline);
	p.setRenderHints(QPainter::Antialiasing, true);
	p.setPen(Qt::NoPen);
	p.setBrush(markerColor);
	p.drawPolygon(cr);
	p.setRenderHints(QPainter::Antialiasing, false);

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
		p.drawLine(qRound(firstMark * sc), scaleS, qRound(firstMark * sc), bottomline);
		firstMark += m_iter;
	}
	firstMark = ceil(m_offset / m_iter2) * m_iter2 - m_offset;
	int markC = static_cast<int>(ceil(m_offset / m_iter2));

	const bool isRtl = m_rtl && (m_currItem != nullptr);
	const int pageRightMarkC = static_cast<int>(round(m_doc->currentPage()->width() / m_iter2));

	QString tx;
	double xl, frac;
	while (firstMark < cc)
	{
		int effectiveMarkC = isRtl ? (pageRightMarkC - markC) : markC;
		switch (m_doc->unitIndex())
		{
			case SC_MM:
				tx = QString::number(effectiveMarkC * m_iter2 / (m_iter2 / 100) / m_cor);
				break;
			case SC_IN:
				xl = (effectiveMarkC * m_iter2 / m_iter2) / m_cor;
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
				tx = QString::number(effectiveMarkC * m_iter2 / (m_iter2 / 5) / m_cor);
				break;
			case SC_CM:
				tx = QString::number(effectiveMarkC * m_iter2 / m_iter2 / m_cor);
				break;
			default:
				tx = QString::number(effectiveMarkC * m_iter2);
				break;
		}
		p.drawLine(qRound(firstMark * sc), scaleL, qRound(firstMark * sc), bottomline);
		drawNumber(tx, qRound(firstMark * sc) + 2, p);
		firstMark += m_iter2;
		markC++;
	}

	if (m_doc->unitIndex() == SC_C || m_doc->unitIndex() == SC_P)
		return;

	double tickStep = m_iter2 / 2.0;
	firstMark = ceil(m_offset / tickStep) * tickStep - m_offset;
	int markM = static_cast<int>(ceil(m_offset / tickStep));
	while (firstMark < cc)
	{
		p.drawLine(qRound(firstMark * sc), scaleM, qRound(firstMark * sc), bottomline);
		firstMark += tickStep;
		markM++;
	}
}	

void Hruler::drawTextMarks(double pos, double endPos, QPainter& p) const
{
	double xl;
	QColor color = blendColor(isDarkColor(palette().color(QPalette::Base)), QColor(117, 182, 240), QColor(51, 132, 204));
	p.setPen(QPen(color, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

	for (xl = pos; xl < endPos; xl += m_iter)
	{
		int xli = textPosToLocal(xl);
		p.drawLine(xli, scaleS, xli, bottomline);
	}
	// LTR: start at pos, step right, dist = xl - pos (0 at left edge, grows right)
	// RTL: start at endPos, step left, dist = endPos - xl (0 at right edge, grows left)
	double xlStart = m_rtl ? endPos : pos;
	double xlStep  = m_rtl ? -m_iter2 : m_iter2;
	for (xl = xlStart; m_rtl ? (xl >= pos) : (xl < endPos); xl += xlStep)
	{
		int xli = textPosToLocal(xl);
		p.drawLine(xli, scaleL, xli, bottomline);
		double dist = m_rtl ? (endPos - xl) : (xl - pos);
		auto drawNum = [&](const QString& tx) {
			if (m_reverse) {
				p.save();
				p.translate(xli - 2.0, 0);
				p.scale(-1, 1);
				drawNumber(tx, 0, p);
				p.restore();
			} else {
				drawNumber(tx, xli + 2, p);
			}
		};
		switch (m_doc->unitIndex())
		{
			case SC_IN:
				{
					QString tx;
					int num1 = static_cast<int>(dist / m_iter2 / m_cor);
					if (num1 != 0)
						tx = QString::number(num1);
					double frac = (dist / m_iter2 / m_cor) - num1;
					if ((frac > 0.24) && (frac < 0.26))
						tx += QChar(0xBC);
					if ((frac > 0.49) && (frac < 0.51))
						tx += QChar(0xBD);
					if ((frac > 0.74) && (frac < 0.76))
						tx += QChar(0xBE);
					drawNum(tx);
					break;
				}
			case SC_P:
				drawNum(QString::number(dist / m_iter / m_cor));
				break;
			case SC_CM:
				drawNum(QString::number(dist / m_iter / 10 / m_cor));
				break;
			case SC_C:
				drawNum(QString::number(dist / m_iter / m_cor));
				break;
			default:
				drawNum(QString::number(dist / m_iter * 10 / m_cor));
				break;
		}
	}

	if (m_doc->unitIndex() == SC_C || m_doc->unitIndex() == SC_P)
		return;

	for (xl = pos; xl < endPos; xl += (m_iter2 / 2.0))
	{
		int xli = textPosToLocal(xl);
		p.drawLine(xli, scaleM, xli, bottomline);
	}
}

void Hruler::drawNumber(const QString& txt, int x, QPainter & p) const
{
	const int y = textline;
	p.drawText(x,y,txt);
}

QCursor Hruler::tabulatorCursor() const
{
	QCursor c = IconManager::instance().loadCursor("cursor-tabulator", 3);
	if (m_rtl)
	{
		QPixmap pix = c.pixmap();
		QPixmap mirrored = pix.transformed(QTransform(-1, 0, 0, 1, 0, 0));
		c = QCursor(mirrored, pix.width() - c.hotSpot().x() - 1, c.hotSpot().y());
	}
	return c;
}

double Hruler::ruleSpacing() const
{
	return m_iter;
}

void Hruler::draw(int where)
{
	const int currentCoor = where - m_view->contentsX();
	const int oldCoor = m_oldMark;
	m_whereToDraw = currentCoor;
	m_oldMark = currentCoor;
	m_drawMark = true;
	update(QRect(oldCoor - 4, 0, 8, bottomline).united(QRect(currentCoor - 4, 0, 8, bottomline)));
}

void Hruler::setItem(PageItem * item)
{
	m_currItem = item;
	QTransform mm = m_currItem->getTransform();
	// A table cell's text frame stores a table-relative position, and
	// getTransform() does not move to the table for it (it is not a
	// group child). Compose the table's transform so the ruler origin
	// lands at the cell's true page position.
	if (m_currItem->isTableCell() && m_currItem->Parent)
		mm = mm * m_currItem->Parent->getTransform();
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
	setRtl(item->currentStyle().direction() == ParagraphStyle::RTL);
	m_textEditMode = true;
	m_tabValues = currentStyle.tabValues();
}

void Hruler::setRtl(bool rtl)
{
	m_rtl = rtl;
	languageChange();
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

void Hruler::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void Hruler::languageChange()
{
	IconManager &im = IconManager::instance();

	QSignalBlocker sigMenu(m_contextMenu);
	m_contextMenu->clear();
	if (m_rtl)
	{
		m_contextMenu->addAction(im.loadIcon("tabulator-right"), tr("Right"))->setData(ParagraphStyle::LeftTab);
		m_contextMenu->addAction(im.loadIcon("tabulator-center"), tr("Center"))->setData(ParagraphStyle::CenterTab);
		m_contextMenu->addAction(im.loadIcon("tabulator-comma"), tr("Comma"))->setData(ParagraphStyle::CommaTab);
		m_contextMenu->addAction(im.loadIcon("tabulator-dot"), tr("Period"))->setData(ParagraphStyle::PeriodTab);
		m_contextMenu->addAction(im.loadIcon("tabulator-left"), tr("Left"))->setData(ParagraphStyle::RightTab);
	}
	else
	{
		m_contextMenu->addAction(im.loadIcon("tabulator-left"), tr("Left"))->setData(ParagraphStyle::LeftTab);
		m_contextMenu->addAction(im.loadIcon("tabulator-center"), tr("Center"))->setData(ParagraphStyle::CenterTab);
		m_contextMenu->addAction(im.loadIcon("tabulator-comma"), tr("Comma"))->setData(ParagraphStyle::CommaTab);
		m_contextMenu->addAction(im.loadIcon("tabulator-dot"), tr("Period"))->setData(ParagraphStyle::PeriodTab);
		m_contextMenu->addAction(im.loadIcon("tabulator-right"), tr("Right"))->setData(ParagraphStyle::RightTab);
	}
}

void Hruler::contextMenuRequested(QPoint mouse)
{
	if (m_textEditMode && m_rulerCode == rc_tab)
	{
		m_mousePressed = false;
		m_contextMenu->popup(this->mapToGlobal(mouse));
	}
}

void Hruler::tabTypeChanged(QAction *action)
{
	m_tabValues[m_currTab].tabType = action->data().toInt();
	ParagraphStyle paraStyle;
	paraStyle.setTabValues(m_tabValues);

	emit DocChanged(false);

	Selection tempSelection(this, false);
	tempSelection.addItem(m_currItem);
	m_doc->itemSelection_ApplyParagraphStyle(paraStyle, &tempSelection);
}
