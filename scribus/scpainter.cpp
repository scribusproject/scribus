/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scpainter.h"
#include "util_color.h"
#include "util.h"
#include "util_math.h"

#ifdef HAVE_CAIRO
	#include <cairo.h>
	#if defined(_WIN32)
	#include <cairo-win32.h>
	#elif defined(Q_WS_X11)
	#include <cairo-xlib.h>
	#elif defined(Q_WS_MAC)
//	#include <cairo-quartz.h>
	#endif
	#include <cairo-svg.h>
#endif

#include <math.h>

#ifdef HAVE_CAIRO
/// image painter
ScPainter::ScPainter( QImage *target, unsigned int w, unsigned int h, double transparency, int blendmode )
{
	m_width = w;
	m_height= h;
	m_stroke = QColor(0,0,0);
	strokeMode = 0;
	maskMode = 0;
	m_fill = QColor(0,0,0);
	fill_trans = 1.0;
	stroke_trans = 1.0;
	m_fillRule = true;
	fillMode = 1;
	LineWidth = 1.0;
	m_offset = 0;
	m_layerTransparency = transparency;
	m_blendMode = blendmode;
	m_blendModeFill = 0;
	m_blendModeStroke = 0;
	m_array.clear();
	mf_underline = false;
	mf_strikeout = false;
	mf_shadow = false;
	mf_outlined = false;
	PLineEnd = Qt::FlatCap;
	PLineJoin = Qt::MiterJoin;
	fill_gradient = VGradient(VGradient::linear);
	stroke_gradient = VGradient(VGradient::linear);
	m_zoomFactor = 1;
	layeredMode = true;
	imageMode = true;
	svgMode = false;
	m_image = target;
	m_matrix = QTransform();
	cairo_surface_t *img = cairo_image_surface_create_for_data(m_image->bits(), CAIRO_FORMAT_ARGB32, w, h, w*4);
	m_cr = cairo_create(img);
	cairo_save( m_cr );
	cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	cairo_set_tolerance( m_cr, 0.5 );
}

/// SVG file painter
ScPainter::ScPainter( QString target, unsigned int w, unsigned int h, double transparency, int blendmode )
{
	m_width = w;
	m_height= h;
	m_stroke = QColor(0,0,0);
	strokeMode = 0;
	maskMode = 0;
	m_fill = QColor(0,0,0);
	fill_trans = 1.0;
	stroke_trans = 1.0;
	m_fillRule = true;
	fillMode = 1;
	LineWidth = 1.0;
	m_offset = 0;
	m_layerTransparency = transparency;
	m_blendMode = blendmode;
	m_blendModeFill = 0;
	m_blendModeStroke = 0;
	m_array.clear();
	mf_underline = false;
	mf_strikeout = false;
	mf_shadow = false;
	mf_outlined = false;
	PLineEnd = Qt::FlatCap;
	PLineJoin = Qt::MiterJoin;
	fill_gradient = VGradient(VGradient::linear);
	stroke_gradient = VGradient(VGradient::linear);
	m_zoomFactor = 1;
	layeredMode = true;
	imageMode = false;
	svgMode = true;
	m_matrix = QTransform();
	cairo_surface_t *img = cairo_svg_surface_create(target.toLocal8Bit().constData(), w, h);
	m_cr = cairo_create(img);
	cairo_save( m_cr );
	cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	cairo_set_tolerance( m_cr, 0.5 );
}
// HAVE_CAIRO
#else
/// image painter
ScPainter::ScPainter( QImage *target, unsigned int w, unsigned int h, double transparency, int blendmode )
{
	m_path = QPainterPath();
	m_width = w;
	m_height= h;
	m_stroke = QColor(0,0,0);
	strokeMode = 0;
	maskMode = 0;
	m_fill = QColor(0,0,0);
	fill_trans = 1.0;
	stroke_trans = 1.0;
	m_fillRule = true;
	fillMode = 1;
	LineWidth = 1.0;
	m_offset = 0;
	m_layerTransparency = transparency;
	m_blendMode = blendmode;
	m_blendModeFill = 0;
	m_blendModeStroke = 0;
	m_array.clear();
	mf_underline = false;
	mf_strikeout = false;
	mf_shadow = false;
	mf_outlined = false;
	PLineEnd = Qt::FlatCap;
	PLineJoin = Qt::MiterJoin;
	PLineStyle = Qt::SolidLine;
	fill_gradient = VGradient(VGradient::linear);
	stroke_gradient = VGradient(VGradient::linear);
	m_zoomFactor = 1;
	layeredMode = true;
	imageMode = true;
	svgMode = false;
	m_image = target;
	m_matrix = QTransform();
	painter.begin(m_image);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
}
#endif

ScPainter::~ScPainter()
{
#ifdef HAVE_CAIRO
	cairo_surface_destroy(cairo_get_target(m_cr));
	cairo_destroy( m_cr );
#else
	painter.end();
#endif
}

void ScPainter::beginLayer(double transparency, int blendmode, FPointArray *clipArray)
{
	layerProp la;
	la.blendmode = m_blendMode;
	la.tranparency = m_layerTransparency;
	m_layerTransparency = transparency;
	m_blendMode = blendmode;
	la.pushed = false;
	la.groupClip.resize(0);
	if (clipArray != NULL)
		la.groupClip = *clipArray;
#ifdef HAVE_CAIRO
	la.data = cairo_get_group_target(m_cr);
	cairo_push_group(m_cr);
/*	if (clipArray != NULL)
	{
		setupPolygon(clipArray);
		setClipPath();
	} */
#else
	la.data = new QImage(m_image->copy());
	m_image->fill(qRgba(255, 255, 255, 0));
#endif
	la.pushed = true;
	Layers.push(la);
}

