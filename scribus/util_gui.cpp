#include "util_gui.h"

#include <QPainterPath>
#include <QPalette>
#include "scpainter.h"
#include "scpattern.h"
#include "scribusapp.h"
#include "vgradient.h"

void drawCircularHandle(QPainter *painter, QPointF center, qreal width, QBrush background, bool isEnabled)
{
	qreal radius = width /2.;

	painter->save();

	// Draw background
	painter->setBrush(background);
	painter->setPen(Qt::NoPen);
	painter->drawEllipse(center, radius, radius);

	painter->setBrush(Qt::NoBrush);

	// Draw white ring
	painter->setPen(QPen(ScQApp->palette().color(isEnabled ? QPalette::Active : QPalette::Disabled, QPalette::Window), 2));
	painter->drawEllipse(center, radius-1, radius-1);

	// Draw black ring
	painter->setPen(QPen(ScQApp->palette().color(isEnabled ? QPalette::Active : QPalette::Disabled, QPalette::WindowText), 1));
	painter->drawEllipse(center, radius, radius);

	painter->restore();
}

void drawColorBox(QPainter *painter, QRect rect, QColor color, bool isEnabled)
{
	painter->save();
	painter->setBrush(color);
	painter->setPen(QPen(ScQApp->palette().color(isEnabled ? QPalette::Active : QPalette::Disabled, QPalette::Mid), 1));
	painter->drawRect(rect.adjusted(0, 0, -1, -1));
	painter->restore();
}

void drawPointerHandle(QPainter *painter, QPointF pointer, qreal width, QColor background, bool isEnabled, bool isEmpty)
{
	qreal height = width *1.5;
	QRect rect( pointer.x() -width/2, pointer.y() , width, height );

	QPainterPath path;
	path.moveTo(pointer);
	path.lineTo(pointer + QPoint(width/2, height/3) );
	path.lineTo(pointer + QPoint(width/2, height) );
	path.lineTo(pointer + QPoint(-width/2, height) );
	path.lineTo(pointer + QPoint(-width/2, height/3) );
	path.closeSubpath();

	painter->save();

	// Draw dark border
	painter->setPen(QPen(ScQApp->palette().color(isEnabled ? QPalette::Active : QPalette::Disabled, QPalette::WindowText), 3));
	painter->setBrush(Qt::NoBrush);
	painter->drawPath(path);

	// Draw background
	painter->setClipPath(path);
	painter->setPen(Qt::NoPen);
	renderCheckerPattern(painter, rect);

	// Draw color
	if(isEmpty)
		painter->drawPixmap(rect, renderEmptyPattern(rect.size(), painter->device()->devicePixelRatio()) );
	else
		painter->drawPixmap(rect, renderColor(rect.size(), painter->device()->devicePixelRatio(), background, background, 1) );

	// Draw light border
	painter->setBrush(Qt::NoBrush);
	painter->setClipping(false);
	painter->setPen(QPen(ScQApp->palette().color(isEnabled ? QPalette::Active : QPalette::Disabled, QPalette::Window), 1.5));
	painter->drawPath(path);

	painter->restore();
}

void drawSelectionHandle(QPainter *painter, QPointF point, QPen pen, qreal scaleFactor, bool isActive)
{
	QRectF handleRect(0, 0, 6 / scaleFactor, 6 / scaleFactor);
	handleRect.moveCenter(point);

	QBrush b = QBrush( (isActive) ? pen.color() : Qt::white );

	if (isActive)
		pen.setColor(Qt::white);

	painter->setPen(pen);
	painter->setBrush(b);
	painter->drawRect(handleRect);
}

void drawNodeHandle(QPainter *painter, QPointF point, QPen pen, qreal scaleFactor, bool isActive)
{
	drawSelectionHandle(painter, point, pen, scaleFactor, isActive);
}

void drawNodeControl(QPainter *painter, QPointF point, QPen pen, qreal scaleFactor, bool isActive)
{
	bool aa = painter->testRenderHint(QPainter::Antialiasing);

	QRectF handleRect(0, 0, 7 / scaleFactor, 7 / scaleFactor);
	handleRect.moveCenter(point);

	QBrush b = QBrush( (isActive) ? pen.color() : Qt::white );
	if (isActive)
		pen.setColor(Qt::white);

	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(pen);
	painter->setBrush(b);
	painter->drawEllipse(handleRect);

	if (!aa)
		painter->setRenderHint(QPainter::Antialiasing, false);
}

void drawWeldMarker(QPainter *painter, QPointF point, QColor color, qreal scaleFactor)
{
	bool aa = painter->testRenderHint(QPainter::Antialiasing);

	QRectF handleRect(0, 0, 8 / scaleFactor, 8 / scaleFactor);
	handleRect.moveCenter(point);

	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(color));
	painter->drawEllipse(handleRect);

	if (!aa)
		painter->setRenderHint(QPainter::Antialiasing, false);
}

