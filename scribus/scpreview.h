#ifndef SCPREVIEW_H
#define SCPREVIEW_H

#include "scribusstructs.h"
#include "scpainter.h"

class QString;
class CMYKColor;

class ScPreview
{
public:
	ScPreview(preV *prefs);
	~ScPreview() {};
	QPixmap createPreview(QString data);
	void SetFarbe(QColor *tmp, QString farbe, int shad);
	void DrawZeichenS(ScPainter *p, double xco, double yco, QString ch, QString ZFo, bool Reverse, int Style, int mode, int Siz);
	QMap<QString,CMYKColor> Farben;
	QMap<QString,multiLine> MLineStyles;
	QValueList<uint> Segments;
	preV *Prefs;
	FT_Library   library;
};
#endif

