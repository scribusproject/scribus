/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scpainterex_cairo.h"
#include "sccolorengine.h"
#include "util.h"
#include "util_math.h"

ScPainterEx_Cairo::ScPainterEx_Cairo( cairo_t* context, QRect& rect,  ScribusDoc* doc, bool gray ) : ScPainterExBase()
{
	m_doc    = doc;
	m_width  = rect.width();
	m_height = rect.height();
	m_blendModeLayer = 0;
	m_blendModeFill = 0;
	m_blendModeStroke = 0;
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

void ScPainterEx_Cairo::transformImage(QImage& image)
{
	int rgb, grey;
	int imageWidth = image.width();
	int imageHeight = image.height();
	QRgb *pscan;

	for ( int j = 0; j < imageHeight; j++ )
	{
		pscan = (QRgb *) image.scanLine(j);
		for (int i = 0; i < imageWidth; i++ )
		{
			rgb = pscan[i];
			grey = 0.3 * qRed(rgb) + 0.59 * qGreen(rgb) + 0.11 * qBlue(rgb);
			pscan[i] = qRgba(grey, grey, grey, qAlpha(rgb));
		}
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

const QTransform ScPainterEx_Cairo::worldMatrix()
{
	cairo_matrix_t matrix;
	cairo_get_matrix(m_cr, &matrix);
	QTransform mat(matrix.xx, matrix.yx, matrix.xy, matrix.yy, matrix.x0, matrix.y0);
	return mat;
}

void ScPainterEx_Cairo::setWorldMatrix( const QTransform &mat )
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

void ScPainterEx_Cairo::setStrokeMode( int stroke )
{
	m_strokeMode = stroke;
}

void ScPainterEx_Cairo::setGradient(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew)
{
	m_fillGradient.setType(mode);
	m_fillGradient.setOrigin(orig);
	m_fillGradient.setVector(vec);
	m_fillGradient.setFocalPoint(foc);

	m_strokeGradient.setType(mode);
	m_strokeGradient.setOrigin(orig);
	m_strokeGradient.setVector(vec);
	m_strokeGradient.setFocalPoint(foc);

	m_gradientScale = scale;
	if (skew == 90)
		m_gradientSkew = 1;
	else if (skew == 180)
		m_gradientSkew = 0;
	else if (skew == 270)
		m_gradientSkew = -1;
	else if (skew == 360)
		m_gradientSkew = 0;
	else
		m_gradientSkew = tan(M_PI / 180.0 * skew);
}

void ScPainterEx_Cairo::setMaskMode(int mask)
{
	m_maskMode = mask;
}

void ScPainterEx_Cairo::setGradientMask(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew)
{
	m_maskGradient.setType(mode);
	m_maskGradient.setOrigin(orig);
	m_maskGradient.setVector(vec);
	m_maskGradient.setFocalPoint(foc);
	m_maskGradientScale = scale;
	if (skew == 90)
		m_maskGradientSkew = 1;
	else if (skew == 180)
		m_maskGradientSkew = 0;
	else if (skew == 270)
		m_maskGradientSkew = -1;
	else if (skew == 360)
		m_maskGradientSkew = 0;
	else
		m_maskGradientSkew = tan(M_PI / 180.0 * skew);
}

void ScPainterEx_Cairo::setPatternMask(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	m_maskPattern = pattern;
	m_maskPatternScaleX = scaleX / 100.0;
	m_maskPatternScaleY = scaleY / 100.0;
	m_maskPatternOffsetX = offsetX;
	m_maskPatternOffsetY = offsetY;
	m_maskPatternRotation = rotation;
	m_maskPatternSkewX = skewX;
	m_maskPatternSkewY = skewY;
	m_maskPatternMirrorX = mirrorX;
	m_maskPatternMirrorY = mirrorY;
}

void ScPainterEx_Cairo::setPattern(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	m_pattern = pattern;
	m_patternScaleX = scaleX / 100.0;
	m_patternScaleY = scaleY / 100.0;
	m_patternOffsetX = offsetX;
	m_patternOffsetY = offsetY;
	m_patternRotation = rotation;
	m_patternSkewX   = skewX;
	m_patternSkewY   = skewY;
	m_patternMirrorX = mirrorX;
	m_patternMirrorY = mirrorY;
}

void ScPainterEx_Cairo::set4ColorGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4)
{
	m_fillGradient.setType(VGradientEx::fourcolor);
	m_gradPatchP1 = p1;
	m_gradPatchP2 = p2;
	m_gradPatchP3 = p3;
	m_gradPatchP4 = p4;
	m_gradControlP1 = c1;
	m_gradControlP2 = c2;
	m_gradControlP3 = c3;
	m_gradControlP4 = c4;
}

void ScPainterEx_Cairo::set4ColorColors(const ScColorShade&  col1, const ScColorShade&  col2, const ScColorShade&  col3, const ScColorShade&  col4)
{
	m_gradPatchColor1 = col1;
	m_gradPatchColor2 = col2;
	m_gradPatchColor3 = col3;
	m_gradPatchColor4 = col4;
}

void ScPainterEx_Cairo::setDiamondGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4, FPoint c5)
{
	m_fillGradient.setType(VGradientEx::diamond);
	m_gradPatchP1 = p1;
	m_gradPatchP2 = p2;
	m_gradPatchP3 = p3;
	m_gradPatchP4 = p4;
	m_gradControlP1 = c1;
	m_gradControlP2 = c2;
	m_gradControlP3 = c3;
	m_gradControlP4 = c4;
	m_gradControlP5 = c5;
}

void ScPainterEx_Cairo::setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<QList<meshPoint> > meshArray)
{
	m_fillGradient.setType(VGradientEx::mesh);
	m_meshGradientArray = meshArray;
	m_gradPatchP1 = p1;
	m_gradPatchP2 = p2;
	m_gradPatchP3 = p3;
	m_gradPatchP4 = p4;
}

