/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_document.h"
#include "selection.h"

DocumentAPI::DocumentAPI() : QObject(COLLECTOR)
{
    qDebug() << "DocumentAPI loaded";
    setObjectName("activeDocument"); // XXX: support other documents later
}



DocumentAPI::~DocumentAPI()
{
    qDebug() << "DocumentAPI deleted";
}



/**
 * Scripter.activeDocument.available
 * Property
 * bool value if a document is active
 */
bool DocumentAPI::available()
{
    if (ScCore->primaryMainWindow()->HaveDoc)
        return true;
    return false;
}


/**
 * Scripter.activeDocument.check
 * returns True if found else False and raises error
 */
bool DocumentAPI::check()
{
    if (!available())
    {
        RAISE("Method call requires an open document, none found.");
        return false;
    }
    return true;
}


/**
 * Scripter.activeDocument.modified
 * Property
 * boolean flag if document is modified
 *
 * If a document is modified you will be asked to save it
 * on close or quit.
 */
void DocumentAPI::setModified(bool flag)
{
    if (!check()) return;
    ScCore->primaryMainWindow()->doc->setModified(flag);
}



bool DocumentAPI::modified()
{
    if (!check()) return NULL;
    return ScCore->primaryMainWindow()->doc->isModified();
}



/**
 * Scripter.activeDocument.close()
 *
 * closes the active document without asking save
 */
bool DocumentAPI::close()
{
    if (!check()) return NULL;
    setModified(false);
    bool ret = ScCore->primaryMainWindow()->slotFileClose();
    qApp->processEvents();
    return ret;
}



/**
 * Scripter.activeDocument.pageCount()
 * returns number of pages in active document as int
 */
int DocumentAPI::pageCount()
{
    return ScCore->primaryMainWindow()->doc->Pages->count();
}



/**
 * Scripter.activeDocument.activePage
 * Property
 * Page object of active page in active Document
 */
QObject *DocumentAPI::activePage() {
    return new PageAPI(this);
}


/**
 * Scripter.activeDocument.activeItem
 * Property
 * Currently active page item
 */
QObject *DocumentAPI::activeItem()
{
    if (ScCore->primaryMainWindow()->doc->m_Selection->count() == 0)
        return NULL;
    return ScCore->primaryMainWindow()->doc->m_Selection->itemAt(0);
}


/**
 * Scripter.activeDocument.selection
 * Property
 * List of selected Item objects on active document
 */
QList<QVariant> DocumentAPI::selection()
{
    QList<QVariant> l;
    Selection *sel = ScCore->primaryMainWindow()->doc->m_Selection;
    for (int i=0; i < sel->count(); i++)
        l.append(qVariantFromValue(
                     (QObject *)sel->itemAt(i)));
    return l;
}


/**
 * Scripter.activeDocument.dimensions
 * Property
 * Dimensions object of active page
 */
QObject *DocumentAPI::dimensions()
{
    return new Dimensions(this);
}


/**
 * Scripter.activeDocument.items
 * Property
 * List of all Item objects of active document
 */
QList<QVariant> DocumentAPI::items()
{
    QList<QVariant> l;
    for (int i = 0; i<ScCore->primaryMainWindow()->doc->Items->count(); ++i)
    {
        l.append(qVariantFromValue(
                     (QObject *)(ScCore->primaryMainWindow()->doc->Items->at(i))
                 ));
    }
    return l;
}


Dimensions::Dimensions(QObject *parent) : QObject(COLLECTOR) {
    qDebug() << "Dimensions created";
}



/**
 * Scripter.activeDocument.dimensions.width
 * Property
 * width of active document in points as double (read-only, for now)
 */
double Dimensions::width()
{
	return ScCore->primaryMainWindow()->doc->pageWidth();
}



/**
 * Scripter.activeDocument.dimensions.height
 * Property
 * height of active document in points as double (read-only, for now)
 */
double Dimensions::height()
{
	return ScCore->primaryMainWindow()->doc->pageHeight();
}



/**
 * Scripter.activeDocument.margins
 * Property
 * Margins object of active document
 */
QObject *DocumentAPI::margins()
{
    return new Margins(this);
}



Margins::Margins(QObject *parent) : QObject(COLLECTOR)
{
    qDebug() << "Margins created";
}



/**
 * Scripter.activeDocument.margins.top
 * Property
 * top-margin of active document as double
 */
double Margins::top()
{
	return ScCore->primaryMainWindow()->doc->margins()->Top;
}



/**
 * Scripter.activeDocument.margins.top
 * Property
 * left-margin of active document as double
 */
double Margins::left()
{
	return ScCore->primaryMainWindow()->doc->margins()->Left;
}



/**
 * Scripter.activeDocument.margins.top
 * Property
 * right-margin of active document as double
 */
double Margins::right() {
	return ScCore->primaryMainWindow()->doc->margins()->Right;
}



/**
 * Scripter.activeDocument.margins.top
 * Property
 * top-margin of active document as double
 */
double Margins::bottom() {
	return ScCore->primaryMainWindow()->doc->margins()->Bottom;
}


/**
 * Scripter.activeDocument.margins.set(lr, tpr, btr, rr)
 * args are double
 *
 * Use this method if you want to change more than one margin
 * at a time
 */

void Margins::set(double lr, double tpr, double btr, double rr)
{
    MarginStruct margins(tpr, lr, btr, rr);
	ScCore->primaryMainWindow()->doc->resetPage(ScCore->primaryMainWindow()->doc->pagePositioning(), &margins);
    ScCore->primaryMainWindow()->view->reformPages();
    ScCore->primaryMainWindow()->doc->setModified(true);
    ScCore->primaryMainWindow()->view->GotoPage(ScCore->primaryMainWindow()->doc->currentPageNumber());
    ScCore->primaryMainWindow()->view->DrawNew();
}



void Margins::setTop(double value)
{
    set(left(), value, bottom(), right());
}



void Margins::setLeft(double value)
{
    set(value, top(), bottom(), right());
}



void Margins::setRight(double value)
{
    set(left(), top(), bottom(), value);
}



void Margins::setBottom(double value)
{
    set(left(), top(), value, right());
}
