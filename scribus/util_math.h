/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _UTIL_MATH_H
#define _UTIL_MATH_H

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <vector>

#include <QByteArray>
#include <QPolygon>
#include <QList>
#include <QPainterPath>
#include <QLineF>

#include "fpoint.h"
#include "scribusapi.h"

class FPoint;
class FPointArray;
class QRegion;

/*! \brief Compare double values by pre-multiplying by 10000 and converting to long if possible.
If premultiplication does not allow to store result in a long value, perform a standard comparison.
*/
bool SCRIBUS_API compareDouble(double a, double b);
uint SCRIBUS_API getDouble(const QByteArray& in, bool raw);
FPoint   SCRIBUS_API getMaxClipF(const FPointArray* clip);
FPoint   SCRIBUS_API getMinClipF(const FPointArray* clip);
FPoint   SCRIBUS_API projectPointOnLine(FPoint p, QPointF lineStart, QPointF lineEnd);
bool     SCRIBUS_API regionContainsRect(const QRegion& shape, QRect rect);
QPolygon SCRIBUS_API flattenPath(const FPointArray& ina, QList<uint> &segments);
QList<QPainterPath> SCRIBUS_API decomposePath(const QPainterPath &path);
/**
 * @brief CatmullToBezier
 * @param inputPoints Point Array of at least 4 points (p0, p1, p2, p3). You will get cubic bezier points from p1 to p2.
 * @param t
 * @return
 */
QList<QPointF> SCRIBUS_API catmullToBezier(const QList<QPointF>& inputPoints, double t = 1);
QPainterPath  SCRIBUS_API regularPolygonPath(double w, double h, uint c, bool star, double factor, double rota, double factor2 = 0.0, double innerRot = 0.0, double factor3 = 0.0);
QPainterPath  SCRIBUS_API spiralPath(double spiralWidth, double spiralHeight, double spiralStartAngle, double spiralEndAngle, double spiralFactor);
inline double SCRIBUS_API xy2Deg(double x, double y);
inline double SCRIBUS_API sind(double);
inline double SCRIBUS_API cosd(double);
inline double SCRIBUS_API square(double);
inline double SCRIBUS_API distance(double, double);
inline double SCRIBUS_API manhattanDistance(const QPointF& p1, const QPointF& p2);

/*! \brief Constrains an angle of rotation to 45 degree intervals
   Will make code simpler and reduce interval or provide as a parameter
   \param angle angle Angle in degrees
   \param constrain constrain value in degrees
   \retval double Constrained angle
 */
double SCRIBUS_API constrainAngle(double angle, double constrain);
inline FPoint SCRIBUS_API getPointFromRotation(const FPoint &p1, double distance, double angle);
/*! \brief Get the rotation angle (in radian) from a transformation matrix
   Will make code simpler and reduce interval or provide as a parameter
   \param matrix the transformation matrix
   \param def the value that should be return if matrix is not a rotation matrix
   \retval double the rotation angle
 */
double SCRIBUS_API getRotationFromMatrix(const QTransform& matrix, double def);
/*! \brief Get the rotation angle (in degree) from a transformation matrix
   \param matrix the transformation matrix
   \retval double the rotation angle
 */
double SCRIBUS_API getRotationDFromMatrix(const QTransform& matrix);
void SCRIBUS_API getScaleFromMatrix(const QTransform &matrix, double &scX, double &scY);
void SCRIBUS_API getTransformValuesFromMatrix(const QTransform &matrix, double &scX, double &scY, double &rot, double &dx, double &dy);


// IMPLEMENTATION

inline double square(double x)
{
	return x*x;
}

inline double distance(double x, double y)
{
	return sqrt(x*x + y*y);
}

inline double manhattanDistance(const QPointF& p1, const QPointF& p2)
{
	QPointF point = p2 - p1;
	return point.manhattanLength();
}

inline FPoint getPointFromRotation(const FPoint &p1, double distance, double angle)
{
	FPoint p2;
	p2.setX(p1.x() + cosd(angle) * distance);
	p2.setY(p1.y() + sind(angle) * distance);
	return p2;
}

inline double xy2Deg(double x, double y)
{
	return atan2(y,x) * (180.0/M_PI);
}

inline double sind(double alpha)
{
	return sin(alpha / (180.0/M_PI));
}

inline double cosd(double alpha)
{
	return cos(alpha / (180.0/M_PI));
}

template <typename T> 
inline bool isequiv(const T& v1, const T& v2) { return v1 == v2; }

template <> 
inline bool isequiv<double>(const double& v1, const double& v2) { return compareDouble(v1, v2); }

#endif
