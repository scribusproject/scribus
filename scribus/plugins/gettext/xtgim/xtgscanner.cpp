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
	importTextOnly = textOnly;
	loadRawBytes(filename, input_Buffer);
	top = 0;
	if ((input_Buffer[0] == '\xFF') && (input_Buffer[1] == '\xFE'))
	{
		QByteArray tmpBuf;
		for (int a = 2; a < input_Buffer.count(); a += 2)
		{
			tmpBuf.append(input_Buffer[a]);
		}
		input_Buffer = tmpBuf;
	}
	doc = item->doc();
	initTagMode();
	initTextMode();
	initNameMode();
	initLanguages();
	prevMode = textMode;
	textToAppend = "";
	define = 0;
	styleEffects = ScStyle_None;
	m_codecList = QTextCodec::availableCodecs();
	if (m_codecList.contains("cp1252"))
		m_codec = QTextCodec::codecForName("cp1252");		// Default ANSI codec
	else
		m_codec = QTextCodec::codecForLocale();
	m_isBold = false;
	m_isItalic = false;
	inDef = false;
}

/** Initialise a QHash which maps the values of n## to corresponding language strings
*/

void XtgScanner::initLanguages()
{
	/* For more info, look at langmgr.cpp */
	languages.insert(72,"Bulgarian");
	languages.insert(73,"Catalan");
	languages.insert(52,"Chinese");
	languages.insert(53,"Chinese");
	languages.insert(68,"Croatian");
	languages.insert(56,"Czech");
	languages.insert(9,"Danish");
	languages.insert(5,"Dutch");
	languages.insert(2,"English");
	languages.insert(0,"English");
	languages.insert(17,"Finnish");
	languages.insert(1,"French");
	languages.insert(3,"German");
	languages.insert(70,"German");
	languages.insert(19,"German");
	languages.insert(20,"Greek");
	languages.insert(43,"Hungarian");
	languages.insert(21,"Icelandic");
	languages.insert(4,"Italian");
	languages.insert(14,"Japanese");
	languages.insert(41,"Lithuanian");
	languages.insert(12,"Norwegian (Bokmål)");
	languages.insert(80,"Norwegian (Bokmål)");
	languages.insert(42,"Polish");
	languages.insert(71,"Portuguese (BR)");
	languages.insert(10,"Portuguese");
	languages.insert(39,"Romanian");
	languages.insert(57,"Slovak");
	languages.insert(66,"Slovenian");
	languages.insert(8,"Spanish");
	languages.insert(7,"Swedish");
	languages.insert(62,"Ukranian");

}

/** Initialise the tagModeHash with the tokens returned in tagMode and its corresponding function pointers */

