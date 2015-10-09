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

#include "InfoDestination.h"

#include "rtfreader.h"

namespace RtfReader
{
    InfoDestination::InfoDestination( Reader *reader, AbstractRtfOutput *output, const QString &name ) :
      Destination( reader, output, name )
    {
    }

    InfoDestination::~InfoDestination()
    {}

    void InfoDestination::handleControlWord( const QString &controlWord, bool hasValue, const int value )
    {
	if ( ( controlWord == "edmins" ) && hasValue ) {
	    m_output->setTotalEditingTime( value );
	} else if ( ( controlWord == "nofpages" ) && hasValue ) {
	    m_output->setNumberOfPages( value );
	} else if ( ( controlWord == "nofwords" ) && hasValue ) {
	    m_output->setNumberOfWords( value );
	} else if ( ( controlWord == "nofchars" ) && hasValue ) {
	    m_output->setNumberOfCharacters( value );
	} else if ( ( controlWord == "nofcharsws" ) && hasValue ) {
	    m_output->setNumberOfCharactersWithoutSpaces( value );
	} else if ( ( controlWord == "version" ) && hasValue ) {
	    m_output->setVersionNumber( value );
	} else if ( ( controlWord == "vern" ) && hasValue ) {
	    m_output->setInternalVersionNumber( value );
	} else if ( controlWord == "*" ) {
	    // handled elsewhere
	} else {
//	    qDebug() << "unexpected control word in InfoDestination:" << controlWord;
	}
    }

	void InfoDestination::handlePlainText( const QByteArray &plainText )
    {
//	qDebug() << "unexpected text in InfoDestination:" << plainText;
    }
}
