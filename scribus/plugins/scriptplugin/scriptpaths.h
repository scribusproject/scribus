/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCRIPTPATHS_H
#define SCRIPTPATHS_H

/**
 * Allow the user to add local script to the Script menu and run them.
 */

#include <QStringList>
#include <QPointer>
#include "scraction.h"

class MenuManager;

struct ScriptPathsList
{
		ScriptPathsList();
		void saveToPrefs() const;
		QStringList paths;
};

struct ScriptPathsInfo
{
	ScriptPathsInfo(QString path, QString name): path{path}, name{name} {}

	QString path;
	QString name;

	friend bool operator<(const ScriptPathsInfo& l, const ScriptPathsInfo& r)
	{
		return l.name < r.name;
	}
};

class ScriptPaths : public QObject
{
	Q_OBJECT

public:
	ScriptPaths();
	void saveToPrefs() const;

	void updateScriptsList();
	void attachToMenu(MenuManager* menuManager);
	void buildMenu();

	bool empty() { return paths.paths.count() == 0; }
	QStringList get();
	void append(const QString& s);
	void remove (int i);
	void clear ();
	QString item(int i) const;

signals:
	void runScriptFile(const QString& path);

public slots:
	void runScript(const QString& name);

private:
	MenuManager* menuManager = nullptr;
	QList<ScriptPathsInfo> scriptsList;

	ScriptPathsList paths;
	QMap<QString, QPointer<ScrAction>> scriptsActions;
};

#endif
