/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          hyphenator.cpp  -  description
                             -------------------
    begin                : Sat Nov 02 08:00:00 CEST 2002
    copyright            : (C) 2002 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "hyphenator.h"
//#include "hyphenator.moc"
#include "hyask.h"

#include "scconfig.h"

#include <QRegExp>
#include <QCursor>
#include <QDir>
#include <QCheckBox>
//Added by qt3to4:
#include <QByteArray>
#include <cstdlib>
#include <string>
#include "langmgr.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "text/nlsconfig.h"

Hyphenator::Hyphenator(QWidget* parent, ScribusDoc *dok) : QObject( parent ),
	doc(dok),
	hdict(0),
	useAble(false),
	codec(0),
	MinWordLen(doc->MinWordLen),
	HyCount(doc->HyCount),
	Automatic(doc->Automatic),
	AutoCheck(doc->AutoCheck)
{
		//FIXME:av pick up language from charstyle
	QString pfad = ScPaths::instance().dictDir();
	LanguageManager * lmgr(LanguageManager::instance());
	if (!lmgr->getHyphFilename(doc->Language, false).isEmpty() )
		Language = doc->Language;
	else
	{
		Language = PrefsManager::instance()->appPrefs.Language;
		doc->Language = Language;
	}
// 	pfad += ScCore->primaryMainWindow()->Sprachen[Language];
	pfad += lmgr->getHyphFilename( doc->Language, false );
	QFile f(pfad);
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream st(&f);
    	QString line;
    	line = st.readLine();
		codec = QTextCodec::codecForName(line.toUtf8());
		f.close();
	}
	else
	{
		useAble = false;
		hdict = NULL;
		return;
	}
	QByteArray fn = pfad.toLocal8Bit();
	const char * filename = fn.data();
	hdict = hnj_hyphen_load(filename);
	useAble = hdict == NULL ? false : true;
	rememberedWords.clear();
/* Add reading these special lists from prefs or doc here */
	ignoredWords.clear();
	specialWords.clear();
}

Hyphenator::~Hyphenator()
{
	if (hdict)
		hnj_hyphen_free(hdict);
}

void Hyphenator::NewDict(const QString& name)
{
// 	if (!ScCore->primaryMainWindow()->Sprachen.contains(name))
// 		return;
	if( LanguageManager::instance()->getHyphFilename(name, false).isEmpty() )
		return;
		
	if (Language != name) 
	{
		Language = name;

		char *filename = NULL;
		QString pfad = ScPaths::instance().dictDir();
		
		if (hdict != NULL)
			hnj_hyphen_free(hdict);

		pfad +=  LanguageManager::instance()->getHyphFilename(Language, false) ;
		QFile f(pfad);
		if (f.open(QIODevice::ReadOnly))
		{
			QTextStream st(&f);
			QString line;
			line = st.readLine();
			codec = QTextCodec::codecForName(line.toUtf8());
			f.close();
		}
		else
		{
			useAble = false;
			hdict = NULL;
			return;
		}
		QByteArray fn = pfad.toLocal8Bit();
		filename = fn.data();
		hdict = hnj_hyphen_load(filename);
		useAble = hdict == NULL ? false : true;
	}
}

void Hyphenator::slotNewSettings(int Wordlen, bool Autom, bool ACheck, int Num)
{
	MinWordLen = Wordlen;
	Automatic = Autom;
	AutoCheck = ACheck;
	HyCount = Num;
	doc->MinWordLen = Wordlen;
	doc->Automatic = Autom;
	doc->AutoCheck = AutoCheck;
	doc->HyCount = Num;
}

