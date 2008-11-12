//
// C++ Interface: pageitempointer
//
// Description:
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef PAGEITEMPOINTER_H
#define PAGEITEMPOINTER_H

#include <QDataStream>
#include <QMap>
#include <QObject>
#include <QPointer>
#include <QString>


class PageItem;

class PageItemPointerDB : public QObject ,public QMap<QString, PageItem*> 
{
		Q_OBJECT
	public:
		PageItemPointerDB(){}
		~PageItemPointerDB(){}
		
		void addToDB(const QString& key, PageItem* value );
				
	public slots:
		void removeFromDB ( QObject* obj );
};

/**
 * Holds a guarded pointer and offers some kind of serialization.
 *
 */

class PageItemPointer :  public QPointer<PageItem>
{
	static PageItemPointerDB PIdb;
	public:
		PageItemPointer() :QPointer<PageItem>() {}
		PageItemPointer ( PageItem* pi );
		PageItemPointer ( const PageItemPointer& pip );
		PageItemPointer ( const QString& objname );
		~PageItemPointer();


};

QDataStream & operator<< ( QDataStream & out , const PageItemPointer& pip );
QDataStream & operator>> ( QDataStream & in  , PageItemPointer& pip );




#endif

