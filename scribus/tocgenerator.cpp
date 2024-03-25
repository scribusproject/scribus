/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
    begin                : Jun 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@scribus.info
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

#include <memory>

#include <QDebug>
#include <QMap>
#include <QString>

#include "gtframestyle.h"
#include "gtparagraphstyle.h"
#include "gtwriter.h"
#include "pageitem.h"
#include "pageitemiterator.h"
#include "pagestructs.h"
#include "scpage.h"
#include "scribusdoc.h"
#include "text/storytext.h"

TOCGenerator::TOCGenerator(QObject *parent, ScribusDoc *doc)
            : QObject(parent), m_doc(doc)
{

}

void TOCGenerator::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
}

PageItem* TOCGenerator::findTargetFrame(const QString &targetFrameName)
{
	if (!m_doc)
		return nullptr;

	for (int i = 0; i < m_doc->DocItems.count(); ++i)
	{
		PageItem* docItem = m_doc->DocItems.at(i);
		if (docItem == nullptr)
			continue;
		if (docItem->itemType() != PageItem::TextFrame)
			continue;
		if (docItem->itemName() == targetFrameName)
			return docItem;
	}
	return nullptr;
}

void TOCGenerator::generateByAttribute()
{
	if (m_doc == nullptr)
		return;
	Q_ASSERT(!m_doc->masterPageMode());

	const ToCSetupVector& tocSetups = m_doc->tocSetups();
	for (auto tocSetupIt = tocSetups.cbegin(); tocSetupIt != tocSetups.cend(); ++tocSetupIt)
	{
		if (tocSetupIt->tocSource == "Style")
			continue;
		PageItem* tocFrame = findTargetFrame(tocSetupIt->frameName);
		if (tocFrame == nullptr)
			continue;

		const PageItem *currentDocItem;
		QMap<QString, QString> tocMap;

		std::unique_ptr<int[]> pageCounter(new int[m_doc->DocPages.count()]);
		for (int i = 0; i < m_doc->DocPages.count(); ++i)
			pageCounter[i] = 0;

		int pageNumberWidth = QString("%1").arg(m_doc->DocPages.count()).length();

		for (PageItemIterator itemIter(m_doc->DocItems); *itemIter; ++itemIter)
		{
			currentDocItem = itemIter.current();
			if (currentDocItem == nullptr)
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
				QString key = QString("%1,%2,%3").arg(pageID, tocID, sectionID);
				tocMap.insert(key, objAttr.value);
			}
		}

		//Set up the gtWriter instance with the selected paragraph style
		gtWriter writer(false, tocFrame);
		writer.setUpdateParagraphStyles(false);
		writer.setOverridePStyleFont(false);
		const gtFrameStyle* fstyle = writer.getDefaultStyle();
		gtParagraphStyle* pstyle = new gtParagraphStyle(*fstyle);
		pstyle->setName(tocSetupIt->textStyle);
		writer.setParagraphStyle(pstyle);
		
		QString oldTocPage;
		for (QMap<QString, QString>::Iterator tocIt = tocMap.begin(); tocIt != tocMap.end(); ++tocIt)
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
	}
}

