/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef HUNSPELLPLUGINIMPL_H
#define HUNSPELLPLUGINIMPL_H

#include "hunspelldict.h"
#include "hunspellpluginstructs.h"

#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>

class QString;
class ScribusDoc;
class PageItem;
class StoryText;
class StoryEditor;



class HunspellPluginImpl : public QObject
{
	Q_OBJECT
	public:
		HunspellPluginImpl();
		~HunspellPluginImpl();
		bool run(const QString & target, ScribusDoc* doc=0);
		bool initHunspell();
		bool checkWithHunspell();
		bool checkWithHunspellSE();
		bool parseTextFrame(StoryText *);
		bool openGUIForTextFrame(StoryText *iText);
		bool openGUIForStoryEditor(StoryText *iText);
		void setRunningForSE(bool rfSE, StoryEditor *sE);
		QList<WordsFound> wordsToCorrect;

	protected:
		QMap<QString, QString> dictionaryMap;
		QStringList dictionaryPaths;
		//int numDicts, numAFFs;
		QMap<QString, HunspellDict*> hspellerMap;
		ScribusDoc* m_doc;
		bool m_runningForSE;
		StoryEditor* m_SE;
};

#endif

