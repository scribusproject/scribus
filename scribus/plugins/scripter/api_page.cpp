/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_page.h"
#include "pageitem.h"
#include "selection.h"
#include "util_math.h"
#include "commonstrings.h"
#include "../formatidlist.h"
#include "loadsaveplugin.h"
#include "ui/propertiespalette.h"



PageAPI::PageAPI(QObject *parent) : QObject(COLLECTOR)
{
    qDebug() << "PageAPI loaded";
    setObjectName("activePage");
}



PageAPI::~PageAPI()
{
    qDebug() << "PageAPI deleted";
}



/**
 * Scripter.activeDocument.activePage.remove()
 *
 * Removes the active page from the active document.
 */
void PageAPI::remove()
{
    if ( ScCore->primaryMainWindow()->doc->Pages->count() > 1  )
        ScCore->primaryMainWindow()->DeletePage2(number());
    else
        RAISE("No pages left to remove");
}



/**
 * Scripter.activeDocument.activePage.position
 * Property
 * location of active page as int value (read-only)
 */
int PageAPI::position()
{
    return ScCore->primaryMainWindow()->doc->locationOfPage(number());
}



/**
 * Scripter.activeDocument.activePage.position
 * Property
 * int value of active page number >= 1
 */
int PageAPI::number()
{
    return ScCore->primaryMainWindow()->doc->currentPageNumber() + 1;
}



void PageAPI::setNumber(int value)
{
    value--;
    if ((value < 0) || (value > static_cast<int>(ScCore->primaryMainWindow()->doc->Pages->count())-1))
    {
        RAISE("Page number out of range.");
        return;
    }
    ScCore->primaryMainWindow()->view->GotoPage(value);
}



/**
 * Scripter.activeDocument.activePage.items
 * Property
 * List of Item objects of active page
 */
QList<QVariant> PageAPI::items()
{
    QList<QVariant> l;
    int num = number() - 1;
    for (int i = 0; i<ScCore->primaryMainWindow()->doc->Items->count(); ++i)
    {
        if (num == ScCore->primaryMainWindow()->doc->Items->at(i)->OwnPage)
        {
            /*
             new ItemAPI(this,
                             ScCore->primaryMainWindow()->doc->Items->at(i)->itemName(),
                             ScCore->primaryMainWindow()->doc->Items->at(i)->itemType(),
                             ScCore->primaryMainWindow()->doc->Items->at(i)->ItemNr
                            );
            */
            l.append(qVariantFromValue(
                         (QObject *)(ScCore->primaryMainWindow()->doc->Items->at(i))
                     ));

        }
    }
    return l;
}



/**
 * Scripter.activeDocument.activePage.selection
 * Property
 * List of selected Item objects on active page of active document
 */
QList<QVariant> PageAPI::selection()
{
    QList<QVariant> l;
    int num = number() - 1;
    Selection *sel = ScCore->primaryMainWindow()->doc->m_Selection;
    for (int i=0; i < sel->count(); i++)
    {
        PageItem *item = sel->itemAt(i);
        if (num == item->OwnPage)
            l.append(qVariantFromValue((QObject *)item));
    }
    return l;
}


// copied from scripter/cmdutil.cpp
/// Convert an X co-ordinate part in page units to a document co-ordinate
/// in system units.
double pageXtoDocX(double x)
{
    return x + ScCore->primaryMainWindow()->doc->currentPage()->xOffset();
}


// copied from scripter/cmdutil.cpp
/// Convert a Y co-ordinate part in page units to a document co-ordinate
/// in system units. The document co-ordinates have their origin somewere
/// up and left of the first page, where page co-ordinates have their
/// origin on the top left of the current page.
double pageYtoDocY(double y)
{
    return y + ScCore->primaryMainWindow()->doc->currentPage()->yOffset();
}



QObject *PageAPI::newItem(const PageItem::ItemType itemType,
                          const PageItem::ItemFrameType frameType,
                          const double x, const double y, const double width,
                          const double height, const double w,
                          const QString& fill, const QString& outline)
{
    int i = ScCore->primaryMainWindow()->doc->itemAdd(
                itemType, frameType,
                pageXtoDocX(x), pageYtoDocY(y),
                width, height,
                w, fill, outline, true);
    return ScCore->primaryMainWindow()->doc->Items->at(i);
}



/**
 * Scripter.activeDocument.activePage.newRectangle(x, y, width, height)
 * x is a double
 * y is a double
 * width is a double
 * height is a double
 * returns Item object
 */
QObject *PageAPI::newRectangle(double x, double y, double width, double height)
{
    return newItem(
               PageItem::Polygon, PageItem::Rectangle, x, y, width, height,
               ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth,
               ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeFillColor,
               ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineColor);
}



/**
 * Scripter.activeDocument.activePage.newEllipse(x, y, width, height)
 * x is a double
 * y is a double
 * width is a double
 * height is a double
 * returns Item object
 */
