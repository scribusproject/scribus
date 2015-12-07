/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2009 by Jain Basil Aliyas                               *
*   mail@jainbasil.net                                                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#include "xtgscanner.h"

#include <QString>
#include <QList>
#include <QApplication>
#include <QDebug>

#include "fonts/scface.h"
#include "scribusdoc.h"
#include "text/specialchars.h"
#include "prefsmanager.h"
#include "ui/missing.h"
#include "util.h"


XtgScanner::XtgScanner (QString filename, PageItem *item, bool textOnly)
{
	m_item = item;
	m_importTextOnly = textOnly;
	loadRawBytes(filename, m_input_Buffer);
	m_top = 0;
	if ((m_input_Buffer[0] == '\xFF') && (m_input_Buffer[1] == '\xFE'))
	{
		QByteArray tmpBuf;
		for (int a = 2; a < m_input_Buffer.count(); a += 2)
		{
			tmpBuf.append(m_input_Buffer[a]);
		}
		m_input_Buffer = tmpBuf;
	}
	m_doc = item->doc();
	initTagMode();
	initTextMode();
	initNameMode();
	initLanguages();
	m_prevMode = textMode;
	m_textToAppend = "";
	m_define = 0;
	m_styleEffects = ScStyle_None;
	m_codecList = QTextCodec::availableCodecs();
	if (m_codecList.contains("cp1252"))
		m_codec = QTextCodec::codecForName("cp1252");		// Default ANSI codec
	else
		m_codec = QTextCodec::codecForLocale();
	m_isBold = false;
	m_isItalic = false;
	m_inDef = false;
}

/** Initialise a QHash which maps the values of n## to corresponding language strings
*/

void XtgScanner::initLanguages()
{
	/* For more info, look at langmgr.cpp */
	m_languages.insert(72,"Bulgarian");
	m_languages.insert(73,"Catalan");
	m_languages.insert(52,"Chinese");
	m_languages.insert(53,"Chinese");
	m_languages.insert(68,"Croatian");
	m_languages.insert(56,"Czech");
	m_languages.insert(9,"Danish");
	m_languages.insert(5,"Dutch");
	m_languages.insert(2,"English");
	m_languages.insert(0,"English");
	m_languages.insert(17,"Finnish");
	m_languages.insert(1,"French");
	m_languages.insert(3,"German");
	m_languages.insert(70,"German");
	m_languages.insert(19,"German");
	m_languages.insert(20,"Greek");
	m_languages.insert(43,"Hungarian");
	m_languages.insert(21,"Icelandic");
	m_languages.insert(4,"Italian");
	m_languages.insert(14,"Japanese");
	m_languages.insert(41,"Lithuanian");
	m_languages.insert(12,"Norwegian (Bokmål)");
	m_languages.insert(80,"Norwegian (Bokmål)");
	m_languages.insert(42,"Polish");
	m_languages.insert(71,"Portuguese (BR)");
	m_languages.insert(10,"Portuguese");
	m_languages.insert(39,"Romanian");
	m_languages.insert(57,"Slovak");
	m_languages.insert(66,"Slovenian");
	m_languages.insert(8,"Spanish");
	m_languages.insert(7,"Swedish");
	m_languages.insert(62,"Ukranian");

}

/** Initialise the tagModeHash with the tokens returned in tagMode and its corresponding function pointers */

void XtgScanner::initTagMode()
{
	m_tagModeHash.insert("P",&XtgScanner::setPlain);
	m_tagModeHash.insert("B",&XtgScanner::setBold);
	m_tagModeHash.insert("I",&XtgScanner::setItalics);
	m_tagModeHash.insert("O",&XtgScanner::setOutline);
	m_tagModeHash.insert("S",&XtgScanner::setShadow);
	m_tagModeHash.insert("U",&XtgScanner::setUnderline);
	m_tagModeHash.insert("W",&XtgScanner::setWordUnder);
	m_tagModeHash.insert("/",&XtgScanner::setStrikethrough);
	m_tagModeHash.insert("R",&XtgScanner::setDoubleStrike);
	m_tagModeHash.insert("K",&XtgScanner::setAllCaps);
	m_tagModeHash.insert("H",&XtgScanner::setSmallCaps);
	m_tagModeHash.insert("+",&XtgScanner::setSuperscript);
	m_tagModeHash.insert("-",&XtgScanner::setSubscript);
	m_tagModeHash.insert("V",&XtgScanner::setSuperior);
	m_tagModeHash.insert("f",&XtgScanner::setFont);
	m_tagModeHash.insert("z",&XtgScanner::setFontSize);
	m_tagModeHash.insert("c",&XtgScanner::setColor);
	m_tagModeHash.insert("s",&XtgScanner::setShade);
	m_tagModeHash.insert("h",&XtgScanner::setHorizontalScale);
	m_tagModeHash.insert("k",&XtgScanner::setKern);
	m_tagModeHash.insert("t",&XtgScanner::setTrack);
	m_tagModeHash.insert("b",&XtgScanner::setBaseLineShift);
	m_tagModeHash.insert("y",&XtgScanner::setVerticalScale);
	m_tagModeHash.insert("G",&XtgScanner::setLigatures);
	m_tagModeHash.insert("p",&XtgScanner::csetOpacity);
	m_tagModeHash.insert("o",&XtgScanner::setOpenType);
	m_tagModeHash.insert("*A",&XtgScanner::setCharAlignment);
	m_tagModeHash.insert("L",&XtgScanner::setVertStories);
	m_tagModeHash.insert("M",&XtgScanner::setEmphasisMarks);
	m_tagModeHash.insert("Y",&XtgScanner::setAsncjk);//Apply sending to non-CJK characters.
	m_tagModeHash.insert("n",&XtgScanner::setLanguages);
	m_tagModeHash.insert("A(",&XtgScanner::setXPresOwn); //Apply features like Rubi text, group characters & hyperlink
	m_tagModeHash.insert("*L",&XtgScanner::setAlignLeft);
	m_tagModeHash.insert("*C",&XtgScanner::setAlignCenter);
	m_tagModeHash.insert("*R",&XtgScanner::setAlignRight);
	m_tagModeHash.insert("*J",&XtgScanner::setAlignJustify);
	m_tagModeHash.insert("*F",&XtgScanner::setAlignFJustify);
	m_tagModeHash.insert("*t",&XtgScanner::setTabStops);
	m_tagModeHash.insert("*p",&XtgScanner::setPAttributes);
	m_tagModeHash.insert("*h",&XtgScanner::setHyphenation);
	m_tagModeHash.insert("*ra",&XtgScanner::setPRuleAbove);
	m_tagModeHash.insert("*rb",&XtgScanner::setPRuleBelow);
	m_tagModeHash.insert("*d",&XtgScanner::setDropCap);
	m_tagModeHash.insert("*kn",&XtgScanner::setKeepNextPar);
	m_tagModeHash.insert("*kt",&XtgScanner::setKeepTogether);
	m_tagModeHash.insert("*s",&XtgScanner::setHangingCSet);
	m_tagModeHash.insert("DoO",&XtgScanner::setGlyph);
	m_tagModeHash.insert("EX",&XtgScanner::setTranscodeSeq);
	m_tagModeHash.insert("DO",&XtgScanner::setUnencodedGlyph);

	m_tagModeHash.insert("@>",&XtgScanner::applyCStyle1);
	m_tagModeHash.insert("@$",&XtgScanner::applyCStyle2);
	m_tagModeHash.insert("@$p",&XtgScanner::applyCStyle3);
	m_tagModeHash.insert("$$",&XtgScanner::applyCStyle4);
	m_tagModeHash.insert("a$",&XtgScanner::applyCStyle5);
	m_tagModeHash.insert("a$$",&XtgScanner::applyCStyle6);

	m_tagModeHash.insert("$",&XtgScanner::setTypeStyle);

	m_tagModeHash.insert("\\@",&XtgScanner::appendSpChar1);
	m_tagModeHash.insert("x",&XtgScanner::xFlag);
	m_tagModeHash.insert("\\<",&XtgScanner::appendSpChar2);
	m_tagModeHash.insert("\\",&XtgScanner::appendSpChar3);
	m_tagModeHash.insert(">",&XtgScanner::defClose);
	m_tagModeHash.insert("e",&XtgScanner::setEncoding);
}

