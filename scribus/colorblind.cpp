/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "colorblind.h"
#include "colorblind.moc"

#include <math.h>

//#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))


VisionDefectColor::VisionDefectColor(int r, int g, int b)
{
	Qt::red = (double)r;
	Qt::green = (double)g;
	Qt::blue = (double)b;
	originalColor = QColor(r, g, b);
	init();
}

VisionDefectColor::VisionDefectColor(QColor c)
{
	Qt::red = (double)c.Qt::red();
	Qt::green = (double)c.Qt::green();
	Qt::blue = (double)c.Qt::blue();
	originalColor = c;
	init();
}

VisionDefectColor::VisionDefectColor()
{
	red = 0.0;
	green = 0.0;
	blue = 0.0;
	originalColor = QColor(0, 0, 0);
	init();
}

void VisionDefectColor::init()
{
	rgb2lms[0] = 0.05059983;
	rgb2lms[1] = 0.08585369;
	rgb2lms[2] = 0.00952420;

	rgb2lms[3] = 0.01893033;
	rgb2lms[4] = 0.08925308;
	rgb2lms[5] = 0.01370054;

	rgb2lms[6] = 0.00292202;
	rgb2lms[7] = 0.00975732;
	rgb2lms[8] = 0.07145979;

	lms2rgb[0] =	30.830854;
	lms2rgb[1] = -29.832659;
	lms2rgb[2] =	 1.610474;

	lms2rgb[3] =	-6.481468;
	lms2rgb[4] =	17.715578;
	lms2rgb[5] =	-2.532642;

	lms2rgb[6] =	-0.375690;
	lms2rgb[7] =	-1.199062;
	lms2rgb[8] =	14.273846;

	gammaRGB[0] = 2.1;
	gammaRGB[1] = 2.0;
	gammaRGB[2] = 2.1;
}

void VisionDefectColor::convertDefect()
{
	double tmp;

	/* Remove gamma to linearize RGB intensities */
	Qt::red   = pow(Qt::red, 1.0 / gammaRGB[0]);
	Qt::green = pow(Qt::green, 1.0 / gammaRGB[1]);
	Qt::blue  = pow(Qt::blue, 1.0 / gammaRGB[2]);

	/* Convert to LMS (dot product with transform matrix) */
	double redOld   = Qt::red;
	double greenOld = Qt::green;

	Qt::red   = redOld * rgb2lms[0] + greenOld * rgb2lms[1] + Qt::blue * rgb2lms[2];
	Qt::green = redOld * rgb2lms[3] + greenOld * rgb2lms[4] + Qt::blue * rgb2lms[5];
	Qt::blue  = redOld * rgb2lms[6] + greenOld * rgb2lms[7] + Qt::blue * rgb2lms[8];

	switch (deficiency)
	{
		case normalVision:
			break;
		case deuteranopeVision:
			setupDefect();
			tmp = Qt::blue / Qt::red;
			/* See which side of the inflection line we fall... */
			if (tmp < inflection)
				Qt::green = -(a1 * Qt::red + c1 * Qt::blue) / b1;
			else
				Qt::green = -(a2 * Qt::red + c2 * Qt::blue) / b2;
			break;
		case protanopeVision:
			setupDefect();
			tmp = Qt::blue / Qt::green;
			/* See which side of the inflection line we fall... */
			if (tmp < inflection)
				Qt::red = -(b1 * Qt::green + c1 * Qt::blue) / a1;
			else
				Qt::red = -(b2 * Qt::green + c2 * Qt::blue) / a2;
			break;
		case tritanopeVision:
			setupDefect();
			tmp = Qt::green / Qt::red;
			/* See which side of the inflection line we fall... */
			if (tmp < inflection)
				Qt::blue = -(a1 * Qt::red + b1 * Qt::green) / c1;
			else
				Qt::blue = -(a2 * Qt::red + b2 * Qt::green) / c2;
			break;
		case colorBlindnessVision:
		{
			double Qt::gray = clamp(0.3 * originalColor.Qt::red()
						+ 0.59 * originalColor.Qt::green()
						+ 0.11 * originalColor.Qt::blue(), 0, 255);
			Qt::red = Qt::gray;
			Qt::green = Qt::gray;
			Qt::blue = Qt::gray;
			return; // no other transformations!
		}
		default:
			break;
	}

	/* Convert back to RGB (cross product with transform matrix) */
	redOld   = Qt::red;
	greenOld = Qt::green;

	Qt::red   = redOld * lms2rgb[0] + greenOld * lms2rgb[1] + Qt::blue * lms2rgb[2];
	Qt::green = redOld * lms2rgb[3] + greenOld * lms2rgb[4] + Qt::blue * lms2rgb[5];
	Qt::blue  = redOld * lms2rgb[6] + greenOld * lms2rgb[7] + Qt::blue * lms2rgb[8];

	/* Apply gamma to go back to non-linear intensities */
	Qt::red   = pow(Qt::red, gammaRGB[0]);
	Qt::green = pow(Qt::green, gammaRGB[1]);
	Qt::blue  = pow(Qt::blue, gammaRGB[2]);

	/* Ensure that we stay within the RGB gamut */
	/* *** FIX THIS: it would be better to desaturate than blindly clip. */
	Qt::red   = clamp(Qt::red, 0.0, 255.0);
	Qt::green = clamp(Qt::green, 0.0, 255.0);
	Qt::blue  = clamp(Qt::blue, 0.0, 255.0);
}

