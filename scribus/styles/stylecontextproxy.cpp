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

//#include <QDebug>

#include "style.h"
#include "stylecontextproxy.h"

const Style* StyleContextProxy::resolve(const QString& name) const
{
	const StyleContext* context = m_default->context();
	//	if (!name.isEmpty())
	//		qDebug() << QString("resolve %4 %3 -%1- %2").arg(name)
	//			   .arg(reinterpret_cast<uint>(context),16).arg(reinterpret_cast<uint>(this),16)
	//			   .arg(m_default->name());
	
	if (name.isEmpty() || ! context)
		return m_default;
	else if (this == context)
		return NULL;
	else
		return context->resolve(name);
}
