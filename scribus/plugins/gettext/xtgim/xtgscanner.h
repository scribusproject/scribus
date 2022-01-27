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

#ifndef XTGSCANNER_H
#define XTGSCANNER_H

#include "scconfig.h"
#include "styles/charstyle.h"

#include "pageitem.h"

#include <QByteArray>
#include <QHash>
#include <QString>
#include <QSet>

class QTextCodec;
class QTextDecoder;

/**
Enum datatype for determining the Scanner mode 
*/
enum scannerMode
{
	textMode,
	tagMode,
	nameMode,
	stringMode
};


/**
\brief Class for scanning and parsing an XPress Tags Document
*/

class XtgScanner
{
public:
	XtgScanner(PageItem* item, bool textOnly, bool prefix, bool append);
	~XtgScanner();

	/**
	\brief initialise all hash functions with tokens and corresponding function pointers
	*/
	void initBasicHash();
	void initTagMode();
	void initTextMode();
	void initNameMode();
	void initLanguages();
	/**
	\brief Open file and initialize inputBuffer for parsing
	*/
	bool open(const QString& fileName);

	/**
	\brief Parse function which will parse the inputBuffer and append it into the PageItem
	*/
	void xtgParse();

	/**
	\brief This function will return the character in Buffer to which m_textIndex is now pointing to 
	*/
	QChar lookAhead(int adj = 0);

	/** 
	\brief A function which returns the next symbol in the input stream as character. This function will increment the m_textIndex by 1
	*/
	QChar nextSymbol();

	/**
	 \brief This function will return the current state of Scanner.
	*/
	scannerMode currentState();
	scannerMode previousState();
	/**
	\brief This function will switch the state of Scanner into textMode or tagMode or styleMode.
	*/
	void enterState(scannerMode Mode);

	/**
	\brief This function will return a token which is to be evaluated in parser. Each token will be available in the class member token as a QString.
	*/
	QString getToken();
	/**
	 \brief Function to Slice the string so as to remove the inch character etc
	 */
	QString sliceString();
	/**
	 \name applyFeature
	 \brief Function to applyFeature to a character Style
	 */
	void applyFeature(StyleFlagValue feature);
	/**
	 \brief Function which will empty the m_textToAppend variable by writing into text frame
	 */
	void flushText();
	/**
	 \brief Function which returns the status of Style whether defined or not
	 */
	bool styleStatus(QStringList &name,QString &sfcname);

	QString getFontName(const QString& name);

	/** Set Functions for setting the styles */
	void setPlain();
	void setBold();
	void setItalics();
	void setShadow();
	void setOutline();
	void setUnderline();
	void setWordUnder();
	void setStrikethrough();
	void setDoubleStrike();
	void setAllCaps();
	void setSmallCaps();
	void setSuperscript();
	void setSubscript();
	void setSuperior();
	void setFont();
	void setFontSize();
	void setColor();
	void setShade();
	void setHorizontalScale();
	void setKern();
	void setTrack();
	void setBaseLineShift();
	void setVerticalScale();
	void setLigatures();
	void csetOpacity();
	void setOpenType();
	void setCharAlignment();
	void setVertStories();
	void setEmphasisMarks();
	void setAsncjk();
	void setLanguages();
	void setXPresOwn();
	void setAlignLeft();
	void setAlignCenter();
	void setAlignRight();
	void setAlignJustify();
	void setAlignFJustify();
	void setTabStops();
	void setPAttributes();
	void setHyphenation();
	void setPRuleAbove();
	void setPRuleBelow();
	void setDropCap();
	void setKeepNextPar();
	void setKeepTogether();
	void setHangingCSet();
	void setGlyph();
	void setTranscodeSeq();
	void setUnencodedGlyph();
	void xFlag();
	void definePStyles();
	void defineCStyle();
	void applyCStyle1();
	void applyCStyle2();
	void applyCStyle3();
	void applyCStyle4();
	void applyCStyle5();
	void applyCStyle6();
	void setTypeStyle();
	void appendSpChar1();
	void appendSpChar2();
	void appendSpChar3();
	void setEncoding();
	/**
	 Functions used in textMode Hash
	 */
	void defNewLine();
	void defHardReturn();
	void defFontSet();
	void defColor();
	void defEquals();
	void defClose();
	void defOpen();
	void defAtRate();
	void defColon();

protected:
	void (XtgScanner::*funPointer)(void);

private:
	/**
	 \variable Variables based on which scanner works in different modes
	 */
	scannerMode m_mode;
	scannerMode m_prevMode;
	/**
	 \variable Variables of the importer
	 */
	PageItem* m_item { nullptr };
	bool m_importTextOnly { false};
	bool m_prefixName { false};
	bool m_append { false};
	/**
	 \variable Flag variables used in the scanner
	 */
	bool m_newlineFlag { false };
	bool m_xflag { false };
	bool m_inDef { false };
	 /**
	 \variable Input buffer to which properly encoded file is loaded
	 */
	QByteArray m_inputBuffer;
	int m_bufferIndex { 0 };

	QString m_decodedText;
	int m_textIndex { 0 };

	ScribusDoc* m_doc { nullptr };
	/**
	 \variable current Character and paragraph styles
	 */
	CharStyle m_currentCharStyle;
	ParagraphStyle m_currentParagraphStyle;
	StyleFlag m_styleEffects;

	/** To store unsupported attributes */
	QSet<QString> m_unsupported;
	/** 
	\brief m_textToAppend will be the QString used by the function TextWriter::append(QString& )
	*/ 
	QString m_textToAppend;
	QString m_token;
	QString m_sfcName; // Name of Style/Fontset/Color to be defined, hence named m_sfcName

	QHash<QString,void (XtgScanner::*)(void)> m_tagModeHash;
	QHash<QString,void (XtgScanner::*)(void)> m_textModeHash;
	QHash<QString,void (XtgScanner::*)(void)> m_nameModeHash;
	QHash<int,QString> languages;

	/** m_define variable will take the following values : 
	 \brief
		0	Not a definition
		1	Character Stylesheet Definition
		2	Paragraph Stylesheet Definition
	 */
	int  m_define { 0 };
	bool m_isBold { false };
	bool m_isItalic { false };

	QTextDecoder *m_decoder { nullptr };

	/**
	 \brief Decode text from input buffer until specified index
	 */
	bool decodeText(int index);
};
	
#endif
