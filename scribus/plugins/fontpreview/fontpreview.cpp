#include "fontpreview.h"
#include "ui.h"

QString Name()
{
	return QObject::tr("Fonts Preview");
}


int Type()
{
	return 1;
}


/**
Create dialog and insert font into Style meny when user accepts.
*/
void Run(QWidget *d, ScribusApp *plug)
{
	FontPreview *dlg = new FontPreview(plug, d, "dlg", TRUE, 0);
	//dlg->selectAction(action);
	if (dlg->exec()==QDialog::Accepted)
	{
		//action accepted
		plug->SetNewFont(dlg->fontList->currentText());
	}

	delete dlg;
}

