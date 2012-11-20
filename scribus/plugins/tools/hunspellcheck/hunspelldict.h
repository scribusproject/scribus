/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef HUNSPELLDICT_H
#define HUNSPELLDICT_H

#include <QString>
#include <QStringList>

class Hunspell;
class QTextCodec;

class HunspellDict
{
public:
	HunspellDict(const QString& affPath, const QString& dictPath);
	~HunspellDict();

	int spell(QString word);
	QStringList suggest(QString word);

protected:
	Hunspell*   m_hunspell;
	QTextCodec* m_codec;
};

#endif
