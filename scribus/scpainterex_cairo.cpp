/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scpainterex_cairo.h"
#include "sccolorengine.h"
#include "scimage.h"
#include "scpattern.h"
#include "util.h"
#include "util_math.h"

ScPainterEx_Cairo::ScPainterEx_Cairo(cairo_t* context, const QRect& rect, ScribusDoc* doc, bool gray) : ScPainterExBase()
{
	m_doc    = doc;
	m_width  = rect.width();
	m_height = rect.height();
	m_strokeColor = ScColorShade(QColor(0,0,0), 100);
	m_fillColor = ScColorShade(QColor(0,0,0), 100);
	// Grayscale conversion parameter
	m_convertToGray = gray;
	// Initialization of Windows GDI data
	m_cr = context;

	clear();
}

ScPainterEx_Cairo::~ScPainterEx_Cairo()
{
	if (m_imageMask)
	{
		cairo_surface_destroy(m_imageMask);
		m_imageMask = nullptr;
	}
}

QColor ScPainterEx_Cairo::transformColor(const ScColorShade& colorShade, double trans) const
{
	QColor c = ScColorEngine::getShadeColor(colorShade.color, m_doc, colorShade.shade);
	QColor color = qRgba(qRed(c.rgb()), qGreen(c.rgb()), qBlue(c.rgb()), qAlpha(trans * 255));
	if (m_convertToGray)
	{
		int grayLevel;
		QColor grayColor;
		grayLevel = 0.3 * color.red() + 0.59 * color.green() + 0.11 * color.blue();
		grayColor = qRgba(grayLevel, grayLevel, grayLevel, qAlpha(color.rgb()));
		color = grayColor;
	}
	return color;
}

void ScPainterEx_Cairo::transformImage(QImage& image) const
{
	int rgb, grey;
	int imageWidth = image.width();
	int imageHeight = image.height();
	QRgb *pscan;

	for (int j = 0; j < imageHeight; j++)
	{
		pscan = (QRgb *) image.scanLine(j);
		for (int i = 0; i < imageWidth; i++)
		{
			rgb = pscan[i];
			grey = 0.3 * qRed(rgb) + 0.59 * qGreen(rgb) + 0.11 * qBlue(rgb);
			pscan[i] = qRgba(grey, grey, grey, qAlpha(rgb));
		}
	}
}

void ScPainterEx_Cairo::beginLayer(double transparency, int blendMode, FPointArray* clipArray)
{
	LayerProp la;
	la.blendMode = m_blendModeLayer;
	la.transparency = m_layerTransparency;
	m_layerTransparency = transparency;
	m_blendModeLayer = blendMode;
	la.groupClip.resize(0);
	if (clipArray != nullptr)
		la.groupClip = *clipArray;
	la.maskMode = m_maskMode;
	la.maskPattern = m_maskPattern;
	la.maskPatternTrans = m_maskPatternTrans;
	la.maskPatternMirrorX = m_maskPatternMirrorX;
	la.maskPatternMirrorY = m_maskPatternMirrorY;
	la.maskGradientScale = m_maskGradientScale;
	la.maskGradientSkew = m_maskGradientSkew;
	la.maskGradient = m_maskGradient;
	la.data = cairo_get_group_target(m_cr);
	la.fillRule = m_fillRule;
	cairo_push_group(m_cr);
	m_layers.push(la);
}

void ScPainterEx_Cairo::endLayer()
{
	if (m_layers.isEmpty())
		return;

	LayerProp la = m_layers.pop();
	m_maskMode = la.maskMode;
	m_maskPattern = la.maskPattern;
	m_maskPatternTrans = la.maskPatternTrans;
	m_maskPatternMirrorX = la.maskPatternMirrorX;
	m_maskPatternMirrorY = la.maskPatternMirrorY;
	m_maskGradientScale = la.maskGradientScale;
	m_maskGradientSkew = la.maskGradientSkew;
	m_maskGradient = la.maskGradient;
	m_fillRule = la.fillRule;

	cairo_pop_group_to_source(m_cr);
	if (!la.groupClip.empty())
	{
		if (m_fillRule)
			cairo_set_fill_rule(m_cr, CAIRO_FILL_RULE_EVEN_ODD);
		else
			cairo_set_fill_rule(m_cr, CAIRO_FILL_RULE_WINDING);
		setupPolygon(&la.groupClip);
		setClipPath();
	}
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	if (m_maskMode > 0)
	{
		cairo_pattern_t* patM = getMaskPattern();
		setRasterOp(m_blendModeLayer);
		cairo_mask(m_cr, patM);
		if (m_imageMask)
		{
			cairo_surface_destroy(m_imageMask);
			m_imageMask = nullptr;
		}
		cairo_pattern_destroy(patM);
	}
	else
	{
		setRasterOp(m_blendModeLayer);
		cairo_paint_with_alpha(m_cr, m_layerTransparency);
	}
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);

	m_layerTransparency = la.transparency;
	m_blendModeLayer = la.blendMode;
	m_maskMode = 0;
}

void ScPainterEx_Cairo::clear()
{
	/*SolidBrush brush(Color(255,255,255));
	m_graphics->FillRectangle(&brush, 0, 0, m_width, m_height);*/
}

void ScPainterEx_Cairo::clear(ScColorShade &c)
{
	/*QColor color = transformColor(c, 1.0);
	QRgb cs = color.rgb();
	SolidBrush brush(Color(qAlpha(cs), qRed(cs), qGreen(cs),qBlue(cs)));
	m_graphics->FillRectangle(&brush, 0, 0, m_width, m_height);*/
}

QTransform ScPainterEx_Cairo::worldMatrix() const
{
	cairo_matrix_t matrix;
	cairo_get_matrix(m_cr, &matrix);
	QTransform mat(matrix.xx, matrix.yx, matrix.xy, matrix.yy, matrix.x0, matrix.y0);
	return mat;
}

void ScPainterEx_Cairo::setWorldMatrix(const QTransform &mat)
{
	cairo_matrix_t matrix;
	cairo_matrix_init(&matrix, mat.m11(), mat.m12(), mat.m21(), mat.m22(), mat.dx(), mat.dy());
	cairo_set_matrix(m_cr, &matrix);
}

void ScPainterEx_Cairo::translate(double x, double y)
{
	cairo_translate(m_cr, x, y);
}

void ScPainterEx_Cairo::translate(const QPointF& offset)
{
	cairo_translate(m_cr, offset.x(), offset.y());
}

void ScPainterEx_Cairo::rotate(double r)
{
	cairo_rotate(m_cr, r * 3.1415927 / 180.0);
}

void ScPainterEx_Cairo::scale(double x, double y)
{
	cairo_scale(m_cr, x, y);
}

