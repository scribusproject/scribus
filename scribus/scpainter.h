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
	ScPainter(QImage *target, int w, int h, double transparency = 1.0, int blendmode = 0 );
	virtual ~ScPainter();
	enum FillMode { None, Solid, Gradient, Pattern, Hatch };
	virtual void beginLayer(double transparency, int blendmode, FPointArray *clipArray = 0);
	virtual void endLayer();
	virtual void setAntialiasing(bool enable);
	virtual void begin();
	virtual void end();
	void clear();
	virtual void clear( const QColor & );
	virtual cairo_t* context();

	// matrix manipulation
	virtual void setWorldMatrix( const QTransform & );
	virtual const QTransform worldMatrix();
	virtual void setZoomFactor( double );
	virtual double zoomFactor() { return m_zoomFactor; }
	virtual void translate( double, double );
	virtual void translate( const QPointF& offset );
	virtual void rotate( double );
	virtual void scale( double, double );

	// drawing
	virtual void moveTo( const double &, const double & );
	virtual void lineTo( const double &, const double & );
	virtual void curveTo( FPoint p1, FPoint p2, FPoint p3 );
	virtual void newPath();
	virtual void closePath();
	virtual void fillPath();
	virtual void strokePath();
	virtual void setFillRule( bool fillRule );
	virtual bool fillRule() { return m_fillRule; }
	virtual void setFillMode( int fill );
	virtual int  fillMode() { return m_fillMode; }
	virtual int  maskMode() { return m_maskMode; }
	virtual void setStrokeMode( int stroke );
	virtual int  strokeMode() { return m_strokeMode; }
	virtual void setGradient( VGradient::VGradientType mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew);
	virtual void setPattern(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);

	virtual void setMaskMode( int mask );
	virtual void setGradientMask( VGradient::VGradientType mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew);
	virtual void setPatternMask(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);

	virtual void set4ColorGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4);
	virtual void set4ColorColors(QColor col1, QColor col2, QColor col3, QColor col4);
	virtual void setDiamondGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4, FPoint c5);
	virtual void setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<QList<MeshPoint> > meshArray);
	virtual void setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<meshGradientPatch> meshPatches);

	virtual void setHatchParameters(int mode, double distance, double angle, bool useBackground, QColor background, QColor foreground, double width, double height);

	virtual void setClipPath();

	virtual void drawImage( QImage *image);
	virtual void setupPolygon(const FPointArray *points, bool closed = true);
	virtual void setupSharpPolygon(const FPointArray *points, bool closed = true);
	virtual void sharpLineHelper(FPoint &pp);
	virtual void sharpLineHelper(QPointF &pp);
	virtual void drawPolygon();
	virtual void drawPolyLine();
	virtual void drawLine(FPoint start, FPoint end);
	virtual void drawLine(const QPointF& start, const QPointF& end);
	virtual void drawSharpLine(FPoint start, FPoint end);
	virtual void drawSharpLine(QPointF start, QPointF end);
	virtual void drawRect(double, double, double, double);
	virtual void drawSharpRect(double x, double y, double w, double h);
	virtual void drawText(QRectF area, QString text, bool filled = true, int align = 0);
	virtual void drawShadeCircle(const QRectF &re, const QColor color, bool sunken, int lineWidth);
	virtual void drawShadePanel(const QRectF &r, const QColor color, bool sunken, int lineWidth);
	virtual void colorizeAlpha(QColor color);
	virtual void colorize(QColor color);
	virtual void blurAlpha(int radius);
	virtual void blur(int radius);

	// pen + brush
	virtual QColor pen();
	virtual QColor brush();
	virtual double brushOpacity();
	virtual void setPen( const QColor & );
	virtual void setPen( const QColor &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo );
	virtual void setPenOpacity( double op );
	virtual void setLineWidth( double w);
	virtual void setDash(const QVector<double>& m_array, double ofs);
	virtual void setBrush( const QColor & );
	virtual void setBrushOpacity( double op );
	virtual void setOpacity( double op );
	virtual void setFont(const ScFace &f , double s);

	// stack management
	virtual void save();
	virtual void restore();
	virtual int blendModeFill();


	virtual void setRasterOp( int blendMode );
	virtual void setBlendModeFill( int blendMode );
	virtual void setBlendModeStroke( int blendMode );

	VGradient fill_gradient;
	VGradient stroke_gradient;
	VGradient mask_gradient;
	ScPattern *m_maskPattern;
	ScPattern *m_pattern;

