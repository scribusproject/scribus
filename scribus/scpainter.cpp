/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scpainter.h"
#include "scpattern.h"
#include "util_color.h"
#include "util.h"
#include "util_math.h"

#include <cairo.h>
#if CAIRO_HAS_FC_FONT
#include <cairo-ft.h>
#endif
#include "text/storytext.h"
#include "text/textshaper.h"
#include "text/glyphcluster.h"

#include <cmath>
#include <QDebug>

ScPainter::ScPainter(QImage *target, int w, int h, double transparency, int blendmode)
         : m_image(target),
	       m_layerTransparency(transparency),
	       m_blendMode(blendmode)
{
	Q_ASSERT(w >= 0);
	Q_ASSERT(h >= 0);

	setHatchParameters(0, 2, 0, false, QColor(), QColor(), 0.0, 0.0);

	m_zoomStack.clear();

	cairo_surface_t *img = cairo_image_surface_create_for_data(m_image->bits(), CAIRO_FORMAT_ARGB32, w, h, w * 4);
	cairo_surface_set_device_scale(img, m_image->devicePixelRatio(), m_image->devicePixelRatio());
	m_cr = cairo_create(img);
	cairo_save(m_cr);
	cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	cairo_set_tolerance(m_cr, 0.5);
}

ScPainter::~ScPainter()
{
	cairo_surface_destroy(cairo_get_target(m_cr));
	cairo_destroy(m_cr);
}

void ScPainter::beginLayer(double transparency, int blendmode, FPointArray *clipArray)
{
	layerProp la;
	la.blendmode = m_blendMode;
	la.transparency = m_layerTransparency;
	m_layerTransparency = transparency;
	m_blendMode = blendmode;
	la.groupClip.resize(0);
	if (clipArray != nullptr)
		la.groupClip = *clipArray;
	la.maskMode = m_maskMode;
	la.maskPattern = m_maskPattern;
	la.maskPatternTrans = m_maskPatternTrans;
	la.mask_patternMirrorX = m_mask_patternMirrorX;
	la.mask_patternMirrorY = m_mask_patternMirrorY;
	la.mask_gradientScale = m_mask_gradientScale;
	la.mask_gradientSkew = m_mask_gradientSkew;
	la.mask_gradient = mask_gradient;
	la.data = cairo_get_group_target(m_cr);
	la.fillRule = m_fillRule;
	cairo_push_group(m_cr);
	m_Layers.push(la);
}

void ScPainter::endLayer()
{
	if (m_Layers.isEmpty())
		return;

	layerProp la = m_Layers.pop();
	m_maskMode = la.maskMode;
	m_maskPattern = la.maskPattern;
	m_maskPatternTrans = la.maskPatternTrans;
	m_mask_patternMirrorX = la.mask_patternMirrorX;
	m_mask_patternMirrorY = la.mask_patternMirrorY;
	m_mask_gradientScale = la.mask_gradientScale;
	m_mask_gradientSkew = la.mask_gradientSkew;
	mask_gradient = la.mask_gradient;
	m_fillRule = la.fillRule;

	cairo_pop_group_to_source (m_cr);
	if (!la.groupClip.empty())
	{
		if (m_fillRule)
			cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
		else
			cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_WINDING);
		setupPolygon(&la.groupClip);
		setClipPath();
	}
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	if (m_maskMode > 0)
	{
		cairo_pattern_t *patM = getMaskPattern();
		setRasterOp(m_blendMode);
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
		setRasterOp(m_blendMode);
		cairo_paint_with_alpha (m_cr, m_layerTransparency);
	}
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);

	m_layerTransparency = la.transparency;
	m_blendMode = la.blendmode;
	m_maskMode = 0;
}

void ScPainter::begin()
{
}

void ScPainter::end()
{
	cairo_surface_flush(cairo_get_target(m_cr));
	cairo_restore(m_cr);
}

void ScPainter::clear()
{
	if (m_imageMode)
		m_image->fill(qRgba(255, 255, 255, 255));
}

void ScPainter::clear(const QColor &c)
{
	QRgb cs = c.rgb();
	if (m_imageMode)
		m_image->fill(qRgba(qRed(cs), qGreen(cs), qBlue(cs), qAlpha(cs)));
}

cairo_t *ScPainter::context()
{
	return m_cr;
}

const QTransform ScPainter::worldMatrix()
{
	cairo_matrix_t matrix;
	cairo_get_matrix(m_cr, &matrix);
	QTransform mat(matrix.xx, matrix.yx, matrix.xy, matrix.yy, matrix.x0, matrix.y0);
	return mat;
}

void ScPainter::setWorldMatrix(const QTransform &mat)
{
	cairo_matrix_t matrix;
	cairo_matrix_init(&matrix, mat.m11(), mat.m12(), mat.m21(), mat.m22(), mat.dx(), mat.dy());
	cairo_set_matrix(m_cr, &matrix);
}

void ScPainter::setAntialiasing(bool enable)
{
	if (enable)
		cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_DEFAULT);
	else
		cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_NONE);
}

void ScPainter::setZoomFactor(double zoomFactor)
{
	m_zoomFactor = zoomFactor;
	cairo_scale (m_cr, m_zoomFactor, m_zoomFactor);
}

void ScPainter::translate(double x, double y)
{
	cairo_translate (m_cr, x, y);
}

void ScPainter::translate(const QPointF& offset)
{
	cairo_translate (m_cr, offset.x(), offset.y());
}

void ScPainter::rotate(double r)
{
	cairo_rotate (m_cr, r * 3.1415927 / 180.0);
}

void ScPainter::scale(double x, double y)
{
	cairo_scale (m_cr, x, y);
	m_zoomFactor *= qMax(x, y);
}

void ScPainter::moveTo(const double &x, const double &y)
{
	cairo_move_to(m_cr, x, y);
}

void
ScPainter::lineTo(const double &x, const double &y)
{
	cairo_line_to(m_cr, x, y);
}

void ScPainter::curveTo(const FPoint& p1, const FPoint& p2, const FPoint& p3)
{
	cairo_curve_to(m_cr, p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
}

void ScPainter::newPath()
{
	cairo_new_path(m_cr);
}

void ScPainter::closePath()
{
	cairo_close_path(m_cr);
}

void ScPainter::setFillRule(bool fillRule)
{
	m_fillRule = fillRule;
}

void ScPainter::setFillMode(int fill)
{
	m_fillMode = fill;
}

void ScPainter::setStrokeMode(int stroke)
{
	m_strokeMode = stroke;
}

void ScPainter::setGradient(VGradient::VGradientType mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew)
{
	fill_gradient.setType(mode);
	fill_gradient.setOrigin(orig);
	fill_gradient.setVector(vec);
	fill_gradient.setFocalPoint(foc);
	stroke_gradient.setType(mode);
	stroke_gradient.setOrigin(orig);
	stroke_gradient.setVector(vec);
	stroke_gradient.setFocalPoint(foc);
	m_gradientScale = scale;
	if (skew == 90.0)
		m_gradientSkew = 1;
	else if (skew == 180.0)
		m_gradientSkew = 0;
	else if (skew == 270.0)
		m_gradientSkew = -1;
	else if (skew == 360.0)
		m_gradientSkew = 0;
	else
		m_gradientSkew = tan(M_PI / 180.0 * skew);
}

void ScPainter::setMaskMode(int mask)
{
	m_maskMode = mask;
}

void ScPainter::setGradientMask(VGradient::VGradientType mode, const FPoint& orig, const FPoint& vec, const FPoint& foc, double scale, double skew)
{
	mask_gradient.setType(mode);
	mask_gradient.setOrigin(orig);
	mask_gradient.setVector(vec);
	mask_gradient.setFocalPoint(foc);
	m_mask_gradientScale = scale;
	if (skew == 90.0)
		m_mask_gradientSkew = 1;
	else if (skew == 180.0)
		m_mask_gradientSkew = 0;
	else if (skew == 270.0)
		m_mask_gradientSkew = -1;
	else if (skew == 360.0)
		m_mask_gradientSkew = 0;
	else
		m_mask_gradientSkew = tan(M_PI / 180.0 * skew);
}

void ScPainter::setPatternMask(ScPattern *pattern, const ScMaskTransform& maskTrans, bool mirrorX, bool mirrorY)
{
	m_maskPattern = pattern;
	m_maskPatternTrans = maskTrans;
	m_mask_patternMirrorX = mirrorX;
	m_mask_patternMirrorY = mirrorY;
}

void ScPainter::set4ColorGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4)
{
	fill_gradient.setType(VGradient::fourcolor);
	gradPatchP1 = p1;
	gradPatchP2 = p2;
	gradPatchP3 = p3;
	gradPatchP4 = p4;
	gradControlP1 = c1;
	gradControlP2 = c2;
	gradControlP3 = c3;
	gradControlP4 = c4;
}

