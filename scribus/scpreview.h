#ifndef SCPREVIEW_H
#define SCPREVIEW_H

#include "scribusapi.h"
#include "scribusstructs.h"
//Added by qt3to4:
#include <QPixmap>
#include <Q3ValueList>

class QString;
class ScColor;
class PrefsManager;

class SCRIBUS_API ScPreview
{
public:
	ScPreview();
	~ScPreview() {};
	QPixmap createPreview(QString data);
	void SetFarbe(QColor *tmp, QString farbe, int shad);
	void DrawZeichenS(ScPainter *p, double xco, double yco, QString ch, QString ZFo, bool Reverse, int Style, int mode, int Siz);
	QMap<QString,ScColor> Farben;
	QMap<QString,multiLine> MLineStyles;
	Q3ValueList<uint> Segments;
	FT_Library   library;
protected:
	PrefsManager* prefsManager;
};
#endif

