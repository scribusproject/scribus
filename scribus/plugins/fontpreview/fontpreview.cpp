#include "fontpreview.h"
#include "ui.h"

QString Name()
{
	return QObject::tr("&Fonts Preview");
}

int Type()
{
	return 1;
}

int ID()
{
	return 2;
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

