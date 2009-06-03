/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCFONTMETRICS_H
#define SCFONTMETRICS_H

#include <utility>
#include <QGlobalStatic>
#include <QString>
#include <QColor>
//Added by qt3to4:
#include <QPixmap>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

#include "scribusapi.h"
#include "fpoint.h"
#include "fpointarray.h"

class ScFace;
class Scribusdoc;

int SCRIBUS_API setBestEncoding(FT_Face face);
FPointArray SCRIBUS_API traceChar(FT_Face face, uint chr, int chs, qreal *x, qreal *y, bool *err);
FPointArray SCRIBUS_API traceGlyph(FT_Face face, uint chr, int chs, qreal *x, qreal *y, bool *err);
QPixmap SCRIBUS_API FontSample(const ScFace& fnt, int s, QString ts, QColor back, bool force = false);
//QPixmap SCRIBUS_API fontSamples(const ScFace& fnt, int s, QString ts, QColor back);
bool SCRIBUS_API GlyNames(FT_Face face, QMap<uint, std::pair<QChar, QString> >& GList);

#endif
