#include "scplugin.h"
#include "qwidget.h"
#include "deferredtask.h"
#include "scribus.h"
#include "scribusapp.h"

#include "scplugin.moc"

//=====================================================//
//                        ScPlugin                     //
//=====================================================//

ScPlugin::ScPlugin(PluginType pluginType)
	: QObject(0),
	pluginType(pluginType)
{
}

QWidget* ScPlugin::newPrefsPanelWidget()
{
	return 0;
}

ScPlugin::~ScPlugin()
{
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
			return tr("Persistent", "plugin manager plugin type");
			break;
		case PluginType_Import:
			return tr("Import", "plugin manager plugin type");
			break;
		case PluginType_Export:
			return tr("Export", "plugin manager plugin type");
			break;
		case PluginType_Action:
			return tr("Action", "plugin manager plugin type");
			break;
	}
}

//=====================================================//
//                   ScActionPlugin              //
//=====================================================//

ScActionPlugin::ScActionPlugin(PluginType pluginType)
	: ScPlugin(pluginType)
{
}

ScActionPlugin::~ScActionPlugin()
{
}

// Stub for plugins that don't implement this method to inherit
// Just calls the QIODevice version, assuming target is a file.
bool ScActionPlugin::run(QString target)
{
	int flag = (pluginType == PluginType_Export) ? IO_WriteOnly : IO_ReadOnly;
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
bool ScActionPlugin::run(QIODevice* /* target */)
{
	return false;
}


// Stub for plugins that don't implement this method to inherit
DeferredTask* ScActionPlugin::runAsync(QString /* target */)
{
	return 0;
}


// Stub for plugins that don't implement this method to inherit
DeferredTask* ScActionPlugin::runAsync(QIODevice* /* target */)
{
	return 0;
}

// Legacy code support; avoid relying on in new code.
const QString & ScActionPlugin::runResult() const
{
	return m_runResult;
}

const ScActionPlugin::ActionInfo & ScActionPlugin::actionInfo() const
{
	Q_ASSERT(!m_actionInfo.text.isNull());
	return m_actionInfo;
}

//=====================================================//
//                   ScActionPlugin              //
//=====================================================//

ScPersistentPlugin::ScPersistentPlugin()
	: ScPlugin(ScPlugin::PluginType_Persistent)
{
}

ScPersistentPlugin::~ScPersistentPlugin()
{
}
