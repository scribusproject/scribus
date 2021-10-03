/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QtGlobal>
#include <cmath>

#include "sccolorstructs.h"

void RGBColor::getRgb(RGBColorF& rgbF) const
{
	rgbF.r = r / 255.0;
	rgbF.g = g / 255.0;
	rgbF.b = b / 255.0;
}

void RGBColorF::getRgb(RGBColor& rgb) const
{
	double rgb_r = qMax(0.0, qMin(r, 1.0));
	double rgb_g = qMax(0.0, qMin(g, 1.0));
	double rgb_b = qMax(0.0, qMin(b, 1.0));

	rgb.r = qRound(rgb_r * 255.0);
	rgb.g = qRound(rgb_g * 255.0);
	rgb.b = qRound(rgb_b * 255.0);
}

void RGBColorF::toHsv(HSVColorF& hsv) const
{
	double rgb_r = qMax(0.0, qMin(r, 1.0));
	double rgb_g = qMax(0.0, qMin(g, 1.0));
	double rgb_b = qMax(0.0, qMin(b, 1.0));

	double rgb_min  = qMin(rgb_r, qMin(rgb_g, rgb_b));
	double rgb_max  = qMax(rgb_r, qMax(rgb_g, rgb_b));
	double diff = rgb_max - rgb_min;

	hsv.h = 0;
	if (rgb_max == rgb_min)
		hsv.h = 0;
	else if (rgb_max == rgb_r)
		hsv.h = fmod(60 * (rgb_g - rgb_b) / diff + 360, 360);
	else if (rgb_max == rgb_g)
		hsv.h = 120 + 60 * (rgb_b - rgb_r) / diff;
	else
		hsv.h = 240 + 60 * (rgb_r - rgb_g) / diff;
	hsv.h /= 360;

	hsv.s = (rgb_max == 0.0) ? 0 : (1 - rgb_min / rgb_max);
	hsv.v = rgb_max;
}

void HSVColorF::toRgb(RGBColor& rgb) const
{
	RGBColorF rgbF;
	toRgb(rgbF);

	rgb.r = qRound(rgbF.r * 255.0);
	rgb.g = qRound(rgbF.g * 255.0);
	rgb.b = qRound(rgbF.b * 255.0);
}

void HSVColorF::toRgb(RGBColorF& rgb) const
{
	int     hi;
	double  f, l, m, n;

	double hsv_h = qMax(0.0, qMin(h, 1.0));
	double hsv_s = qMax(0.0, qMin(s, 1.0));
	double hsv_v = qMax(0.0, qMin(v, 1.0));

	if (hsv_h == 1.0)
		hsv_h = 0.0;

	hi = ((int) (hsv_h * 6.0)) % 6;
	f  = hsv_h * 6.0 - hi;

	l = hsv_v * (1 - hsv_s);
	m = hsv_v * (1 - f * hsv_s);
	n = hsv_v * (1 - (1 - f) * hsv_s);

	switch (hi)
	{
	case 0:
		rgb.r = hsv_v;
		rgb.g = n;
		rgb.b = l;
		break;
	case 1:
		rgb.r = m;
		rgb.g = hsv_v;
		rgb.b = l;
		break;
	case 2:
		rgb.r = l;
		rgb.g = hsv_v;
		rgb.b = n;
		break;
	case 3:
		rgb.r = l;
		rgb.g = m;
		rgb.b = hsv_v;
		break;
	case 4:
		rgb.r = n;
		rgb.g = l;
		rgb.b = hsv_v;
		break;
	default:
		rgb.r = hsv_v;
		rgb.g = l;
		rgb.b = m;
	}
}