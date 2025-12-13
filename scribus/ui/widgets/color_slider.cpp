#include "color_slider.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QStyle>
#include <QStyleOption>
#include <QWidget>

#include "sccolorengine.h"
#include "scribusdoc.h"
#include "util_gui.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

const int RADIUS = 6;

ColorSlider::ColorSlider(QWidget *parent): ColorSlider(Mode::RGBRed, parent) {}

ColorSlider::ColorSlider(Mode mode, QWidget *parent): QSlider(parent)
{
	m_mode = mode;
	m_color = ScColor(255, 0, 0);

	setMinimumSize(24, 24);
	setOrientation(Qt::Horizontal);
	//setTickPosition(QSlider::TicksAbove);
	setup();
	updateValues();
}

void ColorSlider::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
}

/* ********************************************************************************* *
 *
 * Slots
 *
 * ********************************************************************************* */

void ColorSlider::setMode(Mode mode)
{
	m_mode = mode;
	setup();
	updateValues();
	renderMap();
	update();
}

void ColorSlider::setColor(const QString &colorName)
{
	if (!m_doc)
		return;

	if (colorName == CommonStrings::tr_NoneColor || colorName == CommonStrings::None)
		m_hasColor = false;
	else if (colorName.isEmpty())
		setColor(m_doc->PageColors.value("White"));
	else
		setColor(m_doc->PageColors.value(colorName));
}

void ColorSlider::setColor(ScColor color)
{
	m_color = color;
	m_hasColor = true;
	updateValues();
	renderMap();
	update();
}

void ColorSlider::setShade(double shade)
{
	m_shade = shade;
	updateValues();
	update();
}

void ColorSlider::setOpacity(double opacity)
{
	m_opacity = opacity;
	updateValues();
	update();
}

void ColorSlider::setIsDynamic(double dynamic)
{
	m_dynamicColor = dynamic;
	renderMap();
	update();
}

void ColorSlider::setValues(const QString &colorName, double opacity, double shade)
{
	if (!m_doc)
		return;

	if (colorName == CommonStrings::tr_NoneColor || colorName == CommonStrings::None)
		m_hasColor = false;
	else if (colorName.isEmpty())
		setValues(m_doc->PageColors.value("White"), opacity, shade);
	else
		setValues(m_doc->PageColors.value(colorName), opacity, shade);
}

void ColorSlider::setValues(ScColor color, double opacity, double shade)
{
	m_color = color;
	m_hasColor = true;
	m_shade = shade;
	m_opacity = opacity;
	updateValues();
	renderMap();
	update();
}

/* ********************************************************************************* *
 *
 * Functions
 *
 * ********************************************************************************* */

