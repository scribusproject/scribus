#ifndef HYPLUG_H
#define HYPLUG_H

#include <qobject.h>
#include "hyphen.h"
class ScribusDoc;
class ScribusApp;
class PageItem;

class Hyphenator : public QObject
{
    Q_OBJECT

public:
    Hyphenator(QWidget* parent, ScribusDoc *dok, ScribusApp* app);
    ~Hyphenator();
		int MinWordLen;
		QString Language;
		bool Automatic;
		bool AutoCheck;
		HyphenDict *hdict;
		ScribusDoc *doc;
		ScribusApp *Sap;

public slots:
		void slotNewDict(QString name);
		void slotNewSettings(int Wordlen, bool Autom, bool ACheck);
		void slotHyphenateWord(PageItem* it, QString text, int firstC);
		void slotHyphenate(PageItem *it);
};
#endif
