/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_window.h"
#include "canvas.h"

WindowAPI::WindowAPI() : QObject(COLLECTOR)
{
    qDebug() << "WindowAPI loaded";
    // XXX: support other windows later
    setObjectName("activeWindow");
}



WindowAPI::~WindowAPI()
{
    qDebug() << "WindowAPI deleted";
}


/**
 * Scripter.activeWindow.zoom
 * Property
 * The zoom factor is a double > 0.0
 *
 * -100.0 is special value to see the whole page.
 */
void WindowAPI::setZoom(double factor)
{
    if (factor > 0.0 || factor == -100.0)
        ScCore->primaryMainWindow()->slotZoom(factor);
    else
        RAISE("ValueError: Invalid zoom factor, should be greater than 0 or -100");
}



double WindowAPI::zoom()
{
    // XXX: better use the following?
    // ScCore->primaryMainWindow()->view->m_canvas->scale()/Prefs->DisScale*100
    // @xxx: ale: yes?
    // return ScCore->primaryMainWindow()->view->zoomSpinBox->value();
    return ScCore->primaryMainWindow()->view->m_canvas->scale()/PrefsManager::instance()->appPrefs.displayPrefs.displayScale*100;
}



/**
 * Scripter.activeWindow.scroll(x, y)
 * x is an int
 * y is an int
 *
 * scrolls the visible view to (x, y)
 */
void WindowAPI::scroll(int x, int y)
{
    ScCore->primaryMainWindow()->view->scrollBy(x, y);
}



/**
 * Scripter.activeWindow.update()
 *
 * forces a complete redraw of the view
 */
void WindowAPI::update()
{
    ScCore->primaryMainWindow()->view->DrawNew();
}


/**
 * Scripter.activeWindow.redraw
 * Property
 * boolean flag
 *
 * This can be used to switch of redrawing for complex operations
 * to safe time and reduce flicker.
 * Don't forget to activate redrawing later.
 * After that you can redraw the complete view with
 * ->Scripter.activeWindow.update()
 */
void WindowAPI::setRedraw(bool flag)
{
    ScCore->primaryMainWindow()->doc->DoDrawing = flag;
}



bool WindowAPI::redraw()
{
    return ScCore->primaryMainWindow()->doc->DoDrawing;
}
