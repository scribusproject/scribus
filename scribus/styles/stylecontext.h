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


#ifndef STYLECONTEXT_H
#define STYLECONTEXT_H

#include <cassert>
#include <QString>
#include <QList>
#include "scribusapi.h"
#include "observable.h"

class Style;


/**
 Style inheritance works with names. A StyleContext finds the Style to a given name.
 If there are changes to the Styles contained in a StyleContext, you have to call
 invalidate() to increase the version info. Styles using this StyleContext will then
 update their cached values the next time they are used.
 */
class SCRIBUS_API StyleContext : public Observable<StyleContext> {
	
public:
	StyleContext() 
	: m_version(0), m_cnt(0)
	{
//		qDebug(QString("constr. %1 /%2").arg(reinterpret_cast<uint>(this),16).arg(m_level));
	}

	StyleContext(const StyleContext& o) 
	: m_version(o.m_version), m_cnt(0)
	{
//		qDebug(QString("constr. cp %1 /%2").arg(reinterpret_cast<uint>(this),16).arg(m_level));
	}


	StyleContext& operator= (const StyleContext& o)
	{
		m_version = o.m_version;
		m_cnt = 0;
		return *this;
	}


	int version() const  { return m_version; }
	
	virtual bool contextContained(const StyleContext* context) const { return context == this; }
	virtual bool checkConsistency() const { return true; }
	virtual const Style* resolve(const QString& name) const = 0;
	virtual ~StyleContext() 
	{
//		qDebug(QString("destr. %1").arg(reinterpret_cast<uint>(this),16));
	}

	void invalidate(); 
	
	bool connect(QObject* receiver, const char *member ) const;
	bool disconnect(QObject* receiver, const char *member=0 ) const;
	
	
protected:
	int m_version;
	mutable int m_cnt;
};

#endif
