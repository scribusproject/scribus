/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          dynamictip.h  -  description
                             -------------------
    begin                : Wed Aug 31 2005
    copyright            : (C) 2005 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DYNAMICTIP_H
#define DYNAMICTIP_H

#include <qtooltip.h>
#include <q3listbox.h>
#include <q3table.h>
#include <qstringlist.h>
#include <q3listview.h>
#include "scribusapi.h"
#include "scribusstructs.h"

// Qt4 FIXME: QToolTip is totally rewritten in the Qt4. We have to use new API.
// Qt4 FIXME: all references and calls of DynamicTip are commented out

// class SCRIBUS_API DynamicTip : public QToolTip
// {
// public:
// 	enum TipType
// 	{
// 		ColorListBox,
// 		Table,
// 		TableHeader,
// 		TreeView
// 	};
// 
// 	DynamicTip( Q3ListBox* parent, ColorList* pale );
// 	DynamicTip( Q3Table* parent );
// 	DynamicTip( Q3Header *parent );
// 	DynamicTip( Q3ListView *parent );
// 	virtual ~DynamicTip() {};
// 	void addHeaderTip(QString tip);
// 	void clearHeaderTips();
// protected:
// 	void maybeTip( const QPoint & );
// 	ColorList* colorList;
// 	Q3ListBox* listB;
// 	Q3Table* table;
// 	Q3Header *header;
// 	QStringList headerTips;
// 	Q3ListView *listV;
// 	TipType kind;
// };

#endif
