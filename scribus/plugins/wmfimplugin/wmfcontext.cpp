/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <iostream>
#include "wmfcontext.h"

using namespace std;

WMFGraphicsState::WMFGraphicsState(void)
				: windowOrg(0.0, 0.0),
				  windowExt(1.0, 1.0),
				  viewportOrg(0.0, 0.0),
				  viewportExt(1.0, 1.0),
				  position (0.0, 0.0),
				  textAlign(0),
				  textCharset(1), //DEFAULT_CHARSET
				  textRotation(0),
				  backgroundMode(Qt::TransparentMode),
				  windingFill(false)
{

}

void WMFGraphicsState::setWindowOrg(double x, double y)
{
	windowOrg = QPointF(x, y);
	updateWorldMatrix();
}

void WMFGraphicsState::setWindowExt(double x, double y)
{
	windowExt = QSizeF(x, y);
	updateWorldMatrix();
}

void WMFGraphicsState::setViewportOrg(double x, double y)
{
	viewportOrg = QPointF(x, y);
	updateWorldMatrix();
}

void WMFGraphicsState::setViewportExt(double x, double y)
{
	viewportExt = QSizeF(x, y);
	updateWorldMatrix();
}

void WMFGraphicsState::updateWorldMatrix(void)
{
	double xscale = (windowExt.width()  != 0.0) ? (viewportExt.width()  / windowExt.width())  : 1.0;
	double yscale = (windowExt.height() != 0.0) ? (viewportExt.height() / windowExt.height()) : 1.0;
	double dx     = viewportOrg.x() - xscale * windowOrg.x();
	double dy     = viewportOrg.y() - yscale * windowOrg.y();
	worldMatrix.setMatrix( xscale, 0, 0, yscale, dx, dy );
}

WMFContext::WMFContext(void)
{
	push( WMFGraphicsState() ) ;
}

void WMFContext::save(void)
{
	if ( this->count() > 0 )
		push( WMFGraphicsState(top()) );
	else
	{
		cerr << "WMFContext : unexpected save call" << endl;
	}
}

void WMFContext::restore(void)
{
	if ( this->count() > 1 )
		pop();
	else
	{
		cerr << "WMFContext : unexpected restore call" << endl; 
	}
}

void WMFContext::reset(void)
{
	clear();
	push( WMFGraphicsState() ) ;
}

WMFGraphicsState& WMFContext::current(void)
{
	if ( this->count() <= 0 )
		push( WMFGraphicsState() ) ;
	return top();
}