#ifdef HAVE_CAIRO
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
void ScPainter::endLayer()
{
	layerProp la;
	if (Layers.count() == 0)
		return;
	la = Layers.pop();
	if (la.pushed)
	{
		cairo_pop_group_to_source (m_cr);
		if (la.groupClip.size() != 0)
		{
			setupPolygon(&la.groupClip);
			setClipPath();
		}
		cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
		if (maskMode > 0)
		{
			cairo_pattern_t *patM = getMaskPattern();
			setRasterOp(m_blendMode);
			cairo_mask(m_cr, patM);
			if ((maskMode == 2) || (maskMode == 4))
				cairo_surface_destroy(imageMask);
			cairo_pattern_destroy(patM);
		}
		else
		{
			setRasterOp(m_blendMode);
			cairo_paint_with_alpha (m_cr, m_layerTransparency);
		}
		cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	}
	m_layerTransparency = la.tranparency;
	m_blendMode = la.blendmode;
}
#else
void ScPainter::endLayer()
{
	layerProp la;
	if (Layers.count() == 0)
		return;
	la = Layers.top();
	if (la.pushed)
	{
		if ((m_blendMode != 0) && (Layers.count() != 0))
		{
			cairo_surface_t *tmp = cairo_get_group_target(m_cr);
			cairo_surface_t *tmpB = Layers.top().data;
			if ((tmp != NULL) && (tmpB != NULL))
			{
				if ((cairo_surface_get_type(tmp) == CAIRO_SURFACE_TYPE_IMAGE) && (cairo_surface_get_type(tmpB) == CAIRO_SURFACE_TYPE_IMAGE))
				{
					cairo_surface_flush(tmp);
					int stride = cairo_image_surface_get_stride(tmp);
					unsigned char *s = cairo_image_surface_get_data(tmp);
					unsigned char *d = cairo_image_surface_get_data(tmpB);
					int h = cairo_image_surface_get_height(tmp);
					int w = cairo_image_surface_get_width(tmp);
					uint oldDst = 1;
					uint oldSrc = 2;
					uint newResult = 0;
					bool first = true;
					QRgb *src;
					QRgb *dst;
					uchar src_a, src_r, src_g, src_b, dst_a, dst_r, dst_g, dst_b, new_r, new_g, new_b;
					for( int yi=0; yi < h; ++yi )
					{
						dst = (QRgb*)d;
						src = (QRgb*)s;
						for( int xi=0; xi < w; ++xi )
						{
							src_r = qRed(*src);
							src_g = qGreen(*src);
							src_b = qBlue(*src);
							src_a = qAlpha(*src);
							dst_r = qRed(*dst);
							dst_g = qGreen(*dst);
							dst_b = qBlue(*dst);
							dst_a = qAlpha(*dst);
							if ((src_a > 0) && (dst_a > 0))
							{
								if (((*dst) != oldDst) || ((*src) != oldSrc) || (first))
								{
									if (m_blendMode == 1)
									{
										src_r = dst_r  < src_r ? dst_r  : src_r;
										src_g = dst_g < src_g ? dst_g : src_g;
										src_b = dst_b < src_b ? dst_b : src_b;
									}
									else if (m_blendMode == 2)
									{
										src_r = dst_r  < src_r ? src_r : dst_r;
										src_g = dst_g < src_g ? src_g : dst_g;
										src_b = dst_b < src_b ? src_b : dst_b;
									}
									else if (m_blendMode == 3)
									{
										src_r = INT_MULT(src_r, dst_r);
										src_g = INT_MULT(src_g, dst_g);
										src_b = INT_MULT(src_b, dst_b);
									}
									else if (m_blendMode == 4)
									{
										src_r = 255 - ((255-src_r) * (255-dst_r) / 255);
										src_g = 255 - ((255-src_g) * (255-dst_g) / 255);
										src_b = 255 - ((255-src_b) * (255-dst_b) / 255);
									}
									else if (m_blendMode == 5)
									{
										src_r = dst_r < 128 ? src_r * dst_r / 128 : 255 - ((255-src_r) * (255-dst_r) / 128);
										src_g = dst_g < 128 ? src_g * dst_g / 128 : 255 - ((255-src_g) * (255-dst_g) / 128);
										src_b = dst_b < 128 ? src_b * dst_b / 128 : 255 - ((255-src_b) * (255-dst_b) / 128);
									}
									else if (m_blendMode == 6)
									{
										src_r = src_r < 128 ? src_r * dst_r / 128 : 255 - ((255-src_r) * (255-dst_r) / 128);
										src_g = src_g < 128 ? src_g * dst_g / 128 : 255 - ((255-src_g) * (255-dst_g) / 128);
										src_b = src_b < 128 ? src_b * dst_b / 128 : 255 - ((255-src_b) * (255-dst_b) / 128);
									}
									else if (m_blendMode == 7)
									{
										float s_r = (255 - src_r) / 255.0;
										float s_g = (255 - src_g) / 255.0;
										float s_b = (255 - src_b) / 255.0;
										float d_r = (255 - dst_r) / 255.0;
										float d_g = (255 - dst_g) / 255.0;
										float d_b = (255 - dst_b) / 255.0;
										float dzr = d_r > 0.25 ? sqrt(d_r) : ((16 * d_r - 12) * d_r + 4) * d_r;
										float dzg = d_g > 0.25 ? sqrt(d_g) : ((16 * d_g - 12) * d_g + 4) * d_g;
										float dzb = d_b > 0.25 ? sqrt(d_b) : ((16 * d_b - 12) * d_b + 4) * d_b;
										s_r = s_r <= 0.5 ? d_r - (1 - 2 * s_r) * d_r * (1 - d_r) : d_r + (2 * s_r - 1) * (dzr  - d_r);
										s_g = s_g <= 0.5 ? d_g - (1 - 2 * s_g) * d_g * (1 - d_g) : d_g + (2 * s_g - 1) * (dzg  - d_g);
										s_b = s_b <= 0.5 ? d_b - (1 - 2 * s_b) * d_b * (1 - d_b) : d_b + (2 * s_b - 1) * (dzb  - d_b);
										src_r = 255 - qRound(s_r * 255);
										src_g = 255 - qRound(s_g * 255);
										src_b = 255 - qRound(s_b * 255);
									}
									else if (m_blendMode == 8)
									{
										src_r = dst_r > src_r ? dst_r - src_r : src_r - dst_r;
										src_g = dst_g > src_g ? dst_g - src_g : src_g - dst_g;
										src_b = dst_b > src_b ? dst_b - src_b : src_b - dst_b;
									}
									else if (m_blendMode == 9)
									{
										src_r = dst_r + src_r - src_r * dst_r / 128;
										src_g = dst_g + src_g - src_g * dst_g / 128;
										src_b = dst_b + src_b - src_b * dst_b / 128;
									}
									else if (m_blendMode == 10)
									{
										src_r = src_r == 255 ? qMin(255, dst_r * 256) : qMin(255, ((dst_r * 256) / (255-src_r)));
										src_g = src_g == 255 ? qMin(255, dst_g * 256) : qMin(255, ((dst_g * 256) / (255-src_g)));
										src_b = src_b == 255 ? qMin(255, dst_b * 256) : qMin(255, ((dst_b * 256) / (255-src_b)));
									}
									else if (m_blendMode == 11)
									{
										src_r = qMax<uchar>(1, src_r);
										src_g = qMax<uchar>(1, src_g);
										src_b = qMax<uchar>(1, src_b);
										src_r = static_cast<int>(255 - (((255-dst_r) * 256) / src_r)) < 0 ? 0 : 255 - (((255-dst_r) * 256) / src_r);
										src_g = static_cast<int>(255 - (((255-dst_g) * 256) / src_g)) < 0 ? 0 : 255 - (((255-dst_g) * 256) / src_g);
										src_b = static_cast<int>(255 - (((255-dst_b) * 256) / src_b)) < 0 ? 0 : 255 - (((255-dst_b) * 256) / src_b);
									}
									else if (m_blendMode == 12)
									{
										new_r = dst_r;
										new_g = dst_g;
										new_b = dst_b;
										RGBTOHLS(src_r, src_g, src_b);
										RGBTOHLS(new_r, new_g, new_b);
										new_r = src_r;
										HLSTORGB(new_r, new_g, new_b);
										src_r = new_r;
										src_g = new_g;
										src_b = new_b;
									}
									else if (m_blendMode == 13)
									{
										new_r = dst_r;
										new_g = dst_g;
										new_b = dst_b;
										RGBTOHLS(src_r, src_g, src_b);
										RGBTOHLS(new_r, new_g, new_b);
										new_b = src_b;
										HLSTORGB(new_r, new_g, new_b);
										src_r = new_r;
										src_g = new_g;
										src_b = new_b;
									}
									else if (m_blendMode == 14)
									{
										new_r = src_r;
										new_g = src_g;
										new_b = src_b;
										setLum(new_r, new_g, new_b, Lum(dst_r, dst_g, dst_b));
										src_r = new_r;
										src_g = new_g;
										src_b = new_b;
									}
									else if (m_blendMode == 15)
									{
										new_r = dst_r;
										new_g = dst_g;
										new_b = dst_b;
										setLum(new_r, new_g, new_b, Lum(src_r, src_g, src_b));
										src_r = new_r;
										src_g = new_g;
										src_b = new_b;
									}
									newResult = qRgba(src_r, src_g, src_b, src_a);
									oldDst = (*dst);
									oldSrc = (*src);
									first = false;
								}
								(*src) = newResult;
							}
							src++;
							dst++;
						}
						s += stride;
						d += stride;
					}
					cairo_surface_mark_dirty(tmp);
				}
			}
		}
	}
	la = Layers.pop();
	if (la.pushed)
	{
		cairo_pop_group_to_source (m_cr);
		if (la.groupClip.size() != 0)
		{
			setupPolygon(&la.groupClip);
			setClipPath();
		}
		if (m_blendMode == 0)
		{
			cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
			if (maskMode == 1)
			{
				cairo_pattern_t *patM = getMaskPattern();
				cairo_mask(m_cr, patM);
				if ((maskMode == 2) || (maskMode == 4))
					cairo_surface_destroy(imageMask);
				cairo_pattern_destroy(patM);
			}
			else
				cairo_paint_with_alpha (m_cr, m_layerTransparency);
		}
		else
		{
			if (maskMode == 1)
			{
				cairo_pattern_t *patM = getMaskPattern();
				cairo_mask(m_cr, patM);
				if ((maskMode == 2) || (maskMode == 4))
					cairo_surface_destroy(imageMask);
				cairo_pattern_destroy(patM);
			}
			else
			{
				cairo_set_operator(m_cr, CAIRO_OPERATOR_DEST_OUT);
				cairo_paint_with_alpha (m_cr, m_layerTransparency);
				cairo_set_operator(m_cr, CAIRO_OPERATOR_ADD);
				cairo_paint_with_alpha (m_cr, m_layerTransparency);
			}
		}
		cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	}
	m_layerTransparency = la.tranparency;
	m_blendMode = la.blendmode;
}
#endif
#else
void ScPainter::endLayer()
{
	layerProp la;
	if (Layers.count() == 0)
		return;
	la = Layers.top();
	if (la.pushed)
	{
		if ((m_blendMode > 11) && (Layers.count() != 0))
		{
			int h = m_image->height();
			int w = m_image->width();
			uint oldDst = 1;
			uint oldSrc = 2;
			uint newResult = 0;
			bool first = true;
			QRgb *src;
			QRgb *dst;
			uchar src_a, src_r, src_g, src_b, dst_a, dst_r, dst_g, dst_b, new_r, new_g, new_b;
			for( int yi = 0; yi < h; ++yi )
			{
				dst = (QRgb*)la.data->scanLine(yi);
				src = (QRgb*)m_image->scanLine(yi);
				for( int xi=0; xi < w; ++xi )
				{
					src_r = qRed(*src);
					src_g = qGreen(*src);
					src_b = qBlue(*src);
					src_a = qAlpha(*src);
					dst_r = qRed(*dst);
					dst_g = qGreen(*dst);
					dst_b = qBlue(*dst);
					dst_a = qAlpha(*dst);
					if ((src_a > 0) && (dst_a > 0))
					{
						if (((*dst) != oldDst) || ((*src) != oldSrc) || (first))
						{
							if (m_blendMode == 12)
							{
								new_r = dst_r;
								new_g = dst_g;
								new_b = dst_b;
								RGBTOHLS(src_r, src_g, src_b);
								RGBTOHLS(new_r, new_g, new_b);
								new_r = src_r;
								HLSTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
							else if (m_blendMode == 13)
							{
								new_r = dst_r;
								new_g = dst_g;
								new_b = dst_b;
								RGBTOHLS(src_r, src_g, src_b);
								RGBTOHLS(new_r, new_g, new_b);
								new_b = src_b;
								HLSTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
							else if (m_blendMode == 14)
							{
								new_r = src_r;
								new_g = src_g;
								new_b = src_b;
								setLum(new_r, new_g, new_b, Lum(dst_r, dst_g, dst_b));
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
							else if (m_blendMode == 15)
							{
								new_r = dst_r;
								new_g = dst_g;
								new_b = dst_b;
								setLum(new_r, new_g, new_b, Lum(src_r, src_g, src_b));
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
							newResult = qRgba(src_r, src_g, src_b, src_a);
							oldDst = (*dst);
							oldSrc = (*src);
							first = false;
						}
						(*src) = newResult;
					}
					src++;
					dst++;
				}
			}
		}
	}
	la = Layers.pop();
	if (la.pushed)
	{
		QRgb *src;
		QRgb *dst;
		QRgb tmp;
		int h = m_image->height();
		int w = m_image->width();
		for( int yi = 0; yi < h; ++yi )
		{
			src = (QRgb*)la.data->scanLine(yi);
			dst = (QRgb*)m_image->scanLine(yi);
			for( int xi=0; xi < w; ++xi )
			{
				tmp = *dst;
				*dst = *src;
				*src = tmp;
				dst++;
				src++;
			}
		}
		painter.save();
		if (la.groupClip.size() != 0)
		{
			setupPolygon(&la.groupClip);
			setClipPath();
		}
		painter.resetTransform();
		if ((m_blendMode > 0) && (m_blendMode < 12))
		{
			if (m_blendMode == 1)
				painter.setCompositionMode(QPainter::CompositionMode_Darken);
			else if (m_blendMode == 2)
				painter.setCompositionMode(QPainter::CompositionMode_Lighten);
			else if (m_blendMode == 3)
				painter.setCompositionMode(QPainter::CompositionMode_Multiply);
			else if (m_blendMode == 4)
				painter.setCompositionMode(QPainter::CompositionMode_Screen);
			else if (m_blendMode == 5)
				painter.setCompositionMode(QPainter::CompositionMode_Overlay);
			else if (m_blendMode == 6)
				painter.setCompositionMode(QPainter::CompositionMode_HardLight);
			else if (m_blendMode == 7)
				painter.setCompositionMode(QPainter::CompositionMode_SoftLight);
			else if (m_blendMode == 8)
				painter.setCompositionMode(QPainter::CompositionMode_Difference);
			else if (m_blendMode == 9)
				painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
			else if (m_blendMode == 10)
				painter.setCompositionMode(QPainter::CompositionMode_ColorDodge);
			else if (m_blendMode == 11)
				painter.setCompositionMode(QPainter::CompositionMode_ColorBurn);
		}
		if (maskMode > 0)
		{
			QBrush brushm;
			QTransform qmatrix;
			if (maskMode == 1)
			{
				QGradient patm;
				double x1 = mask_gradient.origin().x();
				double y1 = mask_gradient.origin().y();
				double x2 = mask_gradient.vector().x();
				double y2 = mask_gradient.vector().y();
				double fx = mask_gradient.focalPoint().x();
				double fy = mask_gradient.focalPoint().y();
				if (mask_gradient.type() == VGradient::linear)
					patm = QLinearGradient(x1, y1,  x2, y2);
				else
					patm = QRadialGradient(x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)), fx, fy);
				QList<VColorStop*> colorStops = mask_gradient.colorStops();
				QColor qStopColor;
				for( int offset = 0 ; offset < colorStops.count() ; offset++ )
				{
					qStopColor = colorStops[ offset ]->color;
					int h, s, v, sneu, vneu;
					int shad = colorStops[offset]->shade;
					qStopColor.getHsv(&h, &s, &v);
					sneu = s * shad / 100;
					vneu = 255 - ((255 - v) * shad / 100);
					qStopColor.setHsv(h, sneu, vneu);
					qStopColor.setAlphaF(colorStops[offset]->opacity);
					patm.setColorAt(colorStops[ offset ]->rampPoint, qStopColor);
				}
				if (mask_gradient.type() == VGradient::radial)
				{
					double rotEnd = xy2Deg(x2 - x1, y2 - y1);
					qmatrix.translate(x1, y1);
					qmatrix.rotate(rotEnd);
					qmatrix.shear(mask_gradientSkew, 0);
					qmatrix.translate(0, y1 * (1.0 - mask_gradientScale));
					qmatrix.translate(-x1, -y1);
					qmatrix.scale(1, mask_gradientScale);
				}
				else
				{
					qmatrix.translate(x1, y1);
					qmatrix.shear(-mask_gradientSkew, 0);
					qmatrix.translate(-x1, -y1);
				}
				brushm = QBrush(patm);
				brushm.setTransform(qmatrix);
			}
			else if (maskMode == 2)
			{
				qmatrix.translate(mask_patternOffsetX, mask_patternOffsetY);
				qmatrix.rotate(mask_patternRotation);
				qmatrix.shear(-mask_patternSkewX, mask_patternSkewY);
				qmatrix.scale(mask_patternScaleX, mask_patternScaleY);
				qmatrix.scale(m_maskPattern->width / static_cast<double>(m_maskPattern->getPattern()->width()), m_maskPattern->height / static_cast<double>(m_maskPattern->getPattern()->height()));
				if (mask_patternMirrorX)
					qmatrix.scale(-1, 1);
				if (mask_patternMirrorY)
					qmatrix.scale(1, -1);
				brushm = QBrush(*m_maskPattern->getPattern());
				brushm.setTransform(qmatrix);
			}
			QImage tmpImgM = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
			QPainter painterM;
			painterM.begin(&tmpImgM);
			painterM.setWorldTransform(painter.worldTransform());
			painterM.setRenderHint(QPainter::Antialiasing, true);
			painterM.setRenderHint(QPainter::SmoothPixmapTransform, true);
			painterM.fillPath(painter.clipPath(), brushm);
			painterM.end();
			QImage tmpImgO = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
			QPainter painterO;
			painterO.begin(&tmpImgO);
			painterO.setWorldTransform(painter.worldTransform());
			painterO.setRenderHint(QPainter::Antialiasing, true);
			painterO.setRenderHint(QPainter::SmoothPixmapTransform, true);
			painterO.drawImage(0, 0, *la.data);
			painterO.end();
			int h = tmpImgO.height();
			int w = tmpImgO.width();
			QRgb *s, *t;
			for( int yi=0; yi < h; ++yi )
			{
				s = (QRgb*)(tmpImgM.scanLine( yi ));
				t = (QRgb*)(tmpImgO.scanLine( yi ));
				for( int xi=0; xi < w; ++xi )
				{
					*t = qRgba(qRed(*t), qGreen(*t), qBlue(*t), qAlpha(*s));
					s++;
					t++;
				}
			}
			painter.save();
			painter.resetTransform();
			painter.drawImage(0, 0, tmpImgO);
			painter.restore();
		}
		else
		{
			painter.setOpacity(m_layerTransparency);
			painter.drawImage(0, 0, *la.data);
		}
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		painter.restore();
		delete la.data;
	}
	m_layerTransparency = la.tranparency;
	m_blendMode = la.blendmode;
}
#endif

void ScPainter::begin()
{
}

void ScPainter::end()
{
#ifdef HAVE_CAIRO
	if (svgMode)
		cairo_show_page (m_cr);
	if (layeredMode)
	{
		cairo_surface_flush(cairo_get_target(m_cr));
		cairo_restore( m_cr );
		return;
	}
#endif
}

void ScPainter::clear()
{
#ifdef HAVE_CAIRO
	if (imageMode)
		m_image->fill( qRgba(255, 255, 255, 255) );
#else
	m_image->fill(qRgba(255, 255, 255, 255));
#endif
}

void ScPainter::clear( const QColor &c )
{
	QRgb cs = c.rgb();
#ifdef HAVE_CAIRO
	if (imageMode)
		m_image->fill( qRgba(qRed(cs), qGreen(cs), qBlue(cs), qAlpha(cs)) );
#else
	m_image->fill(qRgba(qRed(cs), qGreen(cs), qBlue(cs), qAlpha(cs)));
#endif
}

const QTransform ScPainter::worldMatrix()
{
#ifdef HAVE_CAIRO
	cairo_matrix_t matrix;
	cairo_get_matrix(m_cr, &matrix);
	QTransform mat = QTransform( matrix.xx, matrix.yx, matrix.xy, matrix.yy, matrix.x0, matrix.y0 );
	return mat;
#else
	return painter.worldTransform();
#endif
}

void ScPainter::setWorldMatrix( const QTransform &mat )
{
#ifdef HAVE_CAIRO
	cairo_matrix_t matrix;
	cairo_matrix_init(&matrix, mat.m11(), mat.m12(), mat.m21(), mat.m22(), mat.dx(), mat.dy());
	cairo_set_matrix(m_cr, &matrix);
#else
	painter.setWorldTransform(mat);
#endif
}

void ScPainter::setAntialiasing(bool enable)
{
#ifdef HAVE_CAIRO
	if (enable)
		cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_DEFAULT);
	else
		cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_NONE);
#else
	painter.setRenderHint(QPainter::Antialiasing, enable);
#endif
}