void ScPainter::set4ColorColors(const QColor& col1, const QColor& col2, const QColor& col3, const QColor& col4)
{
	gradPatchColor1 = col1;
	gradPatchColor2 = col2;
	gradPatchColor3 = col3;
	gradPatchColor4 = col4;
}

void ScPainter::setDiamondGeometry(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const FPoint& c1, const FPoint& c2, const FPoint& c3, const FPoint& c4, const FPoint& c5)
{
	fill_gradient.setType(VGradient::diamond);
	gradPatchP1 = p1;
	gradPatchP2 = p2;
	gradPatchP3 = p3;
	gradPatchP4 = p4;
	gradControlP1 = c1;
	gradControlP2 = c2;
	gradControlP3 = c3;
	gradControlP4 = c4;
	gradControlP5 = c5;
}

void ScPainter::setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const QList<QList<MeshPoint>>& meshArray)
{
	fill_gradient.setType(VGradient::mesh);
	meshGradientArray = meshArray;
	gradPatchP1 = p1;
	gradPatchP2 = p2;
	gradPatchP3 = p3;
	gradPatchP4 = p4;
}

void ScPainter::setMeshGradient(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& p4, const QList<meshGradientPatch>& meshPatches)
{
	fill_gradient.setType(VGradient::freemesh);
	meshGradientPatches = meshPatches;
	gradPatchP1 = p1;
	gradPatchP2 = p2;
	gradPatchP3 = p3;
	gradPatchP4 = p4;
}

void ScPainter::setHatchParameters(int mode, double distance, double angle, bool useBackground, const QColor& background, const QColor& foreground, double width, double height)
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

void ScPainter::fillPath()
{
	if (m_fillMode != 0)
		fillPathHelper();
}

void ScPainter::strokePath()
{
	if (m_strokeMode != 0)
		strokePathHelper();
}

QColor ScPainter::pen()
{
	return m_stroke;
}

QColor ScPainter::brush()
{
	return m_fill;
}

double ScPainter::brushOpacity()
{
	return m_fill_trans;
}

void ScPainter::setPen(const QColor &c)
{
	m_stroke = c;
}

void ScPainter::setPen(const QColor &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo)
{
	m_stroke = c;
	m_LineWidth = w;
	PLineEnd = ca;
	PLineJoin = jo;
	m_offset = 0;
	getDashArray(st, w, m_array);
}

void ScPainter::setLineWidth(double w)
{
	m_LineWidth = w;
}

void ScPainter::setPenOpacity(double op)
{
	m_stroke_trans = op;
}


void ScPainter::setDash(const QVector<double>& array, double ofs)
{
	m_array = array;
	m_offset = ofs;
}

void ScPainter::setBrush(const QColor &c)
{
	m_fill = c;
}

void ScPainter::setBrushOpacity(double op)
{
	m_fill_trans = op;
}

void ScPainter::setOpacity(double op)
{
	m_fill_trans = op;
	m_stroke_trans = op;
}

void ScPainter::setFont(const ScFace &f, double s)
{
	m_font = f;
	m_fontSize = s;
}

void ScPainter::save()
{
	cairo_save(m_cr);
	m_zoomStack.push(m_zoomFactor);
}

void ScPainter::restore()
{
	cairo_restore(m_cr);
	if (!m_zoomStack.isEmpty())
		m_zoomFactor = m_zoomStack.pop();
}

int ScPainter::blendModeFill()
{
	return m_blendModeFill;
}

void ScPainter::setRasterOp(int blendMode)
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

void ScPainter::setBlendModeFill(int blendMode)
{
	m_blendModeFill = blendMode;
}

void ScPainter::setBlendModeStroke(int blendMode)
{
	m_blendModeStroke = blendMode;
}

void ScPainter::setPattern(ScPattern *pattern, const ScPatternTransform& patternTrans, bool mirrorX, bool mirrorY)
{
	m_pattern = pattern;
	m_patternTrans = patternTrans;
	m_patternMirrorX = mirrorX;
	m_patternMirrorY = mirrorY;
}

cairo_pattern_t * ScPainter::getMaskPattern()
{
	cairo_save(m_cr);
	cairo_pattern_t *pat { nullptr };
	if ((m_maskMode == 1) || (m_maskMode == 3))
	{
		double x1 = mask_gradient.origin().x();
		double y1 = mask_gradient.origin().y();
		double x2 = mask_gradient.vector().x();
		double y2 = mask_gradient.vector().y();
		double fx = mask_gradient.focalPoint().x();
		double fy = mask_gradient.focalPoint().y();
		if (mask_gradient.type() == VGradient::linear)
			pat = cairo_pattern_create_linear (x1, y1,  x2, y2);
		else
			pat = cairo_pattern_create_radial (fx, fy, 0, x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)));
		QList<VColorStop*> colorStops = mask_gradient.colorStops();
		QColor qStopColor;
		for (int offset = 0 ; offset < colorStops.count() ; offset++)
		{
			qStopColor = colorStops[ offset ]->color;
			double a = colorStops[offset]->opacity;
			double r, g, b;
			qStopColor.getRgbF(&r, &g, &b);
			if (m_maskMode == 3)
				a = /* 1.0 - */(0.3 * r + 0.59 * g + 0.11 * b);
			cairo_pattern_add_color_stop_rgba (pat, colorStops[ offset ]->rampPoint, r, g, b, a);
		}
		cairo_matrix_t matrix;
		QTransform qmatrix;
		if (mask_gradient.type() == VGradient::radial)
		{
			double rotEnd = xy2Deg(x2 - x1, y2 - y1);
			qmatrix.translate(x1, y1);
			qmatrix.rotate(rotEnd);
			qmatrix.shear(m_mask_gradientSkew, 0);
			qmatrix.translate(0, y1 * (1.0 - m_mask_gradientScale));
			qmatrix.translate(-x1, -y1);
			qmatrix.scale(1, m_mask_gradientScale);
		}
		else
		{
			qmatrix.translate(x1, y1);
			qmatrix.shear(-m_mask_gradientSkew, 0);
			qmatrix.translate(-x1, -y1);
		}
		cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
		cairo_matrix_invert(&matrix);
		cairo_pattern_set_matrix (pat, &matrix);
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
			QRgb *s;
			QRgb r;
			for (int yi = 0; yi < h; ++yi)
			{
				s = (QRgb*)(m_imageQ.scanLine(yi));
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
			m_imageMask = cairo_image_surface_create_for_data (m_imageQ.bits(), CAIRO_FORMAT_ARGB32, w, h, w * 4);
		}
		else
		{
			m_imageQ = m_maskPattern->pattern.copy();
			if (m_maskMode == 6)
				m_imageQ.invertPixels(QImage::InvertRgba);
			m_imageMask = cairo_image_surface_create_for_data (m_imageQ.bits(), CAIRO_FORMAT_ARGB32, m_maskPattern->getPattern().width(), m_maskPattern->getPattern().height(), m_maskPattern->getPattern().width() * 4);
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
		qmatrix.scale(m_maskPattern->width / static_cast<double>(m_maskPattern->getPattern().width()), m_maskPattern->height / static_cast<double>(m_maskPattern->getPattern().height()));
		if (m_mask_patternMirrorX)
			qmatrix.scale(-1, 1);
		if (m_mask_patternMirrorY)
			qmatrix.scale(1, -1);
		cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
		cairo_matrix_invert(&matrix);
		cairo_pattern_set_matrix (pat, &matrix);
	}
	cairo_restore(m_cr);
	return pat;
}

