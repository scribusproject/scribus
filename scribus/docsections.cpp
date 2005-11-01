#include "docsections.h"
#include "docsections.moc"

// This class implements only the non-GUI parts of the
// Document Sections dialog. Please use Qt Designer on
// ui/docsectionsbase.ui if you need to modify the layout,
// widget properties, etc.

#include <qtable.h>
#include <qpushbutton.h>

#include "pagestructs.h"
#include "scribusdoc.h"

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
}

void DocSections::setup(ScribusDoc *doc)
{
	currDoc=doc;
	localSections=currDoc->sections;
	maxpageindex=currDoc->DocPages.count()-1;
	
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
		QTableItem *item3 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, QString::number((*it).fromindex));
		sectionsTable->setItem(row, i++, item3);
		//ToIndex
		QTableItem *item4 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, QString::number((*it).toindex));
		sectionsTable->setItem(row, i++, item4);
		//Style
		QComboTableItem *item5 = new QComboTableItem(sectionsTable, styles);
		sectionsTable->setItem(row, i++, item5);
		item5->setCurrentItem((*it).type);
		//Start Page Number
		QTableItem *item6 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, QString::number((*it).sectionstartindex));
		sectionsTable->setItem(row, i++, item6);
		//End Page Number
		QTableItem *item7 = new QTableItem(sectionsTable, QTableItem::WhenCurrent, QString::number((*it).sectionstartindex + (*it).toindex - (*it).fromindex));
		item7->setEnabled(false);
		sectionsTable->setItem(row, i++, item7);
		
		sectionsTable->verticalHeader()->setLabel(row, QString("%1").arg(row));
		row++;
	}
	deleteButton->setEnabled(localSections.count()>1);
}

void DocSections::tableItemChanged( int row, int col )
{
	switch (col)
	{
	case 0:
		localSections[row].name=sectionsTable->text(row, col);
		break;
	case 1:
		localSections[row].active=static_cast<QCheckTableItem*>(sectionsTable->item(row, col))->isChecked();
		break;
	case 2:
		localSections[row].fromindex=sectionsTable->text(row, col).toUInt();
		break;
	case 3:
		localSections[row].toindex=sectionsTable->text(row, col).toUInt();
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
}

void DocSections::addEntry()
{
	struct DocumentSection blank;
	uint count=localSections.count();
	blank.number=count;
	blank.name=QString::number(count);
	blank.fromindex=maxpageindex;
	blank.toindex=maxpageindex;
	blank.type=Type_1_2_3;
	blank.sectionstartindex=1;
	blank.reversed=false;
	blank.active=true;
	localSections.insert(count, blank);
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
		localSections[newCount-1].toindex=maxpageindex;
		updateTable();
	}
}

const DocumentSectionMap& DocSections::getNewSections()
{
	return localSections;
}