void ScPainter::setZoomFactor( double zoomFactor )
{
	m_zoomFactor = zoomFactor;
#ifdef HAVE_CAIRO
	cairo_scale (m_cr, m_zoomFactor, m_zoomFactor);
#else
	painter.scale(m_zoomFactor, m_zoomFactor);
#endif
}

void ScPainter::translate( double x, double y )
{
#ifdef HAVE_CAIRO
	cairo_translate (m_cr, x, y);
#else
	painter.translate(x, y);
#endif
}

void ScPainter::rotate( double r )
{
#ifdef HAVE_CAIRO
	cairo_rotate (m_cr, r * 3.1415927 / 180.0);
#else
	painter.rotate(r);
#endif
}

void ScPainter::scale( double x, double y )
{
#ifdef HAVE_CAIRO
	cairo_scale (m_cr, x, y);
#else
	painter.scale(x, y);
#endif
}

void ScPainter::moveTo( const double &x, const double &y )
{
#ifdef HAVE_CAIRO
	cairo_move_to( m_cr, x, y);
#else
	m_path.moveTo(x, y);
#endif
}

void
ScPainter::lineTo( const double &x, const double &y )
{
#ifdef HAVE_CAIRO
	cairo_line_to( m_cr, x, y);
#else
	m_path.lineTo(x, y);
#endif
}

