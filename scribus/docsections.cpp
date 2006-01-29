/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2005 by Craig Bradney                                   *
*   cbradney@zip.com.au                                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "docsections.h"
#include "docsections.moc"

// This class implements only the non-GUI parts of the
// Document Sections dialog. Please use Qt Designer on
// ui/docsectionsbase.ui if you need to modify the layout,
// widget properties, etc.

#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qtable.h>
#include <qtooltip.h>

#include "pagestructs.h"


DocSections::DocSections( QWidget* parent )
	: DocSectionsBase( parent, "DocSections" )
{
	languageChange();
	connect( sectionsTable, SIGNAL(valueChanged(int, int)), this, SLOT(tableItemChanged(int, int)));
	connect( addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
	connect( deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
}

DocSections::~DocSections()
{
}

void DocSections::languageChange()
{
	QToolTip::add(addButton, "<qt>"+ tr("Add a page numbering section to the document. The new section will be added after the currently selected section.") + "</qt>");
	QToolTip::add(deleteButton, "<qt>"+ tr("Delete the currently selected section.") + "</qt>");
	QToolTip::add(sectionsTable, "<qt>"+ tr("Name: Optional name for section eg. Index<br/>"
											"Shown: Select to show the page numbers in this section if there is one or more text frames setup to do so.<br/>"
											"From: The page index for this section to start at.<br/>"
											"To: The page index for this section to stop at.<br/>"
											"Style: Select the page number style to be used.<br/>"
											"Start: The index within the Style's range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b.") +"</qt>");
}

void DocSections::setup(const DocumentSectionMap docSections, int maxPageIndex)
{
	localSections=docSections;
	m_maxpageindex=maxPageIndex;
	
	styles << tr("1, 2, 3, ...") << tr("i, ii, iii, ...") << tr("I, II, III, ...") << tr("a, b, c, ...") << tr("A, B, C, ...");
	
	updateTable();
}

void DocSections::updateTable()
{
	sectionsTable->setNumRows(localSections.count());
	int row=0;
	for(DocumentSectionMap::Iterator it = localSections.begin(); it!= localSections.end(); ++it)
	{
		uint i=0;
		//Name
		QTableItem *item1 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, (*it).name);
		sectionsTable->setItem(row, i++, item1);
		//Active
		QCheckTableItem *item2 = new QCheckTableItem(sectionsTable,"");
		item2->setChecked((*it).active);
		sectionsTable->setItem(row, i++, item2);
		//FromIndex
		QTableItem *item3 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, QString::number((*it).fromindex+1));
		sectionsTable->setItem(row, i++, item3);
		//ToIndex
		QTableItem *item4 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, QString::number((*it).toindex+1));
		sectionsTable->setItem(row, i++, item4);
		//Style
		QComboTableItem *item5 = new QComboTableItem(sectionsTable, styles);
		sectionsTable->setItem(row, i++, item5);
		item5->setCurrentItem((*it).type);
		//Start Page Number
		QTableItem *item6 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, QString::number((*it).sectionstartindex));
		sectionsTable->setItem(row, i++, item6);
		//End Page Number
		/*
		QTableItem *item7 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, QString::number((*it).sectionstartindex + (*it).toindex - (*it).fromindex));
		item7->setEnabled(false);
		sectionsTable->setItem(row, i++, item7);
		*/
		sectionsTable->verticalHeader()->setLabel(row, QString("%1").arg(row));
		row++;
	}
	for (int i=0;i<6;++i)
		sectionsTable->adjustColumn(i);
	deleteButton->setEnabled(localSections.count()>1);
}

