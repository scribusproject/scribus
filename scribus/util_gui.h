#ifndef _UTIL_GUI_H
#define _UTIL_GUI_H

#include <QPainter>
#include <QPalette>
#include "scribusapi.h"

class VGradient;
class VColorStop;
class ScPattern;

void SCRIBUS_API drawCircularHandle(QPainter * painter, QPointF center, qreal width, QBrush background, bool isEnabled = true);
void SCRIBUS_API drawPointerHandle(QPainter * painter, QPointF pointer, qreal width, QColor background, bool isEnabled = true, bool isEmpty = false);
void SCRIBUS_API renderCheckerPattern(QPainter *painter, QRectF rect);
void SCRIBUS_API drawSelectionHandle(QPainter *painter, QPointF point, QPen pen, qreal scaleFactor, bool isActive = false);
void SCRIBUS_API drawNodeHandle(QPainter *painter, QPointF point, QPen pen, qreal scaleFactor, bool isActive = false);
void SCRIBUS_API drawNodeControl(QPainter *painter, QPointF point, QPen pen, qreal scaleFactor, bool isActive = false);
void SCRIBUS_API drawWeldMarker(QPainter *painter, QPointF point, QColor color, qreal scaleFactor);
QPixmap SCRIBUS_API renderEmptyPattern(QSize size);
QPixmap SCRIBUS_API renderColor(QSize size, QColor color, QColor colorShade, double alpha = 1.0);
QPixmap SCRIBUS_API renderGradientLinear(QSize size, const VGradient& gradient);
QPixmap SCRIBUS_API renderGradientRadial(QSize size, const VGradient& gradient);
QPixmap SCRIBUS_API renderGradientConical(QSize size, const VGradient& gradient);
QPixmap SCRIBUS_API renderGradient4Colors(QSize size, QColor col1, QColor col2, QColor col3, QColor col4);
QPixmap SCRIBUS_API renderGradientDiamond(QSize size, const VGradient& gradient);
QPixmap SCRIBUS_API renderGradientMesh(QSize size);
QPixmap SCRIBUS_API renderGradientPatchMesh(QSize size);
QPixmap SCRIBUS_API renderHatch(QSize size, int type, double distance, double angle, bool hasBackground, QColor backgroundColor, QColor foregroundColor);
QPixmap SCRIBUS_API renderPattern(QSize size, ScPattern pattern);

// Helper
VColorStop SCRIBUS_API computeInBetweenStop(const VColorStop* last, const VColorStop* actual, double t);
QColor SCRIBUS_API disabledColor(QColor color);
QColor SCRIBUS_API blendColor(double factor, QColor color1, QColor color2);
QColor SCRIBUS_API colorByRole(QPalette::ColorRole cr, double opacity, bool active);

#endif
