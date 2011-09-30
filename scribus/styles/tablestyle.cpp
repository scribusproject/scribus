/*
 Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include "commonstrings.h"
#include "desaxe/saxiohelper.h"
#include "util_math.h"
#include "tableborder.h"
#include "tablestyle.h"

QString TableStyle::displayName() const
{
	if (isDefaultStyle())
		return CommonStrings::trDefaultTableStyle;
	if (hasName() || !hasParent() || !m_context)
		return name();
	else 
		return parentStyle()->displayName() + "+";
}

bool TableStyle::equiv(const Style& other) const
{
	other.validate();
	const TableStyle *p_other = dynamic_cast<const TableStyle*> (&other);
	return p_other &&
		parent() == p_other->parent() 
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		&& (inh_##attr_NAME == p_other->inh_##attr_NAME) \
		&& (inh_##attr_NAME || isequiv(m_##attr_NAME, p_other->m_##attr_NAME))
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
		;
}

void TableStyle::erase()
{
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME) \
		reset##attr_NAME();
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	//updateFeatures(); TODO: Investigate this.
}

void TableStyle::update(const StyleContext* context)
{
	Style::update(context);
	const TableStyle* parent = dynamic_cast<const TableStyle*>(parentStyle());
	if (parent) {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		if (inh_##attr_NAME) \
			m_##attr_NAME = parent->attr_GETTER();
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	}
	//updateFeatures(); TODO: Investigate this.
}

void TableStyle::getNamedResources(ResourceCollection& lists) const
{
	for (const Style* style = parentStyle(); style != NULL; style = style->parentStyle())
		lists.collectCellStyle(style->name());
	lists.collectColor(fillColor());
	// TODO: Collect colors of borders.
}

void TableStyle::replaceNamedResources(ResourceCollection& newNames)
{
	QMap<QString, QString>::ConstIterator it;

	if (!isInhFillColor() && (it = newNames.colors().find(fillColor())) != newNames.colors().end())
		setFillColor(it.value());

	// TODO: Do we need to do something else? E.g. CharStyle calls its updateFeatures().
}

void TableStyle::saxx(SaxHandler& handler, const Xml_string& elemtag) const
{
	/*
	Xml_attr att;
	Style::saxxAttributes(att);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME) \
		att.insert(# attr_NAME, toXMLString(m_##attr_NAME));
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
	if (!name().isEmpty())
		att["id"] = mkXMLName(elemtag + name());
	handler.begin(elemtag, att);
//	if (hasParent() && parentStyle())
//		parentStyle()->saxx(handler);
	handler.end(elemtag);
	*/
}

using namespace desaxe;

const Xml_string TableStyle::saxxDefaultElem("tablestyle");

void TableStyle::desaxeRules(const Xml_string& prefixPattern, Digester& ruleset, Xml_string elemtag)
{
	/*
	Xml_string stylePrefix(Digester::concat(prefixPattern, elemtag));
	ruleset.addRule(stylePrefix, Factory<TableStyle>());
	ruleset.addRule(stylePrefix, IdRef<TableStyle>());
	Style::desaxeRules<TableStyle>(prefixPattern, ruleset, elemtag);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	ruleset.addRule(stylePrefix, SetAttributeWithConversion<TableStyle, attr_TYPE> ( & TableStyle::set##attr_NAME,  # attr_NAME, &parse<attr_TYPE> ));
#include "tablestyle.attrdefs.cxx"
#undef ATTRDEF
*/
}