void ColorSlider::updateColor()
{
	double v1, v2, v3, v4;

	switch (m_mode)
	{
	case Mode::Opacity:
		m_opacity = value();
		break;
	case Mode::Shade:
		m_shade = value();
		break;
	case Mode::CMYKCyan:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelCMYK);
		m_color.getCMYK(&v1, &v2, &v3, &v4);
		m_color.setCmykColorF(value() / (double) maximum(), v2, v3, v4);
		break;
	case Mode::CMYKMagenta:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelCMYK);
		m_color.getCMYK(&v1, &v2, &v3, &v4);
		m_color.setCmykColorF(v1, value() / (double) maximum(), v3, v4);
		break;
	case Mode::CMYKYellow:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelCMYK);
		m_color.getCMYK(&v1, &v2, &v3, &v4);
		m_color.setCmykColorF(v1, v2, value() / (double) maximum(), v4);
		break;
	case Mode::CMYKKey:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelCMYK);
		m_color.getCMYK(&v1, &v2, &v3, &v4);
		m_color.setCmykColorF(v1, v2, v3, value() / (double) maximum());
		break;
	case Mode::HSVHue:
	{
		QColor color = ScColorEngine::getRGBColor(m_color, m_doc);
		color.setHslF(value() / (double) maximum(), color.saturationF(), color.valueF());
		m_color.fromQColor(color);
		break;
	}
	case Mode::HSVSaturation:
	{
		QColor color = ScColorEngine::getRGBColor(m_color, m_doc);
		color.setHslF(color.hslHueF(), value() / (double) maximum(), color.valueF());
		m_color.fromQColor(color);
		break;
	}
	case Mode::HSVValue:
	{
		QColor color = ScColorEngine::getRGBColor(m_color, m_doc);
		color.setHslF(color.hslHueF(), color.saturationF(), value() / (double) maximum());
		m_color.fromQColor(color);
		break;
	}
	case Mode::HLCHue:
	{
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		QLineF lin(QPointF(0.0, 0.0), QPointF(v2, v3));
		lin = QLineF::fromPolar(lin.length(), 360 - value());
		m_color.setLabColor(v1, lin.p2().x(), lin.p2().y());
		break;
	}
	case Mode::HLCChroma:
	{
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		QLineF lin(QPointF(0.0, 0.0), QPointF(v2, v3));
		lin = QLineF::fromPolar(value(), lin.angle());
		m_color.setLabColor(v1, lin.p2().x(), lin.p2().y());
		break;
	}
	case Mode::HLCLightness:
	case Mode::LabLightness:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		m_color.setLabColor(value(), v2, v3);
		break;
	case Mode::LabA:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		m_color.setLabColor(v1, value(), v3);
		break;
	case Mode::LabB:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		m_color.setLabColor(v1, v2, value());
		break;
	case Mode::RGBRed:
	case Mode::RGBWebRed:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelRGB);
		m_color.getRGB(&v1, &v2, &v3);
		m_color.setRgbColorF(QSlider::value() / (double) maximum(), v2, v3);
		break;
	case Mode::RGBGreen:
	case Mode::RGBWebGreen:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelRGB);
		m_color.getRGB(&v1, &v2, &v3);
		m_color.setRgbColorF(v1, QSlider::value() / (double) maximum(), v3);
		break;
	case Mode::RGBBlue:
	case Mode::RGBWebBlue:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelRGB);
		m_color.getRGB(&v1, &v2, &v3);
		m_color.setRgbColorF(v1, v2, QSlider::value() / (double) maximum());
		break;
	}

	emit colorChanged(m_color);
}

void ColorSlider::setup()
{
	disconnectSlots();

	switch (m_mode)
	{
	case Mode::Opacity:
	case Mode::CMYKCyan:
	case Mode::CMYKMagenta:
	case Mode::CMYKYellow:
	case Mode::CMYKKey:
	case Mode::HSVSaturation:
	case Mode::HSVValue:
	case Mode::HLCLightness:
	case Mode::LabLightness:
	case Mode::Shade:
		setMinimum(0);
		setMaximum(100);
		m_colorSteps = 100;
		break;
	case Mode::HSVHue:
	case Mode::HLCHue:
		setMinimum(0);
		setMaximum(359);
		m_colorSteps = 359;
		break;
	case Mode::LabA:
	case Mode::LabB:
		setMinimum(-128);
		setMaximum(128);
		m_colorSteps = 256;
		break;
	case Mode::HLCChroma:
		setMinimum(0);
		setMaximum(128);
		m_colorSteps = 128;
		break;
	case Mode::RGBRed:
	case Mode::RGBGreen:
	case Mode::RGBBlue:
		setMinimum(0);
		setMaximum(255);
		m_colorSteps = 255;
		break;
	case Mode::RGBWebRed:
	case Mode::RGBWebGreen:
	case Mode::RGBWebBlue:
		setMinimum(0);
		setMaximum(255);
		m_colorSteps = 5;
		break;
	}

	setSingleStep(maximum() / m_colorSteps);
	setPageStep(maximum() / m_colorSteps);

	connectSlots();
}

void ColorSlider::connectSlots()
{
	connect(this, &QSlider::valueChanged, this, &ColorSlider::updateColor);
}

void ColorSlider::disconnectSlots()
{
	disconnect(this, &QSlider::valueChanged, this, &ColorSlider::updateColor);
}

