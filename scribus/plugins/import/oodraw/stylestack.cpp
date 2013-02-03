/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This file is part of the KDE project
   Copyright (c) 2003 Lukas Tinkl <lukas@kde.org>
   Copyright (c) 2003 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "stylestack.h"
#include "oodrawimp.h"
#include "scclocale.h"
#include <QList>

StyleStack::StyleStack()
{
    clear();
	fillNodeNameList(m_nodeNames, StyleStack::OODraw1x);
}

StyleStack::~StyleStack()
{
}

void StyleStack::setMode( const StyleStack::Mode mode )
{
	m_nodeNames.clear();
	fillNodeNameList(m_nodeNames, mode);
}

void StyleStack::clear()
{
    m_stack.clear();
}

void StyleStack::save()
{
    m_marks.push( m_stack.count() );
}

void StyleStack::restore()
{
    Q_ASSERT( !m_marks.isEmpty() );
    int toIndex = m_marks.pop();
    Q_ASSERT( toIndex > -1 );
    Q_ASSERT( toIndex <= (int)m_stack.count() ); // If equal, nothing to remove. If greater, bug.
    for ( int index = (int)m_stack.count() - 1; index >= toIndex; --index )
        m_stack.pop_back();
}

void StyleStack::pop()
{
    m_stack.pop_back();
}

void StyleStack::push( const QDomElement& style )
{
    m_stack.append( style );
}

bool StyleStack::hasAttribute( const QString& name ) const
{
    // TODO: has to be fixed for complex styles like list-styles
    QList<QDomElement>::ConstIterator it = m_stack.end();
    while ( it != m_stack.begin() )
    {
        --it;
		QDomElement properties = searchAttribute( *it, m_nodeNames, name );
        if ( properties.hasAttribute( name ) )
            return true;
    }

    return false;
}

QString StyleStack::attribute( const QString& name ) const
{
    // TODO: has to be fixed for complex styles like list-styles
    QList<QDomElement>::ConstIterator it = m_stack.end();
    while ( it != m_stack.begin() )
    {
        --it;
        QDomElement properties = searchAttribute( *it, m_nodeNames, name );
        if ( properties.hasAttribute( name ) )
            return properties.attribute( name );
    }

    return QString::null;
}

bool StyleStack::hasAttribute( const QString& name, const QString& detail ) const
{
    QString fullName( name );
    fullName += '-';
    fullName += detail;
    QList<QDomElement>::ConstIterator it = m_stack.end();
    while ( it != m_stack.begin() )
    {
        --it;
		QDomElement properties = searchAttribute( *it, m_nodeNames, name, fullName );
        if ( properties.hasAttribute( name ) || properties.hasAttribute( fullName ) )
            return true;
    }

    return false;
}

QString StyleStack::attribute( const QString& name, const QString& detail ) const
{
    QString fullName( name );
    fullName += '-';
    fullName += detail;
    QList<QDomElement>::ConstIterator it = m_stack.end();
    while ( it != m_stack.begin() )
    {
        --it;
        QDomElement properties = searchAttribute( *it, m_nodeNames, name, fullName );
        if ( properties.hasAttribute( fullName ) )
            return properties.attribute( fullName );
        if ( properties.hasAttribute( name ) )
            return properties.attribute( name );
    }

    return QString::null;
}

// Font size is a bit special. "115%" applies to "the fontsize of the parent style".
// This can be generalized though (hasAttributeThatCanBePercentOfParent() ? :)
// Although, if we also add support for fo:font-size-rel here then it's not general anymore.
double StyleStack::fontSize() const
{
    QString name = "fo:font-size";
    double percent = 1;
    QList<QDomElement>::ConstIterator it = m_stack.end();
    while ( it != m_stack.begin() )
    {
        --it;
        QDomElement properties = searchAttribute( *it, m_nodeNames, name );
        if ( properties.hasAttribute( name ) ) {
            QString value = properties.attribute( name );
            if ( value.endsWith( "%" ) )
				percent *= ScCLocale::toDoubleC(value) / 100.0;
            else
                return percent * OODPlug::parseUnit( value ); // e.g. 12pt
        }
    }
    return 0;
}

bool StyleStack::hasChildNode(const QString & name) const
{
    QList<QDomElement>::ConstIterator it = m_stack.end();
    while ( it != m_stack.begin() )
    {
        --it;
        QDomElement properties = searchAttribute( *it, m_nodeNames, name );
        if ( !properties.namedItem( name ).isNull() )
            return true;
    }

    return false;
}

QDomNode StyleStack::childNode(const QString & name) const
{
    QList<QDomElement>::ConstIterator it = m_stack.end();
    while ( it != m_stack.begin() )
    {
        --it;
        QDomElement properties = searchAttribute( *it, m_nodeNames, name );
        if ( !properties.namedItem( name ).isNull() )
            return properties.namedItem( name );
    }

    return QDomNode();          // a null node
}

static bool isUserStyle( const QDomElement& e )
{
    QDomElement parent = e.parentNode().toElement();
    return parent.tagName() == "office:styles";
}

QString StyleStack::userStyleName() const
{
    QList<QDomElement>::ConstIterator it = m_stack.end();
    while ( it != m_stack.begin() )
    {
        --it;
        if ( isUserStyle( *it ) )
            return (*it).attribute("style:name");
    }
    // Can this ever happen?
    return "Standard";
}

void StyleStack::fillNodeNameList( QStringList& names, const StyleStack::Mode mode )
{
	if ( mode == StyleStack::OODraw2x )
	{
		names.append("style:graphic-properties");
		names.append("style:paragraph-properties");
		names.append("style:page-layout-properties");
		names.append("style:drawing-page-properties");
		names.append("style:text-properties");
	}
	else
		names.append("style:properties");		
}

QDomElement StyleStack::searchAttribute( const QDomElement& element, const QStringList& names,const QString& name ) const
{
	QDomElement node;
	QDomNodeList childNodes;
	childNodes = element.childNodes();
	for ( int i = 0; i < childNodes.count(); i++ )
	{
		QDomNode n = childNodes.item(i);
		if ( n.isElement() )
		{
			QDomElement* e = (QDomElement*) (&n);
			if ( (names.indexOf(e->nodeName()) >= 0) && e->hasAttribute(name) )
			{
				node = *e;
				break;
			}
		}
	}
	return node;
}

QDomElement StyleStack::searchAttribute( const QDomElement& element, const QStringList& names, const QString& name, const QString& fullName ) const
{
	QDomElement node;
	QDomNodeList childNodes;
	childNodes = element.childNodes();
	for ( int i = 0; i < childNodes.count(); i++ )
	{
		QDomNode n = childNodes.item(i);
		if ( n.isElement() )
		{
			QDomElement* e = (QDomElement*) (&n);
			if ( (names.indexOf(e->nodeName()) >= 0) && (e->hasAttribute(name) || e->hasAttribute(fullName)) )
			{
				node = *e;
				break;
			}
		}
	}
	return node;
}
