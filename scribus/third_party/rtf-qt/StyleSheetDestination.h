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

#ifndef RTFREADER_STYLESHEETDESTINATION_H
#define RTFREADER_STYLESHEETDESTINATION_H

#include <QString>
#include <QColor>

#include "Destination.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
namespace RtfReader
{
	class Reader;

	class StyleSheetDestination: public Destination
	{
		public:
			StyleSheetDestination(Reader *reader, AbstractRtfOutput *output, const QString &name);
			virtual ~StyleSheetDestination();
			virtual void handleControlWord( const QString &controlWord, bool hasValue, const int value );
			virtual void handlePlainText(const QByteArray &plainText );
			virtual void aboutToEndDestination();

		protected:
			double pixelsFromTwips(const int twips);
			quint32 		m_currentStyleHandleNumber;
			int			m_currentStyleParent;
			ParagraphStyle m_textStyle;
			QByteArray m_styleName;
			QHash<quint32, int> m_stylesTable;
			int m_charactersToSkip;
			int m_uc_Value;
	};
}

#endif
