/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2005 by Craig Bradney                                   *
 *   cbradney@scribus.info                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include <QApplication>
#include <QCloseEvent>
#include <QHideEvent>
#include <QMainWindow>
#include <QPoint>
#include <QScreen>

#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribuscore.h"
#include "scdockpalette.h"
#include "util.h"

ScDockPalette::ScDockPalette(QWidget * parent, const QString& prefsContext, Qt::WindowFlags f)
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
	: QDockWidget(parent, f | Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
#else
	: QDockWidget(parent, f | Qt::Tool | Qt::CustomizeWindowHint)
#endif
{
	if (PrefsManager::instance().appPrefs.uiPrefs.useSmallWidgets)
	{
		setStyleSheet("	QToolButton { margin: 1px; padding: 0px; font-size: 10px; } \
						QToolButton:pressed { padding-top: 2px; padding-left: 2px } \
						QPushButton { margin: 1px; padding: 0px; font-size: 10px; } \
						QPushButton:pressed { padding-top: 2px; padding-left: 2px } \
						QRadioButton, QComboBox, QLineEdit \
							QListView, QLabel { margin:1px; padding: 0px; font-size: 10px; } \
						QCheckBox, QSpinBox, QDoubleSpinBox \
							{ margin:1px; padding: 0px; font-size: 10px; } \
						QTabWidget, QTabBar, QTableView, QGroupBox, QTreeView \
							{ font-size: 10px ; } \
						QToolBox::tab { font-size: 10px; padding: 0px; margin: 0px; } \
			  		");
	}
	m_originalParent = parent;
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setWindowIcon(IconManager::instance().loadPixmap("AppIcon.png"));
	setPrefsContext(prefsContext);
	setObjectName(prefsContext);
	connect(&PrefsManager::instance(), SIGNAL(prefsChanged()), this, SLOT(setFontSize()));
}

void ScDockPalette::setPrefsContext(const QString& context)
{
	if (m_prefsContextName.isEmpty())
	{
		m_prefsContextName=context;
		if (!m_prefsContextName.isEmpty())
		{
			m_palettePrefs = PrefsManager::instance().prefsFile->getContext(m_prefsContextName);
			if (m_palettePrefs)
				m_visibleOnStartup = m_palettePrefs->getBool("visible");
		}
		else
			m_palettePrefs = nullptr;
	}
}

void ScDockPalette::startup()
{
	setFontSize();
	setVisible(m_visibleOnStartup);
	emit paletteShown(m_visibleOnStartup);
}

void ScDockPalette::setPaletteShown(bool visible)
{
	storeVisibility(visible);
	storeDockState();
	if (!visible)
		hide();
	else if (!isVisible())
	{
		show();
		activateWindow();
	}
}

void ScDockPalette::setFontSize()
{
	QFont newfont(font());
	newfont.setPointSize(PrefsManager::instance().appPrefs.uiPrefs.paletteFontSize);
	setFont(newfont);
}

void ScDockPalette::closeEvent(QCloseEvent *closeEvent)
{
	emit paletteShown(false);
	closeEvent->ignore();
	hide();
}

void ScDockPalette::hideEvent(QHideEvent* hideEvent)
{
	storePosition();
	storeSize();
	storeDockState();
	QDockWidget::hideEvent(hideEvent);
}

void ScDockPalette::showEvent(QShowEvent *showEvent)
{
//#13978 related, do not need this: #if QT_VERSION < 0x050600
	// According to Qt doc, non-spontaneous show events are sent to widgets
	// immediately before they are shown. We want to restore geometry for those
	// events as spontaneous events are delivered after dialog has been shown
	if (m_palettePrefs && !showEvent->spontaneous() && isFloating())
	{
		QScreen *s = ScCore->primaryMainWindow()->screen();
		QSize scrSize(s->size());
		if (m_palettePrefs->contains("left"))
		{
			QRect scr = s->availableGeometry();
			// all palettes should have enough room for 3x3 min widgets
			int vwidth  = qMin(qMax(0, m_palettePrefs->getInt("width")), scrSize.width());
			int vheight = qMin(qMax(0, m_palettePrefs->getInt("height")), scrSize.height());
			// palettes should not use too much screen space
			if (vwidth > scrSize.width() / 3 && vheight > scrSize.height()/3)
				vwidth = scrSize.width() / 3;
			// and should be partly visible
			int vleft   = qMin(qMax(scr.left() - vwidth, m_palettePrefs->getInt("left")), scr.right());
			int vtop = qMin(m_palettePrefs->getInt("top"), scrSize.height());
#if defined(Q_OS_MACOS) || defined(_WIN32)
			// on Mac and Windows you're dead if the titlebar is not on screen
			vtop = qMax(64, vtop);
#else
			vtop = qMax(-vheight, vtop);
#endif
			// Check values against current screen size
			if (vleft <= scr.left())
				vleft = scr.left();
			if (vleft >= scr.right())
				vleft = scr.left();
			if (vtop >= scr.bottom())
				vtop = 64;
			if (vtop <= scr.top())
				vtop = scr.top();
			if (vwidth >= scr.width())
				vwidth = qMax(0, scr.width() - vleft);
			if ( vheight >= scr.height() )
				vheight = qMax(0, scr.height() - vtop);
//			qDebug() << QString("root %1x%2 %7 palette %3x%4 @ (%5,%6)").arg(d->width()).arg(d->height())
//				.arg(vwidth).arg(vheight).arg(vleft).arg(vtop).arg(name());
//			setGeometry(vleft, vtop, vwidth, vheight);
			resize(vwidth, vheight);
			move(vleft, vtop);
		}
		storeDockState();
		storeVisibility(true);
	}
//#13978 related, do not need this: #endif
	QDockWidget::showEvent(showEvent);
}

void ScDockPalette::hide()
{
	if (!isVisible())
		return;
	storePosition();
	storeSize();
	storeDockState();
	QDockWidget::hide();
}

void ScDockPalette::storePosition()
{
	if (!m_palettePrefs)
		return;
	QPoint geo = pos();
	m_palettePrefs->set("left", geo.x());
	m_palettePrefs->set("top", geo.y());
}

void ScDockPalette::storePosition(int newX, int newY)
{
	if (!m_palettePrefs)
		return;
	m_palettePrefs->set("left", newX);
	m_palettePrefs->set("top", newY);
}

void ScDockPalette::storeSize()
{
	if (m_palettePrefs)
		return;
	m_palettePrefs->set("width", width());
	m_palettePrefs->set("height", height());
}

void ScDockPalette::storeVisibility(bool vis)
{
	if (m_palettePrefs)
		m_palettePrefs->set("visible", vis);
}

void ScDockPalette::storeDockState()
{
	if (!m_palettePrefs)
		return;
	m_palettePrefs->set("floating", isFloating());
	Qt::DockWidgetArea area = Qt::NoDockWidgetArea;
	QMainWindow* mainWindow = qobject_cast<QMainWindow*>(parent());
	if (mainWindow)
		area = mainWindow->dockWidgetArea(this);
	m_palettePrefs->set("area", (int) area);
}

