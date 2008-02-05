/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef _UTIL_FILE_H
#define _UTIL_FILE_H

#include "scribusapi.h"

class QDataStream;
class QString;
class ScStreamFilter;

/**
* @brief Copy a source file to a target
   * 
   * This function copy a file to a destination. If destination exists, 
   * the target file is overwritten.
   *
   * @param  source the source file
   * @param  target the target file
   * @return true on success, false on failure.
**/
bool SCRIBUS_API copyFile(const QString& source, const QString& target);
/**
* @brief Copy a source file to a stream filter
   * 
   * This function copy a file to a stream filter. The target filter has
   * to be opened before the function call.
   *
   * @param  source the source file
   * @param  target the target filter
   * @return true on success, false on failre.
**/
bool SCRIBUS_API copyFileToFilter(const QString& source, ScStreamFilter& target);
/**
* @brief Copy a source file to a data stream
   * @param  source the source file
   * @param  target the target stream
   * @return true on success, false on failre.
**/
bool SCRIBUS_API copyFileToStream(const QString& source, QDataStream& target);
/**
* @brief Move a source file to a destination
   * 
   * This function move a file to a destination. The source file is deleted
   * when done.
   *
   * @param  source the source file
   * @param  target the target file
   * @return true on success, false on failre.
**/
bool SCRIBUS_API moveFile(const QString& source, const QString& target);

#endif