void renderCheckerPattern(QPainter *painter, QRectF rect)
{
	QImage pattern(rect.width(), rect.height(), QImage::Format_ARGB32_Premultiplied);

	QPainter p(&pattern);
	p.fillRect(pattern.rect(), QBrush(Qt::white));

	int cellSize = 4;
	int cellSizeX = rect.width() / cellSize + cellSize;
	int cellSizeY = rect.height() / cellSize + cellSize;

	for(int y = 0; y < cellSizeY; y++)
		for(int x = y % 2; x < cellSizeX; x += 2)
			p.fillRect(QRectF(x * cellSize, y * cellSize, cellSize, cellSize), QBrush(Qt::lightGray));

	p.end();
	painter->drawImage(rect, pattern);
}

QPixmap renderColor(QSize size, double scale, QColor color, QColor colorShade, double alpha)
{
	alpha = qBound(0., alpha, 1.);

	int w = size.width();
	int h = size.height();
	double mid = 0.5;

	QRectF rectColorSolid( 0,		0,			w * mid, h * mid ); // top left
	QRectF rectColorAlpha( w * mid, 0,			w * mid, h * mid ); // top right
	QRectF rectShadeSolid( 0,		h * mid,	w * mid, h * mid ); // bottom left
	QRectF rectShadeAlpha( w * mid, h * mid,	w * mid, h * mid ); // bottom right

	QColor colorSolid(color);
	colorSolid.setAlphaF(1.0);
	QColor colorAlpha(color);
	colorAlpha.setAlphaF(alpha);
	QColor shadeSolid(colorShade);
	shadeSolid.setAlphaF(1.0);
	QColor shadeAlpha(colorShade);
	shadeAlpha.setAlphaF(alpha);

	if(alpha == 1)
	{
		colorAlpha = colorSolid;
		shadeAlpha = shadeSolid;
	}

	QPixmap pixmap(w * scale, h * scale);
	pixmap.setDevicePixelRatio(scale);
	pixmap.fill(Qt::transparent);

	QPainter p(&pixmap);
	p.setPen(Qt::NoPen);
	p.setBrush(QBrush(colorSolid));
	p.drawRect(rectColorSolid);
	p.setBrush(QBrush(colorAlpha));
	p.drawRect(rectColorAlpha);
	p.setBrush(QBrush(shadeSolid));
	p.drawRect(rectShadeSolid);
	p.setBrush(QBrush(shadeAlpha));
	p.drawRect(rectShadeAlpha);
	p.end();

	return pixmap;
}

QPixmap renderEmptyPattern(QSize size, double scale)
{
	int w = size.width();
	int h = size.height();

	QPixmap pixmap(w * scale, h * scale);
	pixmap.setDevicePixelRatio(scale);
	pixmap.fill(Qt::white);

	QPainter p(&pixmap);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(QPen(QBrush(Qt::red), 2));
	p.drawLine(QPoint(0, h), QPoint(w, 0));
	p.end();

	return pixmap;
}

QPixmap renderGradientLinear(QSize size, double scale, const VGradient& gradient)
{
	int w = size.width();
	int h = size.height();

	QImage pixm(w * scale, h * scale, QImage::Format_ARGB32_Premultiplied);
	pixm.setDevicePixelRatio(scale);
	pixm.fill(Qt::transparent);

	ScPainter *p = new ScPainter(&pixm, w * scale, h * scale);
	p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setFillMode(ScPainter::Gradient);
	p->fill_gradient = gradient;
	p->setGradient(VGradient::linear, FPoint(0,h/2), FPoint(w, h/2), FPoint(0,0), 1, 0);
	p->drawRect(0, 0, w, h);
	p->end();
	delete p;

	return QPixmap::fromImage(pixm);
}

QPixmap renderGradientRadial(QSize size, double scale, const VGradient& gradient)
{
	int w = size.width();
	int h = size.height();
	qreal wHalf = w / 2;
	qreal hHalf = h / 2;

	QImage pixm(w * scale, h * scale, QImage::Format_ARGB32_Premultiplied);
	pixm.setDevicePixelRatio(scale);
	pixm.fill(Qt::transparent);

	ScPainter *p = new ScPainter(&pixm, w * scale, h * scale);
	p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setFillMode(ScPainter::Gradient);
	p->fill_gradient = gradient;
	p->setGradient(VGradient::radial, FPoint(wHalf, hHalf), FPoint(w, hHalf), FPoint(wHalf, hHalf), 1, 0);
	p->drawRect(0, 0, w, h);
	p->end();
	delete p;

	return QPixmap::fromImage(pixm);
}

