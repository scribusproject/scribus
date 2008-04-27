/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _UTIL_COLOR_H
#define _UTIL_COLOR_H

#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QString>

#include "fpointarray.h"
#include "sccolor.h"
#include "scribusapi.h"

class ScribusDoc;

QColor SCRIBUS_API SetColor(ScribusDoc *currentDoc, QString color, int shad);

/*! \brief Create a cool all-infos pixmaps for the specified color.
\param col Scribus color
\retval QPixmap image with various icons depending on the col properties.
 */
QPixmap SCRIBUS_API *getSmallPixmap(QColor rgb);
QPixmap SCRIBUS_API *getWidePixmap(QColor rgb);
QPixmap SCRIBUS_API *getFancyPixmap(const ScColor& col, ScribusDoc* doc);
/*! \brief Put toPaint pixmap into target at the x, y place.
There is handled the alpha channel/transparency too. In the beginning
is the target pixmap filled with all_transparent mask. In the case of
the painting of tiPaint into x, y place, there is this pixmap enabled
in alpha mask too.
\param toPaint a pixmap to be painted into target
\param target a base pixmap. Some kind of painter.
\param x coordinate
\param y coordinate
 */
void SCRIBUS_API paintAlert(QPixmap &toPaint, QPixmap &target, int x = 0, int y = 0, bool useMask = true);
QImage SCRIBUS_API ProofImage(QImage *Im, ScribusDoc* doc);

/*! \brief Associate a color shade to a document color according to pre-1.3.4cvs rgb shade calculation method, 
     creating new document color if necessary
 * \param doc the scribus document
 * \param colName the color name (modified in place)
 * \param shade the color level (modified in place)
 */
void SCRIBUS_API handleOldColorShade(ScribusDoc* doc, QString& colName, int& shade);

/*! \brief Compute color shade of a rgb color using pre-1.3.4 method
 * \param color the base color
 * \param shade shade value.
 */
QColor SCRIBUS_API getOldColorShade(const QColor& color, int shade);
/*! \brief Compute color shade of a rgb color using pre-1.3.4 method
 * \param red the red component.
 * \param green the green component.
 * \param blue the blue component.
 * \param shade shade value.
 */
QColor SCRIBUS_API getOldColorShade(uchar red, uchar green, uchar blue, int shade);
/*! \brief Convert a color in RGB space to HSV space (Hue, Saturation, Value).
 * \param red the red component (modified in place).
 * \param green the green component (modified in place).
 * \param blue the blue component (modified in place).
 */
unsigned char SCRIBUS_API INT_MULT ( unsigned char a, unsigned char b );
/*! \brief Convert a color in HSV space to RGB space.
 * \param hue the hue component (modified in place).
 * \param saturation the saturation component (modified in place).
 * \param value the value component (modified in place).
 */
void SCRIBUS_API RGBTOHSV ( uchar& red, uchar& green, uchar& blue );
/*! \brief Convert a color in RGB space to HLS space (Hue, Lightness, Saturation).
 * \param red the red component (modified in place).
 * \param green the green component (modified in place).
 * \param blue the blue component (modified in place).
 */
void SCRIBUS_API HSVTORGB ( uchar& hue, uchar& saturation, uchar& value );
void SCRIBUS_API RGBTOHLS ( uchar& red, uchar& green, uchar& blue );
/*! \brief Implement the HLS "double hex-cone".
 * \param n1 lightness fraction (?)
 * \param n2 saturation fraction (?)
 * \param hue hue "angle".
 * \return HLS value.
 */
double SCRIBUS_API HLSVALUE ( double n1, double n2, double hue );
/*! \brief Convert a color in HLS space to RGB space.
 * \param hue the hue component (modified in place).
 * \param lightness the lightness component (modified in place).
 * \param saturation the saturation component (modified in place).
 */
void SCRIBUS_API HLSTORGB ( uchar& hue, uchar& lightness, uchar& saturation );

double SCRIBUS_API getCurveYValue(FPointArray &curve, double x, bool linear = false);

double SCRIBUS_API Lum(uchar red, uchar green, uchar blue);
double SCRIBUS_API LumD(double red, double green, double blue);
void SCRIBUS_API setLum(uchar& red, uchar& green, uchar& blue, double lum);
void SCRIBUS_API clipColor(double& red, double& green, double& blue);
/*! \brief return the translated colorspace name */
QString SCRIBUS_API colorSpaceText(int cs);

/*! \brief loads colors from EPS + AI and SLA files, reads also Scribus swatch files 
 * \param fileName the file to import
 * \param EditColors Color list to store the imported colors
 * \return true if any colors have been found
*/
bool SCRIBUS_API importColorsFromFile(QString fileName, ColorList &EditColors);

#endif