void ScPainter::fillPathHelper()
{
	cairo_save(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	if (m_fillRule)
		cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
	else
		cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_WINDING);
	if (m_fillMode == 1)
	{
		double r, g, b;
		m_fill.getRgbF(&r, &g, &b);
		if (m_maskMode > 0)
		{
			cairo_pattern_t *pat = getMaskPattern();
			cairo_set_source_rgb(m_cr, r, g, b);
			setRasterOp(m_blendModeFill);
			cairo_clip_preserve(m_cr);
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
			cairo_set_source_rgba(m_cr, r, g, b, m_fill_trans);
			setRasterOp(m_blendModeFill);
			cairo_fill_preserve(m_cr);
		}
	}
	else if (m_fillMode == 2)
	{
		cairo_pattern_t *pat = nullptr;
		cairo_surface_t *img = nullptr;
		cairo_t *cr = nullptr;
		cairo_pattern_t *mpat = nullptr;
		if (fill_gradient.type() == VGradient::fourcolor)
		{
			double p1x = gradPatchP1.x();
			double p1y = gradPatchP1.y();
			double p2x = gradPatchP2.x();
			double p2y = gradPatchP2.y();
			double p3x = gradPatchP3.x();
			double p3y = gradPatchP3.y();
			double p4x = gradPatchP4.x();
			double p4y = gradPatchP4.y();
			img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, p3x, p3y);
			cr = cairo_create(img);
			cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
			cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
			cairo_set_tolerance(cr, 0.5);
			double r, g, b, a;
			mpat = cairo_pattern_create_mesh();
			cairo_mesh_pattern_begin_patch(mpat);
			cairo_mesh_pattern_move_to(mpat, p1x, p1y);
			cairo_mesh_pattern_line_to(mpat, p2x, p2y);
			cairo_mesh_pattern_line_to(mpat, p3x, p3y);
			cairo_mesh_pattern_line_to(mpat, p4x, p4y);
			cairo_mesh_pattern_line_to(mpat, p1x, p1y);
			cairo_mesh_pattern_set_control_point(mpat, 0, gradControlP1.x(),  gradControlP1.y());
			cairo_mesh_pattern_set_control_point(mpat, 1, gradControlP2.x(),  gradControlP2.y());
			cairo_mesh_pattern_set_control_point(mpat, 2, gradControlP3.x(),  gradControlP3.y());
			cairo_mesh_pattern_set_control_point(mpat, 3, gradControlP4.x(),  gradControlP4.y());
			gradPatchColor1.getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
			gradPatchColor2.getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
			gradPatchColor3.getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
			gradPatchColor4.getRgbF(&r, &g, &b, &a);
			cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
			cairo_mesh_pattern_end_patch(mpat);
			cairo_pattern_set_filter(mpat, CAIRO_FILTER_GOOD);
			cairo_set_source(cr, mpat);
			cairo_paint_with_alpha(cr, 1.0);
			pat = cairo_pattern_create_for_surface(img);
			cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
			cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);
		}
		else if (fill_gradient.type() == VGradient::diamond)
		{
			double p1x = gradControlP1.x();
			double p1y = gradControlP1.y();
			double p2x = gradControlP2.x();
			double p2y = gradControlP2.y();
			double p3x = gradControlP3.x();
			double p3y = gradControlP3.y();
			double p4x = gradControlP4.x();
			double p4y = gradControlP4.y();
			img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, gradPatchP3.x(), gradPatchP3.y());
			cr = cairo_create(img);
			cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
			cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
			cairo_set_tolerance(cr, 0.5);
			double r, g, b, a;
			QList<VColorStop*> colorStops = fill_gradient.colorStops();
			QList<QColor> qStopColors;
			QList<double> qStopRampPoints;
			QColor qStopColor;
			for (int offset = 0 ; offset < colorStops.count() ; offset++)
			{
				qStopColor = colorStops[ offset ]->color;
				qStopColor.setAlphaF(colorStops[offset]->opacity);
				if (offset == 0)
				{
					if (colorStops[offset]->rampPoint > 0)
					{
						qStopRampPoints.append(0);
						qStopColors.append(qStopColor);
					}
				}
				qStopColors.append(qStopColor);
				qStopRampPoints.append(colorStops[offset]->rampPoint);
			}
			qStopColors.last().getRgbF(&r, &g, &b, &a);
			QPointF centerP(gradControlP5.x(), gradControlP5.y());
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
			mpat = cairo_pattern_create_mesh();
			for (int offset = 1 ; offset < qStopRampPoints.count() ; offset++)
			{
				QLineF e1 = edge1;
				QLineF e1s = edge1;
				QLineF e2 = edge2;
				QLineF e2s = edge2;
				QLineF e3 = edge3;
				QLineF e3s = edge3;
				QLineF e4 = edge4;
				QLineF e4s = edge4;
				e1.setLength(edge1.length() * qStopRampPoints[ offset ]);
				e2.setLength(edge2.length() * qStopRampPoints[ offset ]);
				e3.setLength(edge3.length() * qStopRampPoints[ offset ]);
				e4.setLength(edge4.length() * qStopRampPoints[ offset ]);
				e1s.setLength(edge1.length() * qStopRampPoints[ offset - 1 ]);
				e2s.setLength(edge2.length() * qStopRampPoints[ offset - 1 ]);
				e3s.setLength(edge3.length() * qStopRampPoints[ offset - 1 ]);
				e4s.setLength(edge4.length() * qStopRampPoints[ offset - 1 ]);
				if (offset == 1)
				{
					cairo_mesh_pattern_begin_patch(mpat);
					cairo_mesh_pattern_move_to(mpat, centerP.x(), centerP.y());
					cairo_mesh_pattern_line_to(mpat, e1.x2(), e1.y2());
					cairo_mesh_pattern_line_to(mpat, e2.x2(), e2.y2());
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
					qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
					cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
					qStopColors[offset].getRgbF(&r, &g, &b, &a);
					cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
					cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
					qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
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
					qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
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
					qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
					cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
					cairo_mesh_pattern_end_patch(mpat);
					cairo_mesh_pattern_begin_patch(mpat);
					cairo_mesh_pattern_move_to(mpat, e4s.x2(), e4s.y2());
					cairo_mesh_pattern_line_to(mpat, e4.x2(), e4.y2());
					cairo_mesh_pattern_line_to(mpat, e1.x2(), e1.y2());
					cairo_mesh_pattern_line_to(mpat, e1s.x2(), e1s.y2());
					qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
					cairo_mesh_pattern_set_corner_color_rgba(mpat, 0, r, g, b, a);
					qStopColors[offset].getRgbF(&r, &g, &b, &a);
					cairo_mesh_pattern_set_corner_color_rgba(mpat, 1, r, g, b, a);
					cairo_mesh_pattern_set_corner_color_rgba(mpat, 2, r, g, b, a);
					qStopColors[offset - 1].getRgbF(&r, &g, &b, &a);
					cairo_mesh_pattern_set_corner_color_rgba(mpat, 3, r, g, b, a);
					cairo_mesh_pattern_end_patch(mpat);
				}
			}
			cairo_pattern_set_filter(mpat, CAIRO_FILTER_GOOD);
			cairo_set_source(cr, mpat);
			cairo_paint_with_alpha(cr, 1.0);
			pat = cairo_pattern_create_for_surface(img);
			cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
			cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);
		}
		else if (fill_gradient.type() == VGradient::mesh)
		{
			double p3x = gradPatchP3.x();
			double p3y = gradPatchP3.y();
			img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, p3x, p3y);
			cr = cairo_create(img);
			cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
			cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
			cairo_set_tolerance(cr, 0.5);
			double r, g, b, a;
			mpat = cairo_pattern_create_mesh();
			for (int grow = 0; grow < meshGradientArray.count() - 1; ++grow)
			{
				for (int gcol = 0; gcol < meshGradientArray[grow].count() - 1; ++gcol)
				{
					MeshPoint mp1 = meshGradientArray[grow][gcol];
					MeshPoint mp2 = meshGradientArray[grow][gcol + 1];
					MeshPoint mp3 = meshGradientArray[grow + 1][gcol + 1];
					MeshPoint mp4 = meshGradientArray[grow + 1][gcol];
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
			pat = cairo_pattern_create_for_surface(img);
			cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
			cairo_pattern_set_filter(pat, CAIRO_FILTER_BEST);
		}
		else if (fill_gradient.type() == VGradient::freemesh)
		{
			double p3x = gradPatchP3.x();
			double p3y = gradPatchP3.y();
			img = cairo_surface_create_similar(cairo_get_target(m_cr), CAIRO_CONTENT_COLOR_ALPHA, p3x, p3y);
			cr = cairo_create(img);
			cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
			cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
			cairo_set_tolerance(cr, 0.5);
			double r, g, b, a;
			mpat = cairo_pattern_create_mesh();
			for (int col = 0; col < meshGradientPatches.count(); col++)
			{
				meshGradientPatch patch = meshGradientPatches[col];
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
			pat = cairo_pattern_create_for_surface(img);
			cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
			cairo_pattern_set_filter(pat, CAIRO_FILTER_BEST);
		}
		else
		{
			bool   isFirst = true;
			double rampPoint, lastPoint = 0.0;
			double x1 = fill_gradient.origin().x();
			double y1 = fill_gradient.origin().y();
			double x2 = fill_gradient.vector().x();
			double y2 = fill_gradient.vector().y();
			double fx = fill_gradient.focalPoint().x();
			double fy = fill_gradient.focalPoint().y();
			if (fill_gradient.type() == VGradient::linear)
				pat = cairo_pattern_create_linear (x1, y1,  x2, y2);
			else
				pat = cairo_pattern_create_radial (fx, fy, 0, x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)));
			if (fill_gradient.repeatMethod() == VGradient::none)
				cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
			else
				cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
			cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);
			QList<VColorStop*> colorStops = fill_gradient.colorStops();
			for (int offset = 0 ; offset < colorStops.count() ; offset++)
			{
				rampPoint  = colorStops[ offset ]->rampPoint;
				if ((lastPoint == rampPoint) && (!isFirst))
					continue;
				isFirst = false;
				double a = colorStops[offset]->opacity;
				double r, g, b;
				colorStops[ offset ]->color.getRgbF(&r, &g, &b);
				cairo_pattern_add_color_stop_rgba (pat, colorStops[ offset ]->rampPoint, r, g, b, a);
				lastPoint = rampPoint;
			}
			cairo_matrix_t matrix;
			QTransform qmatrix;
			if (fill_gradient.type() == VGradient::radial)
			{
				double rotEnd = xy2Deg(x2 - x1, y2 - y1);
				qmatrix.translate(x1, y1);
				qmatrix.rotate(rotEnd);
				qmatrix.shear(m_gradientSkew, 0);
				qmatrix.translate(0, y1 * (1.0 - m_gradientScale));
				qmatrix.translate(-x1, -y1);
				qmatrix.scale(1, m_gradientScale);
			}
			else
			{
				qmatrix.translate(x1, y1);
				qmatrix.shear(-m_gradientSkew, 0);
				qmatrix.translate(-x1, -y1);
			}
			cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
			cairo_matrix_invert(&matrix);
			cairo_pattern_set_matrix (pat, &matrix);
		}
		cairo_set_source (m_cr, pat);
		cairo_clip_preserve (m_cr);
		if (m_maskMode > 0)
		{
			cairo_pattern_t *patM = getMaskPattern();
			setRasterOp(m_blendModeFill);
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
			cairo_paint_with_alpha (m_cr, m_fill_trans);
		}
		cairo_pattern_destroy (pat);
		if ((fill_gradient.type() == VGradient::fourcolor) || (fill_gradient.type() == VGradient::diamond) || (fill_gradient.type() == VGradient::mesh) || (fill_gradient.type() == VGradient::freemesh))
		{
			cairo_surface_destroy(img);
			cairo_pattern_destroy(mpat);
			cairo_destroy(cr);
		}
	}
	else if (m_fillMode == 3)
	{
		cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_NONE);
		cairo_surface_t *image2 = cairo_image_surface_create_for_data (m_pattern->getPattern().bits(), CAIRO_FORMAT_ARGB32, m_pattern->getPattern().width(), m_pattern->getPattern().height(), m_pattern->getPattern().width() * 4);
		cairo_pattern_t *m_pat = cairo_pattern_create_for_surface(image2);
		cairo_pattern_set_extend(m_pat, CAIRO_EXTEND_REPEAT);
		cairo_pattern_set_filter(m_pat, CAIRO_FILTER_GOOD);
		cairo_matrix_t matrix;
		QTransform qmatrix;
		qmatrix.translate(m_patternTrans.offsetX, m_patternTrans.offsetY);
		qmatrix.rotate(m_patternTrans.rotation);
		qmatrix.shear(-m_patternTrans.skewX, m_patternTrans.skewY);
		qmatrix.scale(m_patternTrans.scaleX, m_patternTrans.scaleY);
		qmatrix.scale(m_pattern->width / static_cast<double>(m_pattern->getPattern().width()), m_pattern->height / static_cast<double>(m_pattern->getPattern().height()));
		if (m_patternMirrorX)
			qmatrix.scale(-1, 1);
		if (m_patternMirrorY)
			qmatrix.scale(1, -1);
		cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
		cairo_matrix_invert(&matrix);
		cairo_pattern_set_matrix (m_pat, &matrix);
		cairo_set_source (m_cr, m_pat);
		cairo_clip_preserve (m_cr);
		if (m_maskMode > 0)
		{
			cairo_pattern_t *patM = getMaskPattern();
			setRasterOp(m_blendModeFill);
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
			cairo_paint_with_alpha (m_cr, m_fill_trans);
		}
		cairo_pattern_destroy (m_pat);
		cairo_surface_destroy (image2);
		cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_DEFAULT);
	}
	else if (m_fillMode == 4)
	{
		cairo_path_t *path = cairo_copy_path(m_cr);
		cairo_push_group(m_cr);
		if (m_hatchUseBackground && m_hatchBackground.isValid())
		{
			double r2, g2, b2;
			m_hatchBackground.getRgbF(&r2, &g2, &b2);
			cairo_set_source_rgb(m_cr, r2, g2, b2);
			cairo_fill_preserve(m_cr);
		}
		double r, g, b;
		m_hatchForeground.getRgbF(&r, &g, &b);
		cairo_clip_preserve (m_cr);
		cairo_set_line_width(m_cr, 1);
		cairo_set_source_rgb(m_cr, r, g, b);
		translate(m_hatchWidth / 2.0, m_hatchHeight / 2.0);
		double lineLen = sqrt((m_hatchWidth / 2.0) * (m_hatchWidth / 2.0) + (m_hatchHeight / 2.0) * (m_hatchHeight / 2.0));
		double dist = 0.0;
		while (dist < lineLen)
		{
			cairo_save(m_cr);
			rotate(-m_hatchAngle);
			newPath();
			moveTo(-lineLen, dist);
			lineTo(lineLen, dist);
			cairo_stroke(m_cr);
			if (dist > 0)
			{
				newPath();
				moveTo(-lineLen, -dist);
				lineTo(lineLen, -dist);
				cairo_stroke(m_cr);
			}
			cairo_restore(m_cr);
			if ((m_hatchType == 1) || (m_hatchType == 2))
			{
				cairo_save(m_cr);
				rotate(-m_hatchAngle + 90);
				newPath();
				moveTo(-lineLen, dist);
				lineTo(lineLen, dist);
				cairo_stroke(m_cr);
				if (dist > 0)
				{
					newPath();
					moveTo(-lineLen, -dist);
					lineTo(lineLen, -dist);
					cairo_stroke(m_cr);
				}
				cairo_restore(m_cr);
			}
			if (m_hatchType == 2)
			{
				cairo_save(m_cr);
				rotate(-m_hatchAngle + 45);
				double dDist = dist * sqrt(2.0);
				newPath();
				moveTo(-lineLen, dDist);
				lineTo(lineLen, dDist);
				cairo_stroke(m_cr);
				if (dist > 0)
				{
					newPath();
					moveTo(-lineLen, -dDist);
					lineTo(lineLen, -dDist);
					cairo_stroke(m_cr);
				}
				cairo_restore(m_cr);
			}
			dist += m_hatchDistance;
		}
		cairo_pop_group_to_source (m_cr);
		setRasterOp(m_blendModeFill);
		if (m_maskMode > 0)
		{
			cairo_pattern_t *patM = getMaskPattern();
			cairo_pattern_set_filter(patM, CAIRO_FILTER_FAST);
			cairo_mask(m_cr, patM);
			if (m_imageMask)
			{
				cairo_surface_destroy(m_imageMask);
				m_imageMask = nullptr;
			}
			cairo_pattern_destroy(patM);
		}
		else
			cairo_paint_with_alpha (m_cr, m_fill_trans);
		newPath();
		cairo_append_path(m_cr, path);
		cairo_path_destroy(path);
	}
	cairo_restore(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
}