/** Initialise the textModeHash with tokens returned in textMode and its corresponding function pointers */

void XtgScanner::initTextMode()
{
	m_textModeHash.insert("\n",&XtgScanner::defNewLine);
	m_textModeHash.insert("<",&XtgScanner::defOpen);
	m_textModeHash.insert("@",&XtgScanner::defAtRate);
	m_textModeHash.insert("\r",&XtgScanner::defHardReturn);
}

/** Initialise the nameModeHash with tokens returned in nameMode and its corresponding function pointers */

void XtgScanner::initNameMode()
{
	m_nameModeHash.insert("[F]",&XtgScanner::defFontSet);
	m_nameModeHash.insert("[C]",&XtgScanner::defColor);
	m_nameModeHash.insert("[S\"",&XtgScanner::definePStyles);
	m_nameModeHash.insert("[Sp",&XtgScanner::definePStyles);
	m_nameModeHash.insert("[St",&XtgScanner::defineCStyle);
	m_nameModeHash.insert("=",&XtgScanner::defEquals);
	m_nameModeHash.insert(":",&XtgScanner::defColon);
}

/** Function for each tokens in tagMode. Each token is hashed with corresponding function pointers
*/

void XtgScanner::setPlain()
{
	m_styleEffects = ScStyle_None;
	m_currentCharStyle.setFeatures(m_styleEffects.featureList());
	m_isBold = true;
	m_isItalic = false;
	setBold();
}


void XtgScanner::setBold()
{
	m_isBold = !m_isBold;
//	QString fam = currentCharStyle.font().family();
	int posC = m_item->itemText.length();
	m_item->itemText.insertChars(posC, "B");
	m_item->itemText.applyStyle(posC, m_currentParagraphStyle);
	m_item->itemText.applyCharStyle(posC, 1, m_currentCharStyle);
	QString fam = m_item->itemText.charStyle(posC).font().family();
	m_item->itemText.removeChars(posC, 1);
	if (fam.isEmpty())
		return;
	QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[fam];
	if (m_isBold)
	{
		if (m_isItalic)
		{
			if (slist.contains("Bold Italic"))
				m_currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold Italic"]);
		}
		else if (slist.contains("Bold"))
			m_currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold"]);
	}
	else
	{
		if (m_isItalic)
		{
			if (slist.contains("Italic"))
				m_currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Italic"]);
		}
		else if (slist.contains("Regular"))
			m_currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Regular"]);
	}
}

void XtgScanner::setItalics()
{
	m_isItalic = !m_isItalic;
	int posC = m_item->itemText.length();
	m_item->itemText.insertChars(posC, "B");
	m_item->itemText.applyStyle(posC, m_currentParagraphStyle);
	m_item->itemText.applyCharStyle(posC, 1, m_currentCharStyle);
	QString fam = m_item->itemText.charStyle(posC).font().family();
	m_item->itemText.removeChars(posC, 1);
//	QString fam = currentCharStyle.font().family();
	if (fam.isEmpty())
		return;
	QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[fam];
	if (m_isItalic)
	{
		if (m_isBold)
		{
			if (slist.contains("Bold Italic"))
				m_currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold Italic"]);
		}
		else if (slist.contains("Italic"))
			m_currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Italic"]);
	}
	else
	{
		if (m_isBold)
		{
			if (slist.contains("Bold"))
				m_currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold"]);
		}
		else if (slist.contains("Regular"))
			m_currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Regular"]);
	}
}

