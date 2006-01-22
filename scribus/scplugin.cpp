/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
	// These tests must be in reverse order of inheritance,
	// ie most specific to least specific.
	if (inherits("LoadSavePlugin"))
		return tr("Load/Save/Import/Export");
	else if (inherits("ScPersistentPlugin"))
		return tr("Persistent", "plugin manager plugin type");
	else if (inherits("ScActionPlugin"))
		return tr("Action", "plugin manager plugin type");
	else
	{
		qDebug("Unknown plugin type: %s", className());
		return tr("Unknown");
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
