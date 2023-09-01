/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCPATTERNTRANSFORM_H
#define SCPATTERNTRANSFORM_H

#include "scribusapi.h"

class SCRIBUS_API ScPatternTransform
{
public:
	double scaleX { 100.0 };
	double scaleY { 100.0 };
	double offsetX { 0.0 };
	double offsetY { 0.0 };
	double rotation {0.0};
	double skewX { 0.0 };
	double skewY { 0.0 };
};

class SCRIBUS_API ScStrokePatternTransform : public ScPatternTransform
{
public:
	double space { 1.0 };
};

using ScMaskTransform = ScPatternTransform;

#endif