void XtgScanner::setShadow()
{
	applyFeature(ScStyle_Shadowed);
}

void XtgScanner::setOutline()
{
	applyFeature(ScStyle_Outline);
}

void XtgScanner::setUnderline()
{
	m_styleEffects &= ~ScStyle_UnderlineWords;
	applyFeature(ScStyle_Underline);
}

void XtgScanner::setWordUnder()
{
	m_styleEffects &= ~ScStyle_Underline;
	applyFeature(ScStyle_UnderlineWords);
}

void XtgScanner::setStrikethrough()
{
	applyFeature(ScStyle_Strikethrough);
}

void XtgScanner::setDoubleStrike()
{
	m_styleEffects &= ~ScStyle_Strikethrough;
	m_currentCharStyle.setFeatures(m_styleEffects.featureList());
	m_unSupported.insert(m_token);
}

void XtgScanner::setAllCaps()
{
	m_styleEffects &= ~ScStyle_SmallCaps;
	applyFeature(ScStyle_AllCaps);
}

void XtgScanner::setSmallCaps()
{
	m_styleEffects &= ~ScStyle_AllCaps;
	applyFeature(ScStyle_SmallCaps);
}

void XtgScanner::setSuperscript()
{
	m_styleEffects &= ~ScStyle_Subscript;
	applyFeature(ScStyle_Superscript);
}

void XtgScanner::setSubscript()
{
	m_styleEffects &= ~ScStyle_Superscript;
	applyFeature(ScStyle_Subscript);
}

void XtgScanner::setSuperior()
{
	m_styleEffects &= ~ScStyle_Subscript;
	applyFeature(ScStyle_Superscript);
}

void XtgScanner::setFont()
{
	/** define/apply font */
	flushText();
	m_token = getToken();
	QString font = PrefsManager::instance()->appPrefs.itemToolPrefs.textFont;
	if (m_token != "$")
		font = getFontName(m_token);
	m_currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[font]);
	if(!m_doc->UsedFonts.contains(font))
		m_doc->AddFont(font);
}

QString XtgScanner::getFontName(QString name)
{
	QString fontName = name;
	bool found = false;
	SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (it.current().family().toLower() == fontName.toLower())
		{
			if (it.currentKey().toLower() == fontName.toLower()) // exact Match
			{
				return fontName;
			}
			else
			{
				QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[it.current().family()];
				slist.sort();
				if (slist.count() > 0)
				{
					int reInd = slist.indexOf("Regular");
					if (reInd < 0)
						fontName = it.current().family() + " " + slist[0];
					else
						fontName = it.current().family() + " " + slist[reInd];
					return fontName;
				}
			}
		}
	}
	if (found)
		return fontName;
	else
	{
		if (!PrefsManager::instance()->appPrefs.fontPrefs.GFontSub.contains(fontName))
		{
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			MissingFont *dia = new MissingFont(0, fontName, m_doc);
			dia->exec();
			QString fontNameR = dia->getReplacementFont();
			delete dia;
			qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
			PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[fontName] = fontNameR;
			fontName = fontNameR;
		}
		else
			fontName = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[fontName];
	}
	return fontName;
}

void XtgScanner::setFontSize()
{
	flushText();
	m_token = getToken();
	if (m_token == "$")
		m_token = "12";
	m_currentCharStyle.setFontSize( m_token.toDouble() * 10 );
}

void XtgScanner::setColor()
{
	flushText();
	m_token = getToken();	

	QHash<QString,QString> color;
	color.insert("cC","Cyan");
	color.insert("cM","Magenta");
	color.insert("cY","Yellow");
	color.insert("cK","Black");

	if (m_token == "C" || m_token == "M" || m_token == "Y" || m_token == "K")
	{
		m_token = "c" + m_token; 
		m_token = color.value(m_token);
	}
	else if (!m_doc->PageColors.contains(m_token))
		m_token = "Black";
	if (m_doc->PageColors.contains(m_token))
		m_currentCharStyle.setFillColor(m_token);
}

void XtgScanner::setShade()
{
	flushText();
	m_token = getToken();
	if (m_token == "$")
		m_token = "100";
	m_currentCharStyle.setFillShade(m_token.toDouble());

}

void XtgScanner::setHorizontalScale()
{
	flushText();
	m_token = getToken();
	if (m_token == "$")
		m_token = "100";
	m_currentCharStyle.setScaleH(m_token.toDouble() * 10.0);
}

void XtgScanner::setKern()
{
	flushText();
	m_token = getToken();
	if (m_token == "$")
		m_token = "0";
//	currentCharStyle.setWordTracking((token.toDouble() * (currentCharStyle.fontSize() / 200.0)) / currentCharStyle.fontSize() * 10);
}

void XtgScanner::setTrack()
{
	flushText();
	m_token = getToken();
	if (m_token == "$")
		m_token = "0";
//	currentCharStyle.setTracking((token.toDouble() * (currentCharStyle.fontSize() / 200.0)) / currentCharStyle.fontSize() * 10);
}

void XtgScanner::setBaseLineShift()
{
	flushText();
	m_token = getToken();
	if (m_token == "$")
		m_token = "0";
	m_currentCharStyle.setBaselineOffset((m_token.toDouble() * 10000) / m_currentCharStyle.fontSize());
}

void XtgScanner::setVerticalScale()
{
	flushText();
	m_token = getToken();
	if (m_token == "$")
		m_token = "100";
//	currentCharStyle.setScaleV(token.toDouble() * 10.0);
}

void XtgScanner::setLigatures()
{
	m_token = getToken();
	m_unSupported.insert("G"+m_token);
}

void XtgScanner::csetOpacity()
{
	m_token = getToken();
	m_unSupported.insert("p"+m_token);
}

