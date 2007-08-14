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
				  position (0.0, 0.0),
				  backgroundMode(Qt::TransparentMode),
				  windingFill(false)
{

}

void WMFGraphicsState::setWindowOrg(double x, double y)
{
	double xscale = worldMatrix.m11();
	double yscale = worldMatrix.m22();
	worldMatrix.setMatrix( xscale, 0, 0, yscale, -x, -y);
	windowOrg = QPointF(x, y);
}

void WMFGraphicsState::setWindowExt(double x, double y)
{
	if ( (x != 0.0) && (y != 0.0) ) 
	{
		QSizeF ext = windowExt;
		double xscale = (ext.width() / x);
		double yscale = (ext.height() / y);
		worldMatrix.setMatrix(xscale, 0, 0, yscale, -windowOrg.x(), -windowOrg.y());
		windowExt = QSizeF(x, y);
	}
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
