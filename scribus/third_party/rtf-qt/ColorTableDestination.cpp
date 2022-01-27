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

#include "ColorTableDestination.h"

#include "rtfreader.h"

namespace RtfReader
{
    ColorTableDestination::ColorTableDestination( Reader *reader, AbstractRtfOutput *output, const QString &name ) :
      Destination( reader, output, name )
    {
	m_currentColor = Qt::black; // this is our default color
    }

    ColorTableDestination::~ColorTableDestination()
    {}

    void ColorTableDestination::handleControlWord( const QString &controlWord, bool hasValue, const int value )
    {
	if ( controlWord == "red" ) {
	    m_currentColor.setRed( value );
	} else if (controlWord == "green" ) {
	    m_currentColor.setGreen( value );
	} else if (controlWord == "blue" ) {
	    m_currentColor.setBlue( value );
	} else {
//	    qDebug() << "unexpected control word in colortbl:" << controlWord;
	}
    }

	void ColorTableDestination::handlePlainText( const QByteArray &plainText )
    {
	if ( plainText == ";" ) {
	    m_output->appendToColourTable( m_currentColor );
	    resetCurrentColor();
	} else {
//	    qDebug() << "unexpected text in ColorTableDestination:" << plainText;
	}
    }

    void ColorTableDestination::resetCurrentColor()
    {
	m_currentColor = Qt::black;
    }
}