void ScPainterEx_Cairo::shear(double x, double y)
{
	QTransform trans;
	trans.shear(x, y);

	cairo_matrix_t matrix;
	cairo_matrix_init(&matrix, trans.m11(), trans.m12(), trans.m21(), trans.m22(), trans.dx(), trans.dy());
	cairo_transform(m_cr, &matrix);
}

void ScPainterEx_Cairo::moveTo(const double &x, const double &y)
{
	cairo_move_to(m_cr, x, y);
}

void ScPainterEx_Cairo::lineTo(const double &x, const double &y)
{
	cairo_line_to(m_cr, x, y);
}

void ScPainterEx_Cairo::curveTo(const FPoint& p1, const FPoint& p2, const FPoint& p3)
{
	cairo_curve_to(m_cr, p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
}

void ScPainterEx_Cairo::newPath()
{
	cairo_new_path(m_cr);
}

void ScPainterEx_Cairo::closePath()
{
	cairo_close_path(m_cr);
}

void ScPainterEx_Cairo::setFillRule(bool fillRule)
{
	m_fillRule = fillRule;
}

void ScPainterEx_Cairo::setFillMode(int fill)
{
	m_fillMode = fill;
}

void ScPainterEx_Cairo::setStrokeMode(int stroke)
{
	m_strokeMode = stroke;
}

void ScPainterEx_Cairo::setGradient(VGradientEx::Type mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew)
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

void ScPainterEx_Cairo::setGradientMask(VGradientEx::Type mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew)
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

void ScPainterEx_Cairo::setPatternMask(ScPattern *pattern, const ScMaskTransform& maskTrans, bool mirrorX, bool mirrorY)
{
	m_maskPattern = pattern;
	m_maskPatternTrans = maskTrans;
	m_maskPatternMirrorX = mirrorX;
	m_maskPatternMirrorY = mirrorY;
}

void ScPainterEx_Cairo::setPattern(ScPattern *pattern, const ScPatternTransform& patternTrans, bool mirrorX, bool mirrorY)
{
	m_pattern = pattern;
	m_patternTrans = patternTrans;
	m_patternMirrorX = mirrorX;
	m_patternMirrorY = mirrorY;
}

void ScPainterEx_Cairo::set4ColorGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4)
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

void ScPainterEx_Cairo::setDiamondGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4, const FPoint& c5)
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

void ScPainterEx_Cairo::setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, QList<QList<MeshPoint> > meshArray)
{
	m_fillGradient.setType(VGradientEx::mesh);
	m_meshGradientArray = meshArray;
	m_gradPatchP1 = p1;
	m_gradPatchP2 = p2;
	m_gradPatchP3 = p3;
	m_gradPatchP4 = p4;
}

void ScPainterEx_Cairo::setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, QList<meshGradientPatch> meshPatches)
{
	m_fillGradient.setType(VGradientEx::freemesh);
	m_meshGradientPatches = meshPatches;
	m_gradPatchP1 = p1;
	m_gradPatchP2 = p2;
	m_gradPatchP3 = p3;
	m_gradPatchP4 = p4;
}

void ScPainterEx_Cairo::setHatchParameters(int mode, double distance, double angle, bool useBackground, const ScColorShade& background, const ScColorShade& foreground, double width, double height)
{
	m_hatchType = mode;
	m_hatchDistance = distance;
	m_hatchAngle = angle;
	m_hatchUseBackground = useBackground;
	m_hatchBackground = background;
	m_hatchForeground = foreground;
	m_hatchWidth = width;
	m_hatchHeight = height;
}

void ScPainterEx_Cairo::fillPath()
{
	if (m_fillMode != 0)
		fillPathHelper();
}

void ScPainterEx_Cairo::strokePath()
{
	//if (m_lineWidth == 0)
	//	return;
	if (m_strokeMode != 0)
		strokePathHelper();
}

ScColorShade ScPainterEx_Cairo::pen()
{
	return m_strokeColor;
}

ScColorShade ScPainterEx_Cairo::brush()
{
	return m_fillColor;
}

void ScPainterEx_Cairo::setPen(const ScColorShade &c)
{
	m_strokeColor = c;
}

void ScPainterEx_Cairo::setPen(const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo)
{
	m_strokeColor = c;
	m_lineWidth   = w;
	m_lineEnd     = ca;
	m_lineJoin    = jo;
	m_array.clear();
	m_offset      = 0;
	getDashArray(st, w, m_array);
}

void ScPainterEx_Cairo::setLineWidth(double w)
{
	m_lineWidth = w;
}

void ScPainterEx_Cairo::setPenOpacity(double op)
{
	m_strokeTrans = op;
}

void ScPainterEx_Cairo::setDash(const QVector<double>& array, double ofs)
{
	m_array = array;
	m_offset = ofs;
}

void ScPainterEx_Cairo::setBrush(const ScColorShade &c)
{
	m_fillColor = c;
}

void ScPainterEx_Cairo::setBrushOpacity(double op)
{
	m_fillTrans = op;
}

void ScPainterEx_Cairo::setOpacity(double op)
{
	m_fillTrans = op;
	m_strokeTrans = op;
}

void ScPainterEx_Cairo::setFont(const QFont &f)
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
	cairo_restore(m_cr);
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
	else if (blendMode == 16)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_ADD);
	else if (blendMode == 17)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_DEST_IN);
	else if (blendMode == 18)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_DEST_OUT);
	else if (blendMode == 19)
		cairo_set_operator(m_cr, CAIRO_OPERATOR_CLEAR);
	else
		cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
}

void ScPainterEx_Cairo::setBlendModeFill(int blendMode)
{
	m_blendModeFill = blendMode;
}

void ScPainterEx_Cairo::setBlendModeStroke(int blendMode)
{
	m_blendModeStroke = blendMode;
}