void ScPainterEx_Cairo::setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<meshGradientPatch> meshPatches)
{
	m_fillGradient.setType(VGradientEx::freemesh);
	m_meshGradientPatches = meshPatches;
	m_gradPatchP1 = p1;
	m_gradPatchP2 = p2;
	m_gradPatchP3 = p3;
	m_gradPatchP4 = p4;
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

void ScPainterEx_Cairo::setRasterOp(int blendMode)
{
	if (blendMode == 0)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	else if (blendMode == 1)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_DARKEN);
	else if (blendMode == 2)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_LIGHTEN);
	else if (blendMode == 3)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_MULTIPLY);
	else if (blendMode == 4)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_SCREEN);
	else if (blendMode == 5)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_OVERLAY);
	else if (blendMode == 6)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_HARD_LIGHT);
	else if (blendMode == 7)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_SOFT_LIGHT);
	else if (blendMode == 8)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_DIFFERENCE);
	else if (blendMode == 9)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_EXCLUSION);
	else if (blendMode == 10)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_COLOR_DODGE);
	else if (blendMode == 11)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_COLOR_BURN);
	else if (blendMode == 12)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_HSL_HUE);
	else if (blendMode == 13)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_HSL_SATURATION);
	else if (blendMode == 14)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_HSL_COLOR);
	else if (blendMode == 15)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_HSL_LUMINOSITY);
	else
		cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
}

void ScPainterEx_Cairo::setBlendModeFill( int blendMode )
{
	m_blendModeFill = blendMode;
}