void ColorSlider::renderMap()
{
	if (!m_doc || !isVisible())
		return;

	QRect rect;
	QPoint pEnd;
	QColor color = ScColorEngine::getDisplayColor(m_color, m_doc);

	if (orientation() == Qt::Horizontal)
	{
		rect = QRect(0, 0, width(), 1);
		pEnd = rect.topRight();
	}
	else
	{
		rect = QRect(0, 0, 1, height());
		pEnd = rect.bottomLeft();
	}

	m_background = QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
	m_background.fill(Qt::transparent);

	QPainter p(&m_background);
	p.setPen(Qt::NoPen);

	// For opacity and shade we can use simple linear gradients
	if (m_mode == Mode::Opacity || m_mode == Mode::Shade)
	{
		QLinearGradient grad = QLinearGradient(QPoint(0, 0), pEnd);

		switch (m_mode)
		{
		case Mode::Opacity:
		{
			QColor colorA = color;
			colorA.setAlphaF(0);
			grad.setColorAt(0, colorA);
			grad.setColorAt(1, color);
			p.setBrush(grad);
			p.drawRect(m_background.rect());
			break;
		}
		default:
		case Mode::Shade:
		{
			grad.setColorAt(0, Qt::white);
			grad.setColorAt(1, color);
			p.setBrush(grad);
			p.drawRect(m_background.rect());
			break;
		}
		}
	}
	else
	{
		double l, a, b;
		int c, m, y, k, red, green, blue;
		int length = qMax(rect.width(), rect.height());
		int step = (length - RADIUS * 2) / m_colorSteps;

		// CMYK
		CMYKColor cmyk;
		ScColorEngine::getCMYKValues(m_color, m_doc, cmyk);
		cmyk.getValues(c, m, y, k);

		// RGB
		RGBColor rgb;
		ScColorEngine::getRGBValues(m_color, m_doc, rgb);
		rgb.getValues(red, green, blue);

		// Lab / HLC
		ScColor sColor = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		sColor.getLab(&l, &a, &b);
		QLineF lin(QPointF(0.0, 0.0), QPointF(a, b));
		double H = lin.angle();
		double C = lin.length();

		for (int x = 0; x < length; x += step)
		{
			switch (m_mode)
			{
			default:
			case Mode::CMYKCyan:
			{
				int val = qRound(((orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length) * 255);
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(val, m, y, k), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(val, 0, 0, 0), m_doc);
			}
			break;
			case Mode::CMYKMagenta:
			{
				int val = qRound(((orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length) * 255);
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(c, val, y, k), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(0, val, 0, 0), m_doc);
			}
			break;
			case Mode::CMYKYellow:
			{
				int val = qRound(((orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length) * 255);
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(c, m, val, k), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(0, 0, val, 0), m_doc);
			}
			break;
			case Mode::CMYKKey:
			{
				int val = qRound(((orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length) * 255);
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(c, m, y, val), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(0, 0, 0, val), m_doc);
			}
			break;
			case Mode::HLCHue:
			{
				qreal val = (orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length;
				lin = QLineF::fromPolar(C, -360 * val);
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(l, lin.p2().x(), lin.p2().y()), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(100.0, lin.p2().x(), lin.p2().y()), m_doc);
			}
			break;
			case Mode::HLCChroma:
			{
				qreal val = (orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length;
				lin = QLineF::fromPolar(128 * val, H);
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(l, lin.p2().x(), lin.p2().y()), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(100.0, lin.p2().x(), lin.p2().y()), m_doc);
			}
			break;
			case Mode::HLCLightness:
			case Mode::LabLightness:
			{
				qreal val = ((orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length) * 100.0;
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(val, a, b), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(val, 0.0, 0.0), m_doc);
			}
			break;
			case Mode::LabA:
			{
				qreal val = ((orientation() == Qt::Horizontal) ? -(0.5 - qreal(x) / length) : (0.5 - qreal(x) / length)) * 255.;
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(l, val, b), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(0.0, val, 0.0), m_doc);
			}
			break;
			case Mode::LabB:
			{
				qreal val = ((orientation() == Qt::Horizontal) ? -(0.5 - qreal(x) / length) : (0.5 - qreal(x) / length)) * 255.;
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(l, a, val), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(0.0, 0.0, val), m_doc);
			}
			break;
			case Mode::RGBRed:
			case Mode::RGBWebRed:
			{
				int val = qRound(((orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length) * 255);
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(val, green, blue), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(val, 0, 0), m_doc);
			}
			break;
			case Mode::RGBGreen:
			case Mode::RGBWebGreen:
			{
				int val = qRound(((orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length) * 255);
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(red, val, blue), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(0, val, 0), m_doc);
			}
			break;
			case Mode::RGBBlue:
			case Mode::RGBWebBlue:
			{
				int val = qRound(((orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length) * 255);
				if (isDynamic())
					color = ScColorEngine::getDisplayColorGC(ScColor(red, green, val), m_doc);
				else
					color = ScColorEngine::getDisplayColorGC(ScColor(0, 0, val), m_doc);
			}
			break;
			case Mode::HSVHue:
			{
				qreal val = (orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length;
				if (isDynamic())
					color.setHsvF(val, color.saturationF(), color.valueF());
				else
					color.setHsvF(val, 1.0, 0.5);
			}
			break;
			case Mode::HSVSaturation:
			{
				qreal val = (orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length;
				if (isDynamic())
					color.setHsvF(color.hueF(), val, color.valueF());
				else
					color.setHsvF(0.0, val, 0.5);
			}
			break;
			case Mode::HSVValue:
			{
				qreal val = (orientation() == Qt::Horizontal) ? qreal(x) / length : 1. - qreal(x) / length;
				if (isDynamic())
					color.setHsvF(color.hueF(), color.saturationF(), val);
				else
					color.setHsvF(0.0, 1.0, val);
			}
			break;
			}

			int w = (x == 0) ? step + RADIUS : step;
			int o = (x == 0) ? 0 : RADIUS;

			p.setBrush(color);
			p.drawRect((orientation() == Qt::Horizontal) ? QRect(o + x - step / 2, 0, w, 1) : QRect(0, o + x - step / 2, 1, w));
		}
	}

	p.end();
}

void ColorSlider::updateValues()
{
	if (!m_doc)
		return;

	disconnectSlots();

	QSignalBlocker blocker(this);

	double v1, v2, v3, v4;
	int multplier = 1;

	switch (m_mode)
	{
	case Mode::Opacity:
		setValue(m_opacity);
		break;
	case Mode::Shade:
		setValue(m_shade);
		break;
	case Mode::CMYKCyan:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelCMYK);
		m_color.getCMYK(&v1, &v2, &v3, &v4);
		setValue(v1 * maximum());
		break;
	case Mode::CMYKMagenta:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelCMYK);
		m_color.getCMYK(&v1, &v2, &v3, &v4);
		setValue(v2 * maximum());
		break;
	case Mode::CMYKYellow:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelCMYK);
		m_color.getCMYK(&v1, &v2, &v3, &v4);
		setValue(v3 * maximum());
		break;
	case Mode::CMYKKey:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelCMYK);
		m_color.getCMYK(&v1, &v2, &v3, &v4);
		setValue(v4 * maximum());
		break;
	case Mode::HSVHue:
		setValue(ScColorEngine::getRGBColor(m_color, m_doc).hueF() * maximum());
		break;
	case Mode::HSVSaturation:
		setValue(ScColorEngine::getRGBColor(m_color, m_doc).saturationF() * maximum());
		break;
	case Mode::HSVValue:
		setValue(ScColorEngine::getRGBColor(m_color, m_doc).valueF() * maximum());
		break;
	case Mode::HLCHue:
	{
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		QLineF lin(QPointF(0.0, 0.0), QPointF(v2, v3));
		setValue(360 - lin.angle());
		break;
	}
	case Mode::HLCChroma:
	{
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		QLineF lin(QPointF(0.0, 0.0), QPointF(v2, v3));
		setValue(lin.length());
		break;
	}
	case Mode::HLCLightness:
	case Mode::LabLightness:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		setValue(v1);
		break;
	case Mode::LabA:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		setValue(v2);
		break;
	case Mode::LabB:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelLab);
		m_color.getLab(&v1, &v2, &v3);
		setValue(v3);
		break;
	case Mode::RGBWebRed:
	case Mode::RGBRed:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelRGB);
		m_color.getRGB(&v1, &v2, &v3);
		setValue(v1 * maximum());
		break;
	case Mode::RGBWebGreen:
	case Mode::RGBGreen:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelRGB);
		m_color.getRGB(&v1, &v2, &v3);
		setValue(v2 * maximum());
		break;
	case Mode::RGBWebBlue:
	case Mode::RGBBlue:
		m_color = ScColorEngine::convertToModel(m_color, m_doc, colorModelRGB);
		m_color.getRGB(&v1, &v2, &v3);
		setValue(v3 * maximum());
		break;
	}

	connectSlots();
}

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void ColorSlider::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e)

	QStyleOptionSlider opt;
	initStyleOption(&opt);

	opt.subControls = QStyle::SC_SliderGroove;
	if (tickPosition() != NoTicks)
		opt.subControls |= QStyle::SC_SliderTickmarks;
	if (isSliderDown())
		opt.state |= QStyle::State_Sunken;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	// Slider Area
	QRectF sliderArea = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
	QRectF borderArea;
	qreal radius;

	if (orientation() == Qt::Horizontal)
	{
		radius = sliderArea.height() / 2;
		sliderArea.adjust(0.5, 0., -0.5, 0.);
		borderArea = sliderArea.adjusted(0, -0.5, 0, 0.5);
	}
	else
	{
		radius = sliderArea.width() / 2;
		sliderArea.adjust(0, 0.5, 0., -0.5);
		borderArea = sliderArea.adjusted(-0.5, 0, 0.5, 0);
	}

	QPainterPath border;
	border.addRoundedRect(borderArea, radius + .5, radius + .5);
	QPainterPath clip;
	clip.addRoundedRect(sliderArea, radius, radius);

	// Draw Slider Gradients
	painter.setClipPath(clip);

	if (m_hasColor)
	{
		switch (m_mode)
		{
		case Mode::Opacity:
			renderCheckerPattern(&painter, sliderArea);
			[[fallthrough]];
		default:
			painter.drawImage(sliderArea, m_background);
			break;
		}
	}

	painter.setClipping(false);

	// Draw Slider Border
	painter.setPen(palette().color(QPalette::Mid));
	painter.setBrush(Qt::NoBrush);
	painter.drawPath(border);
	painter.setPen(Qt::NoPen);

	// Draw Ticker
	if (orientation() == Qt::Horizontal && this->tickPosition() != QSlider::NoTicks)
	{
		QFont font = this->font();
		font.setPixelSize(8);
		painter.setFont(font);

		int interval = 4;
		int max = 100;
		int min = 0;

		switch (m_mode)
		{
		case Mode::HSVHue:
		case Mode::HLCHue:
			interval = 6;
			max = 360;
			break;
		case Mode::LabA:
		case Mode::LabB:
			interval = 4;
			min = 128;
			max = 256;
			break;
		case Mode::HLCChroma:
			interval = 4;
			max = 128;
			break;
		case Mode::RGBRed:
		case Mode::RGBGreen:
		case Mode::RGBBlue:
			interval = 2;
			max = 255;
			break;
		case Mode::RGBWebRed:
		case Mode::RGBWebGreen:
		case Mode::RGBWebBlue:
			interval = 5;
			max = 255;
			break;
		default:
			break;
		}

		double ratio = (sliderArea.width() - 2 - (2 * RADIUS)) / interval;

		for (int x = 0; x <= interval; x++)
		{
			QRect tick(x * ratio + RADIUS + 2, sliderArea.top() - 3, 1, 2);
			QString label = QString::number((x * max / interval) - min);
			int offset = QFontMetrics(painter.font()).size(Qt::TextSingleLine, label).width() / 2;

			painter.setPen(palette().color(QPalette::WindowText));
			painter.drawText(qBound(0, tick.left() - offset, width()), sliderArea.top() - 4, label);
			painter.fillRect(tick, palette().color(QPalette::WindowText));
		}
	}

	// Draw Handle
	QRect handle = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
	drawCircularHandle(&painter, handle.center() + QPointF(0.5, 0.5), RADIUS * 2, Qt::NoBrush, isEnabled());

	painter.end();
}

void ColorSlider::resizeEvent(QResizeEvent *e)
{
	QWidget::resizeEvent(e);

	if (e->oldSize() != e->size())
		renderMap();
}

void ColorSlider::showEvent(QShowEvent *e)
{
	QWidget::showEvent(e);
	renderMap();
}

void ColorSlider::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton)
	{
		switch (m_mode)
		{
		// Override slider movement for custom steps
		case Mode::RGBWebBlue:
		case Mode::RGBWebGreen:
		case Mode::RGBWebRed:
			setSliderPosition(pixelPosToRangeValue(e->pos()));
			break;
		default:
			QSlider::mouseMoveEvent(e);
			break;
		}

		return;
	}

	QSlider::mouseMoveEvent(e);
}

int ColorSlider::pixelPosToRangeValue(QPoint pt) const
{
	int pos, sliderMin, sliderMax;
	if (orientation() == Qt::Horizontal)
	{
		pos = pt.x();
		sliderMin = rect().x();
		sliderMax = rect().right() - RADIUS * 2 + 1;
	}
	else
	{
		pos = pt.y();
		sliderMin = rect().y();
		sliderMax = rect().bottom() - RADIUS * 2 + 1;
	}

	return roundToStep(QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin, sliderMax - sliderMin));
}
