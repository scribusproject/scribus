/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_DOCUMENT_H_
#define API_DOCUMENT_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"



class DocumentAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool available READ available);
    Q_PROPERTY(QObject* margins READ margins);
    Q_PROPERTY(bool modified READ modified WRITE setModified);
    Q_PROPERTY(QObject* activePage READ activePage);
    Q_PROPERTY(int pageCount READ pageCount);
    Q_PROPERTY(QObject* activeItem READ activeItem);
    Q_PROPERTY(QObject* dimensions READ dimensions);
    Q_PROPERTY(QList<QVariant> items READ items);
    Q_PROPERTY(QList<QVariant> selection READ selection);
public:
    DocumentAPI();
    virtual ~DocumentAPI();

public slots:
    bool check();
    bool close();
    QList<QVariant> items();

private:
    QObject *margins();
    bool available();
    bool modified();
    void setModified(bool flag);
    QObject *activePage();
    int pageCount();
    QList<QVariant> selection();
    QObject *activeItem();
    QObject *dimensions();
};


class Margins : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double top READ top WRITE setTop);
    Q_PROPERTY(double left READ left WRITE setLeft);
    Q_PROPERTY(double right READ right WRITE setRight);
    Q_PROPERTY(double bottom READ bottom WRITE setBottom);

public:
    Margins(QObject *parent);
    virtual ~Margins()
    {
        qDebug() << "Margins deleted";
    };

public slots:
    void set(double lr, double tpr, double btr, double rr);

private:
    double top();
    void setTop(double value);
    double left();
    void setLeft(double value);
    double right();
    void setRight(double value);
    double bottom();
    void setBottom(double value);

};



class Dimensions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double width READ width);
    Q_PROPERTY(double height READ height);

public:
    Dimensions(QObject *parent);
    virtual ~Dimensions()
    {
        qDebug() << "Dimensions deleted";
    };

private:
    double width();
    double height();
    QList<QVariant> items;
};


#endif /*API_DOCUMENT_H_*/
