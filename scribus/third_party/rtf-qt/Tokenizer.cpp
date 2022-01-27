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

#include "Tokenizer.h"
#include <ctype.h>
#include <QDebug>

namespace RtfReader
{
	Tokenizer::Tokenizer(QIODevice *inputDevice) : m_inputDevice(inputDevice)
	{
	}

	bool Tokenizer::atEnd()
	{
		return m_inputDevice->atEnd();
	}

	void Tokenizer::pullControlWord(Token *token)
	{
		char next;
		while (m_inputDevice->getChar( &next ))
		{
			if ((next == ' ') || (next == '\r') || (next == '\n'))
			{
				break;
			}
			else if (isalpha(next))
			{
				token->name.append(next);
			}
			else if (isdigit(next) || (next == '-'))
			{
				token->parameter.append(next);
				token->hasParameter = true;
			}
			else
			{
				m_inputDevice->ungetChar(next);
				break;
			}
		}
	}

	void Tokenizer::pullControlSymbol(Token *token)
	{
		if (token->name == "\'")
		{
			char highNibbleHexDigit;
			char lowNibbleHexDigit;
			if (m_inputDevice->getChar(&highNibbleHexDigit) && m_inputDevice->getChar(&lowNibbleHexDigit) && isxdigit(highNibbleHexDigit) && isxdigit(lowNibbleHexDigit))
			{
				QString hexDigits;
				hexDigits.append(QChar(highNibbleHexDigit));
				hexDigits.append(QChar(lowNibbleHexDigit));
				uint codepoint = hexDigits.toUInt(0, 16);
				token->type = Plain;
				QByteArray encodedString;
				encodedString.resize(1);
				encodedString[0] = codepoint;
				token->name = encodedString;
			}
		}
		else if (token->name == "\\")
		{
			token->type = Plain;
			token->name = "\\";
		}
		else if (token->name == "~")
		{
			token->type = Plain;
			QByteArray encodedString;
			encodedString.resize(1);
			encodedString[0] = 0xA0;
			token->name = encodedString;
		}
		else if (token->name == "-")
		{
			token->type = Plain;
			QByteArray encodedString;
			encodedString.resize(1);
			encodedString[0] = 0xAD;
			token->name = encodedString;
		}
		else if (token->name == "{")
		{
			token->type = Plain;
		}
		else if (token->name == "}")
		{
			token->type = Plain;
		}
		else if (token->name == "*")
		{
		// don't need anything else here
		}
		else if ((token->name == "\r") || (token->name == "\n"))
		{
			token->name = "par";
		}
		else if (token->name == "\t")
		{
			token->name = "tab";
		}
		else
		{
			qDebug() << "unhandled control symbol in Tokenizer:" << token->name;
		}
	}

	void Tokenizer::pullControl(Token *token)
	{
		char c;
		m_inputDevice->getChar(&c);
		token->name.append(c);
		if (isalpha(c))
		{
			pullControlWord(token);
		}
		else
		{
			pullControlSymbol(token);
		}
	}

	void Tokenizer::pullPlainText(Token *token)
	{
		char c;
		while (m_inputDevice->getChar(&c))
		{
			switch (c)
			{
				case '{' :
				case '}' :
				case '\\':
					m_inputDevice->ungetChar(c);
					return;
					break;
				case '\r':
				case '\n':
					break;
				default:
					token->name.append(c);
			}
		}
	}

	Token Tokenizer::fetchToken()
	{
		Token token;
		token.hasParameter = false;
		char c;
		while (m_inputDevice->getChar(&c))
		{
			switch (c)
			{
				case '{' :
					token.type = OpenGroup;
					return token;
					break;
				case '}' :
					token.type = CloseGroup;
					return token;
					break;
				case '\r':
					break;
				case '\n':
					break;
				case '\\':
					token.type = Control;
					pullControl(&token);
					return token;
					break;
				default:
					token.type = Plain;
					token.name.append(c);
					pullPlainText(&token);
					return token;
			}
		}
		return token;
	}
}
