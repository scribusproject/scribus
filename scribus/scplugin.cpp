#include "scplugin.h"
#include "qwidget.h"
#include "deferredtask.h"
#include "scribus.h"
#include "scribusapp.h"

//=====================================================//
//                        ScPlugin                     //
//=====================================================//

ScPlugin::ScPlugin(int id, QString& name, PluginType pluginType)
	: QObject(0),
	pluginType(pluginType),
	id(id),
	fullTrName(name)
{
}

QWidget* ScPlugin::newPrefsPanelWidget()
{
	return 0;
}

// Don't call this method; it must be overridden if the plug-in
// returns a prefs widget.
void ScPlugin::destroyPrefsPanelWidget(QWidget* /*prefsPanelWidget*/)
{
	Q_ASSERT(false);
}

const QPixmap ScPlugin::prefsPanelIcon() const
{
	return QPixmap();
}

const QString ScPlugin::prefsPanelName() const
{
	return QString::null;
}

const QString & ScPlugin::lastError() const
{
	return m_lastError;
}

//=====================================================//
//                   ScImportExportPlugin              //
//=====================================================//

ScImportExportPlugin::ScImportExportPlugin(int id, QString& name, PluginType pluginType)
	: ScPlugin(id, name, pluginType)
{
}

// Stub for plugins that don't implement this method to inherit
// Just calls the QIODevice version, assuming target is a file.
bool ScImportExportPlugin::run(QWidget* parent, ScribusApp* mainWindow, QString target)
{
	int flag = id == PluginType_Import ? IO_ReadOnly : IO_WriteOnly;
	QFile f(target);
	if (!f.exists())
	{
		m_lastError = tr("Could not find target file %1: %2", "plugins")
			.arg(target)
			.arg(qApp->translate("QFile",f.errorString()));
		return false;
	}
	if (!f.open(flag))
	{
		m_lastError = tr("Could not open target file %1: %2", "plugins")
			.arg(target)
			.arg(qApp->translate("QFile",f.errorString()));
		return false;
	}
	// Call the QIODevice* based implementation
	bool result = run(parent, mainWindow, &f);
	if (f.isOpen())
		f.close();
	return result;
}

// Stub for plugins that don't implement this method to inherit
bool ScImportExportPlugin::run(
		QWidget* /* parent */,
		ScribusApp* /* mainWindow */,
		QIODevice* /* target */)
{
	return false;
}


// Stub for plugins that don't implement this method to inherit
DeferredTask* ScImportExportPlugin::runAsync(
		QWidget* /* parent */,
		ScribusApp* /* mainWindow */,
		QString /* target */)
{
	return 0;
}


// Stub for plugins that don't implement this method to inherit
DeferredTask* ScImportExportPlugin::runAsync(
		QWidget* /* parent */,
		ScribusApp* /* mainWindow */,
		QIODevice* /* target */)
{
	return 0;
}


//=====================================================//
//                   ScImportExportPlugin              //
//=====================================================//

ScPersistentPlugin::ScPersistentPlugin(int id, QString& name)
	: ScPlugin(id, name, ScPlugin::PluginType_Persistent)
{
}
