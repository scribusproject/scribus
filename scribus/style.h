/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#ifndef STYLE_H
#define STYLE_H

#include <qstring.h>
#include "scribusapi.h"


/**
 *  This as a virtual base class for all style-like objects: CharStyles, 
 *  ParagraphStyles(indirectly), LineStyles, FrameStyles, CellStyles,
 *  FlowStyles,...
 *  It provides a name and an inheritance mechanism.
 */
class Style {
private:
	QString name_;
	const Style*  parent_;
public:
	static const short NOVALUE = -16000;

	Style(): name_(""),parent_(NULL)  {}
    Style(QString n): name_(n), parent_(NULL)  {}
	Style& operator=(const Style& o) 
	{ //assert(typeinfo() == o.typeinfo()); 
		name_ = o.name_; parent_ = o.parent_; return *this;
	}
	Style(const Style& o) : name_(o.name_), parent_(o.parent_) {} 
	
	QString name() const             { return name_; }
	void setName(QString n)          { name_ = n; }
	const Style* parent() const      { return parent_; }
	void setParent(const Style* p)   { parent_ = p; }
	virtual ~Style()                 {}
	// applyStyle(const SubStyle& other)
	// eraseStyle(const SubStyle& other)
	// assign(const SubStyle& other)
};




enum StyleFlag {
	ScStyle_Default       = 0,
    ScStyle_Superscript   = 1,
    ScStyle_Subscript     = 2,
    ScStyle_Outline       = 4,
    ScStyle_Underline     = 8,
    ScStyle_Strikethrough = 16,
    ScStyle_AllCaps       = 32,
    ScStyle_SmallCaps     = 64,
    ScStyle_HyphenationPossible=128, //Hyphenation possible here (Smart Hyphen)
    ScStyle_Shadowed      = 256,
    ScStyle_UnderlineWords= 512,
    ScStyle_Reserved01    = 1024, //free, not used in the moment
    ScStyle_DropCap       = 2048,
    ScStyle_SuppressSpace = 4096,//internal use in PageItem (Suppresses spaces when in Block alignment)
    ScStyle_SmartHyphenVisible=8192, //Smart Hyphen visible at line end
    ScStyle_StartOfLine   = 16384,
	ScStyle_UserStyles    = 2047, // 1919, // == 1024 + 512 + 256 + 128 + 64 + 32 + 16 + 8 + 4 + 2 + 1
	ScStyle_None          = 65535
};

SCRIBUS_API StyleFlag& operator&= (StyleFlag& left, StyleFlag right);

SCRIBUS_API StyleFlag& operator|= (StyleFlag& left, StyleFlag right);

SCRIBUS_API StyleFlag operator& (StyleFlag left, StyleFlag right);

SCRIBUS_API StyleFlag operator| (StyleFlag left, StyleFlag right);

SCRIBUS_API StyleFlag operator^ (StyleFlag left, StyleFlag right);

SCRIBUS_API StyleFlag operator~ (StyleFlag arg);




class SCRIBUS_API CharStyle : public virtual Style {
public:
	static const QString NOCOLOR;
	static const QString NOLANG;
	
    CharStyle() : Style() {
        csize = NOVALUE;
        cshade = NOVALUE;
        cshade2 = NOVALUE;
        cstyle = ScStyle_None;
        cscale = NOVALUE;
        cscalev = NOVALUE;
        cbase = NOVALUE;
        cshadowx = NOVALUE;
        cshadowy = NOVALUE;
        coutline = NOVALUE;
        cunderpos = NOVALUE;
        cunderwidth = NOVALUE;
        cstrikepos = NOVALUE;
        cstrikewidth = NOVALUE;
        cextra = NOVALUE;
		
        cfont = const_cast<Foi*>(&Foi::NONE); 
        ccolor = NOCOLOR;
        cstroke = NOCOLOR;
		language_ = NOLANG;
    };
	
    CharStyle(Foi * font, int size, StyleFlag style = ScStyle_Default) : Style() {
        csize = size;
        cshade = 1;
        cshade2 = 1;
        cstyle = style;
        cscale = 1000;
        cscalev = 1000;
        cbase = 0;
        cshadowx = 0;
        cshadowy = 0;
        coutline = 0;
        cunderpos = 0;
        cunderwidth = 0;
        cstrikepos = 0;
        cstrikewidth = 0;
        cextra = 0;
		
        cfont = font; 
        ccolor = "Black";
        cstroke = "Black";
		language_ = "";
    };
	
	CharStyle(const CharStyle & other);
	
	bool operator==(const CharStyle & other) const;
	bool operator!=(const CharStyle & other) const { return ! (*this==other); }
	CharStyle & operator=(const CharStyle & other);
	
	void applyCharStyle(const CharStyle & other);
	void eraseCharStyle(const CharStyle & other);
	
	QString asString() const;
	
