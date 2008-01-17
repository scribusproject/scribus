#include <QPixmap>
#include <QPaintEvent>

#include "fonts/scface.h"
#include "charzoom.h"
#include "scpainter.h"


CharZoom::CharZoom(QWidget* parent, uint currentChar, ScFace face)
#ifdef Q_WS_MAC
	: QDialog( parent, Qt::FramelessWindowHint | Qt::Popup)
#else
	: QDialog( parent, Qt::FramelessWindowHint)
#endif
{
	int base = 48;
	int size = base + qRound(-face.descent() * base) + 3;
	int sizex = size + 2, sizey = size + 20;
	resize(sizex, sizey);
	setMinimumSize(sizex, sizey);
	setMaximumSize(sizex, sizey);
	
	pixm = QPixmap(size, size);
	QImage pix(size, size, QImage::Format_ARGB32);
	ScPainter *p = new ScPainter(&pix, size, size);
	p->clear();
	pixm.fill(Qt::white);
	QMatrix chma;
	chma.scale(4.8, 4.8);

	uint gl = face.char2CMap(currentChar);
	FPointArray gly = face.glyphOutline(gl);
	double ww = size - face.glyphWidth(gl, base);
	if (gly.size() > 4)
	{
		gly.map(chma);
		p->translate(ww / 2, 1);
		p->setBrush(Qt::black);
		p->setFillMode(1);
		p->setupPolygon(&gly);
		p->fillPath();
		p->end();
	}
	delete p;
	pixm=QPixmap::fromImage(pix);

	QString tmp;
	tmp.sprintf("%04X", currentChar);
	valu = "0x"+tmp;
}

void CharZoom::paintEvent(QPaintEvent *)
{
	QPainter p;
	p.begin(this);
	p.setPen(Qt::black);
	p.setBrush(Qt::NoBrush);
	p.drawRect(0, 0, width()-1, height()-1);
	p.drawPixmap(1, 1, pixm);
	p.drawText(5, height()-3, valu);
	p.end();
}
