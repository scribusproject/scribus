/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This file is part of the KDE project
   Copyright (C) 2001, 2002, 2003 The Karbon Developers

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
/* Adapted for Scribus 22.08.2003 by Franz Schmid */
/* Adapted for Scribus 15.01.2006 by Jean Ghali */

#ifndef __SCPAINTEREX_PS2_H__
#define __SCPAINTEREX_PS2_H__

// libart wrapper

#include <QGlobalStatic>
#include <QColor>
#include <QFont>
#include <QTransform>
#include <QList>
#include <QPixmap>
#include <QStack>
#include <QTextStream>

#include "scribusapi.h"
#include "scconfig.h"
#include "colormgmt/sccolormgmtengine.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "vgradient.h"
#include "scpainterexbase.h"

class ScImage;
class ScribusDoc;

class ScPs2OutputParams
{
public:
	ScPs2OutputParams(ScribusDoc* doc = nullptr);
	ScPainterExBase::ColorMode colorMode { ScPainterExBase::rgbMode };
	ScribusDoc* document { nullptr };
	bool reloadImages { true };
	int resolution { 72 };
	bool useProfiles { false };
	bool toGray { false };
	bool mirrorH { false };
	bool mirrorV { false };
	QString outputProfile;
	ScColorProfile   hProfile;
	ScColorTransform rgbToOutputColorTransform;
	ScColorTransform rgbToOutputImageTransform;
	ScColorTransform cmykToOutputColorTransform;
	ScColorTransform cmykToOutputImageTransform;
};

class SCRIBUS_API ScPainterEx_Ps2 : public ScPainterExBase
{
public:

	ScPainterEx_Ps2( QIODevice* iodev, const QRect& rect, const ScPs2OutputParams& options );
	virtual ~ScPainterEx_Ps2();

	int supportedColorModes() const override { return (int) (rgbMode|cmykMode); }
	ColorMode preferredColorMode() const override { return m_colorMode; }
	virtual void setPreferredColorMode(ScPainterEx_Ps2::ColorMode colorMode) { m_colorMode = colorMode; }
	ImageMode imageMode() const override;

	void begin() override;
	void end() override;
	void clear() override;
	void clear( ScColorShade & ) override;

	void beginLayer(double transparency, int blendmode, FPointArray* clipArray = nullptr) override {};
	void endLayer() override {};

	// matrix manipulation
	void setWorldMatrix(const QTransform&) override;
	QTransform worldMatrix() const override;
	void translate(double, double) override;
	void translate(const QPointF& offset) override;
	void rotate(double) override;
	void scale(double, double) override;
	void shear(double, double) override;

	// drawing
	void moveTo(const double&, const double&) override;
	void lineTo(const double&, const double&) override;
	void curveTo(const FPoint& p1, const FPoint& p2, const FPoint& p3) override;
	void newPath() override;
	virtual void closePath();
	void fillPath() override;
	void strokePath() override;
	void setFillRule(bool fillRule) override;
	bool fillRule() const override { return m_fillRule; }
	void setFillMode(int fill) override;
	int  fillMode() const override { return m_fillMode; }
	void setStrokeMode(int fill) override;
	int  strokeMode() const override { return m_strokeMode; }
	void setGradient(VGradientEx::Type mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew) override;
	void setPattern(ScPattern* pattern, const ScPatternTransform& patternTrans, bool mirrorX, bool mirrorY) override;

	void setMaskMode(int mask) override {};
	void setGradientMask(VGradientEx::Type mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew) override  {};
	void setPatternMask(ScPattern *pattern, const ScMaskTransform& maskTrans, bool mirrorX, bool mirrorY)  override {};

	void set4ColorGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4)  override {};
	void set4ColorColors(const ScColorShade& col1, const ScColorShade& col2, const ScColorShade& col3, const ScColorShade& col4)  override {};
	void setDiamondGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4, const FPoint& c5)  override {};
	void setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, QList<QList<MeshPoint> > meshArray)  override {};
	void setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, QList<meshGradientPatch> meshPatches)  override {};

	void setHatchParameters(int mode, double distance, double angle, bool useBackground, const ScColorShade& background, const ScColorShade& foreground, double width, double height) override {};

	void setClipPath() override;

	void drawImage(ScImage* image, ScPainterExBase::ImageMode mode) override;

	void setupPolygon(const FPointArray *points, bool closed = true) override;
	void drawPolygon() override;
	void drawPolyLine() override;
	void drawLine(const FPoint& start, const FPoint& end) override;
	void drawLine(const QPointF& start, const QPointF& end) override;
	void drawRect(double, double, double, double) override;

	// pen + brush
	ScColorShade pen() override;
	ScColorShade brush() override;
	void setPen( const ScColorShade & ) override;
	void setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo ) override;
	void setPenOpacity( double op ) override;
	void setLineWidth( double w) override;
	void setDash(const QVector<double>& array, double ofs) override;
	void setBrush( const ScColorShade & ) override;
	void setBrushOpacity( double op ) override;
	void setOpacity( double op ) override;
	void setFont( const QFont &f ) override;
	QFont font() override;

	// stack management
	void save() override;
	void restore() override;

	void setRasterOp(int op) override;
	void setBlendModeFill(int blendMode)  override {};
	void setBlendModeStroke(int blendMode) override {};

