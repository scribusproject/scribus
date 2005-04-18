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

#ifdef _MSC_VER
 #if (_MSC_VER >=1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#include <qregexp.h>
#include <qcursor.h>
#include <qdir.h>
#include <cstdlib>
#include <string>
#include "scribus.h"
#include "prefsfile.h"

extern PrefsFile* prefsFile;
/*!
 \fn Hyphenator::Hyphenator(QWidget* parent, ScribusDoc *dok, ScribusApp* app)
 \brief Constructor reads user configuration and sets hyphenator properties.
 \date
 \author Franz Schmid
 \param parent unused
 \param dok ScribusDoc reference. It's used for preferences accessing.
 \param app ScribusApp reference. It's used for preferences accessing.
 \retval None
*/
Hyphenator::Hyphenator(QWidget* parent, ScribusDoc *dok, ScribusApp* app) : QObject( parent, "bu")
{
	char *filename = NULL;
	doc = dok;
	Sap = app;
	MinWordLen = doc->MinWordLen;
	Automatic = doc->Automatic;
	AutoCheck = doc->AutoCheck;
	HyCount = doc->HyCount;
 	QString pfad = LIBDIR;
	if (Sap->Sprachen.contains(doc->Language))
		Language = doc->Language;
	else
	{
		Language = Sap->Prefs.Language;
		doc->Language = Language;
	}
	pfad += "dicts/" + Sap->Sprachen[Language];
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

/*!
 \fn Hyphenator::~Hyphenator()
 \brief Safely cleans hyphen. 
 \date
 \author Franz Schmid
 \param None
 \retval None
 */
Hyphenator::~Hyphenator()
{
	hnj_hyphen_free(hdict);
}

/*!
 \fn void Hyphenator::slotNewDict(QString name)
 \brief Loads dictionary and fills parameters like \a useAble, \a codec, \a hdict.
 \date
 \author Franz Schmid
 \param name is the name of specified language - filename.
 \retval None
 */
void Hyphenator::slotNewDict(QString name)
{
	if (!Sap->Sprachen.contains(name))
		return;
	char *filename = NULL;
	if (hdict != NULL)
		hnj_hyphen_free(hdict);
 	QString pfad = LIBDIR;
	Language = name;
	doc->Language = name;
	pfad += "dicts/" + Sap->Sprachen[Language];
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

/*!
 \fn void Hyphenator::slotNewSettings(int Wordlen, bool Autom, bool ACheck)
 \brief Writes actual hyphen configuration into \a doc object.
 \date
 \author Franz Schmid
 \param WordLen lenght of the word.
 \param Autom is automatic flag.
 \param ACheck AutoCheck flag.
 \retval None
 */
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

/*! 
 \fn void Hyphenator::slotHyphenateWord(PageItem* it, QString text, int firstC)
 \brief Make hyphenation when user edits text in text frame manually.
 \detail If is everything set correctly and textframe selected language fits the
    dictionary language. At first removes any old hyphens then sets new one.
 \param it references \see PageItem - text frame.
 \param text is a word to hyphenate.
 \param firstC is the first character of the word in \a it item.
 \retval None
 */
void Hyphenator::slotHyphenateWord(PageItem* it, QString text, int firstC)
{
	if ((!useAble) || (!Sap->Sprachen.contains(it->Language)))
		return;
	const char *word;
	char *buffer;
	const int BORDER = 2;
	QCString te;

	uint maxC = it->Ptext.count() - 1;
	QString found = text;
	if (static_cast<int>(found.length()) > MinWordLen-1)
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
				it->Ptext.at(QMIN(maxC, i+firstC))->cstyle &= 127;		// Delete any old Hyphens
			for (i = 1; i < found.length()-1; ++i)
			{
				if(buffer[i] & 1)
					it->Ptext.at(QMIN(maxC, i+firstC))->cstyle |= 128;	// Set new Hyphens according Buffer
			}
		}
		free(buffer);
	}
}

/*! 
 \fn void Hyphenator::slotHyphenate(PageItem* it)
 \brief Make hyphenation as described in \see slotHyphenateWord for the whole text frame.
 \date
 \author Franz Schmid
 \param it references \see PageItem - text frame.
 \retval None
 */
void Hyphenator::slotHyphenate(PageItem* it)
{
	PageItem *nb = it;
	PageItem *nb1;
	while (nb != 0)
	{
		if (nb->BackBox != 0)
			nb = nb->BackBox;
		else
			break;
	}
	nb1 = nb;
	while (nb != 0)
	{
		uint a = nb->Ptext.count();
		for (uint s = 0; s < a; ++s)
			nb1->Ptext.append(nb->Ptext.take(0));
		nb->MaxChars = 0;
		nb->CPos = 0;
		nb = nb->NextBox;
	}
	if ((!useAble) || (nb1->PType != 4) || (nb1 ->Ptext.count() == 0))
		return;
	doc->DoDrawing = false;
	const char *word;
	char *buffer;
	const int BORDER = 2;
	QString text = "";
	QString buf;
	QCString te;
	for (uint a = 0; a < nb1->Ptext.count(); ++a)
	{
		if (nb1->HasSel)
		{
			if (nb1->Ptext.at(a)->cselect)
				text += nb1->Ptext.at(a)->ch;
		}
		else
			text += nb1->Ptext.at(a)->ch;
	}
	int firstC = 0;
	if (nb1->HasSel)
	{
		for (uint a = 0; a < nb1->Ptext.count(); ++a)
		{
			if (nb1->Ptext.at(a)->cselect)
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
	uint maxC = nb1->Ptext.count() - 1;
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
		if (Ccount > MinWordLen-1)
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
					nb1->Ptext.at(QMIN(maxC, i+firstC))->cstyle &= 127;		// Delete any old Hyphens
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
						PrefsContext* prefs = prefsFile->getContext("hyhpen_options");
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
									nb1->Ptext.at(QMIN(maxC, ii+firstC))->cstyle |= 128;
								else
									ii++;
							}
						}
						else
						{
							free(buffer);
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
					else
					{
						for (i = 1; i < found.length()-1; ++i)
						{
							if(buffer[i] & 1)
								nb1->Ptext.at(QMIN(maxC, i+firstC))->cstyle |= 128;
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
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	doc->DoDrawing = true;
	nb1->OwnPage->RefreshItem(nb1);
}
