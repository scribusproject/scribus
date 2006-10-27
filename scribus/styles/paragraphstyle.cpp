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




#include "style.h"

ParagraphStyle::ParagraphStyle() : Style(), cstyleBase(StyleBase::PAR_LEVEL, NULL), cstyle()
{
	cstyleBase.setDefaultStyle( &cstyle );
//	qDebug(QString("ParagraphStyle() %1 pbase %2 cbase %3").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(base())).arg(reinterpret_cast<uint>(defaultStyle()->base())));
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = attr_DEFAULT; \
	inh_##attr_NAME = true;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}


ParagraphStyle::ParagraphStyle(const ParagraphStyle& other) : Style(other), cstyleBase(StyleBase::PAR_LEVEL, NULL), cstyle(other.charStyle())
{
	cstyleBase.setDefaultStyle( &cstyle );
//	qDebug(QString("ParagraphStyle(%2) %1").arg(reinterpret_cast<uint>(&other)).arg(reinterpret_cast<uint>(this)));

#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}

ParagraphStyle::~ParagraphStyle()
{
//	qDebug(QString("~ParagraphStyle %1").arg(reinterpret_cast<uint>(this)));
}
	
QString ParagraphStyle::displayName() const
{
	if ( hasName() || !hasParent() || !m_base)
		return name();
	//	else if ( inheritsAll() )
	//		return parent()->displayName();
	else 
		return (parentStyle())->displayName() + "+";
}

namespace {
bool sametabs(const QValueList<ParagraphStyle::TabRecord>& tabs, const QValueList<ParagraphStyle::TabRecord>& other)
{
	ParagraphStyle::TabRecord tb;
	bool tabEQ = false;
	if ((other.count() == 0) && (tabs.count() == 0))
		tabEQ = true;
	else
	{
		for (uint t1 = 0; t1 < other.count(); t1++)
		{
			tb.tabPosition = other[t1].tabPosition;
			tb.tabType = other[t1].tabType;
			tb.tabFillChar = other[t1].tabFillChar;
			tabEQ = false;
			for (uint t2 = 0; t2 < tabs.count(); t2++)
			{
				ParagraphStyle::TabRecord tb2;
				tb2.tabPosition = tabs[t2].tabPosition;
				tb2.tabType = tabs[t2].tabType;
				tb2.tabFillChar = tabs[t2].tabFillChar;
				if ((tb2.tabFillChar == tb.tabFillChar) && (tb2.tabPosition == tb.tabPosition) && (tb2.tabType == tb.tabType))
				{
					tabEQ = true;
					break;
				}
			}
			if (!tabEQ)
				break;
		}
	}
	return tabEQ;
}

} // namespace

bool ParagraphStyle::equiv(const Style& other) const
{
	const ParagraphStyle* oth = dynamic_cast<const ParagraphStyle*> ( & other );
	return  oth &&
		parent() == oth->parent() 
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		&& (inh_##attr_NAME == oth->inh_##attr_NAME) \
		&& (inh_##attr_NAME || m_##attr_NAME == oth->m_##attr_NAME)
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
		;
}	



ParagraphStyle& ParagraphStyle::operator=(const ParagraphStyle& other) 
{
	static_cast<Style&>(*this) = static_cast<const Style&>(other);
	cstyle = other.charStyle();
	cstyleBase = other.cstyleBase;
	// we dont want cstyleBase to point to other's charstyle...
	cstyleBase.setDefaultStyle( &cstyle );
	cstyleBase.invalidate();
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	return *this;
}



void ParagraphStyle::update(StyleBase* base)
{
	Style::update(base);
	const ParagraphStyle * oth = dynamic_cast<const ParagraphStyle*> ( parentStyle() );
	if (oth) {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		if (inh_##attr_NAME) \
			m_##attr_NAME = oth->attr_GETTER();
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	}
}



void ParagraphStyle::applyStyle(const ParagraphStyle& other) 
{
	Style::applyStyle(other);
	cstyle.applyCharStyle(other.charStyle());
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (! other.inh_##attr_NAME) \
		set##attr_NAME(other.m_##attr_NAME);
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}


void ParagraphStyle::eraseStyle(const ParagraphStyle& other) 
{
	Style::eraseStyle(other);
	cstyle.eraseCharStyle(other.charStyle());
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME && m_##attr_NAME == other.attr_GETTER()) \
		reset##attr_NAME();
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}

