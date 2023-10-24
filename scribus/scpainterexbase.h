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

#ifndef __SCPAINTEREXBASE_H__
#define __SCPAINTEREXBASE_H__

#include <QColor>
#include <QFont>
#include <QGlobalStatic>
#include <QList>
#include <QTransform>
#include <QPixmap>
#include <QPointF>
#include <QStack>

#include "scribusapi.h"
#include "scconfig.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "mesh.h"
#include "vgradientex.h"
#include "sccolorshade.h"
#include "scpatterntransform.h"

class ScImage;
class ScPattern;

class SCRIBUS_API ScPainterExBase
{
public:
	virtual ~ScPainterExBase() = default;

	enum FillMode { None, Solid, Gradient, Pattern, Hatch };
	enum ColorMode { rgbMode = 1, cmykMode = 2 };
	enum ImageMode { cmykImages, rgbImages, rawImages };
	enum Capabilities{ basic = 0, transparencies = 1, patterns = 2 };

	virtual Capabilities capabilities() const { return basic; }
	virtual bool hasCapability(Capabilities cap) { return ((m_capabilities & (int) cap) != 0); }

	virtual int supportedColorModes() const = 0;
	virtual ColorMode preferredColorMode() const = 0;
	virtual ImageMode imageMode() const = 0;
	
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void clear() = 0;
	virtual void clear(ScColorShade&) = 0;

	virtual void beginLayer(double transparency, int blendmode, FPointArray* clipArray = nullptr) = 0;
	virtual void endLayer() = 0;

	// matrix manipulation
	virtual void setWorldMatrix(const QTransform &) = 0;
	virtual QTransform worldMatrix() const = 0;
	virtual void translate(double, double) = 0;
	virtual void translate(const QPointF& offset) = 0;
	virtual void rotate(double) = 0;
	virtual void scale(double, double) = 0;
	virtual void shear(double, double) = 0;

	// drawing
	virtual void moveTo(const double&, const double&) = 0;
	virtual void lineTo(const double&, const double&) = 0;
	virtual void curveTo(const FPoint& p1, const FPoint& p2, const FPoint& p3) = 0;
	virtual void newPath() = 0;
	virtual void fillPath() = 0;
	virtual void strokePath() = 0;
	virtual void setFillRule(bool fillRule) = 0;
	virtual bool fillRule() const = 0;
	virtual void setFillMode(int fill) = 0;
	virtual int  fillMode() const = 0;
	virtual void setStrokeMode(int stroke) = 0;
	virtual int  strokeMode() const = 0;
	virtual void setGradient(VGradientEx::Type mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew) = 0;
	virtual void setPattern(ScPattern *pattern, const ScPatternTransform& patternTrans, bool mirrorX, bool mirrorY) = 0;

	virtual void setMaskMode(int mask) = 0;
	virtual void setGradientMask(VGradientEx::Type mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew) = 0;
	virtual void setPatternMask(ScPattern *pattern, const ScMaskTransform& maskTrans, bool mirrorX, bool mirrorY) = 0;

	virtual void set4ColorGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4) = 0;
	virtual void set4ColorColors(const ScColorShade& col1, const ScColorShade& col2, const ScColorShade& col3, const ScColorShade& col4) = 0;
	virtual void setDiamondGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4, const FPoint& c5) = 0;
	virtual void setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, QList<QList<MeshPoint> > meshArray) = 0;
	virtual void setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, QList<meshGradientPatch> meshPatches) = 0;

	virtual void setHatchParameters(int mode, double distance, double angle, bool useBackground, const ScColorShade& background, const ScColorShade& foreground, double width, double height) = 0;

	virtual void setClipPath() = 0;

	virtual void drawImage(ScImage *image, ScPainterExBase::ImageMode mode) = 0;
	virtual void setupPolygon(const FPointArray *points, bool closed = true) = 0;
	virtual void drawPolygon() = 0;
	virtual void drawPolyLine() = 0;
	virtual void drawLine(const FPoint& start, const FPoint& end) = 0;
	virtual void drawLine(const QPointF& start, const QPointF& end) = 0;
	virtual void drawRect(double, double, double, double) = 0;

	// pen + brush
	virtual ScColorShade pen() = 0;
	virtual ScColorShade brush() = 0;
	virtual void setPen(const ScColorShade &c) = 0;
	virtual void setPen(const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo) = 0;
	virtual void setPenOpacity(double op) = 0;
	virtual void setLineWidth(double w) = 0;
	virtual void setDash(const QVector<double>& array, double ofs) = 0;
	virtual void setBrush(const ScColorShade &) = 0;
	virtual void setBrushOpacity(double op) = 0;
	virtual void setOpacity(double op) = 0;
	virtual void setFont(const QFont &f) = 0;
	virtual QFont font() = 0;

	// stack management
	virtual void save() = 0;
	virtual void restore() = 0;

	virtual void setRasterOp(int op) = 0;
	virtual void setBlendModeFill(int blendMode) = 0;
	virtual void setBlendModeStroke(int blendMode) = 0;

	VGradientEx  m_fillGradient { VGradientEx::linear };
	VGradientEx  m_strokeGradient { VGradientEx::linear };
	VGradientEx  m_maskGradient;
	ScPattern*   m_pattern { nullptr };
	ScPattern*   m_maskPattern { nullptr };
	QTransform   m_patternTransform;

protected:
	int m_capabilities { 0 };
	ScPainterExBase(void);
};

#endif
