#include "color_button.h"

#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include "util_gui.h"
#include "popup_menu.h"
#include "floatingwindow.h"
#include "scribusdoc.h"
#include "sccolorengine.h"
#include "manager/widget_manager.h"
#include "ui/colorpicker/colorpicker.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */
ColorButton::ColorButton(QWidget *parent) : QToolButton(parent)
{
	setFixedSize(26,26);
}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */


void ColorButton::setHasDot(bool enabled)
{
	m_hasDot = enabled;
	update();
}

bool ColorButton::hasDot() const
{
	return m_hasDot;
}

QSize ColorButton::circleSize() const
{
	int smallestSize = qMin(height(), width());

	return QSize(smallestSize, smallestSize);
}

QSize ColorButton::dotSize() const
{
	int smallestSize = 12;

	return QSize(smallestSize, smallestSize);
}

void ColorButton::setMenuContextType(MenuContextType type)
{
	m_contextType = type;

	switch (m_contextType)
	{
	case MenuContextType::None:
		break;
	case MenuContextType::Floating:
		setPopupMode(QToolButton::InstantPopup);
		setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
		setMenu(nullptr);
		connect(this, &QToolButton::pressed, this, &ColorButton::toggleFloatingContext);
		break;
	case MenuContextType::List:
		stickyWidget->adjustSize();
		popupmenu = new PopupMenu(stickyWidget);
		setPopupMode(QToolButton::InstantPopup);
		setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
		setMenu(popupmenu);
		disconnect(this, &QToolButton::pressed, this, &ColorButton::toggleFloatingContext);
		break;
	}
}

void ColorButton::setContextWidget(QWidget *widget)
{
	stickyWidget = widget;
	setMenuContextType(MenuContextType::List);

}

QWidget *ColorButton::contextWidget()
{
	if (m_contextType == MenuContextType::List)
		return stickyWidget;
	else
		return nullptr;
}

void ColorButton::setBrush(const QBrush& brush)
{
	m_brush = brush;
	update();
}

QBrush ColorButton::brush() const
{
	if (isEnabled())
		return m_brush;
	else
		return QBrush(palette().color(QPalette::Window));
}


void ColorButton::setDotBrush(const QBrush& brush)
{
	m_dotBrush = brush;
	if (m_hasDot)
		update();
}

QBrush ColorButton::dotBrush() const
{
	if (isEnabled())
		return m_dotBrush;
	else
		return QBrush(palette().color(QPalette::Window));
}

void ColorButton::setGeneral(int overprint)
{
	CPGeneralData i;
	i.overprint = overprint;

	setGeneralData(i);
}

void ColorButton::setColor(QString colorName, double shade, double opacity)
{
	CPColorData c;
	c.Name = colorName;
	c.Shade = shade;
	c.Opacity = opacity;

	setColorData(c);

}

QColor ColorButton::color() const
{
	if (!m_doc || colorName() == CommonStrings::tr_NoneColor || colorName() == CommonStrings::None)
		return QColor();

	ScColor sColor(0, 0, 0);

	if (m_doc->PageColors.contains(colorName()))
		sColor = m_doc->PageColors.value(colorName());

	QColor qColor = ScColorEngine::getDisplayColor(sColor, m_doc, colorData().Shade);
	qColor.setAlphaF(1 - colorData().Opacity);

	return qColor;
}

QString ColorButton::colorName() const
{
	return colorData().Name;
}

void ColorButton::setGradient(QString gradientName, const VGradient& gradient, VGradient::VGradientRepeatMethod repeatMethod,
							  QString color1, QString color2, QString color3, QString color4,
							  double color1Shade, double color2Shade, double color3Shade, double color4Shade,
							  double color1Alpha, double color2Alpha, double color3Alpha, double color4Alpha
							  )
{
	CPGradientData g;
	g.Gradient = gradient;
	g.Name = gradientName;
	g.Type = m_type;
	g.repeatMethod = repeatMethod;
	g.Color1Name = color1;
	g.Color2Name = color2;
	g.Color3Name = color3;
	g.Color4Name = color4;
	g.Color1Shade = color1Shade;
	g.Color2Shade = color2Shade;
	g.Color3Shade = color3Shade;
	g.Color4Shade = color4Shade;
	g.Color1Alpha = color1Alpha;
	g.Color2Alpha = color2Alpha;
	g.Color3Alpha = color3Alpha;
	g.Color4Alpha = color4Alpha;

	setGradientData(g);
}

