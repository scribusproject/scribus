/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#include "nftemplate.h"
#include "nftemplate.moc"
#include "nftdialog.h"
#include <qstring.h>
#include <qcursor.h>
#include <qdir.h>
#include <qwidget.h>
ScribusApp* Carrier;
QWidget* par;

QString Name()
{
    return QObject::tr("New &from Template...");
}

int Type()
{
    return 5;
}

void InitPlug(QWidget *d, ScribusApp *plug)
{
	Carrier = plug;
	par = d;
	Nft = new MenuNFT(d);
	int ind = 0;
	for (uint a = 0; a < plug->fileMenu->count(); ++a)
	{
		if ((plug->fileMenu->text(plug->fileMenu->idAt(a)) == QObject::tr("&New")) ||
			(plug->fileMenu->text(plug->fileMenu->idAt(a)) == "&New"))
			break;
		ind++;
	}
	int id = plug->fileMenu->insertItem(QObject::tr("New &from Template..."), -1, ind+1);
	plug->fileMenu->connectItem(id, Nft, SLOT(RunNFTPlug()));
	plug->fileMenu->setItemEnabled(id, 1);
}

void CleanUpPlug()
{
}

void Run(QWidget *d, ScribusApp *plug)
{
}

void MenuNFT::RunNFTPlug()
{
	nftdialog* nftdia = new nftdialog(par);
	if (nftdia->exec())
	{
		qApp->setOverrideCursor(QCursor(Qt::WaitCursor), true);
		Carrier->LadeDoc(nftdia->currentTemplate->file);
		Carrier->doc->hasName = false;
		Carrier->doc->DocName = nftdia->currentTemplate->name;
		Carrier->ActWin->setCaption("Template: " + nftdia->currentTemplate->name);
		QDir::setCurrent(Carrier->Prefs.DocDir);
		Carrier->RemoveRecent(nftdia->currentTemplate->file);
		qApp->restoreOverrideCursor();
	}
	delete nftdia;
}
