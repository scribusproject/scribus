/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "hunspellpluginimpl.h"
#include "hunspelldialog.h"
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
	QStringList dirs(ScPaths::instance().spellDirs());
	if (dirs.count()==0)
		return false;
	//for development, just take the first for now
	dictPath=dirs.first();
	return true;
}

bool HunspellPluginImpl::initHunspell()
{
	int errorCount=0;
	bool dictPathFound=findDictionaries();
	if (!dictPathFound)
	{
		qDebug()<<"No preinstalled dictonary paths found";
		return false;
	}
	else
		qDebug()<<"Preinstalled dictionary path selected"<<dictPath;

	// Find the dic and aff files in the location
	QDir dictLocation(dictPath);
	QStringList dictFilters, affFilters;
	dictFilters << "*.dic";
	affFilters << "*.aff";
	QStringList dictList(dictLocation.entryList(dictFilters, QDir::Files, QDir::Name));
	dictList.replaceInStrings(".dic","");
	QStringList affList;

	//Ensure we have aff+dic file pairs, remove any hyphenation dictionaries from the list
	QString dictName;
	QStringListIterator dictListIterator(dictList);
	while (dictListIterator.hasNext())
	{
		dictName=dictListIterator.next();
		if (!QFile::exists(dictPath+dictName+".aff"))
			dictList.removeAll(dictName);
	}
	numAFFs=numDicts=dictList.count();
	qDebug()<<"Number of dictionaries/AFFs found:"<<numDicts<<numAFFs;
	if (numDicts==0)
		++errorCount;
	qDebug()<<dictList;

	//Initialise one hunspeller for each dictionary found
	//Maybe we only need the text language related one later on
	dictEntries=dictList;
	affEntries=affList;
	hspellers = new Hunspell* [numDicts];
	for (int i=0; i<numDicts; ++i)
		hspellers[i] = new Hunspell((dictPath+dictList.at(i)+".aff").toLocal8Bit().constData(),
									(dictPath+dictList.at(i)+".dic").toLocal8Bit().constData());

	return errorCount==0;
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
		++wordCount;
		++wordNo;
		QStringList replacements;
		if (hspellers[0]->spell(word.toUtf8().constData())==0)
		{
//			qDebug()<<word;
			++errorCount;
			char **sugglist = NULL;
			int suggCount=hspellers[0]->suggest(&sugglist, word.toUtf8().constData());
			for (int j=0; j < suggCount; ++j)
			{
//				qDebug()<<"Suggestion "<<j<<":"<<sugglist[j];
				replacements << QString::fromUtf8(sugglist[j]);
			}
			hspellers[0]->free_list(&sugglist, suggCount);

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
	hsDialog.set(&dictEntries, hspellers, &wordsToCorrect);
	hsDialog.exec();
	if (hsDialog.docChanged())
		m_doc->changed();
	return true;
}

bool HunspellPluginImpl::openGUIForStoryEditor(StoryText *iText)
{
	m_SE->setSpellActive(true);
	HunspellDialog hsDialog(m_SE, m_doc, iText);
	hsDialog.set(&dictEntries, hspellers, &wordsToCorrect);
	hsDialog.exec();
	m_SE->setSpellActive(false);
	return true;
}

void HunspellPluginImpl::setRunningForSE(bool rfSE, StoryEditor *sE)
{
	m_runningForSE=rfSE;
	m_SE=sE;
}

