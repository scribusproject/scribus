/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          util.cpp  -  description
                             -------------------
    begin                : Fri Sep 14 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#if !defined(_WIN32) && !defined(Q_OS_MAC)
#include <execinfo.h>
#include <cxxabi.h>
#endif
#if defined(_WIN32)
#include <windows.h>
#endif
#include <QDateTime>
#include "util_debug.h"

void sDebug(QString message)
{
	qDebug("%s", message.toLatin1().constData());
}


void tDebug(QString message)
{
	QDateTime debugTime;
	qDebug("%s", QString("%1\t%2").arg(debugTime.currentDateTime().toString("hh:mm:ss:zzz"), message).toLatin1().constData());
}


/**
 * Print a backtrace
 * Please never commit code that uses it.
 * @param nFrames specify how much frames you want to be printed
 */
void printBacktrace ( int nFrames )
{
#if !defined(_WIN32) && !defined(Q_OS_MAC) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_FREEBSD)
	void ** trace = new void*[nFrames + 1];
	char **messages = ( char ** ) NULL;
	int i, trace_size = 0;

	trace_size = backtrace ( trace, nFrames + 1 );
	messages = backtrace_symbols ( trace, trace_size );
	if ( messages )
	{
		for ( i=1; i < trace_size; ++i )
		{
			QString msg ( messages[i] );
			int sep1 ( msg.indexOf ( "(" ) );
			int sep2 ( msg.indexOf ( "+" ) );
			QString mName (	msg.mid ( sep1 + 1,sep2-sep1 -1) );
			
			QString name;
			if(mName.startsWith("_Z"))
			{
				char* outbuf = 0; 
				size_t length = 0;
				int status = 0; 
				outbuf = abi::__cxa_demangle(mName.trimmed().toLatin1().data(), outbuf, &length, &status);
				name = QString::fromLatin1( outbuf );
				if(0 == status)
				{
//					qDebug()<<"Demangle success["<< length <<"]"<<name;
					free(outbuf);
				}
//				else
//				{
//					qDebug()<<"Demangle failed ["<<status<<"]["<< mName.trimmed() <<"]";
//					continue;
//				}
			}
			else
				name = mName;
			if(name.isEmpty())
				name = mName;
			QString bts ( "[ScBT] %1. %2" );
			qDebug ("%s", bts.arg( i ).arg( name ).toUtf8().data() );
		}
		free ( messages );
	}
	delete[] trace;
#endif
}

