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

#include <vector>

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

		auto pageCounter = std::vector<int>(m_doc->DocPages.count(), 0);

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
				QString tocID = QString("%1").arg(pageCounter.at(currentDocItem->OwnPage)++, 3 , 10, QChar('0'));
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

	struct TOCEntryData
	{
		QString entryText;
		QString entryStyle;
		TOCPageLocation entryPageLocation;
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
	std::stable_sort(allTextFramePos.begin(), allTextFramePos.end(), [](const ItemPosInfo &pos1, const ItemPosInfo &pos2) -> bool
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
		QMap<QString, TOCEntryData> tocMap;
		auto pageCounter = std::vector<int>(m_doc->DocPages.count(), 0);

		//Set up the gtWriter instance with the selected paragraph style
		gtWriter writer(false, tocFrame);
		writer.setUpdateParagraphStyles(false);
		writer.setOverridePStyleFont(false);
		int pageNumberWidth = QString("%1").arg(m_doc->DocPages.count()).length();
		for (int j = 0; j < allTextFramePos.count(); ++j)
		{
			PageItem* item = allTextFramePos.at(j).item;
			//If the item doesn't exist, of it not a text frame, or if the name is the same as the TOC destination, or its not on a page continue.
			if (item == nullptr)
				continue;
			if (item->itemType() != PageItem::TextFrame)
				continue;
			if (item->itemName() == tocSetupIt->frameName)
				continue;
			if (item->OwnPage == -1)
				continue;
			//If we don't want to list non printing frames and this one is set to not print, continue
			if (!tocSetupIt->listNonPrintingFrames && !item->printEnabled())
				continue;
			int i = item->firstInFrame();
			while (i <= item->lastInFrame())
			{
				//Empty paragraph, continue
				if (item->itemText.text(i) == SpecialChars::PARSEP)
				{
					// qDebug() << i << "Empty paragraph, continuing, PARSEP";
					++i;
					continue;
				}
				int para_no = item->itemText.nrOfParagraph(i);
				int para_start = item->itemText.startOfParagraph(para_no);
				int para_end = item->itemText.endOfParagraph(para_no);
				QString paraText = item->itemText.text(para_start, para_end - para_start);
				// qDebug() << "Paragraph Text:" << paraText;
				// qDebug() << "Paragraph start/end:" << para_start << para_end;

				//Paragraph starts before this frame, eg paragraph wrapped into next frame in chain but is not caused by a FRAMEBREAK, continu
				if (para_start < item->firstInFrame() && !paraText.startsWith(SpecialChars::FRAMEBREAK))
				{
					// qDebug() << "Paragraph starts before this frame and text doesn't start with a frame break, continuing";
					i = item->itemText.nextParagraph(i);
					continue;
				}

				//If the index is already the last in the frame and its a frame break, move to the next paragraph and continue
				//Frame break Scribus goodness!
				if (i == item->lastInFrame() && paraText.startsWith(SpecialChars::FRAMEBREAK))
				{
					// qDebug() << "Frame break goodness, continuing";
					// i = item->itemText.nextParagraph(i) + 1;
					++i;
					continue;
				}

				QString pname(item->itemText.paragraphStyle(i).parentStyle()->name());
				QString pageID = QString("%1").arg(item->OwnPage + m_doc->FirstPnum, pageNumberWidth);
				QString sectionID = m_doc->getSectionPageNumberForPageIndex(item->OwnPage);
				QString tocID = QString("%1").arg(pageCounter.at(item->OwnPage)++, 3, 10, QChar('0'));
				QString key = QString("%1,%2,%3").arg(pageID, tocID, sectionID);
				paraText.remove(SpecialChars::COLBREAK);
				paraText.remove(SpecialChars::FRAMEBREAK);
				for (auto tocEntryIterator = tocSetupIt->entryData.cbegin(); tocEntryIterator != tocSetupIt->entryData.cend(); ++tocEntryIterator)
				{
					if ((*tocEntryIterator).styleToFind == pname)
					{
						if ((*tocEntryIterator).removeLineBreaks)
							paraText.remove(SpecialChars::LINEBREAK);
						TOCEntryData ted;
						ted.entryText = paraText;
						ted.entryStyle = (*tocEntryIterator).styleForText;
						ted.entryPageLocation = (*tocEntryIterator).pageLocation;
						tocMap.insert(key, ted);
					}
				}
				i = item->itemText.startOfNextParagraph(i);
			}
		}
		QString oldTocPage;
		for (auto tocIt = tocMap.begin(); tocIt != tocMap.end(); ++tocIt)
		{
			QString tocPage(tocIt.key().section(',', 2, 2).trimmed());
			QString tocLine;
			//Start with text or numbers
			TOCPageLocation tpl = tocMap.value(tocIt.key()).entryPageLocation;
			if (tpl == End || tpl == NotShown)
				tocLine = tocMap.value(tocIt.key()).entryText;
			if (tpl == Beginning && oldTocPage != tocPage)
				tocLine = tocPage;
			//Add in the tab for the leaders
			tocLine += "\t";
			//End with text or numbers
			if (tpl == Beginning)
				tocLine += tocMap.value(tocIt.key()).entryText;
			if (tpl == End && oldTocPage != tocPage)
				tocLine += tocPage;
			tocLine += "\n";

			const gtFrameStyle *fstyle = writer.getDefaultStyle();
			gtParagraphStyle *pstyle = new gtParagraphStyle(*fstyle);
			pstyle->setName(tocMap.value(tocIt.key()).entryStyle);
			writer.setParagraphStyle(pstyle);
			writer.append(tocLine);
		}
	}
}

