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
#include <QHash>

#include "scribusapi.h"
#include "prefsstructs.h"
class ScribusDoc;

class SCRIBUS_API HySettings : public QWidget, Ui::hysettingsBase
{ 
	Q_OBJECT

public:
	HySettings( QWidget* parent/*, QMap<QString,QString>* langs*/);
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

private slots:
	void addToIgnoreList();
	void editIgnoreListEntry();
	void removeIgnoreListEntry();
	void enableIgnoreButtons();
	void addToExceptList();
	void editExceptListEntry();
	void removeExceptListEntry();
	void enableExceptButtons();

// protected:
// 	QMap<QString,QString> langsMap;
};

#endif // HYSETTINGS_H
