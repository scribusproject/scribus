/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
//
//
// Author: Craig Bradney <cbradney@zip.com.au>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifndef INSERTAFRAME_H
#define INSERTAFRAME_H

#include "insertaframebase.h"
#include "scribusapi.h"

class SCRIBUS_API InsertAFrame : public InsertAFrameBase
{
    Q_OBJECT

public:
	InsertAFrame(QWidget* parent);
	~InsertAFrame(){};
	
};

#endif
