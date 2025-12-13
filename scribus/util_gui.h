#ifndef _UTIL_GUI_H
#define _UTIL_GUI_H

#include <QPainter>
#include <QPalette>
#include "scribusapi.h"

class VGradient;
class VColorStop;
class ScPattern;

const int RADIUS = 6;

void SCRIBUS_API drawCircularHandle(QPainter * painter, QPointF center, qreal width, QBrush background, bool isEnabled = true);
void SCRIBUS_API drawPointerHandle(QPainter * painter, QPointF pointer, qreal width, QColor background, bool isEnabled = true, bool isEmpty = false);
void SCRIBUS_API renderCheckerPattern(QPainter *painter, QRectF rect);
void SCRIBUS_API drawSelectionHandle(QPainter *painter, QPointF point, QPen pen, qreal scaleFactor, bool isActive = false);
void SCRIBUS_API drawNodeHandle(QPainter *painter, QPointF point, QPen pen, qreal scaleFactor, bool isActive = false);
void SCRIBUS_API drawNodeControl(QPainter *painter, QPointF point, QPen pen, qreal scaleFactor, bool isActive = false);
void SCRIBUS_API drawWeldMarker(QPainter *painter, QPointF point, QColor color, qreal scaleFactor);
void SCRIBUS_API drawColorBox(QPainter * painter, QRect rect, QColor color, bool isEnabled = true);
QPixmap SCRIBUS_API renderEmptyPattern(QSize size, double scale);
QPixmap SCRIBUS_API renderColor(QSize size, double scale, QColor color, QColor colorShade, double alpha = 1.0);
QPixmap SCRIBUS_API renderGradientLinear(QSize size, double scale, const VGradient& gradient);
QPixmap SCRIBUS_API renderGradientRadial(QSize size, double scale, const VGradient& gradient);
QPixmap SCRIBUS_API renderGradientConical(QSize size, double scale, const VGradient& gradient);
QPixmap SCRIBUS_API renderGradient4Colors(QSize size, double scale, QColor col1, QColor col2, QColor col3, QColor col4);
QPixmap SCRIBUS_API renderGradientDiamond(QSize size, double scale, const VGradient& gradient);
QPixmap SCRIBUS_API renderGradientMesh(QSize size, double scale);
QPixmap SCRIBUS_API renderGradientPatchMesh(QSize size, double scale);
QPixmap SCRIBUS_API renderHatch(QSize size, double scale, int type, double distance, double angle, bool hasBackground, QColor backgroundColor, QColor foregroundColor);
QPixmap SCRIBUS_API renderPattern(QSize size, double scale, const ScPattern& pattern);
QPixmap SCRIBUS_API combinePixmaps(const QPixmap& background, const QPixmap& foreground, double scale, bool tintForeground, bool isDarkColor);
bool SCRIBUS_API isDarkColor(QColor color);

// Helper
VColorStop SCRIBUS_API computeInBetweenStop(const VColorStop* last, const VColorStop* actual, double t);
QColor SCRIBUS_API disabledColor(QColor color);
QColor SCRIBUS_API blendColor(double factor, QColor color1, QColor color2);
QColor SCRIBUS_API colorByRole(QPalette::ColorRole cr, double opacity, bool active);

#endif
