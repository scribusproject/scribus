#ifndef HYPLUG_H
#define HYPLUG_H

#include <qobject.h>
#include <qtextcodec.h>

#include "scribusapi.h"
#include "hyphen.h"
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
    Hyphenator(QWidget* parent, ScribusDoc *dok);
    ~Hyphenator();
    /*! Embeded reference to the \see ScribusDoc filled by \a dok */
	ScribusDoc *doc;
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
    /*! Reference to the hyphen dictionary structure. */
	HyphenDict *hdict;
    /*! Flag - if is the dictionary without errors etc. If is it 'false'
        hyphen aborted. */
	bool useAble;
    /*! Qt text codec which handles local characters. */
	QTextCodec *codec;

public slots:
	void slotNewDict(QString name);
	void slotNewSettings(int Wordlen, bool Autom, bool ACheck, int Num);
	void slotHyphenateWord(PageItem* it, QString text, int firstC);
	void slotHyphenate(PageItem *it);
	void slotDeHyphenate(PageItem *it);
};
#endif