void XtgScanner::initTagMode()
{
	tagModeHash.insert("P",&XtgScanner::setPlain);
	tagModeHash.insert("B",&XtgScanner::setBold);
	tagModeHash.insert("I",&XtgScanner::setItalics);
	tagModeHash.insert("O",&XtgScanner::setOutline);
	tagModeHash.insert("S",&XtgScanner::setShadow);
	tagModeHash.insert("U",&XtgScanner::setUnderline);
	tagModeHash.insert("W",&XtgScanner::setWordUnder);
	tagModeHash.insert("/",&XtgScanner::setStrikethrough);
	tagModeHash.insert("R",&XtgScanner::setDoubleStrike);
	tagModeHash.insert("K",&XtgScanner::setAllCaps);
	tagModeHash.insert("H",&XtgScanner::setSmallCaps);
	tagModeHash.insert("+",&XtgScanner::setSuperscript);
	tagModeHash.insert("-",&XtgScanner::setSubscript);
	tagModeHash.insert("V",&XtgScanner::setSuperior);
	tagModeHash.insert("f",&XtgScanner::setFont);
	tagModeHash.insert("z",&XtgScanner::setFontSize);
	tagModeHash.insert("c",&XtgScanner::setColor);
	tagModeHash.insert("s",&XtgScanner::setShade);
	tagModeHash.insert("h",&XtgScanner::setHorizontalScale);
	tagModeHash.insert("k",&XtgScanner::setKern);
	tagModeHash.insert("t",&XtgScanner::setTrack);
	tagModeHash.insert("b",&XtgScanner::setBaseLineShift);
	tagModeHash.insert("y",&XtgScanner::setVerticalScale);
	tagModeHash.insert("G",&XtgScanner::setLigatures);
	tagModeHash.insert("p",&XtgScanner::csetOpacity);
	tagModeHash.insert("o",&XtgScanner::setOpenType);
	tagModeHash.insert("*A",&XtgScanner::setCharAlignment);
	tagModeHash.insert("L",&XtgScanner::setVertStories);
	tagModeHash.insert("M",&XtgScanner::setEmphasisMarks);
	tagModeHash.insert("Y",&XtgScanner::setAsncjk);//Apply sending to non-CJK characters.
	tagModeHash.insert("n",&XtgScanner::setLanguages);
	tagModeHash.insert("A(",&XtgScanner::setXPresOwn); //Apply features like Rubi text, group characters & hyperlink
	tagModeHash.insert("*L",&XtgScanner::setAlignLeft);
	tagModeHash.insert("*C",&XtgScanner::setAlignCenter);
	tagModeHash.insert("*R",&XtgScanner::setAlignRight);
	tagModeHash.insert("*J",&XtgScanner::setAlignJustify);
	tagModeHash.insert("*F",&XtgScanner::setAlignFJustify);
	tagModeHash.insert("*t",&XtgScanner::setTabStops);
	tagModeHash.insert("*p",&XtgScanner::setPAttributes);
	tagModeHash.insert("*h",&XtgScanner::setHyphenation);
	tagModeHash.insert("*ra",&XtgScanner::setPRuleAbove);
	tagModeHash.insert("*rb",&XtgScanner::setPRuleBelow);
	tagModeHash.insert("*d",&XtgScanner::setDropCap);
	tagModeHash.insert("*kn",&XtgScanner::setKeepNextPar);
	tagModeHash.insert("*kt",&XtgScanner::setKeepTogether);
	tagModeHash.insert("*s",&XtgScanner::setHangingCSet);
	tagModeHash.insert("DoO",&XtgScanner::setGlyph);
	tagModeHash.insert("EX",&XtgScanner::setTranscodeSeq);
	tagModeHash.insert("DO",&XtgScanner::setUnencodedGlyph);

	tagModeHash.insert("@>",&XtgScanner::applyCStyle1);
	tagModeHash.insert("@$",&XtgScanner::applyCStyle2);
	tagModeHash.insert("@$p",&XtgScanner::applyCStyle3);
	tagModeHash.insert("$$",&XtgScanner::applyCStyle4);
	tagModeHash.insert("a$",&XtgScanner::applyCStyle5);
	tagModeHash.insert("a$$",&XtgScanner::applyCStyle6);

	tagModeHash.insert("$",&XtgScanner::setTypeStyle);

	tagModeHash.insert("\\@",&XtgScanner::appendSpChar1);
	tagModeHash.insert("x",&XtgScanner::xFlag);
	tagModeHash.insert("\\<",&XtgScanner::appendSpChar2);
	tagModeHash.insert("\\",&XtgScanner::appendSpChar3);
	tagModeHash.insert(">",&XtgScanner::defClose);
	tagModeHash.insert("e",&XtgScanner::setEncoding);
}

/** Initialise the textModeHash with tokens returned in textMode and its corresponding function pointers */

void XtgScanner::initTextMode()
{
	textModeHash.insert("\n",&XtgScanner::defNewLine);
	textModeHash.insert("<",&XtgScanner::defOpen);
	textModeHash.insert("@",&XtgScanner::defAtRate);
	textModeHash.insert("\r",&XtgScanner::defHardReturn);
}

/** Initialise the nameModeHash with tokens returned in nameMode and its corresponding function pointers */

void XtgScanner::initNameMode()
{
	nameModeHash.insert("[F]",&XtgScanner::defFontSet);
	nameModeHash.insert("[C]",&XtgScanner::defColor);
	nameModeHash.insert("[S\"",&XtgScanner::definePStyles);
	nameModeHash.insert("[Sp",&XtgScanner::definePStyles);
	nameModeHash.insert("[St",&XtgScanner::defineCStyle);
	nameModeHash.insert("=",&XtgScanner::defEquals);
	nameModeHash.insert(":",&XtgScanner::defColon);
}