VGradient ColorButton::gradient() const
{
	return gradientData().Gradient;
}

QString ColorButton::gradientName() const
{
	return gradientData().Name;
}

void ColorButton::setGradientMesh(QString colorName, double shade, double opacity)
{
	CPColorData c;
	c.Name = colorName;
	c.Shade = shade;
	c.Opacity = opacity;

	setGradientMeshData(c);
}

void ColorButton::setGradientVector(double StartX, double StartY, double EndX, double EndY, double FocalX, double FocalY, double Scale, double Skew,
									double C1X, double C1Y, double C2X, double C2Y, double C3X, double C3Y, double C4X, double C4Y, double C5X, double C5Y)
{
	CPGradientVectorData gvd;
	gvd.StartX = StartX;
	gvd.StartY = StartY;
	gvd.EndX = EndX;
	gvd.EndY = EndY;
	gvd.FocalX = FocalX;
	gvd.FocalY = FocalY;
	gvd.Scale = Scale;
	gvd.Skew = Skew;
	gvd.C1X = C1X;
	gvd.C1Y = C1Y;
	gvd.C2X = C2X;
	gvd.C2Y = C2Y;
	gvd.C3X = C3X;
	gvd.C3Y = C3Y;
	gvd.C4X = C4X;
	gvd.C4Y = C4Y;
	gvd.C5X = C5X;
	gvd.C5Y = C5Y;

	setGradientVectorData(gvd);
}

void ColorButton::setPattern(QString patternName, double offsetX, double offsetY, double scaleX, double scaleY, double skewX, double skewY,
							 double angle, double spacing, bool mirrorX, bool mirrorY, bool onPath
							 )
{
	CPPatternData p;
	p.Name = patternName;
	p.OffsetX = offsetX;
	p.OffsetY = offsetY;
	p.ScaleX = scaleX;
	p.ScaleY = scaleY;
	p.SkewX = skewX;
	p.SkewY = skewY;
	p.Spacing = spacing;
	p.Angle = angle;
	p.MirrorX = mirrorX;
	p.MirrorY = mirrorY;
	p.OnPath = onPath;

	setPatternData(p);
}

ScPattern ColorButton::pattern() const
{
	if (!m_doc)
		return ScPattern();

	return m_doc->docPatterns.find(patternData().Name).value();
}

QString ColorButton::patternName() const
{
	return patternData().Name;
}

void ColorButton::setHatch(int type, double distance, double angle, bool hasBackground, QString backgroundColor, QString foregroundColor)
{
	CPHatchData h;
	h.Type = type;
	h.Angle = angle;
	h.Distance = distance;
	h.HasBackgroundColor = hasBackground;
	h.ColorBackground = backgroundColor;
	h.ColorForeground = foregroundColor;

	setHatchData(h);

}

void ColorButton::setModeByType(int type)
{
	if (isMask())
	{
		switch(type)
		{
		case GradMask_None:
			m_mode = Mode::Solid;
			break;
		case GradMask_Linear:
		case GradMask_LinearLumAlpha:
		case GradMask_Radial:
		case GradMask_RadialLumAlpha:
			m_mode = Mode::Gradient;
			break;
		case GradMask_Pattern:
		case GradMask_PatternInverted:
		case GradMask_PatternLumAlpha:
		case GradMask_PatternLumAlphaInverted:
			m_mode = Mode::Pattern;
			break;
		}
	}
	else
	{
		switch(type)
		{
		case Gradient_None:
			m_mode = Mode::Solid;
			break;
		case Gradient_Linear:
		case Gradient_Conical:
		case Gradient_Radial:
		case Gradient_Diamond:
		case Gradient_4Colors:
		case Gradient_Mesh:
		case Gradient_PatchMesh:
			m_mode = Mode::Gradient;
			break;
		case Gradient_Pattern:
			m_mode = Mode::Pattern;
			break;
		case Gradient_Hatch:
			m_mode = Mode::Hatch;
			break;
		}
	}
}