void DocSections::tableItemChanged( int row, int col )
{
	bool outOfRange=false;
	switch (col)
	{
	case 0:
		localSections[row].name=sectionsTable->text(row, col);
		break;
	case 1:
		localSections[row].active=static_cast<QCheckTableItem*>(sectionsTable->item(row, col))->isChecked();
		break;
	case 2:
		localSections[row].fromindex=sectionsTable->text(row, col).toUInt()-1;
		if (localSections[row].fromindex<1)
		{
			localSections[row].fromindex=1;
			outOfRange=true;
		}
		else
		if (localSections[row].fromindex>m_maxpageindex)
		{
			localSections[row].fromindex=m_maxpageindex;
			outOfRange=true;
		}
		break;
	case 3:
		localSections[row].toindex=sectionsTable->text(row, col).toUInt()-1;
		if (localSections[row].toindex<1)
		{
			localSections[row].toindex=1;
			outOfRange=true;
		}
		else
		if (localSections[row].toindex>m_maxpageindex)
		{
			localSections[row].toindex=m_maxpageindex;
			outOfRange=true;
		}
		break;
	case 4:
		{
			QComboTableItem* qcti=dynamic_cast<QComboTableItem*>(sectionsTable->item(row,col));
			if (qcti!=NULL)
			{
				uint index=qcti->currentItem();
				if (index<styles.count())
					localSections[row].type=(DocumentSectionType)index;
			}
		}
		break;
	case 5:
		localSections[row].sectionstartindex=sectionsTable->text(row, col).toUInt();
		break;
	default:
		break;
	}
	
	if (outOfRange)
	{
		updateTable();
		QMessageBox::warning(parentWidget(), tr("Page Number Out Of Bounds"),"<qt>"+tr("The value you have entered is outside the range of page numbers in the current document (%1-%2).").arg(1).arg(m_maxpageindex+1)+"</qt>",QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
	}
}

void DocSections::addEntry()
{
	int currRow=sectionsTable->currentRow();
	bool found=false;
	DocumentSectionMap::Iterator it = localSections.begin();
	int count=0;
	for(; it!= localSections.end(); ++it)
	{
		if(count==currRow)
		{
			found=true;
			break;
		}
		++count;
	}
	if (!found) //End of map, just append
	{
		struct DocumentSection blank;
		uint count=localSections.count();
		blank.number=count;
		blank.name=QString::number(count);
		blank.fromindex=m_maxpageindex+1;
		blank.toindex=m_maxpageindex+1;
		blank.type=Type_1_2_3;
		blank.sectionstartindex=1;
		blank.reversed=false;
		blank.active=true;
		localSections.insert(count, blank);
	}
	else
	{
		//Now, copy to a temp map
		DocumentSectionMap tempSections(localSections);
		localSections.clear();
		//Copy the temp map entries over. When we find the number of the current row, also insert a new entry.
		uint i=0;
		for(DocumentSectionMap::Iterator it2 = tempSections.begin(); it2!= tempSections.end(); ++it2)
		{
			it2.data().number=i;
			localSections.insert(i, it2.data());
			
			if ((*it).number==i)
			{
				struct DocumentSection blank;
				blank.number=++i;
				blank.name=QString::number(i);
				blank.fromindex=(*it).toindex+1+1;
				blank.toindex=(*it).toindex+2+1;
				blank.type=Type_1_2_3;
				blank.sectionstartindex=1;
				blank.reversed=false;
				blank.active=true;
				localSections.insert(i, blank);
			}
			++i;
		}
	}
	updateTable();
}

void DocSections::deleteEntry()
{
	int currRow=sectionsTable->currentRow();
	if (currRow==0 && localSections.count()==1)
		return;
	bool found=false;
	DocumentSectionMap::Iterator it = localSections.begin();
	int count=0;
	for(; it!= localSections.end(); ++it)
	{
		if(count==currRow)
		{
			found=true;
			break;
		}
		++count;
	}
	if (found)
	{
		//If we arent at the start, copy the toindex of the current item
		//to the toindex of the previous item
		if (it!=localSections.begin())
		{
			DocumentSectionMap::Iterator it2(it);
			(*--it2).toindex=(*it).toindex;
		}
		//Delete the currently selected entry
		localSections.remove(it);
		//Now, copy to a temp map and reinsert with consecutive keys again
		DocumentSectionMap tempSections(localSections);
		localSections.clear();
		uint i=0;
		it = tempSections.begin();
		for(; it!= tempSections.end(); ++it)
		{
			it.data().number=i;
			localSections.insert(i++, it.data());
		}
		int newCount=localSections.count();
		//int preIndex=QMAX(currentIndex-1, 0);
		localSections[0].fromindex=0;
		localSections[newCount-1].toindex=m_maxpageindex;
		updateTable();
	}
}

const DocumentSectionMap& DocSections::getNewSections()
{
	return localSections;
}