void ScPainter::curveTo( FPoint p1, FPoint p2, FPoint p3 )
{
#ifdef HAVE_CAIRO
	cairo_curve_to(m_cr, p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
#else
	m_path.cubicTo(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
#endif
}

void ScPainter::newPath()
{
#ifdef HAVE_CAIRO
	cairo_new_path( m_cr );
#else
	m_path = QPainterPath();
#endif
}

void ScPainter::closePath()
{
#ifdef HAVE_CAIRO
	cairo_close_path( m_cr );
#else
	m_path.closeSubpath();
#endif
}

void ScPainter::setFillRule( bool fillRule )
{
	m_fillRule = fillRule;
}

void ScPainter::setFillMode( int fill )
{
	fillMode = fill;
}

void ScPainter::setStrokeMode( int stroke )
{
	strokeMode = stroke;
}

void ScPainter::setGradient(VGradient::VGradientType mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew)
{
	fill_gradient.setType(mode);
	fill_gradient.setOrigin(orig);
	fill_gradient.setVector(vec);
	fill_gradient.setFocalPoint(foc);
	stroke_gradient.setType(mode);
	stroke_gradient.setOrigin(orig);
	stroke_gradient.setVector(vec);
	stroke_gradient.setFocalPoint(foc);
	gradientScale = scale;
	if (skew == 90)
		gradientSkew = 1;
	else if (skew == 180)
		gradientSkew = 0;
	else if (skew == 270)
		gradientSkew = -1;
	else if (skew == 360)
		gradientSkew = 0;
	else
		gradientSkew = tan(M_PI / 180.0 * skew);
}

void ScPainter::setMaskMode(int mask)
{
	maskMode = mask;
}

void ScPainter::setGradientMask(VGradient::VGradientType mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew)
{
	mask_gradient.setType(mode);
	mask_gradient.setOrigin(orig);
	mask_gradient.setVector(vec);
	mask_gradient.setFocalPoint(foc);
	mask_gradientScale = scale;
	if (skew == 90)
		mask_gradientSkew = 1;
	else if (skew == 180)
		mask_gradientSkew = 0;
	else if (skew == 270)
		mask_gradientSkew = -1;
	else if (skew == 360)
		mask_gradientSkew = 0;
	else
		mask_gradientSkew = tan(M_PI / 180.0 * skew);
}

void ScPainter::setPatternMask(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	m_maskPattern = pattern;
	mask_patternScaleX = scaleX / 100.0;
	mask_patternScaleY = scaleY / 100.0;
	mask_patternOffsetX = offsetX;
	mask_patternOffsetY = offsetY;
	mask_patternRotation = rotation;
	mask_patternSkewX = skewX;
	mask_patternSkewY = skewY;
	mask_patternMirrorX = mirrorX;
	mask_patternMirrorY = mirrorY;
}

void ScPainter::fillTextPath()
{
	drawVPath( 0 );
}

void ScPainter::strokeTextPath()
{
//	if( LineWidth == 0 )
//		return;
	drawVPath( 1 );
}

void ScPainter::fillPath()
{
	if( fillMode != 0)
		drawVPath( 0 );
}

void ScPainter::strokePath()
{
//	if( LineWidth == 0 )
//		return;
	drawVPath( 1 );
}

QColor ScPainter::pen()
{
	return m_stroke;
}

QColor ScPainter::brush()
{
	return m_fill;
}

void ScPainter::setPen( const QColor &c )
{
	m_stroke = c;
}

void ScPainter::setPen( const QColor &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo )
{
	m_stroke = c;
	LineWidth = w;
	PLineEnd = ca;
	PLineJoin = jo;
#ifdef HAVE_CAIRO
	m_offset = 0;
	getDashArray(st, w, m_array);
#else
	m_array.clear();
	PLineStyle = st;
#endif
}

void ScPainter::setLineWidth( double w )
{
	LineWidth = w;
}

void ScPainter::setPenOpacity( double op )
{
	stroke_trans = op;
}


void ScPainter::setDash(const QVector<double>& array, double ofs)
{
	m_array = array;
	m_offset = ofs;
}

void ScPainter::setBrush( const QColor &c )
{
	m_fill = c;
}

void ScPainter::setBrushOpacity( double op )
{
	fill_trans = op;
}

void ScPainter::setOpacity( double op )
{
	fill_trans = op;
	stroke_trans = op;
}

void ScPainter::setFont( const QFont &f)
{
	m_font = f;
}

QFont ScPainter::font()
{
	return m_font;
}

void ScPainter::save()
{
#ifdef HAVE_CAIRO
	cairo_save( m_cr );
#else
	painter.save();
#endif
}

void ScPainter::restore()
{
#ifdef HAVE_CAIRO
	cairo_restore( m_cr );
#else
	painter.restore();
#endif
}

void ScPainter::setRasterOp(int blendMode)
{
#ifdef HAVE_CAIRO
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
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
#endif
#endif
}

void ScPainter::setBlendModeFill( int blendMode )
{
	m_blendModeFill = blendMode;
}

void ScPainter::setBlendModeStroke( int blendMode )
{
	m_blendModeStroke = blendMode;
}

void ScPainter::setPattern(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	m_pattern = pattern;
	patternScaleX = scaleX / 100.0;
	patternScaleY = scaleY / 100.0;
	patternOffsetX = offsetX;
	patternOffsetY = offsetY;
	patternRotation = rotation;
	patternSkewX = skewX;
	patternSkewY = skewY;
	patternMirrorX = mirrorX;
	patternMirrorY = mirrorY;
}

#ifdef HAVE_CAIRO

cairo_pattern_t * ScPainter::getMaskPattern()
{
	cairo_save( m_cr );
	cairo_pattern_t *pat;
	if ((maskMode == 1) || (maskMode == 3))
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
		for( int offset = 0 ; offset < colorStops.count() ; offset++ )
		{
			qStopColor = colorStops[ offset ]->color;
			int h, s, v, sneu, vneu;
			int shad = colorStops[offset]->shade;
			qStopColor.getHsv(&h, &s, &v);
			sneu = s * shad / 100;
			vneu = 255 - ((255 - v) * shad / 100);
			qStopColor.setHsv(h, sneu, vneu);
			double a = colorStops[offset]->opacity;
			double r, g, b;
			qStopColor.getRgbF(&r, &g, &b);
			if (maskMode == 3)
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
			qmatrix.shear(mask_gradientSkew, 0);
			qmatrix.translate(0, y1 * (1.0 - mask_gradientScale));
			qmatrix.translate(-x1, -y1);
			qmatrix.scale(1, mask_gradientScale);
		}
		else
		{
			qmatrix.translate(x1, y1);
			qmatrix.shear(-mask_gradientSkew, 0);
			qmatrix.translate(-x1, -y1);
		}
		cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
		cairo_matrix_invert(&matrix);
		cairo_pattern_set_matrix (pat, &matrix);
	}
	else
	{
		if (maskMode == 4)
		{
			imageQ = m_maskPattern->pattern.copy();
			int h = imageQ.height();
			int w = imageQ.width();
			int k;
			QRgb *s;
			QRgb r;
			for( int yi=0; yi < h; ++yi )
			{
				s = (QRgb*)(imageQ.scanLine( yi ));
				for( int xi=0; xi < w; ++xi )
				{
					r = *s;
					if (qAlpha(r) == 0)
						k = 0; // 255;
					else
						k = qMin(qRound(0.3 * qRed(r) + 0.59 * qGreen(r) + 0.11 * qBlue(r)), 255);
					*s = qRgba(qRed(r), qGreen(r), qBlue(r), /* 255 - */ k);
					s++;
				}
			}
			imageMask = cairo_image_surface_create_for_data ((uchar*)imageQ.bits(), CAIRO_FORMAT_ARGB32, w, h, w*4);
		}
		else
			imageMask = cairo_image_surface_create_for_data ((uchar*)m_maskPattern->getPattern()->bits(), CAIRO_FORMAT_ARGB32, m_maskPattern->getPattern()->width(), m_maskPattern->getPattern()->height(), m_maskPattern->getPattern()->width()*4);
		pat = cairo_pattern_create_for_surface(imageMask);
		cairo_pattern_set_extend(pat, CAIRO_EXTEND_REPEAT);
		cairo_pattern_set_filter(pat, CAIRO_FILTER_BEST);
		cairo_matrix_t matrix;
		QTransform qmatrix;
		qmatrix.translate(mask_patternOffsetX, mask_patternOffsetY);
		qmatrix.rotate(mask_patternRotation);
		qmatrix.shear(-mask_patternSkewX, mask_patternSkewY);
		qmatrix.scale(mask_patternScaleX, mask_patternScaleY);
		qmatrix.scale(m_maskPattern->width / static_cast<double>(m_maskPattern->getPattern()->width()), m_maskPattern->height / static_cast<double>(m_maskPattern->getPattern()->height()));
		if (mask_patternMirrorX)
			qmatrix.scale(-1, 1);
		if (mask_patternMirrorY)
			qmatrix.scale(1, -1);
		cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
		cairo_matrix_invert(&matrix);
		cairo_pattern_set_matrix (pat, &matrix);
	}
	cairo_restore( m_cr );
	return pat;
}

void ScPainter::drawVPath( int mode )
{
	cairo_save( m_cr );
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	if (mode == 0)
	{
		if( m_fillRule )
			cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_EVEN_ODD);
		else
			cairo_set_fill_rule (m_cr, CAIRO_FILL_RULE_WINDING);
		if (fillMode == 1)
		{
			double r, g, b;
			m_fill.getRgbF(&r, &g, &b);
			if (maskMode > 0)
			{
				cairo_pattern_t *pat = getMaskPattern();
				cairo_set_source_rgb( m_cr, r, g, b );
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
				setRasterOp(m_blendModeFill);
#endif
				cairo_clip_preserve(m_cr);
				cairo_mask(m_cr, pat);
				if ((maskMode == 2) || (maskMode == 4))
					cairo_surface_destroy(imageMask);
				cairo_pattern_destroy(pat);
			}
			else
			{
				cairo_set_source_rgba( m_cr, r, g, b, fill_trans );
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
				setRasterOp(m_blendModeFill);
#endif
				cairo_fill_preserve(m_cr);
			}
		}
		else if (fillMode == 2)
		{
			cairo_pattern_t *pat;
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
			cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
			QList<VColorStop*> colorStops = fill_gradient.colorStops();
			QColor qStopColor;
			for( int offset = 0 ; offset < colorStops.count() ; offset++ )
			{
				qStopColor = colorStops[ offset ]->color;
				int h, s, v, sneu, vneu;
				int shad = colorStops[offset]->shade;
				qStopColor.getHsv(&h, &s, &v);
				sneu = s * shad / 100;
				vneu = 255 - ((255 - v) * shad / 100);
				qStopColor.setHsv(h, sneu, vneu);
				double a = colorStops[offset]->opacity;
				double r, g, b;
				qStopColor.getRgbF(&r, &g, &b);
				cairo_pattern_add_color_stop_rgba (pat, colorStops[ offset ]->rampPoint, r, g, b, a);
			}
			cairo_matrix_t matrix;
			QTransform qmatrix;
			if (fill_gradient.type() == VGradient::radial)
			{
				double rotEnd = xy2Deg(x2 - x1, y2 - y1);
				qmatrix.translate(x1, y1);
				qmatrix.rotate(rotEnd);
				qmatrix.shear(gradientSkew, 0);
				qmatrix.translate(0, y1 * (1.0 - gradientScale));
				qmatrix.translate(-x1, -y1);
				qmatrix.scale(1, gradientScale);
			}
			else
			{
				qmatrix.translate(x1, y1);
				qmatrix.shear(-gradientSkew, 0);
				qmatrix.translate(-x1, -y1);
			}
			cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
			cairo_matrix_invert(&matrix);
			cairo_pattern_set_matrix (pat, &matrix);
			cairo_set_source (m_cr, pat);
			cairo_clip_preserve (m_cr);
			if (maskMode > 0)
			{
				cairo_pattern_t *patM = getMaskPattern();
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
				setRasterOp(m_blendModeFill);
#endif
				cairo_mask(m_cr, patM);
				if ((maskMode == 2) || (maskMode == 4))
					cairo_surface_destroy(imageMask);
				cairo_pattern_destroy (patM);
			}
			else
			{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
				setRasterOp(m_blendModeFill);
#endif
				cairo_paint_with_alpha (m_cr, fill_trans);
			}
			cairo_pattern_destroy (pat);
		}
		else if (fillMode == 3)
		{
			cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_NONE);
			cairo_surface_t *image2 = cairo_image_surface_create_for_data ((uchar*)m_pattern->getPattern()->bits(), CAIRO_FORMAT_ARGB32, m_pattern->getPattern()->width(), m_pattern->getPattern()->height(), m_pattern->getPattern()->width()*4);
			cairo_pattern_t *m_pat = cairo_pattern_create_for_surface(image2);
			cairo_pattern_set_extend(m_pat, CAIRO_EXTEND_REPEAT);
			cairo_pattern_set_filter(m_pat, CAIRO_FILTER_BEST);
			cairo_matrix_t matrix;
			QTransform qmatrix;
			qmatrix.translate(patternOffsetX, patternOffsetY);
			qmatrix.rotate(patternRotation);
			qmatrix.shear(-patternSkewX, patternSkewY);
			qmatrix.scale(patternScaleX, patternScaleY);
			qmatrix.scale(m_pattern->width / static_cast<double>(m_pattern->getPattern()->width()), m_pattern->height / static_cast<double>(m_pattern->getPattern()->height()));
			if (patternMirrorX)
				qmatrix.scale(-1, 1);
			if (patternMirrorY)
				qmatrix.scale(1, -1);
			cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
			cairo_matrix_invert(&matrix);
			cairo_pattern_set_matrix (m_pat, &matrix);
			cairo_set_source (m_cr, m_pat);
			cairo_clip_preserve (m_cr);
			if (maskMode > 0)
			{
				cairo_pattern_t *patM = getMaskPattern();
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
				setRasterOp(m_blendModeFill);
#endif
				cairo_mask(m_cr, patM);
				if ((maskMode == 2) || (maskMode == 4))
					cairo_surface_destroy(imageMask);
				cairo_pattern_destroy (patM);
			}
			else
			{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
				setRasterOp(m_blendModeFill);
#endif
				cairo_paint_with_alpha (m_cr, fill_trans);
			}
			cairo_pattern_destroy (m_pat);
			cairo_surface_destroy (image2);
			cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_DEFAULT);
		}
	}
	else
	{
		if( LineWidth == 0 )
			cairo_set_line_width( m_cr, 1.0 / m_zoomFactor );
		else
			cairo_set_line_width( m_cr, LineWidth );
		if( m_array.count() > 0 )
			cairo_set_dash( m_cr, m_array.data(), m_array.count(), m_offset);
		else
			cairo_set_dash( m_cr, NULL, 0, 0 );
		cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
		if( PLineEnd == Qt::RoundCap )
			cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_ROUND);
		else if( PLineEnd == Qt::SquareCap )
			cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_SQUARE);
		else if( PLineEnd == Qt::FlatCap )
			cairo_set_line_cap (m_cr, CAIRO_LINE_CAP_BUTT);
		if( PLineJoin == Qt::RoundJoin )
			cairo_set_line_join( m_cr, CAIRO_LINE_JOIN_ROUND );
		else if( PLineJoin == Qt::BevelJoin )
			cairo_set_line_join( m_cr, CAIRO_LINE_JOIN_BEVEL );
		else if( PLineJoin == Qt::MiterJoin )
			cairo_set_line_join( m_cr, CAIRO_LINE_JOIN_MITER );
		if (strokeMode == 3)
		{
			cairo_push_group(m_cr);
			cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_NONE);
			cairo_surface_t *image2 = cairo_image_surface_create_for_data ((uchar*)m_pattern->getPattern()->bits(), CAIRO_FORMAT_ARGB32, m_pattern->getPattern()->width(), m_pattern->getPattern()->height(), m_pattern->getPattern()->width()*4);
			cairo_pattern_t *m_pat = cairo_pattern_create_for_surface(image2);
			cairo_pattern_set_extend(m_pat, CAIRO_EXTEND_REPEAT);
			cairo_pattern_set_filter(m_pat, CAIRO_FILTER_BEST);
			cairo_matrix_t matrix;
			QTransform qmatrix;
			qmatrix.translate(-LineWidth / 2.0, -LineWidth / 2.0);
			qmatrix.translate(patternOffsetX, patternOffsetY);
			qmatrix.rotate(patternRotation);
			qmatrix.shear(-patternSkewX, patternSkewY);
			qmatrix.scale(patternScaleX, patternScaleY);
			qmatrix.scale(m_pattern->width / static_cast<double>(m_pattern->getPattern()->width()), m_pattern->height / static_cast<double>(m_pattern->getPattern()->height()));
			if (patternMirrorX)
				qmatrix.scale(-1, 1);
			if (patternMirrorY)
				qmatrix.scale(1, -1);
			cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
			cairo_matrix_invert(&matrix);
			cairo_pattern_set_matrix (m_pat, &matrix);
			cairo_set_source (m_cr, m_pat);
			cairo_stroke_preserve( m_cr );
			cairo_pattern_destroy (m_pat);
			cairo_surface_destroy (image2);
			cairo_set_antialias(m_cr, CAIRO_ANTIALIAS_DEFAULT);
			cairo_pop_group_to_source (m_cr);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
			setRasterOp(m_blendModeStroke);
