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
	hspellers=NULL;
	numDicts=0;
	m_runningForSE=false;
	m_SE=NULL;
}

HunspellPluginImpl::~HunspellPluginImpl()
{
	if (hspellers)
	{
		for (int i = 0; i < numDicts; ++i)
		{
			delete hspellers[i];
			hspellers[i] = NULL;
		}
		delete[] hspellers;
	}
	hspellers = NULL;
	numDicts = 0;
}

bool HunspellPluginImpl::run(const QString & target, ScribusDoc* doc)
{
	m_doc=doc;
	bool initOk=initHunspell();
	qDebug()<<"Hunspell Init Ok:"<<initOk;
	if (!initOk)
		return false;
	bool spellCheckOk=false;
	if (m_runningForSE)
	{
		//qDebug()<<"Running for StoryEditor";
		spellCheckOk=checkWithHunspellSE();
	}
	else
	{
		//qDebug()<<"Running for ScribusMainWindow";
		spellCheckOk=checkWithHunspell();
	}
	if (!spellCheckOk)
		return false;
	return true;
}

bool HunspellPluginImpl::findDictionaries()
{
	dictionaryPaths=ScPaths::instance().spellDirs();
	if (dictionaryPaths.count()==0)
		return false;
	return true;
}

bool HunspellPluginImpl::initHunspell()
{
	bool dictPathFound=findDictionaries();
	if (!dictPathFound)
	{
		qDebug()<<"No preinstalled dictonary paths found";
		return false;
	}
	for (int i=0; i<dictionaryPaths.count(); ++i)
	{
		// Find the dic and aff files in the location
		QDir dictLocation(dictionaryPaths.at(i));
		QStringList dictFilters;
		dictFilters << "*.dic";
		QStringList dictList(dictLocation.entryList(dictFilters, QDir::Files, QDir::Name));
		dictList.replaceInStrings(".dic","");

		//Ensure we have aff+dic file pairs, remove any hyphenation dictionaries from the list
		QString dictName;
		QStringListIterator dictListIterator(dictList);
		while (dictListIterator.hasNext())
		{
			dictName=dictListIterator.next();
			if (!QFile::exists(dictionaryPaths.at(i)+dictName+".aff"))
				dictList.removeAll(dictName);
			else
			{
				if (!dictionaryMap.contains(dictName))
					dictionaryMap.insert(dictName, dictionaryPaths.at(i)+dictName);
			}
		}
		qDebug()<<"Number of dictionaries/AFFs found in"<<dictionaryPaths.at(i)<<":"<<dictList.count();
	}
	numDicts=dictionaryMap.count();
	if (dictionaryMap.count()==0)
		return false;

	//Initialise one hunspeller for each dictionary found
	//Maybe we only need the text language related one later on
	int i=0;
	hspellers = new Hunspell* [numDicts];
	QMap<QString, QString>::iterator it = dictionaryMap.begin();
	while (it != dictionaryMap.end())
	{
		qDebug() << it.key()<< it.value();
		hspellers[i++] = new Hunspell((it.value()+".aff").toLocal8Bit().constData(),
									  (it.value()+".dic").toLocal8Bit().constData());
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
	int len=iText->length();
	int wordCount=0,wordNo=0,errorCount=0;
	int currPos=0;
	while (currPos<len)
	{
		int wordPos=iText->nextWord(currPos);
		currPos=wordPos;
		int eoWord=wordPos;
		while(eoWord < len)
		{
			if (iText->text(eoWord).isLetterOrNumber())
				++eoWord;
			else
				break;
		}
		QString word=iText->text(wordPos,eoWord-wordPos);
		QString wordLang=iText->charStyle(wordPos).language();
		//qDebug()<<word<<"is set to be in language"<<wordLang;
		wordLang=LanguageManager::instance()->getAbbrevFromLang(wordLang, true);
		if (wordLang=="en")
			wordLang="en_GB";
		int spellerIndex=0;
		if (!dictionaryMap.contains(wordLang))
			qDebug()<<"Spelling language to match style language not installed ("<<wordLang<<")";
		else
		{
			int i=0;
			QMap<QString, QString>::iterator it = dictionaryMap.begin();
			while (it != dictionaryMap.end())
			{
				if (it.key()==wordLang)
					break;
				++i;
				++it;
			}
			spellerIndex=i;
		}
		++wordCount;
		++wordNo;
		QStringList replacements;
		if (hspellers[spellerIndex]->spell(word.toUtf8().constData())==0)
		{
//			qDebug()<<word;
			++errorCount;
			char **sugglist = NULL;
			int suggCount=hspellers[spellerIndex]->suggest(&sugglist, word.toUtf8().constData());
			for (int j=0; j < suggCount; ++j)
			{
//				qDebug()<<"Suggestion "<<j<<":"<<sugglist[j];
				replacements << QString::fromUtf8(sugglist[j]);
			}
			hspellers[spellerIndex]->free_list(&sugglist, suggCount);

			struct WordsFound wf;
			wf.start=currPos;
			wf.end=eoWord;
			wf.w=word;
			wf.replacements=replacements;
			wf.changed=false;
			wf.ignore=false;
			wf.changeOffset=0;
			wordsToCorrect.append(wf);
		}
	}
//	qDebug()<<"Errors found:"<<errorCount;
	return true;
}

bool HunspellPluginImpl::openGUIForTextFrame(StoryText *iText)
{
	HunspellDialog hsDialog(m_doc->scMW(), m_doc, iText);
	hsDialog.set(&dictionaryMap, hspellers, &wordsToCorrect);
	hsDialog.exec();
	if (hsDialog.docChanged())
		m_doc->changed();
	return true;
}

bool HunspellPluginImpl::openGUIForStoryEditor(StoryText *iText)
{
	m_SE->setSpellActive(true);
	HunspellDialog hsDialog(m_SE, m_doc, iText);
	hsDialog.set(&dictionaryMap, hspellers, &wordsToCorrect);
	hsDialog.exec();
	m_SE->setSpellActive(false);
	return true;
}

void HunspellPluginImpl::setRunningForSE(bool rfSE, StoryEditor *sE)
{
	m_runningForSE=rfSE;
	m_SE=sE;
}

