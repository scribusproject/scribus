/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCCOLORSTRUCTS_H
#define SCCOLORSTRUCTS_H

#ifdef HAVE_CONFIG_H
#include "scconfig.h"
#endif

#include "scribusapi.h"

struct HSVColorF;
struct RGBColorF;

struct RGBColor
{
	int r { 0 };
	int g { 0 };
	int b { 0 };
	
	RGBColor() = default;

	void getRgb(RGBColorF& rgb) const;
	void getValues(int& vr, int& vg, int& vb) const
	{
		vr = r;
		vg = g;
		vb = b;
	}
};

struct RGBColorF
{
	double r { 0.0 };
	double g { 0.0 };
	double b { 0.0 };
	
	RGBColorF() = default;

	void getRgb(RGBColor& rgb) const;
	void getValues(double& vr, double& vg, double& vb) const
	{
		vr = r;
		vg = g;
		vb = b;
	}
	void toHsv(HSVColorF& hsv) const;
};

struct CMYKColor
{
	int c { 0 };
	int m { 0 };
	int y { 0 };
	int k { 0 };
	
	CMYKColor() = default;

	void getValues(int& vc, int& vm, int& vy, int& vk) const
	{
		vc = c;
		vm = m;
		vy = y;
		vk = k;
	}
};

struct CMYKColorF
{
	double c { 0.0 };
	double m { 0.0 };
	double y { 0.0 };
	double k { 0.0 };
	
	CMYKColorF() = default;

	void getValues(double& vc, double& vm, double& vy, double& vk) const
	{
		vc = c;
		vm = m;
		vy = y;
		vk = k;
	}
};

struct HSVColorF
{
	double h { 0.0 };
	double s { 0.0 };
	double v { 0.0 };
	
	HSVColorF() = default;

	void getValues(double& vh, double& vs, double& vv) const
	{
		vh = h;
		vs = s;
		vv = v;
	}
	void toRgb(RGBColor& rgb)  const;
	void toRgb(RGBColorF& rgb) const;
};

#endif

