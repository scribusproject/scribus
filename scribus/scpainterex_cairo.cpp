/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cmath>
#include "scpainterex_cairo.h"
#include "sccolorengine.h"
#include "util.h"

ScPainterEx_Cairo::ScPainterEx_Cairo( cairo_t* context, QRect& rect,  ScribusDoc* doc, bool gray ) : ScPainterExBase()
{
	m_doc    = doc;
	m_width  = rect.width();
	m_height = rect.height();
	m_strokeColor = ScColorShade( QColor(0,0,0), 100 );
	m_fillColor = ScColorShade( QColor(0,0,0), 100 );
	m_fillTrans = 1.0;
	m_strokeTrans = 1.0;
	m_fillRule  = true;
	m_fillMode  = 1;
	m_lineWidth = 1.0;
	m_offset    = 0;
	m_lineEnd   = Qt::SquareCap;
	m_lineJoin  = Qt::RoundJoin;
	m_fillGradient   = VGradientEx(VGradientEx::linear);
	m_strokeGradient = VGradientEx(VGradientEx::linear);
	// Grayscale conversion parameter
	m_convertToGray = gray;
	// Initialization of Windows GDI data
	m_cr = context;
	m_positionX = m_positionY = 0;
	clear();
}

ScPainterEx_Cairo::~ScPainterEx_Cairo()
{

}

QColor ScPainterEx_Cairo::transformColor( ScColorShade& colorShade, double trans )
{
	QColor c, color;
	c = ScColorEngine::getShadeColor(colorShade.color, m_doc, colorShade.shade);
	color = qRgba( qRed(c.rgb()), qGreen(c.rgb()), qBlue(c.rgb()), qAlpha(trans * 255));
	if ( m_convertToGray )
	{
		int grayLevel;
		QColor grayColor;
		grayLevel = 0.3 * color.red() + 0.59 * color.green() + 0.11 * color.blue();
		grayColor = qRgba( grayLevel, grayLevel, grayLevel, qAlpha(color.rgb()));
		color = grayColor;
	}
	return color;
}

void ScPainterEx_Cairo::transformImage( QImage* image, uchar* data, int scan)
{
	int rgb, grey;
	int imageWidth = image->width();
	int imageHeight = image->height();
	QRgb *pscani, *imageBits = (QRgb *) image->bits();
	uchar *pscand;

	pscand = data;
	for ( int j = 0; j < imageHeight; j++ )
	{
		pscani = (QRgb *) image->scanLine(j);
		for (int i = 0; i < imageWidth; i++ )
		{
			rgb = pscani[i];
			grey = 0.3 * qRed(rgb) + 0.59 * qGreen(rgb) + 0.11 * qBlue(rgb);
			pscand[i] = grey;
		}
		pscand += scan;
	}
}

void ScPainterEx_Cairo::begin()
{
}

void ScPainterEx_Cairo::end()
{
}

void ScPainterEx_Cairo::clear()
{
	/*SolidBrush brush( Color(255,255,255) );
	m_graphics->FillRectangle(&brush, 0, 0, m_width, m_height);*/
}

void ScPainterEx_Cairo::clear( ScColorShade &c )
{
	/*QColor color = transformColor( c, 1.0 );
	QRgb cs = color.rgb();
	SolidBrush brush( Color(qAlpha(cs), qRed(cs), qGreen(cs),qBlue(cs)) );
	m_graphics->FillRectangle(&brush, 0, 0, m_width, m_height);*/
}

const QMatrix ScPainterEx_Cairo::worldMatrix()
{
	cairo_matrix_t matrix;
	cairo_get_matrix(m_cr, &matrix);
	QMatrix mat(matrix.xx, matrix.yx, matrix.xy, matrix.yy, matrix.x0, matrix.y0);
	return mat;
}

void ScPainterEx_Cairo::setWorldMatrix( const QMatrix &mat )
{
	cairo_matrix_t matrix;
	cairo_matrix_init(&matrix, mat.m11(), mat.m12(), mat.m21(), mat.m22(), mat.dx(), mat.dy());
	cairo_set_matrix(m_cr, &matrix);
}

