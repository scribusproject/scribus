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

ScGTPluginManager* ScGTPluginManager::m_instance_ = 0; // init static variables

ScGTPluginManager::ScGTPluginManager()
{
}

ScGTPluginManager* ScGTPluginManager::instance()
{
	if (!ScGTPluginManager::m_instance_)
		m_instance_ = new ScGTPluginManager();
	
	return m_instance_;
}

void ScGTPluginManager::deleteInstance()
{
	delete m_instance_;
	m_instance_ = 0;
}

void ScGTPluginManager::registerGTPlugin(ScGTPlugin *plugin)
{
	if (plugin)
		m_plugins_.append(plugin);
}

void ScGTPluginManager::unRegisterGTPlugin(ScGTPlugin *plugin)
{
	m_plugins_.removeAll(plugin);
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
	for (int i = 0; i < m_plugins_.count(); ++i)
	{
		if (m_plugins_.at(i)->fileExtensions().count() != 0)
		{
			filters += m_plugins_.at(i)->fileFormatName() + " (";
			QStringList fileExtensions = m_plugins_.at(i)->fileExtensions();
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
	return m_showOptionsBox_->isChecked();
}

bool ScGTFileDialog::append() const
{
	return m_appendBox_->isChecked();
}

void ScGTFileDialog::customize()
{
	m_diaExtension_ = new QWidget(this);
	m_diaExtension_->setObjectName("diaExtension_");

	m_appendBox_      = new QCheckBox( tr("Append"), m_diaExtension_);
	m_appendBox_->setObjectName("appendBox_"); 
	m_showOptionsBox_ = new QCheckBox( tr("Show options"), m_diaExtension_);
	m_showOptionsBox_->setObjectName("showOptionsBox_"); 

	QHBoxLayout *layout = new QHBoxLayout;
	layout->setSpacing(5);
	layout->addWidget(m_appendBox_);
	layout->addWidget(m_showOptionsBox_);

	m_diaExtension_->setLayout(layout);

	addWidgets(m_diaExtension_);
}

ScGTFileDialog::~ScGTFileDialog()
{
	delete m_showOptionsBox_;
	delete m_appendBox_;
	delete m_diaExtension_;
}

/***************************************************************************************/
/***************************************************************************************/