/** Function for each tokens in tagMode. Each token is hashed with corresponding function pointers
*/

void XtgScanner::setPlain()
{
	styleEffects = ScStyle_None;
	currentCharStyle.setFeatures(styleEffects.featureList());
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
	m_item->itemText.applyStyle(posC, currentParagraphStyle);
	m_item->itemText.applyCharStyle(posC, 1, currentCharStyle);
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
				currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold Italic"]);
		}
		else if (slist.contains("Bold"))
			currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold"]);
	}
	else
	{
		if (m_isItalic)
		{
			if (slist.contains("Italic"))
				currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Italic"]);
		}
		else if (slist.contains("Regular"))
			currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Regular"]);
	}
}

void XtgScanner::setItalics()
{
	m_isItalic = !m_isItalic;
	int posC = m_item->itemText.length();
	m_item->itemText.insertChars(posC, "B");
	m_item->itemText.applyStyle(posC, currentParagraphStyle);
	m_item->itemText.applyCharStyle(posC, 1, currentCharStyle);
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
				currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold Italic"]);
		}
		else if (slist.contains("Italic"))
			currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Italic"]);
	}
	else
	{
		if (m_isBold)
		{
			if (slist.contains("Bold"))
				currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold"]);
		}
		else if (slist.contains("Regular"))
			currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Regular"]);
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
	styleEffects &= ~ScStyle_UnderlineWords;
	applyFeature(ScStyle_Underline);
}

void XtgScanner::setWordUnder()
{
	styleEffects &= ~ScStyle_Underline;
	applyFeature(ScStyle_UnderlineWords);
}

void XtgScanner::setStrikethrough()
{
	applyFeature(ScStyle_Strikethrough);
}

void XtgScanner::setDoubleStrike()
{
	styleEffects &= ~ScStyle_Strikethrough;
	currentCharStyle.setFeatures(styleEffects.featureList());
	unSupported.insert(token);
}

void XtgScanner::setAllCaps()
{
	styleEffects &= ~ScStyle_SmallCaps;
	applyFeature(ScStyle_AllCaps);
}

void XtgScanner::setSmallCaps()
{
	styleEffects &= ~ScStyle_AllCaps;
	applyFeature(ScStyle_SmallCaps);
}

void XtgScanner::setSuperscript()
{
	styleEffects &= ~ScStyle_Subscript;
	applyFeature(ScStyle_Superscript);
}

void XtgScanner::setSubscript()
{
	styleEffects &= ~ScStyle_Superscript;
	applyFeature(ScStyle_Subscript);
}

void XtgScanner::setSuperior()
{
	styleEffects &= ~ScStyle_Subscript;
	applyFeature(ScStyle_Superscript);
}

void XtgScanner::setFont()
{
	/** define/apply font */
	flushText();
	token = getToken();
	QString font = PrefsManager::instance()->appPrefs.itemToolPrefs.textFont;
	if (token != "$")
		font = getFontName(token);
	currentCharStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[font]);
	if(!doc->UsedFonts.contains(font))
		doc->AddFont(font);
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
			MissingFont *dia = new MissingFont(0, fontName, doc);
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
	token = getToken();
	if (token == "$")
		token = "12";
	currentCharStyle.setFontSize( token.toDouble() * 10 );
}

void XtgScanner::setColor()
{
	flushText();
	token = getToken();	

	QHash<QString,QString> color;
	color.insert("cC","Cyan");
	color.insert("cM","Magenta");
	color.insert("cY","Yellow");
	color.insert("cK","Black");

	if (token == "C" || token == "M" || token == "Y" || token == "K")
	{
		token = "c" + token; 
		token = color.value(token);
	}
	else if (!doc->PageColors.contains(token))
		token = "Black";
	if (doc->PageColors.contains(token))
		currentCharStyle.setFillColor(token);
}

void XtgScanner::setShade()
{
	flushText();
	token = getToken();
	if (token == "$")
		token = "100";
	currentCharStyle.setFillShade(token.toDouble());

}

