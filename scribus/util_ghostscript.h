/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/*!
 * @brief GS utility fucntions
 * Moved functions written or modified by various people over the time from util.cpp
 * 
 * @author Craig Bradney
 */
#ifndef _GSUTIL_H
#define _GSUTIL_H

#include <QMap>
#include <QPixmap>
#include <QString>
#include <QStringList>

#include "scribusapi.h"

QPixmap SCRIBUS_API LoadPDF(QString fn, int Page, int Size, int *w, int *h);
/**
 * @brief Call GhostScript synchronously and store output
   *
   * The gs commands are all similar and consist of a few constant
   * arguments, the variable arguments and the end arguments which
   * are also invariant. It will always use -q -dNOPAUSE and
   * will always end with -c showpage -c quit. It also does automatic
   * device selection unless overridden, and uses the user's antialiasing
   * preferences and font search path.
   *
   * Shell metacharacters are not expanded - that includes quotes.
   * @sa System .
   *
   * @param args_in Custom arguments to GhostScript
   * @param device GS device to use (defaults to an image device if omitted)
 */
int     SCRIBUS_API callGS(const QStringList& args_in, const QString device="");
int     SCRIBUS_API callGS(const QString& args_in, const QString device="");
int     SCRIBUS_API convertPS2PS(QString in, QString out, const QStringList& opts, int level);
int     SCRIBUS_API convertPS2PDF(QString in, QString out, const QStringList& opts);
bool    SCRIBUS_API testGSAvailability( void );
bool    SCRIBUS_API testGSAvailability( const QString& gsPath );
bool    SCRIBUS_API testGSDeviceAvailability( const QString& device );
/*! \brief Return gs version. If gs couldn't be found or there was a problem parsing output, return false
 (in which case minor and major have undefined values).
 \retval QString version or false on error */
QString SCRIBUS_API getGSVersion();
bool    SCRIBUS_API getNumericGSVersion(int & major, int & minor);
bool    SCRIBUS_API getNumericGSVersion(const QString& ver, int&major, int& minor);
QString SCRIBUS_API getGSDefaultExeName(void);
QMap<int, QString> SCRIBUS_API getGSExePaths(const QString& regKey, bool alternateView = false);
#endif
