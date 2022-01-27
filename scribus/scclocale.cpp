//
// C++ Implementation: sclocale
//
// Description: 
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "scclocale.h"

#include <cstdlib>
#include <sstream>

#include <QByteArray>
#include <QDebug>

ScCLocale * ScCLocale::m_instance = nullptr;
ScCLocale::ScCLocale()
	: qLocale(QLocale::C)
{
	qLocale.setNumberOptions(QLocale::OmitGroupSeparator);

#if defined(Q_OS_WIN)
	cLocale = _create_locale(LC_ALL, "C");
#else
  #if not defined(Q_OS_SOLARIS) and not defined(Q_OS_OPENBSD) and not defined(Q_OS_FREEBSD) and not defined(Q_OS_HAIKU)
	cLocale = newlocale(LC_ALL_MASK, "C", nullptr);
  #endif
#endif
}

ScCLocale::~ScCLocale()
{
#if defined(Q_OS_WIN)
	_free_locale(cLocale);
#else
  #if not defined(Q_OS_SOLARIS) and not defined(Q_OS_OPENBSD) and not defined(Q_OS_FREEBSD) and not defined(Q_OS_HAIKU)
	freelocale(cLocale);
  #endif
#endif
}

ScCLocale * ScCLocale::that()
{
	if (!m_instance)
	{
		m_instance = new ScCLocale();
		Q_ASSERT(m_instance);
	}
	return m_instance;
}

// The code of this function has been borrowed
// from Qt SVG module
double ScCLocale::toDoubleC(const QChar *&str)
{
	const int maxLen = 255;//technically doubles can go til 308+ but whatever
	char temp[maxLen+1];
	int pos = 0;

	if (*str == QLatin1Char('-')) {
		temp[pos++] = '-';
		++str;
	} else if (*str == QLatin1Char('+')) {
		++str;
	}
	while (isDigit(str->unicode()) && pos < maxLen) {
		temp[pos++] = str->toLatin1();
		++str;
	}
	if (*str == QLatin1Char('.') && pos < maxLen) {
		temp[pos++] = '.';
		++str;
	}
	while (isDigit(str->unicode()) && pos < maxLen) {
		temp[pos++] = str->toLatin1();
		++str;
	}
	bool exponent = false;
	if ((*str == QLatin1Char('e') || *str == QLatin1Char('E')) && pos < maxLen) {
		exponent = true;
		temp[pos++] = 'e';
		++str;
		if ((*str == QLatin1Char('-') || *str == QLatin1Char('+')) && pos < maxLen) {
			temp[pos++] = str->toLatin1();
			++str;
		}
		while (isDigit(str->unicode()) && pos < maxLen) {
			temp[pos++] = str->toLatin1();
			++str;
		}
	}

	temp[pos] = '\0';

	double val;
	if (!exponent && pos < 10) {
		int ival = 0;
		const char *t = temp;
		bool neg = false;
		if (*t == '-') {
			neg = true;
			++t;
		}
		while (*t && *t != '.') {
			ival *= 10;
			ival += (*t) - '0';
			++t;
		}
		if (*t == '.') {
			++t;
			int div = 1;
			while (*t) {
				ival *= 10;
				ival += (*t) - '0';
				div *= 10;
				++t;
			}
			val = ((double) ival) / ((double) div);
		} else {
			val = ival;
		}
		if (neg)
			val = -val;
	} else {
		val = QByteArray::fromRawData(temp, pos).toDouble();
	}
	return val;
}

double ScCLocale::toDoubleC(const QString & str, bool * ok) 
{
	double ret( that()->qLocale.toDouble(str, ok) );
	return ret;
}

double ScCLocale::toDoubleC(const QString& str, double defValue)
{
	double ret = defValue;
	if (!str.isEmpty())
	{
		bool ok  = false;
		double d = ScCLocale::toDoubleC(str, &ok);
		if (ok)
			ret = d;
	}
	return ret;
}

float ScCLocale::toFloatC(const QString & str, bool * ok) 
{
	double ret( that()->qLocale.toFloat(str, ok) );
	return ret;
}

float ScCLocale::toFloatC(const QString& str, float defValue)
{
	double ret = defValue;
	if (!str.isEmpty())
	{
		bool ok  = false;
		double d = ScCLocale::toFloatC(str, &ok);
		if (ok)
			ret = d;
	}
	return ret;
}

QString ScCLocale::toQStringC(double d, int prec)
{
	return that()->qLocale.toString(d, 'f', prec);
}

double ScCLocale::strtod(const char * str, char ** endptr)
{
	if (nullptr == that()->cLocale)
	{
		// a sad workaround
		double result(0.0);
		std::streamoff bytesRead;
		std::istringstream sstream(str);
		sstream.imbue(std::locale::classic());
		sstream >> result;
		bytesRead = sstream.eof() ? strlen(str) : (std::streamoff) sstream.tellg();
		*endptr = const_cast<char*>(str) + bytesRead;
		return result;
	}

#if defined(Q_OS_SOLARIS) || defined (Q_OS_OPENBSD) || defined(Q_OS_FREEBSD) || defined(Q_OS_HAIKU)
	double result(0.0);
	std::streamoff bytesRead;
	std::istringstream sstream(str);
	sstream.imbue(std::locale::classic());
	sstream >> result;
	bytesRead = sstream.eof() ? strlen(str) : (std::streamoff) sstream.tellg();
	*endptr = const_cast<char*>(str) + bytesRead;
	return result;
#elif defined(Q_OS_WIN)
	return _strtod_l(str, endptr, that()->cLocale);
#else
	return strtod_l(str, endptr, that()->cLocale);
#endif
}

