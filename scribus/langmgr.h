#ifndef LANGMANAGER_H
#define LANGMANAGER_H

#include <qmap.h>
#include <qstring.h>

class LanguageManager
{
public:
	void init();
	const QString getLangFromAbbrev(QString);
	const QString getAbbrevFromLang(QString);
	void fillInstalledStringList(QStringList *stringListToFill, bool addDefaults);
	void printInstalledList();

private:
	QMap<QString, QString> langList;
	QMap<QString, QString> installedLangList;

	void generateLangList();
	void generateInstalledLangList();
	
};

#endif

