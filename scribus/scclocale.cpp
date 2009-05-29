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
#include <QDebug>

ScCLocale * ScCLocale::m_instance = 0;
ScCLocale::ScCLocale()
	:qLocale(QLocale::C)
{
	qLocale.setNumberOptions(QLocale::OmitGroupSeparator);

#if defined(Q_WS_WIN)
	cLocale = _create_locale(LC_ALL, "C");
#else
	cLocale = newlocale(LC_ALL_MASK, "C", NULL);
#endif
}

ScCLocale::~ScCLocale()
{
#if defined(Q_WS_WIN)
	_free_locale(cLocale);
#else
	freelocale(cLocale);
#endif
}

ScCLocale * ScCLocale::that()
{
	if(!m_instance)
	{
		m_instance = new ScCLocale();
		Q_ASSERT(m_instance);
	}
	return m_instance;
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

QString ScCLocale::toQStringC(double d)
{
	return that()->qLocale.toString(d, 'f', 3);
}

double ScCLocale::strtod ( const char * str, char ** endptr )
{
	if(NULL == that()->cLocale)
	{
		// a sade workaround
		setlocale(LC_NUMERIC, "C");
		return strtod(str, endptr);
		setlocale(LC_NUMERIC, "");
	}
	else
	{
#if defined(Q_WS_WIN)
		return _strtod_l(str, endptr, that()->cLocale);
#else
		return strtod_l(str, endptr, that()->cLocale);
#endif
	}
}

