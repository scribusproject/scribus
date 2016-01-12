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

#include <QTextCodec>
#include "rtfreader.h"
#include "controlword.h"
#include "commonstrings.h"

namespace RtfReader
{

	StyleSheetDestination::StyleSheetDestination(Reader *reader, AbstractRtfOutput *output, const QString &name) : Destination(reader, output, name), m_uc_Value(1)
	{
		m_currentStyleHandleNumber = 0; /* default */
		m_textStyle.setParent(CommonStrings::DefaultParagraphStyle);
		m_textStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		m_textStyle.charStyle().setFontVariant("");
		m_textStyle.charStyle().setFontSize(120.0);
		QList<ParagraphStyle::TabRecord> tbs;
		tbs.clear();
		m_textStyle.setTabValues(tbs);
		m_styleName = "";
		m_currentStyleParent = -1;
	}

	StyleSheetDestination::~StyleSheetDestination()
	{
	}

	void StyleSheetDestination::handleControlWord(const QString &controlWord, bool hasValue, const int value)
	{
		if (controlWord == "ql")
			m_textStyle.setAlignment(ParagraphStyle::Leftaligned);
		else if (controlWord == "qj")
			m_textStyle.setAlignment(ParagraphStyle::Justified);
		else if (controlWord == "qc")
			m_textStyle.setAlignment(ParagraphStyle::Centered);
		else if (controlWord == "qr")
			m_textStyle.setAlignment(ParagraphStyle::Rightaligned);
		else if ((controlWord == "li") && hasValue)
			m_textStyle.setLeftMargin(pixelsFromTwips(value));
		else if ((controlWord == "ri") && hasValue)
			m_textStyle.setRightMargin(pixelsFromTwips(value));
		else if ((controlWord == "sb") && hasValue)
			m_textStyle.setGapBefore(pixelsFromTwips(value));
		else if (controlWord == "sb")
			m_textStyle.setGapBefore(pixelsFromTwips(0));
		else if ((controlWord == "sa") && hasValue)
			m_textStyle.setGapAfter(pixelsFromTwips(value));
		else if (controlWord == "sa")
			m_textStyle.setGapAfter(pixelsFromTwips(0));
		else if (controlWord == "sl")
		{
			if (value == 0)
				m_textStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
			else
			{
				m_textStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
				m_textStyle.setLineSpacing(pixelsFromTwips(qAbs(value)));
			}
		}
		else if (controlWord == "fs")
		{
			if (hasValue && ( value != 0))
				m_textStyle.charStyle().setFontSize(value * 5.0);
			else
				m_textStyle.charStyle().setFontSize(120.0);
		}
		else if (controlWord == "f")
			m_textStyle.charStyle().setFontVariant(QString("%1").arg(value));
		else if (controlWord == "ul")
		{
			StyleFlag styleEffects = m_textStyle.charStyle().effects();
			if (!hasValue || (hasValue && value != 0))
				styleEffects |= ScStyle_Underline;
			else
				styleEffects &= ~ScStyle_Underline;
			m_textStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (controlWord == "ulw")
		{
			StyleFlag styleEffects = m_textStyle.charStyle().effects();
			if (!hasValue || (hasValue && value != 0))
				styleEffects |= ScStyle_UnderlineWords;
			else
				styleEffects &= ~ScStyle_UnderlineWords;
			m_textStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (controlWord == "outl")
		{
			StyleFlag styleEffects = m_textStyle.charStyle().effects();
			if (!hasValue || (hasValue && value != 0))
				styleEffects |= ScStyle_Outline;
			else
				styleEffects &= ~ScStyle_Outline;
			m_textStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (controlWord == "shad")
		{
			StyleFlag styleEffects = m_textStyle.charStyle().effects();
			if (!hasValue || (hasValue && value != 0))
				styleEffects |= ScStyle_Shadowed;
			else
				styleEffects &= ~ScStyle_Shadowed;
			m_textStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (controlWord == "scaps")
		{
			StyleFlag styleEffects = m_textStyle.charStyle().effects();
			if (!hasValue || (hasValue && value != 0))
				styleEffects |= ScStyle_SmallCaps;
			else
				styleEffects &= ~ScStyle_SmallCaps;
			m_textStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (controlWord == "caps")
		{
			StyleFlag styleEffects = m_textStyle.charStyle().effects();
			if (!hasValue || (hasValue && value != 0))
				styleEffects |= ScStyle_AllCaps;
			else
				styleEffects &= ~ScStyle_AllCaps;
			m_textStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (controlWord == "strike")
		{
			StyleFlag styleEffects = m_textStyle.charStyle().effects();
			if (!hasValue || (hasValue && value != 0))
				styleEffects |= ScStyle_Strikethrough;
			else
				styleEffects &= ~ScStyle_Strikethrough;
			m_textStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (controlWord == "super")
		{
			StyleFlag styleEffects = m_textStyle.charStyle().effects();
			styleEffects |= ScStyle_Superscript;
			m_textStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (controlWord == "sub")
		{
			StyleFlag styleEffects = m_textStyle.charStyle().effects();
			styleEffects |= ScStyle_Subscript;
			m_textStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (controlWord == "charscalex")
			m_textStyle.charStyle().setScaleH(value * 10.0);
		else if (controlWord == "dn")
			m_textStyle.charStyle().setBaselineOffset((-value * 10000 / 2) / m_textStyle.charStyle().fontSize());
		else if (controlWord == "up")
			m_textStyle.charStyle().setBaselineOffset((value * 10000 / 2) / m_textStyle.charStyle().fontSize());
		else if (controlWord == "expnd")
			m_textStyle.charStyle().setTracking((value * 10000 / 4) / m_textStyle.charStyle().fontSize());
		else if (controlWord == "expndtw")
			m_textStyle.charStyle().setTracking((pixelsFromTwips(value) * 10000) / m_textStyle.charStyle().fontSize());
		else if (controlWord == "sbasedon")
			m_currentStyleParent = value;
		else if ((controlWord == "s") && hasValue)
			m_currentStyleHandleNumber = value;
		else if ((controlWord == "u") && hasValue)
		{
			m_styleName.append(m_output->getCurrentCodec()->fromUnicode(QChar(value)));
			m_charactersToSkip = m_uc_Value; /* TODO: this should be driven by \uc, default to 1 */
		}
		else if ((controlWord == "uc") && hasValue)
			m_uc_Value = value;
	//	else
	//		qDebug() << "unhandled control word in StyleSheetDestination:" << controlWord;
	}

	void StyleSheetDestination::handlePlainText(const QByteArray &plainText)
	{
		QByteArray pT = plainText;
		if (m_charactersToSkip > 0)
		{
		//	qDebug() << "skipping" << m_charactersToSkip << "of" << plainText;
			if (m_charactersToSkip >= plainText.size())
			{
				m_charactersToSkip -= plainText.size();
				return;
			}
			else if (plainText.size() > m_charactersToSkip)
			{
				pT.remove(0, m_charactersToSkip);
				m_charactersToSkip = 0;
			}
		}
		if (pT == ";")
		{
			m_stylesTable.insert(m_currentStyleHandleNumber, m_currentStyleParent);
			m_output->insertStyleSheetTableEntry(m_currentStyleHandleNumber, m_textStyle);
			m_textStyle.erase();
			m_textStyle.setParent(CommonStrings::DefaultParagraphStyle);
			m_textStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
			m_textStyle.charStyle().setFontVariant("");
			m_textStyle.charStyle().setFontSize(120.0);
			QList<ParagraphStyle::TabRecord> tbs;
			tbs.clear();
			m_textStyle.setTabValues(tbs);
			m_styleName = "";
			m_currentStyleParent = -1;
		}
		else if (pT.endsWith(";"))
		{
			// probably a style name with a terminating delimiter
			int delimiterPosition = pT.indexOf(";");
			if (delimiterPosition == (pT.length() - 1))
			{
				// It is at the end, chop it off
				QByteArray styleName = pT.left(delimiterPosition);
				m_styleName.append(styleName);
				m_textStyle.setName(m_output->getCurrentCodec()->toUnicode(m_styleName));
				m_stylesTable.insert(m_currentStyleHandleNumber, m_currentStyleParent);
				m_output->insertStyleSheetTableEntry(m_currentStyleHandleNumber, m_textStyle);
				m_textStyle.erase();
				m_textStyle.setParent(CommonStrings::DefaultParagraphStyle);
				m_textStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
				m_textStyle.charStyle().setFontVariant("");
				m_textStyle.charStyle().setFontSize(120.0);
				QList<ParagraphStyle::TabRecord> tbs;
				tbs.clear();
				m_textStyle.setTabValues(tbs);
				m_styleName = "";
				m_currentStyleParent = -1;
			}
			else
			{
			// we were not expecting a name with a delimiter other than at the end
			//qDebug() << "Style name with embedded delimiter: " << plainText;
			}
		}
		else
			m_styleName.append(pT);
	}

	void StyleSheetDestination::aboutToEndDestination()
	{
		m_output->resolveStyleSheetParents(m_stylesTable);
	}

	double StyleSheetDestination::pixelsFromTwips(const int twips)
	{
		return twips / 1440.0 * 72.0;
	}
}
