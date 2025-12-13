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
