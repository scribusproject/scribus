/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scplugin.h"

#include "deferredtask.h"
#include "scribus.h"
#include "scribusapp.h"
#include "prefspanel.h"
#include "selection.h"

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
		qDebug("Unknown plugin type: %s", metaObject()->className());
		return tr("Unknown");
	}
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
	m_actionInfo.name = "";
	m_actionInfo.text = "";
	m_actionInfo.keySequence = "";
	m_actionInfo.menu = "";
	m_actionInfo.menuAfterName = "";
	m_actionInfo.parentMenu = "";
	m_actionInfo.subMenuName = "";
	m_actionInfo.toolbar = "";
	m_actionInfo.toolBarName = "";
	m_actionInfo.icon1 = QPixmap();
	m_actionInfo.icon2 = QPixmap();
	m_actionInfo.notSuitableFor.clear();
	m_actionInfo.forAppMode.clear();
	m_actionInfo.needsNumObjects = -1;
	m_actionInfo.firstObjectType.clear();
	m_actionInfo.secondObjectType.clear();
	m_actionInfo.enabledOnStartup = false;
}

ScActionPlugin::~ScActionPlugin()
{
}

// Stub for plugins that don't implement this method to inherit
bool ScActionPlugin::run(ScribusDoc* /*doc*/, QIODevice* /* target */)
{
	return false;
}

bool ScActionPlugin::run(QWidget *, ScribusDoc* /*doc*/, QString /* target */)
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

bool ScActionPlugin::handleSelection(ScribusDoc* doc, int SelectedType)
{
	const uint docSelectionCount=doc->m_Selection->count();
	ActionInfo ai(actionInfo());
	if (SelectedType != -1)
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
			else
			{
				if (ai.needsNumObjects > 2)
				{
					bool setter = true;
					for (uint bx = 0; bx < docSelectionCount; ++bx)
					{
						if (ai.notSuitableFor.contains(doc->m_Selection->itemAt(bx)->itemType()))
							setter = false;
					}
					return setter;
				}
				else
				{
					if (docSelectionCount == static_cast<uint>(ai.needsNumObjects))
					{
						if (ai.needsNumObjects == 2)
						{
							int sel1 = doc->m_Selection->itemAt(0)->itemType();
							int sel2 = doc->m_Selection->itemAt(1)->itemType();
							if (ai.notSuitableFor.contains(sel1))
								return false;
							else if (ai.notSuitableFor.contains(sel2))
								return false;
							else
							{
								if ((ai.firstObjectType.count() == 0) && (ai.secondObjectType.count() == 0))
									return true;
								else if ((ai.firstObjectType.count() == 0) && (ai.secondObjectType.count() != 0))
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
						}
						else if (!ai.notSuitableFor.contains(SelectedType))
							return true;
						else
							return false;
					}
					else
						return false;
				}
			}
		}
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
			if ((ai.needsNumObjects == -1) || (ai.needsNumObjects > 2))
				return true;
			else
				return false;
		}
		else
			return false;
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
