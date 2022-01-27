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

#include "UserPropsDestination.h"

#include "rtfreader.h"

namespace RtfReader
{
    UserPropsDestination::UserPropsDestination( Reader *reader, AbstractRtfOutput *output, const QString &name ) :
      Destination( reader, output, name ), m_nextPlainTextIsPropertyName( true ),
      m_propertyType( QVariant::String )
    {
    }

    UserPropsDestination::~UserPropsDestination()
    {}

    void UserPropsDestination::handleControlWord( const QString &controlWord, bool hasValue, const int value )
    {
	if ( controlWord == "propname" ) {
	    m_nextPlainTextIsPropertyName = true;
	} else if ( ( controlWord == "proptype" ) && hasValue ) {
	    if ( value == 30 ) {
		m_propertyType = QVariant::String;
	    } else if ( value == 3 ) {
		m_propertyType = QVariant::Int;
	    } else if ( value == 5 ) {
		m_propertyType = QVariant::Double;
	    } else if ( value == 64 ) {
	      	m_propertyType = QVariant::Date;
	    } else if ( value == 11 ) {
		m_propertyType = QVariant::Bool;
	    } else {
	//	qDebug() << "unhandled value type in UserPropsDestination:" << value;
	    }
	} else if ( controlWord == "staticval" ) {
	    m_nextPlainTextIsPropertyName = false;
	} else {
	//    qDebug() << "unexpected control word in UserPropsDestination:" << controlWord;
	}
    }

	void UserPropsDestination::handlePlainText( const QByteArray &plainText )
    {
	if ( m_nextPlainTextIsPropertyName ) {
	    m_propertyName = plainText;
	} else {
	    QVariant value;
	    if ( m_propertyType == QVariant::String ) {
		value = QVariant( plainText );
	    } else {
		// TODO: Really need some examples of this stuff - int, float, date and boolean
	//	qDebug() << "unhandled value type in UserPropsDestination:" << m_propertyType;
	    }
	    m_output->addUserProp( m_propertyName, value );
	}
    }
}