void ScPainterEx_Cairo::setBlendModeStroke( int blendMode )
{
	m_blendModeStroke = blendMode;
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
		if (m_fillMode == ScPainterExBase::Solid)
		{
			double r, g, b;
			QColor fillColor = transformColor(m_fillColor, 1.0);
			fillColor.getRgbF(&r, &g, &b);
			if (m_maskMode > 0)
			{
				/*cairo_pattern_t *pat = getMaskPattern();
				cairo_set_source_rgb( m_cr, r, g, b );
				setRasterOp(m_blendModeFill);
				cairo_clip_preserve(m_cr);
				cairo_mask(m_cr, pat);
				if ((maskMode == 2) || (maskMode == 4) || (maskMode == 5) || (maskMode == 6))
					cairo_surface_destroy(imageMask);
				cairo_pattern_destroy(pat);*/
			}
			else
			{
				cairo_set_source_rgba(m_cr, r, g, b, m_fillTrans);
				setRasterOp(m_blendModeFill);
				cairo_fill_preserve(m_cr);
			}
		}
		else if (m_fillMode == ScPainterExBase::Gradient)
		{
			drawGradient( m_fillGradient );
		}
		else if (m_fillMode == ScPainterExBase::Pattern)
		{

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
	QImage qImage(image->qImage());
	if (m_convertToGray)
		transformImage(qImage);
	cairo_save(m_cr);
	cairo_set_fill_rule(m_cr, cairo_get_fill_rule(m_cr));
	cairo_surface_t *image2 = cairo_image_surface_create_for_data ((uchar*)qImage.bits(), CAIRO_FORMAT_ARGB32, qImage.width(), qImage.height(), qImage.width()*4);
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
	else if ( gradient.type() == VGradientEx::fourcolor )
		drawFourColorGradient( clipPathRect );
	else if ( gradient.type() == VGradientEx::diamond )
		drawDiamondGradient( gradient, clipPathRect );
	else if ( gradient.type() == VGradientEx::mesh )
		drawMeshGradient( clipPathRect );
	else if ( gradient.type() == VGradientEx::freemesh )
		drawFreeMeshGradient( clipPathRect );
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

	bool   isFirst  = true;
	double lastStop = 0.0;
	for( uint index = 0 ; index < gradient.Stops(); index++)
	{
		stop  = colorStops.at(index);
		if ((lastStop == stop->rampPoint) && (!isFirst))
			continue;
		isFirst = false;
		color = transformColor( ScColorShade(stop->color, stop->shade), 1.0 );
		color.getRgbF(&r, &g, &b);
		cairo_pattern_add_color_stop_rgba (pat, stop->rampPoint, r, g, b, stop->opacity);
		lastStop = stop->rampPoint;
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

	bool   isFirst  = true;
	double lastStop = 0.0;
	for( uint index = 0 ; index < gradient.Stops() ; index++)
	{
		stop  = colorStops.at(index);
		if ((lastStop == stop->rampPoint) && (!isFirst))
			continue;
		isFirst = false;
		color = transformColor( ScColorShade(stop->color, stop->shade), 1.0 );
		color.getRgbF(&r, &g, &b);
		cairo_pattern_add_color_stop_rgba (pat, stop->rampPoint, r, g, b, stop->opacity);
		lastStop = stop->rampPoint;
	}
	
	cairo_set_source (m_cr, pat);
	//cairo_clip_preserve (m_cr);
	cairo_paint_with_alpha (m_cr, m_fillTrans);
	cairo_pattern_destroy (pat);
}

void ScPainterEx_Cairo::drawFourColorGradient( const QRect& rect )
{
	cairo_pattern_t *pat = NULL;
	cairo_surface_t *img = NULL;
	cairo_t *cr = NULL;
	cairo_pattern_t *mpat = NULL;
	QColor color;

	double p1x = m_gradPatchP1.x();
	double p1y = m_gradPatchP1.y();
	double p2x = m_gradPatchP2.x();
	double p2y = m_gradPatchP2.y();
	double p3x = m_gradPatchP3.x();
	double p3y = m_gradPatchP3.y();
	double p4x = m_gradPatchP4.x();
	double p4y = m_gradPatchP4.y();
	img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, p3x, p3y);
	cr = cairo_create(img);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_tolerance(cr, 0.5 );
	double r, g, b, a;
	mpat = cairo_pattern_create_mesh();
	cairo_pattern_mesh_begin_patch(mpat);
	cairo_pattern_mesh_move_to(mpat, p1x, p1y);
	cairo_pattern_mesh_line_to(mpat, p2x, p2y);
	cairo_pattern_mesh_line_to(mpat, p3x, p3y);
	cairo_pattern_mesh_line_to(mpat, p4x, p4y);
	cairo_pattern_mesh_line_to(mpat, p1x, p1y);
	cairo_pattern_mesh_set_control_point(mpat, 0, m_gradControlP1.x(),  m_gradControlP1.y());
	cairo_pattern_mesh_set_control_point(mpat, 1, m_gradControlP2.x(),  m_gradControlP2.y());
	cairo_pattern_mesh_set_control_point(mpat, 2, m_gradControlP3.x(),  m_gradControlP3.y());
	cairo_pattern_mesh_set_control_point(mpat, 3, m_gradControlP4.x(),  m_gradControlP4.y());
	color = transformColor(m_gradPatchColor1, 1.0);
	color.getRgbF(&r, &g, &b, &a);
	cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
	color = transformColor(m_gradPatchColor2, 1.0);
	color.getRgbF(&r, &g, &b, &a);
	cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
	color = transformColor(m_gradPatchColor3, 1.0);
	color.getRgbF(&r, &g, &b, &a);
	cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
	color = transformColor(m_gradPatchColor4, 1.0);
	color.getRgbF(&r, &g, &b, &a);
	cairo_pattern_mesh_set_corner_color_rgba(mpat, 3, r, g, b, a);
	cairo_pattern_mesh_end_patch(mpat);
	cairo_pattern_set_filter(mpat, CAIRO_FILTER_GOOD);
	cairo_set_source(cr, mpat);
	cairo_paint_with_alpha(cr, 1.0);
	pat = cairo_pattern_create_for_surface(img);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);

	cairo_set_source (m_cr, pat);
	cairo_clip_preserve (m_cr);

	if (m_maskMode > 0)
	{
		/*cairo_pattern_t *patM = getMaskPattern();
		setRasterOp(m_blendModeFill);
		cairo_mask(m_cr, patM);
		if ((m_maskMode == 2) || (m_maskMode == 4) || (m_maskMode == 5) || (m_maskMode == 6))
			cairo_surface_destroy(imageMask);
		cairo_pattern_destroy (patM);*/
	}
	else
	{
		setRasterOp(m_blendModeFill);
		cairo_paint_with_alpha (m_cr, m_fillTrans);
	}
	cairo_pattern_destroy (pat);

	if (img) cairo_surface_destroy(img);
	if (mpat) cairo_pattern_destroy(mpat);
	if (cr) cairo_destroy( cr );
}

