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
#include <QMatrix>
#include <QFont>
#include <QImage>
#include "scribusapi.h"
#include "scconfig.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "vgradient.h"
#include "scpattern.h"

#ifdef HAVE_CAIRO
typedef struct _cairo cairo_t;
typedef struct _cairo_surface cairo_surface_t;
typedef struct _cairo_pattern cairo_pattern_t;
#endif

class SCRIBUS_API ScPainter
{
public:
	ScPainter( QImage *target, unsigned int w, unsigned int h, double transparency = 1.0, int blendmode = 0 );
#ifdef HAVE_CAIRO
	ScPainter( QString target, unsigned int w, unsigned int h, double transparency = 1.0, int blendmode = 0 );
#endif
	virtual ~ScPainter();
	enum FillMode { None, Solid, Gradient, Pattern };
	virtual void beginLayer(double transparency, int blendmode, FPointArray *clipArray = 0);
	virtual void endLayer();
	virtual void setAntialiasing(bool enable);
	virtual void begin();
	virtual void end();
	void clear();
	virtual void clear( const QColor & );

	// matrix manipulation
	virtual void setWorldMatrix( const QMatrix & );
	virtual const QMatrix worldMatrix();
	virtual void setZoomFactor( double );
	virtual double zoomFactor() { return m_zoomFactor; }
	virtual void translate( double, double );
	virtual void rotate( double );
	virtual void scale( double, double );

	// drawing
	virtual void moveTo( const double &, const double & );
	virtual void lineTo( const double &, const double & );
	virtual void curveTo( FPoint p1, FPoint p2, FPoint p3 );
	virtual void newPath();
	virtual void closePath();
	virtual void fillTextPath();
	virtual void strokeTextPath();
	virtual void fillPath();
	virtual void strokePath();
	virtual void setFillRule( bool fillRule );
	virtual bool fillRule() { return m_fillRule; }
	virtual void setFillMode( int fill );
	virtual void setGradient( VGradient::VGradientType mode, FPoint orig, FPoint vec, FPoint foc = FPoint(0,0));
	virtual void setPattern(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation);
	virtual void setClipPath();

	virtual void drawImage( QImage *image );
	virtual void setupPolygon(FPointArray *points, bool closed = true);
	virtual void drawPolygon();
	virtual void drawPolyLine();
	virtual void drawLine(FPoint start, FPoint end);
	virtual void drawRect(double, double, double, double);
	virtual void drawText(QRectF area, QString text);

	// pen + brush
	virtual QColor pen();
	virtual QColor brush();
	virtual void setPen( const QColor & );
	virtual void setPen( const QColor &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo );
	virtual void setPenOpacity( double op );
	virtual void setLineWidth( double w);
	virtual void setDash(const QVector<double>& array, double ofs);
	virtual void setBrush( const QColor & );
	virtual void setBrushOpacity( double op );
	virtual void setOpacity( double op );
	virtual void setFont( const QFont &f );
	virtual QFont font();

	// stack management
	virtual void save();
	virtual void restore();


	virtual void setRasterOp( int op );

	VGradient fill_gradient;
	VGradient stroke_gradient;
	ScPattern *m_pattern;

private:
	void drawVPath( int mode );
#ifdef HAVE_CAIRO
	cairo_t *m_cr;
	struct layerProp
	{
		cairo_surface_t *data;
		int blendmode;
		double tranparency;
		FPointArray groupClip;
		bool pushed;
	};
#else
	QPainter painter;
	QPainterPath m_path;
	struct layerProp
	{
		QImage *data;
		int blendmode;
		double tranparency;
		FPointArray groupClip;
		bool pushed;
	};
	Qt::PenStyle PLineStyle;
#endif
	QStack<layerProp> Layers;
	QImage *m_image;
	double  m_layerTransparency;
	int  m_blendMode;
	unsigned int m_width;
	unsigned int m_height;
	QMatrix m_matrix;
	QFont m_font;
	bool mf_underline;
	bool mf_strikeout;
	bool mf_shadow;
	bool mf_outlined;
	/*! \brief Filling */
	QColor m_fill;
	double fill_trans;
	bool m_fillRule;
	int fillMode;				// 0 = none, 1 = solid, 2 = gradient 3 = pattern
	int gradientMode;		// 1 = linear, 2 = radial
	double patternScaleX;
	double patternScaleY;
	double patternOffsetX;
	double patternOffsetY;
	double patternRotation;
	/*! \brief Stroking */
	QColor m_stroke;
	double stroke_trans;
	double LineWidth;

	/*! \brief Line End Style */
	Qt::PenCapStyle PLineEnd;
  /*! \brief Line Join Style */
	Qt::PenJoinStyle PLineJoin;
  /*! \brief The Dash Array */
	QVector<double> m_array;
	double m_offset;
	/*! \brief Zoom Factor of the Painter */
	double m_zoomFactor;
	bool imageMode;
	bool layeredMode;
	bool svgMode;
};

#endif
