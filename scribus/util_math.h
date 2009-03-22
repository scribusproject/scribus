/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _UTIL_MATH_H
#define _UTIL_MATH_H

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <vector>

#include <QString>
#include <QPolygon>
#include <QList>
#include <QPainterPath>

#include "scribusapi.h"

class FPoint;
class FPointArray;


/*! \brief Compare double values by pre-multiplying by 10000 and converting to long if possible.
If premultiplication does not allow to store result in a long value, perform a standard comparison.
*/
bool SCRIBUS_API compareDouble(double a, double b);
FPoint SCRIBUS_API getMaxClipF(FPointArray* Clip);
FPoint SCRIBUS_API getMinClipF(FPointArray* Clip);
inline double SCRIBUS_API xy2Deg(double x, double y);
FPoint SCRIBUS_API projectPointOnLine(FPoint p, QPointF lineStart, QPointF lineEnd);
QPolygon SCRIBUS_API FlattenPath(const FPointArray& ina, QList<uint> &Segs);
QList<QPainterPath> SCRIBUS_API decomposePath(QPainterPath &path);
QPainterPath SCRIBUS_API RegularPolygon(double w, double h, uint c, bool star, double factor, double rota, double factor2 = 0.0);
uint SCRIBUS_API getDouble(QString in, bool raw);
inline double SCRIBUS_API sind(double);
inline double SCRIBUS_API cosd(double);
inline double SCRIBUS_API square(double);
inline double SCRIBUS_API distance(double, double);
/*! \brief Constrains an angle of rotation to 45 degree intervals
   Will make code simpler and reduce interval or provide as a parameter
   \param angle angle Angle in degrees
   \param constrain contrain value in degrees
   \retval double Constrained angle
 */
double SCRIBUS_API constrainAngle(double angle, double constrain);
/*! \brief Get the rotation angle (in radian) from a transformation matrix
   Will make code simpler and reduce interval or provide as a parameter
   \param matrix the transformation matrix
   \param def the value that should be return if matrix is not a rotation matrix
   \retval double the rotation angle
 */
double SCRIBUS_API getRotationFromMatrix(QMatrix& matrix, double def);


// IMPLEMENTATION

inline double square(double x)
{
	return x*x;
}

inline double distance(double x, double y)
{
	return sqrt(x*x + y*y);
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
