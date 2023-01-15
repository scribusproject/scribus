//
// C++ Interface: sclocale
//
// Description: 
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SCCLOCALE_H
#define SCCLOCALE_H

#include <QLocale>
#include <QString>

#include <clocale>
#if defined(Q_OS_MACOS)
#include <xlocale.h>
#endif

#if defined(Q_OS_SOLARIS) || defined (Q_OS_OPENBSD) || defined(Q_OS_FREEBSD) || defined(Q_OS_HAIKU)
#include <locale.h>
#endif

#if defined(Q_OS_WIN)
#define XLocaleType _locale_t
#else
  #if defined (Q_OS_SOLARIS) || defined (Q_OS_OPENBSD) || defined(Q_OS_FREEBSD) || defined(Q_OS_HAIKU)
  #define XLocaleType char*   //dummy?
  #else
  #define XLocaleType locale_t
  #endif
#endif

#include "scribusapi.h"

class SCRIBUS_API ScCLocale
{
	ScCLocale();
	~ScCLocale();

	QLocale qLocale;
	XLocaleType cLocale { nullptr };

	static ScCLocale * m_instance;
	static ScCLocale * that();
	
	public:
		static inline bool isDigit(ushort ch);
		static double toDoubleC(const QChar *&str);
		static double toDoubleC(const QString& str, bool* ok = nullptr);
		static double toDoubleC(const QString& str, double defValue);
		static float toFloatC(const QString& str, bool* ok = nullptr);
		static float toFloatC(const QString& str, float defValue);
		static QString toQStringC(double d, int prec = 3);
		static double strtod(const char * str, char ** endptr);
};

// The code of this function has been borrowed from Qt SVG module
inline bool ScCLocale::isDigit(ushort ch)
{
	static quint16 magic = 0x3ff;
	return ((ch >> 4) == 3) && (magic >> (ch & 15));
}

#endif // SCCLOCALE_H



