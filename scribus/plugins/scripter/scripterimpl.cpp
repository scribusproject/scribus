/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"


ScripterImpl::ScripterImpl() : QObject(QApplication::instance())
{
	setObjectName("Scripter");
	_instance = this;
	qDebug() << "ScripterImpl object created";
}



ScripterImpl::~ScripterImpl() {
    qDebug() << "destructor";
    Q_ASSERT(python);
    delete python;
    //Q_ASSERT(collected);
    //delete collected;
    _instance = NULL;
    qDebug() << "Scripter deleted";
};



ScripterImpl *ScripterImpl::instance()
{
    //return QApplication::instance()->findChild<ScripterImpl *>("Scripter");
    return _instance;
}


bool ScripterImpl::init()
{
    collected = new QObject(this);
    collected->setObjectName(QString("internal_garbage_collector"));
    new PreferencesAPI();
    new DialogsAPI();
    python = new Pythonize();
    Q_CHECK_PTR(python);
    path = ScPaths::instance().libDir() + "plugins/scripter/";
    //qRegisterMetaType< QList<QVariant*> >("QList<QVariant*>");
    QString init_py = path + "init_scripter.py";
    bool ok = runScript(init_py);
    return ok;
}



bool ScripterImpl::runScript(const QString & filename) {
    qDebug()  <<  "Running" << filename;
    if (!python->runScript(filename.toLocal8Bit().data()))
    {
        qDebug()  << "Running" << filename << "failed";
        return false;
    }
    return true;
}


bool ScripterImpl::cleanup()
{
    QString clean_py = path + "cleanup_scripter.py";
    return runScript(clean_py);
}


/**
 * Scripter.fromVariant(variant)
 * variant is a QVariant
 * returns instance of QObject-subclass
 *
 * This is a helper method for PyQt
 * Because PyQt cannot cast a variant to a QObject or QWidget
 * I hope that will change some time.
 */
QObject *ScripterImpl::fromVariant(const QVariant& v)
{

    if (qVariantCanConvert< QWidget* >(v))
    {
        QObject* obj = qvariant_cast< QWidget* >(v);
        return obj;
    }
    else if (qVariantCanConvert< QObject* >(v))
    {
        QObject* obj = qvariant_cast< QObject* >(v);
        return obj;
    }
    else
        return 0;
}


/**
 * Scripter.openDocument(filename)
 * filename is a string
 * returns opened active Document object
 */
QObject *ScripterImpl::openDocument(const QString & filename)
{
    bool ret = ScCore->primaryMainWindow()->loadDoc(filename);
    if (!ret)
    {
        RAISE("Failed to open " + filename);
        return NULL;
    }
    return activeDocument();
}



/**
 * Scripter.activeDocument
 * Property
 * returns a Document object if a document is open
 */
QObject *ScripterImpl::activeDocument() {
    if (ScCore->primaryMainWindow()->HaveDoc)
        return new DocumentAPI();
    else
        return NULL;
}


/**
 * Scripter.activeWindow
 * Property
 * returns a Window object if a window is open
 */
QObject *ScripterImpl::activeWindow() {
    if (ScCore->primaryMainWindow()->HaveDoc)
        return new WindowAPI();
    else
        return NULL;
}

/**
 * Scripter.newDocument(topMargin, bottomMargin, leftMargin, rightMargin, pageWIdth, pageHeight, orientation, firstPageNr, pagesTypes, facingPages, firstPageOrger, numPages)
 * topMargin is double
 * bottomMargin is double
 * leftMargin is double
 * rightMargin is double
 * pageWidth is double
 * pageHeight is double
 * orientation is int
 * firstPageNr is int
 * unit is int
 * pagesType is int
 * facingPages is int
 * firstPageOrder is int
 * numPages is int
 * returns new a new active Document object
 */
QObject *ScripterImpl::newDocument(
    double topMargin, double bottomMargin,
    double leftMargin, double rightMargin,
    double pageWidth, double pageHeight,
    int orientation, int firstPageNr, int unit, int pagesType,
    int facingPages, int firstPageOrder, int numPages)
{
    if (numPages <= 0)
        numPages = 1;
    if (pagesType == 0)
    {
        facingPages = 0;
        firstPageOrder = 0;
    }
    else
        facingPages = 1;
    // checking the bounds
    if (pagesType < firstPageOrder)
    {
        RAISE("firstPageOrder is bigger than allowed.");
        return NULL;
    }
    if (orientation == 1)
    {
        double x = pageWidth;
        pageWidth = pageHeight;
        pageHeight = x;
    }
    bool ret = ScCore->primaryMainWindow()->doFileNew(
                   pageWidth, pageHeight, topMargin, leftMargin,
                   rightMargin, bottomMargin,
                   // XXX: add later?
                   // autoframes. It's disabled in python
                   // columnDistance, numberCols, autoframes,
                   0, 1, false,
                   pagesType, unit, firstPageOrder,
                   orientation, firstPageNr, "Custom", true, numPages);
    if (!ret)
    {
        RAISE("Page creation failed");
        return NULL;
    }
    ScCore->primaryMainWindow()->doc->setPageSetFirstPage(pagesType, firstPageOrder);
    return activeDocument();
}




/*
 * slot which emits a signal for PyQt
 * init_scripter.py waits for it to create the menu
 */
void ScripterImpl::addToMainWindowMenu(ScribusMainWindow *mainwin)
{
    emit createMenu(mainwin);
}



/*
 * A simple test method: input some code and evaluate it
 * It was used before there was a script editor and console
 * I will keep it for now but will remove it later.
 */
bool ScripterImpl::test()
{
    bool ok;
    QString code = QInputDialog::getText(
                       0, tr("Scripter"), tr("Please enter a Python command:"), QLineEdit::Normal,
                       "import scripterconsole; scripterconsole.show_console()", &ok);
    if (ok && !code.isEmpty())
    {
        bool success = python->runString(code.toUtf8().data());
        if (!success)
            qDebug() << "python->runString(..) failed";
    }
    return true;
}



/**
 * Scripter.aboutScripter()
 *
 * Like qApp.aboutQt()
 * The quickest way to see if calling Scripter works.
 */
void ScripterImpl::aboutScripter()
{
    QMessageBox::information(
        0, //(QWidget*)doc->scMW(),
        tr("Scribus - Scripter Plugin"),
        tr("If you see this box, Scripter probably works :)"),
        QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
        QMessageBox::NoButton);
}


ScripterImpl *ScripterImpl::_instance;