void ScPainter::strokePathHelper()
{
	cairo_save(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	if (m_LineWidth == 0)
		cairo_set_line_width(m_cr, 1.0 / m_zoomFactor);
	else
		cairo_set_line_width(m_cr, m_LineWidth);
	if (m_array.count() > 0)
		cairo_set_dash(m_cr, m_array.data(), m_array.count(), m_offset);
	else
		cairo_set_dash(m_cr, nullptr, 0, 0);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	if (PLineEnd == Qt::RoundCap)
		cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_ROUND);
	else if (PLineEnd == Qt::SquareCap)
		cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_SQUARE);
	else if (PLineEnd == Qt::FlatCap)
		cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_BUTT);
	if (PLineJoin == Qt::RoundJoin)
		cairo_set_line_join(m_cr, CAIRO_LINE_JOIN_ROUND);
	else if (PLineJoin == Qt::BevelJoin)
		cairo_set_line_join(m_cr, CAIRO_LINE_JOIN_BEVEL);
	else if (PLineJoin == Qt::MiterJoin)
		cairo_set_line_join(m_cr, CAIRO_LINE_JOIN_MITER);
	if (m_strokeMode == 3)
	{
		cairo_push_group(m_cr);
		cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_NONE);
		cairo_surface_t *image2 = cairo_image_surface_create_for_data (m_pattern->getPattern().bits(), CAIRO_FORMAT_ARGB32, m_pattern->getPattern().width(), m_pattern->getPattern().height(), m_pattern->getPattern().width() * 4);
		cairo_pattern_t *m_pat = cairo_pattern_create_for_surface(image2);
		cairo_pattern_set_extend(m_pat, CAIRO_EXTEND_REPEAT);
		cairo_pattern_set_filter(m_pat, CAIRO_FILTER_GOOD);
		cairo_matrix_t matrix;
		QTransform qmatrix;
		qmatrix.translate(-m_LineWidth / 2.0, -m_LineWidth / 2.0);
		qmatrix.translate(m_patternTrans.offsetX, m_patternTrans.offsetY);
		qmatrix.rotate(m_patternTrans.rotation);
		qmatrix.shear(-m_patternTrans.skewX, m_patternTrans.skewY);
		qmatrix.scale(m_patternTrans.scaleX, m_patternTrans.scaleY);
		qmatrix.scale(m_pattern->width / static_cast<double>(m_pattern->getPattern().width()), m_pattern->height / static_cast<double>(m_pattern->getPattern().height()));
		if (m_patternMirrorX)
			qmatrix.scale(-1, 1);
		if (m_patternMirrorY)
			qmatrix.scale(1, -1);
		cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
		cairo_matrix_invert(&matrix);
		cairo_pattern_set_matrix (m_pat, &matrix);
		cairo_set_source (m_cr, m_pat);
		cairo_stroke_preserve(m_cr);
		cairo_pattern_destroy (m_pat);
		cairo_surface_destroy (image2);
		cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_DEFAULT);
		cairo_pop_group_to_source (m_cr);
		setRasterOp(m_blendModeStroke);
		cairo_paint_with_alpha (m_cr, m_stroke_trans);
	}
	else if (m_strokeMode == 2)
	{
		cairo_push_group(m_cr);
		cairo_pattern_t *pat;
		bool   isFirst = true;
		double rampPoint, lastPoint = 0.0;
		double x1 = stroke_gradient.origin().x();
		double y1 = stroke_gradient.origin().y();
		double x2 = stroke_gradient.vector().x();
		double y2 = stroke_gradient.vector().y();
		double fx = stroke_gradient.focalPoint().x();
		double fy = stroke_gradient.focalPoint().y();
		if (stroke_gradient.type() == VGradient::linear)
			pat = cairo_pattern_create_linear (x1, y1,  x2, y2);
		else
			pat = cairo_pattern_create_radial (fx, fy, 0, x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)));
		if (stroke_gradient.repeatMethod() == VGradient::none)
			cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
		else
			cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
		cairo_pattern_set_filter(pat, CAIRO_FILTER_GOOD);
		QList<VColorStop*> colorStops = stroke_gradient.colorStops();
		for (int offset = 0 ; offset < colorStops.count() ; offset++)
		{
			rampPoint  = colorStops[ offset ]->rampPoint;
			if ((lastPoint == rampPoint) && (!isFirst))
				continue;
			isFirst = false;
			double a = colorStops[offset]->opacity;
			double r, g, b;
			colorStops[ offset ]->color.getRgbF(&r, &g, &b);
			cairo_pattern_add_color_stop_rgba (pat, rampPoint, r, g, b, a);
			lastPoint = rampPoint;
		}
		cairo_matrix_t matrix;
		QTransform qmatrix;
		if (stroke_gradient.type() == VGradient::radial)
		{
			double rotEnd = xy2Deg(x2 - x1, y2 - y1);
			qmatrix.translate(x1, y1);
			qmatrix.rotate(rotEnd);
			qmatrix.shear(m_gradientSkew, 0);
			qmatrix.translate(0, y1 * (1.0 - m_gradientScale));
			qmatrix.translate(-x1, -y1);
			qmatrix.scale(1, m_gradientScale);
		}
		else
		{
			qmatrix.translate(x1, y1);
			qmatrix.shear(-m_gradientSkew, 0);
			qmatrix.translate(-x1, -y1);
		}
		cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
		cairo_matrix_invert(&matrix);
		cairo_pattern_set_matrix (pat, &matrix);
		cairo_set_source (m_cr, pat);
		cairo_stroke_preserve(m_cr);
		cairo_pattern_destroy (pat);
		cairo_pop_group_to_source (m_cr);
		setRasterOp(m_blendModeStroke);
		cairo_paint_with_alpha (m_cr, m_stroke_trans);
	}
	else
	{
		double r, g, b;
		m_stroke.getRgbF(&r, &g, &b);
		cairo_set_source_rgba(m_cr, r, g, b, m_stroke_trans);
		setRasterOp(m_blendModeStroke);
		cairo_stroke_preserve(m_cr);
	}
	cairo_restore(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
}

