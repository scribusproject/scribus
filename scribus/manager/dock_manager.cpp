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
#include "dock_manager.h"

#include <QMenu>
#include "third_party/Qt-Advanced-Docking-System/src/DockAreaWidget.h"
#include "third_party/Qt-Advanced-Docking-System/src/DockSplitter.h"
#include "ui/docks/dock_centralwidget.h"

#include "iconmanager.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribusapp.h"
#include "ui/aligndistribute.h"
#include "ui/bookmarkpalette.h"
#include "ui/contentpalette.h"
#include "ui/inlinepalette.h"
#include "ui/layers.h"
#include "ui/outlinepalette.h"
#include "ui/pagepalette.h"
#include "ui/propertiespalette.h"
#include "ui/scrapbookpalette.h"
#include "ui/symbolpalette.h"
#include "undogui.h"


/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

DockManager::DockManager(QWidget *parent)
	: CDockManager(parent)
{
	dockCenter = new DockCentralWidget();
	auto *areaCenter = CDockManager::setCentralWidget(dockCenter);
	areaCenter->setAllowedAreas(LeftDockWidgetArea | RightDockWidgetArea);

	m_palettePrefs = PrefsManager::instance().prefsFile->getContext("user_preferences");

	setStyleSheet(""); // reset style sheet to use custom icons

	connect( ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()) );
}

void DockManager::setupDocks()
{
	pagePalette = new PagePalette((QWidget *) this->parent());
	contentPalette = new ContentPalette(this);
	propertiesPalette = new PropertiesPalette(this);
	outlinePalette = new OutlinePalette(this);
	layerPalette = new LayerPalette(this);
	inlinePalette = new InlinePalette(this);
	alignDistributePalette = new AlignDistributePalette();
	scrapbookPalette = new Biblio(this);
	bookPalette = new BookPalette(this);
	undoPalette = new UndoPalette(this);
	symbolPalette = new SymbolPalette(this);

	// Panel ToolProperties
	//    PanelToolProperties * panelTest = new PanelToolProperties();
	//    dockToolProperties->setWidget(panelTest);
	//    dockToolProperties->setFeature(ads::CDockWidget::NoTab, true);
}

void DockManager::setCentralWidget(QWidget *widget)
{
	dockCenter->setWidget(widget);
}

/* ********************************************************************************* *
 *
 * Public Methods
 *
 * ********************************************************************************* */

