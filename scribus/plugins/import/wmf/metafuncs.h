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
    void ( WMFImport::*method )( QList<PageItem*>& items, long, short* );
} metaFuncTab[] =
  {
      { "SETBKCOLOR",           0x0201, &WMFImport::setBkColor },
      { "SETBKMODE",            0x0102, &WMFImport::setBkMode },
      { "SETMAPMODE",           0x0103, &WMFImport::noop },
      { "SETROP2",              0x0104, &WMFImport::noop },
      { "SETRELABS",            0x0105, &WMFImport::noop },
      { "SETPOLYFILLMODE",      0x0106, &WMFImport::setPolyFillMode },
      { "SETSTRETCHBLTMODE",    0x0107, &WMFImport::noop },
      { "SETTEXTCHAREXTRA",     0x0108, &WMFImport::noop },
      { "SETTEXTCOLOR",         0x0209, &WMFImport::setTextColor },
      { "SETTEXTJUSTIFICATION", 0x020A, &WMFImport::noop },
      { "SETWINDOWORG",         0x020B, &WMFImport::setWindowOrg },
      { "SETWINDOWEXT",         0x020C, &WMFImport::setWindowExt },
      { "SETVIEWPORTORG",       0x020D, &WMFImport::noop },
      { "SETVIEWPORTEXT",       0x020E, &WMFImport::noop },
      { "OFFSETWINDOWORG",      0x020F, &WMFImport::noop },
      { "SCALEWINDOWEXT",       0x0410, &WMFImport::noop },
      { "OFFSETVIEWPORTORG",    0x0211, &WMFImport::noop },
      { "SCALEVIEWPORTEXT",     0x0412, &WMFImport::noop },
      { "LINETO",               0x0213, &WMFImport::lineTo },
      { "MOVETO",               0x0214, &WMFImport::moveTo },
      { "EXCLUDECLIPRECT",      0x0415, &WMFImport::excludeClipRect },
      { "INTERSECTCLIPRECT",    0x0416, &WMFImport::intersectClipRect },
      { "ARC",                  0x0817, &WMFImport::arc },
      { "ELLIPSE",              0x0418, &WMFImport::ellipse },
      { "FLOODFILL",            0x0419, &WMFImport::noop },
      { "PIE",                  0x081A, &WMFImport::pie },
      { "RECTANGLE",            0x041B, &WMFImport::rectangle },
      { "ROUNDRECT",            0x061C, &WMFImport::roundRect },
      { "PATBLT",               0x061D, &WMFImport::noop },
      { "SAVEDC",               0x001E, &WMFImport::saveDC },
      { "SETPIXEL",             0x041F, &WMFImport::noop },
      { "OFFSETCLIPRGN",        0x0220, &WMFImport::noop },
      { "TEXTOUT",              0x0521, &WMFImport::textOut },
      { "BITBLT",               0x0922, &WMFImport::noop },
      { "STRETCHBLT",           0x0B23, &WMFImport::noop },
      { "POLYGON",              0x0324, &WMFImport::polygon },
      { "POLYLINE",             0x0325, &WMFImport::polyline },
      { "ESCAPE",               0x0626, &WMFImport::noop },
      { "RESTOREDC",            0x0127, &WMFImport::restoreDC },
      { "FILLREGION",           0x0228, &WMFImport::noop },
      { "FRAMEREGION",          0x0429, &WMFImport::noop },
      { "INVERTREGION",         0x012A, &WMFImport::noop },
      { "PAINTREGION",          0x012B, &WMFImport::noop },
      { "SELECTCLIPREGION",     0x012C, &WMFImport::noop },
      { "SELECTOBJECT",         0x012D, &WMFImport::selectObject },
      { "SETTEXTALIGN",         0x012E, &WMFImport::setTextAlign },
      { "CHORD",                0x0830, &WMFImport::chord },
      { "SETMAPPERFLAGS",       0x0231, &WMFImport::noop },
      { "EXTTEXTOUT",           0x0a32, &WMFImport::extTextOut },
      { "SETDIBTODEV",          0x0d33, &WMFImport::noop },
      { "SELECTPALETTE",        0x0234, &WMFImport::noop },
      { "REALIZEPALETTE",       0x0035, &WMFImport::noop },
      { "ANIMATEPALETTE",       0x0436, &WMFImport::noop },
      { "SETPALENTRIES",        0x0037, &WMFImport::noop },
      { "POLYPOLYGON",          0x0538, &WMFImport::polyPolygon },
      { "RESIZEPALETTE",        0x0139, &WMFImport::noop },
      { "DIBBITBLT",            0x0940, &WMFImport::noop },
      { "DIBSTRETCHBLT",        0x0b41, &WMFImport::noop },
      { "DIBCREATEPATTERNBRUSH", 0x0142, &WMFImport::noop },
      { "STRETCHDIB",           0x0f43, &WMFImport::noop },
      { "EXTFLOODFILL",         0x0548, &WMFImport::noop },
      { "DELETEOBJECT",         0x01f0, &WMFImport::deleteObject },
      { "CREATEPALETTE",        0x00f7, &WMFImport::createEmptyObject },
      { "CREATEPATTERNBRUSH",   0x01F9, &WMFImport::createEmptyObject },
      { "CREATEPENINDIRECT",    0x02FA, &WMFImport::createPenIndirect },
      { "CREATEFONTINDIRECT",   0x02FB, &WMFImport::createFontIndirect },
      { "CREATEBRUSHINDIRECT",  0x02FC, &WMFImport::createBrushIndirect },
      { "CREATEREGION",         0x06FF, &WMFImport::createEmptyObject },
      { "END",                   0,      &WMFImport::end },
      // always the latest in the table : in case of unknown function
      { NULL,                   0,      &WMFImport::noop },
  };


#endif /*metafunc_h*/