QColor ColorButton::colorFromName(QString colorName, double shade) const
{
	ScColor sColor(0, 0, 0);
	shade = qBound(0.0, shade, 100.0);

	if (m_doc->PageColors.contains(colorName))
		sColor = m_doc->PageColors.value(colorName);

	return ScColorEngine::getDisplayColor(sColor, m_doc, shade);
}

Context ColorButton::context() const
{
	return m_context;
}

void ColorButton::setContext(Context config)
{
	m_context = config;
}

QBrush ColorButton::renderBrush() const
{
	switch(m_mode)
	{
	default:
	case Mode::Solid:
		return brushSolid();
		break;
	case Mode::Gradient:
		return brushGradient();
		break;
	case Mode::Hatch:
		return brushHatch();
		break;
	case Mode::Pattern:
		return brushPattern();
		break;
	}

	return QBrush();
}

bool ColorButton::isMask() const
{
	return (m_context == Context::FillMask || m_context == Context::LineMask) ? true : false;
}

QBrush ColorButton::brushSolid() const
{
	if (!m_doc || m_colorData.Name == CommonStrings::tr_NoneColor || m_colorData.Name == CommonStrings::None)
	{
		if (icon().isNull())
			return renderEmptyPattern(this->size(), devicePixelRatio());
		else
			return combinePixmaps(renderEmptyPattern(this->size(), devicePixelRatio()), icon().pixmap(iconSize()), devicePixelRatio(), m_onIcon, false);
	}

	ScColor sColor(0, 0, 0);

	if (m_doc->PageColors.contains(m_colorData.Name))
		sColor = m_doc->PageColors.value(m_colorData.Name);

	QColor qColorShade = ScColorEngine::getDisplayColor(sColor, m_doc, m_colorData.Shade);
	QColor qColor = ScColorEngine::getDisplayColor(sColor, m_doc, 100.0);
	QPixmap pBackground(renderColor(this->size(), devicePixelRatio(), qColor, qColorShade, 1 - m_colorData.Opacity));

	// simulate grey tone for mask color
	if (isMask())
	{
		qColor = ScColorEngine::getDisplayColor(sColor, m_doc, m_colorData.Opacity * 100.0);
		qColorShade = qColor;
		pBackground = renderColor(this->size(), devicePixelRatio(), qColor, qColor, 1);
	}

	if (icon().isNull())
		return pBackground;
	else
		return combinePixmaps(pBackground, icon().pixmap(iconSize()), devicePixelRatio(), m_onIcon, isDarkColor(qColor));

}

