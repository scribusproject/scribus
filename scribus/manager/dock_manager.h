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

	void setCentralWidget(QWidget * widget);
	void setupDocks();
	void loadDefaultWorkspace();
	void setTheme(QString theme);
	void removeAllDockWidgets();
	CDockAreaWidget* addDockFromPlugin(CDockWidget * dock, bool closed = true);


	PagePalette *pagePalette {nullptr};
	OutlinePalette * outlinePalette {nullptr};
	PropertiesPalette * propertiesPalette {nullptr};
	ContentPalette * contentPalette {nullptr};
	LayerPalette * layerPalette {nullptr};
	AlignDistributePalette * alignDistributePalette {nullptr};
	InlinePalette * inlinePalette {nullptr};
	Biblio * scrapbookPalette {nullptr};
	BookPalette * bookPalette {nullptr};
	UndoPalette * undoPalette {nullptr};
	SymbolPalette * symbolPalette {nullptr};

public slots:
	void loadWorkspaceFromFile();
	void saveWorkspaceToFile();

private:
	DockCentralWidget * dockCenter {nullptr};
	PrefsContext* m_palettePrefs {nullptr};

	CDockAreaWidget * areaCenterLeft {nullptr};

};

#endif // DOCK_MANAGER_H
