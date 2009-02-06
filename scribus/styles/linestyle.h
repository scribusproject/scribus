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


#ifndef LINESTYLE_H
#define LINESTYLE_H

#include <QString>
#include "style.h"
#include "styles/stylecontextproxy.h"

class SCRIBUS_API LineStyle : public Style {
public:

	
    LineStyle() : Style(), lineStyleProxy(this) {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		m_##attr_NAME = attr_DEFAULT; \
		inh_##attr_NAME = true;
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
    };
	
    LineStyle(qreal width, const QString& color, qreal shade=100) : Style(), lineStyleProxy(this)  {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		m_##attr_NAME = attr_DEFAULT; \
		inh_##attr_NAME = true;
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
		setWidth(width);
		setColor(color);
		setShade(shade);
    };
	
	LineStyle(const LineStyle & other);
	
	LineStyle & operator=(const LineStyle & other);
	
	static const Xml_string saxxDefaultElem;
	static void  desaxeRules(const Xml_string& prefixPattern, desaxe::Digester& ruleset, Xml_string elemtag = saxxDefaultElem);
	
	virtual void saxx(SaxHandler& handler, const Xml_string& elemtag) const;
	virtual void saxx(SaxHandler& handler)                     const { saxx(handler, saxxDefaultElem); }
	
	void getNamedResources(ResourceCollection& lists) const;
	void replaceNamedResources(ResourceCollection& newNames);

	QString displayName() const;

	void update(const StyleContext * b);
	
	bool equiv(const Style& other) const;	
	
	void applyLineStyle(const LineStyle & other);
	void eraseLineStyle(const LineStyle & other);
	void setStyle(const LineStyle & other);
	void erase() { eraseLineStyle(*this); }
	
	QString asString() const;
	
	/** getter: validates and returns the attribute's value */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	attr_TYPE attr_GETTER() const { validate(); return m_##attr_NAME; }
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	
	/** setter: sets the attribute's value and clears inherited flag */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	void set##attr_NAME(attr_TYPE v) { m_##attr_NAME = v; inh_##attr_NAME = false; }
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	void appendSubline(const LineStyle& subline) { validate(); m_Sublines.append(subline); inh_Sublines = false; }
	
	/** setter: resets the attribute's value and sets inherited flag */
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	void reset##attr_NAME() { m_##attr_NAME = attr_DEFAULT; inh_##attr_NAME = true; }
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	
	/** isInherited: returns true if the attribute is inherited */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	bool isInh##attr_NAME() const { return inh_##attr_NAME; }
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	
	
	/** isDefined: returns true if the attribute is defined in this style or any parent */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	bool isDef##attr_NAME() const { \
		if ( !inh_##attr_NAME ) return true; \
		const LineStyle * par = dynamic_cast<const LineStyle*>(parentStyle()); \
		return par && par->isDef##attr_NAME(); \
	}
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	
	
private:

	StyleContextProxy lineStyleProxy;
	// FIXME: see pstyle how this works for nested styles
	
	
	// member declarations:
		
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	attr_TYPE m_##attr_NAME; \
	bool inh_##attr_NAME;
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
};


inline LineStyle & LineStyle::operator=(const LineStyle & other)
{
	static_cast<Style&>(*this) = static_cast<const Style&>(other);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	m_contextversion = -1;
	return *this;
}

inline LineStyle::LineStyle(const LineStyle & other) : Style(other), lineStyleProxy(this)
{
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	m_contextversion = -1;
}

#endif
