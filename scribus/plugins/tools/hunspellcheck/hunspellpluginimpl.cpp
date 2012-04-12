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
#include "scribusdoc.h"
#include "scribus.h"
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
	bool spellCheckOk=checkWithHunspell();
	if (!spellCheckOk)
		return false;
	return true;
}

bool HunspellPluginImpl::findDictionaries()
{
	//dictionaryPaths
	QString macPortsPath("/opt/local/share/hunspell/");
	QString finkPath("/sw/share/hunspell/");
	QString osxLibreOfficePath("/Applications/LibreOffice.app/Contents/share/extensions");
	QString osxUserLibreOfficePath(QDir::homePath()+"/Applications/LibreOffice.app/Contents/share/extensions");
	QString linuxLocalPath("/usr/local/share/hunspell/");
	QString linuxPath("/usr/share/hunspell/");
	QString windowsLOPath("LibreOffice 3.5/share/extensions");
	QDir d;

#ifdef Q_OS_MAC
	d.setPath(macPortsPath);
	if (d.exists())
	{
		dictPath=macPortsPath;
		return true;
	}
	d.setPath(finkPath);
	if (d.exists())
	{
		dictPath=finkPath;
		return true;
	}
	d.setPath(osxLibreOfficePath);
	if (d.exists())
	{
		dictPath=osxLibreOfficePath;
		return true;
	}
	d.setPath(osxUserLibreOfficePath);
	if (d.exists())
	{
		dictPath=osxUserLibreOfficePath;
		return true;
	}
	return false;
#elif defined(Q_WS_X11)
	d.setPath(linuxPath);
	if (d.exists())
	{
		dictPath=linuxPath;
		return true;
	}
	d.setPath(linuxLocalPath);
	if (d.exists())
	{
		dictPath=linuxLocalPath;
		return true;
	}
#elif defined(Q_OS_WIN32)
	QString progFiles = getSpecialDir(CSIDL_PROGRAM_FILES);
	d.setPath(progFiles+windowsLOPath);
	if (d.exists())
	{
		dictPath=progFiles+windowsLOPath;
		return true;
	}
#endif
	return false;
}

bool HunspellPluginImpl::initHunspell()
{
	int errorCount=0;
	bool dictPathFound=findDictionaries();
	if (!dictPathFound)
		qDebug()<<"No preinstalled dictonary paths found";
	else
		qDebug()<<"Preinstalled dictionary path selected"<<dictPath;
	//TODO: Find this somehow
//	QString startPath;
//	startPath="/Applications/LibreOffice.app/Contents/share/extensions/dict-en/";
//	dictPath=startPath;

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
	numDicts=dictList.count();
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
		parseTextFrame(frameToCheck);
		openGUIForTextFrame(frameToCheck);
		m_doc->view()->DrawNew();
	}
	return true;
}

bool HunspellPluginImpl::parseTextFrame(PageItem *frameToCheck)
{
	static QString wordBoundaries(" .,:;\"'!?\n");

	StoryText *iText=&frameToCheck->itemText;
	int len=iText->length();
	QString text=iText->text(0,len);
//	qDebug()<<text;
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
		if (hspellers[0]->spell(word.toLocal8Bit().constData())==0)
		{
//			qDebug()<<word;
			++errorCount;
			char **sugglist = NULL;
			int suggCount=hspellers[0]->suggest(&sugglist, word.toLocal8Bit().constData());
			for (int j=0; j < suggCount; ++j)
			{
//				qDebug()<<"Suggestion "<<j<<":"<<sugglist[j];
				replacements<<sugglist[j];
			}
			hspellers[0]->free_list(&sugglist, suggCount);

			struct WordsFound wf;
			wf.start=currPos;
			wf.end=eoWord;
			wf.w=word;
			wf.replacements=replacements;
			wf.changed=false;
			wf.ignore=false;
			wordsToCorrect.append(wf);
		}
	}
//	qDebug()<<"Errors found:"<<errorCount;
	return true;
}

bool HunspellPluginImpl::openGUIForTextFrame(PageItem *frameToCheck)
{
	HunspellDialog hsDialog(m_doc->scMW(), m_doc, frameToCheck);
	hsDialog.set(&dictEntries, hspellers, &wordsToCorrect);
	hsDialog.exec();
	if (hsDialog.docChanged())
		m_doc->changed();
	return true;
}

