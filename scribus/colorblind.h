/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef COLORBLIND_H
#define COLORBLIND_H

#include <QVariant>
#include <QColor>
#include <QObject>

/*! \brief This code is an implementation of an algorithm described by Hans Brettel,
Francoise Vienot and John Mollon in the Journal of the Optical Society of
America V14(10), pg 2647. (See http://vischeck.com/ for more info.).

Based on the GIMP's cdisplay_colorblind.c
by Michael Natterer <mitch@gimp.org>, Sven Neumann <sven@gimp.org>,
Robert Dougherty <bob@vischeck.com> and Alex Wade <alex@vischeck.com>.

This code is written using "Scribus coding standard" as a part of the
Scribus project (www.scribus.net).

\author Petr Vanek <petr@scribus.info>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "scribusapi.h"

class SCRIBUS_API VisionDefectColor : public QObject, public QColor
{
	Q_OBJECT
public:
	/*! \brief Enumerate Vision Defects for readable ComboBox access */
	enum defectMode {
		normalVision = 0,
		protanopeVision = 1,
		deuteranopeVision = 2,
		tritanopeVision = 3,
		colorBlindnessVision = 4
	};

	/*! \brief QColor like constructors. Setup the RGB parts, then init().
	\param r red part of the color
	\param g green part of the color
	\param b blue part of the color
	*/
	VisionDefectColor(int r, int g, int b);
	/*! \brief QColor like constructors. Setup the RGB parts, then init().
	\param c QColor object
	 */
	VisionDefectColor(QColor c);

	/*! \brief Default constructors. Setup the RGB parts to default values, then init().
	 */
	VisionDefectColor();

	/*! \brief Actual defect type. One of the 'defectMode' */
	int deficiency;

	/*! \brief Perform conversion itself.
	Gama removing, RGB2LSM, matrix operation for selected deficiency,
	LSM2RGB, gama applying. */
	void convertDefect();

	/*! \brief Perform conversion itself. Confidence overload to perform the conversion in one step;
	\param c QColor object
	\param d defect type
	\retval QColor converted color
	Gama removing, RGB2LSM, matrix operation for selected deficiency,
	LSM2RGB, gama applying. */
	QColor convertDefect(QColor c, int d);

	/*! \brief Return the color parts
	\retval uint red part
	*/
	uint getRed();
	/*! \brief Return the color parts
	\retval uint green part
	*/
	uint getGreen();
	/*! \brief Return the color parts
	\retval uint blue part
	*/
	uint getBlue();
	/*! \brief Return QColor
	\retval QColor new QColor
	*/
	QColor getColor();

private:
	/*! \brief Convert matrix */
	double rgb2lms[9];
	/*! \brief Convert matrix */
	double lms2rgb[9];
	/*! \brief Gamma matrix */
	double gammaRGB[3];

	/*! \brief cross products with transform matrixes for actual defect */
	double a1, b1, c1;
	/*! \brief cross products with transform matrixes for actual defect */
	double a2, b2, c2;
	/*! \brief Inflection border line */
	double inflection;

	/*! \brief Internal RGB color parts */
	double red, green, blue;

	/*! \brief original color */
	QColor originalColor;

	/*! \brief Initialization of the internal values. rgb2lms, lms2rgb etc.
	For most modern Cathode-Ray Tube monitors (CRTs), the following
	are good estimates of the RGB->LMS and LMS->RGB transform
	matrices.  They are based on spectra measured on a typical CRT
	with a PhotoResearch PR650 spectral photometer and the Stockman
	human cone fundamentals. NOTE: these estimates will NOT work well
	for LCDs!
	The RGB<->LMS transforms are computed from the human cone
	photo-pigment absorption spectra and the monitor phosphor
	emission spectra. These parameters are fairly constant for most
	humans and most montiors (at least for modern CRTs). However,
	gamma will vary quite a bit, as it is a property of the monitor
	(eg. amplifier gain), the video card, and even the
	software. Further, users can adjust their gammas (either via
	adjusting the monitor amp gains or in software). That said, the
	following are the gamma estimates that we have used in the
	Vischeck code. Many colorblind users have viewed our simulations
	and told us that they "work" (simulated and original images are
	indistinguishabled).
	*/
	void init();

	/*! \brief This function performs initialisations that are dependant
	on the type of color deficiency (see defectMode). */
	void setupDefect();

	/*! \brief Ensures that x is between the limits set by low and high.
	Glib CLAMP() macro replacement for C++ speedup optimalization.
	\param x value itself
	\param low low border
	\param high high border
	\retval double value from interval
	*/
	double clamp(double x, double low, double high);

};

#endif
