/*
    Copyright (C)  2010  Brad Hards <bradh@frogmouth.net>

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "InfoTimeDestination.h"

#include "rtfreader.h"

namespace RtfReader
{
    InfoTimeDestination::InfoTimeDestination( Reader *reader, AbstractRtfOutput *output, const QString &name ) :
      Destination( reader, output, name )
    {
    }

    InfoTimeDestination::~InfoTimeDestination()
    {
    }

    void InfoTimeDestination::handleControlWord( const QString &controlWord, bool hasValue, const int value )
    {
	if ( controlWord == "yr" ) {
	    m_year = value;
	} else if ( controlWord == "mo" ) {
	    m_month = value;
	} else if ( controlWord == "dy" ) {
	    m_day = value;
	} else if ( controlWord == "hr" ) {
	    m_hour = value;
	} else if ( controlWord == "min" ) {
	    m_minute = value;
	} else {
//	    qDebug() << "unexpected control word in" << m_name << ": " << controlWord;
	}
    }

	void InfoTimeDestination::handlePlainText( const QByteArray &plainText )
    {
//	qDebug() << "unexpected text in InfoTimeDestination:" << plainText;
    }

    QDateTime InfoTimeDestination::dateTime() const
    {
	return QDateTime( QDate( m_year, m_month, m_day ),
			  QTime( m_hour, m_minute, 0 ) );
    }
}