void ScPainter::setClipPath()
{
	cairo_clip (m_cr);
}

void ScPainter::drawImage(QImage *image)
{
	cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_NONE);
/*
	cairo_surface_t *image3 = cairo_image_surface_create_for_data ((uchar*)image->bits(), CAIRO_FORMAT_ARGB32, image->width(), image->height(), image->width() * 4);
	cairo_set_source_surface (m_cr, image3, 0, 0);
	cairo_paint_with_alpha (m_cr, fill_trans);
	cairo_surface_destroy (image3);
*/
/* Code with Layers, crashes on cairo_push_group */
//	cairo_scale(m_cr, m_zoomFactor, m_zoomFactor);
	cairo_push_group(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	cairo_set_fill_rule(m_cr, cairo_get_fill_rule(m_cr));
	cairo_surface_t *image2  = cairo_image_surface_create_for_data (image->bits(), CAIRO_FORMAT_RGB24, image->width(), image->height(), image->width() * 4);
	cairo_surface_t *image3 = cairo_image_surface_create_for_data (image->bits(), CAIRO_FORMAT_ARGB32, image->width(), image->height(), image->width() * 4);
	cairo_set_source_surface (m_cr, image2, 0, 0);
	cairo_pattern_set_filter(cairo_get_source(m_cr), CAIRO_FILTER_GOOD);
	cairo_mask_surface (m_cr, image3, 0, 0);
	cairo_surface_destroy (image2);
	cairo_surface_destroy (image3);
	cairo_pop_group_to_source (m_cr);
	cairo_pattern_set_filter(cairo_get_source(m_cr), CAIRO_FILTER_GOOD);
	setRasterOp(m_blendModeFill);
	if (m_maskMode > 0)
	{
		cairo_pattern_t *patM = getMaskPattern();
		cairo_pattern_set_filter(patM, CAIRO_FILTER_GOOD);
		cairo_mask(m_cr, patM);
		if (m_imageMask)
		{
			cairo_surface_destroy(m_imageMask);
			m_imageMask = nullptr;
		}
		cairo_pattern_destroy(patM);
	}
	else
		cairo_paint_with_alpha (m_cr, m_fill_trans);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_DEFAULT);
}

void ScPainter::setupPolygon(const FPointArray *points, bool closed)
{
	bool nPath = true;
	bool first = true;
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
			cairo_move_to(m_cr, np.x(), np.y());
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
			cairo_line_to(m_cr, np3.x(), np3.y());
		else
			cairo_curve_to(m_cr, np1.x(), np1.y(), np2.x(), np2.y(), np3.x(), np3.y());
		np4 = np3;
	}
	if (closed)
		cairo_close_path(m_cr);
}

