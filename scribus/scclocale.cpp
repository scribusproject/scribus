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
	:QLocale(QLocale::C)
{
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
	double ret( that()->toDouble(str, ok) );
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
	double ret( that()->toFloat(str, ok) );
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
	return that()->toString(d);
}
