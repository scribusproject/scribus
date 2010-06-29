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
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "xtgscanner.h"
#include "text/specialchars.h"
#include "fonts/scface.h"
#include <QString>
#include <QList>
#include <QDebug>
#include <QMessageBox>

class ScribusDoc;

XtgScanner::XtgScanner ( QString documentName,TextWriter *wr,QString& buffer, bool textOnly, bool prefix)
{
	writer = wr;
	docname = documentName;
	importTextOnly = textOnly;
	usePrefix    = prefix;
	input_Buffer = buffer;
	doc = writer->currentDoc();
	initTagMode();
	initTextMode();
	initNameMode();
	initLanguages();
	top=0;
	prevMode = textMode;
	textToAppend = "";
	define = 0;
	isBold=false;
	isItalic=false;
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
	tagModeHash.insert("\\>",&XtgScanner::appendSpChar2);
	tagModeHash.insert("\\",&XtgScanner::appendSpChar3);
	tagModeHash.insert(">",&XtgScanner::defClose);
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
	nameModeHash.insert("[Sp",&XtgScanner::definePStyles);
	nameModeHash.insert("[St",&XtgScanner::defineCStyle);
	nameModeHash.insert("=",&XtgScanner::defEquals);
	nameModeHash.insert(":",&XtgScanner::defColon);
}

/** Function for each tokens in tagMode. Each token is hashed with corresponding function pointers
*/

void XtgScanner::setPlain()
{
}


void XtgScanner::setBold()
{
	/* We need to toggle the bold status, hence we set the font as Regular if it is Bold and Italic if it is Bold   Italic. If the case is not to toggle the font, we just set it to Bold or Bold Italic */
	if (curFontUsed.isNone()==true)
	{
		isBold=true;
	}
	else
	{
		QHash<QString,QString> font;
		font.insert("Regular","Bold");
		font.insert("Italic","Bold Italic");
		font.insert("Bold","Regular");
		font.insert("Bold Italic","Italic");
		QString fontStyle = curFontUsed.style();
		fontStyle = font.value(fontStyle);
		curFontUsed = (*(doc->AllFonts)).value( curFontUsed.family() +" "+ fontStyle );
		if (define==0)
		{
			flushText();
			currentCharStyle.setFont(curFontUsed);
			writer->setCharStyle(currentCharStyle);
			currentCharStyle = writer->getCurrentCharStyle();
		}
		if (define==1)
			defCharStyle.setFont(curFontUsed);
		if (define==2)
			defParagraphStyle.charStyle().setFont(curFontUsed);
	}
}

void XtgScanner::setItalics()
{
	/* Hash is formed as in the case of setBold() */
	if (curFontUsed.isNone()==true)
		isItalic = true;
	else
	{
		QHash<QString,QString> font;
		font.insert("Regular","Italic");
		font.insert("Bold","Bold Italic");
		font.insert("Italic","Regular");
		font.insert("Bold Italic","Bold");
		QString fontStyle = curFontUsed.style();
		fontStyle = font.value(fontStyle);
		curFontUsed = (*(doc->AllFonts)).value( curFontUsed.family() +" "+ fontStyle );

		if (define == 0)
		{
			flushText();
			currentCharStyle.setFont(curFontUsed);
			writer->setCharStyle(currentCharStyle);
			currentCharStyle = writer->getCurrentCharStyle();
		}
		if (define == 1)
			defCharStyle.setFont(curFontUsed);
		if (define == 2)
			defParagraphStyle.charStyle().setFont(curFontUsed);
	}
}

void XtgScanner::setShadow()
{
	applyFeature(CharStyle::SHADOWED);
}

void XtgScanner::setOutline()
{
	applyFeature(CharStyle::OUTLINE);
}

void XtgScanner::setUnderline()
{
	applyFeature(CharStyle::UNDERLINE);
}

void XtgScanner::setWordUnder()
{
	unSupported.append(token);
}

void XtgScanner::setStrikethrough()
{
	applyFeature(CharStyle::STRIKETHROUGH);
}

