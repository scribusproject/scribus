#include "scriptpaths.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"

#include <QWidget>
#include "menumanager.h"

ScriptPaths::ScriptPaths()
{
}

void ScriptPaths::saveToPrefs() const
{
	paths.saveToPrefs();
}

/**
 * Add a Scripts sub menu to the Scripts navigation (if there is any user script).
 * It will be hidden if buildMenu() detects that no scripts have been found.
 */
void ScriptPaths::attachToMenu(MenuManager* menuManager)
{
	menuManager->createMenu("OwnScripts", QObject::tr("&Scripts"), "Scripter", false, true);
	menuManager->addMenuItemString("OwnScripts", "Scripter");
	this->menuManager = menuManager;
}

void ScriptPaths::buildMenu()
{
	if (menuManager == nullptr)
		return;

	updateScriptsList();

	if (empty())
	{
		menuManager->hideRemberedMenu("OwnScripts");
		return;
	}

	menuManager->showRemberedMenu("OwnScripts");

	scriptsActions.clear();
	menuManager->clearMenuStrings("OwnScripts");

	for (const auto& scriptInfo: std::as_const(scriptsList))
	{
		// TODO: strippedName should be unique for the whole menu (repo index + script name?)... we could use the full path...
		auto scriptName = scriptInfo.name;
		scriptName.replace('_', ' ');
		scriptName = scriptName.first(1).toUpper() + scriptName.mid(1);
		scriptsActions.insert(scriptName, new ScrAction(ScrAction::OwnScript, scriptName, QKeySequence(), this, scriptInfo.path));
		connect(
		    scriptsActions[scriptName], qOverload<QString>(&ScrAction::triggeredData),
			this, &ScriptPaths::runScript
		);
		menuManager->addMenuItemString(scriptName, "OwnScripts");
	}

	menuManager->addMenuItemStringsToRememberedMenu("OwnScripts", scriptsActions);
}

/**
 * Detect the scripts in the paths defined in the Preferences > Scripter.
 *
 * The following scripts are detected:
 *
 * - The *.py files at the root of the repository.
 * - The main.py files in the directories at the root of the repository.
 * - The .py files with the same name as their directory (at root level):
 *   the-script/the-script.py
 *
 *   TODO: we might prefer using a QList<QPair<QString, QString>> with the name to be shown and the absolute path to the script.
 */
void ScriptPaths::updateScriptsList()
{
	scriptsList.clear();
	// TODO: how to manage duplicate file names? (in different repositories)
	for (const auto& path: std::as_const(paths.paths)) {
		{
			QDir scriptDirectory(QDir::toNativeSeparators(path), "*.py", QDir::Name | QDir::IgnoreCase, QDir::Files | QDir::NoSymLinks);
			for (const auto& file: scriptDirectory.entryList())
			{
				scriptsList.append(ScriptPathsInfo(QDir(path).filePath(file), QFileInfo{file}.baseName()));
			}
		}
		{
			QDir scriptDirectory(QDir::toNativeSeparators(path), "*", QDir::Name | QDir::IgnoreCase, QDir::Dirs | QDir::NoSymLinks);
			for (const auto& directory: scriptDirectory.entryList())
			{
				{
					QFileInfo script(QDir(path).filePath(directory + "/main.py"));
					if (script.exists() || script.isFile())
					{
						scriptsList.append(ScriptPathsInfo(script.absoluteFilePath(), directory));
						continue;
					}
				}
				{
					QFileInfo script(QDir(path).filePath(directory + "/" + directory + ".py"));
					if (script.exists() || script.isFile())
					{
						scriptsList.append(ScriptPathsInfo(script.absoluteFilePath(), directory));
					}
				}
			}
		}
	}
	std::sort(scriptsList.begin(), scriptsList.end());
}

void ScriptPaths::runScript(const QString& path)
{
	// TODO: give some sort of warning?
	if (!QFileInfo::exists(path))
		return;

	emit runScriptFile(path);
}

QStringList ScriptPaths::get()
{
	return paths.paths;
}

void ScriptPaths::append(const QString& s)
{
	paths.paths.append(s);
}

void ScriptPaths::remove(int i)
{
	paths.paths.remove(i);
}

void ScriptPaths::clear()
{
	paths.paths.clear();
}

QString ScriptPaths::item(int i) const
{
	return paths.paths.at(i);
}

ScriptPathsList::ScriptPathsList()
{
	PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("scriptplugin");
	if (!prefs)
		return;

	auto scriptPaths = prefs->getTable("scriptpaths");
	for (int i = 0; i < scriptPaths->getRowCount(); ++i)
	{
		paths.append(QDir::toNativeSeparators(scriptPaths->get(i, 0)));
	}
}

void ScriptPathsList::saveToPrefs() const
{
	PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("scriptplugin");
	if (!prefs)
		return;

	auto scriptPaths = prefs->getTable("scriptpaths");
	scriptPaths->clear();
	for (int i = 0; i < paths.count(); i++)
	{
		// pathListWidget->item(i)->text()
		scriptPaths->set(i, 0, QDir::fromNativeSeparators(paths[i]));
	}
}