void ScPainterEx_Cairo::drawDiamondGradient( VGradientEx& gradient, const QRect& rect )
{
	cairo_pattern_t *pat = NULL;
	cairo_surface_t *img = NULL;
	cairo_t *cr = NULL;
	cairo_pattern_t *mpat = NULL;
	VColorStopEx* stop = NULL;
	QColor color;

	double p1x = m_gradControlP1.x();
	double p1y = m_gradControlP1.y();
	double p2x = m_gradControlP2.x();
	double p2y = m_gradControlP2.y();
	double p3x = m_gradControlP3.x();
	double p3y = m_gradControlP3.y();
	double p4x = m_gradControlP4.x();
	double p4y = m_gradControlP4.y();
	img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, m_gradPatchP3.x(), m_gradPatchP3.y());
	cr = cairo_create(img);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_tolerance(cr, 0.5 );
	double r, g, b, a;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	QList<QColor> qStopColors;
	QColor qStopColor;
	for( int offset = 0 ; offset < colorStops.count() ; offset++ )
	{
		stop = colorStops.at(offset);
		qStopColor = transformColor( ScColorShade(stop->color, stop->shade), 1.0);
		int h, s, v, sneu, vneu;
		int shad = stop->shade;
		qStopColor.getHsv(&h, &s, &v);
		sneu = s * shad / 100;
		vneu = 255 - ((255 - v) * shad / 100);
		qStopColor.setHsv(h, sneu, vneu);
		qStopColor.setAlphaF(colorStops[offset]->opacity);
		qStopColors.append(qStopColor);
	}
	qStopColors[qStopColors.count()-1].getRgbF(&r, &g, &b, &a);
	QPointF centerP = QPointF(m_gradControlP5.x(), m_gradControlP5.y());
	QLineF edge1 = QLineF(centerP, QPointF(p1x, p1y));
	QLineF edge2 = QLineF(centerP, QPointF(p2x, p2y));
	QLineF edge3 = QLineF(centerP, QPointF(p3x, p3y));
	QLineF edge4 = QLineF(centerP, QPointF(p4x, p4y));
	QPointF p1 = edge1.pointAt(colorStops[colorStops.count() - 1]->rampPoint);
	QPointF p2 = edge2.pointAt(colorStops[colorStops.count() - 1]->rampPoint);
	QPointF p3 = edge3.pointAt(colorStops[colorStops.count() - 1]->rampPoint);
	QPointF p4 = edge4.pointAt(colorStops[colorStops.count() - 1]->rampPoint);
	cairo_set_source_rgba(cr, r, g, b, a);
	cairo_paint_with_alpha(cr, 1.0);
	cairo_set_operator(cr, CAIRO_OPERATOR_DEST_OUT);
	cairo_new_path(cr);
	cairo_move_to(cr, p1.x(), p1.y());
	cairo_line_to(cr, p2.x(), p2.y());
	cairo_line_to(cr, p3.x(), p3.y());
	cairo_line_to(cr, p4.x(), p4.y());
	cairo_close_path(cr);
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_fill(cr);
	cairo_set_operator(cr, CAIRO_OPERATOR_ADD);
	mpat = cairo_pattern_create_mesh();
	for( int offset = 1 ; offset < colorStops.count() ; offset++ )
	{
		QLineF e1 = edge1;
		QLineF e1s = edge1;
		QLineF e2 = edge2;
		QLineF e2s = edge2;
		QLineF e3 = edge3;
		QLineF e3s = edge3;
		QLineF e4 = edge4;
		QLineF e4s = edge4;
		e1.setLength(edge1.length() * colorStops[ offset ]->rampPoint);
		e2.setLength(edge2.length() * colorStops[ offset ]->rampPoint);
		e3.setLength(edge3.length() * colorStops[ offset ]->rampPoint);
		e4.setLength(edge4.length() * colorStops[ offset ]->rampPoint);
		e1s.setLength(edge1.length() * colorStops[ offset - 1 ]->rampPoint);
		e2s.setLength(edge2.length() * colorStops[ offset - 1 ]->rampPoint);
		e3s.setLength(edge3.length() * colorStops[ offset - 1 ]->rampPoint);
		e4s.setLength(edge4.length() * colorStops[ offset - 1 ]->rampPoint);
		if (offset == 1)
		{
			cairo_pattern_mesh_begin_patch(mpat);
			cairo_pattern_mesh_move_to(mpat, centerP.x(), centerP.y());
			cairo_pattern_mesh_line_to(mpat, e1.x2(), e1.y2());
			cairo_pattern_mesh_line_to(mpat, e2.x2(), e2.y2());
			cairo_pattern_mesh_line_to(mpat, centerP.x(), centerP.y());
			qStopColors[0].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
			cairo_pattern_mesh_end_patch(mpat);
			cairo_pattern_mesh_begin_patch(mpat);
			cairo_pattern_mesh_move_to(mpat, centerP.x(), centerP.y());
			cairo_pattern_mesh_line_to(mpat, e2.x2(), e2.y2());
			cairo_pattern_mesh_line_to(mpat, e3.x2(), e3.y2());
			cairo_pattern_mesh_line_to(mpat, centerP.x(), centerP.y());
			qStopColors[0].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
			cairo_pattern_mesh_end_patch(mpat);
			cairo_pattern_mesh_begin_patch(mpat);
			cairo_pattern_mesh_move_to(mpat, centerP.x(), centerP.y());
			cairo_pattern_mesh_line_to(mpat, e3.x2(), e3.y2());
			cairo_pattern_mesh_line_to(mpat, e4.x2(), e4.y2());
			cairo_pattern_mesh_line_to(mpat, centerP.x(), centerP.y());
			qStopColors[0].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
			cairo_pattern_mesh_end_patch(mpat);
			cairo_pattern_mesh_begin_patch(mpat);
			cairo_pattern_mesh_move_to(mpat, centerP.x(), centerP.y());
			cairo_pattern_mesh_line_to(mpat, e4.x2(), e4.y2());
			cairo_pattern_mesh_line_to(mpat, e1.x2(), e1.y2());
			cairo_pattern_mesh_line_to(mpat, centerP.x(), centerP.y());
			qStopColors[0].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
			cairo_pattern_mesh_end_patch(mpat);
		}
		else
		{
			cairo_pattern_mesh_begin_patch(mpat);
			cairo_pattern_mesh_move_to(mpat, e1s.x2(), e1s.y2());
			cairo_pattern_mesh_line_to(mpat, e1.x2(), e1.y2());
			cairo_pattern_mesh_line_to(mpat, e2.x2(), e2.y2());
			cairo_pattern_mesh_line_to(mpat, e2s.x2(), e2s.y2());
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[offset].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_pattern_mesh_end_patch(mpat);
			cairo_pattern_mesh_begin_patch(mpat);
			cairo_pattern_mesh_move_to(mpat, e2s.x2(), e2s.y2());
			cairo_pattern_mesh_line_to(mpat, e2.x2(), e2.y2());
			cairo_pattern_mesh_line_to(mpat, e3.x2(), e3.y2());
			cairo_pattern_mesh_line_to(mpat, e3s.x2(), e3s.y2());
			qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[offset].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_pattern_mesh_end_patch(mpat);
			cairo_pattern_mesh_begin_patch(mpat);
			cairo_pattern_mesh_move_to(mpat, e3s.x2(), e3s.y2());
			cairo_pattern_mesh_line_to(mpat, e3.x2(), e3.y2());
			cairo_pattern_mesh_line_to(mpat, e4.x2(), e4.y2());
			cairo_pattern_mesh_line_to(mpat, e4s.x2(), e4s.y2());
			qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[offset].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_pattern_mesh_end_patch(mpat);
			cairo_pattern_mesh_begin_patch(mpat);
			cairo_pattern_mesh_move_to(mpat, e4s.x2(), e4s.y2());
			cairo_pattern_mesh_line_to(mpat, e4.x2(), e4.y2());
			cairo_pattern_mesh_line_to(mpat, e1.x2(), e1.y2());
			cairo_pattern_mesh_line_to(mpat, e1s.x2(), e1s.y2());
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[offset].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_pattern_mesh_end_patch(mpat);
		}
	}
	cairo_pattern_set_filter(mpat, CAIRO_FILTER_GOOD);
	cairo_set_source(cr, mpat);
	cairo_paint_with_alpha(cr, 1.0);
	pat = cairo_pattern_create_for_surface(img);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);

	cairo_set_source (m_cr, pat);
	cairo_clip_preserve (m_cr);

	if (m_maskMode > 0)
	{
		/*cairo_pattern_t *patM = getMaskPattern();
		setRasterOp(m_blendModeFill);
		cairo_mask(m_cr, patM);
		if ((m_maskMode == 2) || (m_maskMode == 4) || (m_maskMode == 5) || (m_maskMode == 6))
			cairo_surface_destroy(imageMask);
		cairo_pattern_destroy (patM);*/
	}
	else
	{
		setRasterOp(m_blendModeFill);
		cairo_paint_with_alpha (m_cr, m_fillTrans);
	}
	cairo_pattern_destroy (pat);

	if (img) cairo_surface_destroy(img);
	if (mpat) cairo_pattern_destroy(mpat);
	if (cr) cairo_destroy( cr );
}