void Hyphenator::slotHyphenateWord(PageItem* it, const QString& text, int firstC)
{
	if ((!useAble))//FIXME:av || (!ScMW->Sprachen.contains(it->Language)))
		return;
	const char *word;
	char *buffer;
	const int BORDER = 2;
	QByteArray te;

	//uint maxC = it->itemText.length() - 1;
	QString found = text;
	if (found.contains(SpecialChars::SHYPHEN))
		return;
	// else if (findException(found, &buffer) it->itemText.hyphenateWord(firstC, found.length(), buffer);
	else if (signed(found.length()) >= MinWordLen)
	{
		NewDict(it->itemText.charStyle(firstC).language());
  		te = codec->fromUnicode( found );
		word = te.data();
		int wordlen = strlen(word);
		buffer = static_cast<char*>(malloc(wordlen+BORDER+3));
		if (buffer == NULL)
			return;
		char ** rep = NULL;
		int * pos = NULL;
		int * cut = NULL;
		if (!hnj_hyphen_hyphenate2(hdict, word, wordlen, buffer, NULL, &rep, &pos, &cut))
		{
			//uint i = 0;
		  	buffer[wordlen] = '\0';
			it->itemText.hyphenateWord(firstC, found.length(), buffer); 
		}
		free(buffer);
		if (rep)
		{
			for (int i = 0; i < wordlen - 1; ++i)
				if (rep[i])
					free(rep[i]);
			free(rep);
		}
		if (pos) free(pos);
		if (cut) free(cut);
		buffer = NULL;
		rep = NULL;
		pos = NULL;
		cut = NULL;
	}
}

