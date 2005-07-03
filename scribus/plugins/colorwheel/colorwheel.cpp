/* $Id$ */
#include "colorwheel.h"
#include "cwdialog.h"
#include "pluginmanager.h"
#include <qcursor.h>
#include <qlistview.h>

QString name()
{
	return QObject::tr("&Color Wheel...");
}

PluginManager::PluginType type()
{
	return PluginManager::Standard;
}

int ID()
{
	return 13;
}


QString actionName()
{
	return "ColorWheel";
}

QString actionKeySequence()
{
	return "";
}

QString actionMenu()
{
	return "Extras";
}

QString actionMenuAfterName()
{
	return "";
}

bool actionEnabledOnStartup()
{
	return true;
}

/** Create dialog. Everything else is handled in separated classes. */
void run(QWidget *d, ScribusApp */*plug*/)
{
	ColorWheelDialog *dlg = new ColorWheelDialog(d, "dlg", true, 0);
	dlg->exec();
	delete dlg;
}
