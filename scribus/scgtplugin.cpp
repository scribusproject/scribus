/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
// kk2006

#include <QObject>
#include <QHBoxLayout>

#include "scgtplugin.h"

ScGTPluginManager* ScGTPluginManager::m_instance = nullptr; // init static variables

ScGTPluginManager::ScGTPluginManager() = default;

ScGTPluginManager* ScGTPluginManager::instance()
{
	if (!ScGTPluginManager::m_instance)
		m_instance = new ScGTPluginManager();
	
	return m_instance;
}

void ScGTPluginManager::deleteInstance()
{
	delete m_instance;
	m_instance = nullptr;
}

void ScGTPluginManager::registerGTPlugin(ScGTPlugin *plugin)
{
	if (plugin)
		m_plugins.append(plugin);
}

void ScGTPluginManager::unRegisterGTPlugin(ScGTPlugin *plugin)
{
	m_plugins.removeAll(plugin);
}

void ScGTPluginManager::run()
{
	// TODO prefs
	ScGTFileDialog *dia = new ScGTFileDialog(".", fileFilter(), nullptr, "dia");
	if (dia->exec())
	{
		
	}
	delete dia;
}

QString ScGTPluginManager::fileFilter()
{
	QString filters;
	QString allSupported = QObject::tr("All Supported Formats") + " (";
	for (int i = 0; i < m_plugins.count(); ++i)
	{
		if (m_plugins.at(i)->fileExtensions().count() != 0)
		{
			filters += m_plugins.at(i)->fileFormatName() + " (";
			QStringList fileExtensions = m_plugins.at(i)->fileExtensions();
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

ScGTPluginManager::~ScGTPluginManager() = default;

/***************************************************************************************/
/***************************************************************************************/

ScGTFileDialog::ScGTFileDialog(const QString& dirName,
							   const QString& filters,
                               QWidget * parent, const char * name)
: CustomFDialog(parent, dirName, tr("Select a file to import"), filters)
{
	customize();
}

bool ScGTFileDialog::showOptions() const
{
	return m_showOptionsBox->isChecked();
}

bool ScGTFileDialog::append() const
{
	return m_appendBox->isChecked();
}

void ScGTFileDialog::customize()
{
	m_diaExtension = new QWidget(this);
	m_diaExtension->setObjectName("diaExtension_");

	m_appendBox = new QCheckBox( tr("Append"), m_diaExtension);
	m_appendBox->setObjectName("appendBox_");
	m_showOptionsBox = new QCheckBox( tr("Show options"), m_diaExtension);
	m_showOptionsBox->setObjectName("showOptionsBox_");

	QHBoxLayout *layout = new QHBoxLayout;
	layout->setSpacing(6);
	layout->addWidget(m_appendBox);
	layout->addWidget(m_showOptionsBox);

	m_diaExtension->setLayout(layout);

	addWidgets(m_diaExtension);
}

ScGTFileDialog::~ScGTFileDialog()
{
	delete m_showOptionsBox;
	delete m_appendBox;
	delete m_diaExtension;
}

/***************************************************************************************/
/***************************************************************************************/