void ScPainterEx_Cairo::translate( double x, double y )
{
	cairo_translate(m_cr, x, y);
}

void ScPainterEx_Cairo::rotate( double r )
{
	cairo_rotate(m_cr, r * 3.1415927 / 180.0);
}

void ScPainterEx_Cairo::scale( double x, double y )
{
	cairo_scale(m_cr, x, y);
}

void ScPainterEx_Cairo::moveTo( const double &x, const double &y )
{
	cairo_move_to(m_cr, x, y);
}

void ScPainterEx_Cairo::lineTo( const double &x, const double &y )
{
	cairo_line_to(m_cr, x, y);
}

void ScPainterEx_Cairo::curveTo( FPoint p1, FPoint p2, FPoint p3 )
{
	cairo_curve_to(m_cr, p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y() );
}

void ScPainterEx_Cairo::newPath()
{
	cairo_new_path( m_cr );
}

void ScPainterEx_Cairo::closePath()
{
	cairo_close_path( m_cr);
}

void ScPainterEx_Cairo::setFillRule( bool fillRule )
{
	m_fillRule = fillRule;
}

void ScPainterEx_Cairo::setFillMode( int fill )
{
	m_fillMode = fill;
}

void ScPainterEx_Cairo::setGradient(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc)
{
	m_fillGradient.setType(mode);
	m_fillGradient.setOrigin(orig);
	m_fillGradient.setVector(vec);
	m_fillGradient.setFocalPoint(foc);
}

void ScPainterEx_Cairo::setPattern( ScPattern* pattern, QMatrix& patternTransform )
{
	m_pattern = pattern;
	m_patternTransform = patternTransform;
}

void ScPainterEx_Cairo::fillTextPath()
{
	drawVPath( 0 );
}

void ScPainterEx_Cairo::strokeTextPath()
{
	if( m_lineWidth == 0 )
		return;
	drawVPath( 1 );
}

void ScPainterEx_Cairo::fillPath()
{
	if( m_fillMode != 0)
		drawVPath( 0 );
}

void ScPainterEx_Cairo::strokePath()
{
	if( m_lineWidth == 0 )
		return;
	drawVPath( 1 );
}

ScColorShade ScPainterEx_Cairo::pen()
{
	return m_strokeColor;
}

ScColorShade ScPainterEx_Cairo::brush()
{
	return m_fillColor;
}

void ScPainterEx_Cairo::setPen( const ScColorShade &c )
{
	m_strokeColor = c;
}

void ScPainterEx_Cairo::setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo )
{
	m_strokeColor = c;
	m_lineWidth   = w;
	m_lineEnd     = ca;
	m_lineJoin    = jo;
	m_array.clear();
	m_offset      = 0;
	getDashArray(st, w, m_array);
}

void ScPainterEx_Cairo::setLineWidth( double w )
{
	m_lineWidth = w;
}

void ScPainterEx_Cairo::setPenOpacity( double op )
{
	m_strokeTrans = op;
}

void ScPainterEx_Cairo::setDash(const QVector<double>& array, double ofs)
{
	m_array = array;
	m_offset = ofs;
}

void ScPainterEx_Cairo::setBrush( const ScColorShade &c )
{
	m_fillColor = c;
}

void ScPainterEx_Cairo::setBrushOpacity( double op )
{
	m_fillTrans = op;
}

void ScPainterEx_Cairo::setOpacity( double op )
{
	m_fillTrans = op;
	m_strokeTrans = op;
}

void ScPainterEx_Cairo::setFont( const QFont &f)
{
	m_font = f;
}

QFont ScPainterEx_Cairo::font()
{
	return m_font;
}

void ScPainterEx_Cairo::save()
{
	cairo_save(m_cr);
}

void ScPainterEx_Cairo::restore()
{
	cairo_restore( m_cr );
}

