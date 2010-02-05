/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_item.h"

ItemAPI::ItemAPI(QObject *parent, const QString &name, int type, int number
                ) : QObject(
                            QApplication::instance()->findChild<ScripterImpl *>("Scripter"))
{
    qDebug() << "ItemAPI loaded";
    setObjectName("Item");
}



ItemAPI::~ItemAPI()
{
    qDebug() << "ItemAPI deleted";
}


