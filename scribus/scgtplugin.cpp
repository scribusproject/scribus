/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
// kk2006


#include "scgtplugin.h"

#include <QCheckBox>
#include <QDir>
#include <QObject>
#include <QHBoxLayout>
#include <QString>
#include <QStringList>
#include <QWidget>

/***************************************************************************************/
/***************************************************************************************/

ScGTPluginManager* ScGTPluginManager::instance_ = 0; // init static variables

ScGTPluginManager::ScGTPluginManager()
{
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
	plugins_.removeAll(plugin);
}

void ScGTPluginManager::run()
{
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
	for (int i = 0; i < plugins_.count(); ++i)
	{
		if (plugins_.at(i)->fileExtensions().count() != 0)
		{
			filters += plugins_.at(i)->fileFormatName() + " (";
			QStringList fileExtensions = plugins_.at(i)->fileExtensions();
			for (int j = 0; j < fileExtensions.count(); ++j)
			{
				filters += "*." + fileExtensions[j] + " ";
				allSupported += "*." + fileExtensions[j] + " ";
			}
			filters = filters.trimmed();
			filters += ");;";
		}
	}
	allSupported = allSupported.trimmed();
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
: CustomFDialog(parent, dirName, tr("Select a file to import"), filters)
{
	customize();
}

bool ScGTFileDialog::showOptions() const
{
	return showOptionsBox_->isChecked();
}

bool ScGTFileDialog::append() const
{
	return appendBox_->isChecked();
}

void ScGTFileDialog::customize()
{
	diaExtension_ = new QWidget(this);
	diaExtension_->setObjectName("diaExtension_");

	appendBox_      = new QCheckBox( tr("Append"), diaExtension_);
	appendBox_->setObjectName("appendBox_"); 
	showOptionsBox_ = new QCheckBox( tr("Show options"), diaExtension_);
	showOptionsBox_->setObjectName("showOptionsBox_"); 

	QHBoxLayout *layout = new QHBoxLayout;
	layout->setSpacing(5);
	layout->addWidget(appendBox_);
	layout->addWidget(showOptionsBox_);

	diaExtension_->setLayout(layout);

	addWidgets(diaExtension_);
}

ScGTFileDialog::~ScGTFileDialog()
{
	delete showOptionsBox_;
	delete appendBox_;
	delete diaExtension_;
}

/***************************************************************************************/
/***************************************************************************************/



