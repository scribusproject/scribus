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

class SCRIBUS_API ParagraphStyle : public Style
{
public:
	enum LineSpacingMode { 
		FixedLineSpacing        = 0, 
		AutomaticLineSpacing    = 1,
		BaselineGridLineSpacing = 2
	};
	enum AlignmentType {
		Leftaligned  = 0,
		Centered     = 1,
		Rightaligned = 2,
		Justified    = 3,
		Extended     = 4
	};
	struct TabRecord
	{
		double tabPosition;
		int tabType;
		QChar tabFillChar;
		bool operator==(const TabRecord& other) const {
			return tabPosition==other.tabPosition && tabType==other.tabType && tabFillChar == other.tabFillChar;
		}
		bool operator<(const TabRecord& other)  const { return tabPosition < other.tabPosition; }
		bool operator<=(const TabRecord& other) const { return tabPosition <= other.tabPosition; }
		bool operator>(const TabRecord& other)  const { return tabPosition > other.tabPosition; }
		bool operator>=(const TabRecord& other) const { return tabPosition >= other.tabPosition; }
	};

	ParagraphStyle();
	ParagraphStyle(const ParagraphStyle& other);
	ParagraphStyle& operator=(const ParagraphStyle& other);
	~ParagraphStyle();

	QString displayName() const;

	void update(const StyleBase*);
	
	bool equiv(const Style& other) const;
	
	void applyStyle(const ParagraphStyle& other);
	void eraseStyle(const ParagraphStyle& other);
	void erase() { eraseStyle(*this); }	

	const StyleBase* charStyleBase() const { return & cstyleBase; }
	CharStyle & charStyle() { return cstyle; }
	const CharStyle& charStyle() const { return cstyle; }
	
	
	/** getter: validates and returns the attribute's value */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	const attr_TYPE attr_GETTER() const { validate(); return m_##attr_NAME; }
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	
	/** setter: sets the attribute's value and clears inherited flag */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	void set##attr_NAME(attr_TYPE v) { m_##attr_NAME = v; inh_##attr_NAME = false; }
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	
	/** setter: resets the attribute's value and sets inherited flag */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	void reset##attr_NAME() { m_##attr_NAME = attr_DEFAULT; inh_##attr_NAME = true; }
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	
	/** isInherited: returns true if the attriute is inherited */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	bool isInh##attr_NAME() const { return inh_##attr_NAME; }
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	
	
	/** isDefined: returns true if the attribute is defined in this style or any parent */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	bool isDef##attr_NAME() const { \
		if ( !inh_##attr_NAME ) return true; \
		const ParagraphStyle * par = dynamic_cast<const ParagraphStyle*>(parentStyle()); \
		return par && par->isDef##attr_NAME(); \
	}
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	
	
private:
		
		// member declarations:
		StyleBaseProxy cstyleBase;
		CharStyle cstyle;
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		attr_TYPE m_##attr_NAME; \
			bool inh_##attr_NAME;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
		
};
