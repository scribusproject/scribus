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

#ifndef DOCK_PANELBASE_H
#define DOCK_PANELBASE_H

#include "scribusapi.h"
#include "third_party/Qt-Advanced-Docking-System/src/DockWidget.h"
using namespace ads;

class PrefsContext;

class SCRIBUS_API DockPanelBase : public CDockWidget
{
	Q_OBJECT

public:
	DockPanelBase(const QString &title, QWidget *parent = 0);
	DockPanelBase(const QString &title, QString iconName, QWidget *parent = 0);

	void startup();

public slots:
	virtual void setFontSize();
	void iconSetChange();

protected:
	/** @brief Set the Preferences context to be used for storage of startup visibility and position and size */
	virtual void setPrefsContext(const QString &context);

	PrefsContext *m_palettePrefs {nullptr};
	QString m_prefsContextName;
	QString m_iconName;
};

#endif // DOCK_PANELBASE_H