void XtgScanner::setDoubleStrike()
{
	unSupported.append(token);
}

void XtgScanner::setAllCaps()
{
	applyFeature(CharStyle::ALLCAPS);
}

void XtgScanner::setSmallCaps()
{
	applyFeature(CharStyle::SMALLCAPS);
}

void XtgScanner::setSuperscript()
{
	applyFeature(CharStyle::SUPERSCRIPT);
}

void XtgScanner::setSubscript()
{
	applyFeature(CharStyle::SUBSCRIPT);
}

void XtgScanner::setSuperior()
{
	unSupported.append(token);	
}

void XtgScanner::setFont()
{
	/** define/apply font */
	flushText();
	token = getToken();
	curFontUsed = (*(doc->AllFonts)).value(token);
	if (curFontUsed.isNone())
	{
		QList<QString> List = ((*(doc->AllFonts)).keys());
		QStringList filter = static_cast<QStringList>(List).filter(token);
		if (!filter.empty())
		List = filter;
		FontSelect *f = new FontSelect(static_cast<QStringList>(List));
		if (f->exec())
		{
			token = f->setFont();
			delete f;
		}
		else
			delete f;
	}
	curFontUsed = (*(doc->AllFonts)).value(token);
	/* Now we check whether isBold=true and isItalic=true. If yes, we will set those */
	if (isBold && !(curFontUsed.style()).contains("Bold"))
	{
		setBold();
		isBold=false;
	}
	if (isItalic && !(curFontUsed.style()).contains("Italic"))
	{
		setItalics();
		isItalic=false;
	}
	if (define == 0)
	{
		flushText();
		currentCharStyle.setFont(curFontUsed);
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 1)
		defCharStyle.setFont(curFontUsed);
	if (define == 2)
		defParagraphStyle.charStyle().setFont( curFontUsed );
}

void XtgScanner::setFontSize()
{
	token = getToken();
	if (define == 0)
	{
		flushText();
		currentCharStyle.setFontSize( token.toDouble() * 10 );
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define ==  1)
		defCharStyle.setFontSize( token.toDouble() * 10 );
	if (define ==  2)
		defParagraphStyle.charStyle().setFontSize( token.toDouble() * 10 );
}

void XtgScanner::setColor()
{
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
	if (define == 0)
	{
		flushText();
		currentCharStyle.setFillColor(token);
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 1)
		defCharStyle.setFillColor(token);
	if (define == 2)
		defParagraphStyle.charStyle().setFillColor(token);
}

void XtgScanner::setShade()
{
	token = getToken();
	if (define == 0)
	{
		flushText();
		currentCharStyle.setFillShade(token.toDouble() * 10);
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 1)
		defCharStyle.setFillShade(token.toDouble() * 10);
	if (define == 2)
		defParagraphStyle.charStyle().setFillShade(token.toDouble() * 10);

}

void XtgScanner::setHorizontalScale()
{
	token = getToken();
	if (define == 0)
	{
		flushText();
		currentCharStyle.setScaleH(token.toDouble() * 10);
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 1)
		defCharStyle.setScaleH(token.toDouble() * 10);
	if (define == 2)
		defParagraphStyle.charStyle().setScaleH(token.toDouble() * 10);
}

void XtgScanner::setKern()
{
	token = getToken();
	if (define == 0)
	{
		flushText();
		currentCharStyle.setWordTracking(token.toDouble() * 10);
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 1)
	defCharStyle.setWordTracking(token.toDouble() * 10);
	if (define == 2)
	defParagraphStyle.charStyle().setWordTracking(token.toDouble() * 10);
}

void XtgScanner::setTrack()
{
	token = getToken();
	if (define == 0)
	{
		flushText();
		currentCharStyle.setTracking(token.toDouble() * 10);
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 1)
		defCharStyle.setTracking(token.toDouble() * 10);
	if (define == 2)
		defParagraphStyle.charStyle().setTracking(token.toDouble() * 10);
}

