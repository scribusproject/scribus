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

#include <QDebug>

#include "style.h"
#include "desaxe/digester.h"
#include "desaxe/simple_actions.h"
#include "desaxe/saxiohelper.h"

const QString BaseStyle::INHERIT_PARENT = "\n";

void BaseStyle::setContext(const StyleContext* context)
{ 
	if (m_context != context) {
		m_context = context;
		m_contextversion = -1;
		assert( !m_context || m_context->checkConsistency() );
	}
	//qDebug() << QString("setContext of %2 context %1").arg(reinterpret_cast<uint>(m_context),16).arg(reinterpret_cast<uint>(this),16);
}

void BaseStyle::setDefaultStyle(bool ids)
{ 
	if (ids && hasParent())
		setParent(QString(""));
	m_isDefaultStyle = ids; 
}

void BaseStyle::setParent(const QString& p)
{ 
	if (m_isDefaultStyle && !p.isEmpty())
	{
		qDebug() << "A default style cannot have a parent";
		return;
	}
	if (m_parent != p) 
		m_contextversion = -1; 
	m_parent = p.isEmpty()? "" : p;
}

void BaseStyle::update(const StyleContext* b)
{
	if (b)
		m_context = b;
	if (m_context)
		m_contextversion = m_context->version(); 
}

const BaseStyle* BaseStyle::parentStyle() const
{ 
	//qDebug() << QString("follow %1").arg(reinterpret_cast<uint>(m_context),16);
	if (m_isDefaultStyle)
		return NULL;
	const BaseStyle * par = m_context ? m_context->resolve(m_parent) : NULL;
	if (par == this) return NULL; else return par;
}

bool BaseStyle::canInherit(const QString& parentName) const
{
	if (m_name.isEmpty() || parentName.isEmpty())
		return true;
	if (!m_context || (m_name == parentName))
		return false;

	const BaseStyle* parentStyle = m_context->resolve(parentName);
	if (!parentStyle)
		return false;

	bool  parentLoop = false;

	const BaseStyle* pStyle = parentStyle;
	while (pStyle)
	{
		if (pStyle->hasParent() && (pStyle->parent() == m_name))
		{
			parentLoop = true;
			break;
		}
		pStyle = pStyle->hasParent() ? pStyle->parentStyle() : NULL;
	}

	return (!parentLoop);
}

void BaseStyle::saxxAttributes(Xml_attr& attr) const
{
	if (!name().isEmpty())
		attr["name"] = name();
	if(!parent().isEmpty())
		attr["parent"] = parent();
	if (!shortcut().isEmpty())
		attr["shortcut"] = shortcut();
}
