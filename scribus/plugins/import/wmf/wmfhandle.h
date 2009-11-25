/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/* Code inspired by KOffice libwmf and adapted for Scribus by Jean Ghali */

#ifndef WMFHANDLE_H
#define WMFHANDLE_H

#include "wmfcontext.h"

class WmfObjHandle
{
public:
    virtual void apply( WMFContext& p ) = 0;
};

class WmfObjBrushHandle: public WmfObjHandle
{
public:
    virtual void apply( WMFContext& p );
    QBrush brush;
    virtual ~WmfObjBrushHandle() {};
};

class WmfObjPenHandle: public WmfObjHandle
{
public:
    virtual void apply( WMFContext& p );
    QPen pen;
    virtual ~WmfObjPenHandle() {};
};

class WmfObjPatternBrushHandle: public WmfObjHandle
{
public:
    virtual void apply( WMFContext& p );
    QBrush brush;
    QPixmap image;
    virtual ~WmfObjPatternBrushHandle() {};
};

class WmfObjFontHandle: public WmfObjHandle
{
public:
    virtual void apply( WMFContext& p );
	int    charset;
    QFont  font;
    double rotation;
	WmfObjFontHandle() { charset = 1; rotation = 0.0; }
    virtual ~WmfObjFontHandle() {};
};

void WmfObjBrushHandle::apply( WMFContext& p )
{
    p.setBrush( brush );
}

void WmfObjPenHandle::apply( WMFContext& p )
{
    p.setPen( pen );
}

void WmfObjPatternBrushHandle::apply( WMFContext& p )
{
    p.setBrush( brush );
}

void WmfObjFontHandle::apply( WMFContext& p )
{
	p.setTextCharset( charset );
	p.setTextRotation( rotation );
    p.setFont( font );
}

#endif
