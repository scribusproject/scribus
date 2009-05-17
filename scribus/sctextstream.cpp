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
#include "sctextstream.h"

ScTextStream & ScTextStream::operator<< ( const QString & val )
{
	qts << val;
	return *this;
}

ScTextStream & ScTextStream::operator<< ( double val )
{
	qts << ScCLocale::toQStringC(val);
	return *this;
}

ScTextStream & ScTextStream::operator>> ( QString & val )
{
	qts >> val;
	return *this;
}

ScTextStream & ScTextStream::operator>> ( double & val )
{
	QString s;
	qts >> s;
	val = ScCLocale::toDoubleC(s);
	return *this;
}

ScTextStream & ScTextStream::operator>> ( float & val )
{
	QString s;
	qts >> s;
	val = ScCLocale::toFloatC(s);
	return *this;
}

ScTextStream & ScTextStream::operator>> ( int & val )
{
	qts >> val;
	return *this;
}

QString ScTextStream::readAll ()
{
	return qts.readAll();
}

QString ScTextStream::readLine ( qint64 maxlen )
{
	return qts.readLine(maxlen);
}

bool ScTextStream::atEnd () const
{
	return qts.atEnd();
}