void XtgScanner::setBaseLineShift()
{
	token = getToken();
	unSupported.append("b"+token);
}

void XtgScanner::setVerticalScale()
{
	token = getToken();
	if (define == 0)
	{
		flushText();
		currentCharStyle.setScaleV(token.toDouble() * 10);
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 1)
		defCharStyle.setScaleV(token.toDouble() * 10);
	if (define == 2)
		defParagraphStyle.charStyle().setScaleV(token.toDouble() * 10);
}

void XtgScanner::setLigatures()
{
	token = getToken();
	unSupported.append("G"+token);
}

void XtgScanner::csetOpacity()
{
	token = getToken();
	unSupported.append("p"+token);
}

void XtgScanner::setOpenType()
{
	/* OpenType can be like this : o("xxxx","yyyy","zzzz") */
}

void XtgScanner::setCharAlignment()
{
	token.append( nextSymbol() );
	unSupported.append(token);	
}

void XtgScanner::setVertStories()
{
	token.append( nextSymbol() );
	unSupported.append(token);
}

void XtgScanner::setEmphasisMarks()
{
	token.append( nextSymbol() );
	unSupported.append(token);
}

void XtgScanner::setAsncjk()
{
	//Apply sending to non-CJK characters.
	token.append( nextSymbol() );
	unSupported.append(token);
}

