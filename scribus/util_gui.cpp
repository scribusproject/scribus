#include "util_gui.h"

#include <QPainterPath>

void renderCircularHandle(QPainter *painter, QPointF center, qreal width, QBrush background)
{
	qreal radius = width /2.;

	painter->save();

	// Draw background
	painter->setBrush(background);
	painter->setPen(Qt::NoPen);
	painter->drawEllipse(center, radius, radius);

	painter->setBrush(Qt::NoBrush);

	// Draw white ring
	painter->setPen(QPen(Qt::white, 2));
	painter->drawEllipse(center, radius-1, radius-1);

	// Draw black ring
	painter->setPen(QPen(Qt::black, 1));
	painter->drawEllipse(center, radius, radius);

	painter->restore();
}

void renderPointerHandle(QPainter *painter, QPointF pointer, qreal width, QColor background)
{
	qreal height = width *1.5;
	QRect rect( pointer.x() -width/2, pointer.y() , width, height );

	QPainterPath path;
	path.moveTo(pointer);
	path.lineTo(pointer + QPoint(width/2, height/3) );
	path.lineTo(pointer + QPoint(width/2, height) );
	path.lineTo(pointer + QPoint(-width/2, height) );
	path.lineTo(pointer + QPoint(-width/2, height/3) );
	path.closeSubpath();

	painter->save();

	// Draw white border
	painter->setPen(QPen(Qt::black, 3));
	painter->setBrush(Qt::NoBrush);
	painter->drawPath(path);

	// Draw background
	painter->setClipPath(path);
	painter->setPen(Qt::NoPen);
	renderCheckerPattern(painter, rect);

	// Draw color
	painter->drawPixmap(rect, renderSplitColor(rect.size(), background, Qt::Vertical, 0.7) );

	// Draw black border
	painter->setBrush(Qt::NoBrush);
	painter->setClipping(false);
	painter->setPen(QPen(Qt::white, 1.5));
	painter->drawPath(path);

	painter->restore();
}

void renderCheckerPattern(QPainter *painter, QRectF rect)
{
	QImage pattern(rect.width(), rect.height(), QImage::Format_ARGB32_Premultiplied);

	QPainter p(&pattern);
	p.fillRect(pattern.rect(), QBrush(Qt::white));

	int cellSize = 4;
	int cellSizeX = rect.width() / cellSize + cellSize;
	int cellSizeY = rect.height() / cellSize + cellSize;

	for(int y = 0; y < cellSizeY; y++)
		for(int x = y % 2; x < cellSizeX; x += 2)
			p.fillRect(QRectF(x * cellSize, y * cellSize, cellSize, cellSize), QBrush(Qt::lightGray));

	painter->drawImage(rect, pattern);
}

QPixmap renderSplitColor(QSize size, QColor color, Qt::Orientations orientation, qreal mid)
{
	mid = qBound(0., mid, 1.);

	QPixmap pixmap(size.width(), size.height());
	pixmap.fill(Qt::transparent);

	QRectF solid( 0, 0, pixmap.width()*mid, pixmap.height() );
	QRectF alpha( pixmap.width()*mid, 0, pixmap.width()*(1-mid), pixmap.height() );

	if(orientation == Qt::Vertical){
		solid = QRectF(0, 0, pixmap.width(), pixmap.height() *mid);
		alpha = QRectF(0, pixmap.height() *mid, pixmap.width(), pixmap.height() * (1-mid) );
	}

	QColor col = color;
	col.setAlphaF(1);

	QPainter p(&pixmap);
	p.setBrush(QBrush(col));
	p.setPen(Qt::NoPen);
	p.drawRect(solid);
	p.setBrush(QBrush(color));
	p.drawRect(alpha);

	return pixmap;
}

QPixmap renderEmptyPattern(QSize size)
{
	QPixmap pixmap(size.width(), size.height());
	pixmap.fill(Qt::white);

	QPainter p(&pixmap);
	p.setPen(QPen(QBrush(Qt::red), 2));
	p.drawLine(QPoint(0, size.width()), QPoint(size.width(), 0));

	return pixmap;
}
