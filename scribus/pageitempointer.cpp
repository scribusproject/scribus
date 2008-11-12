//
// C++ Implementation: pageitempointer
//
// Description: 
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "pageitempointer.h"

#include "pageitem.h"

void PageItemPointerDB::addToDB(const QString & key, PageItem * value)
{
	insert(key,value);
	connect ( value,SIGNAL( destroyed ( QObject* ) ),this,SLOT( removeFromDB ( QObject* ) ) );
}

void PageItemPointerDB::removeFromDB ( QObject * obj )
{
	if ( obj )
	{
		QString k ( obj->objectName() );
		remove ( k );
	}
}

/// PageItemPointer
PageItemPointerDB PageItemPointer::PIdb;
PageItemPointer::PageItemPointer ( PageItem * pi ) :QPointer<PageItem> ( pi )
{
	if ( !isNull() )
	{
		PIdb.addToDB( pi->objectName(), pi);
		
	}
}

PageItemPointer::PageItemPointer ( const PageItemPointer & pip ) :QPointer<PageItem> (PIdb[ pip->objectName() ])
{
}

PageItemPointer::PageItemPointer(const QString & objname):QPointer<PageItem>(PIdb[objname])
{
}

PageItemPointer::~PageItemPointer()
{

}

QDataStream &operator << ( QDataStream & out , const PageItemPointer& pip )
{
	out << pip->objectName();
	return out;
}

QDataStream &operator >> ( QDataStream & in , PageItemPointer& pip )
{
	QString piName;
	in >>  piName;
	pip = PageItemPointer(piName);
	return in;
}



