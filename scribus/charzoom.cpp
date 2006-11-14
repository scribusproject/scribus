#include <qpixmap.h>
#include <qpainter.h>

#include "charzoom.h"
#include "charzoom.moc"
#include "scpainter.h"


CharZoom::CharZoom(QWidget* parent, uint currentChar, ScFace face)
#ifdef QT_MAC
	: QDialog( parent, "CharZoom", false, WStyle_Customize | WStyle_NoBorder | WType_Popup)
#else
	: QDialog( parent, "CharZoom", false, WStyle_Customize | WStyle_NoBorder)
#endif
{
	int base = 48;
	int size = base + qRound(-face.descent() * base) + 3;
	int sizex = size + 2, sizey = size + 20;
	resize(sizex, sizey);
	setMinimumSize(sizex, sizey);
	setMaximumSize(sizex, sizey);
	
	pixm.resize(size, size);
	ScPainter *p = new ScPainter(&pixm, size, size);
	p->clear();
	pixm.fill(white);
	QWMatrix chma;
	chma.scale(4.8, 4.8);

	uint gl = face.char2CMap(currentChar);
	FPointArray gly = face.glyphOutline(gl);
	double ww = size - face.glyphWidth(gl, base);
	if (gly.size() > 4)
	{
		gly.map(chma);
		p->translate(ww / 2, 1);
		p->setBrush(black);
		p->setFillMode(1);
		p->setupPolygon(&gly);
		p->fillPath();
		p->end();
	}
	delete p;

	QString tmp;
	tmp.sprintf("%04X", currentChar);
	valu = "0x"+tmp;
}

void CharZoom::paintEvent(QPaintEvent *)
{
	QPainter p;
	p.begin(this);
	p.setPen(black);
	p.setBrush(NoBrush);
	p.drawRect(0, 0, width(), height());
	p.drawPixmap(1, 1, pixm);
	p.drawText(5, height()-3, valu);
	p.end();
}