void DockManager::loadDefaultWorkspace()
{
	/************************************************************
	 *
	 *
	 *      LAYOUT SCHEME
	 *
	 *        290px     290px           *             290px
	 *     |---------|---------|-------------------|---------|
	 *     |  Left   |  Center |      Center       |  Right  |
	 * 2/3 |         |  Left   |                   |         |
	 *     |         |         |                   |         |
	 *     |         |         |                   |         |
	 *     |---------|         |                   |         |
	 * 1/3 | Bottom  |         |                   |         |
	 *     | Left    |         |                   |         |
	 *     |---------|---------|-------------------|---------|
	 *
	 *
	 *************************************************************/

	auto *areaCenter = dockCenter->dockAreaWidget();

	// Left	Center
	areaCenterLeft = addDockWidget(LeftDockWidgetArea, inlinePalette, areaCenter);
	addDockWidget(CenterDockWidgetArea, scrapbookPalette, areaCenterLeft);
	addDockWidget(CenterDockWidgetArea, bookPalette, areaCenterLeft);
	addDockWidget(CenterDockWidgetArea, symbolPalette, areaCenterLeft);

	// Left
	auto *areaLeft = addDockWidget(LeftDockWidgetArea, pagePalette, areaCenterLeft);
	addDockWidget(CenterDockWidgetArea, outlinePalette, areaLeft);

	// Left Bottom
	auto *areaLeftBottom = addDockWidget(BottomDockWidgetArea, layerPalette, areaLeft);
	addDockWidget(CenterDockWidgetArea, alignDistributePalette, areaLeftBottom);
	addDockWidget(CenterDockWidgetArea, undoPalette, areaLeftBottom);

	// Right Panel
	auto *areaRight = addDockWidget(RightDockWidgetArea, propertiesPalette, areaCenter);
	addDockWidget(CenterDockWidgetArea, contentPalette, areaRight);

	// Top Panel
	//    auto * areaTop = addDockWidget(TopDockWidgetArea, dockToolProperties);
	//    areaTop->setAllowedAreas(NoDockWidgetArea);
	//    areaTop->setDockAreaFlag(CDockAreaWidget::HideSingleWidgetTitleBar, true);

	// Resizing area height of left and bottom-left
	auto splitterL = ads::internal::findParent<ads::CDockSplitter *>(areaLeft);
	if (splitterL)
	{
		int heightL = splitterL->height();
		splitterL->setSizes({heightL * 2 / 3, heightL * 1 / 3});
	}

	// Resizing area width of left, center-left, center and right
	auto splitterCL = ads::internal::findParent<ads::CDockSplitter *>(areaCenter);
	if (splitterCL)
	{
		int widthCL = splitterCL->width();
		int panelWidth = 290;
		splitterCL->setSizes({panelWidth, panelWidth, widthCL - 3 * panelWidth, panelWidth});
	}

	// hide panels that are not visible in default workspace
	inlinePalette->toggleView(false);
	scrapbookPalette->toggleView(false);
	bookPalette->toggleView(false);
	symbolPalette->toggleView(false);
	layerPalette->toggleView(false);
	undoPalette->toggleView(false);
	outlinePalette->toggleView(false);

	// active palettes
	areaLeft->setCurrentDockWidget(pagePalette);
	areaRight->setCurrentDockWidget(propertiesPalette);
	areaLeftBottom->setCurrentDockWidget(alignDistributePalette);

	// add perspective for a later usage, like reset workspace to default.
	this->addPerspective("Default");

	// try to load custom layout from preferences
	if (!m_palettePrefs->get("ads_dockstate").isEmpty())
		loadWorkspaceFromFile();
}

void DockManager::removeAllDockWidgets()
{
	QMap<QString, CDockWidget *> map = dockWidgetsMap();
	foreach (QString key, map.keys())
		removeDockWidget(map.value(key));
}

CDockAreaWidget *DockManager::addDockFromPlugin(CDockWidget *dock, bool closed)
{
	CDockAreaWidget *a = addDockWidget(RightDockWidgetArea, dock, dockCenter->dockAreaWidget());
	dock->toggleView(!closed);
	return a;
}

void DockManager::loadWorkspaceFromFile()
{
	if (!m_palettePrefs)
		return;
	QByteArray ba = QByteArray::fromHex(m_palettePrefs->get("ads_dockstate").toLatin1());
	this->restoreState(ba);
}

void DockManager::saveWorkspaceToFile()
{
	if (!m_palettePrefs)
		return;
	QByteArray data = this->saveState();
	QString s = data.toHex();
	m_palettePrefs->set("ads_dockstate", s);
}

void DockManager::iconSetChange()
{
	IconManager &iconManager = IconManager::instance();

	for (int i = 0; i < this->dockAreaCount(); ++i)
	{
		CDockAreaWidget *daw = this->dockArea(i);

		daw->titleBarButton(ads::TitleBarButtonClose)->setIcon(iconManager.loadIcon("close", 12));
		daw->titleBarButton(ads::TitleBarButtonUndock)->setIcon(iconManager.loadIcon("dock-float", 12));
		daw->titleBarButton(ads::TitleBarButtonTabsMenu)->setIcon(iconManager.loadIcon("menu-down", 12));
	//	daw->titleBarButton(ads::TitleBarButtonAutoHide)->setIcon(iconManager.loadIcon("dock-auto-hide", 12));

		for (auto tabCloseButton : daw->findChildren<QAbstractButton*>("tabCloseButton"))
		{
			tabCloseButton->setIcon(iconManager.loadIcon("close", 12));
		}
	}
}
