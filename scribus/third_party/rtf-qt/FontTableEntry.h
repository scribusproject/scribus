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

#ifndef RTFREADER_FONTTABLEENTRY_H
#define RTFREADER_FONTTABLEENTRY_H
namespace RtfReader
{
	class FontTableEntry
	{
		public:
			FontTableEntry()
			{
				m_fontName = "";
				m_encoding = 0;
			}

			QString fontName() const
			{
				return m_fontName;
			}

			void setFontName(const QString &fontName)
			{
				m_fontName = fontName;
			}

			int encoding() const
			{
				return m_encoding;
			}

			void setEncoding(const int enc)
			{
				m_encoding = enc;
			}

		protected:
			QString m_fontName;
			int m_encoding;
	};
}

#endif
