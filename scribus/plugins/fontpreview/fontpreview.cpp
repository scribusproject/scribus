#include "fontpreview.h"
#include "ui.h"
#include "pluginmanager.h"
#include <qcursor.h>
#include <qlistview.h>

QString Name()
{
	return QObject::tr("&Fonts Preview");
}

int Type()
{
	return 6;
}

int ID()
{
	return 2;
}


QString actionName()
{
	return "FontsPreview";
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

/**
Create dialog and insert font into Style menu when user accepts.
*/
void Run(QWidget *d, ScribusApp *plug)
{
	// I don't know how many fonts user has...
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	FontPreview *dlg = new FontPreview(plug, d, "dlg", TRUE, 0);
	qApp->restoreOverrideCursor();
	// run it and wait for user's reaction
	if (dlg->exec() == QDialog::Accepted)
	{
		if  (plug->pluginManager->dllInput == "")
			plug->SetNewFont(dlg->fontList->currentItem()->text(0));
		else
			plug->pluginManager->dllReturn = dlg->fontList->currentItem()->text(0);
	}
	delete dlg;
}
