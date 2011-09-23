/*
 Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

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

#ifndef TABLESTYLE_H
#define TABLESTYLE_H

#include "style.h"
#include "commonstrings.h"
#include "resourcecollection.h"
#include "styles/stylecontextproxy.h"
#include "tableborder.h"

/**
 * The TableStyle class represents a style for a table.
 */
class SCRIBUS_API TableStyle : public Style {
public:
	/**
	 * Constructs a new table style with all attributes inherited.
	 */
	TableStyle() : Style(), tableStyleProxy(this) {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		m_##attr_NAME = attr_DEFAULT; \
		inh_##attr_NAME = true;
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
    };

	/**
	 * Constructs a new table style with values and inheritance states of the
	 * attributes set to match those of another table style
	 *
	 * @param other the other table style.
	 */
	TableStyle(const TableStyle& other);

	/**
	 * Sets values and inheritance states of attributes to match those of another
	 * table style.
	 *
	 * @param other the other table style.
	 * @return this table style.
	 */
	TableStyle& operator=(const TableStyle& other);

	/// Not implemented.
	virtual void saxx(SaxHandler& handler, const Xml_string& elemtag) const {};

	/// Not implemented.
	virtual void saxx(SaxHandler& handler) const {};

	/**
	 * Returns the display name of the table style.
	 *
	 * @param display display name of the table style.
	 */
	QString displayName() const;

	/**
	 * Returns true if all attributes values and inheritance states of this
	 * table style matches those of another table style.
	 *
	 * @param other the other table style.
	 */
	bool equiv(const Style& other) const;

	/**
	 * Resets all non-inherited attributes in this style to their default values.
	 */
	void erase();

	/**
	 * Sets the style context to @a context and update locally cached property values.
	 */
	void update(const StyleContext* context);

	/**
	 * Collects named resources used by the table style in @a lists.
	 */
	void getNamedResources(ResourceCollection& lists) const;

	/**
	 * Replaces named resources used by the table style with names in @a newNames.
	 */
	void replaceNamedResources(ResourceCollection& newNames);

	/**
	 * Gets the value of the attribute.
	 *
	 * @return value of the attribute.
	 */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	const attr_TYPE &attr_GETTER() const { validate(); return m_##attr_NAME; }
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	
	/**
	 * Sets the value of the attribute in this style and disinherits it.
	 *
	 * @param v value of the attribute.
	 */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	void set##attr_NAME(attr_TYPE v) { m_##attr_NAME = v; inh_##attr_NAME = false; }
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	
	/**
	 * Resets the attribute to its default value in this style and makes it
	 * inherited.
	 */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	void reset##attr_NAME() { m_##attr_NAME = attr_DEFAULT; inh_##attr_NAME = true; }
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	
	/**
	 * Returns true if the attribute is inherited.
	 *
	 * @return true if the attribute is inherited, otherwise false.
	 */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	bool isInh##attr_NAME() const { return inh_##attr_NAME; }
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	
	/**
	 * Returns true if the attribute is defined in this style or any ancestor
	 * style.
	 *
	 * @return true if the attribute is defined, otherwise false.
	 */
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	bool isDef##attr_NAME() const { \
		if (!inh_##attr_NAME) return true; \
		const TableStyle *parent = dynamic_cast<const TableStyle*>(parentStyle()); \
		return parent && parent->isDef##attr_NAME(); \
	}
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	
private:
	StyleContextProxy tableStyleProxy;

	// Attribute related member declarations.
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	attr_TYPE m_##attr_NAME; \
	bool inh_##attr_NAME;
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
};

inline TableStyle& TableStyle::operator=(const TableStyle& other)
{
	static_cast<Style&>(*this) = static_cast<const Style&>(other);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	m_contextversion = -1;
	return *this;
}

inline TableStyle::TableStyle(const TableStyle& other) : Style(other), tableStyleProxy(this)
{
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	m_contextversion = -1;
}

#endif // TABLESTYLE_H
