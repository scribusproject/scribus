/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* \brief This is the Scribus Short Words various constants and preprocessor
definitions. There should be all SPACEs defined (unbreak, em, en etc.)

This code is based on the Scribus-Vlna plug in rewritten for
international use.

2004 Petr Vanek <petr@yarpen.cz>

This program is free software - see LICENSE file in the distribution
or documentation
*/

#ifndef _SCRIBUS_SHORTVER_H_
#define _SCRIBUS_SHORTVER_H_

#define RC_PATH QDir::convertSeparators(ScPaths::instance().shareDir() + "/plugins/scribus-short-words.rc")
#define RC_PATH_USR QDir::convertSeparators(ScPaths::getApplicationDataDir() + "/scribus-short-words.rc")

#define SPACE QChar(' ')

#endif