#endif
			cairo_paint_with_alpha (m_cr, stroke_trans);
		}
		else if (strokeMode == 2)
		{
			cairo_push_group(m_cr);
			cairo_pattern_t *pat;
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
			QList<VColorStop*> colorStops = stroke_gradient.colorStops();
			QColor qStopColor;
			for( int offset = 0 ; offset < colorStops.count() ; offset++ )
			{
				qStopColor = colorStops[ offset ]->color;
				int h, s, v, sneu, vneu;
				int shad = colorStops[offset]->shade;
				qStopColor.getHsv(&h, &s, &v);
				sneu = s * shad / 100;
				vneu = 255 - ((255 - v) * shad / 100);
				qStopColor.setHsv(h, sneu, vneu);
				double a = colorStops[offset]->opacity;
				double r, g, b;
				qStopColor.getRgbF(&r, &g, &b);
				cairo_pattern_add_color_stop_rgba (pat, colorStops[ offset ]->rampPoint, r, g, b, a);
			}
			cairo_matrix_t matrix;
			QTransform qmatrix;
			if (stroke_gradient.type() == VGradient::radial)
			{
				double rotEnd = xy2Deg(x2 - x1, y2 - y1);
				qmatrix.translate(x1, y1);
				qmatrix.rotate(rotEnd);
				qmatrix.shear(gradientSkew, 0);
				qmatrix.translate(0, y1 * (1.0 - gradientScale));
				qmatrix.translate(-x1, -y1);
				qmatrix.scale(1, gradientScale);
			}
			else
			{
				qmatrix.translate(x1, y1);
				qmatrix.shear(-gradientSkew, 0);
				qmatrix.translate(-x1, -y1);
			}
			cairo_matrix_init(&matrix, qmatrix.m11(), qmatrix.m12(), qmatrix.m21(), qmatrix.m22(), qmatrix.dx(), qmatrix.dy());
			cairo_matrix_invert(&matrix);
			cairo_pattern_set_matrix (pat, &matrix);
			cairo_set_source (m_cr, pat);
			cairo_stroke_preserve( m_cr );
			cairo_pattern_destroy (pat);
			cairo_pop_group_to_source (m_cr);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
			setRasterOp(m_blendModeStroke);
#endif
			cairo_paint_with_alpha (m_cr, stroke_trans);
		}
		else
		{
			double r, g, b;
			m_stroke.getRgbF(&r, &g, &b);
			cairo_set_source_rgba( m_cr, r, g, b, stroke_trans );
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
			setRasterOp(m_blendModeStroke);
#endif
			cairo_stroke_preserve( m_cr );
		}
	}
	cairo_restore( m_cr );
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
}
#else
	// not HAVE_CAIRO
