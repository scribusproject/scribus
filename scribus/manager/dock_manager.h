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

/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/


#ifndef DOCK_MANAGER_H
#define DOCK_MANAGER_H

#include "prefscontext.h"
#include "scribusapi.h"
#include "third_party/Qt-Advanced-Docking-System/src/DockManager.h"

using namespace ads;

class ScribusMainWindow;
class PagePalette;
class OutlinePalette;
class PropertiesPalette;
class ContentPalette;
class LayerPalette;
class AlignDistributePalette;
class InlinePalette;
class Biblio;
class BookPalette;
class UndoPalette;
class SymbolPalette;
class DockCentralWidget;

class SCRIBUS_API DockManager : public CDockManager
{
	Q_OBJECT

public:
	DockManager(QWidget *parent);

	void setCentralWidget(QWidget *widget);
	void setupDocks();
	/**
	 * @brief Creates all preconfigured workspaces as perspectives.
	 */
	void initWorkspaces();
	/**
	 * @brief Removes all dock widgets including docks added by plugins.
	 */
	void removeAllDockWidgets();
	/**
	 * @brief Hides all docked and floating dock containers.
	 */
	void hideAllDocks();
	/**
	 * @brief Toggles the temporary visibility of all docks.
	 */
	void toggleDocksVisibility();
	/**
	 * @brief Retruns true if all docks are temporary hidden.
	 * @return
	 */
	bool hasTemporaryHiddenDocks();
	/**
	 * @brief Adds a dock widget to dock manager. Useful for docks that are not initialized by dock manager, e.g. from plugins.
	 * @param dock
	 * @param closed
	 * @return
	 */
	CDockAreaWidget *addDockFromPlugin(CDockWidget *dock, bool closed = true);

	PagePalette *pagePalette {nullptr};
	OutlinePalette *outlinePalette {nullptr};
	PropertiesPalette *propertiesPalette {nullptr};
	ContentPalette *contentPalette {nullptr};
	LayerPalette *layerPalette {nullptr};
	AlignDistributePalette *alignDistributePalette {nullptr};
	InlinePalette *inlinePalette {nullptr};
	Biblio *scrapbookPalette {nullptr};
	BookPalette *bookPalette {nullptr};
	UndoPalette *undoPalette {nullptr};
	SymbolPalette *symbolPalette {nullptr};

public slots:
	/**
	 * @brief Restore current workspace from local preferences file
	 */
	void restoreWorkspaceFromPrefs();
	/**
	 * @brief Save current workspace to local preferences file
	 */
	void saveWorkspaceToPrefs();

	void iconSetChange();
	/**
	 * @brief Show all hidden dock palettes if temporary hidden.
	 */
	void restoreHiddenWorkspace();

protected:
	DockCentralWidget *dockCenter {nullptr};
	bool m_dockTemporaryHidden {false};
	QMap<CDockWidget *, bool> m_temporaryHiddenDocks;

	/**
	 * @brief Refreshes all icons of a dock. Needed for icon set or GUI changes.
	 * @param dockWidgets
	 */
	void updateIcons(QList<CDockWidget *> dockWidgets);
	void updateIcon(CDockWidget * dockWidget);

	void configureDock(CDockWidget *dock);

	/**
	 * @brief Creates a default workspace as perspective.
	 */
	void createDefaultWorkspace();

};

#endif // DOCK_MANAGER_H
