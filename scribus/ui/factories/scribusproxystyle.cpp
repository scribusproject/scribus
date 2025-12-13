#include "scribusproxystyle.h"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QEvent>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QStatusBar>
#include <QStyleFactory>
#include <QStyleHints>
#include <QStyleOption>
#include <QStyleOptionDockWidget>
#include <QStyleOptionTab>
#include <QStylePainter>
#include <QToolBar>

#include "prefsmanager.h"
#include "scribus.h"
#include "ui/widgets/color_slider.h"
#include "util_gui.h"

// https://doc.qt.io/qt-6/qstyle.html#standardPalette
QPalette createLightPalette()
{
	QPalette lightPalette;
	QStyle *fusion = QStyleFactory::create("Fusion");
	if (fusion)
	{
		lightPalette = fusion->standardPalette();
		delete fusion;
	}
	return lightPalette;
}

QPalette createDarkPalette()
{
	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::black);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);
	darkPalette.setColor(QPalette::Shadow, Qt::black);

	darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
	darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

	return darkPalette;
}

ScribusProxyStyle::ScribusProxyStyle(QStyle *style) : QProxyStyle(style) {}
ScribusProxyStyle::ScribusProxyStyle(const QString &key) : QProxyStyle(key) {}

ScribusProxyStyle* ScribusProxyStyle::instance()
{
	return static_cast<ScribusProxyStyle*>(qApp->style());
}

QRect ScribusProxyStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex *option, SubControl sc, const QWidget *widget) const
{
	QRect rect = QProxyStyle::subControlRect(cc, option, sc, widget);

	switch (cc)
	{
	case QStyle::ComplexControl::CC_Slider:
	{
		const QStyleOptionSlider *soSlider = qstyleoption_cast<const QStyleOptionSlider *>(option);

		if (soSlider && widget->inherits("ColorSlider") )
		{
			QRect ret;
			bool horizontal = soSlider->orientation == Qt::Horizontal;
			int thickness = RADIUS * 2;
			int center = (horizontal) ? (soSlider->rect.height() - thickness) / 2 : (soSlider->rect.width() - thickness) / 2;
			int sliderOffset = (soSlider->tickPosition != QSlider::NoTicks) ? thickness : center;

			switch (sc)
			{
			case SC_SliderHandle:
			{
				int len = thickness + 2; //proxy()->pixelMetric(PM_SliderLength, styleOption, widget);
				int sliderPos = sliderPositionFromValue(soSlider->minimum, soSlider->maximum, soSlider->sliderPosition, (horizontal ? soSlider->rect.width() : soSlider->rect.height()) - len, soSlider->upsideDown);

				if (horizontal)
					ret.setRect(soSlider->rect.x() + sliderPos, soSlider->rect.y() + sliderOffset, len, thickness);
				else
					ret.setRect(soSlider->rect.x() + 1, soSlider->rect.y() + sliderPos, thickness, len);

				break;
			}
			case SC_SliderGroove:
				if (horizontal)
					ret.setRect(soSlider->rect.x(), soSlider->rect.y() + sliderOffset + 1, soSlider->rect.width(), thickness - 3);
				else
					ret.setRect(soSlider->rect.x() + 2, soSlider->rect.y(), thickness - 3, soSlider->rect.height());

				break;
			case SC_SliderTickmarks:
			{
				int tickOffset = (soSlider->tickPosition != QSlider::NoTicks) ? thickness : 0;

				if (soSlider->tickPosition != QSlider::NoTicks)
				{
					if (horizontal)
						ret.setRect(soSlider->rect.x() + 1, soSlider->rect.y(), soSlider->rect.width() - 3, thickness);
					else
						ret.setRect(soSlider->rect.x() + tickOffset, soSlider->rect.y() + 1, thickness, soSlider->rect.height() - 3);
				}
				break;
			}
			default:
				break;
			}

			return visualRect(soSlider->direction, soSlider->rect, ret);
		}
	}
	break;
	default:
		break;
	}

	return rect;
}

