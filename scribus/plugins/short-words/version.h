/*! This is the Scribus Short Words various constants and preprocessor
definitions. There should be all SPACEs defined (unbreak, em, en etc.)

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>

This program is free software - see LICENSE file in the distribution
or documentation
*/

#ifndef _SCRIBUS_SHORTVER_H_
#define _SCRIBUS_SHORTVER_H_

#define RC_PATH QDir::convertSeparators( ScPaths::instance().pluginDir() + "/scribus-short-words.rc")
#define RC_PATH_USR QDir::homeDirPath() + QDir::convertSeparators("/.scribus/scribus-short-words.rc")

#define UNBREAKABLE_SPACE QString(QChar(29))
#define SPACE QChar(' ')

#endif
