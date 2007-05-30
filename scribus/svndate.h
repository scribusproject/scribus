#ifndef SCRIBUS_SVNDATE_H
#define SCRIBUS_SVNDATE_H

#define SCRIBUS_SVN_REV "$Rev$"
#define SCRIBUS_SVN_DATE "$Date$"

#include <qstring.h>
#include <qdatetime.h>

/**
 * Return the subversion revision that this build is based on.
 */
inline int getSvnRev()
{
    // SCRIBUS_SVN_REV is in the format "$Rev$"
    return QString( SCRIBUS_SVN_REV ).section( QChar(' '), 1, 1 ).toInt();
}

/**
 * Return the date & time the subversion revision this build is based on was created
 * as a text string in "YYYY-MM-DD HH:MM:SS +Z" format.
 */
inline QString getSvnDateStr()
{
    return QString( SCRIBUS_SVN_DATE ).section( QChar(' '), 1, 3 );
}

/**
 * Return the date & time the subversion revision this build is based on was created
 * as a QDateTime. The QDateTime is in UTC time.
 */
inline QDateTime getSvnDateTime()
{
    // SCRIBUS_SVN_DATE is in the format "$Date$"
    const QString dateStr = QString( SCRIBUS_SVN_DATE ).section( QChar(' '), 1, 2);
    QDateTime tm = QDateTime::fromString( dateStr, Qt::ISODate );
    // Adjust the time based on time zone information. We discard the minutes from the timezone
    // since we just don't care that much. We need to convert to UTC because QDateTime's fromString
    // can't understand the timezone natively, and there's no guarantee that the local time where
    // this copy of svn was checked out is the same as the local time the program is being run in.
    tm.setTimeSpec(Qt::UTC);
    const int tzSecs = QString( SCRIBUS_SVN_DATE ).section( QChar(' '), 3, 3 ).left(3).toInt() * -3600;
    return tm.addSecs(tzSecs);
}

#endif
