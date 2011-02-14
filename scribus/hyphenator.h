/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef HYPLUG_H
#define HYPLUG_H

#include <QObject>
#include <QTextCodec>
#include <QHash>
#include <QSet>

#include "scribusapi.h"
#include <hyphen.h>
class ScribusDoc;
class ScribusMainWindow;
class PageItem;

/*!
This class is the core of the Scribus hyphenation system.
*/

class SCRIBUS_API Hyphenator : public QObject
{
	Q_OBJECT

public:
	/*!
	\brief Constructor reads user configuration and sets hyphenator properties.
	\date
	\author Franz Schmid
	\param parent unused
	\param dok ScribusDoc reference. It's used for preferences accessing.
	*/
	Hyphenator(QWidget* parent, ScribusDoc *dok);
	/*!
	\fn Hyphenator::~Hyphenator()
	\brief Safely cleans hyphen. 
	\date
	\author Franz Schmid
	*/
	~Hyphenator();
	
private:

	/*! Embeded reference to the \see ScribusDoc filled by \a dok */
	ScribusDoc *doc;
	/*! Reference to the hyphen dictionary structure. */
	HyphenDict *hdict;
	/*! Flag - if is the dictionary without errors etc. If is it 'false'
		hyphen aborted. */
	bool useAble;
	/*! Qt text codec which handles local characters. */
	QTextCodec *codec;

	/*!
		\brief Loads dictionary and fills parameters like \a useAble, \a codec, \a hdict.
	 \date
	 \author Franz Schmid
	 \param name is the name of specified language - filename.
	 */
	void NewDict(const QString& name);
	
public:
	/*! There are languages having rule not to hyphen word shorter than
		MinWordLen */
	int MinWordLen;
	/*! Maximum number of hyphenations allowed following each other */
	int HyCount;
	/*! Language in use */
	QString Language;
	/*! Flag - if user set auto hyphen processing.*/
	bool Automatic;
	/*! Flag - obsolete? */
	bool AutoCheck;
	QHash<QString, QString> rememberedWords;
	QHash<QString, QString> specialWords;
	QSet<QString> ignoredWords;
	
public slots:
	/*!
	\brief Writes actual hyphen configuration into \a doc object.
	\date
	\author Franz Schmid
	\param Wordlen lenght of the word.
	\param Autom is automatic flag.
	\param ACheck AutoCheck flag.
	\param Num HyCount
	*/
	void slotNewSettings(int Wordlen, bool Autom, bool ACheck, int Num);
	/*! 
	\brief Make hyphenation when user edits text in text frame manually.
	If is everything set correctly and textframe selected language fits the
	dictionary language. At first removes any old hyphens then sets new one.
	\param it references \see PageItem - text frame.
	\param text is a word to hyphenate.
	\param firstC is the first character of the word in \a it item.
	*/
	void slotHyphenateWord(PageItem* it, const QString& text, int firstC);
	/*!
	\brief Make hyphenation as described in \see slotHyphenateWord for the whole text frame.
	\date
	\author Franz Schmid
	\param it references \see PageItem - text frame.
	*/
	void slotHyphenate(PageItem *it);
	/*!
	\fn void Hyphenator::slotDeHyphenate(PageItem* it)
	\brief Removes hyphenation either for the whole text frame or the selected text if there is a selection.
	\date
	\author Franz Schmid
	\param it references \see PageItem - text frame.
	*/
	void slotDeHyphenate(PageItem *it);
};

#endif
