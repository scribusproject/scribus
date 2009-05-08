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
#include "scribusapi.h"

class SCRIBUS_API ScCLocale : public QLocale
{
	ScCLocale();
	static ScCLocale * m_instance;
	static ScCLocale * that();
	
	public:
		static double toDoubleC(const QString& str, bool * ok = 0);
		
};

#endif // SCCLOCALE_H