QPixmap renderGradientConical(QSize size, double scale, const VGradient& gradient)
{
	int w = size.width();
	int h = size.height();
	qreal wHalf = w / 2;
	qreal hHalf = h / 2;

	QConicalGradient grad(wHalf, hHalf, 0);
	grad.setStops(gradient.toQGradientStops());

	QImage pixm(w * scale, h * scale, QImage::Format_ARGB32_Premultiplied);
	pixm.setDevicePixelRatio(scale);
	pixm.fill(Qt::transparent);

	QPainter p(&pixm);
	p.setPen(Qt::NoPen);
	p.setBrush(grad);	
	p.translate(0, h); // we have to flip vertically, because QConicalGradient is drawing counter-clockwise
	p.scale(1.0, -1.0);
	p.drawRect(0, 0, w, h);
	p.end();

	return QPixmap::fromImage(pixm);
}

QPixmap renderGradient4Colors(QSize size, double scale, QColor col1, QColor col2, QColor col3, QColor col4)
{
	int w = size.width();
	int h = size.height();

	QImage pixm(w * scale, h * scale, QImage::Format_ARGB32_Premultiplied);
	pixm.setDevicePixelRatio(scale);
	pixm.fill(Qt::transparent);

	ScPainter *p = new ScPainter(&pixm, w * scale, h * scale);
	p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setFillMode(ScPainter::Gradient);
	p->fill_gradient = VGradient();
	p->set4ColorGeometry(FPoint(0, 0), FPoint(w, 0), FPoint(w, h), FPoint(0, h), FPoint(0, 0), FPoint(w, 0), FPoint(w, h), FPoint(0, h));
	p->set4ColorColors(col1, col2, col3, col4);
	p->drawRect(0, 0, w, h);
	p->end();
	delete p;

	return QPixmap::fromImage(pixm);
}

QPixmap renderGradientDiamond(QSize size, double scale, const VGradient& gradient)
{
	int w = size.width();
	int h = size.height();

	QImage pixm(w * scale, h * scale, QImage::Format_ARGB32_Premultiplied);
	pixm.setDevicePixelRatio(scale);
	pixm.fill(Qt::transparent);

	ScPainter *p = new ScPainter(&pixm, w * scale, h * scale);
	p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setFillMode(ScPainter::Gradient);
	p->fill_gradient = gradient;
	p->setDiamondGeometry(FPoint(0, 0), FPoint(w, 0), FPoint(w, h), FPoint(0, h), FPoint(0, 0), FPoint(w, 0), FPoint(w, h), FPoint(0, h), FPoint(w/2,h/2));
	p->drawRect(0, 0, w, h);
	p->end();
	delete p;

	return QPixmap::fromImage(pixm);
}

QPixmap renderGradientMesh(QSize size, double scale)
{
	int w = size.width();
	int w25 = w * .25;
	int w50 = w * .5;
	int w75 = w * .75;
	int h = size.height();
	int h25 = h * .25;
	int h50 = h * .5;
	int h75 = h * .75;

	QImage pixm(w * scale, h * scale, QImage::Format_ARGB32_Premultiplied);
	pixm.setDevicePixelRatio(scale);
	pixm.fill(Qt::transparent);

	QPainter p(&pixm);
	p.setRenderHint(QPainter::Antialiasing);
	p.setBrush(QBrush(Qt::white));
	p.drawRect(0, 0, w, h);
	p.setPen(Qt::black);
	p.drawLine(QPoint(0, h25), QPoint(w, h25));
	p.drawLine(QPoint(0, h50), QPoint(w, h50));
	p.drawLine(QPoint(0, h75), QPoint(w, h75));
	p.drawLine(QPoint(w25, 0), QPoint(w25, h));
	p.drawLine(QPoint(w50, 0), QPoint(w50, h));
	p.drawLine(QPoint(w75, 0), QPoint(w75, h));
	p.setBrush(QBrush(Qt::red));
	p.drawEllipse(QPoint(w50, h50), 3, 3);
	p.end();

	return QPixmap::fromImage(pixm);
}

QPixmap renderGradientPatchMesh(QSize size, double scale)
{
	return renderGradientMesh(size, scale);
}

QPixmap renderHatch(QSize size, double scale, int type, double distance, double angle, bool hasBackground, QColor backgroundColor, QColor foregroundColor)
{
	int w = size.width();
	int h = size.height();

	QImage pixm(w * scale, h * scale, QImage::Format_ARGB32_Premultiplied);
	pixm.setDevicePixelRatio(scale);
	pixm.fill(Qt::transparent);

	ScPainter *p = new ScPainter(&pixm, w * scale, h * scale);
	p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	p->setFillMode(ScPainter::Hatch);
	p->setHatchParameters(type, distance, angle, hasBackground, backgroundColor, foregroundColor, w, h);
	p->drawRect(0, 0, w, h);
	p->end();
	delete p;

	return QPixmap::fromImage(pixm);
}