QBrush ColorButton::brushGradient() const
{
	if (isMask())
	{
		switch(type())
		{
		default:
		case GradMask_Linear:
		case GradMask_LinearLumAlpha:
			return QBrush(renderGradientLinear(this->size(), devicePixelRatio(), gradientData().Gradient));
			break;
		case GradMask_Radial:
		case GradMask_RadialLumAlpha:
			return QBrush(renderGradientRadial(this->size(), devicePixelRatio(), gradientData().Gradient));
			break;
		}
	}
	else
	{
		switch(type())
		{
		case Gradient_Linear:
			return QBrush(renderGradientLinear(this->size(), devicePixelRatio(), gradientData().Gradient));
			break;
		case Gradient_Radial:
			return QBrush(renderGradientRadial(this->size(), devicePixelRatio(), gradientData().Gradient));
			break;
		case Gradient_Diamond:
			return QBrush(renderGradientDiamond(this->size(), devicePixelRatio(), gradientData().Gradient));
			break;
		case Gradient_Conical:
			return QBrush(renderGradientConical(this->size(), devicePixelRatio(), gradientData().Gradient));
			break;
		case Gradient_4Colors:
		{
			QColor color1;
			QColor color2;
			QColor color3;
			QColor color4;

			if (m_gradientData.Color1Name != CommonStrings::None)
			{
				if (m_doc->PageColors.contains(m_gradientData.Color1Name))
				{
					const ScColor& col = m_doc->PageColors[m_gradientData.Color1Name];
					color1 = ScColorEngine::getShadeColorProof(col, m_doc, m_gradientData.Color1Shade);
				}
			}

			if (m_gradientData.Color2Name != CommonStrings::None)
			{
				if (m_doc->PageColors.contains(m_gradientData.Color2Name))
				{
					const ScColor& col = m_doc->PageColors[m_gradientData.Color2Name];
					color2 = ScColorEngine::getShadeColorProof(col, m_doc, m_gradientData.Color2Shade);
				}
			}

			if (m_gradientData.Color3Name != CommonStrings::None)
			{
				if (m_doc->PageColors.contains(m_gradientData.Color3Name))
				{
					const ScColor& col = m_doc->PageColors[m_gradientData.Color3Name];
					color3 = ScColorEngine::getShadeColorProof(col, m_doc, m_gradientData.Color3Shade);
				}
			}

			if (m_gradientData.Color4Name != CommonStrings::None)
			{
				if (m_doc->PageColors.contains(m_gradientData.Color4Name))
				{
					const ScColor& col = m_doc->PageColors[m_gradientData.Color4Name];
					color4 = ScColorEngine::getShadeColorProof(col, m_doc, m_gradientData.Color4Shade);
				}
			}

			return QBrush(renderGradient4Colors(this->size(), devicePixelRatio(), color1, color2, color3, color4));
		}
			break;
		case Gradient_Mesh:
			return QBrush(renderGradientMesh(this->size(), devicePixelRatio()));
			break;
		case Gradient_PatchMesh:
			return QBrush(renderGradientPatchMesh(this->size(), devicePixelRatio()));
			break;
		}
	}

	return QBrush();
}

QBrush ColorButton::brushHatch() const
{
	if (!m_doc)
		return QBrush();

	QColor fg;
	QColor bg;

	if (m_hatchData.ColorForeground != CommonStrings::None)
		fg = colorFromName(m_hatchData.ColorForeground, 100.0);

	if (m_hatchData.ColorBackground != CommonStrings::None)
		bg = colorFromName(m_hatchData.ColorBackground, 100.0);

	return renderHatch(this->size(), devicePixelRatio(),
					   m_hatchData.Type,
					   m_hatchData.Distance,
					   m_hatchData.Angle,
					   m_hatchData.HasBackgroundColor,
					   bg,
					   fg
					   );
}

QBrush ColorButton::brushPattern() const
{
	if (!m_doc || m_patternData.Name.isEmpty())
		return QBrush();

	if (!m_doc->docPatterns.contains(m_patternData.Name))
		return QBrush();

	QImage img = pattern().getPattern();
	img.setDevicePixelRatio(qApp->devicePixelRatio());

	return QBrush(img);
}

void ColorButton::setType(int type)
{
	m_type = type;
	setModeByType(type);
}

void ColorButton::setPersistentToolTip(const QString &tooltip)
{
	m_persistenToolTip = tooltip;
	buildToolTip();
}

void ColorButton::setIcon(const QIcon &icon)
{
	QToolButton::setIcon(icon);
	updatePreview();
}

void ColorButton::setDotIcon(const QIcon &icon)
{
	m_dotIcon = icon;
	update();
}

void ColorButton::setApplyColorOnIcon(bool onIcon)
{
	m_onIcon = onIcon;
	updatePreview();
}

void ColorButton::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
}

