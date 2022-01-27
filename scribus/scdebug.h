/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	copyright            : (C) 2010 by Marcus Holland-Moritz
	email                : scribus@mhxnet.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

/**
  * @brief A per-file debug stream based on qDebug()
  *
  * Define SC_DEBUG_FILE to zero (debugging disabled) or non-zero
  * (debugging enabled) before including this file. Not defining it
  * means enabling debug support unless QT_NO_DEBUG_OUTPUT is defined.
  * Debugging will be disabled at compile time, so there's no need to
  * comment all lines that generate debug output.
  *
  * @author Marcus Holland-Moritz
  */

#if defined(QT_NO_DEBUG_OUTPUT) || (defined(SC_DEBUG_FILE) && SC_DEBUG_FILE == 0)

class ScNoDebug
{
public:
	inline ScNoDebug() {}
	inline ~ScNoDebug() {}
};
template<typename T>
inline ScNoDebug operator<<(ScNoDebug debug, const T &) { return debug; }
inline ScNoDebug scDebug() { return ScNoDebug(); }

#else

#include <QDebug>
#include <QTime>
inline QDebug scDebug() { return QDebug(QtDebugMsg) << QTime::currentTime().toString("[hh:mm:ss.zzz]"); }

#endif