void XtgScanner::setLanguages()
{
	token = getToken();
	if (define == 0)
	{
		flushText();
		currentCharStyle.setLanguage( languages.value( token.toInt() ) );
		writer->setCharStyle(currentCharStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 1)
		defCharStyle.setLanguage( languages.value( token.toInt() ) );
	if (define == 2)
		defParagraphStyle.charStyle().setLanguage( languages.value( token.toInt() ) );	
}

void XtgScanner::setXPresOwn()
{

	unSupported.append(token+')');
	//All these tags are unsupported in Scribus, so just ignoring them till we find close paranthesis
	while (lookAhead() != ')')
		top=top+1;
	top=top+1;//Ignore )
}

void XtgScanner::setAlignLeft()
{
	if (define == 0)
	{
		flushText();
		currentParagraphStyle.setAlignment(ParagraphStyle::Leftaligned);
		writer->setStyle(currentParagraphStyle);
		currentCharStyle = writer->getCurrentCharStyle();
	}
	if (define == 2)
		defParagraphStyle.setAlignment(ParagraphStyle::Leftaligned);
}

void XtgScanner::setAlignCenter()
{
	if (define == 0)
	{
		flushText();
		currentParagraphStyle.setAlignment(ParagraphStyle::Centered);
		writer->setStyle(currentParagraphStyle);
		currentParagraphStyle = writer->getCurrentStyle();
		currentCharStyle = currentParagraphStyle.charStyle();
	}
	if (define == 2)
		defParagraphStyle.setAlignment(ParagraphStyle::Centered);
}

void XtgScanner::setAlignRight()
{
	if (define == 0)
	{
		flushText();
		currentParagraphStyle.setAlignment(ParagraphStyle::Rightaligned);
		writer->setStyle(currentParagraphStyle);
		currentParagraphStyle = writer->getCurrentStyle();
		currentCharStyle = currentParagraphStyle.charStyle();
	}
	if (define == 2)
		defParagraphStyle.setAlignment(ParagraphStyle::Rightaligned);
}

void XtgScanner::setAlignJustify()
{
	if (define == 0)
	{
		flushText();
		currentParagraphStyle.setAlignment(ParagraphStyle::Justified);
		writer->setStyle(currentParagraphStyle);
		currentParagraphStyle = writer->getCurrentStyle();
		currentCharStyle = currentParagraphStyle.charStyle();
	}
	if (define == 2)
		defParagraphStyle.setAlignment(ParagraphStyle::Justified);
}

void XtgScanner::setAlignFJustify()
{
	if (define == 0)
	{
		flushText();
		currentParagraphStyle.setAlignment(ParagraphStyle::Extended);
		writer->setStyle(currentParagraphStyle);
		currentParagraphStyle = writer->getCurrentStyle();
		currentCharStyle = currentParagraphStyle.charStyle();
	}
	if (define == 2)
		defParagraphStyle.setAlignment(ParagraphStyle::Extended);
}

void XtgScanner::setTabStops()
{
	//tag *t
	//	token = getToken(); // Will contain the position
	
}

void XtgScanner::setPAttributes()
{
	double leftIndent,firstlineIndent,rightIndent,leading,gapBefore,gapAfter;
	leftIndent = getToken().toDouble();
	firstlineIndent = getToken().toDouble();
	rightIndent = getToken().toDouble();
	leading = getToken().toDouble();
	gapBefore = getToken().toDouble();
	gapAfter = getToken().toDouble();
	top = top+1;
	token="";
	token.append( nextSymbol() );

	if (token == "g")
	{
		/* We have to discard (P,S)) since it is not yet supported in Scribus
		*/
		while (lookAhead() != ')' )
			top=top+1;
	}
	if (define == 0)
	{
		flushText();
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
		writer->setStyle(currentParagraphStyle);
		currentParagraphStyle = writer->getCurrentStyle();
		currentCharStyle = currentParagraphStyle.charStyle();
	}
	if (define == 2)
	{
		defParagraphStyle.setLeftMargin(leftIndent);
		defParagraphStyle.setRightMargin(rightIndent);
		defParagraphStyle.setFirstIndent(firstlineIndent);
		defParagraphStyle.setLineSpacing(leading);
		defParagraphStyle.setGapBefore(gapBefore);
		defParagraphStyle.setGapAfter(gapAfter);
		if (token == "G")
			defParagraphStyle.setUseBaselineGrid(true);
		if (token == "g")
			defParagraphStyle.setUseBaselineGrid(false);
	}	
}

void XtgScanner::setHyphenation()
{
	token=getToken();
}

void XtgScanner::setPRuleAbove()
{
	unSupported.append(token);
	while (lookAhead() != '>')
		top = top+1;
}

void XtgScanner::setPRuleBelow()
{
	unSupported.append(token);
	while (lookAhead() != '>')
		top = top+1;
}

void XtgScanner::setDropCap()
{
	int lineCount;
	double charCount;
	lineCount = getToken().toInt();
	if (lineCount==0) //Specify No rule
	{
		if (define == 0)
		{
			flushText();
			currentParagraphStyle.setHasDropCap(false);
			writer->setStyle(currentParagraphStyle);
			currentParagraphStyle = writer->getCurrentStyle();
			currentCharStyle = currentParagraphStyle.charStyle();
		}
		if (define == 2)
		{
			defParagraphStyle.setHasDropCap(false);
		}
	}
	else
	{
		charCount = getToken().toDouble();

		if (define == 0)
		{
			flushText();
			currentParagraphStyle.setDropCapLines(lineCount);
			currentParagraphStyle.setDropCapOffset(charCount);
			currentParagraphStyle.setHasDropCap(true);
			writer->setStyle(currentParagraphStyle);
			currentParagraphStyle = writer->getCurrentStyle();
			currentCharStyle = currentParagraphStyle.charStyle();
		}

		if (define == 2)
		{
			defParagraphStyle.setDropCapLines(lineCount);
			defParagraphStyle.setDropCapOffset(charCount);
			defParagraphStyle.setHasDropCap(true);
		}
	}
}

void XtgScanner::setKeepNextPar()
{
	token.append(nextSymbol());
	unSupported.append(token);
}

void XtgScanner::setKeepTogether()
{
	if (lookAhead() == '(')
	{
		while (lookAhead() != ')')
			token.append( nextSymbol() );
	}
	else
		token.append(nextSymbol());
	unSupported.append(token);
}

void XtgScanner::setHangingCSet()
{
	token.append( getToken() );
	unSupported.append(token);
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
	writer->setCharStyle("Default Character Style");
	currentCharStyle = writer->getCurrentCharStyle();
}

void XtgScanner::applyCStyle2()
{
	//apply normal character style sheet <@$>
	define = 0;
	flushText();
	writer->setCharStyle("Default Character Style");
	currentCharStyle = writer->getCurrentCharStyle();
}

void XtgScanner::applyCStyle3()
{
	//apply paragraph's character stylesheet <@$p>
	define = 0;
	flushText();
	currentCharStyle = currentParagraphStyle.charStyle();
	writer->setCharStyle(currentCharStyle);
	currentCharStyle = writer->getCurrentCharStyle();
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
}

/** Functions corresponding to tokens in textMode
*/

void XtgScanner::defNewLine()
{
	flushText();
	if (lookAhead() == '@')
		newlineFlag = true;
	else
		writer->appendText(SpecialChars::PARSEP);
}

void XtgScanner::defHardReturn()
{
	/* I saw a strange nature in XPress Tags that every hardreturn is followed by a soft return, hence fixing the
		new line flag to true */
	if (lookAhead() == '\n')
		newlineFlag = true;
	else
		writer->appendText(SpecialChars::PARSEP);
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
}

/** Functions corresponding to tokens in nameMode
*/

void XtgScanner::defClose()	//Token >
{
	if (usePrefix)
		sfcName = docname+"-"+sfcName;
	if (define == 1)
	{
		writer->defineCharStyle(sfcName,defCharStyle);
		definedCStyles.append(sfcName);
		defCharStyle.erase();
	}
	if (define == 2)
	{
		writer->defineStyle(sfcName,defParagraphStyle);
		definedStyles.append(sfcName);
		defParagraphStyle.erase();
	}
	if (define != 0)
	{
		define = 0;
		dcsFeatures.clear();
	}
	enterState(textMode);	
}

void XtgScanner::defEquals()	//Token =
{
	//define a character stylesheet with the name in sfcName if the next character is not [
	newlineFlag = false;
	if (lookAhead() != '[')
	{
		define = 1;
		defCharStyle.setParent("");
		enterState(textMode);
	}
	else 
		define = 2;
}

void XtgScanner::defColon()
{
	flushText();
	if (sfcName.isEmpty())
		qDebug()<<"Empty String";
	if (sfcName == "@")
		sfcName = "Default Paragraph Style";
	if (sfcName == "@$")
		sfcName = "Default Paragraph Style";
	writer->setStyle(sfcName);
	currentParagraphStyle = writer->getCurrentStyle();
	writer->setCharStyle(currentParagraphStyle.charStyle());
	currentCharStyle = writer->getCurrentCharStyle();
	if (newlineFlag)
		writer->appendText(SpecialChars::PARSEP);
	newlineFlag = false;
	enterState(textMode);			
	define = 0;
}

void XtgScanner::defFontSet()
{
	unSupported.append("[F]");
	while (lookAhead() != '>' )
		top = top+1;
	top = top-1;
}

void XtgScanner::defColor()
{
	//Define the color using writer->defineColor()
	
}

void XtgScanner::definePStyles()
{
	QString s1,s2,s3;
	enterState(stringMode);
	define = 2;
	top=top+1;//skip the inch
	s1 = getToken();//will contain the string 1
	top=top+2;//we have to skip comma and next inch character
	s2 = getToken();
	if (lookAhead() != ']' )
	{
		top = top+2;
		s3 = getToken();
	}
	top++; // to ensure that ] is avoided
	
	/** Type 1 definition of Paragraph Stylesheet. Refer Documentation for details
	*/
	if (s1=="" && s2=="" && s3=="")
	{
		//define paragraph stylesheet with default character attributes
		defParagraphStyle.setParent("");
		defParagraphStyle.charStyle().setParent("Default Character Style");
	}
	
	/** Type 2 definition of Paragraph Stylesheet. Refer Documentation for details
	*/
	else if (s1=="" && s2=="" && s3!="")
	{
		//define paragraphstyle with character style whose name is in s3
		defParagraphStyle.setParent("");
		if(styleStatus(definedCStyles,s3))
			defParagraphStyle.charStyle().setParent(s3);
		else
		{
			showWarning(s3);
			defParagraphStyle.charStyle().setParent("Default Character Style");
		}
	}

	/** Type 3 definition of Paragraph Stylesheet. Refer Documentation for details
	*/
	else if (s1 != "" && s2 != "" && s3 != "")
	{
		if (styleStatus(definedStyles, s1) )
			defParagraphStyle.setParent(s1);
		else
		{
			showWarning(s1);
			defParagraphStyle.setParent("Default Paragraph Style");
		}
		/** s2 is the next paragraph style which is to be applied, but this cannot be done at moment in Scribus 
		*/
		currentParagraphStyle = writer->getCurrentStyle();
		currentCharStyle = currentParagraphStyle.charStyle();
		defParagraphStyle.charStyle().setParent(s3);
	}
	else
	{
		//generate error : parser error
	}
	enterState(textMode);
}

void XtgScanner::defineCStyle()
{
	//token [St
	QString s4;
	top = top+10;
	s4 = getToken();
	if (styleStatus(definedCStyles,s4))
		defCharStyle.setParent(s4);
	else
	{
		showWarning(s4);
		defCharStyle.setParent("Default Character Style");
	}
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
		if ( (input_Buffer.at(top-1)=='@') && (temp == ':') ) // get the simplest token @
		{
			token.append('@');
		}
		else if ( (input_Buffer.at(top-1)=='@') && (temp == '$') ) //get the token @$
		{
			token.append('@');
			token.append( nextSymbol() );
		}
		else if (temp == '\"' )
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
				token.append( input_Buffer.at(top++) );
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

QChar XtgScanner::lookAhead()
{
	return input_Buffer.at ( top );
}

QChar XtgScanner::nextSymbol()
{
	return input_Buffer.at ( top++ );
}

void XtgScanner::flushText()
{
	/* Append any text if exist in textToAppend */
	if (!textToAppend.isEmpty())
	{
		writer->appendText(textToAppend);
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

void XtgScanner::showWarning(QString &name)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Message");
	QString message = "Style " + name + " is not defined, falling back to Scribus Default";
	msgBox.setText(message);
	msgBox.exec();
}
void XtgScanner::applyFeature(const QString &feature)
{
	if (define == 0)
	{
		flushText();
		//checks whether the feature exist in feature list
		if( (featureIndex=ccsFeatures.indexOf(feature,0)) == -1 )
			ccsFeatures.append(feature);
		else //feature exist in list, so remove it and update the style
			ccsFeatures.removeAt( featureIndex );
		currentCharStyle.setFeatures(ccsFeatures);	
		currentCharStyle.updateFeatures();
		writer->setCharStyle(currentCharStyle);
	}
	if (define ==  1)
	{
		dcsFeatures.append(feature);
		defCharStyle.setFeatures(dcsFeatures);
	}
	if (define ==  2)
	{
		dcsFeatures.append(feature);
		defParagraphStyle.charStyle().setFeatures(dcsFeatures);
	}
}

void XtgScanner::xtgParse()
{
	/* Enter the default mode as textMode */
	enterState(textMode);
	writer->setCharStyle("");
	writer->setStyle("");
	currentCharStyle = writer->getCurrentCharStyle();
	currentParagraphStyle = writer->getCurrentStyle();
	while (input_Buffer.at(top) != '\0')
	{
		token = getToken();
		QHash<QString,void (XtgScanner::*)(void)> *temp = NULL;
		if (Mode == tagMode)
			temp = &tagModeHash;
		if (Mode == nameMode)
			temp = &nameModeHash;
		if (Mode == textMode)
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
			if (styleStatus(definedCStyles,sfcName))
				writer->setCharStyle(sfcName);
			else
			{
				showWarning(sfcName);
				writer->setCharStyle("");
			}
			currentCharStyle = writer->getCurrentCharStyle();
		}
	}
	writer->appendText(textToAppend);
	qDebug()<<"Unsupported : "<<unSupported;
}

XtgScanner::~XtgScanner()
{
}