/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

void ColorButton::updatePreview()
{
	setBrush(renderBrush());
}

void ColorButton::updateFloatingContext()
{
	if (floatingWidget)
	{
		ColorPicker *colorPicker = qobject_cast<ColorPicker*>(floatingWidget->child());
		if (colorPicker && (colorPicker->colorButton() == this))
			updateColorPicker(colorPicker);
	}
}

void ColorButton::unsetDoc()
{
	setDoc(nullptr);
}

void ColorButton::toggleFloatingContext()
{
	WidgetManager &widgetManager = WidgetManager::instance();
	floatingWidget = widgetManager.colorPickerWindow();
	if (!floatingWidget)
		return;

	ColorPicker *colorPicker = qobject_cast<ColorPicker*>(floatingWidget->child());
	if (!colorPicker)
		return;

	updateColorPicker(colorPicker);

	connect(colorPicker, &ColorPicker::colorChanged, this, &ColorButton::updateColor, Qt::UniqueConnection);
	connect(colorPicker, &ColorPicker::gradientChanged, this, &ColorButton::updateGradient, Qt::UniqueConnection);
	connect(colorPicker, &ColorPicker::gradientVectorChanged, this, &ColorButton::updateGradientVector, Qt::UniqueConnection);
	connect(colorPicker, &ColorPicker::gradientMeshChanged, this, &ColorButton::updateGradientMesh, Qt::UniqueConnection);
	connect(colorPicker, &ColorPicker::hatchChanged, this, &ColorButton::updateHatch, Qt::UniqueConnection);
	connect(colorPicker, &ColorPicker::patternChanged, this, &ColorButton::updatePattern, Qt::UniqueConnection);
	connect(colorPicker, &ColorPicker::colorListChanged, this, &ColorButton::updateFloatingContext, Qt::UniqueConnection);

	floatingWidget->show(this);
}

void ColorButton::updateColor()
{
	ColorPicker * colorPicker = qobject_cast<ColorPicker*>(sender());
	if (!colorPicker || (colorPicker->colorButton() != this))
		return;

	setType(colorPicker->type());
	setGeneralData(colorPicker->generalData());
	setColorData(colorPicker->colorData());
	m_dynamicToolTip = colorPicker->toolTipText();
	buildToolTip();
	updatePreview();
	emit changed();
	emit colorChanged();
}

void ColorButton::updateGradient()
{
	ColorPicker * colorPicker = qobject_cast<ColorPicker*>(sender());
	if (!colorPicker || (colorPicker->colorButton() != this))
		return;

	setType(colorPicker->type());
	setGeneralData(colorPicker->generalData());
	setGradientData(colorPicker->gradientData());
	m_dynamicToolTip = colorPicker->toolTipText();
	buildToolTip();
	updatePreview();
	emit changed();
	emit gradientChanged();
}

void ColorButton::updateGradientVector()
{
	ColorPicker * colorPicker = qobject_cast<ColorPicker*>(sender());
	if (!colorPicker || (colorPicker->colorButton() != this))
		return;

	setType(colorPicker->type());
	setGradientVectorData(colorPicker->gradientVectorData());
	emit gradientVectorChanged();
}

void ColorButton::updateGradientMesh()
{
	ColorPicker * colorPicker = qobject_cast<ColorPicker*>(sender());
	if (!colorPicker || (colorPicker->colorButton() != this))
		return;

	setType(colorPicker->type());
	setGradientMeshData(colorPicker->gradientMeshData());
	emit gradientMeshChanged();
}

void ColorButton::updateHatch()
{
	ColorPicker * colorPicker = qobject_cast<ColorPicker*>(sender());
	if (!colorPicker || (colorPicker->colorButton() != this))
		return;

	setType(colorPicker->type());
	setGeneralData(colorPicker->generalData());
	setHatchData(colorPicker->hatchData());
	m_dynamicToolTip = colorPicker->toolTipText();
	buildToolTip();
	updatePreview();
	emit changed();
	emit hatchChanged();
}