void ScPainterEx_Cairo::drawMeshGradient( const QRect& rect )
{
	cairo_pattern_t *pat = NULL;
	cairo_surface_t *img = NULL;
	cairo_t *cr = NULL;
	cairo_pattern_t *mpat = NULL;
	VColorStopEx* stop = NULL;
	QColor color;

	double p3x = m_gradPatchP3.x();
	double p3y = m_gradPatchP3.y();
	img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, p3x, p3y);
	cr  = cairo_create(img);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_tolerance(cr, 0.5 );
	double r, g, b, a;
	mpat = cairo_pattern_create_mesh();
	for (int grow = 0; grow < m_meshGradientArray.count()-1; grow++)
	{
		for (int gcol = 0; gcol < m_meshGradientArray[grow].count()-1; gcol++)
		{
			meshPoint mp1 = m_meshGradientArray[grow][gcol];
			meshPoint mp2 = m_meshGradientArray[grow][gcol+1];
			meshPoint mp3 = m_meshGradientArray[grow+1][gcol+1];
			meshPoint mp4 = m_meshGradientArray[grow+1][gcol];
			cairo_pattern_mesh_begin_patch(mpat);
			cairo_pattern_mesh_move_to(mpat, mp1.gridPoint.x(), mp1.gridPoint.y());
			cairo_pattern_mesh_curve_to(mpat, mp1.controlRight.x(), mp1.controlRight.y(), mp2.controlLeft.x(), mp2.controlLeft.y(), mp2.gridPoint.x(), mp2.gridPoint.y());
			cairo_pattern_mesh_curve_to(mpat, mp2.controlBottom.x(), mp2.controlBottom.y(), mp3.controlTop.x(), mp3.controlTop.y(), mp3.gridPoint.x(), mp3.gridPoint.y());
			cairo_pattern_mesh_curve_to(mpat, mp3.controlLeft.x(), mp3.controlLeft.y(), mp4.controlRight.x(), mp4.controlRight.y(), mp4.gridPoint.x(), mp4.gridPoint.y());
			cairo_pattern_mesh_curve_to(mpat, mp4.controlTop.x(), mp4.controlTop.y(), mp1.controlBottom.x(), mp1.controlBottom.y(), mp1.gridPoint.x(), mp1.gridPoint.y());
			cairo_pattern_mesh_set_control_point(mpat, 0, mp1.controlColor.x(),  mp1.controlColor.y());
			cairo_pattern_mesh_set_control_point(mpat, 1, mp2.controlColor.x(),  mp2.controlColor.y());
			cairo_pattern_mesh_set_control_point(mpat, 2, mp3.controlColor.x(),  mp3.controlColor.y());
			cairo_pattern_mesh_set_control_point(mpat, 3, mp4.controlColor.x(),  mp4.controlColor.y());
			mp1.color.getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
			mp2.color.getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
			mp3.color.getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
			mp4.color.getRgbF(&r, &g, &b, &a);
			cairo_pattern_mesh_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_pattern_mesh_end_patch(mpat);
		}
	}
	cairo_pattern_set_filter(mpat, CAIRO_FILTER_BEST);
	cairo_set_source(cr, mpat);
	cairo_paint_with_alpha(cr, 1.0);
	pat = cairo_pattern_create_for_surface(img);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_BEST);

	cairo_set_source (m_cr, pat);
	cairo_clip_preserve (m_cr);

	if (m_maskMode > 0)
	{
		/*cairo_pattern_t *patM = getMaskPattern();
		setRasterOp(m_blendModeFill);
		cairo_mask(m_cr, patM);
		if ((m_maskMode == 2) || (m_maskMode == 4) || (m_maskMode == 5) || (m_maskMode == 6))
			cairo_surface_destroy(imageMask);
		cairo_pattern_destroy (patM);*/
	}
	else
	{
		setRasterOp(m_blendModeFill);
		cairo_paint_with_alpha (m_cr, m_fillTrans);
	}
	cairo_pattern_destroy (pat);

	if (img) cairo_surface_destroy(img);
	if (mpat) cairo_pattern_destroy(mpat);
	if (cr) cairo_destroy( cr );
}