void XtgScanner::setOpenType()
{
	/* OpenType can be like this : o("xxxx","yyyy","zzzz") */
}

void XtgScanner::setCharAlignment()
{
	m_token.append( nextSymbol() );
	m_unSupported.insert(m_token);
}

void XtgScanner::setVertStories()
{
	m_token.append( nextSymbol() );
	m_unSupported.insert(m_token);
}

void XtgScanner::setEmphasisMarks()
{
	m_token.append( nextSymbol() );
	m_unSupported.insert(m_token);
}

void XtgScanner::setAsncjk()
{
	//Apply sending to non-CJK characters.
	m_token.append( nextSymbol() );
	m_unSupported.insert(m_token);
}

void XtgScanner::setLanguages()
{
	flushText();
	m_token = getToken();
/*	if (define == 0)
	{
		flushText();
		currentCharStyle.setLanguage( languages.value( token.toInt() ) );
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 1)
		defCharStyle.setLanguage( languages.value( token.toInt() ) );
	if (define == 2)
		defParagraphStyle.charStyle().setLanguage( languages.value( token.toInt() ) );	*/
}

void XtgScanner::setXPresOwn()
{

	m_unSupported.insert(m_token+')');
	//All these tags are unsupported in Scribus, so just ignoring them till we find close paranthesis
	while (lookAhead() != ')')
		m_top=m_top+1;
	m_top=m_top+1;//Ignore )
}

void XtgScanner::setAlignLeft()
{
	flushText();
	m_currentParagraphStyle.setAlignment(ParagraphStyle::Leftaligned);
}

void XtgScanner::setAlignCenter()
{
	flushText();
	m_currentParagraphStyle.setAlignment(ParagraphStyle::Centered);
}

void XtgScanner::setAlignRight()
{
	flushText();
	m_currentParagraphStyle.setAlignment(ParagraphStyle::Rightaligned);
}

void XtgScanner::setAlignJustify()
{
	flushText();
	m_currentParagraphStyle.setAlignment(ParagraphStyle::Justified);
}

void XtgScanner::setAlignFJustify()
{
	flushText();
	m_currentParagraphStyle.setAlignment(ParagraphStyle::Extended);
}

void XtgScanner::setTabStops()
{
	QList<ParagraphStyle::TabRecord> tbs = m_currentParagraphStyle.tabValues();
	tbs.clear();
	if (lookAhead() == '0')
	{
		m_currentParagraphStyle.setTabValues(tbs);
		return;
	}
	while (lookAhead() != ')')
	{
		m_token = getToken();
		double pos = m_token.toDouble();
		m_token = getToken();
		int typ = m_token.toInt();
		m_token = getToken();
		ParagraphStyle::TabRecord tb;
		if (typ == 0)
			tb.tabType = 0;
		else if (typ == 2)
			tb.tabType = 1;
		else if (typ == 1)
			tb.tabType = 4;
		else if (typ == 4)
			tb.tabType = 3;
		tb.tabType = typ;
		tb.tabPosition = pos;
		tb.tabFillChar = QChar();
		tbs.append(tb);
	}
	m_currentParagraphStyle.setTabValues(tbs);
	
}

void XtgScanner::setPAttributes()
{
	double leftIndent = getToken().toDouble();
	double firstlineIndent = getToken().toDouble();
	double rightIndent = getToken().toDouble();
	double leading = getToken().toDouble();
	double gapBefore = getToken().toDouble();
	double gapAfter = getToken().toDouble();
	m_top++;
	m_token="";
	m_token.append( nextSymbol() );
	if ((m_token == "g") || (m_token == "G"))
	{
		// We have to discard (P,S)) since it is not yet supported in Scribus

		while (lookAhead() != ')' )
			m_top++;
	}
	m_currentParagraphStyle.setLeftMargin(leftIndent);
	m_currentParagraphStyle.setRightMargin(rightIndent);
	m_currentParagraphStyle.setFirstIndent(firstlineIndent);
	m_currentParagraphStyle.setLineSpacing(leading);
	m_currentParagraphStyle.setGapBefore(gapBefore);
	m_currentParagraphStyle.setGapAfter(gapAfter);
	if (m_token == "G")
		m_currentParagraphStyle.setUseBaselineGrid(true);
	if (m_token == "g")
		m_currentParagraphStyle.setUseBaselineGrid(false);
}

void XtgScanner::setHyphenation()
{
	m_unSupported.insert(m_token);
	m_token=getToken();
}

void XtgScanner::setPRuleAbove()
{
	m_unSupported.insert(m_token);
	if (lookAhead() == '0')
	{
		m_top++;
		return;
	}
	while (lookAhead() != ')')
		m_top++;
}

void XtgScanner::setPRuleBelow()
{
	m_unSupported.insert(m_token);
	if (lookAhead() == '0')
	{
		m_top++;
		return;
	}
	while (lookAhead() != ')')
		m_top++;
}

void XtgScanner::setDropCap()
{
	flushText();
	int charCount = getToken().toInt();
	if (charCount == 0) //Specify No rule
	{
		m_currentParagraphStyle.setHasDropCap(false);
	}
	else
	{
		int lineCount = getToken().toInt();
		m_currentParagraphStyle.setHasDropCap(true);
		m_currentParagraphStyle.setDropCapLines(lineCount);
	}
}

void XtgScanner::setKeepNextPar()
{
	m_token.append(nextSymbol());
	m_unSupported.insert(m_token);
}

void XtgScanner::setKeepTogether()
{
	if (lookAhead() == '(')
	{
		while (lookAhead() != ')')
		{
			m_token.append( nextSymbol() );
		}
	}
	m_token.append(nextSymbol());
	m_unSupported.insert(m_token);
}

void XtgScanner::setHangingCSet()
{
	m_token.append( getToken() );
	m_unSupported.insert(m_token);
}

void XtgScanner::setGlyph()
{
}

void XtgScanner::setTranscodeSeq()
{
}