void XtgScanner::setHorizontalScale()
{
	flushText();
	token = getToken();
	if (token == "$")
		token = "100";
	currentCharStyle.setScaleH(token.toDouble() * 10.0);
}

void XtgScanner::setKern()
{
	flushText();
	token = getToken();
	if (token == "$")
		token = "0";
//	currentCharStyle.setWordTracking((token.toDouble() * (currentCharStyle.fontSize() / 200.0)) / currentCharStyle.fontSize() * 10);
}

void XtgScanner::setTrack()
{
	flushText();
	token = getToken();
	if (token == "$")
		token = "0";
//	currentCharStyle.setTracking((token.toDouble() * (currentCharStyle.fontSize() / 200.0)) / currentCharStyle.fontSize() * 10);
}

void XtgScanner::setBaseLineShift()
{
	flushText();
	token = getToken();
	if (token == "$")
		token = "0";
	currentCharStyle.setBaselineOffset((token.toDouble() * 10000) / currentCharStyle.fontSize());
}

void XtgScanner::setVerticalScale()
{
	flushText();
	token = getToken();
	if (token == "$")
		token = "100";
//	currentCharStyle.setScaleV(token.toDouble() * 10.0);
}

void XtgScanner::setLigatures()
{
	token = getToken();
	unSupported.insert("G"+token);
}

void XtgScanner::csetOpacity()
{
	token = getToken();
	unSupported.insert("p"+token);
}

void XtgScanner::setOpenType()
{
	/* OpenType can be like this : o("xxxx","yyyy","zzzz") */
}

void XtgScanner::setCharAlignment()
{
	token.append( nextSymbol() );
	unSupported.insert(token);
}

void XtgScanner::setVertStories()
{
	token.append( nextSymbol() );
	unSupported.insert(token);
}

void XtgScanner::setEmphasisMarks()
{
	token.append( nextSymbol() );
	unSupported.insert(token);
}

void XtgScanner::setAsncjk()
{
	//Apply sending to non-CJK characters.
	token.append( nextSymbol() );
	unSupported.insert(token);
}

void XtgScanner::setLanguages()
{
	flushText();
	token = getToken();
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

	unSupported.insert(token+')');
	//All these tags are unsupported in Scribus, so just ignoring them till we find close paranthesis
	while (lookAhead() != ')')
		top=top+1;
	top=top+1;//Ignore )
}

void XtgScanner::setAlignLeft()
{
	flushText();
	currentParagraphStyle.setAlignment(ParagraphStyle::Leftaligned);
}

void XtgScanner::setAlignCenter()
{
	flushText();
	currentParagraphStyle.setAlignment(ParagraphStyle::Centered);
}

void XtgScanner::setAlignRight()
{
	flushText();
	currentParagraphStyle.setAlignment(ParagraphStyle::Rightaligned);
}

void XtgScanner::setAlignJustify()
{
	flushText();
	currentParagraphStyle.setAlignment(ParagraphStyle::Justified);
}

void XtgScanner::setAlignFJustify()
{
	flushText();
	currentParagraphStyle.setAlignment(ParagraphStyle::Extended);
}