void TOCGenerator::generateByStyle()
{
	if (m_doc == nullptr)
		return;
	Q_ASSERT(!m_doc->masterPageMode());

	struct ItemPosInfo
	{
		PageItem* item;
		double xPos; // Absolute x position in document
		double yPos; // Absolute y position in document
	};

	// Collect all text frames including those placed inside groups;
	QVector<ItemPosInfo> allTextFramePos;
	allTextFramePos.reserve(100);

	QStack<QList<PageItem*> > itemsStack;
	itemsStack.push(m_doc->DocItems);

	while (!itemsStack.isEmpty())
	{
		QList<PageItem*> itemList = itemsStack.pop();
		for (int i = 0; i < itemList.count(); ++i)
		{
			PageItem* item = itemList.at(i);
			if (item->isGroup())
			{
				itemsStack.push(item->asGroupFrame()->groupItemList);
				continue;
			}
			if (!item->isTextFrame())
				continue;

			ItemPosInfo itemPos { item, item->xPos(), item->yPos() };
			if (item->Parent)
			{
				QTransform itemTrans = item->getTransform();
				QPointF itemPoint = itemTrans.map(QPointF(0.0, 0.0));
				itemPos.xPos = itemPoint.x();
				itemPos.yPos = itemPoint.y();
			}
			allTextFramePos.append(itemPos);
		}
	}

	// Start items by y position, x ascending
	// Note : this will have to be changed once we support document binding on the right
	std::stable_sort(allTextFramePos.begin(), allTextFramePos.end(), [](const ItemPosInfo & pos1, const ItemPosInfo & pos2) -> bool
					 {
						 if (pos1.yPos < pos2.yPos)
							 return true;
						 if (pos1.yPos == pos2.yPos)
							 return (pos1.xPos < pos2.xPos);
						 return false;
					 });


	ToCSetupVector &tocSetups = m_doc->tocSetups();
	for (auto tocSetupIt = tocSetups.begin(); tocSetupIt != tocSetups.end(); ++tocSetupIt)
	{
		if (tocSetupIt->tocSource == "Attribute")
			continue;
		PageItem *tocFrame = findTargetFrame(tocSetupIt->frameName);
		if (tocFrame == nullptr)
			continue;
		tocFrame->clearContents();
		QMap<QString, QString> tocMap;
		QMap<QString, QString> styleMap;
		QMap<QString, TOCPageLocation> pageLocationMap;

		std::unique_ptr<int[]> pageCounter(new int[m_doc->DocPages.count()]);
		for (int i = 0; i < m_doc->DocPages.count(); ++i)
			pageCounter[i] = 0;

		//Set up the gtWriter instance with the selected paragraph style
		gtWriter writer(false, tocFrame);
		writer.setUpdateParagraphStyles(false);
		writer.setOverridePStyleFont(false);
		int pageNumberWidth = QString("%1").arg(m_doc->DocPages.count()).length();
		for (int j = 0; j < allTextFramePos.count(); ++j)
		{
			PageItem* item = allTextFramePos.at(j).item;

			if (item == nullptr)
				continue;
			if (item->itemType() != PageItem::TextFrame)
				continue;
			if (item->itemName() == tocSetupIt->frameName)
				continue;
			//Item not on a page, continue
			if (item->OwnPage == -1)
				continue;
			//If we don't want to list non printing frames and this one is set to not print, continue
			if (!tocSetupIt->listNonPrintingFrames && !item->printEnabled())
				continue;
			//get the frame text
			StoryText story = item->itemText;
			QString pageID = QString("%1").arg(item->OwnPage + m_doc->FirstPnum, pageNumberWidth);
			QString sectionID = m_doc->getSectionPageNumberForPageIndex(item->OwnPage);
			int i = item->firstInFrame();
			while (i <= item->lastInFrame())
			{
				int pno = item->itemText.nrOfParagraph(i);
				int pstart = item->itemText.startOfParagraph(pno);
				int pend = item->itemText.endOfParagraph(pno);
				QString pname(item->itemText.paragraphStyle(i).parentStyle()->name());
				for (QList<ToCSetupEntryStyleData>::Iterator tocEntryIterator = tocSetupIt->entryData.begin();
					 tocEntryIterator != tocSetupIt->entryData.end(); ++tocEntryIterator)
				{
					if ((*tocEntryIterator).styleToFind == pname)
					{
						QString tocID = QString("%1").arg(pageCounter[item->OwnPage]++, 3, 10, QChar('0'));
						QString key = QString("%1,%2,%3").arg(pageID, tocID, sectionID);
						QString paraText = item->itemText.text(pstart, pend - pstart);
						if ((*tocEntryIterator).removeLineBreaks)
							paraText.remove(SpecialChars::LINEBREAK);
						paraText.remove(SpecialChars::COLBREAK);
						paraText.remove(SpecialChars::FRAMEBREAK);
						tocMap.insert(key, paraText);
						styleMap.insert(key, (*tocEntryIterator).styleForText);
						pageLocationMap.insert(key, (*tocEntryIterator).pageLocation);
					}
				}
				i = item->itemText.nextParagraph(i) + 1;
			}
		}
		QString oldTocPage;
		for (QMap<QString, QString>::Iterator tocIt = tocMap.begin(); tocIt != tocMap.end(); ++tocIt)
		{
			QString t = tocIt.key();
			QString t2 = tocIt.value();
			QString tocPage(tocIt.key().section(',', 2, 2).trimmed());
			QString tocLine;
			//Start with text or numbers
			TOCPageLocation tpl = pageLocationMap.value(tocIt.key());
			if (tpl == End || tpl == NotShown)
				tocLine = tocIt.value();
			if (tpl == Beginning && oldTocPage != tocPage)
				tocLine = tocPage;
			//Add in the tab for the leaders
			tocLine += "\t";
			//End with text or numbers
			if (tpl == Beginning)
				tocLine += tocIt.value();
			if (tpl == End && oldTocPage != tocPage)
				tocLine += tocPage;
			tocLine += "\n";

			const gtFrameStyle *fstyle = writer.getDefaultStyle();
			gtParagraphStyle *pstyle = new gtParagraphStyle(*fstyle);
			pstyle->setName(styleMap.value(tocIt.key()));
			writer.setParagraphStyle(pstyle);
			writer.append(tocLine);
		}
	}
}
