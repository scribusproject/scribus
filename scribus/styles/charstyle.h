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


// DONT INCLUDE this file directly, include "style.h" instead!


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

	
    CharStyle() : Style() {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		m_##attr_NAME = attr_DEFAULT; \
		inh_##attr_NAME = true;
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
    };
	
    CharStyle(const ScFace& font, int size, StyleFlag style = ScStyle_Default) : Style() {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		m_##attr_NAME = attr_DEFAULT; \
		inh_##attr_NAME = true;
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
		m_Font = font;
        m_FontSize = size;
		m_Effects = style;
    };
	
	CharStyle(const CharStyle & other);
	
	CharStyle & operator=(const CharStyle & other);
	
	QString displayName() const;

	void update(const StyleBase * b);
	
	bool equiv(const Style& other) const;	
	
	void applyCharStyle(const CharStyle & other);
	void eraseCharStyle(const CharStyle & other);
	void erase() { eraseCharStyle(*this); }
	
	QString asString() const;
	
	/** getter: validates and returns the attribute's value */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	const attr_TYPE attr_GETTER() const { validate(); return m_##attr_NAME; }
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
	
	/** setter: sets the attribute's value and clears inherited flag */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	void set##attr_NAME(attr_TYPE v) { m_##attr_NAME = v; inh_##attr_NAME = false; }
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
	
	/** setter: resets the attribute's value and sets inherited flag */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	void reset##attr_NAME() { m_##attr_NAME = attr_DEFAULT; inh_##attr_NAME = true; }
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
	
	/** isInherited: returns true if the attribute is inherited */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	bool isInh##attr_NAME() const { return inh_##attr_NAME; }
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
	
	
	/** isDefined: returns true if the attribute is defined in this style or any parent */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	bool isDef##attr_NAME() const { \
		if ( !inh_##attr_NAME ) return true; \
		const CharStyle * par = dynamic_cast<const CharStyle*>(parentStyle()); \
		return par && par->isDef##attr_NAME(); \
	}
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
	
	
private:

	// member declarations:
		
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	attr_TYPE m_##attr_NAME; \
	bool inh_##attr_NAME;
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
};


inline CharStyle & CharStyle::operator=(const CharStyle & other)
{
	other.validate();
	static_cast<Style&>(*this) = static_cast<const Style&>(other);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
	return *this;
}

inline CharStyle::CharStyle(const CharStyle & other) : Style(other) 
{
	other.validate();
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
}

