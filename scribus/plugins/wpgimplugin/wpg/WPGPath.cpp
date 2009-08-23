/* libwpg
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the 
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA  02111-1301 USA
 *
 * For further information visit http://libwpg.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPGPath.h"
#include "WPGPoint.h"

#include <vector>

namespace libwpg
{

class WPGPathPrivate
{
public:
    WPGPathPrivate() : elements(std::vector<WPGPathElement>()) {};
	std::vector<WPGPathElement> elements;
};

} // namespace libwpg

libwpg::WPGPathElement::WPGPathElement():
	type(NullElement),
	point(WPGPoint()),
	extra1(WPGPoint()),
	extra2(WPGPoint())
{}

libwpg::WPGPath::WPGPath(): closed(true), d(new libwpg::WPGPathPrivate())
{}
	
libwpg::WPGPath::~WPGPath()
{
	delete d;
}
	
libwpg::WPGPath::WPGPath(const libwpg::WPGPath& path): closed(path.closed), framed(path.framed), filled(path.filled), d(new libwpg::WPGPathPrivate())
{
	d->elements = std::vector<WPGPathElement>(path.d->elements);
}
	
libwpg::WPGPath& libwpg::WPGPath::operator=(const libwpg::WPGPath& path)
{
	d->elements = std::vector<WPGPathElement>(path.d->elements);
	return *this;
}
	
unsigned libwpg::WPGPath::count() const
{
	return d->elements.size();
}
	
libwpg::WPGPathElement libwpg::WPGPath::element(unsigned index) const
{
	return d->elements[index];
}
	
void libwpg::WPGPath::moveTo(const libwpg::WPGPoint& point)
{
	libwpg::WPGPathElement tmpElement;
	tmpElement.type = libwpg::WPGPathElement::MoveToElement;
	tmpElement.point = point;
	addElement(tmpElement);
}
	
void libwpg::WPGPath::lineTo(const libwpg::WPGPoint& point)
{
	libwpg::WPGPathElement tmpElement;
	tmpElement.type = libwpg::WPGPathElement::LineToElement;
	tmpElement.point = point;
	addElement(tmpElement);
}
	
void libwpg::WPGPath::curveTo(const libwpg::WPGPoint& c1, const libwpg::WPGPoint& c2, const libwpg::WPGPoint& endPoint)
{
	libwpg::WPGPathElement tmpElement;
	tmpElement.type = libwpg::WPGPathElement::CurveToElement;
	tmpElement.point = endPoint;
	tmpElement.extra1 = c1;
	tmpElement.extra2 = c2;
	addElement(tmpElement);
}
	
void libwpg::WPGPath::addElement(const libwpg::WPGPathElement& elem)
{
	d->elements.push_back(elem);
}

void libwpg::WPGPath::append(const libwpg::WPGPath& path)
{
	for(unsigned i = 0; i < path.count(); i++)
		addElement(path.element(i));
}