void XtgScanner::setUnencodedGlyph()
{
}

void XtgScanner::appendSpChar1()
{
	m_textToAppend.append("@");
}

void XtgScanner::appendSpChar2()
{
	m_textToAppend.append("<");
}

void XtgScanner::appendSpChar3()
{
	m_textToAppend.append("\\");
}

void XtgScanner::xFlag()
{
	m_xflag = true;
}

void XtgScanner::applyCStyle1()
{
	//apply nostyle character sheet <@>
	m_define = 0;
	flushText();
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setParent(pStyle);
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	m_currentCharStyle = newStyle.charStyle();
	m_currentCharStyle.setFontSize(120.0);
	m_styleEffects = ScStyle_None;
	m_currentCharStyle.setFeatures(m_styleEffects.featureList());
}

void XtgScanner::applyCStyle2()
{
	//apply normal character style sheet <@$>
	m_define = 0;
	flushText();
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setParent(pStyle);
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	m_currentCharStyle = newStyle.charStyle();
	m_currentCharStyle.setFontSize(120.0);
	m_styleEffects = ScStyle_None;
	m_currentCharStyle.setFeatures(m_styleEffects.featureList());
}

void XtgScanner::applyCStyle3()
{
	//apply paragraph's character stylesheet <@$p>
	m_define = 0;
	flushText();
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setParent(pStyle);
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	m_currentCharStyle = newStyle.charStyle();
	m_currentCharStyle.setFontSize(120.0);
	m_styleEffects = ScStyle_None;
	m_currentCharStyle.setFeatures(m_styleEffects.featureList());
//	currentCharStyle = currentParagraphStyle.charStyle();
//	writer->setCharStyle(currentCharStyle);
//	currentCharStyle = writer->getCurrentCharStyle();
}

void XtgScanner::applyCStyle4()
{

}

void XtgScanner::applyCStyle5()
{
	//tag = a$
}

void XtgScanner::applyCStyle6()
{
	//tag = a$$
}

void XtgScanner::setTypeStyle()
{
	//tag = $
	flushText();
	m_currentCharStyle = m_currentParagraphStyle.charStyle();
	m_currentCharStyle.setFontSize(120.0);
	m_styleEffects = ScStyle_None;
	m_currentCharStyle.setFeatures(m_styleEffects.featureList());
}

void XtgScanner::setEncoding()
{
	m_token = getToken();
	int enc = m_token.toInt();
	QByteArray encTest = "cp1252";
	if (enc == 0)
		encTest = "macroman";
	else if (enc == 1)
		encTest = "cp1252";
	else if (enc == 2)
		encTest = "ISO-8859-1";
	else if (enc == 3)
		encTest = "windows-932";
	else if (enc == 6)
		encTest = "Big5";
	else if (enc == 7)
		encTest = "GB2312";
	else if (enc == 8)
		encTest = "UTF-8";
	else if (enc == 9)
		encTest = "UTF-8";
	else if (enc == 19)
		encTest = "windows-949";
	else if (enc == 20)
		encTest = "KSC_5601";
	if (m_codecList.contains(encTest))
		m_codec = QTextCodec::codecForName(encTest);
	else
		m_codec = QTextCodec::codecForName("cp1252");
}

/** Functions corresponding to tokens in textMode
*/

void XtgScanner::defNewLine()
{
	flushText();
	if (m_inDef)
		m_newlineFlag = true;
	else
	{
		int posT = m_item->itemText.length();
		if (posT > 0)
		{
			m_item->itemText.insertChars(posT, SpecialChars::PARSEP);
			m_item->itemText.applyStyle(posT, m_currentParagraphStyle);
		}
		m_inDef = false;
	}
}

void XtgScanner::defHardReturn()
{
	/* I saw a strange nature in XPress Tags that every hardreturn is followed by a soft return, hence fixing the
		new line flag to true */
	flushText();
	if (lookAhead() == '\n')
		m_newlineFlag = true;
	else
	{
		int posT = m_item->itemText.length();
		if (posT > 0)
		{
			m_item->itemText.insertChars(posT, SpecialChars::PARSEP);
			m_item->itemText.applyStyle(posT, m_currentParagraphStyle);
		}
		m_inDef = false;
	}
}

void XtgScanner::defOpen()
{
	// token <
	enterState(tagMode);
}

void XtgScanner::defAtRate()
{
	enterState(nameMode);
	m_sfcName = getToken();
	if (m_sfcName == "@$:")
	{
		if (m_doc->paragraphStyles().contains(m_item->itemName() + "_Normal"))
		{
			ParagraphStyle newStyle;
			newStyle.setParent(m_item->itemName() + "_Normal");
			m_currentParagraphStyle = newStyle;
			m_currentCharStyle = newStyle.charStyle();
		}
		else if (m_doc->paragraphStyles().contains("Normal"))
		{
			ParagraphStyle newStyle;
			newStyle.setParent("Normal");
			m_currentParagraphStyle = newStyle;
			m_currentCharStyle = newStyle.charStyle();
		}
		enterState(previousState());
	}
	else if (m_sfcName == "@:")
	{
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		m_currentParagraphStyle = newStyle;
		m_currentCharStyle = newStyle.charStyle();
		m_currentCharStyle.setFontSize(120.0);
		m_styleEffects = ScStyle_None;
		m_currentCharStyle.setFeatures(m_styleEffects.featureList());
		enterState(textMode);
	}
	else if (m_doc->paragraphStyles().contains(m_item->itemName() + "_" + m_sfcName))
	{
		ParagraphStyle newStyle;
		newStyle.setParent(m_item->itemName() + "_" + m_sfcName);
		m_currentParagraphStyle = newStyle;
		m_currentCharStyle = newStyle.charStyle();
		if (lookAhead() == ':')
			m_top++;
		enterState(textMode);
	}
	else if (m_doc->paragraphStyles().contains(m_sfcName))
	{
		ParagraphStyle newStyle;
		newStyle.setParent(m_sfcName);
		m_currentParagraphStyle = newStyle;
		m_currentCharStyle = newStyle.charStyle();
		if (lookAhead() == ':')
			m_top++;
		enterState(textMode);
	}
}