	int fontSize() const { return csize==NOVALUE && parent()? inh().fontSize() : csize; }
	int fillShade() const { return cshade==NOVALUE && parent()? inh().fillShade() : cshade; }
	int strokeShade() const { return cshade2==NOVALUE && parent()? inh().strokeShade() : cshade2; }
	StyleFlag effects() const { return cstyle==ScStyle_None && parent()? inh().effects() : cstyle; }
	int scaleH() const { return cscale==NOVALUE && parent()? inh().scaleH() : cscale; }
	int scaleV() const { return cscalev==NOVALUE && parent()? inh().scaleV() : cscalev; }
	int baselineOffset() const { return cbase==NOVALUE && parent()? inh().baselineOffset() : cbase; }
	int shadowXOffset() const { return cshadowx==NOVALUE && parent()? inh().shadowXOffset() : cshadowx; }
	int shadowYOffset() const { return cshadowy==NOVALUE && parent()? inh().shadowYOffset() : cshadowy; }
	int outlineWidth() const { return coutline==NOVALUE && parent()? inh().outlineWidth() : coutline; }
	int underlineOffset() const { return cunderpos==NOVALUE && parent()? inh().underlineOffset() : cunderpos; }
	int underlineWidth() const { return cunderwidth==NOVALUE && parent()? inh().underlineWidth() : cunderwidth; }
	int strikethruOffset() const { return cstrikepos==NOVALUE && parent()? inh().strikethruOffset() : cstrikepos; }
	int strikethruWidth() const { return cstrikewidth==NOVALUE && parent()? inh().strikethruWidth() : cstrikewidth; }
	int tracking() const { return cextra==NOVALUE && parent()? inh().tracking() : cextra; }
	
	QString fillColor() const { return ccolor==NOCOLOR && parent()? inh().fillColor() : ccolor; }
	QString strokeColor() const { return cstroke==NOCOLOR && parent()? inh().strokeColor() : cstroke; }
	QString language() const { return language_==NOLANG && parent()? inh().language() : language_; }
	
	Foi* font() const { return cfont==&Foi::NONE && parent()? inh().font() : cfont; } 
	
	void setFontSize(int s) { csize = s; }
	void setFillShade(int s) { cshade = s; }
	void setStrokeShade(int s) { cshade2 = s; }
	void setEffects(StyleFlag e) { cstyle = e; }
	void setScaleH(int s) { cscale = s; }
	void setScaleV(int s) { cscalev = s; }
	void setBaselineOffset(int o) { cbase = o; }
	void setShadowXOffset(int o) { cshadowx = o; }
	void setShadowYOffset(int o) { cshadowy = o; }
	void setOutlineWidth(int w) { coutline = w; }
	void setUnderlineOffset(int o) { cunderpos = o; }
	void setUnderlineWidth(int w) { cunderwidth = w; }
	void setStrikethruOffset(int o) { cstrikepos = o; }
	void setStrikethruWidth(int w) { cstrikewidth = w; }
	void setTracking(int t) { cextra = t; }
	
	void setFillColor(QString c) { ccolor = c; }
	void setStrokeColor(QString c) { cstroke = c; }
	void setLanguage(QString l) { language_ = l; }
	
	void setFont(Foi* f) { cfont = f; } 
	
	
	
private:
	// shorthand:
	const CharStyle& inh() const { return *dynamic_cast<const CharStyle*>(parent()); };
    int csize;
    short cshade;
    short cshade2;
    StyleFlag cstyle;
    short cscale;
    short cscalev;
    short cbase;
    short cshadowx;
    short cshadowy;
    short coutline;
    short cunderpos;
    short cunderwidth;
    short cstrikepos;
    short cstrikewidth;
    short cextra;
	
    Foi* cfont;
    QString ccolor;
    QString cstroke;
    QString language_;
	
};

inline bool CharStyle::operator==(const CharStyle & other) const
{
	return  (csize == other.csize &&
			 cshade == other.cshade &&
			 cshade2 == other.cshade2 &&
			 cstyle == other.cstyle &&
			 cscale == other.cscale &&
			 cscalev == other.cscalev &&
			 cbase == other.cbase &&
			 cshadowx == other.cshadowx &&
			 cshadowy == other.cshadowy &&
			 coutline == other.coutline &&
			 cunderpos == other.cunderpos &&
			 cunderwidth == other.cunderwidth &&
			 cstrikepos == other.cstrikepos &&
			 cstrikewidth == other.cstrikewidth &&
			 cextra == other.cextra &&
			 cfont == other.cfont &&
			 ccolor == other.ccolor &&
			 cstroke == other.cstroke &&	
			 language_ == other.language_ );	
}

inline CharStyle & CharStyle::operator=(const CharStyle & other)
{
	static_cast<Style&>(*this) = static_cast<const Style&>(other);
	csize = other.csize;
	cshade = other.cshade;
	cshade2 = other.cshade2;
	cstyle = other.cstyle;
	cscale = other.cscale;
	cscalev = other.cscalev;
	cbase = other.cbase;
	cshadowx = other.cshadowx;
	cshadowy = other.cshadowy;
	coutline = other.coutline;
	cunderpos = other.cunderpos;
	cunderwidth = other.cunderwidth;
	cstrikepos = other.cstrikepos;
	cstrikewidth = other.cstrikewidth;
	cextra = other.cextra;
	cfont = other.cfont;
	ccolor = other.ccolor;
	cstroke = other.cstroke;
	language_ = other.language_;
	return *this;
}

