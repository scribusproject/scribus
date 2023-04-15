/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "arrowchooser.h"

#include <QImage>
#include <QPalette>
#include <QPixmap>

#include "commonstrings.h"
#include "fpointarray.h"
#include "scpainter.h"
#include "scribusstructs.h"
#include "util_math.h"

ArrowChooser::ArrowChooser(QWidget* pa, ArrowDirection direction) : QComboBox(pa)
{
	setEditable(false);
	clear();
	setIconSize(QSize(22, 22));
	addItem(CommonStrings::tr_None);
	setArrowDirection(direction);
}

void ArrowChooser::setArrowDirection(ArrowDirection direction)
{
	m_arrowDirection = direction;
}

void ArrowChooser::rebuildList(const QList<ArrowDesc> *arrowStyles)
{
	FPointArray path;

	clear();

	const QPalette& pal = this->palette();
	QColor textColor = pal.color(QPalette::Active, QPalette::Text);

	addItem(CommonStrings::tr_None);
	for (int i = 0; i < arrowStyles->count(); ++i)
	{
		QImage image(22, 22, QImage::Format_ARGB32_Premultiplied);
		image.fill(0);
		ScPainter *painter = new ScPainter(&image, 22, 22);
//		painter->clear();
		painter->setBrush(textColor);
		painter->setPen(textColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		painter->setFillMode(ScPainter::Solid);
		painter->translate(3.0, 3.0);
		path.resize(0);
		path = arrowStyles->at(i).points.copy();
		FPoint min = getMinClipF(&path);
		path.translate(-min.x(), -min.y());
		FPoint max = path.widthHeight();
		QTransform mm;
		QTransform mm2;
		if (m_arrowDirection == ArrowDirection::StartArrow)
		{
			mm2.scale(-1, 1);
			mm2.translate(-max.x(), 0);
		}
		mm.scale(16.0 / qMax(max.x(), max.y()), 16.0 / qMax(max.x(), max.y()));
		path.map(mm2 * mm);
		painter->setupPolygon(&path);
		painter->drawPolygon();
		painter->drawPolyLine();
		painter->end();
		delete painter;
/*		int wi = image.width();
		int hi = image.height();
		for( int yi=0; yi < hi; ++yi )
		{
			QRgb *s = (QRgb*)(image.scanLine( yi ));
			for(int xi=0; xi < wi; ++xi )
			{
				if ((*s) == 0xffffffff)
					(*s) &= 0x00ffffff;
				s++;
			}
		} */
		QPixmap ico = QPixmap::fromImage(image);
		addItem(ico, arrowStyles->at(i).name);
	}
}