/** Functions corresponding to tokens in nameMode
*/

void XtgScanner::defClose()	//Token >
{
	if (m_define == 1)
	{
		StyleSet<CharStyle>tmp;
		tmp.create(m_currentCharStyle);
		m_doc->redefineCharStyles(tmp, false);
		m_inDef = false;
	}
	else if (m_define == 2)
	{
		StyleSet<ParagraphStyle>tmp;
		tmp.create(m_currentParagraphStyle);
		m_doc->redefineStyles(tmp, false);
		m_inDef = false;
	}
	if (m_define != 0)
		m_define = 0;
	enterState(textMode);	
}

void XtgScanner::defEquals()	//Token =
{
	//define a character stylesheet with the name in sfcName if the next character is not [
	m_newlineFlag = false;
	if (lookAhead() != '[')
	{
		m_define = 1;
		m_currentCharStyle.setName(m_item->itemName() + "_" + m_sfcName);
		enterState(textMode);
	}
	else 
		m_define = 2;
	m_inDef = true;
}

void XtgScanner::defColon()
{
	flushText();
	if ((m_sfcName == "@") || (m_sfcName == "@$:") || (m_sfcName == "@:"))
	{
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		m_currentParagraphStyle = newStyle;
		m_currentCharStyle = newStyle.charStyle();
		m_currentCharStyle.setFontSize(120.0);
		m_styleEffects = ScStyle_None;
		m_currentCharStyle.setFeatures(m_styleEffects.featureList());
	}
	else if (m_doc->paragraphStyles().contains(m_item->itemName() + "_" + m_sfcName))
	{
		ParagraphStyle newStyle;
		newStyle.setParent(m_item->itemName() + "_" + m_sfcName);
		m_currentParagraphStyle = newStyle;
		m_currentCharStyle = newStyle.charStyle();
	}
	else if (m_doc->paragraphStyles().contains(m_sfcName))
	{
		ParagraphStyle newStyle;
		newStyle.setParent(m_sfcName);
		m_currentParagraphStyle = newStyle;
		m_currentCharStyle = newStyle.charStyle();
	}
	else
	{
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		m_currentParagraphStyle = newStyle;
		m_currentCharStyle = newStyle.charStyle();
		m_currentCharStyle.setFontSize(120.0);
		m_styleEffects = ScStyle_None;
		m_currentCharStyle.setFeatures(m_styleEffects.featureList());
	}
	if (m_newlineFlag)
	{
		int posT = m_item->itemText.length();
		if (posT > 0)
		{
			m_item->itemText.insertChars(posT, SpecialChars::PARSEP);
			m_item->itemText.applyStyle(posT, m_currentParagraphStyle);
		}
	}
	m_newlineFlag = false;
	enterState(textMode);
	m_define = 0;
	if (!((lookAhead() == '\r') || (lookAhead() == '\n')))
		m_inDef = false;
}

void XtgScanner::defFontSet()
{
	m_unSupported.insert("[F]");
	while (lookAhead() != '>' )
		m_top = m_top+1;
	m_top = m_top-1;
}

void XtgScanner::defColor()
{
	bool isSpot = false;
	ScColor tmp;
	enterState(stringMode);
	m_token = getToken();
	while (lookAhead() != '>' )
	{
		m_token = getToken();
		if ((m_token == "CMJN") || (m_token == "CMYK"))
		{
			enterState(tagMode);
			m_token = getToken();
			if (m_token == "S")
			{
				m_token = getToken();
				isSpot = true;
			}
			double c = getToken().toDouble();
			double m = getToken().toDouble();
			double y = getToken().toDouble();
			double k = getToken().toDouble();
			tmp.setColor(qRound(c * 2.55), qRound(m * 255), qRound(y * 255), qRound(k * 255));
			tmp.setSpotColor(isSpot);
			tmp.setRegistrationColor(false);
			m_doc->PageColors.tryAddColor(m_sfcName, tmp);
		}
		else if (m_token == "RGB")
		{
			enterState(tagMode);
			m_token = getToken();
			if (m_token == "S")
			{
				m_token = getToken();
				isSpot = true;
			}
			double r = getToken().toDouble();
			double g = getToken().toDouble();
			double b = getToken().toDouble();
			tmp.setColorRGB(qRound(r * 2.55), qRound(g * 255), qRound(b * 255));
			tmp.setSpotColor(isSpot);
			tmp.setRegistrationColor(false);
			m_doc->PageColors.tryAddColor(m_sfcName, tmp);
		}
	}
	m_top++;
	enterState(textMode);
}

void XtgScanner::definePStyles()
{
	QString s1,s2,s3;
	enterState(stringMode);
	m_define = 2;
	if (m_token == "[S\"")
		s1 = getToken();
	else
	{
		while (lookAhead() != '\"')
			m_top++;					//skip the inch
		m_top++;
		s1 = getToken();			//will contain the string 1
	}
//	top = top + 2;				//we have to skip comma and next inch character
	while (lookAhead() != '\"')
		m_top++;
	m_top++;
	s2 = getToken();
	if (lookAhead() != ']' )
	{
		while (lookAhead() != '\"')
			m_top++;
		m_top++;
	//	top = top + 2;
		s3 = getToken();
	}
	m_top++; // to ensure that ] is avoided
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	if (s1 != "")
	{
		if (m_doc->paragraphStyles().contains(m_item->itemName() + "_" + s1))
			newStyle.setParent(m_item->itemName() + "_" + s1);
		else if  (m_doc->paragraphStyles().contains(s1))
			newStyle.setParent(s1);
		else
			newStyle.setParent(pStyle);
	}
	else
		newStyle.setParent(pStyle);
	newStyle.setName(m_item->itemName() + "_" + m_sfcName);
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	if (s3 != "")
	{
		if (m_doc->charStyles().contains(m_item->itemName() + "_" + s3))
			newStyle.charStyle().setParent(m_item->itemName() + "_" + s3);
		else if  (m_doc->charStyles().contains(s3))
			newStyle.charStyle().setParent(s3);
		else
			newStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	}
	else
	{
		newStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
		newStyle.charStyle().setFontSize(120.0);
		m_styleEffects = ScStyle_None;
		newStyle.charStyle().setFeatures(m_styleEffects.featureList());
	}
	m_currentParagraphStyle = newStyle;
	m_currentCharStyle = newStyle.charStyle();
	enterState(textMode);
}

