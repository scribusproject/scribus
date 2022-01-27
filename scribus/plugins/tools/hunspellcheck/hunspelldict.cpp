/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "hunspelldict.h"

#include <hunspell/hunspell.hxx>
#include <QDebug>
#include <QTextCodec>

#include "scconfig.h"

HunspellDict::HunspellDict(const QString& affPath, const QString& dictPath)
{
	m_hunspell = nullptr;
	m_codec = nullptr;

	QString encoding = "ISO8859-1";
	m_hunspell = new Hunspell(affPath.toLocal8Bit().constData(), dictPath.toLocal8Bit().constData());
	if (m_hunspell)
	{
		const char* dictEncoding = m_hunspell->get_dic_encoding();
		if (dictEncoding)
			encoding = QString::fromLatin1(dictEncoding);
	}

	if (encoding.isEmpty())
		encoding = "ISO8859-1";
	m_codec = QTextCodec::codecForName(encoding.toLatin1().constData());
}

HunspellDict::~HunspellDict()
{
	delete m_hunspell;
	m_hunspell = nullptr;
}

#ifndef HUNSPELL_NEWAPI
int HunspellDict::spell(const QString& word)
{
	if (m_hunspell)
		return m_hunspell->spell(m_codec->fromUnicode(word).constData());
	return -1;
}

QStringList HunspellDict::suggest(const QString& word)
{
	char **sugglist = nullptr;
	QStringList replacements;

	int suggCount = m_hunspell->suggest(&sugglist, m_codec->fromUnicode(word).constData());
	for (int j = 0; j < suggCount; ++j)
		replacements << m_codec->toUnicode(sugglist[j]);
	m_hunspell->free_list(&sugglist, suggCount);

	return replacements;
}
#else
int HunspellDict::spell(const QString& word)
{
	if (!m_hunspell)
		return -1;
	std::string s = m_codec->fromUnicode(word).toStdString();
	return m_hunspell->spell(s);
}

QStringList HunspellDict::suggest(const QString& word)
{
	QStringList replacements;
	if (!m_hunspell)
		return replacements;
	std::string s = word.toStdString();
	std::vector<std::string> sugglist = m_hunspell->suggest(s);
	for (uint i = 0; i < sugglist.size(); ++i)
		replacements << m_codec->toUnicode(QByteArray::fromStdString(sugglist[i]));
	return replacements;
}
#endif