void TOCGenerator::generateIndex()
{
	if (m_doc == nullptr)
		return;
	Q_ASSERT(!m_doc->masterPageMode());

	struct IndexEntryData
	{
		QString entryText;
		QString entryLabel;
		QString pageID;
		QString sectionID;
		QString tocID;
		TOCPageLocation entryPageLocation;
	};

	IndexSetupVector &indexSetups = m_doc->indexSetups();
	for (auto indexSetupIt = indexSetups.begin(); indexSetupIt != indexSetups.end(); ++indexSetupIt)
	{
		PageItem *indexFrame = findTargetFrame(indexSetupIt->frameName);
		if (indexFrame == nullptr)
			continue;
		indexFrame->clearContents();
		QMap<QString, IndexEntryData> indexMap;
		QMultiMap<QString, IndexEntryData> indexMultiMap;
		auto pageCounter = std::vector<int>(m_doc->DocPages.count(), 0);

		int pageNumberWidth = QString("%1").arg(m_doc->DocPages.count()).length();

		const QList<Mark *> &marks = m_doc->marksList();
		for (int i = 0; i < marks.size(); ++i)
		{
			if (marks[i]->isType(MARKIndexType))
			{
				QString pageID = QString("%1").arg(marks[i]->OwnPage + m_doc->FirstPnum, pageNumberWidth);
				QString sectionID = m_doc->getSectionPageNumberForPageIndex(marks[i]->OwnPage);
				QString tocID = QString("%1").arg(pageCounter.at(marks[i]->OwnPage)++, 3, 10, QChar('0'));
				QString key = QString("%1,%2,%3,%4").arg(marks[i]->label, pageID, tocID, sectionID);
				QString multikey = QString("%1,%2,%3,%4").arg(marks[i]->getString(), pageID, tocID, sectionID);

				IndexEntryData ied;
				ied.entryLabel = marks[i]->label;
				ied.entryText = marks[i]->getString();
				ied.pageID = pageID;
				ied.sectionID = sectionID;
				ied.tocID = tocID;
				ied.entryPageLocation = End;
				indexMap.insert(key, ied);
				if (!indexSetupIt->caseSensitiveCombination)
					indexMultiMap.insert(marks[i]->getString().toUpper(), ied);
				else
					indexMultiMap.insert(marks[i]->getString(), ied);
			}
		}

		//Set up the gtWriter instance with the selected paragraph style
		gtWriter writer(false, indexFrame);
		writer.setUpdateParagraphStyles(false);
		writer.setOverridePStyleFont(false);
		const gtFrameStyle *fstyle = writer.getDefaultStyle();
		gtParagraphStyle *pstyle = new gtParagraphStyle(*fstyle);
		pstyle->setName(indexSetupIt->headingStyle); //temp heading style uisage
		writer.setParagraphStyle(pstyle);
		QStringList skippedKeys;
		QString oldIndexPage;
		QString firstAlpha;
		bool firstAlphaAdded = false;
		for (auto indexIt = indexMultiMap.begin(); indexIt != indexMultiMap.end(); ++indexIt)
		{
			QString indexPage;
			QString indexLine;
			QStringList pages;
			//Combining entries across the document into one index entry
			if (indexSetupIt->combineIdenticalEntries)
			{
				//Use upper case version of the displayed text if we're case insensitive
				QString tmpKey = indexSetupIt->caseSensitiveCombination ? indexIt.key() : indexIt.key().toUpper();
				//To avoid repeating, add the keys to a list
				if (skippedKeys.contains(tmpKey))
					continue;
				skippedKeys.append(tmpKey);

				auto [i, end] = indexMultiMap.equal_range(tmpKey);
				while (i != end)
				{
					// If we're combining page number entries don't add the page number to the page number list.
					if (!indexSetupIt->combinePageNumbers || (!pages.contains(i.value().pageID)))
						pages.append(i.value().pageID);
					i++;
				}
				//Sort the page list and join back by comma (should probably use QLocale::createSeparatedList based on the para style language)
				pages.sort();
				indexPage = pages.join(",");
			} else
				indexPage = indexIt->pageID;

			QString tmpEntryText(indexIt->entryText);
			//Capitalise our text if requested
			if (indexSetupIt->autoCapitalizeEntries)
			{
				QStringList words = tmpEntryText.split(" ", Qt::SkipEmptyParts);
				for (QString &word : words)
					word.front() = word.front().toUpper();
				tmpEntryText = words.join(" ");
			}
			//If we're adding separators based on the first character of the entries
			if (indexSetupIt->addAlphaSeparators)
			{
				if (firstAlpha.isEmpty() || firstAlpha.front() != tmpEntryText.front())
				{
					firstAlpha = QString("%1\n").arg(tmpEntryText.front());
					firstAlphaAdded = false;
				}
				if (!firstAlphaAdded)
				{
					const gtFrameStyle *fstyle = writer.getDefaultStyle();
					gtParagraphStyle *pstyle = new gtParagraphStyle(*fstyle);
					pstyle->setName(indexSetupIt->separatorStyle);
					writer.setParagraphStyle(pstyle);
					writer.append(firstAlpha);
					firstAlphaAdded = true;
				}
			}
			//Write the index entry text, starting with text or numbers
			TOCPageLocation tpl = indexIt->entryPageLocation;
			if (tpl == End || tpl == NotShown)
				indexLine = tmpEntryText;
			if (tpl == Beginning && oldIndexPage != indexPage)
				indexLine = indexPage;
			//Add in the tab for the leaders
			indexLine += "\t";
			//End with text or numbers
			if (tpl == Beginning)
				indexLine += tmpEntryText;
			if (tpl == End && oldIndexPage != indexPage)
				indexLine += indexPage;
			indexLine += "\n";
			const gtFrameStyle *fstyle = writer.getDefaultStyle();
			gtParagraphStyle *pstyle = new gtParagraphStyle(*fstyle);
			pstyle->setName(indexSetupIt->level1Style);
			writer.setParagraphStyle(pstyle);
			writer.append(indexLine);
		}
	}
}
