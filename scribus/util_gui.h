#ifndef _UTIL_GUI_H
#define _UTIL_GUI_H

#include <QPainter>
#include "scribusapi.h"

void SCRIBUS_API renderCircularHandle(QPainter * painter, QPointF center, qreal width, QBrush background);
void SCRIBUS_API renderPointerHandle(QPainter * painter, QPointF pointer, qreal width, QColor background);
void SCRIBUS_API renderCheckerPattern(QPainter *painter, QRectF rect);
QPixmap SCRIBUS_API renderEmptyPattern(QSize size);
QPixmap SCRIBUS_API renderSplitColor(QSize size, QColor color, Qt::Orientations orientation = Qt::Horizontal, qreal mid = 0.5);

#endif