void ScribusProxyStyle::drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *painter, const QWidget *widget) const
{
	switch (cc)
	{
	case QStyle::ComplexControl::CC_Slider:
	{
		const ColorSlider *wdg = qobject_cast<const ColorSlider *>(widget);
		const QStyleOptionSlider *soSlider = qstyleoption_cast<const QStyleOptionSlider *>(opt);

		if (soSlider && wdg)
		{
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing, true);

			QRect groove = subControlRect(CC_Slider, soSlider, SC_SliderGroove, widget);
			QRect handle = subControlRect(CC_Slider, soSlider, SC_SliderHandle, widget);
			QRect tickmarks = subControlRect(CC_Slider, soSlider, SC_SliderTickmarks, widget);

			// Draw Slider
			if ((soSlider->subControls & SC_SliderGroove) && groove.isValid())
			{
				double radius = (soSlider->orientation == Qt::Horizontal) ? groove.height() / 2 + 0.5 : groove.width() / 2 + 0.5;

				QPainterPath border;
				border.addRoundedRect(groove.toRectF().adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);

				// Draw Slider Gradients
				painter->setClipPath(border);
				if (wdg->backgroundImage().isNull() || !widget->isEnabled())
					painter->fillRect(groove, widget->palette().color(QPalette::Window));
				else
				{
					renderCheckerPattern(painter, groove);
					painter->drawImage(groove, wdg->backgroundImage());
				}
				painter->setClipping(false);

				// Draw Slider Border
				painter->setPen(widget->palette().color(QPalette::Mid));
				painter->setBrush(Qt::NoBrush);
				painter->drawPath(border);
				painter->setPen(Qt::NoPen);
			}

			// Draw Handle
			if ((soSlider->subControls & SC_SliderHandle) && handle.isValid())
				drawCircularHandle(painter, handle.center() + QPointF(0.5, 0.5), RADIUS * 2, Qt::NoBrush, widget->isEnabled());

			// Draw Tickmarks
			if ((soSlider->subControls & SC_SliderTickmarks) && tickmarks.isValid() && soSlider->tickInterval > 0)
			{
				QFont font = widget->font();
				font.setPixelSize(8);
				painter->setFont(font);

				if (soSlider->orientation == Qt::Horizontal)
				{
					double ratio = double(tickmarks.width() - (2 * RADIUS)) / double(soSlider->tickInterval);

					for (int x = 0; x <= soSlider->tickInterval; x++)
					{
						QRect tick(double(x) * ratio + RADIUS + 1, tickmarks.bottom() - 3, 1, 2);
						QString label = QString::number((x * soSlider->maximum / soSlider->tickInterval) - abs(soSlider->minimum));
						int offset = QFontMetrics(painter->font()).size(Qt::TextSingleLine, label).width() / 2;

						painter->setPen(widget->palette().color(QPalette::WindowText));
						painter->drawText(qBound(0, tick.left() - offset, tickmarks.width()), tickmarks.bottom() - 4, label);
						painter->fillRect(tick, widget->palette().color(QPalette::WindowText));
					}
				}
				else
				{
					double ratio = double(tickmarks.height() - (2 * RADIUS)) / double(soSlider->tickInterval);

					for (int y = 0; y <= soSlider->tickInterval; y++)
					{
						QRect tick(tickmarks.left() + 2, double(y) * ratio + RADIUS + 1, 2, 1);
						painter->fillRect(tick, widget->palette().color(QPalette::WindowText));
					}
				}
			}

			painter->restore();
			return;
		}
	}
		break;
	default:
		break;
	}

	QProxyStyle::drawComplexControl(cc, opt, painter, widget);
}

bool ScribusProxyStyle::eventFilter(QObject *object, QEvent *event)
{
	if (object == qApp && event->type() == QEvent::ThemeChange)
	{
		if (PrefsManager::instance().appPrefs.uiPrefs.stylePalette == "auto" && !blockRefresh)
			setApplicationTheme(ScribusProxyStyle::ApplicationTheme::System);

		return true;
	}

	return QObject::eventFilter(object, event);
}

void ScribusProxyStyle::setBaseStyleName(const QString &styleName)
{
	blockRefresh = true;

	QStyle *oldStyle = baseStyle();
	QStyle *newStyle = QStyleFactory::create(styleName);

	// QProxyStyle takes ownership
	setBaseStyle(newStyle);

	if (oldStyle) {
		oldStyle->deleteLater();
	}

	blockRefresh = false;
}


void ScribusProxyStyle::setApplicationTheme(ApplicationTheme theme)
{
#if (QT_VERSION >= QT_VERSION_CHECK(6, 8, 0))
	blockRefresh = true;

	// For Linux exception see bugreport: https://bugreports.qt.io/browse/QTBUG-132929

	switch (theme)
	{
	case ApplicationTheme::System:
	{
		qApp->styleHints()->unsetColorScheme();
#if (defined Q_OS_LINUX)
		qApp->setPalette(baseStyle()->standardPalette());
#endif
		break;
	}
	case ApplicationTheme::Light:
		qApp->styleHints()->setColorScheme(Qt::ColorScheme::Light);
#if (defined Q_OS_LINUX)
		qApp->setPalette(createLightPalette());
#endif
		break;

	case ApplicationTheme::Dark:
		qApp->styleHints()->setColorScheme(Qt::ColorScheme::Dark);
#if (defined Q_OS_LINUX)
		qApp->setPalette(createDarkPalette());
#endif
		break;
	}

	blockRefresh = false;
#endif
}
