/***************************************************************************
 *   Copyright (C) 2009 by Pierre Marchand                                 *
 *   pierre@oep-h.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef PAGEITEMSETTERBASE_H
#define PAGEITEMSETTERBASE_H

#include <QWidget>
#include <QString>


//Not exactly clear what would be the best but we go for the Selection atm -pm
//class PageItem;
class Selection;

/**
*  PageItemSetterBase is base class for any widget that can set
*  properties of a page item.
*
* note about dragging:
* this base class is expected to handle propper dragging, droppers
* must ask the pageitemsettersmanager a clone of a dropped setter based
* on the name.
*
*/
class PageItemSetterBase : public QWidget
{
public:
	/**
	  * Default constructor
	  */
	PageItemSetterBase(QWidget * parent = 0);

	/**
	*  Reset parameters and internal state to reflect properties of
	*  newly assigned (page item)^^selection .
	*/
	virtual void changeItem(Selection*) = 0;

	/**
	  * Return a full name which first identifies the type of a setter and can be used to assossiate setters
	  * and generally help the caller to layout widgets (think pages in PP as of 2009 :)
	  * Prototype of a name could be (retaining actual layout in PP):
	  * type.group.page or the reverse page.group.type
	  */
//	let put this in objectName which is yet available.
//	virtual QString name() const = 0;

	/**
	  * Return a setter on same property
	  */
	virtual PageItemSetterBase* clone() = 0;


protected:
	QPoint startDragPoint;
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);

	/**
	  * Enable/Disable only children because doing the same
	  * on the widget itself disables dragging.
	  */
	virtual void activate(const bool& act);

};
#endif // PAGEITEMSETTERBASE_H
