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

#ifndef PARAGRAPHSTYLE_H
#define PARAGRAPHSTYLE_H

#include "style.h"
#include "charstyle.h"
#include "styles/stylecontextproxy.h"


class ResourceCollection;


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
	enum OpticalMarginType {
		OM_None  = 0,
		OM_LeftProtruding    = 1,
		OM_RightProtruding   = 2,
		OM_LeftHangingPunct  = 4,
		OM_RightHangingPunct = 8,
		OM_Default           = OM_RightProtruding + OM_LeftHangingPunct + OM_RightHangingPunct
	};
	enum HyphenationMode {
		NoHyphenation        = 0,
		ManualHyphenation    = 1,
		AutomaticHyphenation = 2
	};
	struct TabRecord
	{
		qreal tabPosition;
		int tabType;
		QChar tabFillChar;
		bool operator==(const TabRecord& other) const;
		bool operator<(const TabRecord& other)  const { return tabPosition < other.tabPosition; }
		bool operator<=(const TabRecord& other) const { return tabPosition <= other.tabPosition; }
		bool operator>(const TabRecord& other)  const { return tabPosition > other.tabPosition; }
		bool operator>=(const TabRecord& other) const { return tabPosition >= other.tabPosition; }
	};

	ParagraphStyle();
	ParagraphStyle(const ParagraphStyle& other);
	ParagraphStyle& operator=(const ParagraphStyle& other);
	~ParagraphStyle();

	static const Xml_string saxxDefaultElem;
	static void  desaxeRules(const Xml_string& prefixPattern, desaxe::Digester& ruleset, Xml_string elemtag = saxxDefaultElem);
	
	virtual void saxx(SaxHandler& handler, const Xml_string& elemtag) const;
	virtual void saxx(SaxHandler& handler)                     const { saxx(handler, saxxDefaultElem); }
	

	void getNamedResources(ResourceCollection& lists) const;
	void replaceNamedResources(ResourceCollection& newNames);

	QString displayName() const;

	void setContext(const StyleContext* context);
	void update(const StyleContext*);
	
	bool equiv(const Style& other) const;
	
	void applyStyle(const ParagraphStyle& other);
	void eraseStyle(const ParagraphStyle& other);
	void setStyle(const ParagraphStyle& other);
	void erase() { eraseStyle(*this); }

	StyleContext* charStyleContext() { return & cstyleContext; }
	const StyleContext* charStyleContext() const { return & cstyleContext; }
	CharStyle & charStyle() { return cstyle; }
	const CharStyle& charStyle() const { return cstyle; }
	/** Normally the context for charStyle() is parentStyle()->charStyleContext()
		Use this method to break that relation and set charStyle()'s context manually
	*/
	void breakImplicitCharStyleInheritance(bool val = true);
	/** used internally to establish the implicit relation 
		charStyle().context() == parentStyle()->charStyleContext()
		Done implicitly in setContext(), update(), operator= and breakImplicitCharStyleInheritance()
	*/
	void repairImplicitCharStyleInheritance();
	
	/** getter: validates and returns the attribute's value */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	attr_TYPE attr_GETTER() const { validate(); return m_##attr_NAME; }
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	
	/** setter: sets the attribute's value and clears inherited flag */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	void set##attr_NAME(attr_TYPE v) { m_##attr_NAME = v; inh_##attr_NAME = false; }
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	void appendTabValue(const TabRecord& tab) { validate(); m_TabValues.append(tab); inh_TabValues = false; }
	
	
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
	StyleContextProxy cstyleContext;
	bool cstyleContextIsInh;
	CharStyle cstyle;
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		attr_TYPE m_##attr_NAME; \
			bool inh_##attr_NAME;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
		
};

#endif
