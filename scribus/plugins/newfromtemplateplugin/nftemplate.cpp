/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#include <qstring.h>
#include <qcursor.h>
#include <qdir.h>
#include <qwidget.h>

#include "nftemplate.h"
#include "nftemplate.moc"
#include "nftdialog.h"
#include "scraction.h"
#include "menumanager.h"
#include "pluginmanager.h"

ScribusApp* Carrier;
QWidget* par;

QString name()
{
    return QObject::tr("New &from Template...");
}

PluginManager::PluginType type()
{
	return PluginManager::Standard;
}

int ID()
{
	return 3;
}

QString actionName()
{
	return "NewFromTemplate";
}

QString actionKeySequence()
{
	return "Ctrl+Alt+N";
}

QString actionMenu()
{
	return "File";
}

QString actionMenuAfterName()
{
	return "New";
}

bool actionEnabledOnStartup()
{
	return true;
}
/*
void InitPlug(QWidget *d, ScribusApp *plug)
{
	Carrier = plug;
	par = d;
	Nft = new MenuNFT(d);
	int id = plug->fileMenu->insertItem(QObject::tr("New &from Template..."), -1, plug->fileMenu->indexOf(plug->scrActions["fileNew"]->getMenuIndex())+1);
	plug->fileMenu->connectItem(id, Nft, SLOT(RunNFTPlug()));
	plug->fileMenu->setItemEnabled(id, 1);
}
*/
void cleanUpPlug()
{
}

void run(QWidget *d, ScribusApp *plug)
{
	Carrier = plug;
	par = d;
	Nft = new MenuNFT(d);
	Nft->RunNFTPlug();
}


void MenuNFT::RunNFTPlug()
{
	nftdialog* nftdia = new nftdialog(par, Carrier->getGuiLanguage(), Carrier->Prefs.TemplateDir);
	if (nftdia->exec())
	{
		qApp->setOverrideCursor(QCursor(Qt::WaitCursor), true);
		Carrier->LadeDoc(QDir::cleanDirPath(nftdia->currentTemplate->file));
		Carrier->doc->hasName = false;
		Carrier->doc->DocName = nftdia->currentTemplate->name;
		Carrier->ActWin->setCaption(QObject::tr("Template: ") + nftdia->currentTemplate->name);
		QDir::setCurrent(Carrier->Prefs.DocDir);
		Carrier->removeRecent(QDir::cleanDirPath(nftdia->currentTemplate->file));
		qApp->restoreOverrideCursor();
	}
	delete nftdia;
}