void ScPainter::drawVPath(int mode)
{
	painter.save();
	if (mode == 0)
	{
		if( m_fillRule )
			m_path.setFillRule(Qt::OddEvenFill);
		else
			m_path.setFillRule(Qt::WindingFill);
		if (fillMode == 1)
		{
			if (maskMode > 0)
			{
				QTransform qmatrix;
				QBrush brushm;
				if (maskMode == 1)
				{
					QGradient pat;
					double x1 = mask_gradient.origin().x();
					double y1 = mask_gradient.origin().y();
					double x2 = mask_gradient.vector().x();
					double y2 = mask_gradient.vector().y();
					double fx = mask_gradient.focalPoint().x();
					double fy = mask_gradient.focalPoint().y();
					if (mask_gradient.type() == VGradient::linear)
						pat = QLinearGradient(x1, y1,  x2, y2);
					else
						pat = QRadialGradient(x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)), fx, fy);
					QList<VColorStop*> colorStops = mask_gradient.colorStops();
					QColor qStopColor;
					for( int offset = 0 ; offset < colorStops.count() ; offset++ )
					{
						qStopColor = colorStops[ offset ]->color;
						int h, s, v, sneu, vneu;
						int shad = colorStops[offset]->shade;
						qStopColor.getHsv(&h, &s, &v);
						sneu = s * shad / 100;
						vneu = 255 - ((255 - v) * shad / 100);
						qStopColor.setHsv(h, sneu, vneu);
						qStopColor.setAlphaF(colorStops[offset]->opacity);
						pat.setColorAt(colorStops[ offset ]->rampPoint, qStopColor);
					}
					QTransform qmatrix;
					if (mask_gradient.type() == VGradient::radial)
					{
						double rotEnd = xy2Deg(x2 - x1, y2 - y1);
						qmatrix.translate(x1, y1);
						qmatrix.rotate(rotEnd);
						qmatrix.shear(mask_gradientSkew, 0);
						qmatrix.translate(0, y1 * (1.0 - mask_gradientScale));
						qmatrix.translate(-x1, -y1);
						qmatrix.scale(1, mask_gradientScale);
					}
					else
					{
						qmatrix.translate(x1, y1);
						qmatrix.shear(-mask_gradientSkew, 0);
						qmatrix.translate(-x1, -y1);
					}
					brushm = QBrush(pat);
					brushm.setTransform(qmatrix);
				}
				else if (maskMode == 2)
				{
					QTransform qmatrix;
					qmatrix.translate(mask_patternOffsetX, mask_patternOffsetY);
					qmatrix.rotate(mask_patternRotation);
					qmatrix.shear(-mask_patternSkewX, mask_patternSkewY);
					qmatrix.scale(mask_patternScaleX, mask_patternScaleY);
					qmatrix.scale(m_maskPattern->width / static_cast<double>(m_maskPattern->getPattern()->width()), m_maskPattern->height / static_cast<double>(m_maskPattern->getPattern()->height()));
					if (mask_patternMirrorX)
						qmatrix.scale(-1, 1);
					if (mask_patternMirrorY)
						qmatrix.scale(1, -1);
					brushm = QBrush(*m_maskPattern->getPattern());
					brushm.setTransform(qmatrix);
				}
				QImage tmpImgM = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
				QPainter painterM;
				painterM.begin(&tmpImgM);
				painterM.setWorldTransform(painter.worldTransform());
				painterM.setRenderHint(QPainter::Antialiasing, true);
				painterM.setRenderHint(QPainter::SmoothPixmapTransform, true);
				painterM.fillPath(m_path, brushm);
				painterM.end();
				QImage tmpImgO = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
				QPainter painterO;
				painterO.begin(&tmpImgO);
				painterO.setWorldTransform(painter.worldTransform());
				painterO.setRenderHint(QPainter::Antialiasing, true);
				painterO.setRenderHint(QPainter::SmoothPixmapTransform, true);
				QColor paint = m_fill;
				painterO.fillPath(m_path, paint);
				painterO.end();
				int h = tmpImgO.height();
				int w = tmpImgO.width();
				QRgb *s, *t;
				for( int yi=0; yi < h; ++yi )
				{
					s = (QRgb*)(tmpImgM.scanLine( yi ));
					t = (QRgb*)(tmpImgO.scanLine( yi ));
					for( int xi=0; xi < w; ++xi )
					{
						*t = qRgba(qRed(*t), qGreen(*t), qBlue(*t), qAlpha(*s));
						s++;
						t++;
					}
				}
				painter.save();
				painter.resetTransform();
				painter.drawImage(0, 0, tmpImgO);
				painter.restore();
			}
			else
			{
				QColor paint = m_fill;
				paint.setAlphaF(fill_trans);
				painter.fillPath(m_path, paint);
			}
		}
		else if (fillMode == 2)
		{
			QGradient pat;
			double x1 = fill_gradient.origin().x();
			double y1 = fill_gradient.origin().y();
			double x2 = fill_gradient.vector().x();
			double y2 = fill_gradient.vector().y();
			double fx = fill_gradient.focalPoint().x();
			double fy = fill_gradient.focalPoint().y();
			if (fill_gradient.type() == VGradient::linear)
				pat = QLinearGradient(x1, y1,  x2, y2);
			else
				pat = QRadialGradient(x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)), fx, fy);
			QList<VColorStop*> colorStops = fill_gradient.colorStops();
			QColor qStopColor;
			for( int offset = 0 ; offset < colorStops.count() ; offset++ )
			{
				qStopColor = colorStops[ offset ]->color;
				int h, s, v, sneu, vneu;
				int shad = colorStops[offset]->shade;
				qStopColor.getHsv(&h, &s, &v);
				sneu = s * shad / 100;
				vneu = 255 - ((255 - v) * shad / 100);
				qStopColor.setHsv(h, sneu, vneu);
				qStopColor.setAlphaF(colorStops[offset]->opacity);
				pat.setColorAt(colorStops[ offset ]->rampPoint, qStopColor);
			}
			QTransform qmatrix;
			if (fill_gradient.type() == VGradient::radial)
			{
				double rotEnd = xy2Deg(x2 - x1, y2 - y1);
				qmatrix.translate(x1, y1);
				qmatrix.rotate(rotEnd);
				qmatrix.shear(gradientSkew, 0);
				qmatrix.translate(0, y1 * (1.0 - gradientScale));
				qmatrix.translate(-x1, -y1);
				qmatrix.scale(1, gradientScale);
			}
			else
			{
				qmatrix.translate(x1, y1);
				qmatrix.shear(-gradientSkew, 0);
				qmatrix.translate(-x1, -y1);
			}
			QBrush brush = QBrush(pat);
			brush.setTransform(qmatrix);
			if (maskMode > 0)
			{
				QTransform qmatrix;
				QBrush brushm;
				if (maskMode == 1)
				{
					QGradient patm;
					double x1 = mask_gradient.origin().x();
					double y1 = mask_gradient.origin().y();
					double x2 = mask_gradient.vector().x();
					double y2 = mask_gradient.vector().y();
					double fx = mask_gradient.focalPoint().x();
					double fy = mask_gradient.focalPoint().y();
					if (mask_gradient.type() == VGradient::linear)
						patm = QLinearGradient(x1, y1,  x2, y2);
					else
						patm = QRadialGradient(x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)), fx, fy);
					QList<VColorStop*> colorStops = mask_gradient.colorStops();
					QColor qStopColor;
					for( int offset = 0 ; offset < colorStops.count() ; offset++ )
					{
						qStopColor = colorStops[ offset ]->color;
						int h, s, v, sneu, vneu;
						int shad = colorStops[offset]->shade;
						qStopColor.getHsv(&h, &s, &v);
						sneu = s * shad / 100;
						vneu = 255 - ((255 - v) * shad / 100);
						qStopColor.setHsv(h, sneu, vneu);
						qStopColor.setAlphaF(colorStops[offset]->opacity);
						patm.setColorAt(colorStops[ offset ]->rampPoint, qStopColor);
					}
					QTransform qmatrix;
					if (mask_gradient.type() == VGradient::radial)
					{
						double rotEnd = xy2Deg(x2 - x1, y2 - y1);
						qmatrix.translate(x1, y1);
						qmatrix.rotate(rotEnd);
						qmatrix.shear(mask_gradientSkew, 0);
						qmatrix.translate(0, y1 * (1.0 - mask_gradientScale));
						qmatrix.translate(-x1, -y1);
						qmatrix.scale(1, mask_gradientScale);
					}
					else
					{
						qmatrix.translate(x1, y1);
						qmatrix.shear(-mask_gradientSkew, 0);
						qmatrix.translate(-x1, -y1);
					}
					brushm = QBrush(patm);
					brushm.setTransform(qmatrix);
				}
				else if (maskMode == 2)
				{
					QTransform qmatrix;
					qmatrix.translate(mask_patternOffsetX, mask_patternOffsetY);
					qmatrix.rotate(mask_patternRotation);
					qmatrix.shear(-mask_patternSkewX, mask_patternSkewY);
					qmatrix.scale(mask_patternScaleX, mask_patternScaleY);
					qmatrix.scale(m_maskPattern->width / static_cast<double>(m_maskPattern->getPattern()->width()), m_maskPattern->height / static_cast<double>(m_maskPattern->getPattern()->height()));
					if (mask_patternMirrorX)
						qmatrix.scale(-1, 1);
					if (mask_patternMirrorY)
						qmatrix.scale(1, -1);
					brushm = QBrush(*m_maskPattern->getPattern());
					brushm.setTransform(qmatrix);
				}
				QImage tmpImgM = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
				QPainter painterM;
				painterM.begin(&tmpImgM);
				painterM.setWorldTransform(painter.worldTransform());
				painterM.setRenderHint(QPainter::Antialiasing, true);
				painterM.setRenderHint(QPainter::SmoothPixmapTransform, true);
				painterM.fillPath(m_path, brushm);
				painterM.end();
				QImage tmpImgO = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
				QPainter painterO;
				painterO.begin(&tmpImgO);
				painterO.setWorldTransform(painter.worldTransform());
				painterO.setRenderHint(QPainter::Antialiasing, true);
				painterO.setRenderHint(QPainter::SmoothPixmapTransform, true);
				painterO.fillPath(m_path, brush);
				painterO.end();
				int h = tmpImgO.height();
				int w = tmpImgO.width();
				QRgb *s, *t;
				for( int yi=0; yi < h; ++yi )
				{
					s = (QRgb*)(tmpImgM.scanLine( yi ));
					t = (QRgb*)(tmpImgO.scanLine( yi ));
					for( int xi=0; xi < w; ++xi )
					{
						*t = qRgba(qRed(*t), qGreen(*t), qBlue(*t), qAlpha(*s));
						s++;
						t++;
					}
				}
				painter.save();
				painter.resetTransform();
				painter.drawImage(0, 0, tmpImgO);
				painter.restore();
			}
			else
			{
				painter.setOpacity(fill_trans);
				painter.fillPath(m_path, brush);
			}
		}
		else if (fillMode == 3)
		{
			painter.setRenderHint(QPainter::Antialiasing, false);
			painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
			QTransform qmatrix;
			qmatrix.translate(patternOffsetX, patternOffsetY);
			qmatrix.rotate(patternRotation);
			qmatrix.shear(-patternSkewX, patternSkewY);
			qmatrix.scale(patternScaleX, patternScaleY);
			qmatrix.scale(m_pattern->width / static_cast<double>(m_pattern->getPattern()->width()), m_pattern->height / static_cast<double>(m_pattern->getPattern()->height()));
			if (patternMirrorX)
				qmatrix.scale(-1, 1);
			if (patternMirrorY)
				qmatrix.scale(1, -1);
			QBrush brush = QBrush(*m_pattern->getPattern());
			brush.setTransform(qmatrix);
			if (maskMode > 0)
			{
				QTransform qmatrix;
				QBrush brushm;
				if (maskMode == 1)
				{
					QGradient patm;
					double x1 = mask_gradient.origin().x();
					double y1 = mask_gradient.origin().y();
					double x2 = mask_gradient.vector().x();
					double y2 = mask_gradient.vector().y();
					double fx = mask_gradient.focalPoint().x();
					double fy = mask_gradient.focalPoint().y();
					if (mask_gradient.type() == VGradient::linear)
						patm = QLinearGradient(x1, y1,  x2, y2);
					else
						patm = QRadialGradient(x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)), fx, fy);
					QList<VColorStop*> colorStops = mask_gradient.colorStops();
					QColor qStopColor;
					for( int offset = 0 ; offset < colorStops.count() ; offset++ )
					{
						qStopColor = colorStops[ offset ]->color;
						int h, s, v, sneu, vneu;
						int shad = colorStops[offset]->shade;
						qStopColor.getHsv(&h, &s, &v);
						sneu = s * shad / 100;
						vneu = 255 - ((255 - v) * shad / 100);
						qStopColor.setHsv(h, sneu, vneu);
						qStopColor.setAlphaF(colorStops[offset]->opacity);
						patm.setColorAt(colorStops[ offset ]->rampPoint, qStopColor);
					}
					if (mask_gradient.type() == VGradient::radial)
					{
						double rotEnd = xy2Deg(x2 - x1, y2 - y1);
						qmatrix.translate(x1, y1);
						qmatrix.rotate(rotEnd);
						qmatrix.shear(mask_gradientSkew, 0);
						qmatrix.translate(0, y1 * (1.0 - mask_gradientScale));
						qmatrix.translate(-x1, -y1);
						qmatrix.scale(1, mask_gradientScale);
					}
					else
					{
						qmatrix.translate(x1, y1);
						qmatrix.shear(-mask_gradientSkew, 0);
						qmatrix.translate(-x1, -y1);
					}
					brushm = QBrush(patm);
					brushm.setTransform(qmatrix);
				}
				else if (maskMode == 2)
				{
					qmatrix.translate(mask_patternOffsetX, mask_patternOffsetY);
					qmatrix.rotate(mask_patternRotation);
					qmatrix.shear(-mask_patternSkewX, mask_patternSkewY);
					qmatrix.scale(mask_patternScaleX, mask_patternScaleY);
					qmatrix.scale(m_maskPattern->width / static_cast<double>(m_maskPattern->getPattern()->width()), m_maskPattern->height / static_cast<double>(m_maskPattern->getPattern()->height()));
					if (mask_patternMirrorX)
						qmatrix.scale(-1, 1);
					if (mask_patternMirrorY)
						qmatrix.scale(1, -1);
					brushm = QBrush(*m_maskPattern->getPattern());
					brush.setTransform(qmatrix);
				}
				QImage tmpImgM = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
				QPainter painterM;
				painterM.begin(&tmpImgM);
				painterM.setWorldTransform(painter.worldTransform());
				painterM.setRenderHint(QPainter::Antialiasing, true);
				painterM.setRenderHint(QPainter::SmoothPixmapTransform, true);
				painterM.fillPath(m_path, brushm);
				painterM.end();
				QImage tmpImgO = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
				QPainter painterO;
				painterO.begin(&tmpImgO);
				painterO.setWorldTransform(painter.worldTransform());
				painterO.setRenderHint(QPainter::Antialiasing, true);
				painterO.setRenderHint(QPainter::SmoothPixmapTransform, true);
				painterO.fillPath(m_path, brush);
				painterO.end();
				int h = tmpImgO.height();
				int w = tmpImgO.width();
				QRgb *s, *t;
				for( int yi=0; yi < h; ++yi )
				{
					s = (QRgb*)(tmpImgM.scanLine( yi ));
					t = (QRgb*)(tmpImgO.scanLine( yi ));
					for( int xi=0; xi < w; ++xi )
					{
						*t = qRgba(qRed(*t), qGreen(*t), qBlue(*t), qAlpha(*s));
						s++;
						t++;
					}
				}
				painter.save();
				painter.resetTransform();
				painter.drawImage(0, 0, tmpImgO);
				painter.restore();
			}
			else
			{
				painter.setOpacity(fill_trans);
				painter.fillPath(m_path, brush);
			}
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
		}
	}
	else
	{
		QColor paint = m_stroke;
		paint.setAlphaF(stroke_trans);
		QPen pen;
		if( m_array.count() == 0 )
		{
			if (PLineStyle == Qt::SolidLine)
				pen = QPen(paint, LineWidth, PLineStyle, PLineEnd, PLineJoin);
			else
			{
				getDashArray(PLineStyle, 1, m_array);
				pen.setDashPattern(m_array);
				pen.setColor(paint);
				pen.setWidthF(LineWidth);
				pen.setCapStyle(PLineEnd);
				pen.setJoinStyle(PLineJoin);
			}
		}
		else
		{
			double lw = LineWidth;
			if( LineWidth == 0 )
				lw = 1.0 / m_zoomFactor;
			QVector<qreal> dashes;
			for (int a = 0; a < m_array.count(); a++)
			{
				dashes.append(m_array[a] / lw);
			}
			pen.setDashPattern(dashes);
			pen.setDashOffset(m_offset / lw);
			pen.setColor(paint);
			pen.setWidthF(LineWidth);
			pen.setCapStyle(PLineEnd);
			pen.setJoinStyle(PLineJoin);
		}
		if (strokeMode == 3)
		{
			QTransform qmatrix;
			qmatrix.translate(-LineWidth / 2.0, -LineWidth / 2.0);
			qmatrix.translate(patternOffsetX, patternOffsetY);
			qmatrix.rotate(patternRotation);
			qmatrix.shear(-patternSkewX, patternSkewY);
			qmatrix.scale(patternScaleX, patternScaleY);
			qmatrix.scale(m_pattern->width / static_cast<double>(m_pattern->getPattern()->width()), m_pattern->height / static_cast<double>(m_pattern->getPattern()->height()));
			if (patternMirrorX)
				qmatrix.scale(-1, 1);
			if (patternMirrorY)
				qmatrix.scale(1, -1);
			QBrush brush = QBrush(*m_pattern->getPattern());
			brush.setTransform(qmatrix);
//			painter.rotate(0.0001);	// hack to get Qt-4's strange pattern rendering working
			painter.setOpacity(stroke_trans);
			pen.setBrush(brush);
		}
		else if (strokeMode == 2)
		{
			QGradient pat;
			double x1 = stroke_gradient.origin().x();
			double y1 = stroke_gradient.origin().y();
			double x2 = stroke_gradient.vector().x();
			double y2 = stroke_gradient.vector().y();
			double fx = stroke_gradient.focalPoint().x();
			double fy = stroke_gradient.focalPoint().y();
			if (stroke_gradient.type() == VGradient::linear)
				pat = QLinearGradient(x1, y1,  x2, y2);
			else
				pat = QRadialGradient(x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)), fx, fy);
			QList<VColorStop*> colorStops = stroke_gradient.colorStops();
			QColor qStopColor;
			for( int offset = 0 ; offset < colorStops.count() ; offset++ )
			{
				qStopColor = colorStops[ offset ]->color;
				int h, s, v, sneu, vneu;
				int shad = colorStops[offset]->shade;
				qStopColor.getHsv(&h, &s, &v);
				sneu = s * shad / 100;
				vneu = 255 - ((255 - v) * shad / 100);
				qStopColor.setHsv(h, sneu, vneu);
				qStopColor.setAlphaF(colorStops[offset]->opacity);
				pat.setColorAt(colorStops[ offset ]->rampPoint, qStopColor);
			}
			QTransform qmatrix;
			if (stroke_gradient.type() == VGradient::radial)
			{
				double rotEnd = xy2Deg(x2 - x1, y2 - y1);
				qmatrix.translate(x1, y1);
				qmatrix.rotate(rotEnd);
				qmatrix.shear(gradientSkew, 0);
				qmatrix.translate(0, y1 * (1.0 - gradientScale));
				qmatrix.translate(-x1, -y1);
				qmatrix.scale(1, gradientScale);
			}
			else
			{
				qmatrix.translate(x1, y1);
				qmatrix.shear(-gradientSkew, 0);
				qmatrix.translate(-x1, -y1);
			}
			QBrush brush = QBrush(pat);
			brush.setTransform(qmatrix);
			painter.setOpacity(stroke_trans);
			pen.setBrush(brush);
		}
		painter.strokePath(m_path, pen);
	}
	painter.restore();
}
#endif