QPixmap renderPattern(QSize size, double scale, const ScPattern& pattern)
{
	int w = size.width();
	int h = size.height();
	int pW = pattern.getPattern().width();
	int pH = pattern.getPattern().height();

	QImage pixm(w * scale, h * scale, QImage::Format_ARGB32_Premultiplied);
	pixm.setDevicePixelRatio(scale);
	pixm.fill(Qt::transparent);

	Qt::TransformationMode tm = Qt::FastTransformation;

	// use smooth transformation for downscale pattern
	if(pW > w || pH > h)
		tm = Qt::SmoothTransformation;

	QImage img;
	if (pW >= pH)
		img = pattern.getPattern().scaledToWidth(w, tm);
	else
		img = pattern.getPattern().scaledToHeight(h, tm);

	QPainter qp(&pixm);
	qp.setPen(Qt::NoPen);
	qp.drawImage(w / 2 - img.width() / 2, h / 2 - img.height() / 2, img);
	qp.end();

	return QPixmap::fromImage(pixm);
}

QPixmap combinePixmaps(const QPixmap &background, const QPixmap &foreground, double scale, bool tintForeground, bool isDarkColor)
{
	int w = qMax(background.deviceIndependentSize().width(), foreground.deviceIndependentSize().width());
	int h = qMax(background.deviceIndependentSize().height(), foreground.deviceIndependentSize().height());

	QImage pixm(w * scale, h * scale, QImage::Format_ARGB32_Premultiplied);
	pixm.setDevicePixelRatio(scale);
	pixm.fill(Qt::transparent);

	QRect frame(0, 0, w, h);

	QRect rB(0, 0, background.deviceIndependentSize().width(), background.deviceIndependentSize().height());
	rB.moveCenter(frame.center());

	QRect rF(0, 0, foreground.deviceIndependentSize().width(), foreground.deviceIndependentSize().height());
	rF.moveCenter(frame.center());

	QPainter p;
	p.begin(&pixm);
	p.drawPixmap(rF, foreground);
	p.setCompositionMode( tintForeground ? QPainter::CompositionMode_SourceIn : QPainter::CompositionMode_SourceOut);
	p.drawPixmap(rB, background);
	p.end();

	p.begin(&pixm);
	p.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
	p.fillRect(frame, isDarkColor? Qt::white : Qt::black);
	p.end();

	return QPixmap::fromImage(pixm);
}

VColorStop computeInBetweenStop(const VColorStop* last, const VColorStop* actual, double t)
{
	double dist = actual->rampPoint - last->rampPoint;
	double perc = (t - last->rampPoint) / dist;
	double Rn = ((actual->color.redF() - last->color.redF()) * perc) + last->color.redF();
	double Gn = ((actual->color.greenF() - last->color.greenF()) * perc) + last->color.greenF();
	double Bn = ((actual->color.blueF() - last->color.blueF()) * perc) + last->color.blueF();
	QColor color;
	color.setRgbF(Rn, Gn, Bn);
	double opacity = ((actual->opacity - last->opacity) * perc) + last->opacity;
	int shade = qRound(((actual->shade - last->shade) * perc) + last->shade);
	QString name = last->name + actual->name + QString("%1").arg(perc);
	return VColorStop(t, t, color, opacity, name, shade);
}

QColor disabledColor(QColor color)
{
	QColor paleColor = blendColor(.5, color, ScQApp->palette().color(QPalette::Window));
	return QColor::fromHslF(0, 0, paleColor.lightnessF(), paleColor.alphaF());
}

QColor blendColor(double factor, QColor color1, QColor color2)
{
	factor = qBound(0.0, factor, 1.0);

	QColor tmp = color1;
	tmp.setRedF((tmp.redF() * factor) + (color2.redF() * (1. - factor)));
	tmp.setGreenF((tmp.greenF() * factor) + (color2.greenF() * (1. - factor)));
	tmp.setBlueF((tmp.blueF() * factor) + (color2.blueF() * (1. - factor)));
	return tmp;
}

QColor colorByRole(QPalette::ColorRole cr, double opacity, bool active)
{
	QColor fg = ScQApp->palette().color(cr);
	QColor bg = ScQApp->palette().color(QPalette::Window);

	if (!active)
	{
		fg = disabledColor(fg);
		bg = disabledColor(bg);
	}

	return blendColor(opacity, fg, bg);
}

bool isDarkColor(QColor color) {
	// sRGB luminance(Y) values
	double lum = 0.2126 * color.redF() + 0.7152 * color.greenF() + 0.0722 * color.blueF();
	return color.alphaF() > 0 && lum <= 0.65; // It is a dark color if the luminance is below 65%
}
