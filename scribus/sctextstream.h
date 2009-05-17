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

#ifndef SCTEXTSTREAM_H
#define SCTEXTSTREAM_H

#include <QString>
#include <QTextStream>

#include "scribusapi.h"

/**
	This class fixes a bug in QTextStream up to version Qt 4.4: using locale-aware strtod() for parsing double/float arguments
 */
class SCRIBUS_API ScTextStream
{
private:
	QTextStream qts;

public:
	ScTextStream() : qts() {}
	ScTextStream(QIODevice* device) : qts(device) {}
	ScTextStream(QString * string, QIODevice::OpenMode openMode = QIODevice::ReadWrite) : qts(string, openMode) {}
	ScTextStream(QByteArray * array, QIODevice::OpenMode openMode = QIODevice::ReadWrite ) : qts(array, openMode) {}

	ScTextStream & operator<< ( const QString & val );
	ScTextStream & operator<< ( double val );

	ScTextStream & operator>> ( QString & val );
	ScTextStream & operator>> ( double & val );
	ScTextStream & operator>> ( float & val );
	ScTextStream & operator>> ( int & val );

	QString readAll ();
	QString readLine ( qint64 maxlen = 0 );

	bool atEnd () const;
};

#endif // SCTEXTSTREAM_H


