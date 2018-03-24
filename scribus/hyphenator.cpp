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
#include "ui/hyask.h"

#include "scconfig.h"

#include <QApplication>
#include <QCursor>
#include <QCheckBox>
#include <QByteArray>

#include "langmgr.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "prefsfile.h"
#include "prefsmanager.h"

Hyphenator::Hyphenator(QWidget* parent, ScribusDoc *dok) : QObject( parent ),
	m_doc(dok),
	m_hdict(NULL),
	m_codec(NULL),
	m_automatic(m_doc->hyphAutomatic()),
	AutoCheck(m_doc->hyphAutoCheck())
{
	rememberedWords.clear();
/* Add reading these special lists from prefs or doc here */
	ignoredWords.clear();
	specialWords.clear();
}

Hyphenator::~Hyphenator()
{
	if (m_hdict)
		hnj_hyphen_free(m_hdict);
}

bool Hyphenator::loadDict(const QString& name)
{
	if (LanguageManager::instance()->getHyphFilename(name).isEmpty())
		return false;
		
	if (m_language != name)
	{
		m_language = name;

		QFile file(LanguageManager::instance()->getHyphFilename(m_language));
		
		if (m_hdict != NULL)
			hnj_hyphen_free(m_hdict);

		if (file.open(QIODevice::ReadOnly))
		{
			m_codec = QTextCodec::codecForName(file.readLine());
			m_hdict = hnj_hyphen_load(file.fileName().toLocal8Bit().data());
			file.close();
			return true;
		}
		else
		{
			m_hdict = NULL;
			return false;
		}
	}

	return (m_codec != 0 && m_hdict != 0);
}

void Hyphenator::slotNewSettings(bool Autom, bool ACheck)
{
	AutoCheck = ACheck;
	m_doc->setHyphAutomatic(Autom);
	m_doc->setHyphAutoCheck(AutoCheck);
}

void Hyphenator::slotHyphenateWord(PageItem* it, const QString& text, int firstC)
{
	if (text.contains(SpecialChars::SHYPHEN))
		return;

	const CharStyle& style = it->itemText.charStyle(firstC);
	if (text.length() < style.hyphenWordMin())
		return;

	bool ok = loadDict(style.language());
	if (!ok)
		return;

	QByteArray te = m_codec->fromUnicode(text);
	char *buffer = static_cast<char*>(malloc(te.length() + 5));
	if (buffer == NULL)
		return;
	char **rep = NULL;
	int *pos = NULL;
	int *cut = NULL;
	// TODO: support non-standard hyphenation, see hnj_hyphen_hyphenate2 docs
	if (!hnj_hyphen_hyphenate2(m_hdict, te.data(), te.length(), buffer, NULL, &rep, &pos, &cut))
	{
		buffer[te.length()] = '\0';
		it->itemText.hyphenateWord(firstC, text.length(), buffer);
	}
	free(buffer);
	if (rep)
	{
		for (int i = 0; i < te.length() - 1; ++i)
			free(rep[i]);
	}
	free(rep);
	free(pos);
	free(cut);
}

void Hyphenator::slotHyphenate(PageItem* it)
{
	if (!(it->asTextFrame()) || (it->itemText.length() == 0))
		return;
	m_doc->DoDrawing = false;

	QString text = "";

	int startC = 0;
	if (it->itemText.lengthOfSelection() > 0)
	{
		startC = it->itemText.startOfSelection();
		text = it->itemText.text(startC, it->itemText.lengthOfSelection());
	}
	else {
		text = it->itemText.text(0, it->itemText.length());
	}

	rememberedWords.clear();
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));

	BreakIterator* bi = StoryText::getWordIterator();
	bi->setText((const UChar*) text.utf16());
	int pos = bi->first();
	while (pos != BreakIterator::DONE)
	{
		int firstC = pos;
		pos = bi->next();
		int lastC = pos;
		int countC = lastC - firstC;

		const CharStyle& style = it->itemText.charStyle(firstC);
		if (countC > 0 && countC > style.hyphenWordMin() - 1)
		{
			QString word = text.mid(firstC, countC);
			QString wordLower = QLocale(style.language()).toLower(word);
			if (wordLower.contains(SpecialChars::SHYPHEN))
				break;

			bool ok = loadDict(style.language());
			if (!ok)
				continue;

			QByteArray te = m_codec->fromUnicode(wordLower);
			char *buffer = static_cast<char*>(malloc(te.length() + 5));
			if (buffer == NULL)
				break;

			char **rep = NULL;
			int *pos = NULL;
			int *cut = NULL;
			// TODO: support non-standard hyphenation, see hnj_hyphen_hyphenate2 docs
			if (!hnj_hyphen_hyphenate2(m_hdict, te.data(), te.length(), buffer, NULL, &rep, &pos, &cut))
			{
	  			int i = 0;
				buffer[te.length()] = '\0';
				bool hasHyphen = false;
				for (i = 1; i < wordLower.length()-1; ++i)
				{
					if(buffer[i] & 1)
					{
						hasHyphen = true;
						break;
					}
				}
				QString outs = "";
				QString input = "";
				outs += word[0];
				for (i = 1; i < wordLower.length()-1; ++i)
				{
					outs += word[i];
					if(buffer[i] & 1)
						outs += "-";
				}
				outs += word.rightRef(1);
				input = outs;
				if (!ignoredWords.contains(word))
				{
					if (!hasHyphen)
						it->itemText.hyphenateWord(startC + firstC, wordLower.length(), NULL);
					else if (m_automatic)
					{
						if (specialWords.contains(word))
						{
							outs = specialWords.value(word);
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
						it->itemText.hyphenateWord(startC + firstC, wordLower.length(), buffer);
					}
					else
					{
						if (specialWords.contains(word))
						{
							outs = specialWords.value(word);
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
							it->itemText.hyphenateWord(firstC, wordLower.length(), buffer);
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
									if (!ignoredWords.contains(word))
										ignoredWords.insert(word);
								}
								if (dia->addToExceptionList->isChecked())
								{
									if (!specialWords.contains(word))
										specialWords.insert(word, outs);
								}
								it->itemText.hyphenateWord(firstC, wordLower.length(), buffer);
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
				for (int i = 0; i < te.length() - 1; ++i)
					free(rep[i]);
			}
			free(rep);
			free(pos);
			free(cut);
		}
	}
	qApp->restoreOverrideCursor();
	m_doc->DoDrawing = true;
	rememberedWords.clear();
}

void Hyphenator::slotDeHyphenate(PageItem* it)
{
	if (!(it->asTextFrame()) || (it ->itemText.length() == 0))
		return;

	if (it->itemText.lengthOfSelection() > 0)
	{
		it->itemText.hyphenateWord(it->itemText.startOfSelection(), it->itemText.lengthOfSelection(), NULL);
	}
	else {
		uint a = it->itemText.length();
		it->itemText.hyphenateWord(0, a, NULL);
	}
	m_doc->DoDrawing = true;
}
