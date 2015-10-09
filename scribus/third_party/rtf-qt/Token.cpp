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

#include "Token.h"

#include <QDebug>

namespace RtfReader
{
    void Token::dump() const
    {
/*        switch ( type ) {
	  case OpenGroup:
	    qDebug() << "token type: OpenGroup";
	    break;
	  case CloseGroup:
	    qDebug() << "token type: CloseGroup";
	    break;
	  case Control:
	    qDebug() << "token type: Control";
	    break;
	  case Plain:
	    qDebug() << "token type: Plain";
	    break;
	  case Binary:
	    qDebug() << "token type: Binary";
	    break;
	  default:
	    qDebug() << "unexpected token type: " << type;
	}
	if ( type > CloseGroup ) {
	    qDebug() << "name: " << name;
	    if ( hasParameter ) {
		qDebug() << "parameter: " << parameter;
	    }
	}*/
    }
}
