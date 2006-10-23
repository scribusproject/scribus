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
#include "style.h"


StyleFlag& operator&= (StyleFlag& left, StyleFlag right){        
	int result = static_cast<int>(left) & static_cast<int>(right);        
	left = static_cast<StyleFlag>(result);
	return left;
}

StyleFlag& operator|= (StyleFlag& left, StyleFlag right)
{
	int result = static_cast<int>(left) | static_cast<int>(right);
	left = static_cast<StyleFlag>(result);
	return left;
}

StyleFlag operator& (StyleFlag left, StyleFlag right)
{
	int result = static_cast<int>(left) & static_cast<int>(right);        
	return static_cast<StyleFlag>(result);
}

StyleFlag operator| (StyleFlag left, StyleFlag right)
{        
	int result = static_cast<int>(left) | static_cast<int>(right);
	return static_cast<StyleFlag>(result);
}

StyleFlag operator^ (StyleFlag left, StyleFlag right){
	int result = static_cast<int>(left) ^ static_cast<int>(right);
	return static_cast<StyleFlag>(result);
}

StyleFlag operator~ (StyleFlag arg)
{
	int result = ~ static_cast<int>(arg);
	return static_cast<StyleFlag>(result);
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
	Style::eraseStyle(other);
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME && m_##attr_NAME == other.attr_GETTER()) \
		reset##attr_NAME();
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
}

bool CharStyle::equiv(const Style & other) const
{
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
	if ( hasName() || !hasParent() || ! m_base)
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


void CharStyle::update(StyleBase* base)
{
	Style::update(base);
	const CharStyle * oth = dynamic_cast<const CharStyle*> ( parentStyle() );
	if (oth) {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		if (inh_##attr_NAME) \
			m_##attr_NAME = oth->attr_GETTER();
#include "charstyle.attrdefs.cxx"
#undef ATTRDEF
	}
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