cairo_pattern_t* ScPainterEx_Cairo::getMaskPattern()
{
	cairo_save(m_cr);
	cairo_pattern_t* pat { nullptr };
	if ((m_maskMode == 1) || (m_maskMode == 3))
	{
		double x1 = m_maskGradient.origin().x();
		double y1 = m_maskGradient.origin().y();
		double x2 = m_maskGradient.vector().x();
		double y2 = m_maskGradient.vector().y();
		double fx = m_maskGradient.focalPoint().x();
		double fy = m_maskGradient.focalPoint().y();
		if (m_maskGradient.type() == VGradientEx::linear)
			pat = cairo_pattern_create_linear(x1, y1, x2, y2);
		else
			pat = cairo_pattern_create_radial(fx, fy, 0, x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
		QList<VColorStopEx*> colorStops = m_maskGradient.colorStops();
		QColor qStopColor;
		for (int offset = 0; offset < colorStops.count(); offset++)
		{
			const auto* stop = colorStops[offset];
			qStopColor = transformColor(ScColorShade(stop->color, stop->shade), 1.0);
			double a = colorStops[offset]->opacity;
			double r, g, b;
			qStopColor.getRgbF(&r, &g, &b);
			if (m_maskMode == 3)
				a = 0.3 * r + 0.59 * g + 0.11 * b;
			cairo_pattern_add_color_stop_rgba(pat, colorStops[offset]->rampPoint, r, g, b, a);
		}
		cairo_matrix_t matrix;
		QTransform qmatrix;
		if (m_maskGradient.type() == VGradientEx::radial)
		{
			double rotEnd = xy2Deg(x2 - x1, y2 - y1);
			qmatrix.translate(x1, y1);
			qmatrix.rotate(rotEnd);
			qmatrix.shear(m_maskGradientSkew, 0);
			qmatrix.translate(0, y1 * (1.0 - m_maskGradientScale));
			qmatrix.translate(-x1, -y1);
			qmatrix.scale(1, m_maskGradientScale);
		}
		else
		{
			qmatrix.translate(x1, y1);
			qmatrix.shear(-m_maskGradientSkew, 0);
			qmatrix.translate(-x1, -y1);
		}
		cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
		cairo_matrix_invert(&matrix);
		cairo_pattern_set_matrix(pat, &matrix);
	}
	else
	{
		if ((m_maskMode == 4) || (m_maskMode == 5))
		{
			m_imageQ = m_maskPattern->pattern.copy();
			if (m_maskMode == 5)
				m_imageQ.invertPixels();
			int h = m_imageQ.height();
			int w = m_imageQ.width();
			int k;
			QRgb* s;
			QRgb r;
			for (int yi = 0; yi < h; ++yi)
			{
				s = (QRgb*) (m_imageQ.scanLine(yi));
				for (int xi = 0; xi < w; ++xi)
				{
					r = *s;
					if (qAlpha(r) == 0)
						k = 0;
					else
						k = qMin(qRound(0.3 * qRed(r) + 0.59 * qGreen(r) + 0.11 * qBlue(r)), 255);
					*s = qRgba(qRed(r), qGreen(r), qBlue(r), k);
					s++;
				}
			}
			m_imageMask = cairo_image_surface_create_for_data(m_imageQ.bits(), CAIRO_FORMAT_ARGB32, w, h, w * 4);
		}
		else
		{
			m_imageQ = m_maskPattern->pattern.copy();
			if (m_maskMode == 6)
				m_imageQ.invertPixels(QImage::InvertRgba);
			m_imageMask = cairo_image_surface_create_for_data(m_imageQ.bits(), CAIRO_FORMAT_ARGB32, m_maskPattern->getPattern()->width(), m_maskPattern->getPattern()->height(), m_maskPattern->getPattern()->width() * 4);
		}
		pat = cairo_pattern_create_for_surface(m_imageMask);
		cairo_pattern_set_extend(pat, CAIRO_EXTEND_REPEAT);
		cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);
		cairo_matrix_t matrix;
		QTransform qmatrix;
		qmatrix.translate(m_maskPatternTrans.offsetX, m_maskPatternTrans.offsetY);
		qmatrix.rotate(m_maskPatternTrans.rotation);
		qmatrix.shear(-m_maskPatternTrans.skewX, m_maskPatternTrans.skewY);
		qmatrix.scale(m_maskPatternTrans.scaleX, m_maskPatternTrans.scaleY);
		qmatrix.scale(m_maskPattern->width / static_cast<double>(m_maskPattern->getPattern()->width()), m_maskPattern->height / static_cast<double>(m_maskPattern->getPattern()->height()));
		if (m_maskPatternMirrorX)
			qmatrix.scale(-1, 1);
		if (m_maskPatternMirrorY)
			qmatrix.scale(1, -1);
		cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
		cairo_matrix_invert(&matrix);
		cairo_pattern_set_matrix(pat, &matrix);
	}
	cairo_restore(m_cr);
	return pat;
}

void ScPainterEx_Cairo::fillPathHelper()
{
	cairo_save(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	if (m_fillRule)
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
			cairo_pattern_t *pat = getMaskPattern();
			cairo_set_source_rgb(m_cr, r, g, b);
			setRasterOp(m_blendModeFill);
			cairo_clip_preserve(m_cr);
			if (pat)
				cairo_mask(m_cr, pat);
			if (m_imageMask)
			{
				cairo_surface_destroy(m_imageMask);
				m_imageMask = nullptr;
			}
			cairo_pattern_destroy(pat);
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
		drawGradient(m_fillGradient);
	}
	else if (m_fillMode == ScPainterExBase::Pattern)
	{
		// Pattern painting is emulated in ScPageOutput::drawPattern(),
		// So we have nothing to do here
	}
	else if (m_fillMode == ScPainterExBase::Hatch)
	{
		drawHatch();
	}
	cairo_restore(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
}

void ScPainterEx_Cairo::strokePathHelper()
{
	cairo_save(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	cairo_set_line_width(m_cr, m_lineWidth);
	if (m_array.count() > 0)
		cairo_set_dash(m_cr, m_array.data(), m_array.count(), m_offset);
	else
		cairo_set_dash(m_cr, nullptr, 0, 0);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	if (m_lineEnd == Qt::RoundCap)
		cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_ROUND);
	else if (m_lineEnd == Qt::SquareCap)
		cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_SQUARE);
	else if (m_lineEnd == Qt::FlatCap)
		cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_BUTT);
	if (m_lineJoin == Qt::RoundJoin)
		cairo_set_line_join(m_cr, CAIRO_LINE_JOIN_ROUND);
	else if (m_lineJoin == Qt::BevelJoin)
		cairo_set_line_join(m_cr, CAIRO_LINE_JOIN_BEVEL);
	else if (m_lineJoin == Qt::MiterJoin)
		cairo_set_line_join(m_cr, CAIRO_LINE_JOIN_MITER);
	if (m_strokeMode == 3)
	{

	}
	else if (m_strokeMode == 2)
	{
		strokeGradient(m_strokeGradient);
	}
	else
	{
		double r, g, b;
		QColor strokeColor = transformColor(m_strokeColor, 1.0);
		strokeColor.getRgbF(&r, &g, &b);
		cairo_set_source_rgba(m_cr, r, g, b, m_strokeTrans);
		setRasterOp(m_blendModeStroke);
		cairo_stroke_preserve(m_cr);
	}
	cairo_restore(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
}

void ScPainterEx_Cairo::setClipPath()
{
	cairo_clip (m_cr);
}

