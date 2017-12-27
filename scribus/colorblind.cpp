/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "colorblind.h"
//#include "colorblind.moc"

#include <math.h>

//#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))


VisionDefectColor::VisionDefectColor(int r, int g, int b)
{
	m_red = (double)r;
	m_green = (double)g;
	m_blue = (double)b;
	m_originalColor = QColor(r, g, b);
	init();
}

VisionDefectColor::VisionDefectColor(QColor c)
{
	m_red = (double)c.red();
	m_green = (double)c.green();
	m_blue = (double)c.blue();
	m_originalColor = c;
	init();
}

VisionDefectColor::VisionDefectColor()
{
	m_red = 0.0;
	m_green = 0.0;
	m_blue = 0.0;
	m_originalColor = QColor(0, 0, 0);
	init();
}

void VisionDefectColor::init()
{
	deficiency = 0;
	m_a1 = m_a2 = m_b1 = m_b2 = m_c1 = m_c2 = 0.0;
	m_inflection = 0.0;

	m_rgb2lms[0] = 0.05059983;
	m_rgb2lms[1] = 0.08585369;
	m_rgb2lms[2] = 0.00952420;

	m_rgb2lms[3] = 0.01893033;
	m_rgb2lms[4] = 0.08925308;
	m_rgb2lms[5] = 0.01370054;

	m_rgb2lms[6] = 0.00292202;
	m_rgb2lms[7] = 0.00975732;
	m_rgb2lms[8] = 0.07145979;

	m_lms2rgb[0] =	30.830854;
	m_lms2rgb[1] = -29.832659;
	m_lms2rgb[2] =	 1.610474;

	m_lms2rgb[3] =	-6.481468;
	m_lms2rgb[4] =	17.715578;
	m_lms2rgb[5] =	-2.532642;

	m_lms2rgb[6] =	-0.375690;
	m_lms2rgb[7] =	-1.199062;
	m_lms2rgb[8] =	14.273846;

	m_gammaRGB[0] = 2.1;
	m_gammaRGB[1] = 2.0;
	m_gammaRGB[2] = 2.1;
}

void VisionDefectColor::convertDefect()
{
	double tmp;

	/* Remove gamma to linearize RGB intensities */
	m_red   = pow(m_red, 1.0 / m_gammaRGB[0]);
	m_green = pow(m_green, 1.0 / m_gammaRGB[1]);
	m_blue  = pow(m_blue, 1.0 / m_gammaRGB[2]);

	/* Convert to LMS (dot product with transform matrix) */
	double redOld   = m_red;
	double greenOld = m_green;

	m_red   = redOld * m_rgb2lms[0] + greenOld * m_rgb2lms[1] + m_blue * m_rgb2lms[2];
	m_green = redOld * m_rgb2lms[3] + greenOld * m_rgb2lms[4] + m_blue * m_rgb2lms[5];
	m_blue  = redOld * m_rgb2lms[6] + greenOld * m_rgb2lms[7] + m_blue * m_rgb2lms[8];

	switch (deficiency)
	{
		case normalVision:
			break;
		case deuteranopeVision:
			setupDefect();
			tmp = m_blue / m_red;
			/* See which side of the inflection line we fall... */
			if (tmp < m_inflection)
				m_green = -(m_a1 * m_red + m_c1 * m_blue) / m_b1;
			else
				m_green = -(m_a2 * m_red + m_c2 * m_blue) / m_b2;
			break;
		case protanopeVision:
			setupDefect();
			tmp = m_blue / m_green;
			/* See which side of the inflection line we fall... */
			if (tmp < m_inflection)
				m_red = -(m_b1 * m_green + m_c1 * m_blue) / m_a1;
			else
				m_red = -(m_b2 * m_green + m_c2 * m_blue) / m_a2;
			break;
		case tritanopeVision:
			setupDefect();
			tmp = m_green / m_red;
			/* See which side of the inflection line we fall... */
			if (tmp < m_inflection)
				m_blue = -(m_a1 * m_red + m_b1 * m_green) / m_c1;
			else
				m_blue = -(m_a2 * m_red + m_b2 * m_green) / m_c2;
			break;
		case colorBlindnessVision:
		{
			double gray = clamp(0.3 * m_originalColor.red()
						+ 0.59 * m_originalColor.green()
						+ 0.11 * m_originalColor.blue(), 0, 255);
			m_red = gray;
			m_green = gray;
			m_blue = gray;
			return; // no other transformations!
		}
		default:
			break;
	}

	/* Convert back to RGB (cross product with transform matrix) */
	redOld   = m_red;
	greenOld = m_green;

	m_red   = redOld * m_lms2rgb[0] + greenOld * m_lms2rgb[1] + m_blue * m_lms2rgb[2];
	m_green = redOld * m_lms2rgb[3] + greenOld * m_lms2rgb[4] + m_blue * m_lms2rgb[5];
	m_blue  = redOld * m_lms2rgb[6] + greenOld * m_lms2rgb[7] + m_blue * m_lms2rgb[8];

	/* Apply gamma to go back to non-linear intensities */
	m_red   = pow(m_red, m_gammaRGB[0]);
	m_green = pow(m_green, m_gammaRGB[1]);
	m_blue  = pow(m_blue, m_gammaRGB[2]);

	/* Ensure that we stay within the RGB gamut */
	/* *** FIX THIS: it would be better to desaturate than blindly clip. */
	m_red   = clamp(m_red, 0.0, 255.0);
	m_green = clamp(m_green, 0.0, 255.0);
	m_blue  = clamp(m_blue, 0.0, 255.0);
}

