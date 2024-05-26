/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef COLORPICKER_ENUM_H
#define COLORPICKER_ENUM_H

#include <QString>
#include "vgradient.h"
#include "commonstrings.h"

enum class Context
{
	Simple = 0,
	Fill = 1,
	FillMask = 2,
	Line = 3,
	LineMask = 4,
	Text = 5,
	TextOutline = 6,
	TextBackground = 7,
	DropShadow = 8,
	TableCellFill = 9,
	TableCellStroke = 10
};

enum class Mode
{
	Solid = 0,
	Gradient = 1,
	Hatch = 2,
	Pattern = 3
};

struct CPGeneralData
{
	int overprint = 0;
};

struct CPHatchData
{
	QString ColorForeground = CommonStrings::tr_NoneColor;
	QString ColorBackground = CommonStrings::tr_NoneColor;
	bool HasBackgroundColor = false;
	double Angle = 0.0;
	double Distance = 1.0;
	int Type = 0;
};

struct CPGradientData
{
	VGradient Gradient;
	QString Name = "";
	int Type = 0;
	VGradient::VGradientRepeatMethod repeatMethod = VGradient::pad;
	// Four Colors
	QString Color1Name = CommonStrings::tr_NoneColor;
	QString Color2Name = CommonStrings::tr_NoneColor;
	QString Color3Name = CommonStrings::tr_NoneColor;
	QString Color4Name = CommonStrings::tr_NoneColor;
	double Color1Shade = 100.0;
	double Color2Shade = 100.0;
	double Color3Shade = 100.0;
	double Color4Shade = 100.0;
	double Color1Alpha = 1.0;
	double Color2Alpha = 1.0;
	double Color3Alpha = 1.0;
	double Color4Alpha = 1.0;
};

struct CPGradientVectorData
{
	// Regular Gradient Vector
	double StartX;
	double EndX;
	double StartY;
	double EndY;
	double Skew;
	double Scale;
	double FocalX;
	double FocalY;
	// Control Points for Four Colors & Diamond
	double C1X;
	double C1Y;
	double C2X;
	double C2Y;
	double C3X;
	double C3Y;
	double C4X;
	double C4Y;
	double C5X;
	double C5Y;

};

struct CPColorData
{
	QString Name = CommonStrings::tr_NoneColor;
	double Opacity = 1.0;
	double Shade = 100.0;
};

struct CPPatternData
{
	QString Name = "";
	double OffsetX = 0.0;
	double OffsetY = 0.0;
	double ScaleX = 1.0;
	double ScaleY = 1.0;
	double SkewX = 0.0;
	double SkewY = 0.0;
	double Angle = 0.0;
	double Spacing = 1.0;
	bool MirrorX = false;
	bool MirrorY = false;
	bool OnPath = false;
};


#endif // COLORPICKER_ENUM_H
