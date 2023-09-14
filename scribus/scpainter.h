/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef __SCPAINTER_H__
#define __SCPAINTER_H__

#include <QPainterPath>
#include <QPainter>
#include <QVector>
#include <QStack>
#include <QColor>
#include <QTransform>
#include <QFont>
#include <QImage>
#include <QPointF>

#include "scribusapi.h"
#include "scconfig.h"
#include "scpatterntransform.h"
#include "sctextstruct.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "vgradient.h"
#include "mesh.h"

class ScPattern;

typedef struct _cairo cairo_t;
typedef struct _cairo_surface cairo_surface_t;
typedef struct _cairo_pattern cairo_pattern_t;

class SCRIBUS_API ScPainter
{
public:
	ScPainter(QImage *target, int w, int h, double transparency = 1.0, int blendmode = 0);
	virtual ~ScPainter();

	enum FillMode { None, Solid, Gradient, Pattern, Hatch };
	virtual void beginLayer(double transparency, int blendmode, FPointArray *clipArray = nullptr);
	virtual void endLayer();
	virtual void setAntialiasing(bool enable);
	virtual void begin();
	virtual void end();
	void clear();
	virtual void clear(const QColor &);
	virtual cairo_t* context();

	// matrix manipulation
	virtual void setWorldMatrix(const QTransform &);
	virtual const QTransform worldMatrix();
	virtual void setZoomFactor(double);
	virtual double zoomFactor() { return m_zoomFactor; }
	virtual void translate(double, double);
	virtual void translate(const QPointF& offset);
	virtual void rotate(double);
	virtual void scale(double, double);

	// drawing
	virtual void moveTo(const double &, const double &);
	virtual void lineTo(const double &, const double &);
	virtual void curveTo(const FPoint& p1, const FPoint& p2, const FPoint& p3);
	virtual void newPath();
	virtual void closePath();
	virtual void fillPath();
	virtual void strokePath();
	virtual void setFillRule(bool fillRule);
	virtual bool fillRule() const { return m_fillRule; }
	virtual void setFillMode(int fill);
	virtual int  fillMode() const  { return m_fillMode; }
	virtual int  maskMode() const  { return m_maskMode; }
	virtual void setStrokeMode(int stroke);
	virtual int  strokeMode() const  { return m_strokeMode; }
	virtual void setGradient(VGradient::VGradientType mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew);
	virtual void setPattern(ScPattern *pattern, const ScPatternTransform& patternTrans, bool mirrorX, bool mirrorY);

	virtual void setMaskMode(int mask);
	virtual void setGradientMask(VGradient::VGradientType mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew);
	virtual void setPatternMask(ScPattern *pattern, const ScMaskTransform& maskTrans, bool mirrorX, bool mirrorY);

	virtual void set4ColorGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4);
	virtual void set4ColorColors(const QColor& col1, const QColor& col2, const QColor& col3, const QColor& col4);
	virtual void setDiamondGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4, const FPoint& c5);
	virtual void setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const QList<QList<MeshPoint> >& meshArray);
	virtual void setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const QList<meshGradientPatch>& meshPatches);

	virtual void setHatchParameters(int mode, double distance, double angle, bool useBackground, const QColor& background, const QColor& foreground, double width, double height);

	virtual void setClipPath();

	virtual void drawImage(QImage *image);
	virtual void setupPolygon(const FPointArray *points, bool closed = true);
	virtual void setupSharpPolygon(const FPointArray *points, bool closed = true);
	virtual void sharpLineHelper(FPoint &pp);
	virtual void sharpLineHelper(QPointF &pp);
	virtual void drawPolygon();
	virtual void drawPolyLine();
	virtual void drawLine(const FPoint& start, const FPoint& end);
	virtual void drawLine(const QPointF& start, const QPointF& end);
	virtual void drawSharpLine(FPoint start, FPoint end);
	virtual void drawSharpLine(QPointF start, QPointF end);
	virtual void drawRect(double, double, double, double);
	virtual void drawSharpRect(double x, double y, double w, double h);
	virtual void drawText(const QRectF& area, const QString& text, bool filled = true, int align = 0);
	virtual void drawShadeCircle(const QRectF &re, const QColor& color, bool sunken, int lineWidth);
	virtual void drawShadePanel(const QRectF &r, const QColor& color, bool sunken, int lineWidth);
	virtual void drawUnderlinedRect(const QRectF &r, const QColor& color, int lineWidth);
	virtual void colorizeAlpha(const QColor& color);
	virtual void colorize(const QColor& color);
	virtual void blurAlpha(int radius);
	virtual void blur(int radius);

	// pen + brush
	virtual QColor pen();
	virtual QColor brush();
	virtual double brushOpacity();
	virtual void setPen(const QColor &);
	virtual void setPen(const QColor &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo);
	virtual void setPenOpacity(double op);
	virtual void setLineWidth(double w);
	virtual void setDash(const QVector<double>& m_array, double ofs);
	virtual void setBrush(const QColor &);
	virtual void setBrushOpacity(double op);
	virtual void setOpacity(double op);
	virtual void setFont(const ScFace &f , double s);

	// stack management
	virtual void save();
	virtual void restore();
	virtual int blendModeFill();

	virtual void setRasterOp(int blendMode);
	virtual void setBlendModeFill(int blendMode);
	virtual void setBlendModeStroke(int blendMode);

	VGradient fill_gradient { VGradient::linear };
	VGradient stroke_gradient { VGradient::linear };
	VGradient mask_gradient;
	ScPattern *m_maskPattern { nullptr };
	ScPattern *m_pattern { nullptr };