void XtgScanner::setTabStops()
{
	QList<ParagraphStyle::TabRecord> tbs = currentParagraphStyle.tabValues();
	tbs.clear();
	if (lookAhead() == '0')
	{
		currentParagraphStyle.setTabValues(tbs);
		return;
	}
	while (lookAhead() != ')')
	{
		token = getToken();
		double pos = token.toDouble();
		token = getToken();
		int typ = token.toInt();
		token = getToken();
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
	currentParagraphStyle.setTabValues(tbs);
	
}

void XtgScanner::setPAttributes()
{
	double leftIndent = getToken().toDouble();
	double firstlineIndent = getToken().toDouble();
	double rightIndent = getToken().toDouble();
	double leading = getToken().toDouble();
	double gapBefore = getToken().toDouble();
	double gapAfter = getToken().toDouble();
	top++;
	token="";
	token.append( nextSymbol() );
	if ((token == "g") || (token == "G"))
	{
		// We have to discard (P,S)) since it is not yet supported in Scribus

		while (lookAhead() != ')' )
			top++;
	}
	currentParagraphStyle.setLeftMargin(leftIndent);
	currentParagraphStyle.setRightMargin(rightIndent);
	currentParagraphStyle.setFirstIndent(firstlineIndent);
	currentParagraphStyle.setLineSpacing(leading);
	currentParagraphStyle.setGapBefore(gapBefore);
	currentParagraphStyle.setGapAfter(gapAfter);
	if (token == "G")
		currentParagraphStyle.setUseBaselineGrid(true);
	if (token == "g")
		currentParagraphStyle.setUseBaselineGrid(false);
}

void XtgScanner::setHyphenation()
{
	unSupported.insert(token);
	token=getToken();
}

void XtgScanner::setPRuleAbove()
{
	unSupported.insert(token);
	if (lookAhead() == '0')
	{
		top++;
		return;
	}
	while (lookAhead() != ')')
		top++;
}

void XtgScanner::setPRuleBelow()
{
	unSupported.insert(token);
	if (lookAhead() == '0')
	{
		top++;
		return;
	}
	while (lookAhead() != ')')
		top++;
}

void XtgScanner::setDropCap()
{
	flushText();
	int charCount = getToken().toInt();
	if (charCount == 0) //Specify No rule
	{
		currentParagraphStyle.setHasDropCap(false);
	}
	else
	{
		int lineCount = getToken().toInt();
		currentParagraphStyle.setHasDropCap(true);
		currentParagraphStyle.setDropCapLines(lineCount);
	}
}

void XtgScanner::setKeepNextPar()
{
	token.append(nextSymbol());
	unSupported.insert(token);
}

void XtgScanner::setKeepTogether()
{
	if (lookAhead() == '(')
	{
		while (lookAhead() != ')')
		{
			token.append( nextSymbol() );
		}
	}
	token.append(nextSymbol());
	unSupported.insert(token);
}

void XtgScanner::setHangingCSet()
{
	token.append( getToken() );
	unSupported.insert(token);
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
	textToAppend.append("@");
}

void XtgScanner::appendSpChar2()
{
	textToAppend.append("<");
}

void XtgScanner::appendSpChar3()
{
	textToAppend.append("\\");
}

void XtgScanner::xFlag()
{
	xflag = true;
}

void XtgScanner::applyCStyle1()
{
	//apply nostyle character sheet <@>
	define = 0;
	flushText();
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setParent(pStyle);
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	currentCharStyle = newStyle.charStyle();
	currentCharStyle.setFontSize(120.0);
	styleEffects = ScStyle_None;
	currentCharStyle.setFeatures(styleEffects.featureList());
}

void XtgScanner::applyCStyle2()
{
	//apply normal character style sheet <@$>
	define = 0;
	flushText();
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setParent(pStyle);
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	currentCharStyle = newStyle.charStyle();
	currentCharStyle.setFontSize(120.0);
	styleEffects = ScStyle_None;
	currentCharStyle.setFeatures(styleEffects.featureList());
}

void XtgScanner::applyCStyle3()
{
	//apply paragraph's character stylesheet <@$p>
	define = 0;
	flushText();
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setParent(pStyle);
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	currentCharStyle = newStyle.charStyle();
	currentCharStyle.setFontSize(120.0);
	styleEffects = ScStyle_None;
	currentCharStyle.setFeatures(styleEffects.featureList());
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
	currentCharStyle = currentParagraphStyle.charStyle();
	currentCharStyle.setFontSize(120.0);
	styleEffects = ScStyle_None;
	currentCharStyle.setFeatures(styleEffects.featureList());
}

void XtgScanner::setEncoding()
{
	token = getToken();
	int enc = token.toInt();
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
	if (inDef)
		newlineFlag = true;
	else
	{
		int posT = m_item->itemText.length();
		if (posT > 0)
		{
			m_item->itemText.insertChars(posT, SpecialChars::PARSEP);
			m_item->itemText.applyStyle(posT, currentParagraphStyle);
		}
		inDef = false;
	}
}

void XtgScanner::defHardReturn()
{
	/* I saw a strange nature in XPress Tags that every hardreturn is followed by a soft return, hence fixing the
		new line flag to true */
	flushText();
	if (lookAhead() == '\n')
		newlineFlag = true;
	else
	{
		int posT = m_item->itemText.length();
		if (posT > 0)
		{
			m_item->itemText.insertChars(posT, SpecialChars::PARSEP);
			m_item->itemText.applyStyle(posT, currentParagraphStyle);
		}
		inDef = false;
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
	sfcName = getToken();
	if (sfcName == "@$:")
	{
		if (doc->paragraphStyles().contains(m_item->itemName() + "_Normal"))
		{
			ParagraphStyle newStyle;
			newStyle.setParent(m_item->itemName() + "_Normal");
			currentParagraphStyle = newStyle;
			currentCharStyle = newStyle.charStyle();
		}
		else if (doc->paragraphStyles().contains("Normal"))
		{
			ParagraphStyle newStyle;
			newStyle.setParent("Normal");
			currentParagraphStyle = newStyle;
			currentCharStyle = newStyle.charStyle();
		}
		enterState(previousState());
	}
	else if (sfcName == "@:")
	{
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		currentParagraphStyle = newStyle;
		currentCharStyle = newStyle.charStyle();
		currentCharStyle.setFontSize(120.0);
		styleEffects = ScStyle_None;
		currentCharStyle.setFeatures(styleEffects.featureList());
		enterState(textMode);
	}
	else if (doc->paragraphStyles().contains(m_item->itemName() + "_" + sfcName))
	{
		ParagraphStyle newStyle;
		newStyle.setParent(m_item->itemName() + "_" + sfcName);
		currentParagraphStyle = newStyle;
		currentCharStyle = newStyle.charStyle();
		if (lookAhead() == ':')
			top++;
		enterState(textMode);
	}
	else if (doc->paragraphStyles().contains(sfcName))
	{
		ParagraphStyle newStyle;
		newStyle.setParent(sfcName);
		currentParagraphStyle = newStyle;
		currentCharStyle = newStyle.charStyle();
		if (lookAhead() == ':')
			top++;
		enterState(textMode);
	}
}

/** Functions corresponding to tokens in nameMode
*/

void XtgScanner::defClose()	//Token >
{
	if (define == 1)
	{
		StyleSet<CharStyle>tmp;
		tmp.create(currentCharStyle);
		doc->redefineCharStyles(tmp, false);
		inDef = false;
	}
	else if (define == 2)
	{
		StyleSet<ParagraphStyle>tmp;
		tmp.create(currentParagraphStyle);
		doc->redefineStyles(tmp, false);
		inDef = false;
	}
	if (define != 0)
		define = 0;
	enterState(textMode);	
}

void XtgScanner::defEquals()	//Token =
{
	//define a character stylesheet with the name in sfcName if the next character is not [
	newlineFlag = false;
	if (lookAhead() != '[')
	{
		define = 1;
		currentCharStyle.setName(m_item->itemName() + "_" + sfcName);
		enterState(textMode);
	}
	else 
		define = 2;
	inDef = true;
}

void XtgScanner::defColon()
{
	flushText();
	if ((sfcName == "@") || (sfcName == "@$:") || (sfcName == "@:"))
	{
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		currentParagraphStyle = newStyle;
		currentCharStyle = newStyle.charStyle();
		currentCharStyle.setFontSize(120.0);
		styleEffects = ScStyle_None;
		currentCharStyle.setFeatures(styleEffects.featureList());
	}
	else if (doc->paragraphStyles().contains(m_item->itemName() + "_" + sfcName))
	{
		ParagraphStyle newStyle;
		newStyle.setParent(m_item->itemName() + "_" + sfcName);
		currentParagraphStyle = newStyle;
		currentCharStyle = newStyle.charStyle();
	}
	else if (doc->paragraphStyles().contains(sfcName))
	{
		ParagraphStyle newStyle;
		newStyle.setParent(sfcName);
		currentParagraphStyle = newStyle;
		currentCharStyle = newStyle.charStyle();
	}
	else
	{
		QString pStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setParent(pStyle);
		newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
		currentParagraphStyle = newStyle;
		currentCharStyle = newStyle.charStyle();
		currentCharStyle.setFontSize(120.0);
		styleEffects = ScStyle_None;
		currentCharStyle.setFeatures(styleEffects.featureList());
	}
	if (newlineFlag)
	{
		int posT = m_item->itemText.length();
		if (posT > 0)
		{
			m_item->itemText.insertChars(posT, SpecialChars::PARSEP);
			m_item->itemText.applyStyle(posT, currentParagraphStyle);
		}
	}
	newlineFlag = false;
	enterState(textMode);
	define = 0;
	if (!((lookAhead() == '\r') || (lookAhead() == '\n')))
		inDef = false;
}

void XtgScanner::defFontSet()
{
	unSupported.insert("[F]");
	while (lookAhead() != '>' )
		top = top+1;
	top = top-1;
}

void XtgScanner::defColor()
{
	bool isSpot = false;
	ScColor tmp;
	enterState(stringMode);
	token = getToken();
	while (lookAhead() != '>' )
	{
		token = getToken();
		if ((token == "CMJN") || (token == "CMYK"))
		{
			enterState(tagMode);
			token = getToken();
			if (token == "S")
			{
				token = getToken();
				isSpot = true;
			}
			double c = getToken().toDouble();
			double m = getToken().toDouble();
			double y = getToken().toDouble();
			double k = getToken().toDouble();
			tmp.setColor(qRound(c * 2.55), qRound(m * 255), qRound(y * 255), qRound(k * 255));
			tmp.setSpotColor(isSpot);
			tmp.setRegistrationColor(false);
			doc->PageColors.tryAddColor(sfcName, tmp);
		}
		else if (token == "RGB")
		{
			enterState(tagMode);
			token = getToken();
			if (token == "S")
			{
				token = getToken();
				isSpot = true;
			}
			double r = getToken().toDouble();
			double g = getToken().toDouble();
			double b = getToken().toDouble();
			tmp.setColorRGB(qRound(r * 2.55), qRound(g * 255), qRound(b * 255));
			tmp.setSpotColor(isSpot);
			tmp.setRegistrationColor(false);
			doc->PageColors.tryAddColor(sfcName, tmp);
		}
	}
	top++;
	enterState(textMode);
}

void XtgScanner::definePStyles()
{
	QString s1,s2,s3;
	enterState(stringMode);
	define = 2;
	if (token == "[S\"")
		s1 = getToken();
	else
	{
		while (lookAhead() != '\"')
			top++;					//skip the inch
		top++;
		s1 = getToken();			//will contain the string 1
	}
//	top = top + 2;				//we have to skip comma and next inch character
	while (lookAhead() != '\"')
		top++;
	top++;
	s2 = getToken();
	if (lookAhead() != ']' )
	{
		while (lookAhead() != '\"')
			top++;
		top++;
	//	top = top + 2;
		s3 = getToken();
	}
	top++; // to ensure that ] is avoided
	QString pStyle = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	if (s1 != "")
	{
		if (doc->paragraphStyles().contains(m_item->itemName() + "_" + s1))
			newStyle.setParent(m_item->itemName() + "_" + s1);
		else if  (doc->paragraphStyles().contains(s1))
			newStyle.setParent(s1);
		else
			newStyle.setParent(pStyle);
	}
	else
		newStyle.setParent(pStyle);
	newStyle.setName(m_item->itemName() + "_" + sfcName);
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	if (s3 != "")
	{
		if (doc->charStyles().contains(m_item->itemName() + "_" + s3))
			newStyle.charStyle().setParent(m_item->itemName() + "_" + s3);
		else if  (doc->charStyles().contains(s3))
			newStyle.charStyle().setParent(s3);
		else
			newStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	}
	else
	{
		newStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
		newStyle.charStyle().setFontSize(120.0);
		styleEffects = ScStyle_None;
		newStyle.charStyle().setFeatures(styleEffects.featureList());
	}
	currentParagraphStyle = newStyle;
	currentCharStyle = newStyle.charStyle();
	enterState(textMode);
}

void XtgScanner::defineCStyle()
{
	//token [St
	QString s4;
	top = top+10;
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
	prevMode = Mode;
	Mode = mode;
}

QString XtgScanner::sliceString()
{
	QString name;
	if (Mode == stringMode)
	{
		while (lookAhead() != '\"')
		{
			name.append( nextSymbol() );
		}
		top = top+1;
	}
	if (Mode == nameMode)
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
			top = top+1;
			textToAppend.append( nextSymbol() );
		}
		else
			textToAppend.append( nextSymbol() );
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
			top = top+1;
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
			top = top+1;
			token = getToken();
		}
		else if (temp == ',')
		{
			top = top+1;
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
			top = top+1;
			token = getToken();
			enterState( previousState() );
		}
		else if (temp == ',' )
			top = top+1;
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
				if (top >= input_Buffer.length())
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
	return Mode;
}
scannerMode XtgScanner::previousState()
{
	return prevMode;
}

