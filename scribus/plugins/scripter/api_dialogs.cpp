/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QMessageBox>

#include "api_dialogs.h"

DialogsAPI::DialogsAPI() : QObject(SCRIPTER)
{
    qDebug() << "DialogsAPI loaded";
    setObjectName("dialogs");
}



DialogsAPI::~DialogsAPI()
{
    qDebug() << "DialogsAPI deleted";
}



/**
 * Scripter.dialogs.alert(message)
 * message is a string
 *
 * inspired by alert in Javascript
 */
void DialogsAPI::alert(const QString & message)
{
    QMessageBox::information(
        0, //(QWidget*)doc->scMW(),
        tr("Alert - Scribus"),
        message,
        QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
        QMessageBox::NoButton);
}
