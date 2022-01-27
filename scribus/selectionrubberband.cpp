#include "selectionrubberband.h"

#include <QApplication>
#include <QPainter>
#include <QStyleOptionRubberBand>

SelectionRubberBand::SelectionRubberBand(QRubberBand::Shape s, QWidget *p)
	: QRubberBand(s,p)
{
}

SelectionRubberBand::~SelectionRubberBand()
{
}

void SelectionRubberBand::paintEvent(QPaintEvent *p)
{
	QColor drawColor = qApp->palette().color(QPalette::Active, QPalette::Highlight);
	QPen pen(drawColor, 2, Qt::DashLine, Qt::FlatCap, Qt::MiterJoin);
	pen.setStyle(Qt::DashLine);

	QPainter painter;
	painter.begin(this);
	painter.setPen(pen);
	drawColor.setAlpha(64);
	painter.setBrush(drawColor);
	painter.drawRect(p->rect());
	painter.end();
}
