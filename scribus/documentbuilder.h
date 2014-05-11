/*
 *  documentbuilder.h
 *  
 *
 *  Created by Andreas Vox on 01.06.12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "scribusapi.h"


#ifndef DOCUMENTBUILDER_H
#define DOCUMENTBUILDER_H


class QString;
class ScribusDoc;
class ScPage;
class PageItem;


class SCRIBUS_API DocumentBuilder {
    ScribusDoc* m_doc;

public:

	DocumentBuilder();
	DocumentBuilder(ScribusDoc*);
	virtual ~DocumentBuilder();

	virtual ScribusDoc* createDocument();
	virtual void setDocument(ScribusDoc*);
	virtual ScribusDoc* finishDocument(ScribusDoc*);

	virtual ScPage* createScPage(int number, const QString& name);
	virtual ScPage* finishScPage(ScPage*);

	virtual PageItem* createPageItem(int itemType, int frameType, double x, double y, double b, double h, double w, const QString& fill, const QString& outline);
	virtual PageItem* finishPageItem(PageItem*);

	ScribusDoc* document()
	{
		return m_doc;
	}
};

#endif
