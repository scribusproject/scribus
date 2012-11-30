/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef HYSETTINGS_H
#define HYSETTINGS_H

#include "ui_hysettingsBase.h"

#include <QSet>
#include <QStringList>
#include <QHash>

#include "scribusapi.h"
#include "prefsstructs.h"
class ScribusDoc;

class SCRIBUS_API HySettings : public QWidget, Ui::hysettingsBase
{ 
	//TODO: Dict license showing, URL background unzipping, checksumming, pkg mgr platforms warning
	struct DictData
	{
		QString lang;
		QString version;
		QString files;
		QString url;
		QString desc;
		QString license;
		QString filetype;
		bool download;
	};

	Q_OBJECT

public:
	HySettings( QWidget* parent);
	~HySettings() {};
	void restoreDefaults(struct ApplicationPrefs *prefsData);
	void restoreDefaults(ScribusDoc *prefsData);
	int getWordLen();
	int getMaxCount();
	bool getVerbose();
	bool getInput();
	QString getLanguage();
	QSet<QString> getIgnoreList();
	QHash<QString, QString> getExceptionList();
	void setSpellingInvisible();

private slots:
	void addToIgnoreList();
	void editIgnoreListEntry();
	void removeIgnoreListEntry();
	void enableIgnoreButtons();
	void addToExceptList();
	void editExceptListEntry();
	void removeExceptListEntry();
	void enableExceptButtons();
	void downloadSpellDicts();
	void updateDictList();
	void updateAvailDictList();
	void downloadDictListFinished();
	void downloadSpellDictsFinished();
	void updateProgressBar();

private:
	QString affixFileName(QStringList files);
	QString dictFileName(QStringList files);
	void setAvailDictsXMLFile(QString availDictsXMLDataFile);

protected:
	QMap<QString, QString> dictionaryMap;
	QStringList dictionaryPaths;
	QString downloadLocation;
	QList <DictData> dictList;
	QList <DictData> downloadList;
};

#endif // HYSETTINGS_H