void ScPainter::setClipPath()
{
#ifdef HAVE_CAIRO
	cairo_clip (m_cr);
#else
	painter.setClipPath(m_path, Qt::IntersectClip);
	m_path = QPainterPath();
#endif
}

void ScPainter::drawImage( QImage *image)
{
#ifdef HAVE_CAIRO
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 2, 6)
/*
	cairo_surface_t *image3 = cairo_image_surface_create_for_data ((uchar*)image->bits(), CAIRO_FORMAT_ARGB32, image->width(), image->height(), image->width()*4);
	cairo_set_source_surface (m_cr, image3, 0, 0);
	cairo_paint_with_alpha (m_cr, fill_trans);
	cairo_surface_destroy (image3);
*/
/* Code with Layers, crashes on cairo_push_group */
//	cairo_scale(m_cr, m_zoomFactor, m_zoomFactor);
	cairo_push_group(m_cr);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
	cairo_set_fill_rule(m_cr, cairo_get_fill_rule(m_cr));
	cairo_surface_t *image2  = cairo_image_surface_create_for_data ((uchar*)image->bits(), CAIRO_FORMAT_RGB24, image->width(), image->height(), image->width()*4);
	cairo_surface_t *image3 = cairo_image_surface_create_for_data ((uchar*)image->bits(), CAIRO_FORMAT_ARGB32, image->width(), image->height(), image->width()*4);
	cairo_set_source_surface (m_cr, image2, 0, 0);
    cairo_mask_surface (m_cr, image3, 0, 0);
	cairo_surface_destroy (image2);
	cairo_surface_destroy (image3);
	cairo_pop_group_to_source (m_cr);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 4)
	setRasterOp(m_blendModeFill);
