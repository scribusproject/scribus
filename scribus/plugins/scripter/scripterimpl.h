/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIPTERIMPL_H
#define SCRIPTERIMPL_H

#include <QObject>
#include <QtDebug>

#include "pythonize.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribuscore.h"
#include "scpaths.h"
#include "api_prefs.h"
#include "api_item.h"
#include "api_page.h"
#include "api_dialogs.h"
#include "api_window.h"
#include "api_document.h"

// XXX make this INTO method inside a new util.cpp
#define RAISE(msg) QApplication::instance()->setProperty("MIKRO_EXCEPTION", QVariant(msg))
#define COLLECTOR ScripterImpl::instance()->collector()
#define SCRIPTER ScripterImpl::instance()

class QString;
class ScribusDoc;

//Q_DECLARE_METATYPE(QList<QVariant*>)

class ScripterImpl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* collector READ collector);
    Q_PROPERTY(QObject* activeDocument READ activeDocument);
    Q_PROPERTY(QObject* activeWindow READ activeWindow);

public:
    ScripterImpl();
    ~ScripterImpl();
    static ScripterImpl *instance();
    bool init();
    bool cleanup();
    void addToMainWindowMenu(ScribusMainWindow *mainwin);
    QObject *collector()  {
        return collected;
    };

public slots:
    void aboutScripter();
    QObject *openDocument(const QString & filename);
    QObject *newDocument(
        double topMargin, double bottomMargin,
        double leftMargin, double rightMargin,
        double pageWidth, double pageHeight,
        int orientation, int firstPageNr, int unit, int pagesType,
        int facingPages, int firstPageOrder, int numPages);
    QObject *fromVariant(const QVariant& v);
    bool test();

signals:
    void createMenu(QMainWindow *mainwin);

private:
    /*
      collected is used for dumb garbage collection.
      Use it as a parent to make sure that your object gets deleted
      inside the scripting environment if it is not needed anymore.
     */
    QObject *collected;
    Pythonize *python;
    static ScripterImpl *_instance;
    QString path;
    bool runScript(const QString & filename);
    QObject *activeDocument();
    QObject *activeWindow();
};

#endif