void ScPainterEx_Cairo::setRasterOp( int   )
{
}

void ScPainterEx_Cairo::drawVPath( int mode )
{
	cairo_save( m_cr );
	if (mode == 0)
	{
		if( m_fillRule )
			cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
		else
			cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_WINDING);
		if (m_fillMode == ScPainterExBase::Gradient)
		{
			drawGradient( m_fillGradient );
		}
		else
		{
			double r, g, b;
			QColor fillColor = transformColor(m_fillColor, 1.0);
			fillColor.getRgbF(&r, &g, &b);
			cairo_set_source_rgba( m_cr, r, g, b, m_fillTrans );
			if (m_fillTrans != 1.0)
				cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
			else
				cairo_set_operator(m_cr, CAIRO_OPERATOR_SOURCE);
			cairo_fill_preserve( m_cr );
		}
	}
	else
	{
		double r, g, b;
		QColor strokeColor = transformColor( m_strokeColor, 1.0 );
		strokeColor.getRgbF(&r, &g, &b);
		cairo_set_source_rgba( m_cr, r, g, b, m_strokeTrans );
		
		cairo_set_line_width( m_cr, m_lineWidth );
		if( m_array.count() > 0 )
			cairo_set_dash( m_cr, m_array.data(), m_array.count(), static_cast<double>(m_offset));
		else
			cairo_set_dash( m_cr, NULL, 0, 0 );
		if (m_strokeTrans != 1.0)
			cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
		else
			cairo_set_operator(m_cr, CAIRO_OPERATOR_SOURCE);
		if( m_lineEnd == Qt::RoundCap )
			cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_ROUND);
		else if( m_lineEnd == Qt::SquareCap )
			cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_SQUARE);
		else if( m_lineEnd == Qt::FlatCap )
			cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_BUTT);
		if( m_lineJoin == Qt::RoundJoin )
			cairo_set_line_join( m_cr, CAIRO_LINE_JOIN_ROUND );
		else if( m_lineJoin == Qt::BevelJoin )
			cairo_set_line_join( m_cr, CAIRO_LINE_JOIN_BEVEL );
		else if( m_lineJoin == Qt::MiterJoin )
			cairo_set_line_join( m_cr, CAIRO_LINE_JOIN_MITER );
		cairo_stroke_preserve( m_cr );
	}
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	cairo_restore( m_cr );
}

void ScPainterEx_Cairo::setClipPath()
{
	cairo_clip (m_cr);
}

void ScPainterEx_Cairo::drawImage( ScImage *image, ScPainterExBase::ImageMode mode )
{
	cairo_save(m_cr);
	cairo_set_fill_rule(m_cr, cairo_get_fill_rule(m_cr));
	cairo_surface_t *image2 = cairo_image_surface_create_for_data ((uchar*)image->qImage().bits(), CAIRO_FORMAT_ARGB32, image->width(), image->height(), image->width()*4);
	cairo_set_source_surface (m_cr, image2, 0, 0);
	cairo_paint_with_alpha (m_cr, m_fillTrans);
	cairo_surface_destroy (image2);
	cairo_restore(m_cr);
}

void ScPainterEx_Cairo::setupPolygon(FPointArray *points, bool closed)
{
	bool nPath = true, first = true;
	FPoint np, np1, np2, np3;
	if (points->size() > 3)
	{
		newPath();
		for (uint poi=0; poi<points->size()-3; poi += 4)
		{
			if (points->point(poi).x() > 900000)
			{
				nPath = true;
				continue;
			}
			if (nPath)
			{
				np = points->point(poi);
				if ((!first) && (closed))
					cairo_close_path( m_cr );
				moveTo( np.x(), np.y() );
				nPath = first = false;
			}
			np  = points->point(poi);
			np1 = points->point(poi+1);
			np2 = points->point(poi+3);
			np3 = points->point(poi+2);
			if ((np == np1) && (np2 == np3))
				lineTo( np3.x(), np3.y() );
			else
				curveTo(np1, np2, np3);
		}
		if (closed)
			cairo_close_path( m_cr );
	}
}

