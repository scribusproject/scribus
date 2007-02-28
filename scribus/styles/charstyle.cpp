/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */


#include <qvaluelist.h>
#include <qobject.h>
#include "sctextstruct.h"
#include "scfonts.h"

#include "styles/style.h"
#include "charstyle.h"
#include "desaxe/saxiohelper.h"
#include "desaxe/simple_actions.h"
#include "prefsmanager.h"

StyleFlag& StyleFlag::operator&= (const StyleFlag& right){        
	int result = static_cast<int>(value) & static_cast<int>(right.value);        
	value = static_cast<StyleFlagValue>(result);
	return *this;
}

StyleFlag& StyleFlag::operator|= (const StyleFlag& right)
{
	int result = static_cast<int>(value) | static_cast<int>(right.value);
	value = static_cast<StyleFlagValue>(result);
	return *this;
}

StyleFlag StyleFlag::operator& (const StyleFlag& right)
{
	int val = static_cast<int>(value) & static_cast<int>(right.value);
	StyleFlag result(static_cast<StyleFlagValue>(val)); 
	return result;
}

StyleFlag StyleFlag::operator& (int right)
{
	int val = static_cast<int>(value) & right;
	StyleFlag result(static_cast<StyleFlagValue>(val)); 
	return result;
}

StyleFlag StyleFlag::operator| (const StyleFlag& right)
{        
	int val = static_cast<int>(value) | static_cast<int>(right.value);
	StyleFlag result(static_cast<StyleFlagValue>(val)); 
	return result;
}

StyleFlag StyleFlag::operator^ (const StyleFlag& right)
{
	int val = static_cast<int>(value) ^ static_cast<int>(right.value);
	StyleFlag result(static_cast<StyleFlagValue>(val)); 
	return result;
}

StyleFlag StyleFlag::operator^  (int right)
{
	int val = static_cast<int>(value) ^ right;
	StyleFlag result(static_cast<StyleFlagValue>(val)); 
	return result;
}

StyleFlag StyleFlag::operator~ ()
{
	int val = ~ static_cast<int>(value);
	StyleFlag result(static_cast<StyleFlagValue>(val)); 
	return result;
}

bool StyleFlag::operator== (const StyleFlag& right) const
{        
	int result = static_cast<int>( (value ^ right.value) & ScStyle_UserStyles);
	return (result == 0);
}

bool StyleFlag::operator== (const StyleFlagValue right) const
{
	int result = static_cast<int>( (value ^ right) & ScStyle_UserStyles);
	return (result == 0);
}

bool StyleFlag::operator== (int right) const
{
	int result = static_cast<int>( (value ^ right) & ScStyle_UserStyles);
	return (result == 0);
}

bool StyleFlag::operator!= (const StyleFlag& right) const
{
	return !(*this==right);
}

bool StyleFlag::operator!= (const StyleFlagValue right) const
{
	return !(*this==right);
}


void CharStyle::applyCharStyle(const CharStyle & other)
{
	Style::applyStyle(other);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (! other.inh_##attr_NAME) \
		set##attr_NAME(other.m_##attr_NAME);
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
}


void CharStyle::eraseCharStyle(const CharStyle & other)
{
	other.validate();
	Style::eraseStyle(other);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME && m_##attr_NAME == other.m_##attr_NAME) \
		reset##attr_NAME();
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
}

bool CharStyle::equiv(const Style & other) const
{
	other.validate();
	const CharStyle * oth = dynamic_cast<const CharStyle*> ( & other );
	return  oth &&
		parent() == oth->parent() 
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		&& (inh_##attr_NAME == oth->inh_##attr_NAME) \
		&& (inh_##attr_NAME || m_##attr_NAME == oth->m_##attr_NAME)
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
		;	
}


QString CharStyle::displayName() const
{
	if ( hasName() || !hasParent() || ! m_context)
		return name();
//	else if ( inheritsAll() )
//		return parent()->displayName();
	else 
		return parentStyle()->displayName() + "+";
}



QString CharStyle::asString() const
{
	QString result;
	if ( !inh_Font )
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
	return result.stripWhiteSpace();
}


void CharStyle::update(const StyleContext* context)
{
	Style::update(context);
	const CharStyle * oth = dynamic_cast<const CharStyle*> ( parentStyle() );
	if (oth) {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		if (inh_##attr_NAME) \
			m_##attr_NAME = oth->attr_GETTER();
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
	}
}


void CharStyle::setStyle(const CharStyle& other) 
{
	other.validate();
	setParent(other.parent());
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	inh_##attr_NAME = other.inh_##attr_NAME; \
	m_##attr_NAME = other.m_##attr_NAME;
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
}



/*
bool CharStyle::definesAll() const
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
	charStyle().definesAll() ;
	
}

// equiv. to "*this == CharStyle()"
bool CharStyle::inheritsAll() const
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
	charStyle().inheritsAll() ;
}
*/


Xml_string toXMLString(StyleFlag val)
{
	return toXMLString(static_cast<unsigned int>(val & ScStyle_UserStyles));
}


void CharStyle::saxx(SaxHandler& handler, const Xml_string elemtag) const
{
	Xml_attr att;
	Style::saxxAttributes(att);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME) \
		att.insert(# attr_NAME, toXMLString(m_##attr_NAME));
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
	if (!name().isEmpty())
		att["id"] = mkXMLName(elemtag + name());
	handler.begin(elemtag, att);
	if (hasParent() && parentStyle())
		parentStyle()->saxx(handler);	
	handler.end(elemtag);
}



template<>
StyleFlag parse<StyleFlag>(Xml_string str)
{
	return StyleFlag(parseInt(str));
}

template<>
ScFace parse<ScFace>(Xml_string str)
{
	// FIXME: enable font substitution here
	return PrefsManager::instance()->appPrefs.AvailFonts[str];
}


using namespace desaxe;


const Xml_string CharStyle::saxxDefaultElem("charstyle");

void CharStyle::desaxeRules(Xml_string prefixPattern, Digester& ruleset, Xml_string elemtag)
{
	Xml_string stylePrefix(Digester::concat(prefixPattern, elemtag));
	ruleset.addRule(stylePrefix, Factory<CharStyle>());
	ruleset.addRule(stylePrefix, IdRef<CharStyle>());
	Style::desaxeRules<CharStyle>(prefixPattern, ruleset, elemtag);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	ruleset.addRule(stylePrefix, SetAttributeWithConversion<CharStyle, attr_TYPE> ( & CharStyle::set##attr_NAME,  # attr_NAME, &parse<attr_TYPE> ));
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF		
}