QChar XtgScanner::lookAhead(int adj)
{
	char ch = 0;
	if (top < input_Buffer.length())
	{
		ch = input_Buffer.at(top + adj);
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
	if (top < input_Buffer.length())
	{
		ch = input_Buffer.at(top++);
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
	if (!textToAppend.isEmpty())
	{
		textToAppend.replace(QChar(10), SpecialChars::LINEBREAK);
		textToAppend.replace(QChar(12), SpecialChars::FRAMEBREAK);
		textToAppend.replace(QChar(30), SpecialChars::NBHYPHEN);
		textToAppend.replace(QChar(160), SpecialChars::NBSPACE);
		int posC = m_item->itemText.length();
		m_item->itemText.insertChars(posC, textToAppend);
		m_item->itemText.applyStyle(posC, currentParagraphStyle);
		m_item->itemText.applyCharStyle(posC, textToAppend.length(), currentCharStyle);
		textToAppend="";
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
	if (styleEffects & feature)
		styleEffects &= ~feature;
	else
		styleEffects |= feature;
	currentCharStyle.setFeatures(styleEffects.featureList());
}

void XtgScanner::xtgParse()
{
	/* Enter the default mode as textMode */
	enterState(textMode);
	currentParagraphStyle.setParent(CommonStrings::DefaultParagraphStyle);
	currentParagraphStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	currentParagraphStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	currentCharStyle = currentParagraphStyle.charStyle();
	while (lookAhead() != '\0')
	{
		token = getToken();
		QHash<QString,void (XtgScanner::*)(void)> *temp = NULL;
		if (Mode == tagMode)
			temp = &tagModeHash;
		else if (Mode == nameMode)
			temp = &nameModeHash;
		else if (Mode == textMode)
			temp = &textModeHash;
		if (temp->contains(token) )
		{
			funPointer = temp->value(token);
			(this->*funPointer)();
		}

		/**
				Various character Style Applications <@stylesheetname>. We cannot hash this since stylesheetname
				is not constant
				*/

		else if ( (currentState() == tagMode ) && token.startsWith('@') && token.endsWith('>') )
		{
			/*here we receive a token @stylesheetname>, hence we have to slice of token to 
				get the name of character stylesheet to be applied
			*/
			define = 0;
			sfcName = token.remove(0,1);
			sfcName = sfcName.remove(sfcName.size()-1,1);
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
		if (top >= input_Buffer.length())
			break;
	}
	if (!textToAppend.isEmpty())
	{
		textToAppend.replace(QChar(10), SpecialChars::LINEBREAK);
		textToAppend.replace(QChar(12), SpecialChars::FRAMEBREAK);
		textToAppend.replace(QChar(30), SpecialChars::NBHYPHEN);
		textToAppend.replace(QChar(160), SpecialChars::NBSPACE);
		ParagraphStyle newStyle;
		newStyle.setParent(currentParagraphStyle.name());
		int posC = m_item->itemText.length();
		m_item->itemText.insertChars(posC, textToAppend);
		m_item->itemText.applyStyle(posC, newStyle);
		m_item->itemText.applyCharStyle(posC, textToAppend.length(), currentCharStyle);
	}
//	qDebug()<<"Unsupported : "<<unSupported;
}

XtgScanner::~XtgScanner()
{
}