void ColorButton::updatePattern()
{
	ColorPicker * colorPicker = qobject_cast<ColorPicker*>(sender());
	if (!colorPicker || (colorPicker->colorButton() != this))
		return;

	setType(colorPicker->type());
	setGeneralData(colorPicker->generalData());
	setPatternData(colorPicker->patternData());
	m_dynamicToolTip = colorPicker->toolTipText();
	buildToolTip();
	updatePreview();
	emit changed();
	emit patternChanged();
}

void ColorButton::updateColorPicker(ColorPicker *colorPicker)
{
	QSignalBlocker sig(colorPicker);

	colorPicker->setColorButton( this );
	colorPicker->setDoc( m_doc );
	colorPicker->setContext( context() );
	colorPicker->updateUiByType( type() );
	colorPicker->setGeneralData( generalData() );
	colorPicker->setColorData( colorData() );
	colorPicker->setGradientData( gradientData() );
	colorPicker->setGradientVectorData( gradientVectorData() );
	colorPicker->setGradientMeshData( gradientMeshData() );
	colorPicker->setHatchData( hatchData() );
	colorPicker->setPatternData( patternData() );
	colorPicker->updatePreview();
}

void ColorButton::buildToolTip()
{
	QString sb = (m_persistenToolTip.isEmpty() || m_dynamicToolTip.isEmpty()) ? "" : "<br/>";
	QString tt = "<html><head/><body><p>" + m_persistenToolTip + sb + m_dynamicToolTip + "</p></body></html>";
	setToolTip(tt);
}

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void ColorButton::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QSize bSize = circleSize();
	QSize fSize = dotSize();
	QPainterPath mask;

	int inset = 1;

	// Draw Background Dot
	QRectF bDot(rect().center() - QPointF(bSize.width() / 2. - inset - 0.5, bSize.height() / 2. - inset - 0.5), QSizeF(bSize.width() - inset, bSize.height() - inset));
	mask.addEllipse(bDot.adjusted(inset, inset, -inset, -inset));
	painter.setClipPath(mask);
	renderCheckerPattern(&painter, mask.boundingRect());
	painter.setClipping(false);

	drawCircularHandle(&painter, bDot.center(), bSize.width() - inset, brush(), isEnabled());

	// Draw Foreground Dot
	if (m_hasDot && dotIcon().isNull())
	{
		mask.clear();
		QRectF fDot(0, 0, fSize.width(), fSize.height());
		fDot.moveBottom(this->height() - .5);
		fDot.moveRight(this->width() - .5);
		mask.addEllipse(fDot.center(), fDot.width() / 2., fDot.height() / 2.);
		painter.setClipPath(mask);
		renderCheckerPattern(&painter, mask.boundingRect());
		painter.setClipping(false);

		QTransform t = dotBrush().transform();
		t.translate(fDot.x() * devicePixelRatio(), fDot.y() * devicePixelRatio());

		QBrush br = dotBrush();
		br.setTransform(t);

		drawCircularHandle(&painter, fDot.center(), fDot.width(), br, isEnabled());
	}

	// Draw Icon
	if (m_hasDot && !dotIcon().isNull())
	{
		QIcon::Mode iMode = isEnabled() ? QIcon::Normal : QIcon::Disabled;

		int w = 8;
		int h = 8;
		QPixmap pix = dotIcon().pixmap(QSize(w, h), iMode);
		QRectF fDot(rect().right() - fSize.width() + 0.5, rect().bottom() - fSize.height() + 0.5, fSize.width(), fSize.height()); // bottom right corner
		painter.setPen(QPen(palette().color(QPalette::WindowText)));
		painter.setBrush(palette().color(QPalette::Base));
		painter.drawEllipse(fDot.center(), fDot.width() / 2., fDot.height() / 2.);
		painter.drawPixmap(fDot.center() - QPointF(w/2, h/2), pix);
	}

	painter.end();
}

