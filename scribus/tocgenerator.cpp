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
#include "tocgenerator.moc"

#include <qmap.h>
#include <qstring.h>

#include "scribusdoc.h"
#include "pagestructs.h"
#include "pageitem.h"
#include "gtwriter.h"


TOCGenerator::TOCGenerator(QObject *parent, const char *name, ScribusDoc *doc) : QObject(parent, name)
{
	currDoc=doc;
}


TOCGenerator::~TOCGenerator()
{
}

void TOCGenerator::setDoc(ScribusDoc *doc)
{
	currDoc=doc;
}

void TOCGenerator::generateDefault()
{
	if (currDoc==NULL)
		return;
	for(ToCSetupVector::Iterator tocSetupIt = currDoc->docToCSetups.begin() ; tocSetupIt != currDoc->docToCSetups.end(); ++tocSetupIt )
	{
		bool found=false;
		uint d;
		PageItem* tocFrame=NULL;
		for (d = 0; d < currDoc->DocItems.count(), found==false; ++d)
		{
			if (currDoc->DocItems.at(d) !=NULL )
			{
				if (currDoc->DocItems.at(d)->itemType()==PageItem::TextFrame && currDoc->DocItems.at(d)->itemName()==(*tocSetupIt).frameName)
				{
					found=true;
					tocFrame=currDoc->DocItems.at(d);
				}
			}
		}
		if (found && tocFrame!=NULL)
		{
			PageItem *currentDocItem;
			QMap<QString, QString> tocMap;
			tocMap.clear();
			uint pageCounter[currDoc->pageCount];
			for (int i=0;i<=currDoc->pageCount;++i)
				pageCounter[i]=0;
			unsigned int maxDataWidth=0;
			for (uint d = 0; d < currDoc->DocItems.count(); ++d)
			{
				currentDocItem = currDoc->DocItems.at(d);
				if (currentDocItem!=NULL)
				{
					//Item not on a page, continue
					if (currentDocItem->OwnPage==-1)
						continue;
					ObjectAttribute objattr=currentDocItem->getObjectAttribute((*tocSetupIt).itemAttrName);
					if (objattr.name!=QString::null)
					{
						//TODO Handle docs with non consecutive page numbers when that is possible
						QString key=QString("%1,%2").arg(currentDocItem->OwnPage + currDoc->FirstPnum).arg(pageCounter[currentDocItem->OwnPage]++);
						tocMap.insert(key, objattr.value);
						if (objattr.value.length()>maxDataWidth)
							maxDataWidth=objattr.value.length();
					}
				}
			}
			gtWriter* writer = new gtWriter(false, tocFrame);
			QString oldTocPage=QString::null;
			for (QMap<QString, QString>::Iterator tocIt=tocMap.begin();tocIt!=tocMap.end();++tocIt)
			{
				QString tocPage = tocIt.key().section( ',', 0, 0 );
				QString tocLine = tocIt.data();
				//QString tocPageItemNo = tocIt.key().section( ',', 1, 1 );
				if (oldTocPage!=tocPage)
				{
					oldTocPage=tocPage;
					tocLine = tocLine.leftJustify(maxDataWidth+5, '.');
					tocLine = tocLine + tocPage + "\n";
				}
				else
					tocLine += "\n";
				writer->append(tocLine);
			}
			if (writer!=NULL)
				delete writer;
		}
	}
}
