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
#include "scribusapp.h"
#include "third_party/Qt-Advanced-Docking-System/src/DockWidgetTab.h"

DockPanelBase::DockPanelBase(const QString &title, QString iconName, QWidget *parent)
	: CDockWidget(title, parent)
{
	/*if (PrefsManager::instance().appPrefs.uiPrefs.useSmallWidgets) {
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
	}*/

	m_iconName = iconName;

	iconSetChange();

	setPrefsContext(title);
	setObjectName(title);
	connect(&PrefsManager::instance(), SIGNAL(prefsChanged()), this, SLOT(setFontSize()));
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

DockPanelBase::DockPanelBase(const QString &title, QWidget *parent)
	: DockPanelBase(title, QString(), parent){}

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
	const int oldPointSize = font().pointSize();
	const int newPointSize = PrefsManager::instance().appPrefs.uiPrefs.paletteFontSize;

	QFont newfont(font());
	newfont.setPointSize(newPointSize);
	setFont(newfont);
	if (auto *tab = tabWidget())
	{
		tab->setFont(newfont);
		if (QWidget *titleLabel = tab->findChild<QWidget *>("dockWidgetTabLabel"))
			titleLabel->setFont(newfont);
	}

	// Qt style sheets can block runtime font inheritance. Propagate only the
	// palette font size, leaving widgets with a deliberately different size alone.
	const auto childWidgets = findChildren<QWidget *>();
	for (QWidget *child : childWidgets)
	{
		if (child->isWindow() || child->font().pointSize() != oldPointSize)
			continue;

		QFont childFont(child->font());
		childFont.setPointSize(newPointSize);
		child->setFont(childFont);
	}
}

void DockPanelBase::iconSetChange()
{
	IconManager &iconManager = IconManager::instance();

	if(m_iconName.isEmpty())
	{
		setWindowIcon(iconManager.loadPixmap("app-icon"));
		setIcon(QIcon());
	}
	else
	{
		setWindowIcon(iconManager.loadPixmap(m_iconName));
		setIcon(iconManager.loadPixmap(m_iconName));
	}

}
