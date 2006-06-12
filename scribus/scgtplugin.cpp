/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
// kk2006


#include "scgtplugin.h"

#include <qcheckbox.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qptrlist.h>
#include <qdir.h>
#include <qobject.h>

/***************************************************************************************/
/***************************************************************************************/

ScGTPluginManager* ScGTPluginManager::instance_ = 0; // init static variables

ScGTPluginManager::ScGTPluginManager()
{
	plugins_.setAutoDelete(false);
}

ScGTPluginManager* ScGTPluginManager::instance()
{
	if (!ScGTPluginManager::instance_)
		instance_ = new ScGTPluginManager();
	
	return instance_;
}

void ScGTPluginManager::deleteInstance()
{
	delete instance_;
	instance_ = 0;
}

void ScGTPluginManager::registerGTPlugin(ScGTPlugin *plugin)
{
	if (plugin)
		plugins_.append(plugin);
}

void ScGTPluginManager::unRegisterGTPlugin(ScGTPlugin *plugin)
{
	plugins_.remove(plugin);
}

void ScGTPluginManager::run()
{
	qDebug("run()");
	// TODO prefs
	ScGTFileDialog *dia = new ScGTFileDialog(".", fileFilter(), 0, "dia");
	if (dia->exec())
	{
		
	}
	delete dia;
}

QString ScGTPluginManager::fileFilter()
{
	QString filters = "";
	QString allSupported = QObject::tr("All Supported Formats") + " (";
	for (uint i = 0; i < plugins_.count(); ++i)
	{
		if (plugins_.at(i)->fileExtensions().count() != 0)
		{
			filters += plugins_.at(i)->fileFormatName() + " (";
			QStringList fileExtensions = plugins_.at(i)->fileExtensions();
			for (uint j = 0; j < fileExtensions.count(); ++j)
			{
				filters += "*." + fileExtensions[j] + " ";
				allSupported += "*." + fileExtensions[j] + " ";
			}
			filters = filters.stripWhiteSpace();
			filters += ");;";
		}
	}
	allSupported = allSupported.stripWhiteSpace();
	allSupported += ");;";
	filters = allSupported + filters;
	filters += QObject::tr("All Files (*)");

	return filters;
}

void ScGTPluginManager::options()
{
	
}

ScGTPluginManager::~ScGTPluginManager()
{
	
}

/***************************************************************************************/
/***************************************************************************************/

ScGTFileDialog::ScGTFileDialog(const QString & dirName,
                               const QString & filters,
                               QWidget * parent, const char * name)
: QFileDialog(dirName, filters, parent, name, true)
{
	setCaption(tr("Select file to import"));
	setMode(QFileDialog::ExistingFile);
	customize();
}

bool ScGTFileDialog::showOptions() const
{
	return showOptionsBox_->isChecked();
}

void ScGTFileDialog::customize()
{
	showOptionsBox_ = new QCheckBox(tr("Show options"), this, "showOptionsBox_");	
	addWidgets(0, showOptionsBox_, 0);
	
	
// 	HomeB = new QToolButton(this);
// 	HomeB->setIconSet(loadIcon("gohome.png"));
// 	HomeB->setAutoRaise(true);
// 	connect(HomeB, SIGNAL(clicked()), this, SLOT(slotHome()));
// 	addToolButton(HomeB);
}

ScGTFileDialog::~ScGTFileDialog()
{
	delete showOptionsBox_;
}

/***************************************************************************************/
/***************************************************************************************/