private:
	void fillPathHelper();
	void strokePathHelper();

	cairo_t *m_cr;
	struct layerProp
	{
		cairo_surface_t *data;
		int blendmode;
		double tranparency;
		int maskMode;				// 0 = none, 1 = gradient 2 = pattern
		double mask_patternScaleX;
		double mask_patternScaleY;
		double mask_patternOffsetX;
		double mask_patternOffsetY;
		double mask_patternRotation;
		double mask_patternSkewX;
		double mask_patternSkewY;
		bool mask_patternMirrorX;
		bool mask_patternMirrorY;
		double mask_gradientScale;
		double mask_gradientSkew;
		VGradient mask_gradient;
		ScPattern *maskPattern;
		FPointArray groupClip;
		bool pushed;
		bool fillRule;
	};
	cairo_pattern_t *getMaskPattern();
	cairo_surface_t *m_imageMask;
	QImage m_imageQ;

	QStack<layerProp> m_Layers;
	QStack<double> m_zoomStack;
	QImage *m_image;
	double  m_layerTransparency;
	int  m_blendMode;
	int  m_blendModeFill;
	int  m_blendModeStroke;
	unsigned int m_width;
	unsigned int m_height;
	QTransform m_matrix;
	ScFace m_font;
	double m_fontSize;
	bool mf_underline;
	bool mf_strikeout;
	bool mf_shadow;
	bool mf_outlined;
	/*! \brief Filling */
	QColor m_fill;
	double m_fill_trans;
	bool m_fillRule;
	int m_fillMode;				// 0 = none, 1 = solid, 2 = gradient 3 = pattern 4 = hatch
	double m_patternScaleX;
	double m_patternScaleY;
	double m_patternOffsetX;
	double m_patternOffsetY;
	double m_patternRotation;
	double m_patternSkewX;
	double m_patternSkewY;
	bool m_patternMirrorX;
	bool m_patternMirrorY;
	double m_gradientScale;
	double m_gradientSkew;
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
	double m_hatchAngle;
	double m_hatchDistance;
	int m_hatchType;				// 0 = single 1 = double 2 = triple
	bool m_hatchUseBackground;
	QColor m_hatchBackground;
	QColor m_hatchForeground;
	double m_hatchWidth;
	double m_hatchHeight;
	/*! \brief Stroking */
	QColor m_stroke;
	double m_stroke_trans;
	double m_LineWidth;
	int m_strokeMode;				// 0 = none, 1 = solid, 2 = gradient 3 = pattern
	int m_maskMode;				// 0 = none, 1 = gradient 2 = pattern
	double m_mask_patternScaleX;
	double m_mask_patternScaleY;
	double m_mask_patternOffsetX;
	double m_mask_patternOffsetY;
	double m_mask_patternRotation;
	double m_mask_patternSkewX;
	double m_mask_patternSkewY;
	bool m_mask_patternMirrorX;
	bool m_mask_patternMirrorY;
	double m_mask_gradientScale;
	double m_mask_gradientSkew;

	/*! \brief Line End Style */
	Qt::PenCapStyle PLineEnd;
  /*! \brief Line Join Style */
	Qt::PenJoinStyle PLineJoin;
  /*! \brief The Dash Array */
	QVector<double> m_array;
	double m_offset;
	/*! \brief Zoom Factor of the Painter */
	double m_zoomFactor;
	bool m_imageMode;
	bool m_layeredMode;
	bool m_svgMode;
};

#endif