void ScPainterEx_Cairo::drawImage(ScImage *image, ScPainterExBase::ImageMode mode)
{
	QImage qImage(image->qImage());
	if (m_convertToGray)
		transformImage(qImage);

	cairo_push_group(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	cairo_set_fill_rule(m_cr, cairo_get_fill_rule(m_cr));
	cairo_surface_t* image2 = cairo_image_surface_create_for_data(qImage.bits(), CAIRO_FORMAT_RGB24, qImage.width(), qImage.height(), qImage.width() * 4);
	cairo_surface_t* image3 = cairo_image_surface_create_for_data(qImage.bits(), CAIRO_FORMAT_ARGB32, qImage.width(), qImage.height(), qImage.width() * 4);
	cairo_set_source_surface(m_cr, image2, 0, 0);
	cairo_pattern_set_filter(cairo_get_source(m_cr), CAIRO_FILTER_GOOD);
	cairo_mask_surface(m_cr, image3, 0, 0);
	cairo_surface_destroy(image2);
	cairo_surface_destroy(image3);
	cairo_pop_group_to_source(m_cr);
	cairo_pattern_set_filter(cairo_get_source(m_cr), CAIRO_FILTER_GOOD);
	setRasterOp(m_blendModeFill);
	if (m_maskMode > 0)
	{
		cairo_pattern_t* patM = getMaskPattern();
		cairo_pattern_set_filter(patM, CAIRO_FILTER_GOOD);
		if (patM)
			cairo_mask(m_cr, patM);
		if (m_imageMask)
		{
			cairo_surface_destroy(m_imageMask);
			m_imageMask = nullptr;
		}
		cairo_pattern_destroy(patM);
	}
	else
		cairo_paint_with_alpha(m_cr, m_fillTrans);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
}

void ScPainterEx_Cairo::setupPolygon(const FPointArray *points, bool closed)
{
	bool nPath = true, first = true;
	FPoint np, np1, np2, np3, np4, firstP;

	if (points->size() <= 3)
		return;

	newPath();
	for (int poi = 0; poi < points->size() - 3; poi += 4)
	{
		if (points->isMarker(poi))
		{
			nPath = true;
			continue;
		}
		if (nPath)
		{
			np = points->point(poi);
			if (!first && closed && (np4 == firstP))
				cairo_close_path(m_cr);
			moveTo(np.x(), np.y());
			first = nPath = false;
			firstP = np4 = np;
		}
		np  = points->point(poi);
		np1 = points->point(poi + 1);
		np2 = points->point(poi + 3);
		np3 = points->point(poi + 2);
		if (np4 == np3)
			continue;
		if ((np == np1) && (np2 == np3))
			lineTo(np3.x(), np3.y());
		else
			curveTo(np1, np2, np3);
		np4 = np3;
	}
	if (closed)
		cairo_close_path(m_cr);
}

void ScPainterEx_Cairo::drawPolygon()
{
	fillPath();
}

void ScPainterEx_Cairo::drawPolyLine()
{
	strokePath();
}

void ScPainterEx_Cairo::drawLine(const FPoint& start, const FPoint& end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainterEx_Cairo::drawLine(const QPointF& start, const QPointF& end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainterEx_Cairo::drawRect(double x, double y, double w, double h)
{
	newPath();
	moveTo(x, y);
	lineTo(x + w, y);
	lineTo(x + w, y + h);
	lineTo(x, y + h);
	lineTo(x, y);
	cairo_close_path(m_cr);
	fillPath();
	strokePath();
}

void ScPainterEx_Cairo::drawGradient(const VGradientEx& gradient)
{
	save();
	setClipPath();
	if (gradient.type() == VGradientEx::linear)
		drawLinearGradient(gradient);
	else if (gradient.type() == VGradientEx::radial)
		drawCircularGradient(gradient);
	else if (gradient.type() == VGradientEx::fourcolor)
		drawFourColorGradient();
	else if (gradient.type() == VGradientEx::diamond)
		drawDiamondGradient(gradient);
	else if (gradient.type() == VGradientEx::mesh)
		drawMeshGradient();
	else if (gradient.type() == VGradientEx::freemesh)
		drawFreeMeshGradient();
	restore();
}

void ScPainterEx_Cairo::drawLinearGradient(const VGradientEx& gradient)
{
	double r, g, b;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	const VColorStopEx* stop = nullptr;
	QColor color;

	if (gradient.stops() < 2)
		return;

	double x1 = gradient.origin().x();
	double y1 = gradient.origin().y();
	double x2 = gradient.vector().x();
	double y2 = gradient.vector().y();
	cairo_pattern_t *pat = cairo_pattern_create_linear (x1, y1, x2, y2);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);

	bool   isFirst  = true;
	double lastStop = 0.0;
	for (int index = 0 ; index < gradient.stops(); ++index)
	{
		stop  = colorStops.at(index);
		if ((lastStop == stop->rampPoint) && (!isFirst))
			continue;
		isFirst = false;
		color = transformColor(ScColorShade(stop->color, stop->shade), 1.0);
		color.getRgbF(&r, &g, &b);
		cairo_pattern_add_color_stop_rgba (pat, stop->rampPoint, r, g, b, stop->opacity);
		lastStop = stop->rampPoint;
	}

	QTransform qmatrix;
	qmatrix.translate(x1, y1);
	qmatrix.shear(-m_gradientSkew, 0);
	qmatrix.translate(-x1, -y1);

	cairo_matrix_t matrix;
	cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
	cairo_matrix_invert(&matrix);
	cairo_pattern_set_matrix (pat, &matrix);

	setRasterOp(m_blendModeFill);
	cairo_set_source (m_cr, pat);
	//cairo_clip_preserve (m_cr);
	cairo_paint_with_alpha (m_cr, m_fillTrans);
	cairo_pattern_destroy (pat);
}

void ScPainterEx_Cairo::drawCircularGradient(const VGradientEx& gradient)
{
	double r, g, b;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	const VColorStopEx* stop = nullptr;
	QColor color;

	double x1  = gradient.origin().x();
	double y1  = gradient.origin().y();
	double x2  = gradient.vector().x();
	double y2  = gradient.vector().y();
	double fx  = gradient.focalPoint().x();
	double fy  = gradient.focalPoint().y();
	double rad = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

	cairo_pattern_t* pat = cairo_pattern_create_radial (fx, fy, 0, x1, y1, rad);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);

	bool   isFirst  = true;
	double lastStop = 0.0;
	for (int index = 0 ; index < gradient.stops() ; index++)
	{
		stop  = colorStops.at(index);
		if ((lastStop == stop->rampPoint) && (!isFirst))
			continue;
		isFirst = false;
		color = transformColor(ScColorShade(stop->color, stop->shade), 1.0);
		color.getRgbF(&r, &g, &b);
		cairo_pattern_add_color_stop_rgba (pat, stop->rampPoint, r, g, b, stop->opacity);
		lastStop = stop->rampPoint;
	}

	QTransform qmatrix;
	double rotEnd = xy2Deg(x2 - x1, y2 - y1);
	qmatrix.translate(x1, y1);
	qmatrix.rotate(rotEnd);
	qmatrix.shear(m_gradientSkew, 0);
	qmatrix.translate(0, y1 * (1.0 - m_gradientScale));
	qmatrix.translate(-x1, -y1);
	qmatrix.scale(1, m_gradientScale);
	
	cairo_set_source (m_cr, pat);
	//cairo_clip_preserve (m_cr);

	setRasterOp(m_blendModeFill);
	cairo_paint_with_alpha (m_cr, m_fillTrans);
	cairo_pattern_destroy (pat);
}

void ScPainterEx_Cairo::drawFourColorGradient()
{
	QColor color;

	double p1x = m_gradPatchP1.x();
	double p1y = m_gradPatchP1.y();
	double p2x = m_gradPatchP2.x();
	double p2y = m_gradPatchP2.y();
	double p3x = m_gradPatchP3.x();
	double p3y = m_gradPatchP3.y();
	double p4x = m_gradPatchP4.x();
	double p4y = m_gradPatchP4.y();

	cairo_surface_t* img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, p3x, p3y);
	cairo_t* cr = cairo_create(img);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_tolerance(cr, 0.5);

	double r, g, b, a;
	cairo_pattern_t* mpat = cairo_pattern_create_mesh();
	cairo_mesh_pattern_begin_patch(mpat);
	cairo_mesh_pattern_move_to(mpat, p1x, p1y);
	cairo_mesh_pattern_line_to(mpat, p2x, p2y);
	cairo_mesh_pattern_line_to(mpat, p3x, p3y);
	cairo_mesh_pattern_line_to(mpat, p4x, p4y);
	cairo_mesh_pattern_line_to(mpat, p1x, p1y);
	cairo_mesh_pattern_set_control_point(mpat, 0, m_gradControlP1.x(),  m_gradControlP1.y());
	cairo_mesh_pattern_set_control_point(mpat, 1, m_gradControlP2.x(),  m_gradControlP2.y());
	cairo_mesh_pattern_set_control_point(mpat, 2, m_gradControlP3.x(),  m_gradControlP3.y());
	cairo_mesh_pattern_set_control_point(mpat, 3, m_gradControlP4.x(),  m_gradControlP4.y());
	color = transformColor(m_gradPatchColor1, 1.0);
	color.getRgbF(&r, &g, &b, &a);
	cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
	color = transformColor(m_gradPatchColor2, 1.0);
	color.getRgbF(&r, &g, &b, &a);
	cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
	color = transformColor(m_gradPatchColor3, 1.0);
	color.getRgbF(&r, &g, &b, &a);
	cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
	color = transformColor(m_gradPatchColor4, 1.0);
	color.getRgbF(&r, &g, &b, &a);
	cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
	cairo_mesh_pattern_end_patch(mpat);
	cairo_pattern_set_filter(mpat, CAIRO_FILTER_GOOD);
	cairo_set_source(cr, mpat);
	cairo_paint_with_alpha(cr, 1.0);

	cairo_pattern_t* pat = cairo_pattern_create_for_surface(img);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);

	cairo_set_source (m_cr, pat);
	//cairo_clip_preserve (m_cr);

	if (m_maskMode > 0)
	{
		cairo_pattern_t *patM = getMaskPattern();
		setRasterOp(m_blendModeFill);
		if (patM)
			cairo_mask(m_cr, patM);
		if (m_imageMask)
		{
			cairo_surface_destroy(m_imageMask);
			m_imageMask = nullptr;
		}
		cairo_pattern_destroy (patM);
	}
	else
	{
		setRasterOp(m_blendModeFill);
		cairo_paint_with_alpha (m_cr, m_fillTrans);
	}
	cairo_pattern_destroy (pat);

	if (img) cairo_surface_destroy(img);
	if (mpat) cairo_pattern_destroy(mpat);
	if (cr) cairo_destroy(cr);
}

