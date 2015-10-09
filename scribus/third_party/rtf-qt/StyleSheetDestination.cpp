/*
    Copyright (C)  2010, 2011  Brad Hards <bradh@frogmouth.net>

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

#include "StyleSheetDestination.h"

#include "rtfreader.h"
#include "controlword.h"

namespace RtfReader
{

    StyleSheetDestination::StyleSheetDestination( Reader *reader, AbstractRtfOutput *output, const QString &name ) :
      Destination( reader, output, name )
    {
	m_currentStyleHandleNumber = 0; /* default */
    }

    StyleSheetDestination::~StyleSheetDestination()
    {
    }

    void StyleSheetDestination::handleControlWord( const QString &controlWord, bool hasValue, const int value )
    {
	if ( controlWord == "ql" ) {
	    m_style.setTextAlignment( LeftAligned);
	} else if ( ( controlWord == "li" ) && hasValue ) {
	    m_style.setLeftIndent( value );
	} else if ( ( controlWord == "ri" ) && hasValue ) {
	    m_style.setRightIndent( value );
	} else if ( controlWord == "ltrch" ) {
	    m_style.setLayoutDirection( Qt::LeftToRight );
	} else if ( controlWord == "rtlch" ) {
	    m_style.setLayoutDirection( Qt::RightToLeft );
	} else if ( ( controlWord == "s" ) && hasValue ) {
	    m_currentStyleHandleNumber = value;
        } else if ( ( controlWord == "sb" ) && hasValue ) {
            m_style.setTopMargin( value );
        } else if ( controlWord == "sb" ) {
	//        qDebug() << "space before default (0)";
	} else {
	    if ( ControlWord::isDestination( controlWord ) ) {
	//	qDebug() << "unhandled **Destination** control word in StyleSheetDestination:" << controlWord;
	    } else {
	//	qDebug() << "unhandled control word in StyleSheetDestination:" << controlWord;
	    }
	}
    }

	void StyleSheetDestination::handlePlainText( const QByteArray &plainText )
    {
	if ( plainText == ";" ) {
	    m_output->insertStyleSheetTableEntry( m_currentStyleHandleNumber, m_style );
	} else if ( plainText.endsWith( ";" ) ) {
	    // probably a style name with a terminating delimiter
	    int delimiterPosition = plainText.indexOf( ";" );
	    if ( delimiterPosition == ( plainText.length() - 1) ) {
		// It is at the end, chop it off
		QString styleName = plainText.left( delimiterPosition );
		m_style.setStyleName( styleName );
		m_output->insertStyleSheetTableEntry( m_currentStyleHandleNumber, m_style );
	    } else {
		// we were not expecting a name with a delimiter other than at the end
//		qDebug() << "Style name with embedded delimiter: " << plainText;
	    }
	} else {
	    // plain font name
	    m_style.setStyleName( plainText );
	}
    }

    void StyleSheetDestination::aboutToEndDestination()
    {
	// TODO
    }
}
