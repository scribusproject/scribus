/*
 Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include <QString>

#include "commonstrings.h"
#include "util_math.h"
#include "cellstyle.h"
#include "tableborder.h"

QString CellStyle::displayName() const
{
	if (isDefaultStyle())
		return CommonStrings::trDefaultCellStyle;
	if (hasName() || !hasParent() || !m_context)
		return name();
	else 
		return parentStyle()->displayName() + "+";
}

bool CellStyle::equiv(const Style& other) const
{
	other.validate();
	const CellStyle *p_other = dynamic_cast<const CellStyle*> (&other);
	return p_other &&
		parent() == p_other->parent() 
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		&& (inh_##attr_NAME == p_other->inh_##attr_NAME) \
		&& (inh_##attr_NAME || isequiv(m_##attr_NAME, p_other->m_##attr_NAME))
#include "cellstyle.attrdefs.cxx"
#undef ATTRDEF
		;
}

void CellStyle::erase()
{
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
	if (!inh_##attr_NAME) \
		reset##attr_NAME();
#include "cellstyle.attrdefs.cxx"
#undef ATTRDEF
	//updateFeatures(); TODO: Investigate this.
}

void CellStyle::update(const StyleContext* context)
{
	Style::update(context);
	const CellStyle* parent = dynamic_cast<const CellStyle*>(parentStyle());
	if (parent) {
#define ATTRDEF(attr_TYPE, attr_GETTER, attr_NAME, attr_DEFAULT) \
		if (inh_##attr_NAME) \
			m_##attr_NAME = parent->attr_GETTER();
#include "cellstyle.attrdefs.cxx"
#undef ATTRDEF
	}
	//updateFeatures(); TODO: Investigate this.
}

void CellStyle::getNamedResources(ResourceCollection& lists) const
{
	for (const Style* style = parentStyle(); style != NULL; style = style->parentStyle())
		lists.collectCellStyle(style->name());
	lists.collectColor(fillColor());
	// TODO: Collect border colors.
}

void CellStyle::replaceNamedResources(ResourceCollection& newNames)
{
	QMap<QString, QString>::ConstIterator it;

	if (!isInhFillColor() && (it = newNames.colors().find(fillColor())) != newNames.colors().end())
		setFillColor(it.value());

	// TODO: Do we need to do something else? E.g. CharStyle calls its updateFeatures().
}

QString CellStyle::asString() const
{
	QString str("cellstyle(");
	str += QString("displayName=%1,").arg(displayName());
	str += QString("fillColor=%1%2,").arg(fillColor()).arg(inh_FillColor ? "(inh)" : "");
	str += QString("leftBorder=%1%2,").arg(leftBorder().asString()).arg(inh_LeftBorder ? "(inh)" : "");
	str += QString("rightBorder=%1%2,").arg(rightBorder().asString()).arg(inh_RightBorder ? "(inh)" : "");
	str += QString("topBorder=%1%2,").arg(topBorder().asString()).arg(inh_TopBorder ? "(inh)" : "");
	str += QString("bottomBorder=%1%2,").arg(bottomBorder().asString()).arg(inh_BottomBorder ? "(inh)" : "");
	str += QString("leftPadding=%1%2,").arg(leftPadding()).arg(inh_LeftPadding ? "(inh)" : "");
	str += QString("rightPadding=%1%2,").arg(rightPadding()).arg(inh_RightPadding ? "(inh)" : "");
	str += QString("topPadding=%1%2,").arg(topPadding()).arg(inh_TopPadding ? "(inh)" : "");
	str += QString("bottomPadding=%1%2,").arg(bottomPadding()).arg(inh_BottomPadding ? "(inh)" : "");
	str += QString(hasParent() ? QString("parent=%1").arg(parent()) : "");
	str += QString(")");
	return str;
}