inline CharStyle::CharStyle(const CharStyle & other) : Style(other)
{
	csize = other.csize;
	cshade = other.cshade;
	cshade2 = other.cshade2;
	cstyle = other.cstyle;
	cscale = other.cscale;
	cscalev = other.cscalev;
	cbase = other.cbase;
	cshadowx = other.cshadowx;
	cshadowy = other.cshadowy;
	coutline = other.coutline;
	cunderpos = other.cunderpos;
	cunderwidth = other.cunderwidth;
	cstrikepos = other.cstrikepos;
	cstrikewidth = other.cstrikewidth;
	cextra = other.cextra;
	cfont = other.cfont;
	ccolor = other.ccolor;
	cstroke = other.cstroke;
	language_ = other.language_;
}


class SCRIBUS_API ParagraphStyle : public virtual Style
{
public:
	enum LineSpacingMode { 
		FixedLineSpacing        = 0, 
		AutomaticLineSpacing    = 1,
		BaselineGridLineSpacing = 2
	};
	struct TabRecord
	{
		double tabPosition;
		int tabType;
		QChar tabFillChar;
	};
	
private:
	// shorthand
	const ParagraphStyle& inh() const { return *dynamic_cast<const ParagraphStyle*>(parent()); }
	CharStyle cstyle;
	LineSpacingMode LineSpaMode;
	double LineSpa;
	int textAlignment;
	double Indent;
	double rightMargin_;
	double First;
	double gapBefore_;
	double gapAfter_;
	QValueList<TabRecord> TabValues;
	bool haveTabs;
	int Drop;
	int DropLin;
	double DropDist;
	int BaseAdj;
	
public:
	ParagraphStyle();
	ParagraphStyle(const ParagraphStyle& other);
	int lineSpacingMode() const { return LineSpaMode==NOVALUE && parent()? inh().lineSpacingMode() : LineSpaMode; }
	double lineSpacing() const { return LineSpa<=NOVALUE && parent()? inh().lineSpacing() : LineSpa; }
	int alignment() const { return textAlignment==NOVALUE && parent()? inh().alignment() : textAlignment; }
	double firstIndent() const { return First<=NOVALUE && parent()? inh().firstIndent() : First; }
	double leftMargin() const { return Indent<=NOVALUE && parent()? inh().leftMargin() : Indent; }
	double rightMargin() const { return rightMargin_<=NOVALUE && parent()? inh().rightMargin() : rightMargin_; }
	double gapBefore() const { return gapBefore_<=NOVALUE && parent()? inh().gapBefore() : gapBefore_; }
	double gapAfter() const { return gapAfter_<=NOVALUE && parent()? inh().gapAfter() : gapAfter_; }
	bool hasDropCap() const { return Drop==NOVALUE && parent()? inh().hasDropCap() : Drop > 0; }
	int dropCapLines() const { return DropLin==NOVALUE && parent()? inh().dropCapLines() : DropLin; }
	double dropCapOffset() const { return DropDist<=NOVALUE && parent()? inh().dropCapOffset() : DropDist; }
	bool useBaselineGrid() const { return BaseAdj==NOVALUE && parent()? inh().useBaselineGrid() : BaseAdj > 0; }
	
	void setLineSpacingMode(LineSpacingMode p) { 
		LineSpaMode = p; 
	}
	void setLineSpacing(double p) { 
		LineSpa = p; 
	}
	void setAlignment(int p) { 
		textAlignment = p; 
	}
	void setFirstIndent(double p) { 
		First = p; 
	}
	void setLeftMargin(double p) { 
		Indent = p; 
	}
	void setRightMargin(double p) { 
		rightMargin_ = p; 
	}
	void setGapBefore(double p) {
		gapBefore_ = p;
	}
	void setGapAfter(double p) {
		gapAfter_ = p;
	}
	void setHasDropCap(bool p) { 
		Drop = p? 1 : 0; 
	}
	void setDropCapLines(int p) { 
		DropLin = p; 
	}
	void setDropCapOffset(double p) { 
		DropDist = p; 
	}
	
	void setUseBaselineGrid(bool p) { 
		BaseAdj = p? 1 : 0; 
	}
	
	// these return writeable references for now:
	QValueList<TabRecord> & tabValues() { haveTabs = true; return TabValues; }
	const QValueList<TabRecord> & tabValues() const { return !haveTabs && parent()? inh().tabValues() : TabValues; }

	CharStyle & charStyle() { return cstyle; }
	const CharStyle& charStyle() const { return cstyle; }
	
	void applyStyle(const ParagraphStyle& other);
	void eraseStyle(const ParagraphStyle& other);
	ParagraphStyle& operator=(const ParagraphStyle& other);

	bool equiv(const ParagraphStyle& other) const;
	
	bool operator==(const ParagraphStyle& other) const
	{ 
		return name()==other.name() && equiv(other);
	}
};

#endif
