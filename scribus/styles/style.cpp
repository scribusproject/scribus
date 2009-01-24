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

const QString Style::INHERIT_PARENT = "\n";

void Style::setContext(const StyleContext* context)
{ 
	if (m_context != context) {
		m_context = context;
		m_contextversion = -1;
		assert( !m_context || m_context->checkConsistency() );
	}
	//qDebug() << QString("setContext of %2 context %1").arg(reinterpret_cast<uint>(m_context),16).arg(reinterpret_cast<uint>(this),16);
}

void Style::setDefaultStyle(bool ids)
{ 
	if (ids && hasParent())
		setParent(QString(""));
	m_isDefaultStyle = ids; 
}

void Style::setParent(const QString& p) 
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

void Style::update(const StyleContext* b)
{
	if (b)
		m_context = b;
	if (m_context)
		m_contextversion = m_context->version(); 
}

void Style::validate() const
{ 
	if (m_context && m_contextversion != m_context->version()) {
		const_cast<Style*>(this)->update(m_context); 
		assert( m_context->checkConsistency() );
	}
}


const Style* Style::parentStyle() const 
{ 
	//qDebug() << QString("follow %1").arg(reinterpret_cast<uint>(m_context),16);
	if (m_isDefaultStyle)
		return NULL;
	const Style * par = m_context ? m_context->resolve(m_parent) : NULL;
	if (par == this) return NULL; else return par;
}


void Style::saxxAttributes(Xml_attr& attr) const
{
	if (!name().isEmpty())
		attr["name"] = name();
	if(!parent().isEmpty())
		attr["parent"] = parent();
	if (!shortcut().isEmpty())
		attr["shortcut"] = shortcut();
}