void XtgScanner::defineCStyle()
{
	//token [St
	QString s4;
	m_top = m_top+10;
	s4 = getToken();
/*	if (styleStatus(definedCStyles,s4))
		defCharStyle.setParent(s4);
	else
	{
		showWarning(s4);
		defCharStyle.setParent("Default Character Style");
	}*/
}

void XtgScanner::enterState ( scannerMode mode )
{
	m_prevMode = m_mode;
	m_mode = mode;
}

QString XtgScanner::sliceString()
{
	QString name;
	if (m_mode == stringMode)
	{
		while (lookAhead() != '\"')
		{
			name.append( nextSymbol() );
		}
		m_top = m_top+1;
	}
	if (m_mode == nameMode)
	{
		while (lookAhead() != '=' || lookAhead() != ':')
		{
			name.append( nextSymbol() );
		}
	}
	return name;
}

QString XtgScanner::getToken()
{
	QChar temp;
	QString token;
	temp = lookAhead();
	if (currentState() == textMode)
	{
		/**
		This mode should return only '<' or '@' or newline. It also adds the text_to_be_appended to the QString textToAppend
		*/
		if (temp =='<' || temp =='@' || temp == '\n' || temp == '\r')
			token.append( nextSymbol() );
		else if (temp == '\\')
		{
			/**
			An escape sequence is occurred, hence we will just append 
			the next character in buffer to the text. This will automatically
			deal the occurrance of '\@' or '\:' or any other relevant escapes
			*/
			m_top = m_top+1;
			m_textToAppend.append( nextSymbol() );
		}
		else
			m_textToAppend.append( nextSymbol() );
	}
	
	if (currentState() == tagMode)
	{
		/**
		This mode should return attributes B|I|U|O|....|*L|*C|... and an inch character "
		*/
		if (temp == '@')
		{
			token.append( nextSymbol() ); // this will append @
			if (lookAhead() == '$')
			{
				token.append( nextSymbol() ); //this will result in @$ if $ is found
				if (lookAhead() == 'p' )
					token.append( nextSymbol() ); //this will result in @$p if p is found
			}
			else // to get the name of character stylesheet applied as <@stylesheetname>
			{
				while (lookAhead() != '>' )
					token.append( nextSymbol() );
				token.append('>');
			}
		}
		else if (temp == '*')
		{
			token.append( nextSymbol() );
			if (lookAhead() == 'r' || lookAhead() == 'k')
			{
				token.append( nextSymbol() );
				token.append( nextSymbol() );
			}
			else
				token.append( nextSymbol() );
		}
		else if (temp == '\\' )
		{
			/** append the * character and the nextSymbol to token so as to form a paragraph attribute and '\\' character to deal special characters that contain XPress Tags codes.
			*/
			token.append( nextSymbol() );
			token.append( nextSymbol() ); 
		}
		else if (temp == '\"')
		{
			/**If inch character is found, the scanner should enter the string mode and enter the corresponding string as the token
			*/
			enterState(stringMode);
			m_top = m_top+1;
			token = getToken();
			enterState( previousState() );
		} 
		else if (temp.isDigit())
		{
			while (lookAhead().isDigit() )
			{
				token.append( nextSymbol() );
				if (lookAhead() == '.' )
					token.append( nextSymbol() );
			}
		}
		else if (temp == 'A')
		{
			token.append(nextSymbol());
		}
		else if (temp == 'a')
		{
			token.append( nextSymbol() );
			//look for $ or $$ so as to form the token a$ and a$$
			token.append( getToken() );
		}
		else if (temp == '$') //look for tags like $,$$ etc
		{
			token.append( nextSymbol() );
			//check for existence one more $
			if (lookAhead() == '$' )
				token.append( nextSymbol() );
		}
		else if (temp == '>')
			token.append( nextSymbol() );
		else if (temp == '(')
		{
			m_top = m_top+1;
			token = getToken();
		}
		else if (temp == ',')
		{
			m_top = m_top+1;
			token=getToken();
		}
		else 
			token.append( nextSymbol() );
	}

	if (currentState() == nameMode)
	{
		/**
		This mode should return the name of stylesheet (or font set or color) or '=' or ':','[' etc. This mode works with the assumption that a character '@' have occurred prior to it.Hence inorder to obtain the tags like '@$:' or '@:' ,we will append @ symbol initially to the token.
		*/
		if ( (lookAhead(-1) == '@') && (temp == ':') ) // get the simplest token @
		{
			token.append('@');
		}
		else if ( (lookAhead(-1) == '@') && (temp == '$') ) //get the token @$
		{
			token.append('@');
			token.append( nextSymbol() );
		}
		if (temp == '\"' )
		{
			enterState(stringMode);
			m_top = m_top+1;
			token = getToken();
			enterState( previousState() );
		}
		else if (temp == ',' )
			m_top = m_top+1;
		else if (temp == '[' )
		{
			//return [F] or [C] or [Sp or [St
			token.append( nextSymbol() );
			if (lookAhead() == 'F' || lookAhead() == 'C' || lookAhead() == 'S')
			{
				token.append( nextSymbol() );
				token.append( nextSymbol() ); //append ]
			}
		}
		else if (lookAhead() == '=')
			token.append( nextSymbol() );
		else if (lookAhead() == ':')
			token.append( nextSymbol() );
		else if (lookAhead() == '<')
		{
			token.append( nextSymbol() );
		}
		else
		{ // find the name and return it as a tag
			while( 1 )
			{
				temp = lookAhead();
				if (temp == ':' || temp == '=' )
					break;
				if (m_top >= m_input_Buffer.length())
					break;
				token.append(nextSymbol());
			}
		}
	}
	
	if ( currentState() == stringMode )
	{
		/* This mode should return those strings which are inside an inch character, while in tagMode. Hence this should set the mode to tagMode before returning the string value. This will call a function QString sliceString() which will return the required string and maintains the top correctly
		*/
		token = sliceString();
	}
	return token;
}
scannerMode XtgScanner::currentState()
{
	return m_mode;
}
scannerMode XtgScanner::previousState()
{
	return m_prevMode;
}

