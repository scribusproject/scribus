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

ParagraphStyle::ParagraphStyle() : Style(), cstyleBase(NULL), cstyleBaseIsInh(true), cstyle()
{
	setParent("");
	cstyleBase.setDefaultStyle( &cstyle );
//	qDebug(QString("ParagraphStyle() %1 pbase %2 cbase %3").arg(reinterpret_cast<uint>(this)).arg(reinterpret_cast<uint>(base())).arg(reinterpret_cast<uint>(defaultStyle()->base())));
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = attr_DEFAULT; \
	inh_##attr_NAME = true;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}


ParagraphStyle::ParagraphStyle(const ParagraphStyle& other) : Style(other), cstyleBase(NULL), cstyleBaseIsInh(other.cstyleBaseIsInh), cstyle(other.charStyle())
{
	if (cstyleBaseIsInh)
		cstyle.setBase(NULL);
	else
		cstyle.setBase(other.charStyle().base());
	cstyleBase.setDefaultStyle( &cstyle );
//	qDebug(QString("ParagraphStyle(%2) %1").arg(reinterpret_cast<uint>(&other)).arg(reinterpret_cast<uint>(this)));
	other.validate();

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
		return parentStyle()->displayName() + "+";
}


bool ParagraphStyle::equiv(const Style& other) const
{
	other.validate();
	const ParagraphStyle* oth = dynamic_cast<const ParagraphStyle*> ( & other );
	return  oth &&
		parent() == oth->parent() && cstyle.equiv(oth->charStyle())
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		&& (inh_##attr_NAME == oth->inh_##attr_NAME) \
		&& (inh_##attr_NAME || m_##attr_NAME == oth->m_##attr_NAME)
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
		;
}	



ParagraphStyle& ParagraphStyle::operator=(const ParagraphStyle& other) 
{
	other.validate();
	static_cast<Style&>(*this) = static_cast<const Style&>(other);

	cstyle = other.charStyle();

	// we dont want cstyleBase to point to other's charstyle...
	cstyleBase.setDefaultStyle( &cstyle );
	
	if (cstyleBaseIsInh)
	{
		const ParagraphStyle * parent = dynamic_cast<const ParagraphStyle*> ( parentStyle() );
		cstyle.setBase(parent ? parent->charStyleBase() : NULL);
	}
	else
	{
		cstyle.setBase(other.charStyle().base());
	}
	
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	m_##attr_NAME = other.m_##attr_NAME; \
	inh_##attr_NAME = other.inh_##attr_NAME;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	return *this;
}


void ParagraphStyle::setBase(const StyleBase* base)
{
	Style::setBase(base);
//	qDebug(QString("ParagraphStyle::setBase(%1) parent=%2").arg((unsigned long int)base).arg((unsigned long int)oth));
	repairImplicitCharStyleInheritance();
}

void ParagraphStyle::repairImplicitCharStyleInheritance()
{
	if (cstyleBaseIsInh) {
		const ParagraphStyle * newParent = dynamic_cast<const ParagraphStyle*> ( parentStyle() );
		cstyle.setBase(newParent ? newParent->charStyleBase() : NULL);
	}
}


void ParagraphStyle::breakImplicitCharStyleInheritance(bool val)
{ 
	cstyleBaseIsInh = !val;
	repairImplicitCharStyleInheritance();
}

void ParagraphStyle::update(const StyleBase* base)
{
	Style::update(base);
	assert ( &cstyleBase != cstyle.base());

	repairImplicitCharStyleInheritance();

	const ParagraphStyle * oth = dynamic_cast<const ParagraphStyle*> ( parentStyle() );
//	qDebug(QString("ParagraphStyle::update(%1) parent=%2").arg((unsigned long int)base).arg((unsigned long int)oth));
	if (oth) {
		assert ( &cstyleBase != cstyle.base());
		cstyle.validate();
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		if (inh_##attr_NAME) \
			m_##attr_NAME = oth->attr_GETTER();
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
	}
	cstyleBase.invalidate();
}



void ParagraphStyle::applyStyle(const ParagraphStyle& other) 
{
	Style::applyStyle(other);
	cstyle.applyCharStyle(other.charStyle());
	cstyleBase.invalidate();
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (! other.inh_##attr_NAME) \
		set##attr_NAME(other.m_##attr_NAME);
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}


void ParagraphStyle::eraseStyle(const ParagraphStyle& other) 
{
	other.validate();
	Style::eraseStyle(other);
	cstyle.eraseCharStyle(other.charStyle());
	cstyleBase.invalidate();
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME && m_##attr_NAME == other.m_##attr_NAME) \
		reset##attr_NAME();
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}

void ParagraphStyle::setStyle(const ParagraphStyle & other) 
{
	other.validate();
	setParent(other.parent());
	cstyle.setStyle(other.charStyle());
	cstyleBase.invalidate();
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	inh_##attr_NAME = other.inh_##attr_NAME; \
	m_##attr_NAME = other.m_##attr_NAME;
#include "paragraphstyle.attrdefs.cxx"
#undef ATTRDEF
}