void ScPainter::setupSharpPolygon(const FPointArray *points, bool closed)
{
	bool nPath = true;
	bool first = true;
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
			sharpLineHelper(np);
			cairo_move_to(m_cr, np.x(), np.y());
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
		{
			sharpLineHelper(np3);
			cairo_line_to(m_cr, np3.x(), np3.y());
		}
		else
			cairo_curve_to(m_cr, np1.x(), np1.y(), np2.x(), np2.y(), np3.x(), np3.y());
		np4 = np3;
	}
	if (closed)
		cairo_close_path(m_cr);
}

void ScPainter::sharpLineHelper(FPoint &pp)
{
	double x1 = pp.x();
	double y1 = pp.y();
	cairo_user_to_device (m_cr, &x1, &y1);
	x1 = floor(x1) + 0.5;
	y1 = floor(y1) + 0.5;
	cairo_device_to_user (m_cr, &x1, &y1);
	pp.setXY(x1, y1);
}

void ScPainter::sharpLineHelper(QPointF &pp)
{
	double x1 = pp.x();
	double y1 = pp.y();
	cairo_user_to_device (m_cr, &x1, &y1);
	x1 = floor(x1) + 0.5;
	y1 = floor(y1) + 0.5;
	cairo_device_to_user (m_cr, &x1, &y1);
	pp.setX(x1);
	pp.setY(y1);
}

void ScPainter::drawPolygon()
{
	fillPath();
}

void ScPainter::drawPolyLine()
{
	strokePath();
}

