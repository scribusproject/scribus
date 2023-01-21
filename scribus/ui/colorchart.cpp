/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          colorchart.cpp  -  description
                             -------------------
    begin                : Sat Sep 15 2001
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

#include "colorchart.h"

#include <algorithm>

#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>

#include "iconmanager.h"
#include "util_color.h"
#include "sccolorengine.h"
#include "scribuscore.h"
#include "scribusdoc.h"

ColorChart::ColorChart(QWidget *parent) : QWidget(parent)
{
	setAutoFillBackground(false);
	drawPalette(m_currentValue);
}

ColorChart::ColorChart(QWidget *parent, ScribusDoc* doc) : QWidget(parent), m_doc(doc)
{
	setAutoFillBackground(false);
	drawPalette(m_currentValue);
}

void ColorChart::mouseMoveEvent(QMouseEvent *m)
{
	drawMark(m->x(), m->y());
	if (drawMode > 0)
		emit ColorVal(m->x() * 256 / width() - 128, 256 - (m->y() * 256 / height()) - 128, true);
	else
		emit ColorVal(m->x() * 359 / width(), m->y() * 255 / height(), true);
}

void ColorChart::mousePressEvent(QMouseEvent *m)
{
	drawMark(m->x(), m->y());
	if (drawMode > 0)
		emit ColorVal(m->x() * 256 / width() - 128, 256 - (m->y() * 256 / height()) - 128, true);
	else
		emit ColorVal(m->x() * 359 / width(), m->y() * 255 / height(), true);
}

void ColorChart::mouseReleaseEvent(QMouseEvent *m)
{
	drawMark(m->x(), m->y());
	if (drawMode > 0)
		emit ColorVal(m->x() * 256 / width() - 128, 256 - (m->y() * 256 / height()) - 128, true);
	else
		emit ColorVal(m->x() * 359 / width(), m->y() * 255 / height(), true);
}

void ColorChart::paintEvent(QPaintEvent *e)
{
	QPainter p;
	QPainter p2;

	QImage tmp(width(), height(), QImage::Format_ARGB32_Premultiplied);
	p2.begin(&tmp);
	p2.drawPixmap(0, 0, m_pixmap);
	if (m_drawMark)
	{
		double markX = m_markX * width();
		double markY = m_markY * height();
		p2.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		p2.drawLine(markX - 5, markY - 5, markX - 1, markY - 1);
		p2.drawLine(markX - 5, markY + 5, markX - 1, markY + 1);
		p2.drawLine(markX + 2, markY + 2, markX + 6, markY + 6);
		p2.drawLine(markX + 2, markY - 2, markX + 6, markY - 6);
		m_drawMark = false;
	}
	p2.end();

	p.begin(this);
	p.setClipRect(e->rect());
	p.drawImage(0, 0, tmp);
	p.end();
}

void ColorChart::resizeEvent(QResizeEvent *e)
{
	if (e->oldSize() != e->size())
	{
		if (drawMode > 0)
			drawLabColorMap(m_currentValue);
		else
			drawHsvColorMap(m_currentValue);
	}
	QWidget::resizeEvent(e);
}

void ColorChart::drawMark(int x, int y)
{
	m_markX = x / (double) width();
	m_markY = y / (double) height();
	m_drawMark = true;
	update();
}

void ColorChart::setMark(int h, int s)
{
	if (drawMode > 0)
		drawMark((h + 128) / 256.0 * width(), (-s + 128) / 256.0 * height());
	else
		drawMark(h * width() / 359, (255 - s) * height() / 255);
}

void ColorChart::drawPalette(int val)
{
	m_currentValue = std::max(0, std::min(val, 255));

	if (drawMode > 0)
		drawLabColorMap(m_currentValue);
	else
		drawHsvColorMap(m_currentValue);
	update();
}

void ColorChart::drawHsvColorMap(int val)
{
	int xSize = width();
	int ySize = height();

	QImage image(xSize, ySize, QImage::Format_ARGB32_Premultiplied);
	QColor color;
	for (int y = 0; y < ySize; ++y)
	{
		unsigned int* p = reinterpret_cast<unsigned int*>(image.scanLine(y));
		for(int x = 0; x < xSize; ++x)
		{
			color.setHsv(360 * x / xSize, 256 * (ySize - 1 - y) / ySize, val);
			*p = color.rgb();
			++p;
		}
	}
	m_pixmap = QPixmap::fromImage(ProofImage(&image, m_doc));
}

void ColorChart::drawLabColorMap(int val)
{
	int xSize = width();
	int ySize = height();

	QImage image(128, 128, QImage::Format_ARGB32);
	bool doSoftProofing = m_doc ? m_doc->SoftProofing : false;
	bool doGamutCheck   = m_doc ? m_doc->Gamut : false;
	if (doSoftProofing && doGamutCheck)
	{
		QPainter p;
		QBrush b(QColor(205, 205, 205), IconManager::instance().loadPixmap("testfill.png"));
		p.begin(&image);
		p.fillRect(0, 0, image.width(), image.height(), b);
		p.end();
	}
	QColor color;
	double L = val /  2.55;
	for (int y = 0; y < 128; y++)
	{
		unsigned int* p = reinterpret_cast<unsigned int*>(image.scanLine(y));
		for (int x = 0; x < 128; x++)
		{
			double yy = 256 - (y * 2.0);
			if (doSoftProofing && doGamutCheck)
			{
				bool outOfG = false;
				color = ScColorEngine::getDisplayColorGC(ScColor(L, (x * 2.0) - 128.0, yy - 128.0), m_doc, &outOfG);
				if (!outOfG)
					*p = color.rgb();
			}
			else
			{
				color = ScColorEngine::getDisplayColor(ScColor(L, (x * 2.0) - 128.0, yy - 128.0), m_doc);
				*p = color.rgb();
			}
			++p;
		}
	}
	m_pixmap = QPixmap::fromImage(image.scaled(xSize, ySize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
