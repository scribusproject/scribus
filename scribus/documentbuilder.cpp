/*
 *  documentbuilder.cpp
 *  
 *
 *  Created by Andreas Vox on 01.06.12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "documentbuilder.h"

#include "appmodes.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"
#include "pageitem_latexframe.h"
#include "pageitem_line.h"
#include "pageitem_pathtext.h"
#include "pageitem_polygon.h"
#include "pageitem_polyline.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#ifdef HAVE_OSG
#include "pageitem_osgframe.h"
#endif
#include "pageitem_symbol.h"
#include "pageitem_group.h"
#include "pageitem_regularpolygon.h"
#include "pageitem_arc.h"
#include "pageitem_spiral.h"
#include "scpage.h"
#include "scribusdoc.h"



DocumentBuilder::DocumentBuilder() 
{
	m_doc = createDocument();
}

DocumentBuilder::DocumentBuilder(ScribusDoc* doc) 
{
	m_doc = doc;
}


DocumentBuilder::~DocumentBuilder()
{}



ScribusDoc* DocumentBuilder::createDocument()
{
	ScribusDoc* doc = new ScribusDoc();

	doc->is12doc = false;
	doc->appMode = modeNormal;
	doc->HasCMS = false;
	//doc->setActiveLayer(0); //CB should not need this, the file load process sets it to ALAYER from the doc
	doc->OpenNodes.clear();
	doc->setLoading(true);
	doc->SoftProofing = false;
	doc->Gamut = false;
	
	return doc;
}


void DocumentBuilder::setDocument(ScribusDoc* doc)
{
	m_doc = doc;
}


ScribusDoc* DocumentBuilder::finishDocument(ScribusDoc* doc)
{
	return doc;
}

   
ScPage* DocumentBuilder::createScPage(int number, const QString& name)
{
	m_doc->setMasterPageMode(!name.isEmpty());
	return name.isEmpty() ? m_doc->addPage(number) : m_doc->addMasterPage(number, name);
}


ScPage* DocumentBuilder::finishScPage(ScPage* page)
{
	if (!page->pageName().isEmpty())
		page->MPageNam = QString("");
	else if (page->MPageNam.isEmpty())
		page->MPageNam = QString("Normal");
	page->setInitialWidth(page->width());
	page->setInitialHeight(page->height());
	return page;
}


PageItem* DocumentBuilder::createPageItem(int itemTypeI, int frameTypeI, double x, double y, double b, double h, double w, const QString& fill, const QString& outline)
{
	const PageItem::ItemType itemType = static_cast<PageItem::ItemType> (itemTypeI);
	const PageItem::ItemFrameType frameType = static_cast<PageItem::ItemFrameType> (frameTypeI);
	
	PageItem* newItem = m_doc->createPageItem(itemType, frameType, x,y,b,h,w,fill, outline);
	m_doc->Items->append(newItem);
	return newItem;
}


PageItem* DocumentBuilder::finishPageItem(PageItem* newItem)
{
	if (newItem->asImageFrame() || newItem->asLatexFrame())
	{
		if (!newItem->Pfile.isEmpty())
		{
				m_doc->loadPict(newItem->Pfile, newItem, false);
				/* if (newItem->pixm.imgInfo.PDSpathData.contains(clipPath))
				{
					newItem->imageClip = newItem->pixm.imgInfo.PDSpathData[clipPath].copy();
					newItem->pixm.imgInfo.usedPath = clipPath;
					QTransform cl;
					cl.translate(newItem->imageXOffset()*newItem->imageXScale(), newItem->imageYOffset()*newItem->imageYScale());
					cl.scale(newItem->imageXScale(), newItem->imageYScale());
					newItem->imageClip.map(cl);
				}
				if (layerFound)
				{
					newItem->pixm.imgInfo.isRequest = true;
					m_doc->loadPict(newItem->Pfile, newItem, true);
				} */
		}
	}
	return newItem;
}

