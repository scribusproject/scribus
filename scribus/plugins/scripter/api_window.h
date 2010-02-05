/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_WINDOW_H_
#define API_WINDOW_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"

class WindowAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double zoom READ zoom WRITE setZoom);
    Q_PROPERTY(bool redraw READ redraw WRITE setRedraw);

public:
    WindowAPI();
    virtual ~WindowAPI();

public slots:
    // XXX: relative or absolute scrolling?
    void scroll(int x, int y);
    // XXX: better name?
    void update();

private:
    void setRedraw(bool flag);
    bool redraw();
    // TODO: add zoomInto
    void setZoom(double factor);
    double zoom();
};

#endif /*API_WINDOW_H_*/
