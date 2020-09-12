/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scplugin.h"

#include "deferredtask.h"

#include "scribusdoc.h"
#include "scribusapp.h"
#include "selection.h"
#include "ui/prefs_pane.h"

//=====================================================//
//                        ScPlugin                     //
//=====================================================//

ScPlugin::ScPlugin()
	: QObject(nullptr)
{
}

ScPlugin::~ScPlugin()
{
}

bool ScPlugin::newPrefsPanelWidget( QWidget* /* parent */, Prefs_Pane*& /* panel */)
{
	return false;
}

const QString & ScPlugin::lastError() const
{
	return m_lastError;
}

bool ScPlugin::hasLastError() const
{
	return (!m_lastError.isEmpty());
}

void ScPlugin::clearLastError()
{
	m_lastError.clear();
}

const QString ScPlugin::pluginTypeName() const
{
	// These tests must be in reverse order of inheritance,
	// ie most specific to least specific.
	if (inherits("LoadSavePlugin"))
		return tr("Load/Save/Import/Export");
	if (inherits("ScPersistentPlugin"))
		return tr("Persistent", "plugin manager plugin type");
	if (inherits("ScActionPlugin"))
		return tr("Action", "plugin manager plugin type");
	qDebug("Unknown plugin type: %s", metaObject()->className());
	return tr("Unknown");
}

void ScPlugin::setDoc(ScribusDoc* )
{
}

void ScPlugin::unsetDoc()
{
}

void ScPlugin::changedDoc(ScribusDoc* )
{
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
bool ScActionPlugin::run(ScribusDoc* /*doc*/, QIODevice* /* target */)
{
	return false;
}

bool ScActionPlugin::run(QWidget *, ScribusDoc* /*doc*/, const QString& /* target */)
{
	return false;
}

// Stub for plugins that don't implement this method to inherit
DeferredTask* ScActionPlugin::runAsync(const QString& /* target */)
{
	return nullptr;
}


// Stub for plugins that don't implement this method to inherit
DeferredTask* ScActionPlugin::runAsync(QIODevice* /* target */)
{
	return nullptr;
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

bool ScActionPlugin::handleSelection(ScribusDoc* doc, int SelectedType)
{
	const int docSelectionCount = doc->m_Selection->count();
	ActionInfo ai(actionInfo());
	if (SelectedType != -1)
	{
		bool correctAppMode = false;
		if (ai.forAppMode.count() == 0)
			correctAppMode = true;
		else if (ai.forAppMode.contains(doc->appMode))
			correctAppMode = true;

		if (!correctAppMode)
			return false;

		if (ai.needsNumObjects == -1)
			return true;

		if (ai.needsNumObjects > 2)
		{
			bool setter = true;
			for (int bx = 0; bx < docSelectionCount; ++bx)
			{
				if (ai.notSuitableFor.contains(doc->m_Selection->itemAt(bx)->itemType()))
					setter = false;
			}
			return setter;
		}

		if (docSelectionCount != ai.needsNumObjects)
			return false;

		if (ai.needsNumObjects == 2)
		{
			int sel1 = doc->m_Selection->itemAt(0)->itemType();
			int sel2 = doc->m_Selection->itemAt(1)->itemType();
			if (ai.notSuitableFor.contains(sel1))
				return false;
			if (ai.notSuitableFor.contains(sel2))
				return false;
			if ((ai.firstObjectType.count() == 0) && (ai.secondObjectType.count() == 0))
				return true;
			if ((ai.firstObjectType.count() == 0) && (ai.secondObjectType.count() != 0))
			{
				if ((ai.secondObjectType.contains(sel2)) || (ai.secondObjectType.contains(sel1)))
					return true;
			}
			else if ((ai.firstObjectType.count() != 0) && (ai.secondObjectType.count() == 0))
			{
				if ((ai.firstObjectType.contains(sel2)) || (ai.firstObjectType.contains(sel1)))
					return true;
			}
			if (((ai.firstObjectType.contains(sel1)) && (ai.secondObjectType.contains(sel2))) || ((ai.firstObjectType.contains(sel2)) && (ai.secondObjectType.contains(sel1))))
				return true;
		}
		else if (!ai.notSuitableFor.contains(SelectedType))
			return true;
		else
			return false;
	}
	else
	{
		bool correctAppMode = false;
		if (ai.forAppMode.count() == 0)
			correctAppMode = true;
		else if (ai.forAppMode.contains(doc->appMode))
			correctAppMode = true;
		if (correctAppMode)
		{
			if (ai.needsNumObjects == -1)
				return true;
			if ((ai.needsNumObjects > 2) && (docSelectionCount > 0))
				return true;
			return false;
		}
	}
	return false;
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
