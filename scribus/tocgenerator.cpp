/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
    begin                : Jun 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "tocgenerator.h"

#include <QMap>
#include <QString>

#include "gtframestyle.h"
#include "gtparagraphstyle.h"
#include "gtwriter.h"
#include "pageitem.h"
#include "pagestructs.h"
#include "scpage.h"
#include "scribusdoc.h"

TOCGenerator::TOCGenerator(QObject *parent, ScribusDoc *doc) : QObject(parent)
{
	m_doc = doc;
}

void TOCGenerator::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
}

PageItem* TOCGenerator::findTargetFrame(const QString &targetFrameName)
{
	PageItem* targetFrame=NULL;
	if (m_doc != NULL)
	{
		for (int d = 0; d < m_doc->DocItems.count(); ++d)
		{
			if (m_doc->DocItems.at(d) != NULL)
			{
				if (m_doc->DocItems.at(d)->itemType()==PageItem::TextFrame && m_doc->DocItems.at(d)->itemName()==targetFrameName)
				{
					targetFrame=m_doc->DocItems.at(d);
					break;
				}
			}
		}
	}
	return targetFrame;
}

void TOCGenerator::generateDefault()
{
	if (m_doc == NULL)
		return;
	Q_ASSERT(!m_doc->masterPageMode());
	for(ToCSetupVector::Iterator tocSetupIt = m_doc->tocSetups().begin(); tocSetupIt != m_doc->tocSetups().end(); ++tocSetupIt )
	{
		PageItem* tocFrame = findTargetFrame(tocSetupIt->frameName);
		if (tocFrame == NULL)
			continue;

		PageItem *currentDocItem;
		QMap<QString, QString> tocMap;

		uint *pageCounter = new uint[m_doc->DocPages.count()];
		if (pageCounter == NULL)
			return;
		uint pageNumberWidth = QString("%1").arg(m_doc->DocPages.count()).length();
		for (int i = 0; i < m_doc->DocPages.count(); ++i)
			pageCounter[i] = 0;

		for (int d = 0; d < m_doc->DocItems.count(); ++d)
		{
			currentDocItem = m_doc->DocItems.at(d);
			if (currentDocItem == NULL)
				continue;
			//Item not on a page, continue
			if (currentDocItem->OwnPage == -1)
				continue;
			//If we don't want to list non printing frames and this one is set to not print, continue
			if (!tocSetupIt->listNonPrintingFrames && !currentDocItem->printEnabled())
				continue;

			ObjectAttribute objAttr;
			QList<ObjectAttribute> objAttrs = currentDocItem->getObjectAttributes(tocSetupIt->itemAttrName);
			if (objAttrs.count() <= 0)
				continue;

			QString pageID = QString("%1").arg(currentDocItem->OwnPage + m_doc->FirstPnum, pageNumberWidth);
			QString sectionID = m_doc->getSectionPageNumberForPageIndex(currentDocItem->OwnPage);

			for (int i = 0; i < objAttrs.count(); ++i)
			{
				objAttr = objAttrs.at(i);
				if (objAttr.name.isNull())
					continue;

				//The key is generated to produce a sequence of numbers for the page numbers
				//First is the page of the item
				//Second is an incremented counter for the item so multiple per page works
				//Third is the section based page number which is actually used in the TOC.
				QString tocID = QString("%1").arg(pageCounter[currentDocItem->OwnPage]++, 3 , 10, QChar('0'));
				QString key   = QString("%1,%2,%3").arg(pageID).arg(tocID).arg(sectionID);
				tocMap.insert(key, objAttr.value);
			}
		}

		//Set up the gtWriter instance with the selected paragraph style
		gtWriter writer(false, tocFrame);
		writer.setUpdateParagraphStyles(false);
		writer.setOverridePStyleFont(false);
		gtFrameStyle* fstyle = writer.getDefaultStyle();
		gtParagraphStyle* pstyle = new gtParagraphStyle(*fstyle);
		pstyle->setName(tocSetupIt->textStyle);
		writer.setParagraphStyle(pstyle);
		
		QString oldTocPage = QString::null;
		for (QMap<QString, QString>::Iterator tocIt=tocMap.begin(); tocIt != tocMap.end();++tocIt)
		{
			QString tocPage(tocIt.key().section( ',', 2, 2 ).trimmed());
			QString tocLine;
			//Start with text or numbers
			if (tocSetupIt->pageLocation == End || tocSetupIt->pageLocation == NotShown)
				tocLine = tocIt.value();
			if (tocSetupIt->pageLocation == Beginning && oldTocPage != tocPage)
				tocLine = tocPage;
			//Add in the tab for the leaders
			tocLine += "\t";
			//End with text or numbers
			if (tocSetupIt->pageLocation == Beginning)
				tocLine += tocIt.value();
			if (tocSetupIt->pageLocation == End && oldTocPage != tocPage)
				tocLine += tocPage;
			tocLine += "\n";
			writer.append(tocLine);
		}

		delete[] pageCounter;
	}
}
