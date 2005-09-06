#include "scplugin.h"
#include "qwidget.h"
#include "deferredtask.h"
#include "scribus.h"
#include "scribusapp.h"

#include "scplugin.moc"

//=====================================================//
//                        ScPlugin                     //
//=====================================================//

ScPlugin::ScPlugin(int id, PluginType pluginType)
	: QObject(0),
	pluginType(pluginType),
	id(id)
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

const QString ScPlugin::pluginTypeName() const
{
	switch(pluginType)
	{
		case PluginType_Persistent:
			return tr("Persistent", "plugin manager");
			break;
		case PluginType_Import:
			return tr("Import", "plugin manager");
			break;
		case PluginType_Export:
			// FIXME
			return tr("Standard", "plugin manager");
			break;
		default:
			return tr("Unknown", "plugin manager");
	}
}

//=====================================================//
//                   ScImportExportPlugin              //
//=====================================================//

ScImportExportPlugin::ScImportExportPlugin(int id, PluginType pluginType)
	: ScPlugin(id, pluginType)
{
}

// Stub for plugins that don't implement this method to inherit
// Just calls the QIODevice version, assuming target is a file.
bool ScImportExportPlugin::run(QString target)
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
	bool result = run(&f);
	if (f.isOpen())
		f.close();
	return result;
}

// Stub for plugins that don't implement this method to inherit
bool ScImportExportPlugin::run(QIODevice* /* target */)
{
	return false;
}


// Stub for plugins that don't implement this method to inherit
DeferredTask* ScImportExportPlugin::runAsync(QString /* target */)
{
	return 0;
}


// Stub for plugins that don't implement this method to inherit
DeferredTask* ScImportExportPlugin::runAsync(QIODevice* /* target */)
{
	return 0;
}

// Legacy code support; avoid relying on in new code.
const QString & ScImportExportPlugin::runResult() const
{
	return m_runResult;
}

const ScImportExportPlugin::ActionInfo & ScImportExportPlugin::actionInfo() const
{
	return m_actionInfo;
}

//=====================================================//
//                   ScImportExportPlugin              //
//=====================================================//

ScPersistentPlugin::ScPersistentPlugin(int id)
	: ScPlugin(id, ScPlugin::PluginType_Persistent)
{
}