void ScPainterEx_Cairo::drawPolygon()
{
	fillPath();
}

void ScPainterEx_Cairo::drawPolyLine()
{
	strokePath();
}

void ScPainterEx_Cairo::drawLine(FPoint start, FPoint end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainterEx_Cairo::drawRect(double x, double y, double w, double h)
{
	newPath();
	moveTo( x, y );
	lineTo( x+w, y );
	lineTo( x+w, y+h );
	lineTo( x, y+h );
	lineTo( x, y );
	cairo_close_path( m_cr );
	fillPath();
	strokePath();
}

void ScPainterEx_Cairo::drawGradient( VGradientEx& gradient )
{
	QRect clipPathRect;
	save();
	setClipPath();
	getClipPathDimensions( clipPathRect );
	if ( gradient.type() == VGradientEx::linear )
		drawLinearGradient( gradient, clipPathRect );
	else if ( gradient.type() == VGradientEx::radial )
		drawCircularGradient( gradient, clipPathRect );
	restore();
}

void ScPainterEx_Cairo::drawLinearGradient( VGradientEx& gradient, const QRect& rect )
{
	int index = 0;
	double r, g, b;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx* stop = NULL;
	QColor color;

	if ( gradient.Stops() < 2 )
		return;

	FPoint p1( gradient.origin().x(), gradient.origin().y() );
	FPoint p2( gradient.vector().x(), gradient.vector().y() );
	cairo_pattern_t *pat = cairo_pattern_create_linear (p1.x(), p1.y(), p2.x(), p2.y());	

	for( uint index = 0 ; index < gradient.Stops(); index++)
	{
		stop  = colorStops.at(index);
		color = transformColor( ScColorShade(stop->color, stop->shade), 1.0 );
		color.getRgbF(&r, &g, &b);
		cairo_pattern_add_color_stop_rgba (pat, stop->rampPoint, r, g, b, stop->opacity);
	}

	cairo_set_source (m_cr, pat);
	//cairo_clip_preserve (m_cr);
	cairo_paint_with_alpha (m_cr, m_fillTrans);
	cairo_pattern_destroy (pat);
}

void ScPainterEx_Cairo::drawCircularGradient( VGradientEx& gradient, const QRect& rect )
{
	int offset = 0;
	double r, g, b;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	VColorStopEx* stop = NULL;
	QColor color;

	FPoint pc( gradient.origin().x(), gradient.origin().y() );
	FPoint pf( gradient.focalPoint().x(), gradient.focalPoint().y() );
	FPoint pv( gradient.vector().x(), gradient.vector().y() );
	double rad = sqrt( pow(pv.x() - pc.x(), 2) + pow(pv.y() - pc.y(), 2) );

	cairo_pattern_t* pat = cairo_pattern_create_radial (pc.x(), pc.y(), 0.1, pc.x(), pc.y(), rad);	

	for( uint index = 0 ; index < gradient.Stops() ; index++)
	{
		stop  = colorStops.at(index);
		color = transformColor( ScColorShade(stop->color, stop->shade), 1.0 );
		color.getRgbF(&r, &g, &b);
		cairo_pattern_add_color_stop_rgba (pat, stop->rampPoint, r, g, b, stop->opacity);
	}
	
	cairo_set_source (m_cr, pat);
	//cairo_clip_preserve (m_cr);
	cairo_paint_with_alpha (m_cr, m_fillTrans);
	cairo_pattern_destroy (pat);
}

void ScPainterEx_Cairo::getClipPathDimensions( QRect& r )
{
	double left = 0.0, right = 0.0, bottom = 0.0, top = 0.0;
	cairo_clip_extents(m_cr, &left, &bottom, &right, &top);
	r.setLeft( left );
	r.setRight( right );
	r.setBottom( bottom );
	r.setTop( top );
}