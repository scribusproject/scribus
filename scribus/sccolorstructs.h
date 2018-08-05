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
	int r;
	int g;
	int b;
	
	RGBColor() {r=g=b=0;}

	void getRgb(RGBColorF& rgb);
	void getValues(int& vr, int& vg, int& vb) { vr = r; vg = g; vb = b; }
};

struct RGBColorF
{
	double r;
	double g;
	double b;
	
	RGBColorF() {r=g=b=0.0;}

	void getRgb(RGBColor& rgb);
	void getValues(double& vr, double& vg, double& vb) { vr = r; vg = g; vb = b; }
	void toHsv(HSVColorF& hsv);
};

struct CMYKColor
{
	int c;
	int m;
	int y;
	int k;
	
	CMYKColor() {c=m=y=k=0;}
	void getValues(int& vc, int& vm, int& vy, int& vk) { vc = c; vm = m; vy = y; vk = k; }
};

struct CMYKColorF
{
	double c;
	double m;
	double y;
	double k;
	
	CMYKColorF() {c=m=y=k=0.0;}
	void getValues(double& vc, double& vm, double& vy, double& vk) { vc = c; vm = m; vy = y; vk = k; }
};

struct HSVColorF
{
	double h;
	double s;
	double v;
	
	HSVColorF() { h = s = v = 0.0; }
	void getValues(double& vh, double& vs, double& vv) { vh = h; vs = s; vv = v; }
	void toRgb(RGBColor& rgb)  const;
	void toRgb(RGBColorF& rgb) const;
};

#endif

