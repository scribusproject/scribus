/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "basepointwidget.h"

BasePointWidget::BasePointWidget(QWidget *pa, int mode) : QWidget(pa)
{
	m_mode = mode;
	if (m_mode == 0)
	{
		setMinimumSize(QSize(48, 48));
		setMaximumSize(QSize(48, 48));
	}
	else
	{
		setMinimumSize(QSize(48, 16));
		setMaximumSize(QSize(48, 16));
	}
	setMouseTracking(true);
	selectedBase = 0;
	hooveredBase = -1;
	Mpressed = false;
} 

void BasePointWidget::paintEvent(QPaintEvent *e)
{
	QPainter p;
	p.begin(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setBrush(palette().color(QPalette::Window));
	p.setPen(Qt::NoPen);
	p.drawRect(0, 0, width(), height());
	p.setBrush(Qt::black);
	p.setPen(QPen(palette().color(QPalette::WindowText), 1));
	if (m_mode == 0)
	{
		p.drawEllipse( 4,  4, 10, 10);
		p.drawEllipse(34,  4, 10, 10);
		p.drawEllipse(19, 19, 10, 10);
		p.drawEllipse( 4, 34, 10, 10);
		p.drawEllipse(34, 34, 10, 10);
	}
	else
	{
		p.drawEllipse( 4,  4, 10, 10);
		p.drawEllipse(19,  4, 10, 10);
		p.drawEllipse(34,  4, 10, 10);
	}
	p.setPen(QPen(palette().color(QPalette::WindowText), 2));
	if (m_mode == 0)
	{
		p.drawLine(8, 8, 33, 8);
		p.drawLine(8, 38, 33, 38);
		p.drawLine(8, 8, 8, 33);
		p.drawLine(38, 8, 38, 33);
	}
	else
		p.drawLine(8, 8, 33, 8);
	p.setBrush(Qt::white);
	p.setPen(QPen(palette().color(QPalette::WindowText), 2));
	if (m_mode == 0)
	{
		p.drawEllipse( 3,  3, 10, 10);
		p.drawEllipse(33,  3, 10, 10);
		p.drawEllipse(18, 18, 10, 10);
		p.drawEllipse( 3, 33, 10, 10);
		p.drawEllipse(33, 33, 10, 10);
	}
	else
	{
		p.drawEllipse( 3,  3, 10, 10);
		p.drawEllipse(18,  3, 10, 10);
		p.drawEllipse(33,  3, 10, 10);
	}
	if (hooveredBase > -1)
	{
		p.setBrush(Qt::NoBrush);
		p.setPen(QPen(palette().color(QPalette::Highlight), 1));
		if (hooveredBase == 0)
			p.drawEllipse( 4,  4, 8, 8);
		else if (hooveredBase == 1)
			p.drawEllipse(34,  4, 8, 8);
		else if (hooveredBase == 2)
		{
			if (m_mode == 0)
				p.drawEllipse(19, 19, 8, 8);
			else
				p.drawEllipse(19,  4, 8, 8);
		}
		else if (hooveredBase == 3)
			p.drawEllipse( 4, 34, 8, 8);
		else if (hooveredBase == 4)
			p.drawEllipse(34, 34, 8, 8);
	}
	p.setBrush(palette().color(QPalette::WindowText));
	p.setPen(Qt::NoPen);
	if (selectedBase == 0)
		p.drawEllipse( 5, 5, 6, 6);
	else if (selectedBase == 1)
		p.drawEllipse(35, 5, 6, 6);
	else if (selectedBase == 2)
	{
		if (m_mode == 0)
			p.drawEllipse(20, 20, 6, 6);
		else
			p.drawEllipse(20,  5, 6, 6);
	}
	else if (selectedBase == 3)
		p.drawEllipse( 5, 35, 6, 6);
	else if (selectedBase == 4)
		p.drawEllipse(35, 35, 6, 6);
	p.end();
}

void BasePointWidget::mousePressEvent(QMouseEvent *m)
{
	m->accept();
	Mpressed = true;
}

void BasePointWidget::mouseReleaseEvent(QMouseEvent *m)
{
	m->accept();
	if (Mpressed)
	{
		QPoint p = m->pos();
		if (m_mode == 0)
		{
			if (QRect( 3,  3, 10, 10).contains(p))
				selectedBase = 0;
			else if (QRect(33,  3, 10, 10).contains(p))
				selectedBase = 1;
			else if (QRect(18, 18, 10, 10).contains(p))
				selectedBase = 2;
			else if (QRect( 3, 33, 10, 10).contains(p))
				selectedBase = 3;
			else if (QRect(33, 33, 10, 10).contains(p))
				selectedBase = 4;
		}
		else
		{
			if (QRect( 3,  3, 9, 9).contains(p))
				selectedBase = 0;
			else if (QRect(33,  3, 10, 10).contains(p))
				selectedBase = 1;
			else if (QRect(18,  3, 10, 10).contains(p))
				selectedBase = 2;
		}
		emit buttonClicked(selectedBase);
	}
	update();
}

void BasePointWidget::mouseMoveEvent(QMouseEvent *m)
{
	m->accept();
	QPoint p = m->pos();
	hooveredBase = -1;
	if (m_mode == 0)
	{
		if (QRect( 3,  3, 10, 10).contains(p))
			hooveredBase = 0;
		else if (QRect(33,  3, 10, 10).contains(p))
			hooveredBase = 1;
		else if (QRect(18, 18, 10, 10).contains(p))
			hooveredBase = 2;
		else if (QRect( 3, 33, 10, 10).contains(p))
			hooveredBase = 3;
		else if (QRect(33, 33, 10, 10).contains(p))
			hooveredBase = 4;
	}
	else
	{
		if (QRect( 3,  3, 10, 10).contains(p))
			hooveredBase = 0;
		else if (QRect(33,  3, 10, 10).contains(p))
			hooveredBase = 1;
		else if (QRect(18,  3, 10, 10).contains(p))
			hooveredBase = 2;
	}
	update();
}

void BasePointWidget::leaveEvent(QEvent*)
{
	hooveredBase = -1;
	update();
}

int BasePointWidget::checkedId()
{
	return selectedBase;
}

void BasePointWidget::setCheckedId(int id)
{
	selectedBase = id;
	update();
}