void ScPainterEx_Cairo::drawFreeMeshGradient( const QRect& rect )
{
	cairo_pattern_t *pat = NULL;
	cairo_surface_t *img = NULL;
	cairo_t *cr = NULL;
	cairo_pattern_t *mpat = NULL;
	VColorStopEx* stop = NULL;
	QColor color;

	double p3x = m_gradPatchP3.x();
	double p3y = m_gradPatchP3.y();
	img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, p3x, p3y);
	cr = cairo_create(img);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_tolerance(cr, 0.5 );
	double r, g, b, a;
	mpat = cairo_pattern_create_mesh();
	for (int col = 0; col < m_meshGradientPatches.count(); col++)
	{
		meshGradientPatch patch = m_meshGradientPatches[col];
		meshPoint mp1 = patch.TL;
		meshPoint mp2 = patch.TR;
		meshPoint mp3 = patch.BR;
		meshPoint mp4 = patch.BL;
		cairo_pattern_mesh_begin_patch(mpat);
		cairo_pattern_mesh_move_to(mpat, mp1.gridPoint.x(), mp1.gridPoint.y());
		cairo_pattern_mesh_curve_to(mpat, mp1.controlRight.x(), mp1.controlRight.y(), mp2.controlLeft.x(), mp2.controlLeft.y(), mp2.gridPoint.x(), mp2.gridPoint.y());
		cairo_pattern_mesh_curve_to(mpat, mp2.controlBottom.x(), mp2.controlBottom.y(), mp3.controlTop.x(), mp3.controlTop.y(), mp3.gridPoint.x(), mp3.gridPoint.y());
		cairo_pattern_mesh_curve_to(mpat, mp3.controlLeft.x(), mp3.controlLeft.y(), mp4.controlRight.x(), mp4.controlRight.y(), mp4.gridPoint.x(), mp4.gridPoint.y());
		cairo_pattern_mesh_curve_to(mpat, mp4.controlTop.x(), mp4.controlTop.y(), mp1.controlBottom.x(), mp1.controlBottom.y(), mp1.gridPoint.x(), mp1.gridPoint.y());
		cairo_pattern_mesh_set_control_point(mpat, 0, mp1.controlColor.x(),  mp1.controlColor.y());
		cairo_pattern_mesh_set_control_point(mpat, 1, mp2.controlColor.x(),  mp2.controlColor.y());
		cairo_pattern_mesh_set_control_point(mpat, 2, mp3.controlColor.x(),  mp3.controlColor.y());
		cairo_pattern_mesh_set_control_point(mpat, 3, mp4.controlColor.x(),  mp4.controlColor.y());
		mp1.color.getRgbF(&r, &g, &b, &a);
		cairo_pattern_mesh_set_corner_color_rgba(mpat, 0, r, g, b, a);
		mp2.color.getRgbF(&r, &g, &b, &a);
		cairo_pattern_mesh_set_corner_color_rgba(mpat, 1, r, g, b, a);
		mp3.color.getRgbF(&r, &g, &b, &a);
		cairo_pattern_mesh_set_corner_color_rgba(mpat, 2, r, g, b, a);
		mp4.color.getRgbF(&r, &g, &b, &a);
		cairo_pattern_mesh_set_corner_color_rgba(mpat, 3, r, g, b, a);
		cairo_pattern_mesh_end_patch(mpat);
	}
	cairo_pattern_set_filter(mpat, CAIRO_FILTER_BEST);
	cairo_set_source(cr, mpat);
	cairo_paint_with_alpha(cr, 1.0);
	pat = cairo_pattern_create_for_surface(img);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_BEST);
	
	cairo_set_source (m_cr, pat);
	cairo_clip_preserve (m_cr);

	if (m_maskMode > 0)
	{
		/*cairo_pattern_t *patM = getMaskPattern();
		setRasterOp(m_blendModeFill);
		cairo_mask(m_cr, patM);
		if ((m_maskMode == 2) || (m_maskMode == 4) || (m_maskMode == 5) || (m_maskMode == 6))
			cairo_surface_destroy(imageMask);
		cairo_pattern_destroy (patM);*/
	}
	else
	{
		setRasterOp(m_blendModeFill);
		cairo_paint_with_alpha (m_cr, m_fillTrans);
	}
	cairo_pattern_destroy (pat);

	if (img) cairo_surface_destroy(img);
	if (mpat) cairo_pattern_destroy(mpat);
	if (cr) cairo_destroy( cr );
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