QObject *PageAPI::newEllipse(double x, double y, double width, double height)
{
    return newItem(
               PageItem::Polygon, PageItem::Ellipse, x, y, width, height,
               ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth,
               ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeFillColor,
               ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineColor);
}



/**
 * Scripter.activeDocument.activePage.newImage(x, y, width, height)
 * x is a double
 * y is a double
 * width is a double
 * height is a double
 * returns Item object
 */
QObject *PageAPI::newImage(double x, double y, double width, double height)
{
    return newItem(
               PageItem::ImageFrame, PageItem::Unspecified, x, y, width, height,
               1,
               // @xxx: ale
               // ScCore->primaryMainWindow()->doc->toolSettings.dBrushPict,
               ScCore->primaryMainWindow()->doc->itemToolPrefs().imageFillColor,
               CommonStrings::None);
}



/**
 * Scripter.activeDocument.activePage.newText(x, y, width, height)
 * x is a double
 * y is a double
 * width is a double
 * height is a double
 * returns Item object
 */
QObject *PageAPI::newText(double x, double y, double width, double height)
{
    return newItem(
               PageItem::TextFrame, PageItem::Unspecified, x, y, width, height,
               ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth,
               CommonStrings::None,
               ScCore->primaryMainWindow()->doc->itemToolPrefs().textFont);
}



/**
 * Scripter.activeDocument.activePage.newLine(x, y, width, height)
 * x is a double
 * y is a double
 * width is a double
 * height is a double
 * returns Item object
 */
QObject *PageAPI::newLine(double x, double y, double width, double height)
{
    PageItem *item = (PageItem*)newItem(
                         PageItem::Line, PageItem::Unspecified, x, y, width, height,
                         ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineWidth,
                         ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeFillColor,
                         ScCore->primaryMainWindow()->doc->itemToolPrefs().shapeLineColor);
    item->setRotation(xy2Deg(width-x, height-y));
    item->setWidthHeight(sqrt(pow(x-width, 2.0) + pow(y-height, 2.0)), 1.0);
    item->Sizing = false;
    item->updateClip();
    return item;
}

// XXX: newPolyLine, bezierline


void PageAPI::placeImage(const int formatid, const QString & filename, const double x, const double y)
{
    const FileFormat * fmt = LoadSavePlugin::getFormatById(formatid);
    if ( fmt )
    {
        fmt->loadFile(filename,
                      LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
        if (ScCore->primaryMainWindow()->doc->m_Selection->count() > 1)
        {
            double x2, y2, w, h;
            ScCore->primaryMainWindow()->doc->m_Selection->getGroupRect(
                &x2, &y2, &w, &h);
            ScCore->primaryMainWindow()->view->startGroupTransaction();
            ScCore->primaryMainWindow()->doc->moveGroup(
                pageXtoDocX(x) - x2, pageYtoDocY(y) - y2);
            ScCore->primaryMainWindow()->view->endGroupTransaction();
            ScCore->primaryMainWindow()->propertiesPalette->updateColorList();
            ScCore->primaryMainWindow()->propertiesPalette->paraStyleCombo->updateFormatList();
            ScCore->primaryMainWindow()->propertiesPalette->charStyleCombo->updateFormatList();
            ScCore->primaryMainWindow()->propertiesPalette->SetLineFormats(
                ScCore->primaryMainWindow()->doc);
        }
    } else RAISE("Format plug-in not loaded.");
}


// XXX: join place* into one function and set formatid according to extension


/**
 * Scripter.activeDocument.activePage.placeSVG(filename x, y)
 * filename is string
 * x is double
 * y is double
 * import vector graphics at given position on active page
 */
void PageAPI::placeSVG(const QString & filename, const double x, const double y)
{
    placeImage(FORMATID_SVGIMPORT, filename, x, y);
}



/**
 * Scripter.activeDocument.activePage.placeODG(filename x, y)
 * filename is string
 * x is double
 * y is double
 * import vector graphics at given position on active page
 */
void PageAPI::placeODG(const QString & filename, const double x, const double y)
{
    placeImage(FORMATID_ODGIMPORT, filename, x, y);
}



/**
 * Scripter.activeDocument.activePage.placeEPS(filename x, y)
 * filename is string
 * x is double
 * y is double
 * import vector graphics at given position on active page
 */
void PageAPI::placeEPS(const QString & filename, const double x, const double y)
{
    placeImage(FORMATID_PSIMPORT, filename, x, y);
}



/**
 * Scripter.activeDocument.activePage.placeSXD(filename x, y)
 * filename is string
 * x is double
 * y is double
 * import vector graphics at given position on active page
 */
void PageAPI::placeSXD(const QString & filename, const double x, const double y)
{
    placeImage(FORMATID_SXDIMPORT, filename, x, y);
}
