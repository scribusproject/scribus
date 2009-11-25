/* libwpg
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2004 Marc Oude Kotte (marc@solcon.nl)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02111-1301 USA
 *
 * For further information visit http://libwpg.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef __WPG2PARSER_H__
#define __WPG2PARSER_H__

#include "WPGXParser.h"
#include "WPGBrush.h"
#include "WPGPen.h"

#include <map>
#include <stack>
#include <vector>

class WPG2TransformMatrix
{
public:
	double element[3][3];

	WPG2TransformMatrix()
	{
		// identity transformation
		element[0][0] = element[1][1] = 1; element[2][2] = 1;
		element[0][1] = element[0][2] = 0;
		element[1][0] = element[1][2] = 0;
		element[2][0] = element[2][1] = 0;
	}

	void transform(long& x, long& y) const
	{
		long rx = (long)(element[0][0]*x + element[1][0]*y + element[2][0]);
		long ry = (long)(element[0][1]*x + element[1][1]*y + element[2][1]);
		x = rx;
		y = ry;
	}

	libwpg::WPGPoint transform(const libwpg::WPGPoint& p) const
	{
		libwpg::WPGPoint point;
		point.x = element[0][0]*p.x + element[1][0]*p.y + element[2][0];
		point.y = element[0][1]*p.x + element[1][1]*p.y + element[2][1];
		return point;
	}

	libwpg::WPGRect transform(const libwpg::WPGRect& r) const
	{
		libwpg::WPGRect rect;
		rect.x1 = element[0][0]*r.x1 + element[1][0]*r.y1 + element[2][0];
		rect.y1 = element[0][1]*r.x1 + element[1][1]*r.y1 + element[2][1];
		rect.x2 = element[0][0]*r.x2 + element[1][0]*r.y2 + element[2][0];
		rect.y2 = element[0][1]*r.x2 + element[1][1]*r.y2 + element[2][1];
		return rect;
	}

	WPG2TransformMatrix& transformBy(const WPG2TransformMatrix& m)
	{
		double result[3][3];

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = 0;
				for(int k = 0; k < 3; k++)
					result[i][j] += m.element[i][k]*element[k][j];
			}

		for(int x = 0; x < 3; x++)
			for(int y = 0; y < 3; y++)
				element[x][y] = result[x][y];

		return *this;
	}
};

class WPGCompoundPolygon
{
public:
	WPG2TransformMatrix matrix;
	bool isFilled;
	bool isFramed;
	bool isClosed;

	WPGCompoundPolygon(): matrix(), isFilled(true), isFramed(true), isClosed(true) {}
};

class WPGGroupContext
{
public:
	unsigned subIndex;
	int parentType;
	libwpg::WPGPath compoundPath;
	WPG2TransformMatrix compoundMatrix;
	bool compoundWindingRule;
	bool compoundFilled;
	bool compoundFramed;
	bool compoundClosed;

	WPGGroupContext(): subIndex(0), parentType(0),
	compoundPath(), compoundMatrix(), compoundWindingRule(false),
	compoundFilled(false), compoundFramed(true), compoundClosed(false)	{}

	bool isCompoundPolygon() const { return parentType == 0x1a; }
};

class WPGBitmapContext
{
public:
	double x1, y1, x2, y2;
	long hres, vres;
	WPGBitmapContext(): x1(0), y1(0), x2(0), y2(0), hres(100), vres(100) {}
};

class WPGBinaryDataContext
{
public:
	double x1, y1, x2, y2;
	int numObjects, objectIndex;
	std::vector<libwpg::WPGString> mimeTypes;
	WPGBinaryDataContext(): x1(0), y1(0), x2(0), y2(0), numObjects(0), objectIndex(0), mimeTypes() {}
};

class WPG2Parser : public WPGXParser
{
public:
	WPG2Parser(WPXInputStream *input, libwpg::WPGPaintInterface* painter);
	bool parse();

private:
	void handleStartWPG();
	void handleEndWPG();
	void handleFormSettings();
	void handleLayer();
	void handleCompoundPolygon();

	void handlePenStyleDefinition();
	void handlePatternDefinition();
	void handleColorPalette();
	void handleDPColorPalette();
	void handlePenForeColor();
	void handleDPPenForeColor();
	void handlePenBackColor();
	void handleDPPenBackColor();
	void handlePenStyle();
	void handlePenSize();
	void handleDPPenSize();
	void handleLineCap();
	void handleLineJoin();
	void handleBrushGradient();
	void handleDPBrushGradient();
	void handleBrushForeColor();
	void handleDPBrushForeColor();
	void handleBrushBackColor();
	void handleDPBrushBackColor();
	void handleBrushPattern();

	void handlePolyline();
	void handlePolyspline();
	void handlePolycurve();
	void handleRectangle();
	void handleArc();

	void handleBitmap();
	void handleBitmapData();
	
	void handleObjectCapsule();
	void handleObjectImage();

	void resetPalette();
	void flushCompoundPolygon();

	// parsing context
	int m_recordLength;
	long m_recordEnd;
	bool m_success;
	bool m_exit;
	bool m_graphicsStarted;
	unsigned int m_xres;
	unsigned int m_yres;
	long m_xofs;
	long m_yofs;
	long m_width;
	long m_height;
	bool m_doublePrecision;
	libwpg::WPGPen m_pen;
	libwpg::WPGBrush m_brush;
	std::map<unsigned int,libwpg::WPGDashArray> m_penStyles;
	bool m_layerOpened;
	unsigned int m_layerId;
	WPG2TransformMatrix m_matrix;
	double m_gradientAngle;
	libwpg::WPGPoint m_gradientRef;
	std::stack<WPGGroupContext> m_groupStack;
	WPG2TransformMatrix m_compoundMatrix;
	bool m_compoundWindingRule;
	bool m_compoundFilled;
	bool m_compoundFramed;
	bool m_compoundClosed;
	WPGBitmapContext m_bitmap;
	WPGBinaryDataContext m_binaryData;
	bool m_hFlipped, m_vFlipped;

	class ObjectCharacterization;
	void parseCharacterization(ObjectCharacterization*);
	unsigned m_binaryId;
};

#endif // __WPG2PARSER_H__
