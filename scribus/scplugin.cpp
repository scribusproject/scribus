#include "scplugin.h"
#include "qwidget.h"
#include "deferredtask.h"
#include "scribus.h"
#include "scribusapp.h"
#include "prefspanel.h"

#include "scplugin.moc"

//=====================================================//
//                        ScPlugin                     //
//=====================================================//

ScPlugin::ScPlugin()
	: QObject(0)
{
}

ScPlugin::~ScPlugin()
{
}

bool ScPlugin::newPrefsPanelWidget( QWidget* /* parent */,
									PrefsPanel*& /* panel */,
									QString& /* caption */,
									QPixmap& /* icon */)
{
	return false;
}

const QString & ScPlugin::lastError() const
{
	return m_lastError;
}

const QString ScPlugin::pluginTypeName() const
{
	QCString cname(className());
	if (cname == "ScPersistentPlugin")
		return tr("Persistent", "plugin manager plugin type");
	else if (cname == "ScActionPlugin")
		return tr("Action", "plugin manager plugin type");
	else if (cname == "LoadSavePlugin")
		return tr("Load/Save/Import/Export");
	else
	{
		Q_ASSERT(false);
		return QString();
	}
}

//=====================================================//
//                   ScActionPlugin              //
//=====================================================//

ScActionPlugin::ScActionPlugin() : ScPlugin()
{
}

ScActionPlugin::~ScActionPlugin()
{
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
	: ScPlugin()
{
}

ScPersistentPlugin::~ScPersistentPlugin()
{
}
