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

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#include <qregexp.h>
#include <qcursor.h>
#include <qdir.h>
#include <cstdlib>
#include <string>
#include "scribus.h"

Hyphenator::Hyphenator(QWidget* parent, ScribusDoc *dok, ScribusApp* app) : QObject( parent, "bu")
{
  char *filename = NULL;
	doc = dok;
	Sap = app;
	MinWordLen = doc->MinWordLen;
	Automatic = doc->Automatic;
	AutoCheck = doc->AutoCheck;
 	QString pfad = PREL;
	if (Sap->Sprachen.contains(doc->Language))
		Language = doc->Language;
	else
		{
		Language = Sap->Prefs.Language;
		doc->Language = Language;
		}
	pfad += "/lib/scribus/dicts/" + Sap->Sprachen[Language];
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
	if (hdict == NULL)
		useAble = false;
	else
		useAble = true;
}

Hyphenator::~Hyphenator()
{
	hnj_hyphen_free(hdict);
}

void Hyphenator::slotNewDict(QString name)
{
  char *filename = NULL;
	if (!Sap->Sprachen.contains(name))
		return;
	if (hdict != NULL)
		hnj_hyphen_free(hdict);
 	QString pfad = PREL;
	Language = name;
	doc->Language = name;
	pfad += "/lib/scribus/dicts/" + Sap->Sprachen[Language];
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
	if (hdict == NULL)
		useAble = false;
	else
		useAble = true;
}

void Hyphenator::slotNewSettings(int Wordlen, bool Autom, bool ACheck)
{
	MinWordLen = Wordlen;
	Automatic = Autom;
	AutoCheck = ACheck;
	doc->MinWordLen = Wordlen;
	doc->Automatic = Autom;
	doc->AutoCheck = AutoCheck;
}

void Hyphenator::slotHyphenateWord(PageItem* it, QString text, int firstC)
{
	const char *word;
  char *buffer;
  const int BORDER = 2;
	QCString te;
	if (!useAble)
		return;
	if (!Sap->Sprachen.contains(it->Language))
		return;
	uint maxC = it->Ptext.count() - 1;
	QString found = text;
	if (static_cast<int>(found.length()) > MinWordLen)
		{
  	te = codec->fromUnicode( found );
		word = te.data();
		buffer = static_cast<char*>(malloc(strlen(word)+BORDER+3));
		if (buffer == NULL)
			return;
		if (!hnj_hyphen_hyphenate(hdict, word, strlen(word), buffer));
			{
			uint i = 0;
	  	buffer[strlen(word)] = '\0';
			for (i = 1; i < found.length()-1; ++i)
				{
				it->Ptext.at(QMIN(maxC, i+firstC))->cstyle &= 127;		// Delete any old Hyphens
				}
			for (i = 1; i < found.length()-1; ++i)
				{
				if(buffer[i] & 1)
					it->Ptext.at(QMIN(maxC, i+firstC))->cstyle ^= 128;	// Set new Hyphens according Buffer
				}
			}
		free(buffer);
		}
}

void Hyphenator::slotHyphenate(PageItem* it)
{
	const char *word;
  char *buffer;
  const int BORDER = 2;
	QString text = "";
	QString buf;
	QCString te;
	if (!useAble)
		return;
	if (it->PType != 4)
		return;
	if (it->Ptext.count() == 0)
		return;
	for (uint a = 0; a < it->Ptext.count(); ++a)
		{
		if (it->HasSel)
			{
			if (it->Ptext.at(a)->cselect)
				text += it->Ptext.at(a)->ch;
			}
		else
			text += it->Ptext.at(a)->ch;
		}
	int firstC = 0;
	if (it->HasSel)
		{
		for (uint a = 0; a < it->Ptext.count(); ++a)
			{
			if (it->Ptext.at(a)->cselect)
				{
				firstC = static_cast<int>(a);
				break;
				}
			}
		}
	int lastC = 0;
	int Ccount = 0;
	QString found = "";
	QString found2 = "";
	uint maxC = it->Ptext.count() - 1;
  qApp->setOverrideCursor(QCursor(waitCursor), true);
	while ((firstC+Ccount < static_cast<int>(text.length())) && (firstC != -1) && (lastC < static_cast<int>(text.length())))
		{
		firstC = text.find(QRegExp("\\w"), firstC+Ccount);
		if (firstC < 0)
			break;
		lastC = text.find(QRegExp("\\s|\\W|\\d|\\n|\\r|\\t"), firstC);
		if (lastC < 0)
			lastC = static_cast<int>(text.length());
		Ccount = lastC - firstC;
		if (Ccount > MinWordLen)
			{
			found = text.mid(firstC, Ccount).lower();
			found2 = text.mid(firstC, Ccount);
  		te = codec->fromUnicode( found );
			word = te.data();
			buffer = static_cast<char*>(malloc(strlen(word)+BORDER+3));
			if (buffer == NULL)
				break;
			if (!hnj_hyphen_hyphenate(hdict, word, strlen(word), buffer));
				{
	  		uint i = 0;
  			buffer[strlen(word)] = '\0';
				for (i = 1; i < found.length()-1; ++i)
					{
					it->Ptext.at(QMIN(maxC, i+firstC))->cstyle &= 127;		// Delete any old Hyphens
					}
				bool hasHyphen = false;
				for (i = 1; i < found.length()-1; ++i)
					{
					if(buffer[i] & 1)
						{
						hasHyphen = true;
						break;
						}
					}
				if (hasHyphen)
					{
					if (!Automatic)
						{
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
						HyAsk *dia = new HyAsk((QWidget*)parent(), outs);
						qApp->processEvents();
						if (dia->exec())
							{
							outs = dia->Wort->text();
							uint ii = 0;
							for (i = 1; i < outs.length()-1; ++i)
								{
								QChar cht = outs[i];
								if (cht == "-")
									it->Ptext.at(QMIN(maxC, ii+firstC))->cstyle ^= 128;	// Set new Hyphens according Buffer
								else
									ii++;
								}
							}
						else
							{
							free(buffer);
							delete dia;
							break;
							}
						delete dia;
  					qApp->setOverrideCursor(QCursor(waitCursor), true);
						}
					else
						{
						for (i = 1; i < found.length()-1; ++i)
							{
							if(buffer[i] & 1)
								it->Ptext.at(QMIN(maxC, i+firstC))->cstyle ^= 128;	// Set new Hyphens according Buffer
							}
	  				}
					}
				}
			free(buffer);
			buffer = NULL;
			}
		if (Ccount == 0)
			Ccount++;
		}
	it->Dirty = true;
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	it->OwnPage->RefreshItem(it);
}
