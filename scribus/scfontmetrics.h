#include <qglobal.h>
#include <qstring.h>
#include <qcolor.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

#include "fpoint.h"
#include "fpointarray.h"
#include "libpdf/pdflib.h"

class Foi;

int setBestEncoding(FT_Face face);
FPointArray traceChar(FT_Face face, uint chr, int chs, double *x, double *y, bool *err);
QPixmap FontSample(Foi * fnt, int s, QString ts, QColor back, bool force = false);
QPixmap fontSamples(Foi * fnt, int s, QString ts, QColor back);
bool GlyNames(Foi * fnt, QMap<uint, QString> *GList);
bool GlyIndex(Foi * fnt, QMap<uint, PDFlib::GlNamInd> *GListInd);
