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
#include <cstdlib>
#include <string>
#include "scpaths.h"
#include "scribus.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "text/nlsconfig.h"

Hyphenator::Hyphenator(QWidget* parent, ScribusDoc *dok)
	: QObject( parent, "bu"),
	doc(dok),
	MinWordLen(doc->MinWordLen),
	HyCount(doc->HyCount),
	Automatic(doc->Automatic),
	AutoCheck(doc->AutoCheck),
	hdict(0),
	useAble(false),
	codec(0)
{
		//FIXME:av pick up language from charstyle
 	QString pfad = ScPaths::instance().libDir();
	if (ScMW->Sprachen.contains(doc->Language))
		Language = doc->Language;
	else
	{
		Language = PrefsManager::instance()->appPrefs.Language;
		doc->Language = Language;
	}
	pfad += "dicts/" + ScMW->Sprachen[Language];
	QFile f(pfad);
	if (f.open(IO_ReadOnly))
	{
		QTextStream st(&f);
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
	QCString fn = pfad.latin1();
	const char * filename = fn.data();
	hdict = hnj_hyphen_load(filename);
	useAble = hdict == NULL ? false : true;
}

Hyphenator::~Hyphenator()
{
	hnj_hyphen_free(hdict);
}

void Hyphenator::slotNewDict(QString name)
{
	if (!ScMW->Sprachen.contains(name))
		return;
	char *filename = NULL;
	if (hdict != NULL)
		hnj_hyphen_free(hdict);
 	QString pfad = ScPaths::instance().libDir();
	Language = name;
	doc->Language = name;
	pfad += "dicts/" + ScMW->Sprachen[Language];
	QFile f(pfad);
	if (f.open(IO_ReadOnly))
	{
		QTextStream st(&f);
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
	QCString fn = pfad.latin1();
	filename = fn.data();
	hdict = hnj_hyphen_load(filename);
	useAble = hdict == NULL ? false : true;
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

void Hyphenator::slotHyphenateWord(PageItem* it, QString text, int firstC)
{
	if ((!useAble))//FIXME:av || (!ScMW->Sprachen.contains(it->Language)))
		return;
	const char *word;
	char *buffer;
	const int BORDER = 2;
	QCString te;

	uint maxC = it->itemText.length() - 1;
	QString found = text;
	if (static_cast<int>(found.length()) > MinWordLen-1)
	{
  		te = codec->fromUnicode( found );
		word = te.data();
		buffer = static_cast<char*>(malloc(strlen(word)+BORDER+3));
		if (buffer == NULL)
			return;
		if (!hnj_hyphen_hyphenate(hdict, word, strlen(word), buffer))
		{
			uint i = 0;
		  	buffer[strlen(word)] = '\0';
			it->itemText.hyphenateWord(firstC, found.length(), buffer); 
/*#ifndef NLS_PROTO
			for (i = 1; i < found.length()-1; ++i)
				it->itemText.at(QMIN(maxC, i+firstC))->cstyle &= static_cast<StyleFlag>(1919);		// Delete any old Hyphens
			for (i = 1; i < found.length()-1; ++i)
			{
				if(buffer[i] & 1)
					it->itemText.at(QMIN(maxC, i+firstC))->cstyle |= static_cast<StyleFlag>(128);	// Set new Hyphens according Buffer
			}
#endif*/
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
	QCString te;

	int firstC = 0;
	if (it->HasSel)
	{
		firstC = -1;
		for (int a = 0; a < it->itemText.length(); ++a)
		{
			if (it->itemText.selected(a)) {
				if (firstC < 0)
					firstC = 0;
				text += it->itemText.text(a,1);
			}				
		}
	}
	else {
		text = it->itemText.text(0, it->itemText.length());
	}
		
	int lastC = 0;
	int Ccount = 0;
	QString found = "";
	QString found2 = "";
	uint maxC = it->itemText.length() - 1;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	while ((firstC+Ccount < static_cast<int>(text.length())) && (firstC != -1) && 
			(lastC < static_cast<int>(text.length())))
	{
		firstC = text.find(QRegExp("\\w"), firstC+Ccount);
		if (firstC < 0)
			break;
		lastC = text.find(QRegExp("\\s|\\W|\\d|\\n|\\r|\\t"), firstC);
		if (lastC < 0)
			lastC = static_cast<int>(text.length());
		Ccount = lastC - firstC;
		if (Ccount > MinWordLen-1)
		{
			found = text.mid(firstC, Ccount).lower();
			found2 = text.mid(firstC, Ccount);
  			te = codec->fromUnicode( found );
			word = te.data();
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
				if ( ! hasHyphen ) {
					it->itemText.hyphenateWord(firstC, found.length(), NULL);
				}
				else if (Automatic) {
					it->itemText.hyphenateWord(firstC, found.length(), buffer);
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
	if (!(it->asTextFrame()) || (it ->itemText.count() == 0))
		return;

	uint a = it->itemText.count();
	it->itemText.hyphenateWord(0, a, NULL);
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	doc->DoDrawing = true;
}
