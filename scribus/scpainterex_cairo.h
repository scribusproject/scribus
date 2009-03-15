/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCPAINTEREX_CAIRO_H
#define SCPAINTEREX_CAIRO_H

#include <cairo.h>
#include "scpainterexbase.h"

class ScPainterEx_Cairo : public ScPainterExBase
{
public:
	ScPainterEx_Cairo(cairo_t* context, QRect& rect, ScribusDoc* doc, bool gray );
	virtual ~ScPainterEx_Cairo();

	virtual Capabilities capabilities() { return transparencies; }

	virtual int supportedColorModes() { return (int) rgbMode; }
	virtual ColorMode preferredColorMode() { return rgbMode; }
	virtual ImageMode imageMode() { return rgbImages; }

	virtual void begin();
	virtual void end();
	virtual void clear();
	virtual void clear( ScColorShade & );

	// matrix manipulation
	virtual void setWorldMatrix( const QMatrix & );
	virtual const QMatrix worldMatrix();
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
	virtual int  fillMode() { return m_fillMode; }
	virtual void setGradient( VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc = FPoint(0,0));
	virtual void setPattern ( ScPattern* pattern, QMatrix& patternTransform );
	virtual void setClipPath();

	virtual void drawImage( ScImage *image, ScPainterExBase::ImageMode mode );
	virtual void setupPolygon(FPointArray *points, bool closed = true);
	virtual void drawPolygon();
	virtual void drawPolyLine();
	virtual void drawLine(FPoint start, FPoint end);
	virtual void drawRect(double, double, double, double);

	// pen + brush
	virtual ScColorShade pen();
	virtual ScColorShade brush();
	virtual void setPen( const ScColorShade & );
	virtual void setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo );
	virtual void setPenOpacity( double op );
	virtual void setLineWidth( double w);
	virtual void setDash(const QVector<double>& array, double ofs);
	virtual void setBrush( const ScColorShade & );
	virtual void setBrushOpacity( double op );
	virtual void setOpacity( double op );
	virtual void setFont( const QFont &f );
	virtual QFont font();

	// stack management
	virtual void save();
	virtual void restore();

	virtual void setRasterOp( int op );

private:

	void drawVPath( int mode );
	void drawGradient( VGradientEx& gradient );
	void drawLinearGradient( VGradientEx& gradient, const QRect& rect );
	void drawCircularGradient( VGradientEx& gradient, const QRect& rect );
	void getClipPathDimensions( QRect& r );

	ScribusDoc* m_doc;

	unsigned int m_width;
	unsigned int m_height;
	QMatrix m_matrix;
	QFont m_font;
/* Filling */
	ScColorShade m_fillColor;
	double m_fillTrans;
	bool   m_fillRule;
	int    m_fillMode;			// 0 = none, 1 = solid, 2 = gradient
	int    m_gradientMode;		// 1 = linear, 2 = radial
/* Stroking */
	ScColorShade m_strokeColor;
	double m_strokeTrans;
	double m_lineWidth;
/* Grayscale conversion option */
	bool   m_convertToGray;

/* Line End Style */
	Qt::PenCapStyle m_lineEnd;
/* Line Join Style */
	Qt::PenJoinStyle m_lineJoin;
/* The Dash Array */
	QVector<double> m_array;
	double m_offset;
/* Transformation Stack */
	QStack<QMatrix> m_stack;

/* Cairo context */
	cairo_t* m_cr;

/* Color conversion function */
	QColor transformColor( ScColorShade& colorShade, double trans );
	void   transformImage( QImage& image );

	QStack<int> m_gStates;
	double      m_positionX;
	double      m_positionY;
};

#endif