void ScPainter::drawLine(const FPoint& start, const FPoint& end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainter::drawLine(const QPointF& start, const QPointF& end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainter::drawSharpLine(FPoint start, FPoint end)
{
	newPath();
	sharpLineHelper(start);
	sharpLineHelper(end);
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainter::drawSharpLine(QPointF start, QPointF end)
{
	newPath();
	sharpLineHelper(start);
	sharpLineHelper(end);
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainter::drawRect(double x, double y, double w, double h)
{
	newPath();
	cairo_rectangle(m_cr, x, y, w, h);
	fillPath();
	strokePath();
}

void ScPainter::drawSharpRect(double x, double y, double w, double h)
{
	newPath();
	double x1 = x;
	double y1 = y;
	double w1 = w;
	double h1 = h;
	// see http://www.cairographics.org/FAQ/#sharp_lines
	cairo_user_to_device (m_cr, &x1, &y1);
	cairo_user_to_device (m_cr, &w1, &h1);
	x1 = floor(x1) + 0.5;
	y1 = floor(y1) + 0.5;
	w1 = floor(w1) + 0.5;
	h1 = floor(h1) + 0.5;
	cairo_device_to_user (m_cr, &x1, &y1);
	cairo_device_to_user (m_cr, &w1, &h1);
	cairo_rectangle(m_cr, x1, y1, w1, h1);
	fillPath();
	strokePath();
}

void ScPainter::drawText(const QRectF& area, const QString& text, bool filled, int align)
{
	cairo_text_extents_t extents;
	cairo_font_extents_t extentsF;
	double x;
	if (align == 0)
		x = area.center().x();
	else
		x = area.x();
	double y = area.y();
	double ww = 0;
	double hh = 0;
	double r, g, b;

	assert(!m_font.isNone());

#if CAIRO_HAS_FC_FONT
	FcPattern *pattern = FcPatternBuild(nullptr,
										FC_FILE, FcTypeString, QFile::encodeName(m_font.fontFilePath()).data(),
										FC_INDEX, FcTypeInteger, m_font.faceIndex(),
										nullptr);
	cairo_font_face_t *cairo_face = cairo_ft_font_face_create_for_pattern(pattern);
	FcPatternDestroy(pattern);

	cairo_set_font_face (m_cr, cairo_face);
	cairo_set_font_size(m_cr, m_fontSize);

	cairo_font_options_t* options = cairo_font_options_create();
	cairo_font_options_set_hint_style(options, CAIRO_HINT_STYLE_SLIGHT);
	cairo_set_font_options(m_cr, options);
	cairo_font_options_destroy(options);

	cairo_font_extents (m_cr, &extentsF);
	QStringList textList = text.split("\n");
	for (int a = 0; a < textList.count(); ++a)
	{
		cairo_text_extents (m_cr, textList[a].toUtf8(), &extents);
		if (align == 0)
			x = qMin(area.center().x() - (extents.width / 2.0 + extents.x_bearing), x);
		ww = qMax(ww, extents.width);
	}
	hh = extentsF.height * textList.count();
	if ((align == 0) || (align == 1))
		y = area.center().y() - ((extentsF.height * textList.count()) / 2.0);
	if (filled)
	{
		m_fill.getRgbF(&r, &g, &b);
		cairo_set_source_rgba(m_cr, r, g, b, m_fill_trans);
		cairo_new_path(m_cr);
		cairo_rectangle(m_cr, x, y, ww, hh);
		cairo_fill(m_cr);
	}
	cairo_new_path(m_cr);
	y += extentsF.ascent;
	cairo_move_to (m_cr, x, y);
	m_stroke.getRgbF(&r, &g, &b);
	cairo_set_source_rgba(m_cr, r, g, b, m_stroke_trans);

	QVector<cairo_glyph_t> cairoGlyphs;

	for (int a = 0; a < textList.count(); ++a)
	{
		CharStyle style(m_font, m_fontSize * 10);
		StoryText story;
		story.insertChars(textList[a]);
		story.setCharStyle(0, textList[a].count(), style);

		TextShaper textShaper(story, 0);
		const QList<GlyphCluster> glyphRuns = textShaper.shape(0, story.length()).glyphs();

		double tmpx = x;
		for (const GlyphCluster &run : glyphRuns)
		{
			for (const GlyphLayout &gl : run.glyphs())
			{
				cairo_glyph_t glyph;
				glyph.index = gl.glyph;
				glyph.x = tmpx + gl.xoffset;
				glyph.y = y - gl.yoffset;
				tmpx += gl.xadvance;
				cairoGlyphs.append(glyph);
			}
		}
		y += extentsF.height;
	}

	cairo_show_glyphs(m_cr, cairoGlyphs.data(), cairoGlyphs.count());
#else
	cairo_select_font_face(m_cr, m_font.family().toLatin1(), m_font.isItalic() ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL, m_font.isBold() ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(m_cr, m_fontSize);
	cairo_font_extents (m_cr, &extentsF);
	QStringList textList = text.split("\n");
	for (int a = 0; a < textList.count(); ++a)
	{
		cairo_text_extents (m_cr, textList[a].toUtf8(), &extents);
		if (align == 0)
			x = qMin(area.center().x() - (extents.width / 2.0 + extents.x_bearing), x);
		ww = qMax(ww, extents.width);
	}
	hh = extentsF.height * textList.count();
	if ((align == 0) || (align == 1))
		y = area.center().y() - ((extentsF.height * textList.count()) / 2.0);
	if (filled)
	{
		m_fill.getRgbF(&r, &g, &b);
		cairo_set_source_rgba(m_cr, r, g, b, m_fill_trans);
		cairo_new_path(m_cr);
		cairo_rectangle(m_cr, x, y, ww, hh);
		cairo_fill(m_cr);
	}
	cairo_new_path(m_cr);
	y += extentsF.ascent;
	cairo_move_to (m_cr, x, y);
	m_stroke.getRgbF(&r, &g, &b);
	cairo_set_source_rgba(m_cr, r, g, b, m_stroke_trans);
	for (int a = 0; a < textList.count(); ++a)
	{
		cairo_show_text (m_cr, textList[a].toUtf8());
		y += extentsF.height;
		cairo_move_to (m_cr, x, y);
	}
#endif
}

void ScPainter::drawShadeCircle(const QRectF &re, const QColor& color, bool sunken, int lineWidth)
{
	setStrokeMode(ScPainter::Solid);
	double bezierCircle = 0.55228475;
	double dx = re.width();
	double dy = re.height();
	double cx = dx / 2.0;
	double cy = dy / 2.0;
	double rb = 0.5 * (dx < dy ? dx : dy);
	double r = rb - 0.25 * lineWidth;
	QColor shade;
	shade.setRgbF(color.redF() * 0.5, color.greenF() * 0.5, color.blueF() * 0.5);
	QColor light;
	light.setRgbF(color.redF() * 0.5 + 0.5, color.greenF() * 0.5 + 0.5, color.blueF() * 0.5 + 0.5);
	cairo_save(m_cr);
	translate(0, dy);
	scale(1, -1);
	setPen(color, lineWidth * 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	newPath();
	moveTo(cx + r, cy);
	cairo_curve_to(m_cr, cx + r, cy + bezierCircle * r, cx + bezierCircle * r, cy + r, cx, cy + r);
	cairo_curve_to(m_cr, cx - bezierCircle * r, cy + r, cx - r, cy + bezierCircle * r, cx - r, cy);
	cairo_curve_to(m_cr, cx - r, cy - bezierCircle * r, cx - bezierCircle * r, cy - r, cx, cy - r);
	cairo_curve_to(m_cr, cx + bezierCircle * r, cy - r, cx + r, cy - bezierCircle * r, cx + r, cy);
	strokePath();
	r = rb - 0.73 * lineWidth;
	double r2 = r / 1.414213562;
	if (sunken)
		setPen(shade, lineWidth * 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	else
		setPen(light, lineWidth * 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	newPath();
	moveTo(cx + r2, cy + r2);
	cairo_curve_to(m_cr, cx + (1 - bezierCircle) * r2, cy + (1 + bezierCircle) * r2, cx - (1 - bezierCircle) * r2, cy + (1 + bezierCircle) * r2, cx - r2, cy + r2);
	cairo_curve_to(m_cr, cx - (1 + bezierCircle) * r2, cy + (1 - bezierCircle) * r2, cx - (1 + bezierCircle) * r2, cy - (1 - bezierCircle) * r2, cx - r2, cy - r2);
	strokePath();
	if (sunken)
		setPen(light, lineWidth * 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	else
		setPen(shade, lineWidth * 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	newPath();
	moveTo(cx - r2, cy - r2);
	cairo_curve_to(m_cr, cx - (1 - bezierCircle) * r2, cy - (1 + bezierCircle) * r2, cx + (1 - bezierCircle) * r2, cy - (1 + bezierCircle) * r2, cx + r2, cy - r2);
	cairo_curve_to(m_cr, cx + (1 + bezierCircle) * r2, cy - (1 - bezierCircle) * r2, cx + (1 + bezierCircle) * r2, cy + (1 - bezierCircle) * r2, cx + r2, cy + r2);
	strokePath();
	cairo_restore(m_cr);
}

void ScPainter::drawShadePanel(const QRectF &r, const QColor& color, bool sunken, int lineWidth)
{
	QColor shade;
	QColor light;
	if (sunken)
	{
		shade.setRgbF(color.redF() * 0.5, color.greenF() * 0.5, color.blueF() * 0.5);
		light.setRgbF(color.redF() * 0.5 + 0.5, color.greenF() * 0.5 + 0.5, color.blueF() * 0.5 + 0.5);
	}
	else
	{
		light.setRgbF(color.redF() * 0.5, color.greenF() * 0.5, color.blueF() * 0.5);
		shade.setRgbF(color.redF() * 0.5 + 0.5, color.greenF() * 0.5 + 0.5, color.blueF() * 0.5 + 0.5);
	}
	double x1, y1, y2, x3;
	x1 = r.x();
	x3 = r.x() + r.width();
	y1 = r.y() + r.height();
	y2 = r.y();
	setFillMode(ScPainter::Solid);
	newPath();
	moveTo(x1, y1);
	lineTo(x1, y2);
	lineTo(x3, y2);
	lineTo(x3 - lineWidth, lineWidth);
	lineTo(lineWidth, lineWidth);
	lineTo(lineWidth, y1 - lineWidth);
	closePath();
	setBrush(shade);
	fillPath();
	newPath();
	moveTo(x1, y1);
	lineTo(x3, y1);
	lineTo(x3, y2);
	lineTo(x3 - lineWidth, lineWidth);
	lineTo(x3 - lineWidth, y1 - lineWidth);
	lineTo(lineWidth, y1 - lineWidth);
	closePath();
	setBrush(light);
	fillPath();
}

void ScPainter::drawUnderlinedRect(const QRectF &r, const QColor& color, int lineWidth)
{
	setPen(color, lineWidth, Qt::DashLine, Qt::FlatCap, Qt::MiterJoin);
	setStrokeMode(ScPainter::Solid);
	drawLine(r.bottomLeft(), r.topLeft());
	drawLine(r.topLeft(), r.topRight());
	drawLine(r.bottomRight(), r.topRight());

	setPen(color, lineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	drawLine(r.bottomLeft(), r.bottomRight());
}

void ScPainter::colorizeAlpha(const QColor& color)
{
	cairo_surface_t *data = cairo_get_group_target(m_cr);
	cairo_surface_flush(data);
	int w   = cairo_image_surface_get_width(data);
	int h   = cairo_image_surface_get_height(data);
	int stride = cairo_image_surface_get_stride(data);
	unsigned char *d = cairo_image_surface_get_data(data);
	int cr = color.red();
	int cg = color.green();
	int cb = color.blue();
	for (int y = 0; y < h; ++y)
	{
		QRgb *dst = (QRgb*)d;
		for (int x = 0; x < w; ++x)
		{
			if (qAlpha(*dst) > 0)
				*dst = qRgba(cr, cg, cb, qAlpha(*dst));
			dst++;
		}
		d += stride;
	}
	cairo_surface_mark_dirty(data);
}

void ScPainter::colorize(const QColor& color)
{
	cairo_surface_t *data = cairo_get_group_target(m_cr);
	cairo_surface_flush(data);
	int w   = cairo_image_surface_get_width(data);
	int h   = cairo_image_surface_get_height(data);
	int stride = cairo_image_surface_get_stride(data);
	unsigned char *d = cairo_image_surface_get_data(data);
	int cr = color.red();
	int cg = color.green();
	int cb = color.blue();
	int hu, sa, v;
	int cc2, cm2, cy2, k2;
	QColor tmpR;
	for (int y = 0; y < h; ++y)
	{
		QRgb *dst = (QRgb*)d;
		for (int x = 0; x < w; ++x)
		{
			if (qAlpha(*dst) > 0)
			{
				k2 = 255 - qMin(qRound(0.3 * qRed(*dst) + 0.59 * qGreen(*dst) + 0.11 * qBlue(*dst)), 255);
				tmpR.setRgb(cr, cg, cb);
				tmpR.getHsv(&hu, &sa, &v);
				tmpR.setHsv(hu, sa * k2 / 255, 255 - ((255 - v) * k2 / 255));
				tmpR.getRgb(&cc2, &cm2, &cy2);
				*dst = qRgba(cc2, cm2, cy2, qAlpha(*dst));
			}
			dst++;
		}
		d += stride;
	}
	cairo_surface_mark_dirty(data);
}

void ScPainter::blurAlpha(int radius)
{
	if (radius < 1)
		return;
	cairo_surface_t *data = cairo_get_group_target(m_cr);
	QRgb *pix = (QRgb*)cairo_image_surface_get_data(data);
	int w   = cairo_image_surface_get_width(data);
	int h   = cairo_image_surface_get_height(data);
	int wm  = w - 1;
	int hm  = h - 1;
	int wh  = w * h;
	int div = radius+radius+1;
	int *a = new int[wh];
	int asum, x, y, i, yp, yi, yw;
	QRgb p;
	int *vmin = new int[qMax(w, h)];
	int divsum = (div + 1)>>1;
	divsum *= divsum;
	int *dv = new int[256 * (size_t) divsum];
	for (i = 0; i < 256 * divsum; ++i)
	{
		dv[i] = (i / divsum);
	}
	yw = yi = 0;
	int **stack = new int*[div];
	for (int i = 0; i < div; ++i)
	{
		stack[i] = new int[1];
	}
	int stackpointer;
	int stackstart;
	int *sir;
	int rbs;
	int r1 = radius+1;
	int aoutsum;
	int ainsum;
	for (y = 0; y < h; ++y)
	{
		ainsum = aoutsum = asum = 0;
		for (i = -radius; i <= radius; ++i)
		{
			p = pix[yi + qMin(wm, qMax(i, 0))];
			sir = stack[i + radius];
			sir[0] = qAlpha(p);
			rbs = r1 - abs(i);
			asum += sir[0] * rbs;
			if (i > 0)
				ainsum += sir[0];
			else
				aoutsum += sir[0];
		}
		stackpointer = radius;
		for (x = 0; x < w; ++x)
		{
			a[yi] = dv[asum];
			asum -= aoutsum;
			stackstart = stackpointer - radius + div;
			sir = stack[stackstart % div];
			aoutsum -= sir[0];
			if (y == 0)
				vmin[x] = qMin(x + radius + 1, wm);
			p = pix[yw + vmin[x]];
			sir[0] = qAlpha(p);
			ainsum += sir[0];
			asum += ainsum;
			stackpointer = (stackpointer + 1) % div;
			sir = stack[stackpointer % div];
			aoutsum += sir[0];
			ainsum -= sir[0];
			++yi;
		}
		yw += w;
	}
	for (x = 0; x < w; ++x)
	{
		ainsum = aoutsum = asum = 0;
		yp = -radius * w;
		for (i = -radius; i <= radius; ++i)
		{
			yi = qMax(0, yp) + x;
			sir = stack[i + radius];
			sir[0] = a[yi];
			rbs = r1 - abs(i);
			asum += a[yi] * rbs;
			if (i > 0)
				ainsum += sir[0];
			else
				aoutsum += sir[0];
			if (i < hm)
			{
				yp += w;
			}
		}
		yi = x;
		stackpointer = radius;
		for (y = 0; y < h; ++y)
		{
			pix[yi] = qRgba(qRed(pix[yi]), qGreen(pix[yi]), qBlue(pix[yi]), dv[asum]);
			asum -= aoutsum;
			stackstart = stackpointer - radius + div;
			sir = stack[stackstart%div];
			aoutsum -= sir[0];
			if (x == 0)
			{
				vmin[y] = qMin(y + r1, hm) * w;
			}
			p = x + vmin[y];
			sir[0] = a[p];
			ainsum += sir[0];
			asum += ainsum;
			stackpointer = (stackpointer+1)%div;
			sir = stack[stackpointer];
			aoutsum += sir[0];
			ainsum -= sir[0];
			yi += w;
		}
	}
	delete [] a;
	delete [] vmin;
	delete [] dv;
	for (int i = 0; i < div; ++i)
	{
		delete [] stack[i];
	}
	delete [] stack;
	cairo_surface_mark_dirty(data);
}

void ScPainter::blur(int radius)
{
	if (radius < 1)
		return;
	cairo_surface_t *data = cairo_get_group_target(m_cr);
	QRgb *pix = (QRgb*) cairo_image_surface_get_data(data);
	int w   = cairo_image_surface_get_width(data);
	int h   = cairo_image_surface_get_height(data);
	int wm  = w - 1;
	int hm  = h - 1;
	int wh  = w * h;
	int div = radius + radius + 1;
	int *r = new int[wh];
	int *g = new int[wh];
	int *b = new int[wh];
	int *a = new int[wh];
	int rsum, gsum, bsum, asum, x, y, i, yp, yi, yw;
	QRgb p;
	int *vmin = new int[qMax(w, h)];
	int divsum = (div+1)>>1;
	divsum *= divsum;
	int *dv = new int[256 * (size_t) divsum];
	for (i = 0; i < 256 * divsum; ++i)
	{
		dv[i] = (i / divsum);
	}
	yw = yi = 0;
	int **stack = new int*[div];
	for (int i = 0; i < div; ++i)
	{
		stack[i] = new int[4];
	}
	int stackpointer;
	int stackstart;
	int *sir;
	int rbs;
	int r1 = radius+1;
	int routsum, goutsum, boutsum, aoutsum;
	int rinsum, ginsum, binsum, ainsum;
	for (y = 0; y < h; ++y)
	{
		rinsum = ginsum = binsum = ainsum = routsum = goutsum = boutsum = aoutsum = rsum = gsum = bsum = asum = 0;
		for (i = -radius; i <= radius; ++i)
		{
			p = pix[yi + qMin(wm, qMax(i, 0))];
			sir = stack[i + radius];
			sir[0] = qRed(p);
			sir[1] = qGreen(p);
			sir[2] = qBlue(p);
			sir[3] = qAlpha(p);
			rbs = r1 - abs(i);
			rsum += sir[0] * rbs;
			gsum += sir[1] * rbs;
			bsum += sir[2] * rbs;
			asum += sir[3] * rbs;
			if (i > 0)
			{
				rinsum += sir[0];
				ginsum += sir[1];
				binsum += sir[2];
				ainsum += sir[3];
			}
			else
			{
				routsum += sir[0];
				goutsum += sir[1];
				boutsum += sir[2];
				aoutsum += sir[3];
			}
		}
		stackpointer = radius;
		for (x = 0; x < w; ++x)
		{
			r[yi] = dv[rsum];
			g[yi] = dv[gsum];
			b[yi] = dv[bsum];
			a[yi] = dv[asum];
			rsum -= routsum;
			gsum -= goutsum;
			bsum -= boutsum;
			asum -= aoutsum;
			stackstart = stackpointer - radius + div;
			sir = stack[stackstart % div];
			routsum -= sir[0];
			goutsum -= sir[1];
			boutsum -= sir[2];
			aoutsum -= sir[3];
			if (y == 0)
			{
				vmin[x] = qMin(x + radius + 1,wm);
			}
			p = pix[yw + vmin[x]];
			sir[0] = qRed(p);
			sir[1] = qGreen(p);
			sir[2] = qBlue(p);
			sir[3] = qAlpha(p);
			rinsum += sir[0];
			ginsum += sir[1];
			binsum += sir[2];
			ainsum += sir[3];
			rsum += rinsum;
			gsum += ginsum;
			bsum += binsum;
			asum += ainsum;
			stackpointer = (stackpointer + 1) % div;
			sir = stack[stackpointer % div];
			routsum += sir[0];
			goutsum += sir[1];
			boutsum += sir[2];
			aoutsum += sir[3];
			rinsum -= sir[0];
			ginsum -= sir[1];
			binsum -= sir[2];
			ainsum -= sir[3];
			++yi;
		}
		yw += w;
	}
	for (x = 0; x < w; ++x)
	{
		rinsum = ginsum = binsum = ainsum = routsum = goutsum = boutsum = aoutsum = rsum = gsum = bsum = asum = 0;
		yp =- radius * w;
		for (i = -radius; i <= radius; ++i)
		{
			yi = qMax(0, yp) + x;
			sir = stack[i + radius];
			sir[0] = r[yi];
			sir[1] = g[yi];
			sir[2] = b[yi];
			sir[3] = a[yi];
			rbs = r1-abs(i);
			rsum += r[yi] * rbs;
			gsum += g[yi] * rbs;
			bsum += b[yi] * rbs;
			asum += a[yi] * rbs;
			if (i > 0)
			{
				rinsum += sir[0];
				ginsum += sir[1];
				binsum += sir[2];
				ainsum += sir[3];
			}
			else
			{
				routsum += sir[0];
				goutsum += sir[1];
				boutsum += sir[2];
				aoutsum += sir[3];
			}
			if (i < hm)
			{
				yp += w;
			}
		}
		yi = x;
		stackpointer = radius;
		for (y = 0; y < h; ++y)
		{
			pix[yi] = qRgba(dv[rsum], dv[gsum], dv[bsum], dv[asum]);
			rsum -= routsum;
			gsum -= goutsum;
			bsum -= boutsum;
			asum -= aoutsum;
			stackstart = stackpointer - radius + div;
			sir = stack[stackstart % div];
			routsum -= sir[0];
			goutsum -= sir[1];
			boutsum -= sir[2];
			aoutsum -= sir[3];
			if (x == 0)
			{
				vmin[y] = qMin(y + r1, hm) * w;
			}
			p = x + vmin[y];
			sir[0] = r[p];
			sir[1] = g[p];
			sir[2] = b[p];
			sir[3] = a[p];
			rinsum += sir[0];
			ginsum += sir[1];
			binsum += sir[2];
			ainsum += sir[3];
			rsum += rinsum;
			gsum += ginsum;
			bsum += binsum;
			asum += ainsum;
			stackpointer = (stackpointer + 1) % div;
			sir = stack[stackpointer];
			routsum += sir[0];
			goutsum += sir[1];
			boutsum += sir[2];
			aoutsum += sir[3];
			rinsum -= sir[0];
			ginsum -= sir[1];
			binsum -= sir[2];
			ainsum -= sir[3];
			yi += w;
		}
	}
	delete [] r;
	delete [] g;
	delete [] b;
	delete [] a;
	delete [] vmin;
	delete [] dv;
	for (int i = 0; i < div; ++i)
	{
		delete [] stack[i];
	}
	delete [] stack;
	cairo_surface_mark_dirty(data);
}