void ScPainterEx_Cairo::drawDiamondGradient(const VGradientEx& gradient)
{
	double p1x = m_gradControlP1.x();
	double p1y = m_gradControlP1.y();
	double p2x = m_gradControlP2.x();
	double p2y = m_gradControlP2.y();
	double p3x = m_gradControlP3.x();
	double p3y = m_gradControlP3.y();
	double p4x = m_gradControlP4.x();
	double p4y = m_gradControlP4.y();

	cairo_surface_t *img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, m_gradPatchP3.x(), m_gradPatchP3.y());
	cairo_t *cr = cairo_create(img);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_tolerance(cr, 0.5);

	double r, g, b, a;
	const VColorStopEx* stop = nullptr;
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	QList<QColor> qStopColors;
	QList<double> qStopRampPoints;
	QColor qStopColor;
	for (int offset = 0 ; offset < colorStops.count() ; offset++)
	{
		stop = colorStops.at(offset);
		qStopColor = transformColor(ScColorShade(stop->color, stop->shade), 1.0);
		int h, s, v, sneu, vneu;
		int shad = stop->shade;
		qStopColor.getHsv(&h, &s, &v);
		sneu = s * shad / 100;
		vneu = 255 - ((255 - v) * shad / 100);
		qStopColor.setHsv(h, sneu, vneu);
		qStopColor.setAlphaF(stop->opacity);
		if (offset == 0)
		{
			if (colorStops[offset]->rampPoint > 0)
			{
				qStopColors.append(qStopColor);
				qStopRampPoints.append(0);
			}
		}
		qStopColors.append(qStopColor);
		qStopRampPoints.append(colorStops[offset]->rampPoint);
	}
	qStopColors.last().getRgbF(&r, &g, &b, &a);
	QPointF centerP(m_gradControlP5.x(), m_gradControlP5.y());
	QLineF edge1(centerP, QPointF(p1x, p1y));
	QLineF edge2(centerP, QPointF(p2x, p2y));
	QLineF edge3(centerP, QPointF(p3x, p3y));
	QLineF edge4(centerP, QPointF(p4x, p4y));
	QPointF p1 = edge1.pointAt(colorStops.last()->rampPoint);
	QPointF p2 = edge2.pointAt(colorStops.last()->rampPoint);
	QPointF p3 = edge3.pointAt(colorStops.last()->rampPoint);
	QPointF p4 = edge4.pointAt(colorStops.last()->rampPoint);
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

	cairo_pattern_t *mpat = cairo_pattern_create_mesh();
	for (int offset = 1 ; offset <  qStopRampPoints.count() ; offset++)
	{
		QLineF e1 = edge1;
		QLineF e1s = edge1;
		QLineF e2 = edge2;
		QLineF e2s = edge2;
		QLineF e3 = edge3;
		QLineF e3s = edge3;
		QLineF e4 = edge4;
		QLineF e4s = edge4;
		e1.setLength(edge1.length()  *  qStopRampPoints[ offset ]);
		e2.setLength(edge2.length()  *  qStopRampPoints[ offset ]);
		e3.setLength(edge3.length()  *  qStopRampPoints[ offset ]);
		e4.setLength(edge4.length()  *  qStopRampPoints[ offset ]);
		e1s.setLength(edge1.length() *  qStopRampPoints[ offset - 1 ]);
		e2s.setLength(edge2.length() *  qStopRampPoints[ offset - 1 ]);
		e3s.setLength(edge3.length() *  qStopRampPoints[ offset - 1 ]);
		e4s.setLength(edge4.length() *  qStopRampPoints[ offset - 1 ]);
		if (offset == 1)
		{
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, centerP.x(), centerP.y());
			cairo_mesh_pattern_line_to(mpat, e1.x2(), e1.y2());
			cairo_mesh_pattern_line_to(mpat, e2.x2(), e2.y2());
			cairo_mesh_pattern_line_to(mpat, centerP.x(), centerP.y());
			qStopColors[0].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, centerP.x(), centerP.y());
			cairo_mesh_pattern_line_to(mpat, e2.x2(), e2.y2());
			cairo_mesh_pattern_line_to(mpat, e3.x2(), e3.y2());
			cairo_mesh_pattern_line_to(mpat, centerP.x(), centerP.y());
			qStopColors[0].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, centerP.x(), centerP.y());
			cairo_mesh_pattern_line_to(mpat, e3.x2(), e3.y2());
			cairo_mesh_pattern_line_to(mpat, e4.x2(), e4.y2());
			cairo_mesh_pattern_line_to(mpat, centerP.x(), centerP.y());
			qStopColors[0].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, centerP.x(), centerP.y());
			cairo_mesh_pattern_line_to(mpat, e4.x2(), e4.y2());
			cairo_mesh_pattern_line_to(mpat, e1.x2(), e1.y2());
			cairo_mesh_pattern_line_to(mpat, centerP.x(), centerP.y());
			qStopColors[0].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
		}
		else
		{
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, e1s.x2(), e1s.y2());
			cairo_mesh_pattern_line_to(mpat, e1.x2(), e1.y2());
			cairo_mesh_pattern_line_to(mpat, e2.x2(), e2.y2());
			cairo_mesh_pattern_line_to(mpat, e2s.x2(), e2s.y2());
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[offset].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, e2s.x2(), e2s.y2());
			cairo_mesh_pattern_line_to(mpat, e2.x2(), e2.y2());
			cairo_mesh_pattern_line_to(mpat, e3.x2(), e3.y2());
			cairo_mesh_pattern_line_to(mpat, e3s.x2(), e3s.y2());
			qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[offset].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, e3s.x2(), e3s.y2());
			cairo_mesh_pattern_line_to(mpat, e3.x2(), e3.y2());
			cairo_mesh_pattern_line_to(mpat, e4.x2(), e4.y2());
			cairo_mesh_pattern_line_to(mpat, e4s.x2(), e4s.y2());
			qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[offset].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, e4s.x2(), e4s.y2());
			cairo_mesh_pattern_line_to(mpat, e4.x2(), e4.y2());
			cairo_mesh_pattern_line_to(mpat, e1.x2(), e1.y2());
			cairo_mesh_pattern_line_to(mpat, e1s.x2(), e1s.y2());
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			qStopColors[offset].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			qStopColors[offset-1].getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
		}
	}
	cairo_pattern_set_filter(mpat, CAIRO_FILTER_GOOD);
	cairo_set_source(cr, mpat);
	cairo_paint_with_alpha(cr, 1.0);

	cairo_pattern_t *pat = cairo_pattern_create_for_surface(img);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);

	cairo_set_source (m_cr, pat);
	//cairo_clip_preserve (m_cr);

	if (m_maskMode > 0)
	{
		cairo_pattern_t *patM = getMaskPattern();
		setRasterOp(m_blendModeFill);
		if (patM)
			cairo_mask(m_cr, patM);
		if (m_imageMask)
		{
			cairo_surface_destroy(m_imageMask);
			m_imageMask = nullptr;
		}
		cairo_pattern_destroy (patM);
	}
	else
	{
		setRasterOp(m_blendModeFill);
		cairo_paint_with_alpha (m_cr, m_fillTrans);
	}
	cairo_pattern_destroy (pat);

	if (img) cairo_surface_destroy(img);
	if (mpat) cairo_pattern_destroy(mpat);
	if (cr) cairo_destroy(cr);
}

