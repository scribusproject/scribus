#include "fontpreview.h"
#include "ui.h"

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
Create dialog and insert font into Style meny when user accepts.
*/
void Run(QWidget *d, ScribusApp *plug)
{
	FontPreview *dlg = new FontPreview(plug, d, "dlg", TRUE, 0);
	if (dlg->exec() == QDialog::Accepted)
	{
		if  (plug->DLLinput == "")
			plug->SetNewFont(dlg->fontList->currentText());
		else
			plug->DLLReturn = dlg->fontList->currentText();
	}
	delete dlg;
}

