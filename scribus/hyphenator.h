#ifndef HYPLUG_H
#define HYPLUG_H

#include <qobject.h>
#include <qtextcodec.h>
#include "hyphen.h"
class ScribusDoc;
class ScribusApp;
class PageItem;

/*!
This class is the core of the Scribus hyphenation system.
*/

class Hyphenator : public QObject
{
    Q_OBJECT

public:
    Hyphenator(QWidget* parent, ScribusDoc *dok, ScribusApp* app);
    ~Hyphenator();
    /*! There are languages having rule not to hyphen word shorter than
    MinWordLen */
	int MinWordLen;
    /*! Language in use */
	QString Language;
    /*! Flag - if user set auto hyphen processing.*/
	bool Automatic;
    /*! Flag - obsolete? */
	bool AutoCheck;
    /*! Reference to the hyphen dictionary structure. */
	HyphenDict *hdict;
    /*! Embeded reference to the \see ScribusDoc filled by \a dok */
	ScribusDoc *doc;
    /*! Embeded reference to the \see ScribusApp filled by \a app */
	ScribusApp *Sap;
    /*! Flag - if is the dictionary without errors etc. If is it 'false'
        hyphen aborted. */
	bool useAble;
    /*! Qt text codec which handles local characters. */
	QTextCodec *codec;

public slots:
	void slotNewDict(QString name);
	void slotNewSettings(int Wordlen, bool Autom, bool ACheck);
	void slotHyphenateWord(PageItem* it, QString text, int firstC);
	void slotHyphenate(PageItem *it);
};
#endif