QColor VisionDefectColor::convertDefect(QColor c, int d)
{
	m_red = (double)c.red();
	m_green = (double)c.green();
	m_blue = (double)c.blue();
	m_originalColor = c;
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
	anchor_e[0] = m_rgb2lms[0] + m_rgb2lms[1] + m_rgb2lms[2];
	anchor_e[1] = m_rgb2lms[3] + m_rgb2lms[4] + m_rgb2lms[5];
	anchor_e[2] = m_rgb2lms[6] + m_rgb2lms[7] + m_rgb2lms[8];

	switch (deficiency)
	{
		case deuteranopeVision:
			/* find a,b,c for lam=575nm and lam=475 */
			m_a1 = anchor_e[1] * anchor[8] - anchor_e[2] * anchor[7];
			m_b1 = anchor_e[2] * anchor[6] - anchor_e[0] * anchor[8];
			m_c1 = anchor_e[0] * anchor[7] - anchor_e[1] * anchor[6];
			m_a2 = anchor_e[1] * anchor[2] - anchor_e[2] * anchor[1];
			m_b2 = anchor_e[2] * anchor[0] - anchor_e[0] * anchor[2];
			m_c2 = anchor_e[0] * anchor[1] - anchor_e[1] * anchor[0];
			m_inflection = (anchor_e[2] / anchor_e[0]);
			break;
		case protanopeVision:
			/* find a,b,c for lam=575nm and lam=475 */
			m_a1 = anchor_e[1] * anchor[8] - anchor_e[2] * anchor[7];
			m_b1 = anchor_e[2] * anchor[6] - anchor_e[0] * anchor[8];
			m_c1 = anchor_e[0] * anchor[7] - anchor_e[1] * anchor[6];
			m_a2 = anchor_e[1] * anchor[2] - anchor_e[2] * anchor[1];
			m_b2 = anchor_e[2] * anchor[0] - anchor_e[0] * anchor[2];
			m_c2 = anchor_e[0] * anchor[1] - anchor_e[1] * anchor[0];
			m_inflection = (anchor_e[2] / anchor_e[1]);
			break;
		case tritanopeVision:
			/* Set 1: regions where lambda_a=575, set 2: lambda_a=475 */
			m_a1 = anchor_e[1] * anchor[11] - anchor_e[2] * anchor[10];
			m_b1 = anchor_e[2] * anchor[9]  - anchor_e[0] * anchor[11];
			m_c1 = anchor_e[0] * anchor[10] - anchor_e[1] * anchor[9];
			m_a2 = anchor_e[1] * anchor[5]  - anchor_e[2] * anchor[4];
			m_b2 = anchor_e[2] * anchor[3]  - anchor_e[0] * anchor[5];
			m_c2 = anchor_e[0] * anchor[4]  - anchor_e[1] * anchor[3];
			m_inflection = (anchor_e[1] / anchor_e[0]);
			break;
	}
}

uint VisionDefectColor::getRed()
{
	return (uint)m_red;
}

uint VisionDefectColor::getGreen()
{
	return (uint)m_green;
}

uint VisionDefectColor::getBlue()
{
	return (uint)m_blue;
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