void ScPainterEx_Cairo::drawMeshGradient()
{
	double p3x = m_gradPatchP3.x();
	double p3y = m_gradPatchP3.y();

	cairo_surface_t* img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, p3x, p3y);
	cairo_t* cr = cairo_create(img);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_tolerance(cr, 0.5);

	double r, g, b, a;
	cairo_pattern_t* mpat = cairo_pattern_create_mesh();
	for (int grow = 0; grow < m_meshGradientArray.count() - 1; ++grow)
	{
		for (int gcol = 0; gcol < m_meshGradientArray[grow].count() - 1; ++gcol)
		{
			MeshPoint mp1 = m_meshGradientArray[grow][gcol];
			MeshPoint mp2 = m_meshGradientArray[grow][gcol + 1];
			MeshPoint mp3 = m_meshGradientArray[grow + 1][gcol + 1];
			MeshPoint mp4 = m_meshGradientArray[grow + 1][gcol];
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, mp1.gridPoint.x(), mp1.gridPoint.y());
			cairo_mesh_pattern_curve_to(mpat, mp1.controlRight.x(), mp1.controlRight.y(), mp2.controlLeft.x(), mp2.controlLeft.y(), mp2.gridPoint.x(), mp2.gridPoint.y());
			cairo_mesh_pattern_curve_to(mpat, mp2.controlBottom.x(), mp2.controlBottom.y(), mp3.controlTop.x(), mp3.controlTop.y(), mp3.gridPoint.x(), mp3.gridPoint.y());
			cairo_mesh_pattern_curve_to(mpat, mp3.controlLeft.x(), mp3.controlLeft.y(), mp4.controlRight.x(), mp4.controlRight.y(), mp4.gridPoint.x(), mp4.gridPoint.y());
			cairo_mesh_pattern_curve_to(mpat, mp4.controlTop.x(), mp4.controlTop.y(), mp1.controlBottom.x(), mp1.controlBottom.y(), mp1.gridPoint.x(), mp1.gridPoint.y());
			cairo_mesh_pattern_set_control_point(mpat, 0, mp1.controlColor.x(),  mp1.controlColor.y());
			cairo_mesh_pattern_set_control_point(mpat, 1, mp2.controlColor.x(),  mp2.controlColor.y());
			cairo_mesh_pattern_set_control_point(mpat, 2, mp3.controlColor.x(),  mp3.controlColor.y());
			cairo_mesh_pattern_set_control_point(mpat, 3, mp4.controlColor.x(),  mp4.controlColor.y());
			mp1.color.getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			mp2.color.getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			mp3.color.getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			mp4.color.getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
		}
	}
	cairo_pattern_set_filter(mpat, CAIRO_FILTER_BEST);
	cairo_set_source(cr, mpat);
	cairo_paint_with_alpha(cr, 1.0);

	cairo_pattern_t* pat = cairo_pattern_create_for_surface(img);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_BEST);

	cairo_set_source (m_cr, pat);
	//cairo_clip_preserve (m_cr);

	if (m_maskMode > 0)
	{
		cairo_pattern_t *patM = getMaskPattern();
		setRasterOp(m_blendModeFill);
		if (patM)
			cairo_mask(m_cr, patM);
		if (m_imageMask)
		{
			cairo_surface_destroy(m_imageMask);
			m_imageMask = nullptr;
		}
		cairo_pattern_destroy (patM);
	}
	else
	{
		setRasterOp(m_blendModeFill);
		cairo_paint_with_alpha (m_cr, m_fillTrans);
	}
	cairo_pattern_destroy (pat);

	if (img) cairo_surface_destroy(img);
	if (mpat) cairo_pattern_destroy(mpat);
	if (cr) cairo_destroy(cr);
}

