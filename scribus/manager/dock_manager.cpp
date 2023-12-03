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
#include "ui/docks/dock_centralwidget.h"

#include "iconmanager.h"
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

	// Apply common configuration for each palette
	configureDock(pagePalette);
	configureDock(contentPalette);
	configureDock(propertiesPalette);
	configureDock(layerPalette);
	configureDock(pagePalette);
	configureDock(inlinePalette);
	configureDock(alignDistributePalette);
	configureDock(scrapbookPalette);
	configureDock(bookPalette);
	configureDock(undoPalette);
	configureDock(symbolPalette);

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

void DockManager::initWorkspaces()
{
	createDefaultWorkspace();
//	restoreWorkspaceFromPrefs();
}

void DockManager::removeAllDockWidgets()
{
	QMap<QString, CDockWidget *> map = dockWidgetsMap();
	foreach (QString key, map.keys())
		removeDockWidget(map.value(key));
}

void DockManager::hideAllDocks()
{
	foreach (CDockWidget *dock, this->dockWidgets())
	{
		if(dock->isCentralWidget())
			continue;
		dock->closeDockWidget();
	}

	foreach (CFloatingDockContainer *dockContainer, this->floatingWidgets())
	{
		dockContainer->close();
	}
}

void DockManager::toggleDocksVisibility()
{
	QString perspective = "_allpaletteshidden";

	if(perspectiveNames().contains(perspective))
	{
		openPerspective(perspective);
		removePerspective(perspective);
		m_dockTemporaryHidden = false;
	}
	else
	{
		addPerspective(perspective);
		hideAllDocks();
		m_dockTemporaryHidden = true;

//		qDebug() << Q_FUNC_INFO << this->dockWidgets();
//		qDebug() << Q_FUNC_INFO << this->floatingWidgets();
	}

}

bool DockManager::hasTemporaryHiddenDocks()
{
	return m_dockTemporaryHidden;
}

CDockAreaWidget *DockManager::addDockFromPlugin(CDockWidget *dock, bool closed)
{
	CDockAreaWidget *a = addDockWidget(RightDockWidgetArea, dock, dockCenter->dockAreaWidget());
	dock->toggleView(!closed);	
	updateIcon(dock);
	configureDock(dock);
	return a;
}

void DockManager::restoreWorkspaceFromPrefs()
{
	QByteArray ba = PrefsManager::instance().appPrefs.uiPrefs.adsDockState;

	if(ba.isEmpty())
		return;

	this->restoreState(ba);
}

void DockManager::saveWorkspaceToPrefs()
{
	PrefsManager::instance().appPrefs.uiPrefs.adsDockState = this->saveState();
}

void DockManager::iconSetChange()
{
	// Update all icons of each docked CDockContainer
	updateIcons(this->dockWidgets());

	// Update all icons of each floating CFloatingDockContainer
	foreach (CFloatingDockContainer *dockContainer, this->floatingWidgets())
		updateIcons(dockContainer->dockWidgets());
}

void DockManager::restoreHiddenWorkspace()
{
	if (m_dockTemporaryHidden)
		toggleDocksVisibility();
}

void DockManager::updateIcons(QList<CDockWidget *> dockWidgets)
{
	foreach (CDockWidget *dock, dockWidgets)
		updateIcon(dock);
}

void DockManager::updateIcon(CDockWidget *dockWidget)
{
	IconManager &iconManager = IconManager::instance();

	dockWidget->dockAreaWidget()->titleBarButton(ads::TitleBarButtonClose)->setIcon(iconManager.loadIcon("close", 12));
	dockWidget->dockAreaWidget()->titleBarButton(ads::TitleBarButtonUndock)->setIcon(iconManager.loadIcon("dock-float", 12));
	dockWidget->dockAreaWidget()->titleBarButton(ads::TitleBarButtonTabsMenu)->setIcon(iconManager.loadIcon("menu-down", 12));
	//	dock->dockAreaWidget()->titleBarButton(ads::TitleBarButtonAutoHide)->setIcon(iconManager.loadIcon("dock-auto-hide", 12));

	for (auto tabCloseButton : dockWidget->dockAreaWidget()->findChildren<QAbstractButton*>("tabCloseButton"))
		tabCloseButton->setIcon(iconManager.loadIcon("close", 12));
}

void DockManager::configureDock(CDockWidget *dock)
{
	/*
	* MinimumSizeHintFromContent
	* MinimumSizeHintFromDockWidgetMinimumSize
	* MinimumSizeHintFromContentMinimumSize
	*/

	dock->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromContent);
	connect( dock, &CDockWidget::viewToggled, this, &DockManager::restoreHiddenWorkspace);
}

void DockManager::createDefaultWorkspace()
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

	// Left
	auto *areaLeft = addDockWidget(LeftDockWidgetArea, pagePalette, areaCenter);
	addDockWidgetTabToArea(outlinePalette, areaLeft);

	// Left	Center
	auto *areaCenterLeft = addDockWidget(LeftDockWidgetArea, inlinePalette, areaCenter);
	addDockWidgetTabToArea(scrapbookPalette, areaCenterLeft);
	addDockWidgetTabToArea(bookPalette, areaCenterLeft);
	addDockWidgetTabToArea(symbolPalette, areaCenterLeft);

	// Left Bottom
	auto *areaLeftBottom = addDockWidget(BottomDockWidgetArea, layerPalette, areaLeft);
	addDockWidgetTabToArea(alignDistributePalette, areaLeftBottom);
	addDockWidgetTabToArea(undoPalette, areaLeftBottom);

	// Right Panel
	auto *areaRight = addDockWidget(RightDockWidgetArea, propertiesPalette, areaCenter);
	addDockWidget(CenterDockWidgetArea, contentPalette, areaRight);

	// Top Panel
	//    auto * areaTop = addDockWidget(TopDockWidgetArea, dockToolProperties);
	//    areaTop->setAllowedAreas(NoDockWidgetArea);
	//    areaTop->setDockAreaFlag(CDockAreaWidget::HideSingleWidgetTitleBar, true);

	// Resizing area height of left and bottom-left
	int heightL = areaLeft->height();
	setSplitterSizes(areaLeft, {heightL * 2 / 3, heightL * 1 / 3});

	// Resizing area width of left, center-left, center and right
	int widthCL = areaCenter->width();
	int panelWidth = 290;
	setSplitterSizes(areaCenter, {panelWidth, panelWidth, widthCL - 3 * panelWidth, panelWidth});


	// hide panels that are not visible in default workspace
	inlinePalette->closeDockWidget();
	scrapbookPalette->closeDockWidget();
	bookPalette->closeDockWidget();
	symbolPalette->closeDockWidget();
	layerPalette->closeDockWidget();
	undoPalette->closeDockWidget();
	outlinePalette->closeDockWidget();

	// active palettes
	areaLeft->setCurrentDockWidget(pagePalette);
	areaRight->setCurrentDockWidget(propertiesPalette);
	areaLeftBottom->setCurrentDockWidget(alignDistributePalette);

	// add perspective for a later usage, like reset workspace to default.
	this->addPerspective("Default");
}
