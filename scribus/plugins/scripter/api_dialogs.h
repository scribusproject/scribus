/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_DIALOGS_H_
#define API_DIALOGS_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"

class DialogsAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QWidget* mainWindow READ mainWindow);
    Q_PROPERTY(QWidget* properties READ properties);
    Q_PROPERTY(QWidget* nodes READ nodes);
    Q_PROPERTY(QWidget* outline READ outline);
    Q_PROPERTY(QWidget* scrapbook READ scrapbook);
    Q_PROPERTY(QWidget* layers READ layers);
    Q_PROPERTY(QWidget* documentChecker READ documentChecker);
    Q_PROPERTY(QWidget* pages READ pages);
    Q_PROPERTY(QWidget* bookmarks READ bookmarks);
public:
    DialogsAPI();
    virtual ~DialogsAPI();

public slots:
    void alert(const QString & message);

private:
    /**
     * Scripter.dialogs.mainWindow
     * Property
     * QDialog object - details in doc/reference_dialogs.html
     */
    QWidget* mainWindow() {
        return ScCore->primaryMainWindow();
    };
    /**
     * Scripter.dialogs.properties
     * Property
     * QDialog object - details in doc/reference_dialogs.html
     */
    QWidget* properties() {
        return (QWidget*)ScCore->primaryMainWindow()->propertiesPalette;
    };
    /**
     * Scripter.dialogs.nodes
     * Property
     * QDialog object - details in doc/reference_dialogs.html
     */
    QWidget* nodes() {
        return (QWidget*)ScCore->primaryMainWindow()->nodePalette;
    };
    /**
     * Scripter.dialogs.outline
     * Property
     * QDialog object - details in doc/reference_dialogs.html
     */
    QWidget* outline() {
        return (QWidget*)ScCore->primaryMainWindow()->outlinePalette;
    };
    /**
     * Scripter.dialogs.scrapbook
     * Property
     * QDialog object - details in doc/reference_dialogs.html
     */
    QWidget* scrapbook() {
        return (QWidget*)ScCore->primaryMainWindow()->scrapbookPalette;
    };
    /**
     * Scripter.dialogs.layers
     * Property
     * QDialog object - details in doc/reference_dialogs.html
     */
    QWidget* layers() {
        return (QWidget*)ScCore->primaryMainWindow()->layerPalette;
    };
    /**
     * Scripter.dialogs.documentChecker
     * Property
     * QDialog object - details in doc/reference_dialogs.html
     */
    QWidget* documentChecker() {
        return (QWidget*)ScCore->primaryMainWindow()->docCheckerPalette;
    };
    /**
     * Scripter.dialogs.pages
     * Property
     * QDialog object - details in doc/reference_dialogs.html
     */
    QWidget* pages() {
        return (QWidget*)ScCore->primaryMainWindow()->pagePalette;
    };
    /**
     * Scripter.dialogs.bookmarks
     * Property
     * QDialog object - details in doc/reference_dialogs.html
     */
    QWidget* bookmarks() {
        return (QWidget*)ScCore->primaryMainWindow()->bookmarkPalette;
    };
};

#endif /*API_DIALOGS_H_*/