QChar XtgScanner::lookAhead(int adj)
{
	char ch = 0;
	if (m_top < m_input_Buffer.length())
	{
		ch = m_input_Buffer.at(m_top + adj);
		QByteArray ba;
		ba.append(ch);
		QString m_txt = m_codec->toUnicode(ba);
		if (!m_txt.isEmpty())
			return m_txt.at(0);
		else
			return QChar(0);
	}
	return QChar(0);
}

QChar XtgScanner::nextSymbol()
{
	char ch = 0;
	if (m_top < m_input_Buffer.length())
	{
		ch = m_input_Buffer.at(m_top++);
		QByteArray ba;
		ba.append(ch);
		QString m_txt = m_codec->toUnicode(ba);
		if (!m_txt.isEmpty())
			return m_txt.at(0);
		else
			return QChar(0);
	}
	return QChar(0);
}

void XtgScanner::flushText()
{
	if (!m_textToAppend.isEmpty())
	{
		m_textToAppend.replace(QChar(10), SpecialChars::LINEBREAK);
		m_textToAppend.replace(QChar(12), SpecialChars::FRAMEBREAK);
		m_textToAppend.replace(QChar(30), SpecialChars::NBHYPHEN);
		m_textToAppend.replace(QChar(160), SpecialChars::NBSPACE);
		int posC = m_item->itemText.length();
		m_item->itemText.insertChars(posC, m_textToAppend);
		m_item->itemText.applyStyle(posC, m_currentParagraphStyle);
		m_item->itemText.applyCharStyle(posC, m_textToAppend.length(), m_currentCharStyle);
		m_textToAppend="";
	}
}

bool XtgScanner::styleStatus(QStringList &name,QString &sfcname)
{
	int i;
	if (sfcname == "")
		return true;
	for (i = 0; i < name.size(); i++)
	{
		if (name.at(i) == sfcname)
			return true;
	}
	return false;
}

void XtgScanner::applyFeature(StyleFlagValue feature)
{
	flushText();
	if (m_styleEffects & feature)
		m_styleEffects &= ~feature;
	else
		m_styleEffects |= feature;
	m_currentCharStyle.setFeatures(m_styleEffects.featureList());
}

void XtgScanner::xtgParse()
{
	/* Enter the default mode as textMode */
	enterState(textMode);
	m_currentParagraphStyle.setParent(CommonStrings::DefaultParagraphStyle);
	m_currentParagraphStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	m_currentParagraphStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	m_currentCharStyle = m_currentParagraphStyle.charStyle();
	while (lookAhead() != '\0')
	{
		m_token = getToken();
		QHash<QString,void (XtgScanner::*)(void)> *temp = NULL;
		if (m_mode == tagMode)
			temp = &m_tagModeHash;
		else if (m_mode == nameMode)
			temp = &m_nameModeHash;
		else if (m_mode == textMode)
			temp = &m_textModeHash;
		if (temp->contains(m_token) )
		{
			funPointer = temp->value(m_token);
			(this->*funPointer)();
		}

		/**
				Various character Style Applications <@stylesheetname>. We cannot hash this since stylesheetname
				is not constant
				*/

		else if ( (currentState() == tagMode ) && m_token.startsWith('@') && m_token.endsWith('>') )
		{
			/*here we receive a token @stylesheetname>, hence we have to slice of token to 
				get the name of character stylesheet to be applied
			*/
			m_define = 0;
			m_sfcName = m_token.remove(0,1);
			m_sfcName = m_sfcName.remove(m_sfcName.size()-1,1);
			flushText();
		//	if (styleStatus(definedCStyles,sfcName))
		//		writer->setCharStyle(sfcName);
		//	else
		//	{
		//		showWarning(sfcName);
		//		writer->setCharStyle("");
		//	}
		//	currentCharStyle = writer->getCurrentCharStyle();
		}
		if (m_top >= m_input_Buffer.length())
			break;
	}
	if (!m_textToAppend.isEmpty())
	{
		m_textToAppend.replace(QChar(10), SpecialChars::LINEBREAK);
		m_textToAppend.replace(QChar(12), SpecialChars::FRAMEBREAK);
		m_textToAppend.replace(QChar(30), SpecialChars::NBHYPHEN);
		m_textToAppend.replace(QChar(160), SpecialChars::NBSPACE);
		ParagraphStyle newStyle;
		newStyle.setParent(m_currentParagraphStyle.name());
		int posC = m_item->itemText.length();
		m_item->itemText.insertChars(posC, m_textToAppend);
		m_item->itemText.applyStyle(posC, newStyle);
		m_item->itemText.applyCharStyle(posC, m_textToAppend.length(), m_currentCharStyle);
	}
//	qDebug()<<"Unsupported : "<<unSupported;
}

XtgScanner::~XtgScanner()
{
}