protected:
	virtual void drawRGBImage_ps2(ScImage* image);
	virtual void drawCMYKImage_ps2(ScImage* image);

	virtual bool hasAlphaChannel(ScImage* image);

	virtual void writeRGBImageToStream_Ascii85(ScImage* image);
	virtual void writeRGBImageToStream_AsciiHex(ScImage* image);
	virtual void writeCMYKImageToStream_Ascii85(ScImage* image);
	virtual void writeCMYKImageToStream_AsciiHex(ScImage* image);
	virtual void writeMaskToStream(QImage* image);

private:

	enum AsciiEncoding
	{
		Ascii85Encoding,
		AsciiHexEncoding
	};

	/* Output stream */
	QTextStream   m_stream;
	ColorMode     m_colorMode;
	AsciiEncoding m_encoding { Ascii85Encoding };

	/* Path bounding boxes*/
	QRect m_pathBBox;
	QRect m_clipBBox;

	/* General output options */
	ScPs2OutputParams m_options;

	/* Basic drawing functions*/
	void drawVPath( int mode );
	void drawGradient(const VGradientEx& gradient );
	void drawLinearGradient(const VGradientEx& gradient, const QRect& rect);
	void drawLinearGradient_RGB( const VGradientEx& gradient, const QRect& rect);
	void drawLinearGradient_CMYK( const VGradientEx& gradient, const QRect& rect);
	void drawCircularGradient(const VGradientEx& gradient, const QRect& rect);
	void drawCircularGradient_RGB(const VGradientEx& gradient, const QRect& rect);
	void drawCircularGradient_CMYK(const VGradientEx& gradient, const QRect& rect);
	void getPathBoundingBox(const FPointArray* points, QRect& r) const;
	void transformPoint(const FPoint& in, FPoint& out) const;
	void transformPoints(const FPoint* in, FPoint* out, uint length) const;

	void putColor(const ScColorShade& colorShade, bool doFill);

	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_x;
	unsigned int m_y;
	QTransform m_pageTrans;
	QTransform m_matrix;
	QFont m_font;
/* Filling */
	ScColorShade m_fillColor;
	double m_fillTrans { 1.0 };
	bool   m_fillRule { true };
	int    m_fillMode { 1 };			// 0 = none, 1 = solid, 2 = gradient

	double m_patternScaleX { 1.0 };
	double m_patternScaleY { 1.0 };
	double m_patternOffsetX { 0.0 };
	double m_patternOffsetY { 0.0 };
	double m_patternRotation { 0.0 };
	double m_patternSkewX { 0.0 };
	double m_patternSkewY { 0.0 };
	bool   m_patternMirrorX { false };
	bool   m_patternMirrorY { false };

/* Stroking */
	ScColorShade m_strokeColor;
	double m_strokeTrans { 1.0 };
	double m_lineWidth { 1.0 };
	int    m_strokeMode { 0 };				// 0 = none, 1 = solid, 2 = gradient 3 = pattern
/* Line End Style */
	Qt::PenCapStyle m_lineEnd { Qt::SquareCap };
/* Line Join Style */
	Qt::PenJoinStyle m_lineJoin { Qt::RoundJoin };
/* The Dash Array */
	QVector<double> m_array;
	double m_offset { 0.0 };
/* Transformation Stack */
	QStack<QTransform> m_stack;

/* Some data to describe state of drawing */	
	bool m_pathIsClosed { false };
	bool m_drawingClosedPath { false };
/* Drawing surface dimensions */
//	double m_deviceDimX { 0.0 };
//	double m_deviceDimY { 0.0 };
/* Device resolutions */
//	double m_deviceResX { 0.0 };
//	double m_deviceResY { 0.0 };

/* Color conversion function */
	void   transformImage(QImage* image, uchar* data, int scan) const;
};

#endif