void Hyphenator::slotHyphenate(PageItem* it)
{
	if ((!useAble) || !(it->asTextFrame()) || (it->itemText.length() == 0))
		return;

	doc->DoDrawing = false;

	const char *word;
	char *buffer;
	const int BORDER = 2;
	QString text = "";
	QString buf;
	QByteArray te;

	int startC = 0;
	if (it->itemText.lengthOfSelection() > 0)
	{
		startC = it->itemText.startOfSelection();
		text = it->itemText.text(startC, it->itemText.lengthOfSelection());
	}
	else {
		text = it->itemText.text(0, it->itemText.length());
	}
	
	int firstC = 0;
	int lastC = 0;
	int Ccount = 0;
	QString found = "";
	QString found2 = "";
	rememberedWords.clear();
	//uint maxC = it->itemText.length() - 1;
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	QRegExp wordBoundary("\\w");
	QRegExp whiteSpace("\\s|\\W|\\d|\\n|\\r|\\t");
	while ((firstC+Ccount < signed(text.length())) && (firstC != -1) && 
			(lastC < signed(text.length())))
	{
		firstC = text.indexOf(wordBoundary, firstC+Ccount);
		if (firstC < 0)
			break;
		if (firstC > 0 && text.at(firstC-1) == SpecialChars::SHYPHEN)
		{
			Ccount = 1;
			continue;
		}
		lastC = text.indexOf(whiteSpace, firstC);
		if (lastC < 0)
			lastC = signed(text.length());
		Ccount = lastC - firstC;
		if (lastC < signed(text.length()) && text.at(lastC) == SpecialChars::SHYPHEN)
		{
			++Ccount;
			continue;
		}
		if (Ccount > MinWordLen-1)
		{
			found = text.mid(firstC, Ccount).toLower();
			found2 = text.mid(firstC, Ccount);
			if (found.contains(SpecialChars::SHYPHEN))
				break;

			NewDict(it->itemText.charStyle(firstC).language());

  			te = codec->fromUnicode( found );
			word = te.data();
			int wordlen = strlen(word);
			buffer = static_cast<char*>(malloc(wordlen+BORDER+3));
			if (buffer == NULL)
				break;
			char ** rep = NULL;
			int * pos = NULL;
			int * cut = NULL;
			if (!hnj_hyphen_hyphenate2(hdict, word, wordlen, buffer, NULL, &rep, &pos, &cut))
			{
	  			int i = 0;
  				buffer[wordlen] = '\0';
				bool hasHyphen = false;
				for (i = 1; i < found.length()-1; ++i)
				{
					if(buffer[i] & 1)
					{
						hasHyphen = true;
						break;
					}
				}
				QString outs = "";
				QString input = "";
				outs += found2[0];
				for (i = 1; i < found.length()-1; ++i)
				{
					outs += found2[i];
					if(buffer[i] & 1)
						outs += "-";
				}
				outs += found2.right(1);
				input = outs;
				if (!ignoredWords.contains(found2))
				{
					if (!hasHyphen)
						it->itemText.hyphenateWord(startC + firstC, found.length(), NULL);
					else if (Automatic)
					{
						if (specialWords.contains(found2))
						{
							outs = specialWords.value(found2);
							uint ii = 1;
							for (i = 1; i < outs.length()-1; ++i)
							{
								QChar cht = outs[i];
								if (cht == '-')
									buffer[ii-1] = 1;
								else
								{
									buffer[ii] = 0;
									++ii;
								}
							}
						}
						it->itemText.hyphenateWord(startC + firstC, found.length(), buffer);
					}
					else
					{
						if (specialWords.contains(found2))
						{
							outs = specialWords.value(found2);
							uint ii = 1;
							for (i = 1; i < outs.length()-1; ++i)
							{
								QChar cht = outs[i];
								if (cht == '-')
									buffer[ii-1] = 1;
								else
								{
									buffer[ii] = 0;
									++ii;
								}
							}
						}
						if (rememberedWords.contains(input))
						{
							outs = rememberedWords.value(input);
							uint ii = 1;
							for (i = 1; i < outs.length()-1; ++i)
							{
								QChar cht = outs[i];
								if (cht == '-')
									buffer[ii-1] = 1;
								else
								{
									buffer[ii] = 0;
									++ii;
								}
							}
							it->itemText.hyphenateWord(firstC, found.length(), buffer);
						}
						else
						{
							qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
							PrefsContext* prefs = PrefsManager::instance()->prefsFile->getContext("hyhpen_options");
							int xpos = prefs->getInt("Xposition", -9999);
							int ypos = prefs->getInt("Yposition", -9999);
							HyAsk *dia = new HyAsk((QWidget*)parent(), outs);
							if ((xpos != -9999) && (ypos != -9999))
								dia->move(xpos, ypos);
							qApp->processEvents();
							if (dia->exec())
							{
								outs = dia->Wort->text();
								uint ii = 1;
								for (i = 1; i < outs.length()-1; ++i)
								{
									QChar cht = outs[i];
									if (cht == '-')
										buffer[ii-1] = 1;
									else
									{
										buffer[ii] = 0;
										++ii;
									}
								}
								if (!rememberedWords.contains(input))
									rememberedWords.insert(input, outs);
								if (dia->addToIgnoreList->isChecked())
								{
									if (!ignoredWords.contains(found2))
										ignoredWords.insert(found2);
								}
								if (dia->addToExceptionList->isChecked())
								{
									if (!specialWords.contains(found2))
										specialWords.insert(found2, outs);
								}
								it->itemText.hyphenateWord(firstC, found.length(), buffer);
							}
							else
							{
								free(buffer);
								buffer = NULL;
								prefs->set("Xposition", dia->xpos);
								prefs->set("Yposition", dia->ypos);
								delete dia;
								break;
							}
							prefs->set("Xposition", dia->xpos);
							prefs->set("Yposition", dia->ypos);
							delete dia;
							qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
						}
					}
				}
			}
			free(buffer);
			if (rep)
			{
				for (int i = 0; i < wordlen - 1; ++i)
					if (rep[i])
						free(rep[i]);
				free(rep);
			}
			if (pos) free(pos);
			if (cut) free(cut);
			buffer = NULL;
			rep = NULL;
			pos = NULL;
			cut = NULL;
		}
		if (Ccount == 0)
			Ccount++;
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	doc->DoDrawing = true;
	rememberedWords.clear();
}

void Hyphenator::slotDeHyphenate(PageItem* it)
{
	if (!(it->asTextFrame()) || (it ->itemText.length() == 0))
		return;

	if (it->itemText.lengthOfSelection() > 0) 
	{
//		int start = it->itemText.startOfSelection();
		it->itemText.hyphenateWord(it->itemText.startOfSelection(), it->itemText.lengthOfSelection(), NULL);
	}
	else {
		uint a = it->itemText.length();
		it->itemText.hyphenateWord(0, a, NULL);
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	doc->DoDrawing = true;
}
