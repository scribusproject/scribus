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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef STYLESTACK_H
#define STYLESTACK_H

#include <QDomElement>
#include <QStringList>
#include <QList>
#include <QStack>


/**
 *  This class implements a stack for the different styles of an object.
 *
 *  There can be several styles that are valid for one object. For example
 *  a textobject on a page has styles 'pr3' and 'P7' and a paragraph in
 *  that textobject has styles 'P1' and 'T3'. And some styles even have
 *  parent-styles...
 *
 *  If you want to know if there is, for example,  the attribute 'fo:font-family'
 *  for this paragraph, you have to look into style 'T3', 'P1', 'P7' and 'pr3'.
 *  When you find this attribute in one style you have to stop processing the list
 *  and take the found attribute for this object.
 *
 *  This is what this class does. You can push styles on the stack while walking
 *  through the xml-tree to your object and then ask the stack if any of the styles
 *  provides a certain attribute. The stack will search from top to bottom, i.e.
 *  in our example from 'T3' to 'pr3' and return the first occurrence of the wanted
 *  attribute.
 *
 *  So this is some sort of inheritance where the styles on top of the stack overwrite
 *  the same attribute of a lower style on the stack.
 */
class StyleStack
{
public:
    StyleStack();
    virtual ~StyleStack();

	enum Mode
	{
		OODraw1x = 1,
		OODraw2x = 2
	};

	/**
     * Set attribute analysis mode.
     */
	void setMode( const StyleStack::Mode mode );

    /**
     * Clears the complete stack.
     */
    void clear();

    /**
     * Save the current state of the stack. Any items added between
     * this call and its corresponding restore() will be removed when calling restore().
     */
    void save();

    /**
     * Restore the stack to the state it was at the corresponding save() call.
     */
    void restore();

    /**
     * Removes the style on top of the stack.
     */
    void pop();

    /**
     * Pushes the new style onto the stack.
     */
    void push( const QDomElement& style );

    /**
     * Check if any of the styles on the stack has an attribute called 'name'.
     */
	bool hasAttribute( const QString& name ) const;

    /**
     * Search for the attribute called 'name', starting on top of the stack,
     * and return it.
     */
    QString attribute( const QString& name ) const;

    /**
     * Check if any of the styles on the stack has an attribute called 'name'-'detail'
     * where detail is e.g. left, right, top or bottom.
     * This allows to also find 'name' alone (e.g. padding implies padding-left, padding-right etc.)
     */
    bool hasAttribute( const QString& name, const QString& detail ) const;

    /**
     * Search for the attribute called 'name', starting on top of the stack,
     * and return it.
     */
	QString attribute( const QString& name, const QString& detail ) const;

    /**
     * Check if any of the styles on the stack has a child node called 'name'.
     */
    bool hasChildNode(const QString & name) const;

    /**
     * Search for a child node called 'name', starting on top of the stack,
     * and return it.
     */
    QDomNode childNode(const QString & name) const;

    /**
     * Special case for the current font size, due to special handling of fo:font-size="115%".
     */
    double fontSize() const;

    /**
     * Return the name of the style specified by the user,
     * i.e. not an auto style
     */
    QString userStyleName() const;

private:

	// Node names to look for style properties
	QStringList m_nodeNames;

    // For save/restore: stack of "marks". Each mark is an index in m_stack.
    QStack<int> m_marks;

    // We use QValueList instead of QValueStack because we need access to all styles
    // not only the top one.
    QList<QDomElement> m_stack;

	// Get node name to look for according to property type
	void fillNodeNameList( QStringList& names, const StyleStack::Mode mode );

	// Search a specific attribute amongst childs of an element
	QDomElement searchAttribute( const QDomElement& element, const QStringList& names,const QString& name ) const;

	// Search a specific attribute amongst childs of an element
	QDomElement searchAttribute( const QDomElement& element, const QStringList& names, const QString& name, const QString& fullName ) const;
};


#endif /* STYLESTACK_H */
