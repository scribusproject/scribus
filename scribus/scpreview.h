#ifndef SCPREVIEW_H
#define SCPREVIEW_H

#include "scribusdoc.h"

class ScPreview
{

public:
    ScPreview(preV *prefs);
    ~ScPreview() {};
    QPixmap createPreview(QString data);
		void SetFarbe(QColor *tmp, QString farbe, int shad);
		void DrawPoly(QPainter *p, QPointArray pts, QColor BackF, struct CLBuf *OB, bool bitm = false);
		void DrawPolyL(QPainter *p, QPointArray pts, struct CLBuf *OB);
		QMap<QString,CMYKColor> Farben;
		QMap<QString,multiLine> MLineStyles;
		QValueList<uint> Segments;
  	preV *Prefs;
};
#endif