void ScPainterEx_Cairo::drawFreeMeshGradient()
{
	double p3x = m_gradPatchP3.x();
	double p3y = m_gradPatchP3.y();

	cairo_surface_t* img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, p3x, p3y);
	cairo_t* cr = cairo_create(img);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_tolerance(cr, 0.5);

	double r, g, b, a;
	cairo_pattern_t* mpat = cairo_pattern_create_mesh();
	for (int col = 0; col < m_meshGradientPatches.count(); col++)
	{
		meshGradientPatch patch = m_meshGradientPatches[col];
		MeshPoint mp1 = patch.TL;
		MeshPoint mp2 = patch.TR;
		MeshPoint mp3 = patch.BR;
		MeshPoint mp4 = patch.BL;
		cairo_mesh_pattern_begin_patch(mpat);
		cairo_mesh_pattern_move_to(mpat, mp1.gridPoint.x(), mp1.gridPoint.y());
		cairo_mesh_pattern_curve_to(mpat, mp1.controlRight.x(), mp1.controlRight.y(), mp2.controlLeft.x(), mp2.controlLeft.y(), mp2.gridPoint.x(), mp2.gridPoint.y());
		cairo_mesh_pattern_curve_to(mpat, mp2.controlBottom.x(), mp2.controlBottom.y(), mp3.controlTop.x(), mp3.controlTop.y(), mp3.gridPoint.x(), mp3.gridPoint.y());
		cairo_mesh_pattern_curve_to(mpat, mp3.controlLeft.x(), mp3.controlLeft.y(), mp4.controlRight.x(), mp4.controlRight.y(), mp4.gridPoint.x(), mp4.gridPoint.y());
		cairo_mesh_pattern_curve_to(mpat, mp4.controlTop.x(), mp4.controlTop.y(), mp1.controlBottom.x(), mp1.controlBottom.y(), mp1.gridPoint.x(), mp1.gridPoint.y());
		cairo_mesh_pattern_set_control_point(mpat, 0, mp1.controlColor.x(),  mp1.controlColor.y());
		cairo_mesh_pattern_set_control_point(mpat, 1, mp2.controlColor.x(),  mp2.controlColor.y());
		cairo_mesh_pattern_set_control_point(mpat, 2, mp3.controlColor.x(),  mp3.controlColor.y());
		cairo_mesh_pattern_set_control_point(mpat, 3, mp4.controlColor.x(),  mp4.controlColor.y());
		mp1.color.getRgbF(&r, &g, &b, &a);
		cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
		mp2.color.getRgbF(&r, &g, &b, &a);
		cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
		mp3.color.getRgbF(&r, &g, &b, &a);
		cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
		mp4.color.getRgbF(&r, &g, &b, &a);
		cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
		cairo_mesh_pattern_end_patch(mpat);
	}
	cairo_pattern_set_filter(mpat, CAIRO_FILTER_BEST);
	cairo_set_source(cr, mpat);
	cairo_paint_with_alpha(cr, 1.0);

	cairo_pattern_t* pat = cairo_pattern_create_for_surface(img);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_BEST);
	
	cairo_set_source (m_cr, pat);
	//cairo_clip_preserve (m_cr);

	if (m_maskMode > 0)
	{
		cairo_pattern_t *patM = getMaskPattern();
		setRasterOp(m_blendModeFill);
		if (patM)
			cairo_mask(m_cr, patM);
		if (m_imageMask)
		{
			cairo_surface_destroy(m_imageMask);
			m_imageMask = nullptr;
		}
		cairo_pattern_destroy (patM);
	}
	else
	{
		setRasterOp(m_blendModeFill);
		cairo_paint_with_alpha (m_cr, m_fillTrans);
	}
	cairo_pattern_destroy (pat);

	if (img) cairo_surface_destroy(img);
	if (mpat) cairo_pattern_destroy(mpat);
	if (cr) cairo_destroy(cr);
}

