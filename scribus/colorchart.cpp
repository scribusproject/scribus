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
#include "colorchart.moc"
#include <qpixmap.h>
#include <qimage.h>
#include <qpainter.h>
extern QImage ProofImage(QImage *Im);

ColorChart::ColorChart(QWidget *parent) : QLabel(parent)
{
	setScaledContents( true );
	setAlignment( static_cast<int>( QLabel::AlignCenter ) );
	Xp = 0;
	Yp = 0;
	setBackgroundMode(NoBackground);
	drawPalette(255);
}

void ColorChart::mouseMoveEvent(QMouseEvent *m)
{
	drawMark(m->x(), m->y());
	emit ColorVal(m->x() * 359 / width(), m->y() * 255 / height(), false);
}

void ColorChart::mousePressEvent(QMouseEvent *m)
{
	drawMark(m->x(), m->y());
	emit ColorVal(m->x() * 359 / width(), m->y() * 255 / height(), false);
}

void ColorChart::mouseReleaseEvent(QMouseEvent *m)
{
	drawMark(m->x(), m->y());
	emit ColorVal(m->x() * 359 / width(), m->y() * 255 / height(), true);
}

void ColorChart::paintEvent(QPaintEvent *e)
{
	QPainter p;
	p.begin(this);
	p.setClipRect(e->rect());
	p.drawPixmap(0, 0, pmx);
	drawMark(Xp, Yp);
	p.end();
}

void ColorChart::drawMark(int x, int y)
{
	QPainter p;
	p.begin(this);
	p.setRasterOp(XorROP);
	p.setPen(QPen(QColor(white), 1, SolidLine, FlatCap, MiterJoin));
	p.drawLine(Xp-5, Yp-5, Xp-1, Yp-1);
	p.drawLine(Xp-5, Yp+5, Xp-1, Yp+1);
	p.drawLine(Xp+2, Yp+2, Xp+6, Yp+6);
	p.drawLine(Xp+2, Yp-2, Xp+6, Yp-6);
	if (!((Xp == x) && (Yp == y)))
	{
		Xp = x;
		Yp = y;
		p.drawLine(Xp-5, Yp-5, Xp-1, Yp-1);
		p.drawLine(Xp-5, Yp+5, Xp-1, Yp+1);
		p.drawLine(Xp+2, Yp+2, Xp+6, Yp+6);
		p.drawLine(Xp+2, Yp-2, Xp+6, Yp-6);
	}
	p.end();
}

void ColorChart::setMark(int h, int s)
{
	drawMark(h * width() / 359, (255-s) * height() / 255);
}

void ColorChart::drawPalette(int val)
{
	int xSize = width();
	int ySize = height();
	QImage image(xSize, ySize, 32);
	QColor color;
	int x;
	int y;
	for (y = 0; y < ySize; ++y)
	{
		unsigned int* p = reinterpret_cast<unsigned int*>(image.scanLine(y));
		for(x = 0; x < xSize; ++x)
		{
			color.setHsv(360*x/xSize, 256*( ySize - 1 - y )/ySize, val);
			*p = color.rgb();
			++p;
		}
	}
	pmx.convertFromImage(ProofImage(&image));
	setPixmap(pmx);
}
