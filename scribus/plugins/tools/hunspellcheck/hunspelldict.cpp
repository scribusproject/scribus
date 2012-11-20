/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "hunspelldict.h"

#include <hunspell/hunspell.hxx>
#include <QTextCodec>

HunspellDict::HunspellDict(const QString& affPath, const QString& dictPath)
{
	m_hunspell = 0;
	m_codec = 0;

	QString encoding = "ISO8859-1";
	m_hunspell = new Hunspell(affPath.toLocal8Bit().constData(), dictPath.toLocal8Bit().constData());
	if (m_hunspell)
	{
		char* dictEncoding = m_hunspell->get_dic_encoding();
		if (dictEncoding)
			encoding = QString::fromLatin1(dictEncoding);
	}

	if (encoding.isEmpty())
		encoding = "ISO8859-1";
	m_codec = QTextCodec::codecForName(encoding.toLatin1().constData());
}

HunspellDict::~HunspellDict()
{
	if (m_hunspell)
	{
		delete m_hunspell;
		m_hunspell = 0;
	}
}

int HunspellDict::spell(QString word)
{
	if (m_hunspell)
		return m_hunspell->spell(m_codec->fromUnicode(word).constData());
	return -1;
}

QStringList HunspellDict::suggest(QString word)
{
	char **sugglist = NULL;
	QStringList replacements;

	int suggCount = m_hunspell->suggest(&sugglist, m_codec->fromUnicode(word).constData());
	for (int j = 0; j < suggCount; ++j)
		replacements << m_codec->toUnicode(sugglist[j]);
	m_hunspell->free_list(&sugglist, suggCount);

	return replacements;
}
