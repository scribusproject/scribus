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

void StyleBase::invalidate()
{
	++m_version; 
	if (m_cnt > 0)          // activate() can be slow even if there's nothing to signal
		m_sig.activate(); 
}

bool StyleBase::connect(const QObject* receiver, const char *member ) const
{
	bool result = m_sig.connect(receiver, member);
	if (result)
		++m_cnt;
	return result;
}

bool StyleBase::disconnect(const QObject* receiver, const char *member ) const
{
	bool result = m_sig.disconnect(receiver, member);
	if (result)
		--m_cnt;
	return result;
}


void Style::setBase(const StyleBase* base)
{ 
	if (m_base != base) {
		m_base = base;
		m_baseversion = -1;
		assert( !m_base || m_base->checkConsistency() );
	}
	//qDebug(QString("setBase of %2 base %1").arg(reinterpret_cast<uint>(m_base),16).arg(reinterpret_cast<uint>(this),16));
}


void Style::update(const StyleBase* b)
{
	if (b)
		m_base = b;
	if (m_base)
		m_baseversion = m_base->version(); 
}

void Style::validate() const
{ 
	if (m_base && m_baseversion != m_base->version()) {
		const_cast<Style*>(this)->update(m_base); 
		assert( m_base->checkConsistency() );
	}
}


const Style* Style::parentStyle() const 
{ 
	//qDebug(QString("follow %1").arg(reinterpret_cast<uint>(m_base),16));
	const Style * par = m_base ? m_base->resolve(m_parent) : NULL;
	if (par == this) return NULL; else return par;
}


const Style* StyleBaseProxy::resolve(const QString& name) const
{
	const StyleBase* base = m_default->base();
//	if (!name.isEmpty())
//		qDebug(QString("resolve %4 %3 -%1- %2").arg(name)
//			   .arg(reinterpret_cast<uint>(base),16).arg(reinterpret_cast<uint>(this),16)
//			   .arg(m_default->name()));

	if (name.isEmpty() || ! base)
		return m_default;
	else if (this == base)
		return NULL;
	else
		return base->resolve(name);
}
