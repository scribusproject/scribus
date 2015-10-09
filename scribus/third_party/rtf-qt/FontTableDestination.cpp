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

#include "FontTableDestination.h"

#include "rtfreader.h"

namespace RtfReader
{
	FontTableDestination::FontTableDestination(Reader *reader, AbstractRtfOutput *output, const QString &name) :  Destination(reader, output, name)
	{
	}

	FontTableDestination::~FontTableDestination()
	{
	}

	void FontTableDestination::handleControlWord(const QString &controlWord, bool hasValue, const int value)
	{
		if ( controlWord == "f" )
			m_currentFontTableIndex = value;
		else if (controlWord == "cpg")
			m_fontTableEntry.setEncoding(value);
		else if (controlWord == "fcharset")
		{
			if (value == 0)
				m_fontTableEntry.setEncoding(1252);
			else if (value == 1)
				m_fontTableEntry.setEncoding(1252);
			else if (value == 2)
				m_fontTableEntry.setEncoding(42);
			else if (value == 77)
				m_fontTableEntry.setEncoding(10000);
			else if (value == 78)
				m_fontTableEntry.setEncoding(10001);
			else if (value == 79)
				m_fontTableEntry.setEncoding(10003);
			else if (value == 80)
				m_fontTableEntry.setEncoding(10008);
			else if (value == 81)
				m_fontTableEntry.setEncoding(10002);
			else if (value == 83)
				m_fontTableEntry.setEncoding(10005);
			else if (value == 84)
				m_fontTableEntry.setEncoding(10004);
			else if (value == 85)
				m_fontTableEntry.setEncoding(10006);
			else if (value == 86)
				m_fontTableEntry.setEncoding(10081);
			else if (value == 87)
				m_fontTableEntry.setEncoding(10021);
			else if (value == 88)
				m_fontTableEntry.setEncoding(10029);
			else if (value == 89)
				m_fontTableEntry.setEncoding(10007);
			else if (value == 128)
				m_fontTableEntry.setEncoding(932);
			else if (value == 129)
				m_fontTableEntry.setEncoding(949);
			else if (value == 130)
				m_fontTableEntry.setEncoding(1361);
			else if (value == 134)
				m_fontTableEntry.setEncoding(936);
			else if (value == 136)
				m_fontTableEntry.setEncoding(950);
			else if (value == 161)
				m_fontTableEntry.setEncoding(1253);
			else if (value == 162)
				m_fontTableEntry.setEncoding(1254);
			else if (value == 163)
				m_fontTableEntry.setEncoding(1258);
			else if (value == 177)
				m_fontTableEntry.setEncoding(1255);
			else if (value == 178)
				m_fontTableEntry.setEncoding(1256);
			else if (value == 186)
				m_fontTableEntry.setEncoding(1257);
			else if (value == 204)
				m_fontTableEntry.setEncoding(1251);
			else if (value == 222)
				m_fontTableEntry.setEncoding(874);
			else if (value == 238)
				m_fontTableEntry.setEncoding(1250);
			else if (value == 254)
				m_fontTableEntry.setEncoding(437);
			else if (value == 255)
				m_fontTableEntry.setEncoding(850);
			else
				m_fontTableEntry.setEncoding(1252);
		}
	}

	void FontTableDestination::handlePlainText(const QByteArray &plainText)
	{
		if (plainText == ";")
			m_output->insertFontTableEntry(m_fontTableEntry, m_currentFontTableIndex);
		else if (plainText.endsWith(";"))
		{
			// probably a font name with a terminating delimiter
			int delimiterPosition = plainText.indexOf(";");
			if (delimiterPosition == (plainText.length() - 1))
			{
			// It is at the end, chop it off
				QString fontName = plainText.left(delimiterPosition);
				m_fontTableEntry.setFontName(fontName);
				m_output->insertFontTableEntry(m_fontTableEntry, m_currentFontTableIndex);
			}
		}
		else
			m_fontTableEntry.setFontName(plainText);
	}

	void FontTableDestination::aboutToEndDestination()
	{
	// TODO
	}
}
