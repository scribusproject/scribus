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

#include "scribusapi.h"

class SCRIBUS_API ScCLocale
{
	ScCLocale();
	QLocale qLocale;

	static ScCLocale * m_instance;
	static ScCLocale * that();
	
	public:
		static double toDoubleC(const QString& str, bool * ok = 0);
		static double toDoubleC(const QString& str, double defValue);
		static float toFloatC(const QString& str, bool * ok = 0);
		static float toFloatC(const QString& str, float defValue);
		static QString toQStringC(double d);
		
};

#endif // SCCLOCALE_H