void ScPainterEx_Cairo::drawHatch()
{
	cairo_path_t* path = cairo_copy_path(m_cr);

	cairo_surface_t *img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, m_hatchWidth, m_hatchHeight);
	cairo_t* cr = cairo_create(img);

	cairo_new_path(cr);
	cairo_append_path(cr, path);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

	if (m_hatchUseBackground)
	{
		double r2, g2, b2;
		QColor hatchBackground = transformColor(m_hatchBackground, 1.0);
		hatchBackground.getRgbF(&r2, &g2, &b2);
		cairo_set_source_rgb(cr, r2, g2, b2);
		cairo_fill_preserve(cr);
	}

	double r, g, b;
	QColor hatchForeground = transformColor(m_hatchForeground, 1.0);
	hatchForeground.getRgbF(&r, &g, &b);
	cairo_set_line_width(cr, 1);
	cairo_set_source_rgb(cr, r, g, b);
	cairo_translate(cr, m_hatchWidth / 2.0, m_hatchHeight / 2.0);

	double lineLen = sqrt((m_hatchWidth / 2.0) * (m_hatchWidth / 2.0) + (m_hatchHeight / 2.0) * (m_hatchHeight / 2.0));
	double dist = 0.0;
	while (dist < lineLen)
	{
		cairo_save(cr);
		cairo_rotate(cr, -m_hatchAngle * 3.1415927 / 180.0);
		cairo_new_path(cr);
		cairo_move_to(cr, -lineLen, dist);
		cairo_line_to(cr, lineLen, dist);
		cairo_stroke(cr);
		if (dist > 0)
		{
			cairo_new_path(cr);
			cairo_move_to(cr, -lineLen, -dist);
			cairo_line_to(cr, lineLen, -dist);
			cairo_stroke(cr);
		}
		cairo_restore(cr);
		if ((m_hatchType == 1) || (m_hatchType == 2))
		{
			cairo_save(cr);
			cairo_rotate(cr, (-m_hatchAngle + 90) * 3.1415927 / 180.0);
			cairo_new_path(cr);
			cairo_move_to(cr, -lineLen, dist);
			cairo_line_to(cr, lineLen, dist);
			cairo_stroke(cr);
			if (dist > 0)
			{
				cairo_new_path(cr);
				cairo_move_to(cr, -lineLen, -dist);
				cairo_line_to(cr, lineLen, -dist);
				cairo_stroke(cr);
			}
			cairo_restore(cr);
		}
		if (m_hatchType == 2)
		{
			cairo_save(cr);
			cairo_rotate(cr, (-m_hatchAngle + 45) * 3.1415927 / 180.0);
			double dDist = dist * sqrt(2.0);
			cairo_new_path(cr);
			cairo_move_to(cr, -lineLen, dDist);
			cairo_line_to(cr, lineLen, dDist);
			cairo_stroke(cr);
			if (dist > 0)
			{
				cairo_new_path(cr);
				cairo_move_to(cr, -lineLen, -dDist);
				cairo_line_to(cr, lineLen, -dDist);
				cairo_stroke(cr);
			}
			cairo_restore(cr);
		}
		dist += m_hatchDistance;
	}

	cairo_pattern_t* pat = cairo_pattern_create_for_surface(img);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_BEST);

	cairo_set_source(m_cr, pat);
	cairo_clip_preserve(m_cr);

	setRasterOp(m_blendModeFill);
	if (m_maskMode > 0)
	{
		cairo_pattern_t* patM = getMaskPattern();
		cairo_pattern_set_filter(patM, CAIRO_FILTER_FAST);
		if (patM)
			cairo_mask(m_cr, patM);
		if (m_imageMask)
		{
			cairo_surface_destroy(m_imageMask);
			m_imageMask = nullptr;
		}
		cairo_pattern_destroy(patM);
	}
	else
		cairo_paint_with_alpha(m_cr, m_fillTrans);

	if (pat)
		cairo_pattern_destroy(pat);
	if (cr)
		cairo_destroy(cr);
	if (img)
		cairo_surface_destroy(img);

	newPath();
	cairo_append_path(m_cr, path);
	cairo_path_destroy(path);
}

void ScPainterEx_Cairo::strokeGradient(const VGradientEx& gradient)
{
	save();
	if (gradient.type() == VGradientEx::linear)
		strokeLinearGradient(gradient);
	else if (gradient.type() == VGradientEx::radial)
		strokeCircularGradient(gradient);
	restore();
}

void ScPainterEx_Cairo::strokeLinearGradient(const VGradientEx& gradient)
{
	double r, g, b;
	double x1 = gradient.origin().x();
	double y1 = gradient.origin().y();
	double x2 = gradient.vector().x();
	double y2 = gradient.vector().y();
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	const VColorStopEx* stop = nullptr;
	QColor color;

	cairo_push_group(m_cr);

	cairo_pattern_t* pat = cairo_pattern_create_linear (x1, y1,  x2, y2);
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);

	bool   isFirst  = true;
	double lastStop = 0.0;
	for (int index = 0 ; index < gradient.stops(); index++)
	{
		stop  = colorStops.at(index);
		if ((lastStop == stop->rampPoint) && (!isFirst))
			continue;
		isFirst = false;
		color = transformColor(ScColorShade(stop->color, stop->shade), 1.0);
		color.getRgbF(&r, &g, &b);
		cairo_pattern_add_color_stop_rgba (pat, stop->rampPoint, r, g, b, stop->opacity);
		lastStop = stop->rampPoint;
	}

	cairo_matrix_t matrix;
	QTransform qmatrix;
	qmatrix.translate(x1, y1);
	qmatrix.shear(-m_gradientSkew, 0);
	qmatrix.translate(-x1, -y1);
	cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
	cairo_matrix_invert(&matrix);
	cairo_pattern_set_matrix (pat, &matrix);
	cairo_set_source (m_cr, pat);
	cairo_stroke_preserve(m_cr);
	cairo_pattern_destroy (pat);

	cairo_pop_group_to_source (m_cr);

	setRasterOp(m_blendModeStroke);
	cairo_paint_with_alpha (m_cr, m_strokeTrans);
}

void ScPainterEx_Cairo::strokeCircularGradient(const VGradientEx& gradient)
{
	double r, g, b;
	double x1 = gradient.origin().x();
	double y1 = gradient.origin().y();
	double x2 = gradient.vector().x();
	double y2 = gradient.vector().y();
	double fx = gradient.focalPoint().x();
	double fy = gradient.focalPoint().y();
	QList<VColorStopEx*> colorStops = gradient.colorStops();
	const VColorStopEx* stop = nullptr;
	QColor color;

	cairo_push_group(m_cr);

	cairo_pattern_t* pat = cairo_pattern_create_radial (fx, fy, 0, x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)));
	cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
	cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);

	bool   isFirst  = true;
	double lastStop = 0.0;
	for (int index = 0 ; index < gradient.stops(); index++)
	{
		stop  = colorStops.at(index);
		if ((lastStop == stop->rampPoint) && (!isFirst))
			continue;
		isFirst = false;
		color = transformColor(ScColorShade(stop->color, stop->shade), 1.0);
		color.getRgbF(&r, &g, &b);
		cairo_pattern_add_color_stop_rgba (pat, stop->rampPoint, r, g, b, stop->opacity);
		lastStop = stop->rampPoint;
	}

	cairo_matrix_t matrix;
	QTransform qmatrix;
	double rotEnd = xy2Deg(x2 - x1, y2 - y1);
	qmatrix.translate(x1, y1);
	qmatrix.rotate(rotEnd);
	qmatrix.shear(m_gradientSkew, 0);
	qmatrix.translate(0, y1 * (1.0 - m_gradientScale));
	qmatrix.translate(-x1, -y1);
	qmatrix.scale(1, m_gradientScale);
	cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
	cairo_matrix_invert(&matrix);
	cairo_pattern_set_matrix (pat, &matrix);
	cairo_set_source (m_cr, pat);
	cairo_stroke_preserve(m_cr);
	cairo_pattern_destroy (pat);

	cairo_pop_group_to_source (m_cr);

	setRasterOp(m_blendModeStroke);
	cairo_paint_with_alpha (m_cr, m_strokeTrans);
}

void ScPainterEx_Cairo::getClipPathDimensions(QRect& r)
{
	double left = 0.0, right = 0.0, bottom = 0.0, top = 0.0;
	cairo_clip_extents(m_cr, &left, &bottom, &right, &top);
	r.setLeft(left);
	r.setRight(right);
	r.setBottom(bottom);
	r.setTop(top);
}