#endif
	if (maskMode > 0)
	{
		cairo_pattern_t *patM = getMaskPattern();
		cairo_mask(m_cr, patM);
		if ((maskMode == 2) || (maskMode == 4))
			cairo_surface_destroy(imageMask);
		cairo_pattern_destroy(patM);
	}
	else
		cairo_paint_with_alpha (m_cr, fill_trans);
	cairo_set_operator(m_cr, CAIRO_OPERATOR_OVER);
#else
/* Working code, sadly we need to create an additional mask image with the same size as the image to be painted */
	cairo_surface_t *image3;
	QImage mask;
	cairo_set_fill_rule(m_cr, cairo_get_fill_rule(m_cr));
	cairo_surface_t *image2  = cairo_image_surface_create_for_data ((uchar*)image->bits(), CAIRO_FORMAT_RGB24, image->width(), image->height(), image->width()*4);
	if (fill_trans != 1.0)
	{
		mask = QImage(image->width(), image->height(), QImage::Format_Mono);
		for( int yi = 0; yi < image->height(); ++yi )
		{
			QRgb * s = (QRgb*)(image->scanLine( yi ));
			unsigned char *d = (unsigned char *)(mask.scanLine( yi ));
			for( int xi=0; xi < image->width(); ++xi )
			{
				*d++ = static_cast<unsigned char>(qAlpha(*s++) * fill_trans);
			}
		}
		int adj;
		if (image->width() % 4 == 0)
			adj = 0;
		else
			adj = 4 - (image->width() % 4);
		image3 = cairo_image_surface_create_for_data ((uchar*)mask.bits(), CAIRO_FORMAT_A8, image->width(), image->height(), image->width() + adj);
	}
	else
		image3 = cairo_image_surface_create_for_data ((uchar*)image->bits(), CAIRO_FORMAT_ARGB32, image->width(), image->height(), image->width()*4);
	cairo_set_source_surface (m_cr, image2, 0, 0);
	cairo_mask_surface (m_cr, image3, 0, 0);
	cairo_surface_destroy (image2);
	cairo_surface_destroy (image3);
#endif
#else
	if (maskMode == 1)
	{
		QGradient patm;
		double x1 = mask_gradient.origin().x();
		double y1 = mask_gradient.origin().y();
		double x2 = mask_gradient.vector().x();
		double y2 = mask_gradient.vector().y();
		double fx = mask_gradient.focalPoint().x();
		double fy = mask_gradient.focalPoint().y();
		if (mask_gradient.type() == VGradient::linear)
			patm = QLinearGradient(x1, y1,  x2, y2);
		else
			patm = QRadialGradient(x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)), fx, fy);
		QList<VColorStop*> colorStops = mask_gradient.colorStops();
		QColor qStopColor;
		for( int offset = 0 ; offset < colorStops.count() ; offset++ )
		{
			qStopColor = colorStops[ offset ]->color;
			int h, s, v, sneu, vneu;
			int shad = colorStops[offset]->shade;
			qStopColor.getHsv(&h, &s, &v);
			sneu = s * shad / 100;
			vneu = 255 - ((255 - v) * shad / 100);
			qStopColor.setHsv(h, sneu, vneu);
			qStopColor.setAlphaF(colorStops[offset]->opacity);
			patm.setColorAt(colorStops[ offset ]->rampPoint, qStopColor);
		}
		QTransform qmatrix;
		if (mask_gradient.type() == VGradient::radial)
		{
			double rotEnd = xy2Deg(x2 - x1, y2 - y1);
			qmatrix.translate(x1, y1);
			qmatrix.rotate(rotEnd);
			qmatrix.shear(mask_gradientSkew, 0);
			qmatrix.translate(0, y1 * (1.0 - mask_gradientScale));
			qmatrix.translate(-x1, -y1);
			qmatrix.scale(1, mask_gradientScale);
		}
		else
		{
			qmatrix.translate(x1, y1);
			qmatrix.shear(-mask_gradientSkew, 0);
			qmatrix.translate(-x1, -y1);
		}
		QImage tmpImgM = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
		QPainter painterM;
		painterM.begin(&tmpImgM);
		painterM.setWorldTransform(painter.worldTransform());
		painterM.setRenderHint(QPainter::Antialiasing, true);
		painterM.setRenderHint(QPainter::SmoothPixmapTransform, true);
		QBrush brushm = QBrush(patm);
		brushm.setTransform(qmatrix);
		painterM.fillPath(painter.clipPath(), brushm);
		painterM.end();
		QImage tmpImgO = QImage(m_image->width(), m_image->height(), QImage::Format_ARGB32_Premultiplied);
		QPainter painterO;
		painterO.begin(&tmpImgO);
		painterO.setWorldTransform(painter.worldTransform());
		painterO.setRenderHint(QPainter::Antialiasing, true);
		painterO.setRenderHint(QPainter::SmoothPixmapTransform, true);
		painterO.drawImage(0, 0, *image);
		painterO.end();
		int h = tmpImgO.height();
		int w = tmpImgO.width();
		QRgb *s, *t;
		for( int yi=0; yi < h; ++yi )
		{
			s = (QRgb*)(tmpImgM.scanLine( yi ));
			t = (QRgb*)(tmpImgO.scanLine( yi ));
			for( int xi=0; xi < w; ++xi )
			{
				*t = qRgba(qRed(*t), qGreen(*t), qBlue(*t), qAlpha(*s));
				s++;
				t++;
			}
		}
		painter.save();
		painter.resetTransform();
		painter.drawImage(0, 0, tmpImgO);
		painter.restore();
	}
	else
	{
		painter.setOpacity(fill_trans);
		painter.drawImage(0, 0, *image);
	}
#endif
}

void ScPainter::setupPolygon(FPointArray *points, bool closed)
{
	bool nPath = true;
	bool first = true;
	FPoint np, np1, np2, np3, np4;
#ifdef HAVE_CAIRO
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
    			cairo_move_to( m_cr, np.x(), np.y());
				nPath = false;
				first = false;
				np4 = np;
			}
			np = points->point(poi);
			np1 = points->point(poi+1);
			np2 = points->point(poi+3);
			np3 = points->point(poi+2);
			if (np4 == np3)
				continue;
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 9, 6)
			if ((np == np1) && (np2 == np3))
				cairo_line_to( m_cr, np3.x(), np3.y());
			else
#endif
				cairo_curve_to(m_cr, np1.x(), np1.y(), np2.x(), np2.y(), np3.x(), np3.y());
			np4 = np3;
		}
		if (closed)
    		cairo_close_path( m_cr );
	}
#else
	// not HAVE_CAIRO
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
					m_path.closeSubpath();
    			m_path.moveTo(np.x(), np.y());
				nPath = false;
				first = false;
			}
			np = points->point(poi);
			np1 = points->point(poi+1);
			np2 = points->point(poi+3);
			np3 = points->point(poi+2);
			if ((np == np1) && (np2 == np3))
				m_path.lineTo(np3.x(), np3.y());
			else
				m_path.cubicTo(np1.x(), np1.y(), np2.x(), np2.y(), np3.x(), np3.y());
		}
		if (closed)
			m_path.closeSubpath();
	}
#endif
}

void ScPainter::drawPolygon()
{
	fillPath();
}

void ScPainter::drawPolyLine()
{
	strokePath();
}

void ScPainter::drawLine(FPoint start, FPoint end)
{
	newPath();
	moveTo(start.x(), start.y());
	lineTo(end.x(), end.y());
	strokePath();
}

void ScPainter::drawRect(double x, double y, double w, double h)
{
	newPath();
#ifdef HAVE_CAIRO
	cairo_rectangle(m_cr, x, y, w, h);
//	moveTo( x, y );
//	lineTo( x+w, y );
//	lineTo( x+w, y+h );
//	lineTo( x, y+h );
//	lineTo( x, y );
//	cairo_close_path( m_cr );
#else
	m_path.addRect(x, y, w, h);
#endif
	fillPath();
	strokePath();
}

void ScPainter::drawText(QRectF area, QString text)
{
#ifdef HAVE_CAIRO
	cairo_text_extents_t extents;
	cairo_font_extents_t extentsF;
	double x = area.center().x();
	double y;
	double ww = 0;
	double hh = 0;
	double r, g, b;
	cairo_select_font_face(m_cr, m_font.family().toLatin1(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(m_cr, m_font.pointSizeF());
	cairo_font_extents (m_cr, &extentsF);
	QStringList textList = text.split("\n");
	for (int a = 0; a < textList.count(); ++a)
	{
		cairo_text_extents (m_cr, textList[a].toUtf8(), &extents);
		x = qMin(area.center().x() - (extents.width / 2.0 + extents.x_bearing), x);
		ww = qMax(ww, extents.width);
	}
	hh = extentsF.height * textList.count();
	y = area.center().y() - ((extentsF.height * textList.count()) / 2.0);
	m_fill.getRgbF(&r, &g, &b);
	cairo_set_source_rgba( m_cr, r, g, b, fill_trans );
	cairo_new_path( m_cr );
	cairo_rectangle(m_cr, x, y, ww, hh);
	cairo_fill( m_cr );
	cairo_new_path( m_cr );
	y += extentsF.ascent;
	cairo_move_to (m_cr, x, y);
	m_stroke.getRgbF(&r, &g, &b);
	cairo_set_source_rgba( m_cr, r, g, b, stroke_trans );
	for (int a = 0; a < textList.count(); ++a)
	{
		cairo_show_text (m_cr, textList[a].toUtf8());
		y += extentsF.height;
		cairo_move_to (m_cr, x, y);
	}
#else
	painter.setFont(m_font);
	painter.setPen(m_stroke);
	painter.drawText(area, text, QTextOption(Qt::AlignCenter));
#endif
}
