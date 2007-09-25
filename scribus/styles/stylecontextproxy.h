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


#ifndef STYLECONTEXTPROXY_H
#define STYLECONTEXTPROXY_H

#include <cassert>
#include <QString>
#include "scfonts.h"
#include "scribusapi.h"
#include "sccolor.h"
#include "styles/stylecontext.h"
#include <QList>


/** This class turns a style into a new stylecontext:
*  it maps the empty name "" to the given default style and uses
*  the style's context to resolve all other names.
*/
class StyleContextProxy: public StyleContext 
{
public:
	const Style* resolve(const QString& name) const;
	
	StyleContextProxy(const Style* style) 
		: StyleContext(), m_default(style) {
		}
	
	StyleContextProxy(const StyleContextProxy& other)
		: StyleContext(other), m_default(other.m_default) {
		}
	
	StyleContextProxy& operator= (const StyleContextProxy& other)
	{
		static_cast<StyleContext&>(*this) = static_cast<const StyleContext&>(other); 
		m_default = other.m_default;
		return *this;
	}
	
	const Style* defaultStyle() const { return m_default; }
	
	void setDefaultStyle(const Style* def) { 
		assert(def);
		m_default = def; 
		invalidate();
	}
	
	bool checkConsistency() const 
	{ 
		const StyleContext* context = m_default->context();
		return !context || context == this || !context->contextContained(this);
	}
	
	bool contextContained(const StyleContext* context) const 
	{
		const StyleContext* mycontext = m_default->context();
		return context == this || 
			(mycontext && mycontext->contextContained(context));
	}
	
private:
		const Style* m_default;
};

#endif
