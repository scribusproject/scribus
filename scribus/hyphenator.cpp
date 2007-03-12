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
#include "hyphenator.moc"
#include "hyask.h"

#include "scconfig.h"

#include <qregexp.h>
#include <qcursor.h>
#include <qdir.h>
//Added by qt3to4:
#include <Q3CString>
#include <cstdlib>
#include <string>
#include "scpaths.h"
#include "scribuscore.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "text/nlsconfig.h"

Hyphenator::Hyphenator(QWidget* parent, ScribusDoc *dok)
	: QObject( parent, "bu"),
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
 	QString pfad = ScPaths::instance().libDir();
	if (ScCore->primaryMainWindow()->Sprachen.contains(doc->Language))
		Language = doc->Language;
	else
	{
		Language = PrefsManager::instance()->appPrefs.Language;
		doc->Language = Language;
	}
	pfad += "dicts/" + ScCore->primaryMainWindow()->Sprachen[Language];
	QFile f(pfad);
	if (f.open(QIODevice::ReadOnly))
	{
		Q3TextStream st(&f);
    	QString line;
    	line = st.readLine();
		codec = QTextCodec::codecForName(line);
		f.close();
	}
	else
	{
		useAble = false;
		hdict = NULL;
		return;
	}
	Q3CString fn = pfad.latin1();
	const char * filename = fn.data();
	hdict = hnj_hyphen_load(filename);
	useAble = hdict == NULL ? false : true;
}

Hyphenator::~Hyphenator()
{
	hnj_hyphen_free(hdict);
}

void Hyphenator::NewDict(const QString& name)
{
	if (!ScCore->primaryMainWindow()->Sprachen.contains(name))
		return;

	if (Language != name) 
	{
		Language = name;

		char *filename = NULL;
		QString pfad = ScPaths::instance().libDir();
		
		if (hdict != NULL)
			hnj_hyphen_free(hdict);

		pfad += "dicts/" + ScCore->primaryMainWindow()->Sprachen[Language];
		QFile f(pfad);
		if (f.open(QIODevice::ReadOnly))
		{
			Q3TextStream st(&f);
			QString line;
			line = st.readLine();
			codec = QTextCodec::codecForName(line);
			f.close();
		}
		else
		{
			useAble = false;
			hdict = NULL;
			return;
		}
		Q3CString fn = pfad.latin1();
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
	Q3CString te;

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
		buffer = static_cast<char*>(malloc(strlen(word)+BORDER+3));
		if (buffer == NULL)
			return;
		if (!hnj_hyphen_hyphenate(hdict, word, strlen(word), buffer))
		{
			//uint i = 0;
		  	buffer[strlen(word)] = '\0';
			it->itemText.hyphenateWord(firstC, found.length(), buffer); 
		}
		free(buffer);
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
	Q3CString te;

	int startC = 0;
	if (it->HasSel)
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
	//uint maxC = it->itemText.length() - 1;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	QRegExp wordBoundary("\\w");
	QRegExp whiteSpace("\\s|\\W|\\d|\\n|\\r|\\t");
	while ((firstC+Ccount < signed(text.length())) && (firstC != -1) && 
			(lastC < signed(text.length())))
	{
		firstC = text.find(wordBoundary, firstC+Ccount);
		if (firstC < 0)
			break;
		if (firstC > 0 && text.at(firstC-1) == SpecialChars::SHYPHEN)
		{
			Ccount = 1;
			continue;
		}
		lastC = text.find(whiteSpace, firstC);
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
			found = text.mid(firstC, Ccount).lower();
			found2 = text.mid(firstC, Ccount);
			if (found.contains(SpecialChars::SHYPHEN))
				break;

			NewDict(it->itemText.charStyle(firstC).language());

  			te = codec->fromUnicode( found );
			word = te.data();
			/*
			qDebug(QString("hyphenate %1: len %2 and %3 in codec %4")
				   .arg(Language)
				   .arg(found2.length())
				   .arg(strlen(word))
				   .arg(codec->name()));
			 */
			buffer = static_cast<char*>(malloc(strlen(word)+BORDER+3));
			if (buffer == NULL)
				break;
			if (!hnj_hyphen_hyphenate(hdict, word, strlen(word), buffer))
			{
	  			uint i = 0;
  				buffer[strlen(word)] = '\0';
				bool hasHyphen = false;
				for (i = 1; i < found.length()-1; ++i)
				{
					if(buffer[i] & 1)
					{
						hasHyphen = true;
						break;
					}
				}
				/*
				QString dump(""), dump2("");
				for (i=0; i < strlen(word); ++i) 
				{
					dump += QChar(word[i]);
					if ( i < found.length() )
						dump2 += found[i];
					if (buffer[i] & 1)
					{
						dump += "-";
						dump2 += "-";
					}
				}
				qDebug(QString("hy %3+%4: %1 / %2")
					   .arg(dump)
					   .arg(dump2)
					   .arg(startC)
					   .arg(firstC));
				 */
				if ( ! hasHyphen ) {
					it->itemText.hyphenateWord(startC + firstC, found.length(), NULL);
				}
				else if (Automatic) {
					it->itemText.hyphenateWord(startC + firstC, found.length(), buffer);
				}
				else {
					QString outs = "";
					outs += found2[0];
					for (i = 1; i < found.length()-1; ++i)
					{
						outs += found2[i];
						if(buffer[i] & 1)
							outs += "-";
					}
					outs += found2.right(1);
					qApp->setOverrideCursor(QCursor(ArrowCursor), true);
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
						uint ii = 0;
						for (i = 1; i < outs.length()-1; ++i)
						{
							QChar cht = outs[i];
							if (cht == "-") 
								buffer[ii] = 1;
							else
								ii++;
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
					qApp->setOverrideCursor(QCursor(waitCursor), true);
				}
			}
			free(buffer);
			buffer = NULL;
		}
		if (Ccount == 0)
			Ccount++;
	}
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	doc->DoDrawing = true;
}

void Hyphenator::slotDeHyphenate(PageItem* it)
{
	if (!(it->asTextFrame()) || (it ->itemText.length() == 0))
		return;

	uint a = it->itemText.length();
	it->itemText.hyphenateWord(0, a, NULL);
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	doc->DoDrawing = true;
}
