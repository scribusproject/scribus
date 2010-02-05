/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_ITEM_H_
#define API_ITEM_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"

class ItemAPI : public QObject
{
    Q_OBJECT
public:
    ItemAPI(QObject *parent, const QString &name, int type, int number);
    virtual ~ItemAPI();

public slots:
    // XXX: put your methods here
};

#endif /*API_ITEM_H_*/
