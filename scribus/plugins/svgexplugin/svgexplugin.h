#ifndef SVGPLUG_H
#define SVGPLUG_H

#include <qdom.h>
#include "scribus.h"

/** Calls the Plugin with the main Application window as parent
  * and the main Application Class as parameter */
extern "C" void Run(QWidget *d, ScribusApp *plug);
/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" QString Name();
/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugins is a import Plugin, which appears in the Import Menue
  * 3 = the Plugins is a export Plugin, which appears in the Export Menue */
extern "C" int Type();

class SVGExPlug : public QObject
{ 
    Q_OBJECT

public:
    SVGExPlug( ScribusApp *plug, QString fName );
    ~SVGExPlug();

private:
		void ProcessPage(ScribusApp *plug, Page *Seite, QDomDocument *docu, QDomElement *elem);
		QString SetClipPath(PageItem *ite);
		QString FToStr(double c);
		QString IToStr(int c);
		void SetTextProps(QDomElement *tp, struct Pti *hl, ScribusApp *plug);
		QString SetFarbe(QString farbe, int shad, ScribusApp *plug);
		QString GetMultiStroke(ScribusApp *plug, struct singleLine *sl, PageItem *Item);
		int GradCount;
		int ClipCount;
};

#endif // CMSPLUG_H
