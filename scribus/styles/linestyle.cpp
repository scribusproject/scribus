/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */


#include <QObject>
#include "sctextstruct.h"
#include "scfonts.h"
#include "resourcecollection.h"

#include "styles/style.h"
#include "linestyle.h"
#include "desaxe/saxiohelper.h"
#include "desaxe/simple_actions.h"
#include "prefsmanager.h"
#include "util_math.h"


void LineStyle::applyLineStyle(const LineStyle & other)
{
	Style::applyStyle(other);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (! other.inh_##attr_NAME) \
		set##attr_NAME(other.m_##attr_NAME);
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
}


void LineStyle::eraseLineStyle(const LineStyle & other)
{
	other.validate();
	Style::eraseStyle(other);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME && m_##attr_NAME == other.m_##attr_NAME) \
		reset##attr_NAME();
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
}

bool LineStyle::equiv(const Style & other) const
{
	other.validate();
	const LineStyle * oth = dynamic_cast<const LineStyle*> ( & other );
	return  oth &&
		parent() == oth->parent() 
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		&& (inh_##attr_NAME == oth->inh_##attr_NAME) \
		&& (inh_##attr_NAME || isequiv(m_##attr_NAME, oth->m_##attr_NAME))
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
		;	
}


QString LineStyle::displayName() const
{
	if ( hasName() || !hasParent() || ! m_context)
		return name();
//	else if ( inheritsAll() )
//		return parent()->displayName();
	else 
		return parentStyle()->displayName() + "+";
}



QString LineStyle::asString() const
{
	QString result;
/*	if ( !inh_Font )
		result += QObject::tr("font %1 ").arg(font().scName());
	if ( !inh_FontSize )
		result += QObject::tr("size %1 ").arg(fontSize());
	if ( !inh_Effects )
		result += QObject::tr("+style ");
	if ( !inh_StrokeColor  ||  !inh_StrokeShade  ||  !inh_FillColor || !inh_FillShade )
		result += QObject::tr("+color ");
	if ( !inh_UnderlineWidth  ||  !inh_UnderlineOffset )
		result += underlineWidth() > 0 ? QObject::tr("+underline ") : QObject::tr("-underline ");
	if ( !inh_StrikethruWidth || !inh_StrikethruOffset )
		result += strikethruWidth() > 0 ? QObject::tr("+strikeout ") : QObject::tr("-strikeout ");
	if ( !inh_ShadowXOffset || !inh_ShadowYOffset )
		result += shadowXOffset() != 0 || shadowYOffset() != 0 ? QObject::tr("+shadow ") : QObject::tr("-shadow ");
	if ( !inh_OutlineWidth )
		result += outlineWidth() > 0 ? QObject::tr("+outline ") : QObject::tr("-outline ");
	if ( !inh_Tracking )
		result += tracking() > 0 ? QObject::tr("+tracking %1 ").arg(tracking()) : QObject::tr("-tracking ");
	if ( !inh_BaselineOffset )
		result += QObject::tr("+baseline %1 ").arg(baselineOffset());
	if ( !inh_ScaleH || !inh_ScaleV )
		result += QObject::tr("+stretch ");
	if ( hasParent() )
		result += QObject::tr("parent= %1").arg(parent());
*/	return result.trimmed();
}


void LineStyle::update(const StyleContext* context)
{
	Style::update(context);
	const LineStyle * oth = dynamic_cast<const LineStyle*> ( parentStyle() );
	if (oth) {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		if (inh_##attr_NAME) \
			m_##attr_NAME = oth->attr_GETTER();
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	}
}


void LineStyle::setStyle(const LineStyle& other) 
{
	other.validate();
	setParent(other.parent());
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	inh_##attr_NAME = other.inh_##attr_NAME; \
	m_##attr_NAME = other.m_##attr_NAME;
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
}

void LineStyle::getNamedResources(ResourceCollection& lists) const
{
	QList<LineStyle>::const_iterator it, itend = m_Sublines.constEnd();

	lists.collectColor(color());
	for (const Style* sty = parentStyle(); sty != NULL; sty = sty->parentStyle())
		lists.collectLineStyle(sty->name());
	for (it = m_Sublines.begin(); it != itend; ++it)
		(*it).getNamedResources(lists);
}

void LineStyle::replaceNamedResources(ResourceCollection& newNames)
{
	QMap<QString,QString>::ConstIterator it;
	QList<LineStyle>::iterator itl, itle = m_Sublines.end();

	if (!inh_Color && (it = newNames.colors().find(color())) != newNames.colors().end())
		setColor(it.value());
	if (hasParent() && (it = newNames.lineStyles().find(parent())) != newNames.lineStyles().end())
		setParent(it.value());
	for (itl = m_Sublines.begin(); itl != itle; ++itl)
		(*itl).replaceNamedResources(newNames);
}

/*
bool LineStyle::definesAll() const
{
	return definesLineSpacing() && 
	definesLeftMargin() && 
	definesRightMargin() && 
	definesFirstIndent() &&
	definesAlignment() && 
	definesGapBefore()  &&
	definesLineSpacingMode()  && 
	definesGapAfter()  && 
	definesHasDropCap() && 
	definesDropCapOffset() && 
	definesDropCapLines() && 
	definesUseBaselineGrid() && 
	lineStyle().definesAll() ;
	
}

// equiv. to "*this == LineStyle()"
bool LineStyle::inheritsAll() const
{
	return inheritsLineSpacing() && 
	inheritsLeftMargin() && 
	inheritsRightMargin() && 
	inheritsFirstIndent() &&
	inheritsAlignment() && 
	inheritsGapBefore()  &&
	inheritsLineSpacingMode()  && 
	inheritsGapAfter()  && 
	inheritsHasDropCap() && 
	inheritsDropCapOffset() && 
	inheritsDropCapLines() && 
	inheritsUseBaselineGrid() && 
	lineStyle().inheritsAll() ;
}
*/

typedef QList<LineStyle> Sublist;

static QString toXMLString(const Sublist & )
{
	return "dummy";
}


void LineStyle::saxx(SaxHandler& handler, const Xml_string& elemtag) const
{
	Xml_attr att;
	Style::saxxAttributes(att);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME && strcmp(# attr_NAME, "Sublines") != 0 ) \
		att.insert(# attr_NAME, toXMLString(m_##attr_NAME));
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	if (!name().isEmpty())
		att["id"] = mkXMLName(elemtag + name());
	handler.begin(elemtag, att);
	if (parentStyle())
		parentStyle()->saxx(handler);	
	Sublist::const_iterator it;
	for (it=m_Sublines.begin(); it != m_Sublines.end(); ++it)
	{
		(*it).saxx(handler, "subline");
	}
	handler.end(elemtag);
}




template<>
Sublist parse<Sublist>(const Xml_string& str)
{
	return Sublist();
}

template<>
Qt::PenStyle parse<Qt::PenStyle>(const Xml_string& str)
{
	return parseEnum<Qt::PenStyle>(str);
}

template<>
Qt::PenCapStyle parse<Qt::PenCapStyle>(const Xml_string& str)
{
	return parseEnum<Qt::PenCapStyle>(str);
}

template<>
Qt::PenJoinStyle parse<Qt::PenJoinStyle>(const Xml_string& str)
{
	return parseEnum<Qt::PenJoinStyle>(str);
}



using namespace desaxe;


const Xml_string LineStyle::saxxDefaultElem("linestyle");

void LineStyle::desaxeRules(const Xml_string& prefixPattern, Digester& ruleset, Xml_string elemtag)
{
	Xml_string stylePrefix(Digester::concat(prefixPattern, elemtag));
	ruleset.addRule(stylePrefix, Factory<LineStyle>());
	ruleset.addRule(stylePrefix, IdRef<LineStyle>());
	Style::desaxeRules<LineStyle>(prefixPattern, ruleset, elemtag);

//  "**" doesnt work yet - av
//	Xml_string stylePrefixRec(Digester::concat(stylePrefix, "**"));
	const Xml_string& stylePrefixRec(stylePrefix);
	Xml_string subPrefix(Digester::concat(stylePrefixRec, "subline"));
	ruleset.addRule(subPrefix, Factory<LineStyle>());
	Style::desaxeRules<LineStyle>(stylePrefixRec, ruleset, "subline");
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if ( strcmp(# attr_NAME, "Sublines") != 0 ) { \
		ruleset.addRule(stylePrefix, SetAttributeWithConversion<LineStyle, attr_TYPE> ( & LineStyle::set##attr_NAME,  # attr_NAME, &parse<attr_TYPE> )); \
		ruleset.addRule(subPrefix, SetAttributeWithConversion<LineStyle, attr_TYPE> ( & LineStyle::set##attr_NAME,  # attr_NAME, &parse<attr_TYPE> )); \
	}
#include "linestyle.attrdefs.cxx"
#undef ATTRDEF
	ruleset.addRule(subPrefix, SetterWithConversion<LineStyle, const LineStyle&, LineStyle>( & LineStyle::appendSubline ));
}
