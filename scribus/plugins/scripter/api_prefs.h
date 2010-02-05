/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_PREFS_H_
#define API_PREFS_H_

#include <QObject>
#include <QVariant>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "prefsmanager.h"

class PreferencesAPI : public QObject
{
    Q_OBJECT
public:
    PreferencesAPI();
    virtual ~PreferencesAPI();
    PrefsContext *prefs(const QString & context);

public slots:
    bool contains(const QString & context, const QString & key);
    QString getString(const QString & context, const QString & key);
    void setString(const QString & context, const QString & , const QString & value);
    int getInt(const QString & context, const QString & key);
    void setInt(const QString & context, const QString & key, int value);
    double getDouble(const QString & context, const QString & key);
    void setDouble(const QString & context, const QString & key, double value);
    bool getBool(const QString & context, const QString & key);
    void setBool(const QString & context, const QString & key, bool value);

};

#endif /*API_PREFS_H_*/