private:
	void fillPathHelper();
	void strokePathHelper();

	cairo_t* m_cr { nullptr };
	struct layerProp
	{
		cairo_surface_t *data { nullptr };
		int blendmode { 0 };
		double transparency { 1.0 };
		int maskMode { 0 };				// 0 = none, 1 = gradient 2 = pattern
		ScMaskTransform maskPatternTrans;
		bool mask_patternMirrorX { false };
		bool mask_patternMirrorY { false };
		double mask_gradientScale { 1.0 };
		double mask_gradientSkew { 0.0 };
		VGradient mask_gradient;
		ScPattern *maskPattern { nullptr };
		FPointArray groupClip;
		bool fillRule { false };
	};
	cairo_pattern_t *getMaskPattern();
	cairo_surface_t *m_imageMask { nullptr };
	QImage m_imageQ;

	QStack<layerProp> m_Layers;
	QStack<double> m_zoomStack;
	QImage *m_image { nullptr };
	double  m_layerTransparency { 1.0 };
	int  m_blendMode { 0 };
	int  m_blendModeFill { 0 };
	int  m_blendModeStroke { 0 };
	QTransform m_matrix;
	ScFace m_font;
	double m_fontSize { 0.0 };
	/*! \brief Filling */
	QColor m_fill { 0, 0, 0 };
	double m_fill_trans { 1.0 };
	bool m_fillRule { true };
	int m_fillMode { 1 };				// 0 = none, 1 = solid, 2 = gradient 3 = pattern 4 = hatch
	ScPatternTransform m_patternTrans;
	bool m_patternMirrorX { false };
	bool m_patternMirrorY { false };
	double m_gradientScale { 0.0 };
	double m_gradientSkew { 0.0 };
	FPoint gradPatchP1;
	FPoint gradPatchP2;
	FPoint gradPatchP3;
	FPoint gradPatchP4;
	FPoint gradControlP1;
	FPoint gradControlP2;
	FPoint gradControlP3;
	FPoint gradControlP4;
	FPoint gradControlP5;
	QColor gradPatchColor1;
	QColor gradPatchColor2;
	QColor gradPatchColor3;
	QColor gradPatchColor4;
	QList<QList<MeshPoint> > meshGradientArray;
	QList<meshGradientPatch> meshGradientPatches;
	double m_hatchAngle { 0.0 };
	double m_hatchDistance { 0.0 };
	int m_hatchType { 0 };				// 0 = single 1 = double 2 = triple
	bool m_hatchUseBackground { false };
	QColor m_hatchBackground;
	QColor m_hatchForeground;
	double m_hatchWidth { 0.0 };
	double m_hatchHeight { 0.0 };
	/*! \brief Stroking */
	QColor m_stroke { 0, 0, 0 };
	double m_stroke_trans { 1.0 };
	double m_LineWidth { 1.0 };
	int m_strokeMode { 0 };				// 0 = none, 1 = solid, 2 = gradient 3 = pattern
	int m_maskMode { 0 };				// 0 = none, 1 = gradient 2 = pattern
	ScMaskTransform m_maskPatternTrans;
	bool m_mask_patternMirrorX { false };
	bool m_mask_patternMirrorY { false };
	double m_mask_gradientScale { 0.0 };
	double m_mask_gradientSkew { 0.0 };

	/*! \brief Line End Style */
	Qt::PenCapStyle PLineEnd { Qt::FlatCap };
  /*! \brief Line Join Style */
	Qt::PenJoinStyle PLineJoin { Qt::MiterJoin };
  /*! \brief The Dash Array */
	QVector<double> m_array;
	double m_offset { 0.0 };
	/*! \brief Zoom Factor of the Painter */
	double m_zoomFactor { 1.0 };
	bool m_imageMode { true };
};

#endif
