/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/* Original code from KOffice libwmf */

/* WMF Metafile Function Description Table
 * Author: Stefan Taferner <taferner@kde.org>
 */
#ifndef metafunc_h
#define metafunc_h

#include <QList>

class WMFImport;
class PageItem;

static const struct MetaFuncRec
{
    const char* name;
    unsigned short func;
    long minParams;
    void ( WMFImport::*method )( QList<PageItem*>& items, long, const short* );
} metaFuncTab[] =
  {
      { "SETBKCOLOR",           0x0201,  2, &WMFImport::setBkColor },
      { "SETBKMODE",            0x0102,  1, &WMFImport::setBkMode },
      { "SETMAPMODE",           0x0103,  0, &WMFImport::noop },
      { "SETROP2",              0x0104,  0, &WMFImport::noop },
      { "SETRELABS",            0x0105,  0, &WMFImport::noop },
      { "SETPOLYFILLMODE",      0x0106,  1, &WMFImport::setPolyFillMode },
      { "SETSTRETCHBLTMODE",    0x0107,  0, &WMFImport::noop },
      { "SETTEXTCHAREXTRA",     0x0108,  0, &WMFImport::noop },
      { "SETTEXTCOLOR",         0x0209,  2, &WMFImport::setTextColor },
      { "SETTEXTJUSTIFICATION", 0x020A,  0, &WMFImport::noop },
      { "SETWINDOWORG",         0x020B,  2, &WMFImport::setWindowOrg },
      { "SETWINDOWEXT",         0x020C,  2, &WMFImport::setWindowExt },
      { "SETVIEWPORTORG",       0x020D,  0, &WMFImport::noop },
      { "SETVIEWPORTEXT",       0x020E,  0, &WMFImport::noop },
      { "OFFSETWINDOWORG",      0x020F,  0, &WMFImport::noop },
      { "SCALEWINDOWEXT",       0x0410,  0, &WMFImport::noop },
      { "OFFSETVIEWPORTORG",    0x0211,  0, &WMFImport::noop },
      { "SCALEVIEWPORTEXT",     0x0412,  0, &WMFImport::noop },
      { "LINETO",               0x0213,  2, &WMFImport::lineTo },
      { "MOVETO",               0x0214,  2, &WMFImport::moveTo },
      { "EXCLUDECLIPRECT",      0x0415,  0, &WMFImport::excludeClipRect },
      { "INTERSECTCLIPRECT",    0x0416,  0, &WMFImport::intersectClipRect },
      { "ARC",                  0x0817,  8, &WMFImport::arc },
      { "ELLIPSE",              0x0418,  4, &WMFImport::ellipse },
      { "FLOODFILL",            0x0419,  0, &WMFImport::noop },
      { "PIE",                  0x081A,  8, &WMFImport::pie },
      { "RECTANGLE",            0x041B,  4, &WMFImport::rectangle },
      { "ROUNDRECT",            0x061C,  6, &WMFImport::roundRect },
      { "PATBLT",               0x061D,  0, &WMFImport::noop },
      { "SAVEDC",               0x001E,  0, &WMFImport::saveDC },
      { "SETPIXEL",             0x041F,  0, &WMFImport::noop },
      { "OFFSETCLIPRGN",        0x0220,  0, &WMFImport::noop },
      { "TEXTOUT",              0x0521,  1, &WMFImport::textOut },
      { "BITBLT",               0x0922,  0, &WMFImport::noop },
      { "STRETCHBLT",           0x0B23,  0, &WMFImport::noop },
      { "POLYGON",              0x0324,  1, &WMFImport::polygon },
      { "POLYLINE",             0x0325,  1, &WMFImport::polyline },
      { "ESCAPE",               0x0626,  0, &WMFImport::noop },
      { "RESTOREDC",            0x0127,  1, &WMFImport::restoreDC },
      { "FILLREGION",           0x0228,  0, &WMFImport::noop },
      { "FRAMEREGION",          0x0429,  0, &WMFImport::noop },
      { "INVERTREGION",         0x012A,  0, &WMFImport::noop },
      { "PAINTREGION",          0x012B,  0, &WMFImport::noop },
      { "SELECTCLIPREGION",     0x012C,  0, &WMFImport::noop },
      { "SELECTOBJECT",         0x012D,  1, &WMFImport::selectObject },
      { "SETTEXTALIGN",         0x012E,  1, &WMFImport::setTextAlign },
      { "CHORD",                0x0830,  8, &WMFImport::chord },
      { "SETMAPPERFLAGS",       0x0231,  0, &WMFImport::noop },
      { "EXTTEXTOUT",           0x0a32,  4, &WMFImport::extTextOut },
      { "SETDIBTODEV",          0x0d33,  0, &WMFImport::noop },
      { "SELECTPALETTE",        0x0234,  0, &WMFImport::noop },
      { "REALIZEPALETTE",       0x0035,  0, &WMFImport::noop },
      { "ANIMATEPALETTE",       0x0436,  0, &WMFImport::noop },
      { "SETPALENTRIES",        0x0037,  0, &WMFImport::noop },
      { "POLYPOLYGON",          0x0538,  1, &WMFImport::polyPolygon },
      { "RESIZEPALETTE",        0x0139,  0, &WMFImport::noop },
      { "DIBBITBLT",            0x0940,  0, &WMFImport::noop },
      { "DIBSTRETCHBLT",        0x0b41,  0, &WMFImport::noop },
      { "DIBCREATEPATTERNBRUSH", 0x0142,  0, &WMFImport::noop },
      { "STRETCHDIB",           0x0f43,  0, &WMFImport::noop },
      { "EXTFLOODFILL",         0x0548,  0, &WMFImport::noop },
      { "DELETEOBJECT",         0x01f0,  1, &WMFImport::deleteObject },
      { "CREATEPALETTE",        0x00f7,  0, &WMFImport::createEmptyObject },
      { "CREATEPATTERNBRUSH",   0x01F9,  0, &WMFImport::createEmptyObject },
      { "CREATEPENINDIRECT",    0x02FA,  5, &WMFImport::createPenIndirect },
      { "CREATEFONTINDIRECT",   0x02FB, 10, &WMFImport::createFontIndirect },
      { "CREATEBRUSHINDIRECT",  0x02FC,  4, &WMFImport::createBrushIndirect },
      { "CREATEREGION",         0x06FF,  0, &WMFImport::createEmptyObject },
      { "END",                   0,  0, &WMFImport::end },
      // always the latest in the table : in case of unknown function
      { NULL,                   0,  0, &WMFImport::noop },
  };


#endif /*metafunc_h*/