QColor VisionDefectColor::convertDefect(QColor c, int d)
{
	red = (double)c.red();
	green = (double)c.green();
	blue = (double)c.blue();
	originalColor = c;
	init();
	deficiency = d;
	convertDefect();
	return getColor();
}

void VisionDefectColor::setupDefect()
{
	double anchor_e[3];
	double anchor[12];

	/*
	Load the LMS anchor-point values for lambda = 475 & 485 nm (for
	protans & deutans) and the LMS values for lambda = 575 & 660 nm
	(for tritans)
	*/
	anchor[0] = 0.08008;  anchor[1]  = 0.1579;    anchor[2]  = 0.5897;
	anchor[3] = 0.1284;   anchor[4]  = 0.2237;    anchor[5]  = 0.3636;
	anchor[6] = 0.9856;   anchor[7]  = 0.7325;    anchor[8]  = 0.001079;
	anchor[9] = 0.0914;   anchor[10] = 0.007009;  anchor[11] = 0.0;

	/* We also need LMS for RGB=(1,1,1)- the equal-energy point (one of
	* our anchors) (we can just peel this out of the rgb2lms transform
	* matrix)
	*/
	anchor_e[0] = rgb2lms[0] + rgb2lms[1] + rgb2lms[2];
	anchor_e[1] = rgb2lms[3] + rgb2lms[4] + rgb2lms[5];
	anchor_e[2] = rgb2lms[6] + rgb2lms[7] + rgb2lms[8];

	switch (deficiency)
	{
		case deuteranopeVision:
			/* find a,b,c for lam=575nm and lam=475 */
			a1 = anchor_e[1] * anchor[8] - anchor_e[2] * anchor[7];
			b1 = anchor_e[2] * anchor[6] - anchor_e[0] * anchor[8];
			c1 = anchor_e[0] * anchor[7] - anchor_e[1] * anchor[6];
			a2 = anchor_e[1] * anchor[2] - anchor_e[2] * anchor[1];
			b2 = anchor_e[2] * anchor[0] - anchor_e[0] * anchor[2];
			c2 = anchor_e[0] * anchor[1] - anchor_e[1] * anchor[0];
			inflection = (anchor_e[2] / anchor_e[0]);
			break;
		case protanopeVision:
			/* find a,b,c for lam=575nm and lam=475 */
			a1 = anchor_e[1] * anchor[8] - anchor_e[2] * anchor[7];
			b1 = anchor_e[2] * anchor[6] - anchor_e[0] * anchor[8];
			c1 = anchor_e[0] * anchor[7] - anchor_e[1] * anchor[6];
			a2 = anchor_e[1] * anchor[2] - anchor_e[2] * anchor[1];
			b2 = anchor_e[2] * anchor[0] - anchor_e[0] * anchor[2];
			c2 = anchor_e[0] * anchor[1] - anchor_e[1] * anchor[0];
			inflection = (anchor_e[2] / anchor_e[1]);
			break;
		case tritanopeVision:
			/* Set 1: regions where lambda_a=575, set 2: lambda_a=475 */
			a1 = anchor_e[1] * anchor[11] - anchor_e[2] * anchor[10];
			b1 = anchor_e[2] * anchor[9]  - anchor_e[0] * anchor[11];
			c1 = anchor_e[0] * anchor[10] - anchor_e[1] * anchor[9];
			a2 = anchor_e[1] * anchor[5]  - anchor_e[2] * anchor[4];
			b2 = anchor_e[2] * anchor[3]  - anchor_e[0] * anchor[5];
			c2 = anchor_e[0] * anchor[4]  - anchor_e[1] * anchor[3];
			inflection = (anchor_e[1] / anchor_e[0]);
			break;
	}
}

uint VisionDefectColor::getRed()
{
	return (uint)red;
}

uint VisionDefectColor::getGreen()
{
	return (uint)green;
}

uint VisionDefectColor::getBlue()
{
	return (uint)blue;
}

QColor VisionDefectColor::getColor()
{
	return QColor(getRed(), getGreen(), getBlue());
}

double VisionDefectColor::clamp(double x, double low, double high)
{
	double ret;
	(x > high) ? ret = high : ((x < low) ? ret = low : ret = x);
	return ret;
}
