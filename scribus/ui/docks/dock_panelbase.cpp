/***************************************************************************
 *   Copyright (C) 2023 by Martin Reininger                                *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "dock_panelbase.h"

#include <QApplication>

#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"

DockPanelBase::DockPanelBase(const QString &title, QIcon icon, QWidget *parent)
	: DockPanelBase(title, parent)
{
	this->setIcon(icon);
}

DockPanelBase::DockPanelBase(const QString &title, QWidget *parent)
	: CDockWidget(title, parent)
{
	if (PrefsManager::instance().appPrefs.uiPrefs.useSmallWidgets) {
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
	//	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setWindowIcon(IconManager::instance().loadPixmap("AppIcon.png"));
	setPrefsContext(title);
	setObjectName(title);
	connect(&PrefsManager::instance(), SIGNAL(prefsChanged()), this, SLOT(setFontSize()));
}

void DockPanelBase::setWidget(QWidget *widget)
{
	ads::CDockWidget::setWidget(widget);
}

void DockPanelBase::setPrefsContext(const QString &context)
{
	if (!m_prefsContextName.isEmpty())
		return;
	m_prefsContextName = context;
	if (!m_prefsContextName.isEmpty())
		m_palettePrefs = PrefsManager::instance().prefsFile->getContext(m_prefsContextName);
	else
		m_palettePrefs = nullptr;
}

void DockPanelBase::startup()
{
	setFontSize();
}

void DockPanelBase::setFontSize()
{
	QFont newfont(font());
	newfont.setPointSize(PrefsManager::instance().appPrefs.uiPrefs.paletteFontSize);
	setFont(newfont);
}
