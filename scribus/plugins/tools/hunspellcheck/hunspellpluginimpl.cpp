/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "hunspellpluginimpl.h"
#include "hunspelldialog.h"
#include "langmgr.h"
#include "pageitem.h"
#include "pageitem_textframe.h"
#include "selection.h"
#include "scpaths.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "storyeditor.h"
#include "text/specialchars.h"
#include "util.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <shlobj.h>
#endif


// Initialize members here, if any
HunspellPluginImpl::HunspellPluginImpl() : QObject(0)
{
	//hspellers=NULL;
//	numDicts=0;
	m_runningForSE=false;
	m_SE=NULL;
}

HunspellPluginImpl::~HunspellPluginImpl()
{
	foreach (HunspellDict* h, hspellerMap)
	{
		delete h;
		h = NULL;
	}
	hspellerMap.clear();
//	numDicts = 0;
}

bool HunspellPluginImpl::run(const QString & target, ScribusDoc* doc)
{
	m_doc=doc;
	bool initOk=initHunspell();
	if (!initOk)
		return false;
	bool spellCheckOk=false;
	if (m_runningForSE)
		spellCheckOk=checkWithHunspellSE();
	else
		spellCheckOk=checkWithHunspell();
	return spellCheckOk;
}

bool HunspellPluginImpl::initHunspell()
{
	bool dictPathFound=LanguageManager::instance()->findDictionaries(dictionaryPaths);
	if (!dictPathFound)
	{
		qDebug()<<"No preinstalled dictonary paths found";
		return false;
	}
	dictionaryMap.clear();
	LanguageManager::instance()->findDictionarySets(dictionaryPaths, dictionaryMap);
//	numDicts=dictionaryMap.count();
	if (dictionaryMap.count()==0)
		return false;

	//Initialise one hunspeller for each dictionary found
	QMap<QString, QString>::iterator it = dictionaryMap.begin();
	while (it != dictionaryMap.end())
	{
		hspellerMap.insert(it.key(), new HunspellDict(it.value()+".aff", it.value()+".dic"));
		++it;
	}
	return true;
}

bool HunspellPluginImpl::checkWithHunspell()
{
	PageItem *frameToCheck;

	for( int i = 0; i < m_doc->m_Selection->count(); ++i )
	{
		frameToCheck = m_doc->m_Selection->itemAt(i);
		StoryText *iText=&frameToCheck->itemText;
		parseTextFrame(iText);
		openGUIForTextFrame(iText);
		m_doc->view()->DrawNew();
	}
	return true;
}

bool HunspellPluginImpl::checkWithHunspellSE()
{
	StoryText *iText=&(m_SE->Editor->StyledText);
	parseTextFrame(iText);
	openGUIForStoryEditor(iText);
	m_SE->Editor->updateAll();
	return true;
}

bool HunspellPluginImpl::parseTextFrame(StoryText *iText)
{
	int len = iText->length();
	int currPos = iText->firstWord(), wordStart;
	while (currPos < len)
	{
		wordStart = currPos;
		int wordEnd = iText->endOfWord(wordStart);
		QString word = iText->text(wordStart,wordEnd - wordStart);
		QString wordLang = iText->charStyle(wordStart).language();
		//qDebug()<<wordLang<<LanguageManager::instance()->getAbbrevFromLang(wordLang, false, false, 1)<<LanguageManager::instance()->getAbbrevFromLang(wordLang, true, false, 2);
		QString langAbbrev = LanguageManager::instance()->getAbbrevFromLang(wordLang, false, false, 1);
		//A little hack as for some reason our en dictionary from the aspell plugin was not called en_GB or en_US but en, content was en_GB though. Meh.
		if (langAbbrev == "en")
			langAbbrev = "en_GB";
		int spellerIndex = 0;
		if (!dictionaryMap.contains(langAbbrev))
		{
			//qDebug()<<"Spelling language to match style language not installed ("<<langAbbrev<<")";
			QString langAbbrev2=LanguageManager::instance()->getAbbrevFromLang(wordLang, false, false, 2);
			if (!langAbbrev2.isEmpty() && dictionaryMap.contains(langAbbrev2))
			{
				//qDebug()<<"Spelling language swapped to :"<<langAbbrev2;
				langAbbrev=langAbbrev2;
			}
		}
		else
		{
			int i=0;
			QMap<QString, QString>::iterator it = dictionaryMap.begin();
			while (it != dictionaryMap.end())
			{
				if (it.key()==langAbbrev)
					break;
				++i;
				++it;
			}
			spellerIndex=i;
		}
		if (hspellerMap.contains(langAbbrev) && hspellerMap[langAbbrev]->spell(word)==0)
		{
			struct WordsFound wf;
			wf.start=wordStart;
			wf.end=wordEnd;
			wf.w=word;
			wf.changed=false;
			wf.ignore=false;
			wf.changeOffset=0;
			wf.lang = langAbbrev;
			wf.replacements = hspellerMap[langAbbrev]->suggest(word);
			wordsToCorrect.append(wf);
		}
		currPos = iText->nextWord(wordStart);
	}
	return true;
}

bool HunspellPluginImpl::openGUIForTextFrame(StoryText *iText)
{
	HunspellDialog hsDialog(m_doc->scMW(), m_doc, iText);
	hsDialog.set(&dictionaryMap, &hspellerMap, &wordsToCorrect);
	hsDialog.exec();
	if (hsDialog.docChanged())
		m_doc->changed();
	return true;
}

bool HunspellPluginImpl::openGUIForStoryEditor(StoryText *iText)
{
	m_SE->setSpellActive(true);
	HunspellDialog hsDialog(m_SE, m_doc, iText);
	hsDialog.set(&dictionaryMap, &hspellerMap, &wordsToCorrect);
	hsDialog.exec();
	m_SE->setSpellActive(false);
	return true;
}

void HunspellPluginImpl::setRunningForSE(bool rfSE, StoryEditor *sE)
{
	m_runningForSE=rfSE;
	m_SE=sE;
}

