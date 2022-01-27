/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_prefs.h"

#include "prefsfile.h"
#include "prefscontext.h"
#include "prefstable.h"
#include "prefsmanager.h"
#include "scripterimpl.h"

PreferencesAPI::PreferencesAPI() : QObject(SCRIPTER)
{
    setObjectName("preferences");
    qDebug() << "PreferencesAPI loaded";
}



PreferencesAPI::~PreferencesAPI()
{
    qDebug() << "PreferencesAPI deleted";
}



/*
 * XXX: getContext, getUserPrefsContex, raise if hasContext = false
 */
PrefsContext *PreferencesAPI::prefs(const QString & context)
{
    return PrefsManager::instance()->prefsFile->getPluginContext(context);
}



/**
 * Scripter.preferences.contains(context, key)
 * context is a string of a plug-in context, e.g. "scripter"
 * key is a string
 *
 * Use this to check if a preference is currently set.
 */
bool PreferencesAPI::contains(const QString & context, const QString & key)
{
    return prefs(context)->contains(key);
}



/**
 * Scripter.preferences.getString(context, key)
 * context is a string
 * key is a string
 * returns value of key as string or "" if key not found
 */
QString PreferencesAPI::getString(const QString & context, const QString & key)
{
    return prefs(context)->get(key);
}



/**
 * Scripter.preferences.setString(context, key, value)
 * context is string
 * key is string
 * value is string
 */
void PreferencesAPI::setString(const QString & context, const QString & key, const QString & value) {
    prefs(context)->set(key, value);
}



/**
 * Scripter.preferences.getInt(context, key)
 * context is string
 * key is string
 * returns value of key as int or -1 if key not found
 */
int PreferencesAPI::getInt(const QString & context, const QString & key) {
    return prefs(context)->getInt(key);
}



/**
 * Scripter.preferences.setInt(context, key, value)
 * context is string
 * key is string
 * value is int
 */
void PreferencesAPI::setInt(const QString & context, const QString & key, int value) {
    prefs(context)->set(key, value);
}



/**
 * Scripter.preferences.getDouble(context, key)
 * context is string
 * key is string
 * returns value of key as double of -1 if key not found
 */
double PreferencesAPI::getDouble(const QString & context, const QString & key) {
    return prefs(context)->getDouble(key);
}



/**
 * Scripter.preferences.setDouble(context, key, value)
 * context is string
 * key is string
 * value is double
 */
void PreferencesAPI::setDouble(const QString & context, const QString & key, double value) {
    prefs(context)->set(key, value);
}



/**
 * Scripter.preferences.getBool(context, key)
 * context is string
 * key is string
 * returns value of key as bool or False if key not found
 *
 * Use ->Scripter.preferences.contains(context, key) if you
 * don't know if the key exists.
 */
bool PreferencesAPI::getBool(const QString & context, const QString & key) {
    return prefs(context)->getBool(key);
}



/**
 * Scripter.preferences.setBool(context, key, value)
 * context ist string
 * key is string
 * value is bool
 */
void PreferencesAPI::setBool(const QString & context, const QString & key, bool value) {
    prefs(context)->set(key, value);
}

