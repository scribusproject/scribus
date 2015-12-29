/*
    Copyright (C)  2010,2011  Brad Hards <bradh@frogmouth.net>

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

#include "DocumentDestination.h"

#include "rtfreader.h"
#include "controlword.h"

namespace RtfReader
{
	DocumentDestination::DocumentDestination(Reader *reader, AbstractRtfOutput *output, const QString &name) : Destination(reader, output, name), m_charactersToSkip(0), m_tabType(0), m_uc_Value(1)
	{
	}

	DocumentDestination::~DocumentDestination()
	{
	}

	void DocumentDestination::handleControlWord(const QString &controlWord, bool hasValue, const int value)
	{
		if (controlWord == "par")
			m_output->insertPar();
		else if (controlWord == "tab")
			m_output->insertTab();
		else if (controlWord == "lquote")
			m_output->insertLeftQuote();
		else if (controlWord == "rquote")
			m_output->insertRightQuote();
		else if (controlWord == "ldblquote")
			m_output->insertLeftDoubleQuote();
		else if (controlWord == "rdblquote")
			m_output->insertRightDoubleQuote();
		else if (controlWord == "endash")
			m_output->insertEnDash();
		else if (controlWord == "emdash")
			m_output->insertEmDash();
		else if (controlWord == "enspace")
			m_output->insertEnSpace();
		else if (controlWord == "emspace")
			m_output->insertEmSpace();
		else if (controlWord == "bullet")
			m_output->insertBullet();
		else if (controlWord == "line")
			m_output->insertNewLine();
		else if (controlWord == "pard")
			m_output->resetParagraphFormat();
		else if (controlWord == "i")
			m_output->setFontItalic(!hasValue || (hasValue && value != 0));
		else if (controlWord == "b")
			m_output->setFontBold(!hasValue || (hasValue && value != 0));
		else if (controlWord == "ul")
			m_output->setFontUnderline(! hasValue || (hasValue && value != 0));
		else if (controlWord == "ulw")
			m_output->setFontWordUnderline(! hasValue || (hasValue && value != 0));
		else if (controlWord == "outl")
			m_output->setFontOutline(! hasValue || (hasValue && value != 0));
		else if (controlWord == "shad")
			m_output->setFontShadow(! hasValue || (hasValue && value != 0));
		else if (controlWord == "scaps")
			m_output->setFontSmallCaps(! hasValue || (hasValue && value != 0));
		else if (controlWord == "caps")
			m_output->setFontCaps(! hasValue || (hasValue && value != 0));
		else if (controlWord == "strike")
			m_output->setFontStrikeOut(! hasValue || (hasValue && value != 0));
		else if (controlWord == "fs")
		{
			if (hasValue && ( value != 0))
				m_output->setFontPointSize(value / 2.0);
			else
			m_output->setFontPointSize(24.0 / 2.0); // use default
		}
		else if (controlWord == "ansicpg")
			m_output->setEncoding(value);
		else if (controlWord == "charscalex")
			m_output->setFontScaleH(value);
		else if (controlWord == "dn")
			m_output->setFontOffset(-value);
		else if (controlWord == "up")
			m_output->setFontOffset(value);
		else if (controlWord == "expnd")
			m_output->setFontStretch(value);
		else if (controlWord == "expndtw")
			m_output->setFontStretchTw(value);
		else if (controlWord == "cf")
			m_output->setForegroundColour(value);
		else if (controlWord == "cbpat" )
			m_output->setParagraphPatternBackgroundColour(value);
		else if (controlWord == "chcbpat" )
			m_output->setCharacterPatternBackgroundColour(value);
		else if (controlWord == "highlight")
			m_output->setHighlightColour(value);
		else if ((controlWord == "deff") && hasValue)
			m_output->setDefaultFont(value);
		else if (controlWord == "f")
			m_output->setFont(value);
		else if (controlWord == "super")
			m_output->setFontSuperscript();
		else if (controlWord == "sub")
			m_output->setFontSubscript();
		else if (controlWord == "ql")
			m_output->setParagraphAlignmentLeft();
		else if (controlWord == "qj")
			m_output->setParagraphAlignmentJustified();
		else if (controlWord == "qc")
			m_output->setParagraphAlignmentCentred();
		else if (controlWord == "qr")
			m_output->setParagraphAlignmentRight();
		else if ((controlWord == "fi") && hasValue)
			m_output->setFirstLineIndent(value);
		else if ((controlWord == "li") && hasValue)
			m_output->setLeftIndent(value);
		else if ((controlWord == "ri") && hasValue)
			m_output->setRightIndent(value);
		else if ((controlWord == "sb") && hasValue)
			m_output->setSpaceBefore(value);
		else if (controlWord == "sb")
			m_output->setSpaceBefore(0);
		else if ((controlWord == "sa") && hasValue)
			m_output->setSpaceAfter(value);
		else if (controlWord == "sa")
			m_output->setSpaceAfter(0);
		else if (controlWord == "sl")
			m_output->setFontLineSpacing(value);
		else if (controlWord == "tx")
		{
			m_output->addTabStop(value, m_tabType);
			m_tabType = 0;
		}
		else if (controlWord == "tqr")
			m_tabType = 1;
		else if (controlWord == "tqc")
			m_tabType = 4;
		else if (controlWord == "tqdec")
			m_tabType = 3;
		else if (controlWord == "ltrch")
			m_output->setTextDirectionLeftToRight();
		else if (controlWord == "rtlch")
			m_output->setTextDirectionRightToLeft();
		else if (controlWord == "plain")
			m_output->resetCharacterProperties();
		else if (controlWord == "paperh")
			m_output->setPageHeight(value);
		else if (controlWord == "paperw")
			m_output->setPageWidth(value);
		else if ((controlWord == "u") && hasValue)
		{
			m_output->appendUnicodeText(QChar(value));
			m_charactersToSkip = m_uc_Value; /* TODO: this should be driven by \uc, default to 1 */
		}
		else if ((controlWord == "uc") && hasValue)
			m_uc_Value = value;
		else if ((controlWord == "s") && hasValue)
			m_output->useStyleSheetTableEntry(value);
	/*	else if ((controlWord == "dropcapli") && hasValue)
			m_output->setDropCapsLines(value);
		else if (controlWord == "dropcapt")
			m_output->setDropCaps();
		else if (controlWord == "keepn")
			m_output->keepWithNext(); */
		else if ((controlWord == "\'") && hasValue)
		{
	//	qDebug() << "special character value:" << value;
		}
		else if (controlWord == "*")
		{
	    // handled elsewhere
		}
		else
		{
			if (ControlWord::isDestination(controlWord))
			{
			//	qDebug() << "unhandled *Destination* control word in DocumentDestination:" << controlWord;
			}
			else
			{
			//	qDebug() << "unhandled control word in DocumentDestination:" << controlWord;
			}
		}
	}

	void DocumentDestination::handlePlainText(const QByteArray &plainText)
	{
		if (m_charactersToSkip > 0)
		{
		//    qDebug() << "skipping" << m_charactersToSkip << "of" << plainText;
			if (m_charactersToSkip >= plainText.size())
			{
				m_charactersToSkip -= plainText.size();
				return;
			}
			else if (plainText.size() > m_charactersToSkip)
			{
				QByteArray partiallySkippedText(plainText);
				partiallySkippedText.remove(0, m_charactersToSkip);
				m_output->appendText( partiallySkippedText );
				m_charactersToSkip = 0;
				return;
			}
		}
		m_output->appendText(plainText);
	}

	void DocumentDestination::aboutToEndDestination()
	{
	// TODO
	}
}
