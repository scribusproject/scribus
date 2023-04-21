#include <QApplication>

#include "scconfig.h"
#include "api/api_application.h"
#include "scribusapi.h"
#include "scribusindigomainwindow.h"

#include "third_party/Qt-Advanced-Docking-System/src/DockAreaWidget.h"
#include "third_party/Qt-Advanced-Docking-System/src/DockAreaTitleBar.h"
#include "third_party/Qt-Advanced-Docking-System/src/DockAreaTabBar.h"
#include "third_party/Qt-Advanced-Docking-System/src/DockManager.h"
#include "third_party/Qt-Advanced-Docking-System/src/IconProvider.h"

ScribusIndigoMainWindow::ScribusIndigoMainWindow(QWidget *parent)
	: QMainWindow{parent}
	{

	// Documentation:
	// https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System/blob/master/doc/user-guide.md
//	ads::CDockManager::setConfigFlag(ads::CDockManager::AlwaysShowTabs, false);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::FloatingContainerHasWidgetIcon, true);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::FloatingContainerHasWidgetTitle, true);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::HideSingleCentralWidgetTitleBar, false);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::OpaqueSplitterResize, true);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::TabCloseButtonIsToolButton, true);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::AllTabsHaveCloseButton, false);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaDynamicTabsMenuButtonVisibility, true);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasCloseButton, false);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasUndockButton, false);
//	ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHideDisabledButtons, true);

	//		ads::CDockManager::iconProvider().registerCustomIcon(ads::TabCloseIcon,
	//IconManager::instance().icon("close"));
	//		ads::CDockManager::iconProvider().registerCustomIcon(ads::DockAreaCloseIcon,
	//IconManager::instance().icon("close"));
	//		ads::CDockManager::iconProvider().registerCustomIcon(ads::DockAreaMenuIcon,
	//IconManager::instance().icon("menu-down"));
	//		ads::CDockManager::iconProvider().registerCustomIcon(ads::DockAreaUndockIcon,
	//IconManager::instance().icon("dock-float"));
	}



ScribusIndigoMainWindow::~ScribusIndigoMainWindow()
{

}

int ScribusIndigoMainWindow::initScIMW()
{
	int retVal = 0;

	QString scribusTitle(ScribusAPI::getVersionScribusTranslated());
	if (ScribusAPI::isSVN() && ScribusAPI::haveSVNRevision())
		scribusTitle.append(QString(" (r%1) - Indigo UI Mockup")
								.arg(ScribusAPI::getSVNRevision()));
	setWindowTitle(scribusTitle);
	setObjectName("MainWindow");
	QWidget *widget = new QWidget;
	setCentralWidget(widget);
	QApplication::processEvents();
	resize(1024, 768);

	return retVal;
}
