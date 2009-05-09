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
