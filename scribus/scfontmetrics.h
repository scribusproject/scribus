#include <qglobal.h>
#include <qstring.h>
#include <qcolor.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

#include "scribusapi.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "pdflib.h"

class Foi;
class Scribusdoc;

int SCRIBUS_API setBestEncoding(FT_Face face);
FPointArray SCRIBUS_API traceChar(FT_Face face, uint chr, int chs, double *x, double *y, bool *err);
QPixmap SCRIBUS_API FontSample(Foi * fnt, int s, QString ts, QColor back, bool force = false);
QPixmap SCRIBUS_API fontSamples(Foi * fnt, int s, QString ts, QColor back);
bool SCRIBUS_API GlyNames(Foi * fnt, QMap<uint, QString> *GList);
bool SCRIBUS_API GlyIndex(Foi * fnt, QMap<uint, PDFlib::GlNamInd> *GListInd);
double SCRIBUS_API Cwidth(ScribusDoc *currentDoc, Foi* name, QString ch, int Siz, QString ch2 = " ");
double SCRIBUS_API RealCWidth(ScribusDoc *currentDoc, Foi* name, QString ch, int Siz);
double SCRIBUS_API RealCAscent(ScribusDoc *currentDoc, Foi* name, QString ch, int Size);
double SCRIBUS_API RealCHeight(ScribusDoc *currentDoc, Foi* name, QString ch, int Size);
double SCRIBUS_API RealFHeight(ScribusDoc *currentDoc, Foi* name, int Size);
