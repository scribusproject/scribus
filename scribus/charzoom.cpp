#include <qpixmap.h>
#include <qpainter.h>

#include "charzoom.h"
#include "charzoom.moc"


#ifdef QT_MAC
CharZoom::CharZoom(QWidget* parent, QPixmap pix, uint val)
	: QDialog( parent, "CharZoom", false, WStyle_Customize | WStyle_NoBorder | WType_Popup)
#else
CharZoom::CharZoom(QWidget* parent, QPixmap pix, uint val)
	: QDialog( parent, "CharZoom", false, WStyle_Customize | WStyle_NoBorder)
#endif
{
	int newwidth=pix.width()+2;
	int newheight=pix.height()+20;
	resize(newwidth,newheight);
	setMinimumSize(QSize(newwidth,newheight));
	setMaximumSize(QSize(newwidth,newheight));
	pixm = pix;
	QString tmp;
	tmp.sprintf("%04X", val);
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
